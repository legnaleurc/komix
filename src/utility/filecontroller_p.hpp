/**
 * @file filecontroller_p.hpp
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
#ifndef KOMIX_UTILITY_FILECONTROLLER_HPP_
#define KOMIX_UTILITY_FILECONTROLLER_HPP_

#include "filecontroller.hpp"

namespace KomiX {

class FileController::Private: public QObject {
	Q_OBJECT
public:
	explicit Private( FileController * owner );

	void fromIndex( const QModelIndex & );

public slots:
	void onModelReady();

signals:
	void imageLoaded( const KomiX::Image & image );

public:
	FileController * owner;
	int index;
	QUrl openingURL;
	model::FileModelSP model;
};

}

#endif
