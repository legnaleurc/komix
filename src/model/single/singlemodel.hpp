/**
 * @file singlemodel.hpp
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
#ifndef KOMIX_MODEL_SINGLEMODEL_HPP
#define KOMIX_MODEL_SINGLEMODEL_HPP

#include "localfilemodel.hpp"


namespace KomiX {
namespace model {

/**
 * @brief The model to open single file
 */
class SingleModel : public LocalFileModel {
public:
    static SP create(const QUrl & url);
    static QString createDialogFilter();

    /// Constructor open @p root as image
    explicit SingleModel(const QFileInfo & root);
};

}
} //end of namespace

#endif
