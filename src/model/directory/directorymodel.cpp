/**
 * @file directorymodel.cpp
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
#include "directorymodel.hpp"


using namespace KomiX::model;


FileModel::SP DirectoryModel::create(const QUrl & url) {
    if (url.scheme() != "file") {
        return nullptr;
    }
    QFileInfo fi(url.toLocalFile());
    if (!fi.isDir()) {
        return nullptr;
    }
    return std::make_shared<DirectoryModel>(fi);
}


DirectoryModel::DirectoryModel(const QFileInfo & root)
    : LocalFileModel(root.absoluteFilePath())
{
}
