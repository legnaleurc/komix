/**
 * @file navigator_p.hpp
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
#ifndef KOMIX_WIDGET_NAVIGATOR_P_HPP
#define KOMIX_WIDGET_NAVIGATOR_P_HPP

#include "navigator.hpp"
#include "ui_navigator.h"

namespace KomiX {
namespace widget {

class Navigator::Private: public QObject {
	Q_OBJECT
public:
	Private( Navigator * owner );

public slots:
	void openHelper();
	void viewImage( const QModelIndex &, const QModelIndex & );

signals:
	void required( const QModelIndex & index );

public:
	Navigator * owner;
	Ui::Navigator * ui;
	model::FileModelSP model;
	QItemSelectionModel * selection;
};

}
}

#endif
