/**
 * @file archivemodel.cpp
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

#include "archivemodel_p.hpp"

#include <QtCore/QCryptographicHash>
#include <QtCore/QThreadPool>

#include <QtCore/QtDebug>

#include "exception.hpp"
#include "global.hpp"
#include "literal.hpp"
#include "unarchivetask.hpp"


namespace KomiX {

/// Private archive error class
class ArchiveException : public Exception {
public:
    ArchiveException(const char * msg)
        : Exception(msg) {
    }
    ArchiveException(const QString & msg)
        : Exception(msg) {
    }
};

} // end of namespace

namespace {

inline bool isPrepared() {
    const auto & global = KomiX::Global::instance();
    return global.getTemporaryDirectory().exists();
}


inline const QStringList & archiveList2() {
    static QStringList a2 = {
        "tar.gz",
        "tgz",
        "tar.bz2",
        "tbz2",
        "tar.lzma",
    };
    return a2;
}


inline QStringList archiveList() {
    QStringList a(archiveList2());
    a << "7z";
    a << "rar";
    a << "zip";
    a << "tar";
    return a;
}


const QStringList & archiveFormats() {
    static QStringList af = archiveList();
    return af;
}


bool isArchiveSupported(const QString & name) {
    for(const auto & ext : archiveFormats()) {
        if (name.endsWith(ext)) {
            return true;
        }
    }
    return false;
}


QDir archiveDir(const QString & dirName) {
    const auto & global = KomiX::Global::instance();
    auto tmp = global.getTemporaryDirectory();
    if (!tmp.exists(dirName)) {
        tmp.mkdir(dirName);
    }
    tmp.cd(dirName);
    return tmp;
}

} // end of namespace


using namespace KomiX::model;


FileModel::SP ArchiveModel::create(const QUrl & url) {
    if (url.scheme() != "file") {
        return nullptr;
    }
    QFileInfo fi(url.toLocalFile());
    if (fi.isDir()) {
        return nullptr;
    }
    if (!isArchiveSupported(fi.fileName().toLower())) {
        return nullptr;
    }
    if (!isPrepared()) {
        throw ArchiveException("Temporary directory does not exist.");
    }
    return std::make_shared<ArchiveModel>(fi);
}


QString ArchiveModel::createDialogFilter() {
    QStringList filter = toNameFilter(archiveFormats());
    QString tpl("%1 ( %2 )");
    return tpl.arg(OPEN_DIALOG_ARCHIVE_FORMATS).arg(filter.join(" "));
}


ArchiveModel::ArchiveModel(const QFileInfo & root)
    : LocalFileModel()
    , p_(new Private(this, root))
{
    this->connect(this->p_.get(),
                  SIGNAL(error(const QString &)),
                  SIGNAL(error(const QString &)));
    this->connect(this->p_.get(), SIGNAL(ready()), SIGNAL(ready()));
}

void ArchiveModel::doInitialize() {
    auto fileName = this->p_->root.fileName().toUtf8();
    auto sha1 = QCryptographicHash::hash(fileName, QCryptographicHash::Sha1);
    this->p_->hash = QString::fromUtf8(sha1.toHex());

    const auto & global = Global::instance();
    const auto & tmp = global.getTemporaryDirectory();
    if (tmp.exists(this->p_->hash)) {
        // uncompressed before
        this->setRoot(archiveDir(this->p_->hash));
        this->LocalFileModel::doInitialize();
        return;
    }

    // HACK MacOSX and Linux use different Unicode Normalization Forms, so they
    // cannot smoothly access each other via file sharing.
    // Create a symbolic link without CJK characters to workaround this.
    auto origPath = this->p_->root.absoluteFilePath();
    auto ext = this->p_->root.completeSuffix();
    auto linkPath = QString("%1.%2").arg(this->p_->hash).arg(ext);
    linkPath = tmp.absoluteFilePath(linkPath);
    QFile::link(origPath, linkPath);

    this->p_->extract(linkPath);
}


ArchiveModel::Private::Private(ArchiveModel * owner, const QFileInfo & root)
    : QObject()
    , owner(owner)
    , root(root)
    , hash() {
}


void ArchiveModel::Private::extract(const QString & aFilePath) {
    auto task = new UnarchiveTask(aFilePath, archiveDir(this->hash).absolutePath());
    this->connect(task,
                  SIGNAL(finished(bool, const QString &)),
                  SLOT(onFinished(bool, const QString &)));
    this->owner->connect(task,
                         SIGNAL(progressUpdated(int, int)),
                         SIGNAL(progressUpdated(int, int)));
    QThreadPool::globalInstance()->start(task);
}


void ArchiveModel::Private::onFinished(bool ok, const QString & message) {
    if (ok) {
        // update the model root
        this->owner->setRoot(archiveDir(this->hash));
        this->owner->LocalFileModel::doInitialize();
    } else {
        emit this->error(message);
    }
}
