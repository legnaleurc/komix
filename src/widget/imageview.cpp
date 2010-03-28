/**
 * @file imageview.cpp
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
#include "imageview.hpp"
#include "navigator.hpp"
#include "scalewidget.hpp"

#include <QtCore/QSettings>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QGraphicsItem>

using namespace KomiX::widget;

ImageView::ImageView( QWidget * parent ):
QGraphicsView( parent ),
controller_( new FileController( this ) ),
imgRatio_( 1.0 ),
imgRect_(),
msInterval_( 1 ),
navigator_( new Navigator( this ) ),
panel_( new ScaleWidget( this ) ),
pixelInterval_( 1 ),
pressEndPosition_(),
pressStartPosition_(),
scaleMode_( Custom ),
vpRect_() {
	this->setScene( new QGraphicsScene( this ) );
	this->vpRect_ = this->mapToScene( this->viewport()->rect() ).boundingRect();

	QObject::connect( this->controller_, SIGNAL( imageLoaded( const QPixmap & ) ), this, SLOT( setImage( const QPixmap & ) ) );
	QObject::connect( this->controller_, SIGNAL( errorOccured( const QString & ) ), this, SIGNAL( errorOccured( const QString & ) ) );

	QObject::connect( this->navigator_, SIGNAL( required( const QModelIndex & ) ), this->controller_, SLOT( open( const QModelIndex & ) ) );

	QObject::connect( this, SIGNAL( scaled( int ) ), this->panel_, SLOT( scale( int ) ) );
	QObject::connect( this->panel_, SIGNAL( scaled( int ) ), this, SLOT( scale( int ) ) );
	QObject::connect( this->panel_, SIGNAL( fitHeight() ), this, SLOT( fitHeight() ) );
	QObject::connect( this->panel_, SIGNAL( fitWidth() ), this, SLOT( fitWidth() ) );
	QObject::connect( this->panel_, SIGNAL( fitWindow() ), this, SLOT( fitWindow() ) );
}

bool ImageView::open( const QUrl & uri ) {
	return this->controller_->open( uri );
}

void ImageView::begin() {
	this->moveItems_( ( this->vpRect_.topRight() - this->imgRect_.topRight() ).toPoint() );
}

void ImageView::end() {
	// TODO
}

void ImageView::fitHeight() {
	double r = this->vpRect_.height() / this->imgRect_.height();
	this->scale( r, r );
	this->vpRect_ = this->mapToScene( this->viewport()->rect() ).boundingRect();
	this->imgRatio_ *= r;
	this->scaleMode_ = Height;
}

void ImageView::fitWidth() {
	double r = this->vpRect_.width() / this->imgRect_.width();
	this->scale( r, r );
	this->vpRect_ = this->mapToScene( this->viewport()->rect() ).boundingRect();
	this->imgRatio_ *= r;
	this->scaleMode_ = Width;
}

void ImageView::fitWindow() {
	if( this->imgRect_.width() > this->imgRect_.height() ) {
		this->fitWidth();
	} else {
		this->fitHeight();
	}
	this->scaleMode_ = Window;
}

void ImageView::loadSettings() {
	QSettings ini;

	this->pixelInterval_ = ini.value( "pixel_interval", 1 ).toInt();
	this->msInterval_ = ini.value( "ms_interval", 1 ).toInt();
}

void ImageView::nextPage() {
	this->controller_->next();
}

void ImageView::previousPage() {
	this->controller_->prev();
}

void ImageView::scale( int ratio ) {
	this->scaleMode_ = Custom;
	if( ratio <= 0 || this->items().empty() ) {
		return;
	}

	double r = ( ratio / 100.0 ) / this->imgRatio_;
	this->scale( r, r );
	this->vpRect_ = this->mapToScene( this->viewport()->rect() ).boundingRect();
	this->imgRatio_ *= r;
}

void ImageView::setImage( const QPixmap & pixmap ) {
	// FIXME: stop all movement
	this->scene()->clear();
	QGraphicsItem * item = this->scene()->addPixmap( pixmap );

	this->imgRect_ = item->sceneBoundingRect();
	this->imgRatio_ = 1.0;
	this->vpRect_ = this->mapToScene( this->viewport()->rect() ).boundingRect();
	this->center_( item );
	this->begin();
}

void ImageView::showControlPanel() {
	this->panel_->show();
}

void ImageView::showNavigator() {
	if( this->controller_->isEmpty() ) {
		emit this->errorOccured( tr( "No openable file." ) );
		return;
	}
	this->navigator_->setModel( this->controller_->getModel() );
	this->navigator_->setCurrentIndex( this->controller_->getCurrentIndex() );
	this->navigator_->exec();
}

void ImageView::smoothMove() {
	// TODO
}

void ImageView::smoothReversingMove() {
	// TODO
}

void ImageView::dragEnterEvent( QDragEnterEvent * event ) {
	if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
		event->acceptProposedAction();
	}
}

void ImageView::dragMoveEvent( QDragMoveEvent * event ) {
	event->acceptProposedAction();
}

void ImageView::dropEvent( QDropEvent * event ) {
	if( event->mimeData()->hasUrls() ) {
		QList< QUrl > urlList = event->mimeData()->urls();

		if( !urlList.empty() ) {
			foreach( QUrl url, urlList ) {
				emit this->fileDropped( url );
			}
		}
	}
	event->acceptProposedAction();
}

void ImageView::keyPressEvent( QKeyEvent * event ) {
	if( event->key() == Qt::Key_Up ) {
		this->moveItems_( QPoint( 0, 10 ) );
	} else if( event->key() == Qt::Key_Down ) {
		this->moveItems_( QPoint( 0, -10 ) );
	} else if( event->key() == Qt::Key_Left ) {
		this->moveItems_( QPoint( 10, 0 ) );
	} else if( event->key() == Qt::Key_Right ) {
		this->moveItems_( QPoint( -10, 0 ) );
	} else {
		// nothing
	}
}

void ImageView::mouseMoveEvent( QMouseEvent * event ) {
	if( event->buttons() & Qt::LeftButton ) {	// left drag event
		// change cursor icon
		foreach( QGraphicsItem * item, this->scene()->items() ) {
			if( item->cursor().shape() == Qt::BlankCursor ) {
				item->setCursor( Qt::ClosedHandCursor );
			}
		}

		QPoint delta = event->pos() - this->pressEndPosition_;
		this->moveItems_( delta );
		this->pressEndPosition_ = event->pos();	// update end point
	} else {
		foreach( QGraphicsItem * item, this->scene()->items() ) {
			if( item->cursor().shape() == Qt::BlankCursor ) {
				item->setCursor( Qt::OpenHandCursor );
			}
		}
	}
}

void ImageView::mousePressEvent( QMouseEvent * event ) {
	this->pressStartPosition_ = event->pos();
	this->pressEndPosition_ = event->pos();

	if( event->button() == Qt::LeftButton ) {
		foreach( QGraphicsItem * item, this->scene()->items() ) {
			item->setCursor( Qt::ClosedHandCursor );
		}
	}
}

void ImageView::mouseReleaseEvent( QMouseEvent * event ) {
	if( event->button() == Qt::LeftButton ) {
		if( this->pressStartPosition_ == event->pos() ) {
			// TODO: start smooth move
		}

		// update cursor icon
		foreach( QGraphicsItem * item, this->scene()->items() ) {
			if( item->cursor().shape() == Qt::ClosedHandCursor ) {
				item->setCursor( Qt::OpenHandCursor );
			}
		}
	} else if( event->button() == Qt::MidButton ) {
		if( this->pressStartPosition_ == event->pos() ) {
			if( event->modifiers() & Qt::ControlModifier ) {
				emit this->scaled( 0 );
			} else {
				emit this->middleClicked();
			}
		}
	} else if( event->button() == Qt::RightButton ) {
		if( this->pressStartPosition_ == event->pos() ) {
			// TODO: start reversing smooth move
		}
	} else {
		// no button?
	}
}

void ImageView::resizeEvent( QResizeEvent * event ) {
	this->QGraphicsView::resizeEvent( event );
	this->vpRect_ = this->mapToScene( this->viewport()->rect() ).boundingRect();
	this->updateScaling_();
}

void ImageView::wheelEvent( QWheelEvent * event ) {
	int delta = event->delta();
	if( event->modifiers() & Qt::ControlModifier ) {
		if( delta < 0 ) {
			emit this->scaled( -10 );
		} else if( delta > 0 ) {
			emit this->scaled( 10 );
		}
	} else {
		if( delta < 0 ) {
			this->controller_->next();
		} else if( delta > 0 ) {
			this->controller_->prev();
		}
	}
}

void ImageView::moveItems_( QPoint delta ) {
	// update viewport rectangle
	this->vpRect_ = this->mapToScene( this->viewport()->rect() ).boundingRect();
	if( this->vpRect_.width() >= this->imgRect_.width() ) {
		delta.setY( 0 );
	}
	if( this->vpRect_.height() >= this->imgRect_.height() ) {
		delta.setX( 0 );
	}

	QRectF reqRect = this->imgRect_.translated( delta );
	if( !reqRect.contains( this->vpRect_ ) ) {
		if( delta.x() != 0 ) {
			if( reqRect.right() < this->vpRect_.right() ) {	// moving left
				delta.setX( this->vpRect_.right() - this->imgRect_.right() );
			} else if( reqRect.left() > this->vpRect_.left() ) {	// moving right
				delta.setX( this->vpRect_.left() - this->imgRect_.left() );
			}
		}

		if( delta.y() != 0 ) {
			if( reqRect.bottom() < this->vpRect_.bottom() ) {	// moving top
				delta.setY( this->vpRect_.bottom() - this->imgRect_.bottom() );
			} else if( reqRect.top() > this->vpRect_.top() ) {	// moving bottom
				delta.setY( this->vpRect_.top() - this->imgRect_.top() );
			}
		}
	}

	foreach( QGraphicsItem * item, this->scene()->items() ) {
		item->setPos( item->pos() + delta );
	}
	this->imgRect_.translate( delta );
}

void ImageView::center_( QGraphicsItem * item ) {
	item->setPos( item->pos() + this->vpRect_.center() - this->imgRect_.center() );
	this->imgRect_ = item->sceneBoundingRect();
}

void ImageView::updateScaling_() {
	switch( this->scaleMode_ ) {
	case Custom:
		break;
	case Width:
		this->fitWidth();
		break;
	case Height:
		this->fitHeight();
		break;
	case Window:
		this->fitWindow();
		break;
	default:
		;
	}
}
