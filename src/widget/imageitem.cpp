/**
 * @file imageitem.cpp
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
#include "imageitem_p.hpp"
#include "blockdeviceloader.hpp"
#include "characterdeviceloader.hpp"

#include <QtCore/QThreadPool>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QLabel>

using KomiX::widget::ImageItem;

ImageItem::Private::Private( ImageItem * owner ):
QObject(),
owner( owner ),
item( nullptr ) {
}

void ImageItem::Private::onFinished( int id, QMovie * movie ) {
	QGraphicsProxyWidget * item = new QGraphicsProxyWidget( this->owner );
	QLabel * label = new QLabel;
	movie->setParent( label );
	item->setWidget( label );
	this->item = item;
}

void ImageItem::Private::onFinished( int id, const QPixmap & pixmap ) {
	QGraphicsPixmapItem * item = new QGraphicsPixmapItem( pixmap, this->owner );
	item->setTransformationMode( Qt::SmoothTransformation );
	this->item = item;
}

ImageItem::ImageItem( const QList< QIODevice * > & devices ):
QGraphicsObject(),
p_( new Private( this ) ) {
	foreach( QIODevice * device, devices ) {
		DeviceLoader * loader = nullptr;
		if( device->isSequential() ) {
			loader = new CharacterDeviceLoader( -1, device );
		} else {
			loader = new BlockDeviceLoader( -1, device );
		}
		this->p_->connect( loader, SIGNAL( finished( int, QMovie * ) ), SLOT( onFinished( int, QMovie * ) ) );
		this->p_->connect( loader, SIGNAL( finished( int, const QPixmap & ) ), SLOT( onFinished( int, const QPixmap & ) ) );
		QThreadPool::globalInstance()->start( loader );
	}
}

QRectF ImageItem::boundingRect() const {
	if( this->p_->item ) {
		return this->p_->item->boundingRect();
	}
}

void ImageItem::paint( QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/ ) {
}
