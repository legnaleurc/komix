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

#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>

#include <algorithm>

namespace {

static inline QMutex * lock() {
	static QMutex m;
	return &m;
}

} // end of namespace

using namespace KomiX::model;

FileModel::FunctorList & FileModel::getFunctorList_() {
	static FileModel::FunctorList fl;
	return fl;
}

FileModelSP FileModel::createModel( const QUrl & url ) {
	QMutexLocker locker( ::lock() );
	FunctorList::const_iterator it = find_if( getFunctorList_().begin(), getFunctorList_().end(), Matcher( url ) );
	if( it == getFunctorList_().end() ) {
		return QSharedPointer< FileModel >();
	} else {
		return it->second( url );
	}
}

bool FileModel::registerModel( const KeyFunctor & key, const ValueFunctor & value ) {
	QMutexLocker locker( ::lock() );
	getFunctorList_().push_back( std::make_pair( key, value ) );
	return true;
}

void FileModel::initialize() {
	this->doInitialize();
}

FileModel::Matcher::Matcher( const QUrl & url ) : url_( url ) {
}

bool FileModel::Matcher::operator ()( const FileModel::FunctorPair & that ) const {
	return that.first( url_ );
}
