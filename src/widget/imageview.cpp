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
#include "imageview_p.hpp"

#include <QtCore/QSettings>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>

using KomiX::widget::ImageView;
using KomiX::widget::Direction;
using KomiX::FileController;

ImageView::Private::Private( ImageView * owner ):
QObject(),
owner( owner ),
image( nullptr ),
anime( nullptr ),
controller( nullptr ),
imgRatio( 1.0 ),
imgRect(),
msInterval( 1 ),
pageBuffer(),
panel( new ScaleWidget( owner ) ),
pixelInterval( 1 ),
pressEndPosition(),
pressStartPosition(),
scaleMode( Custom ),
vpRect(),
vpState() {
}

void ImageView::Private::addImage( QIODevice * image ) {
	this->pageBuffer.push_back( image );
	if( this->pageBuffer.size() == 1 ) {
		// TODO should scale in multi-paging mode
		this->setImage( this->pageBuffer );
		this->pageBuffer.clear();
	}
}

void ImageView::Private::onImageChanged() {
	this->owner->scene()->setSceneRect( this->image->boundingRect() );
	this->image->setPos( 0.0, 0.0 );
	this->imgRect = this->image->sceneBoundingRect();

	this->updateViewportRectangle();
	this->updateScaling();
	this->owner->begin();
}

// TODO this function should consider multi-paging mode
void ImageView::Private::setImage( const QList< QIODevice * > & images ) {
	if( images.empty() ) {
		return;
	}
	// stop all movement
	if( this->anime ) {
		this->anime->stop();
		this->anime->deleteLater();
		this->anime = nullptr;
	}

	this->owner->scene()->clear();

	this->image = new ImageItem( images );
	this->connect( this->image, SIGNAL( changed() ), SLOT( onImageChanged() ) );
	this->owner->scene()->addItem( this->image );

	this->anime = new QPropertyAnimation( this->image, "pos" );
	this->connect( this->anime, SIGNAL( stateChanged( QAbstractAnimation::State, QAbstractAnimation::State ) ), SLOT( animeStateChanged( QAbstractAnimation::State, QAbstractAnimation::State ) ) );
}

void ImageView::Private::animeStateChanged( QAbstractAnimation::State newState, QAbstractAnimation::State /*oldState*/ ) {
	if( newState == QAbstractAnimation::Stopped ) {
		this->imgRect = this->image->sceneBoundingRect();
	}
}

void ImageView::Private::moveBy( const QPointF & delta ) {
	if( this->image ) {
		this->image->moveBy( delta.x(), delta.y() );
		this->imgRect.translate( delta );
	}
}

void ImageView::Private::updateScaling() {
	switch( this->scaleMode ) {
	case Custom:
		this->owner->scale( 1.0 );
		break;
	case Width:
		this->owner->fitWidth();
		break;
	case Height:
		this->owner->fitHeight();
		break;
	case Window:
		this->owner->fitWindow();
		break;
	default:
		;
	}
}

void ImageView::Private::updateViewportRectangle() {
	this->vpRect = this->owner->mapToScene( this->owner->viewport()->rect() ).boundingRect();
}

QLineF ImageView::Private::getMotionVector( Direction d ) {
	double dx = 0.0, dy = 0.0;
	switch( d ) {
		case Top:
			dy = this->vpRect.top() - this->imgRect.top();
			break;
		case Bottom:
			dy = this->vpRect.bottom() - this->imgRect.bottom();
			break;
		case Left:
			dx = this->vpRect.left() - this->imgRect.left();
			break;
		case Right:
			dx = this->vpRect.right() - this->imgRect.right();
			break;
		case TopRight:
			dx = this->vpRect.right() - this->imgRect.right();
			dy = this->vpRect.top() - this->imgRect.top();
			break;
		case BottomRight:
			dx = this->vpRect.right() - this->imgRect.right();
			dy = this->vpRect.bottom() - this->imgRect.bottom();
			break;
		case TopLeft:
			dx = this->vpRect.left() - this->imgRect.left();
			dy = this->vpRect.top() - this->imgRect.top();
			break;
		case BottomLeft:
			dx = this->vpRect.left() - this->imgRect.left();
			dy = this->vpRect.bottom() - this->imgRect.bottom();
			break;
	}
	return this->getMotionVector( dx, dy );
}

