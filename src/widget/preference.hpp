/**
 * @file preference.hpp
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
#ifndef KOMIX_WIDGET_PREFERENCE_HPP
#define KOMIX_WIDGET_PREFERENCE_HPP

#include <QtWidgets/QDialog>

#include <memory>

namespace KomiX {
namespace widget {

/**
 * @brief preference widget
 */
class Preference : public QDialog {
    Q_OBJECT
public:
    /// constructor
    explicit Preference(QWidget * parent);

public slots:
    /// Override from QDialog, won't hide dialog
    virtual void accept();
    /// Override from QDialog
    virtual void reject();

private:
    class Private;
    std::shared_ptr<Private> p_;
};
}
} // end of namespace

#endif
