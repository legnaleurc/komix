/**
 * @file archivemodel.hpp
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
#ifndef KOMIX_MODEL_ARCHIVEMODEL_HPP
#define KOMIX_MODEL_ARCHIVEMODEL_HPP

#include "localfilemodel.hpp"


namespace KomiX {
namespace model {

/**
 * @brief The model using 7-Zip to open compressed file
 *
 * Supported file formats: 7z, zip, rar, tar.gz, tar.bz2.
 * Do not support password.
 */
class ArchiveModel : public LocalFileModel {
public:
    static SP create(const QUrl & url);
    static void registerDialogFilter();

    /**
     * @brief Constructor with given fileinfo
     * @param root top-level file
     */
    explicit ArchiveModel(const QFileInfo & root);

protected:
    virtual void doInitialize() override;

private:
    class Private;
    std::shared_ptr<Private> p_;
};

}
} // end of namespace

#endif