QLineF ImageView::Private::getMotionVector( double dx, double dy ) {
	QRectF req = this->imgRect.translated( dx, dy );
	// horizontal
	if( this->imgRect.width() < this->vpRect.width() ) {
		dx += this->vpRect.center().x() - req.center().x();
	} else if( req.left() > this->vpRect.left() ) {
		dx += this->vpRect.left() - req.left();
	} else if( req.right() < this->vpRect.right() ) {
		dx += this->vpRect.right() - req.right();
	}
	// vertical
	if( this->imgRect.height() < this->vpRect.height() ) {
		dy += this->vpRect.center().y() - req.center().y();
	} else if( req.top() > this->vpRect.top() ) {
		dy += this->vpRect.top() - req.top();
	} else if( req.bottom() < this->vpRect.bottom() ) {
		dy += this->vpRect.bottom() - req.bottom();
	}
	return QLineF( 0, 0, dx, dy );
}

void ImageView::Private::setupAnimation( int msDuration, double dx, double dy ) {
	this->anime->setDuration( msDuration );
	this->anime->setStartValue( this->image->pos() );
	this->anime->setEndValue( this->image->pos() + QPointF( dx, dy ) );
}

ImageView::ImageView( QWidget * parent ):
QGraphicsView( parent ),
p_( new Private( this ) ) {
	this->setScene( new QGraphicsScene( this ) );
	this->p_->vpRect = this->mapToScene( this->viewport()->rect() ).boundingRect();

	this->p_->panel->connect( this, SIGNAL( scaled( int ) ), SLOT( scale( int ) ) );
	this->connect( this->p_->panel, SIGNAL( scaled( int ) ), SLOT( scale( int ) ) );
	this->connect( this->p_->panel, SIGNAL( fitHeight() ), SLOT( fitHeight() ) );
	this->connect( this->p_->panel, SIGNAL( fitWidth() ), SLOT( fitWidth() ) );
	this->connect( this->p_->panel, SIGNAL( fitWindow() ), SLOT( fitWindow() ) );
}

void ImageView::initialize( FileController * controller ) {
	this->p_->controller = controller;

	this->p_->connect( this->p_->controller, SIGNAL( imageLoaded( QIODevice * ) ), SLOT( addImage( QIODevice * ) ) );
}

bool ImageView::open( const QUrl & uri ) {
	return this->p_->controller->open( uri );
}

void ImageView::moveTo( Direction d ) {
	this->p_->anime->stop();
	QLineF v = this->p_->getMotionVector( d );
	this->p_->moveBy( v.p2() );
}

void ImageView::slideTo( Direction d ) {
	this->p_->anime->stop();
	QLineF v = this->p_->getMotionVector( d );
	int t = v.length() / this->p_->pixelInterval * this->p_->msInterval;
	this->p_->setupAnimation( t, v.dx(), v.dy() );
	this->p_->anime->start();
}

void ImageView::moveBy( QPointF delta ) {
	delta /= this->p_->imgRatio;
	if( this->p_->anime ) {
		this->p_->anime->stop();
	}
	QLineF v = this->p_->getMotionVector( delta.x(), delta.y() );
	this->p_->moveBy( v.p2() );
}

void ImageView::begin() {
	this->p_->vpState = TopRight;
	this->moveTo( this->p_->vpState );
}

void ImageView::end() {
	this->p_->vpState = BottomLeft;
	this->moveTo( this->p_->vpState );
}

void ImageView::fitHeight() {
	this->scale( this->p_->vpRect.height() / this->p_->imgRect.height() );
	this->p_->scaleMode = Private::Height;
}

void ImageView::fitWidth() {
	this->scale( this->p_->vpRect.width() / this->p_->imgRect.width() );
	this->p_->scaleMode = Private::Width;
}

void ImageView::fitWindow() {
	double dW = this->p_->imgRect.width() - this->p_->vpRect.width();
	double dH = this->p_->imgRect.height() - this->p_->vpRect.height();
	if( dW > dH ) {
		this->fitWidth();
	} else {
		this->fitHeight();
	}
	this->p_->scaleMode = Private::Window;
}

void ImageView::loadSettings() {
	QSettings ini;

	this->p_->pixelInterval = ini.value( "pixel_interval", 1 ).toInt();
	this->p_->msInterval = ini.value( "ms_interval", 1 ).toInt();
}

void ImageView::nextPage() {
	this->p_->controller->next();
}

void ImageView::previousPage() {
	this->p_->controller->prev();
}

void ImageView::scale( int pcRatio ) {
	if( pcRatio <= 0 || !this->p_->image ) {
		return;
	}

	this->scale( pcRatio / 100.0 / this->p_->imgRatio );
	this->p_->scaleMode = Private::Custom;
}

void ImageView::scale( double ratio ) {
	this->scale( ratio, ratio );
	this->p_->updateViewportRectangle();
	this->moveBy();
	this->p_->imgRatio *= ratio;
}

void ImageView::showControlPanel() {
	this->p_->panel->show();
}

