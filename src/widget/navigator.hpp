/**
 * @file navigator.hpp
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
#ifndef KOMIX_WIDGET_NAVIGATOR_HPP
#define KOMIX_WIDGET_NAVIGATOR_HPP

#include "filemodel.hpp"

#include <QtWidgets/QDialog>

#include <memory>

namespace KomiX {

class FileController;

namespace widget {

/**
 * @brief Preview and goto widget
 *
 * This widget can preview other images in same dicrectory, and
 * open which you want.
 */
class Navigator : public QDialog {
public:
    /**
     * @brief default constructor
     * @param parent parent widget
     */
    Navigator(FileController * controller, QWidget * parent);

    /// set current using model
    void setModel(std::shared_ptr<model::FileModel> model);
    /// set current model index
    void setCurrentIndex(const QModelIndex & index);

    virtual void setVisible(bool visible);

private:
    class Private;
    std::shared_ptr<Private> p_;
};
}
} // end namespace

#endif
