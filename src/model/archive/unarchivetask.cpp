/**
 * @file unarchivetask.cpp
 * @author Wei-Cheng Pan
 *
 * KomiX, a comics viewer.
 * Copyright (C) 2008  Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "unarchivetask_p.hpp"

#include <QtCore/QtDebug>

#include <archive.h>
#include <archive_entry.h>
#include <uchardet.h>

#ifdef Q_OS_UNIX
#include <locale.h>
#endif

namespace {

QString createErrorMessage(const std::shared_ptr<archive> & handle,
                           const QString & fn, int rv) {
    QString tpl("%1: %2 (%3)");
    return tpl.arg(fn).arg(archive_error_string(handle.get())).arg(rv);
}


QString extractArchive(std::shared_ptr<archive> src,
                       std::shared_ptr<archive> dst) {
    for (;;) {
        int rv = 0;
        const void * chunk = NULL;
        size_t length = 0;
        int64_t offset = 0;

        rv = archive_read_data_block(src.get(), &chunk, &length, &offset);
        if (rv == ARCHIVE_EOF) {
            return "";
        }
        if (rv != ARCHIVE_OK) {
            return createErrorMessage(src, "archive_read_data_block", rv);
        }

        rv = archive_write_data_block(dst.get(), chunk, length, offset);
        if (rv != ARCHIVE_OK) {
            return createErrorMessage(dst, "archive_write_data_block", rv);
        }
    }

    return "";
}


std::shared_ptr<archive> openArchiveForReading(const QString & archivePath) {
    std::shared_ptr<archive> src(archive_read_new(), archive_read_free);
    if (!src) {
        qWarning() << "out of memory";
        return nullptr;
    }

    int rv = 0;
    rv = archive_read_support_format_all(src.get());
    if (rv != ARCHIVE_OK) {
        qWarning() << createErrorMessage(src, "archive_read_support_format_all", rv);
        return nullptr;
    }
    rv = archive_read_support_filter_all(src.get());
    if (rv != ARCHIVE_OK) {
        qWarning() << createErrorMessage(src, "archive_read_support_filter_all", rv);
        return nullptr;
    }

    rv = archive_read_open_filename(src.get(), archivePath.toUtf8(), 64 * 1024);
    if (rv != ARCHIVE_OK) {
        qWarning() << createErrorMessage(src, "archive_read_open_filename", rv);
        return nullptr;
    }

    return src;
}


QString getEntryName(archive_entry * entry, QTextCodec * codec) {
    const char * rawEntryName = archive_entry_pathname_utf8(entry);
    if (rawEntryName) {
        return QString::fromUtf8(rawEntryName);
    }
    rawEntryName = archive_entry_pathname(entry);
    if (!rawEntryName) {
        return QString();
    }
    return codec->toUnicode(rawEntryName);
}

}

using namespace KomiX::model;


UnarchiveTask::UnarchiveTask(const QString & archivePath,
                             const QString & outputPath)
    : QRunnable()
    , p_(new Private(this, archivePath, outputPath))
{
}


void UnarchiveTask::run() {
    // See libarchive/libarchive#587 on GitHub
#ifdef Q_OS_UNIX
    setlocale(LC_ALL, "");
#endif

    auto src = openArchiveForReading(this->p_->archivePath);
    if (!src) {
        emit this->finished(false, "out of memory");
        return;
    }

    std::shared_ptr<archive> dst(archive_write_disk_new(), archive_write_free);
    if (!dst) {
        emit this->finished(false, "out of memory");
        return;
    }

    this->p_->prepareArchive();
    int currentEntryCount = 0;
    emit this->progressUpdated(currentEntryCount, this->p_->fileCount);

    while (true) {
        archive_entry * entry = nullptr;
        int rv = archive_read_next_header(src.get(), &entry);

        if (rv == ARCHIVE_EOF) {
            break;
        }

        if (rv != ARCHIVE_OK) {
            auto msg = createErrorMessage(src, "archive_read_next_header", rv);
            emit this->finished(false, msg);
            return;
        }

        auto entryName = getEntryName(entry, this->p_->codec);
        if (entryName.isEmpty()) {
            emit this->finished(false, "read entry path name failed");
            return;
        }

        auto newPath = this->p_->getOutputEntryPath(entryName);
        rv = archive_entry_update_pathname_utf8(entry, newPath.toUtf8());
        if (!rv) {
            emit this->finished(false, "cannot update entry path");
            return;
        }

        rv = archive_write_header(dst.get(), entry);
        if (rv != ARCHIVE_OK) {
            auto msg = createErrorMessage(dst, "archive_write_header", rv);
            emit this->finished(false, msg);
            return;
        }

        auto msg = extractArchive(src, dst);
        if (!msg.isEmpty()) {
            emit this->finished(false, msg);
            return;
        }

        rv = archive_write_finish_entry(dst.get());
        if (rv != ARCHIVE_OK) {
            auto msg = createErrorMessage(dst, "archive_write_finish_entry", rv);
            emit this->finished(false, msg);
            return;
        }

        ++currentEntryCount;
        emit this->progressUpdated(currentEntryCount, this->p_->fileCount);
    }

    emit this->finished(true, "");
}


UnarchiveTask::Private::Private(UnarchiveTask * owner,
                                const QString & archivePath,
                                const QString & outputPath)
    : owner(owner)
    , archivePath(archivePath)
    , outputDir(outputPath)
    , fileCount(0)
    , codec(nullptr)
{
    if (!outputDir.exists()) {
        outputDir.mkpath(".");
    }
}


QString UnarchiveTask::Private::getOutputEntryPath(const QString & pathName) const {
    return this->outputDir.absoluteFilePath(pathName);
}


// Unfortunately we need to process the archive first to
// * count files in the archive
// * find out text encoding
void UnarchiveTask::Private::prepareArchive() {
    auto src = openArchiveForReading(archivePath);
    if (!src) {
        return;
    }
    using CDSP = std::shared_ptr<std::remove_pointer<uchardet_t>::type>;
    CDSP ucd(uchardet_new(), uchardet_delete);
    if (!ucd) {
        return;
    }

    for (int i = 0; ; ++i) {
        emit this->owner->progressUpdated(i, 0);

        archive_entry * entry = nullptr;
        int rv = archive_read_next_header(src.get(), &entry);
        if (rv == ARCHIVE_EOF) {
            uchardet_data_end(ucd.get());
            const char * name = uchardet_get_charset(ucd.get());
            if (!name) {
                return;
            }
            this->codec = QTextCodec::codecForName(name);
            this->fileCount = i;
            return;
        }
        if (rv != ARCHIVE_OK) {
            return;
        }

        const char * entryName = archive_entry_pathname(entry);
        if (!entryName) {
            return;
        }
        rv = uchardet_handle_data(ucd.get(), entryName, strlen(entryName));
        if (rv != 0) {
            return;
        }
    }
}
