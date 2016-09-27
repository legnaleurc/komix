/**
 * @file filemodel.cpp
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
#include "filemodel.hpp"

#include <list>


namespace {

typedef std::list<KomiX::model::FileModel::ModelCreator> FunctorList;

FunctorList & getFunctorList() {
    static FunctorList fl;
    return fl;
}

} // end of namespace


using KomiX::model::FileModel;


FileModel::SP FileModel::createModel(const QUrl & url) {
    const auto & creators = getFunctorList();
    for (const auto & create : creators) {
        auto model = create(url);
        if (model) {
            return model;
        }
    }
    return nullptr;
}


void FileModel::registerModel(const ModelCreator & creator) {
    getFunctorList().push_back(creator);
}


void FileModel::initialize() {
    this->doInitialize();
}
