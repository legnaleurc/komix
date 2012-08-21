/**
 * @file deviceloader.cpp
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
#include "deviceloader.hpp"

#include <QtGui/QImageReader>

namespace KomiX {

class DeviceLoader::Private {
public:
	Private( int id, QIODevice * device );

	int id;
	QIODevice * device;
};

}

using KomiX::DeviceLoader;

DeviceLoader::Private::Private( int id, QIODevice * device ):
id( id ),
device( device ) {
}

DeviceLoader::DeviceLoader( int id, QIODevice * device ):
QObject(),
QRunnable(),
p_( new Private( id, device ) ) {
}

int DeviceLoader::getID() const {
	return this->p_->id;
}

QIODevice * DeviceLoader::getDevice() const {
	return this->p_->device;
}
