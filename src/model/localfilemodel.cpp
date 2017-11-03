/**
 * @file localfilemodel.cpp
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
#include "localfilemodel.hpp"

#include <QtCore/QDirIterator>
#include <QtCore/QRegularExpression>
#include <QtCore/QtDebug>

#include <tuple>

#include "global.hpp"


namespace KomiX {
namespace model {

class LocalFileModel::Private {
public:
    Private();

    QDir root;
    QStringList files;
};


QVariantList parseName(const QString & name) {
    QVariantList parts;
    QRegularExpression pattern("\\d+|\\D+");
    auto match = pattern.match(name, 0, QRegularExpression::PartialPreferFirstMatch);
    while (match.hasPartialMatch() || match.hasMatch()) {
        auto part = match.captured(0);
        bool ok = false;
        int tmp = part.toInt(&ok, 10);
        if (ok) {
            parts.push_back(tmp);
        } else {
            parts.push_back(part);
        }
        match = pattern.match(name, match.capturedEnd(0), QRegularExpression::PartialPreferFirstMatch);
    }
    return parts;
}


typedef std::tuple<QVariantList, QString> Part;


std::vector<Part> parseNameParts(const QStringList & names) {
    std::vector<std::tuple<QVariantList, QString>> parts;
    for (const auto & name : names) {
        auto pivot = parseName(name);
        parts.push_back(std::make_tuple(pivot, name));
    }
    return parts;
}


QStringList smartSort(const QStringList & names) {
    auto partList = parseNameParts(names);
    std::stable_sort(std::begin(partList), std::end(partList), [](const Part & left, const Part & right) -> bool {
        const auto & lp = std::get<0>(left);
        const auto & rp = std::get<0>(right);
        auto length = std::min(lp.size(), rp.size());
        for (int i = 0; i < length; ++i) {
            const auto & l = lp.at(i);
            const auto & r = rp.at(i);
            auto lt = l.type();
            auto rt = r.type();
            if (lt == rt) {
                if (l == r) {
                    continue;
                }
                return l < r;
            } else {
                return lt == QVariant::String;
            }
        }
        return lp.size() < rp.size();
    });
    QStringList newNames;
    for (const auto & part : partList) {
        newNames.push_back(std::get<1>(part));
    }
    return newNames;
}


}
}


using KomiX::model::LocalFileModel;


LocalFileModel::LocalFileModel()
    : FileModel()
    , p_(new Private)
{
}


void LocalFileModel::doInitialize() {
    emit this->ready();
}


void LocalFileModel::setRoot(const QDir & root) {
    this->p_->root = root;

    QDirIterator it(root.path(), SupportedFormatsFilter(), QDir::Files,
                    QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
    QStringList files;
    while (it.hasNext()) {
        auto path = it.next();
        path = root.relativeFilePath(path);
        files.push_back(path);
    }
    this->p_->files = smartSort(files);
}


QModelIndex LocalFileModel::index(const QUrl & url) const {
    int row = this->p_->files.indexOf(QFileInfo(url.toLocalFile()).fileName());
    if (row < 0) {
        return QModelIndex();
    }
    return createIndex(row, 0, row);
}


QModelIndex LocalFileModel::index(int row, int column, const QModelIndex & parent) const {
    if (!parent.isValid()) {
        // query from root
        if (column == 0 && row >= 0 && row < this->p_->files.size()) {
            return createIndex(row, 0, row);
        } else {
            return QModelIndex();
        }
    } else {
        // other node has no child
        return QModelIndex();
    }
}


QModelIndex LocalFileModel::parent(const QModelIndex & child) const {
    if (!child.isValid()) {
        // root has no parent
        return QModelIndex();
    } else {
        if (child.column() == 0 && child.row() >= 0 && child.row() < this->p_->files.size()) {
            return QModelIndex();
        } else {
            return QModelIndex();
        }
    }
}


int LocalFileModel::rowCount(const QModelIndex & parent) const {
    if (!parent.isValid()) {
        // root row size
        return this->p_->files.size();
    } else {
        // others are leaf
        return 0;
    }
}


int LocalFileModel::columnCount(const QModelIndex & /*parent*/) const {
    return 1;
}


QVariant LocalFileModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    switch (index.column()) {
        case 0:
            if (index.row() >= 0 && index.row() < this->p_->files.size()) {
                const auto & relativePath = this->p_->files[index.row()];
                switch (role) {
                    case Qt::DisplayRole:
                        return relativePath;
                    case Qt::UserRole: {
                        auto path = this->p_->root.filePath(relativePath);
                        DeviceCreator dc = [path]() -> std::shared_ptr<QIODevice> {
                            return std::make_shared<QFile>(path);
                        };
                        return QVariant::fromValue(dc);
                    }
                    default:
                        return QVariant();
                }
            } else {
                return QVariant();
            }
        default:
            return QVariant();
    }
}


LocalFileModel::Private::Private()
    : root("__KOMIX_INVALID_ROOT__")
    , files()
{
}
