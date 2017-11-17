/**
 * @file global.cpp
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
#include "global_p.hpp"

#include "filecontroller.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtGui/QImageReader>

#include <QtCore/QtDebug>

#include <algorithm>
#include <cassert>


namespace {

inline QStringList uniqueList() {
    assert(QCoreApplication::instance() || !"application is null");

    auto formatList = QImageReader::supportedImageFormats();
    for (auto & s : formatList) {
        s = s.toLower();
    }
    std::sort(std::begin(formatList), std::end(formatList));
    auto it = std::unique(std::begin(formatList), std::end(formatList));
    formatList.erase(it, std::end(formatList));

    QStringList result;
    for (const auto & s : formatList) {
        result.push_back(QString::fromUtf8(s));
    }
    return result;
}


QDir createTmpDir() {
    qsrand(qApp->applicationPid());
    QString tmpPath(QString("komix_%1").arg(qrand()));
    QDir tmpDir(QDir::temp());
    if (!tmpDir.mkdir(tmpPath)) {
        qWarning("can not make temp dir");
        // tmpDir will remain to tmp dir
    } else {
        tmpDir.cd(tmpPath);
    }
    return tmpDir;
}


int delTree(const QDir & dir) {
    int sum = 0;
    auto flags = QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs;
    QFileInfoList entry = dir.entryInfoList(flags);
    for(const QFileInfo & e : entry) {
        if (e.isDir()) {
            sum += delTree(e.absoluteFilePath());
        } else {
            if (QFile::remove(e.absoluteFilePath())) {
                ++sum;
            }
        }
    }
    dir.rmdir(dir.absolutePath());
    return sum + 1;
}

}

namespace KomiX {

QStringList toNameFilter(const QStringList & exts) {
    QStringList tmp;
    foreach (QString str, exts) {
        tmp << str.prepend("*.");
    }
    return tmp;
}

}


using KomiX::Global;
using KomiX::FileController;
using KomiX::FilterBuilder;


Global & Global::instance() {
    // This is thread-safe since C++11.
    static Global self;
    return self;
}


Global::Global()
    : QObject()
    , p_(new Private(this))
{
}


Global::~Global() {
}


FileController & Global::getFileController() const {
    return *this->p_->fileController;
}


const QDir & Global::getTemporaryDirectory() const {
    return this->p_->tmp;
}


FilterBuilder Global::createDialogFilterBuilder() const {
    return FilterBuilder(this->p_);
}


const QString & Global::getDialogFilter() const {
    if (this->p_->filterIsDirty) {
        this->p_->dialogFilter = this->p_->createFilterString();
        this->p_->filterIsDirty = false;
    }
    return this->p_->dialogFilter;
}


const QStringList & Global::getSupportedFormats() const {
    static QStringList sf = uniqueList();
    return sf;
}


const QStringList & Global::getSupportedFormatsFilter() const {
    static QStringList sff = toNameFilter(this->getSupportedFormats());
    return sff;
}


Global::Private::Private(Global * parent)
    : QObject(parent)
    , tmp(createTmpDir())
    , fileController(new FileController(this))
    , supportedFormats(uniqueList())
    , supportedFormatsFilter(toNameFilter(this->supportedFormats))
    , filters()
    , filterIsDirty(false)
    , dialogFilter()
{
}


Global::Private::~Private() {
    delTree(this->tmp);
}


void Global::Private::addFilters(const QString & summary,
                                 const QMultiMap<QString, QString> & filters) {
    this->filterIsDirty = true;
    this->filters.insert(summary, filters);
}


QString Global::Private::createFilterString() const {
    QStringList filter;
    QStringList globs;
    QString tpl("%1(%2)");
    QMapIterator<QString, QMultiMap<QString, QString>> oit{this->filters};
    while (oit.hasNext()) {
        oit.next();
        const auto & summary = oit.key();
        const auto & filters = oit.value();
        QStringList partialGlobs;
        for (const auto & name : filters.uniqueKeys()) {
            QStringList ext = filters.values(name);
            std::transform(std::begin(ext), std::end(ext), std::begin(ext), [](const QString & s) -> QString {
                return "*." + s;
            });
            partialGlobs += ext;
            filter.push_front(tpl.arg(name).arg(ext.join(" ")));
        }
        globs += partialGlobs;
        filter.push_front(tpl.arg(summary).arg(partialGlobs.join(" ")));
    }
    filter.push_front(tpl.arg("All Supported Files").arg(globs.join(" ")));
    return filter.join(";;");
}


class FilterBuilder::Private {
public:
    explicit Private(Global::Private * p);

    Global::Private * p;
    QString summary;
    QMultiMap<QString, QString> filters;
};


FilterBuilder::FilterBuilder(Global::Private * p)
    : p_(new Private(p))
{}


FilterBuilder::~FilterBuilder() {
    if (this->p_->summary.isEmpty()) {
        return;
    }
    this->p_->p->addFilters(this->p_->summary, this->p_->filters);
}


void FilterBuilder::setSummary(const QString & name) {
    this->p_->summary = name;
}


void FilterBuilder::addFilter(const QString & name, const QString & suffix) {
    this->p_->filters.insertMulti(name, suffix);
}


FilterBuilder::Private::Private(Global::Private * p)
    : p(p)
    , summary()
    , filters()
{}
