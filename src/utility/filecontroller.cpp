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
#include "error.hpp"
#include "filecontroller.hpp"
#include "global.hpp"

#include <QtCore/QFileInfo>
#include <QtGui/QPixmap>
#include <QtGui/QMovie>

#include <QtCore/QtDebug>

using namespace KomiX;

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
		this->fromIndex_( first );
		return true;
	}
}

void FileController::open( const QModelIndex & index ) {
	if( !isEmpty() ) {
		index_ = index.row();
		this->fromIndex_( index );
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
		this->fromIndex_( item );
	}
}

void FileController::prev() {
	if( !isEmpty() ) {
		--index_;
		if( index_ < 0 ) {
			index_ = model_->rowCount() - 1;
		}
		QModelIndex item = model_->index( index_, 0 );
		this->fromIndex_( item );
	}
}

bool FileController::isEmpty() const {
	if( !model_ ) {
		return true;
	}
	return model_->rowCount() == 0;
}

KomiX::model::FileModelSP FileController::getModel() const {
	return model_;
}

void FileController::fromIndex_( const QModelIndex & index ) {
	QString path = index.data( Qt::UserRole ).toString();
	if( path.endsWith( ".gif", Qt::CaseInsensitive ) ) {
		QMovie * anime = new QMovie( path, QByteArray(), this );
		emit imageLoaded( anime );
	} else {
		emit imageLoaded( QPixmap( path ) );
	}
}
