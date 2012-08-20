/**
 * @file navigator.cpp
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
#include "navigator_p.hpp"
#include "filecontroller.hpp"
#include "imagewrapper.hpp"
#include "blockdeviceloader.hpp"
#include "characterdeviceloader.hpp"

#include <QtCore/QThreadPool>
#include <QtCore/QBuffer>
#include <QtGui/QMovie>

using KomiX::widget::Navigator;
using KomiX::FileController;

Navigator::Private::Private( FileController * controller, Navigator * owner ):
QObject(),
owner( owner ),
ui(),
controller( controller ),
model(),
selection( NULL ) {
}

void Navigator::Private::openHelper() {
	this->controller->open( this->ui.list->currentIndex() );
	this->owner->accept();
}

void Navigator::Private::viewImage( const QModelIndex & current, const QModelIndex & /* previous */ ) {
	QIODevice * device = current.data( Qt::UserRole ).value< QIODevice * >();
	DeviceLoader * loader = nullptr;
	if( device->isSequential() ) {
		loader = new CharacterDeviceLoader( -1, device );
	} else {
		loader = new BlockDeviceLoader( -1, device );
	}
	this->connect( loader, SIGNAL( finished( int, const QByteArray & ) ), SLOT( onFinished( int, const QByteArray & ) ) );
	QThreadPool::globalInstance()->start( loader );
}

void Navigator::Private::onFinished( int id, const QByteArray & data ) {
	QBuffer * buffer = new QBuffer;
	buffer->setData( data );
	buffer->open( QIODevice::ReadOnly );
	QImageReader iin( buffer );
	QMovie * tmp = this->ui.preview->movie();
	if( iin.supportsAnimation() ) {
		// QMovie
		buffer->seek( 0 );
		QMovie * movie = new QMovie( buffer );
		buffer->setParent( movie );
		this->ui.preview->setMovie( movie );
		movie->setScaledSize( this->ui.preview->size() );
		movie->start();
	} else {
		QPixmap pixmap = QPixmap::fromImageReader( &iin );
		buffer->deleteLater();
		this->ui.preview->setPixmap( pixmap.scaled( this->ui.preview->size(), Qt::KeepAspectRatio ) );
	}
	if( tmp ) {
		tmp->deleteLater();
	}
}

Navigator::Navigator( FileController * controller, QWidget * parent ) :
QDialog( parent ),
p_( new Private( controller, this ) ) {
	this->p_->ui.setupUi( this );
	this->p_->ui.list->setFixedSize( 160, 480 );
	this->p_->ui.preview->setFixedSize( 480, 480 );
	this->p_->ui.preview->setAlignment( Qt::AlignCenter );

	this->connect( this->p_->ui.buttons, SIGNAL( rejected() ), SLOT( reject() ) );
	this->p_->connect( this->p_->ui.buttons, SIGNAL( accepted() ), SLOT( openHelper() ) );
}

void Navigator::setModel( std::shared_ptr< KomiX::model::FileModel > model ) {
	if( this->p_->selection ) {
		this->p_->selection->disconnect( SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( viewImage( const QModelIndex &, const QModelIndex & ) ) );
	}
	this->p_->model = model;
	this->p_->ui.list->setModel( this->p_->model.get() );
	this->p_->selection = this->p_->ui.list->selectionModel();
	this->p_->connect( this->p_->selection, SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), SLOT( viewImage( const QModelIndex &, const QModelIndex & ) ) );
}

void Navigator::setCurrentIndex( const QModelIndex & index ) {
	this->p_->ui.list->setCurrentIndex( index );
}