void ImageView::smoothMove() {
	if( !this->p_->image ) {
		return;
	}
	this->p_->anime->stop();
	switch( this->p_->vpState ) {
	case TopRight:
		this->p_->vpState = BottomRight;
		if( this->p_->imgRect.bottom() > this->p_->vpRect.bottom() ) {
			this->slideTo( this->p_->vpState );
		} else {
			this->smoothMove();
		}
		break;
	case BottomRight:
		this->p_->vpState = TopLeft;
		if( this->p_->imgRect.left() < this->p_->vpRect.left() ) {
			this->slideTo( this->p_->vpState );
		} else {
			this->smoothMove();
		}
		break;
	case TopLeft:
		this->p_->vpState = BottomLeft;
		if( this->p_->imgRect.bottom() > this->p_->vpRect.bottom() ) {
			this->slideTo( this->p_->vpState );
		} else {
			this->smoothMove();
		}
		break;
	case BottomLeft:
		this->p_->controller->next();
		break;
	}
}

void ImageView::smoothReversingMove() {
	if( !this->p_->image ) {
		return;
	}
	this->p_->anime->stop();
	switch( this->p_->vpState ) {
	case BottomLeft:
		this->p_->vpState = TopLeft;
		if( this->p_->imgRect.top() < this->p_->vpRect.top() ) {
			this->slideTo( this->p_->vpState );
		} else {
			this->smoothReversingMove();
		}
		break;
	case TopLeft:
		this->p_->vpState = BottomRight;
		if( this->p_->imgRect.right() > this->p_->vpRect.right() ) {
			this->slideTo( this->p_->vpState );
		} else {
			this->smoothReversingMove();
		}
		break;
	case BottomRight:
		this->p_->vpState = TopRight;
		if( this->p_->imgRect.top() < this->p_->vpRect.top() ) {
			this->slideTo( this->p_->vpState );
		} else {
			this->smoothReversingMove();
		}
		break;
	case TopRight:
		this->p_->controller->prev();
		this->end();
		break;
	}
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
		this->moveBy( QPoint( 0, 10 ) );
	} else if( event->key() == Qt::Key_Down ) {
		this->moveBy( QPoint( 0, -10 ) );
	} else if( event->key() == Qt::Key_Left ) {
		this->moveBy( QPoint( 10, 0 ) );
	} else if( event->key() == Qt::Key_Right ) {
		this->moveBy( QPoint( -10, 0 ) );
	} else {
		// nothing
	}
}

void ImageView::mouseMoveEvent( QMouseEvent * event ) {
	if( event->buttons() & Qt::LeftButton ) {	// left drag event
		// change cursor icon
		if( this->p_->image->cursor().shape() == Qt::BlankCursor ) {
			this->p_->image->setCursor( Qt::ClosedHandCursor );
		}

		QPoint delta = event->pos() - this->p_->pressEndPosition;
		this->moveBy( delta );
		this->p_->pressEndPosition = event->pos();	// update end point
	} else {
		if( this->p_->image->cursor().shape() == Qt::BlankCursor ) {
			this->p_->image->setCursor( Qt::OpenHandCursor );
		}
	}
}

void ImageView::mousePressEvent( QMouseEvent * event ) {
	this->p_->pressStartPosition = event->pos();
	this->p_->pressEndPosition = event->pos();

	if( event->button() == Qt::LeftButton ) {
		this->p_->image->setCursor( Qt::ClosedHandCursor );
	}
}

void ImageView::mouseReleaseEvent( QMouseEvent * event ) {
	if( event->button() == Qt::LeftButton ) {
		if( this->p_->pressStartPosition == event->pos() ) {
			this->smoothMove();
		}

		// update cursor icon
		if( this->p_->image->cursor().shape() == Qt::ClosedHandCursor ) {
			this->p_->image->setCursor( Qt::OpenHandCursor );
		}
	} else if( event->button() == Qt::MidButton ) {
		if( this->p_->pressStartPosition == event->pos() ) {
			if( event->modifiers() & Qt::ControlModifier ) {
				emit this->scaled( 0 );
			} else {
				emit this->middleClicked();
			}
		}
	} else if( event->button() == Qt::RightButton ) {
		if( this->p_->pressStartPosition == event->pos() ) {
			this->smoothReversingMove();
		}
	} else {
		// no button?
	}
}

void ImageView::resizeEvent( QResizeEvent * event ) {
	this->QGraphicsView::resizeEvent( event );
	this->p_->updateViewportRectangle();
	this->p_->updateScaling();
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
#ifdef Q_OS_MAC
		if( event->orientation() == Qt::Horizontal ) {
			this->moveBy( QPointF( delta, 0 ) );
		} else {
			this->moveBy( QPointF( 0, delta ) );
		}
#else
		if( delta < 0 ) {
			this->p_->controller->next();
		} else if( delta > 0 ) {
			this->p_->controller->prev();
		}
#endif
	}
}
