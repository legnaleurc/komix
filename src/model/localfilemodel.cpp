/**
 * @file localfilemodel.cpp
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
#include "global.hpp"
#include "localfilemodel.hpp"

#include <QtGui/QPixmap>

using namespace KomiX::model;

LocalFileModel::LocalFileModel( const QDir & root ):
root_( root ),
files_( root_.entryList( SupportedFormatsFilter(), QDir::Files ) ) {
}

void LocalFileModel::setRoot( const QDir & root ) {
	root_ = root;
	files_ = root_.entryList( SupportedFormatsFilter(), QDir::Files );
}

QModelIndex LocalFileModel::index( const QUrl & url ) const {
	int row = files_.indexOf( QFileInfo( url.toLocalFile() ).fileName() );
	return ( row < 0 ) ? QModelIndex() : createIndex( row, 0, row );
}


QModelIndex LocalFileModel::index( int row, int column, const QModelIndex & parent ) const {
	if( !parent.isValid() ) {
		// query from root
		if( column == 0 && row >= 0 && row < files_.size() ) {
			return createIndex( row, 0, row );
		} else {
			return QModelIndex();
		}
	} else {
		// other node has no child
		return QModelIndex();
	}
}

QModelIndex LocalFileModel::parent( const QModelIndex & child ) const {
	if( !child.isValid() ) {
		// root has no parent
		return QModelIndex();
	} else {
		if( child.column() == 0 && child.row() >= 0 && child.row() < files_.size() ) {
			return QModelIndex();
		} else {
			return QModelIndex();
		}
	}
}

int LocalFileModel::rowCount( const QModelIndex & parent ) const {
	if( !parent.isValid() ) {
		// root row size
		return files_.size();
	} else {
		// others are leaf
		return 0;
	}
}

int LocalFileModel::columnCount( const QModelIndex & /*parent*/ ) const {
	return 1;
}

QVariant LocalFileModel::data( const QModelIndex & index, int role ) const {
	if( !index.isValid() ) {
		return QVariant();
	}
	switch( index.column() ) {
	case 0:
		if( index.row() >= 0 && index.row() < files_.size() ) {
			switch( role ) {
			case Qt::DisplayRole:
				return files_[index.row()];
			case Qt::UserRole:
				return QString( root_.filePath( files_[index.row()] ) );
			default:
				return QVariant();
			}
		} else {
			return QVariant();
		}
	default:
		return QVariant();
	}
}
