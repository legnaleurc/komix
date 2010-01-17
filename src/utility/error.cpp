/**
 * @file error.cpp
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
#include "error.hpp"

using namespace KomiX::error;

BasicError::BasicError( const char * msg ) : msg_( QString::fromUtf8( msg ) ) {}

BasicError::BasicError( const QString & msg ) : msg_( msg ) {}

BasicError::~BasicError() throw() {}

const char * BasicError::what() const throw() {
	return msg_.toLocal8Bit().constData();
}

const QString & BasicError::getMessage() const {
	return msg_;
}
