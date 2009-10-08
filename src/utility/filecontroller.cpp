/**
 * @file filecontroller.cpp
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
#include "filecontroller.hpp"
#include "global.hpp"
#include "error.hpp"

#include <QFileInfo>

namespace KomiX {// namespace private_ {

using model::FileModel;

FileController::FileController( QObject * parent ) :
QObject( parent ),
index_( 0 ),
model_( NULL ) {
}

bool FileController::open( const QUrl & url ) {
	try {
		model_ = FileModel::createModel( url );
	} catch( error::BasicError & e ) {
		emit errorOccured( e.getMessage() );
		return false;
	}
	if( isEmpty() ) {
		return false;
	} else {
		QModelIndex first = model_->index( url );
		if( first.isValid() ) {
			index_ = first.row();
		} else {
			first = model_->index( 0, 0 );
			index_ = 0;
		}
		emit imageLoaded( first.data( Qt::UserRole ).value< QPixmap >() );
		return true;
	}
}

void FileController::open( const QModelIndex & index ) {
	if( !isEmpty() ) {
		index_ = index.row();
		emit imageLoaded( index.data( Qt::UserRole ).value< QPixmap >() );
	}
}

QModelIndex FileController::getCurrentIndex() const {
	if( !isEmpty() ) {
		return model_->index( index_, 0 );
	} else {
		return QModelIndex();
	}
}

void FileController::next() {
	if( !isEmpty() ) {
		++index_;
		if( index_ >= model_->rowCount() ) {
			index_ = 0;
		}
		QModelIndex item = model_->index( index_, 0 );
		emit imageLoaded( item.data( Qt::UserRole ).value< QPixmap >() );
	}
}

void FileController::prev() {
	if( !isEmpty() ) {
		--index_;
		if( index_ < 0 ) {
			index_ = model_->rowCount() - 1;
		}
		QModelIndex item = model_->index( index_, 0 );
		emit imageLoaded( item.data( Qt::UserRole ).value< QPixmap >() );
	}
}

bool FileController::isEmpty() const {
	if( !model_ ) {
		return true;
	}
	return model_->rowCount() == 0;
}

QSharedPointer< FileModel > FileController::getModel() const {
	return model_;
}

}// } // end of namespace
