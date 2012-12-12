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
using KomiX::FileController;
using KomiX::ViewState;

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
pixelInterval( 1 ),
pressEndPosition(),
pressStartPosition(),
scaleMode( Custom ),
vpRect(),
trState( new ViewState( [this]()->QLineF {
	return QLineF( QPointF( 0.0, 0.0 ), this->vpRect.topRight() - this->imgRect.topRight() );
} ) ),
brState( new ViewState( [this]()->QLineF {
	return QLineF( QPointF( 0.0, 0.0 ), this->vpRect.bottomRight() - this->imgRect.bottomRight() );
} ) ),
tlState( new ViewState( [this]()->QLineF {
	return QLineF( QPointF( 0.0, 0.0 ), this->vpRect.topLeft() - this->imgRect.topLeft() );
} ) ),
blState( new ViewState( [this]()->QLineF {
	return QLineF( QPointF( 0.0, 0.0 ), this->vpRect.bottomLeft() - this->imgRect.bottomLeft() );
} ) ),
tState( new ViewState( [this]()->QLineF {
	return QLineF( 0.0, 0.0, 0.0, this->vpRect.top() - this->imgRect.top() );
} ) ),
bState( new ViewState( [this]()->QLineF {
	return QLineF( 0.0, 0.0, 0.0, this->vpRect.bottom() - this->imgRect.bottom() );
} ) ),
rState( new ViewState( [this]()->QLineF {
	return QLineF( 0.0, 0.0, this->vpRect.right() - this->imgRect.right(), 0.0 );
} ) ),
lState( new ViewState( [this]()->QLineF {
	return QLineF( 0.0, 0.0, this->vpRect.left() - this->imgRect.left(), 0.0 );
} ) ),
cState( new ViewState( []()->QLineF {
	return QLineF();
} ) ),
currentState() {
	this->addTransition( this->trState->next(), this->brState );
	this->trState->previous().connect( std::bind( &ImageView::previousPage, this->owner ) );
	this->trState->previous().connect( std::bind( &ImageView::end, this->owner ) );
	this->addTransition( this->trState->last(), this->blState );
	this->addTransition( this->trState->bottom(), this->brState );
	this->addTransition( this->trState->left(), this->tlState );
	this->addTransition( this->trState->narrower(), this->tState );
	this->addTransition( this->trState->lower(), this->rState );

	this->addTransition( this->brState->next(), this->tlState );
	this->addTransition( this->brState->previous(), this->trState );
	this->addTransition( this->brState->first(), this->trState );
	this->addTransition( this->brState->last(), this->blState );
	this->addTransition( this->brState->top(), this->trState );
	this->addTransition( this->brState->left(), this->blState );
	this->addTransition( this->brState->narrower(), this->bState );
	this->addTransition( this->brState->lower(), this->rState );

	this->addTransition( this->tlState->next(), this->blState );
	this->addTransition( this->tlState->previous(), this->brState );
	this->addTransition( this->tlState->first(), this->trState );
	this->addTransition( this->tlState->last(), this->blState );
	this->addTransition( this->tlState->bottom(), this->blState );
	this->addTransition( this->tlState->right(), this->trState );
	this->addTransition( this->tlState->narrower(), this->tState );
	this->addTransition( this->tlState->lower(), this->lState );

	this->blState->next().connect( std::bind( &ImageView::nextPage, this->owner ) );
	this->blState->next().connect( std::bind( &ImageView::begin, this->owner ) );
	this->addTransition( this->blState->previous(), this->tlState );
	this->addTransition( this->blState->first(), this->trState );
	this->addTransition( this->blState->top(), this->tlState );
	this->addTransition( this->blState->right(), this->brState );
	this->addTransition( this->blState->narrower(), this->bState );
	this->addTransition( this->blState->lower(), this->lState );

	this->addTransition( this->tState->next(), this->bState );
	this->tState->previous().connect( std::bind( &ImageView::previousPage, this->owner ) );
	this->tState->previous().connect( std::bind( &ImageView::end, this->owner ) );
	this->addTransition( this->tState->last(), this->bState );
	this->addTransition( this->tState->bottom(), this->bState );
	this->addTransition( this->tState->wider(), this->trState );
	this->addTransition( this->tState->lower(), this->cState );

	this->bState->next().connect( std::bind( &ImageView::nextPage, this->owner ) );
	this->bState->next().connect( std::bind( &ImageView::begin, this->owner ) );
	this->addTransition( this->bState->previous(), this->tState );
	this->addTransition( this->bState->first(), this->tState );
	this->addTransition( this->bState->top(), this->tState );
	this->addTransition( this->bState->wider(), this->brState );
	this->addTransition( this->bState->lower(), this->cState );

	this->addTransition( this->rState->next(), this->lState );
	this->rState->previous().connect( std::bind( &ImageView::previousPage, this->owner ) );
	this->rState->previous().connect( std::bind( &ImageView::end, this->owner ) );
	this->addTransition( this->rState->last(), this->lState );
	this->addTransition( this->rState->left(), this->lState );
	this->addTransition( this->rState->narrower(), this->cState );
	this->addTransition( this->rState->higher(), this->trState );

	this->lState->next().connect( std::bind( &ImageView::nextPage, this->owner ) );
	this->lState->next().connect( std::bind( &ImageView::begin, this->owner ) );
	this->addTransition( this->lState->previous(), this->rState );
	this->addTransition( this->lState->first(), this->rState );
	this->addTransition( this->lState->right(), this->rState );
	this->addTransition( this->lState->narrower(), this->cState );
	this->addTransition( this->lState->higher(), this->tlState );

	this->cState->previous().connect( std::bind( &ImageView::previousPage, this->owner ) );
	this->cState->previous().connect( std::bind( &ImageView::end, this->owner ) );
	this->cState->next().connect( std::bind( &ImageView::nextPage, this->owner ) );
	this->cState->next().connect( std::bind( &ImageView::begin, this->owner ) );
	this->addTransition( this->cState->wider(), this->rState );
	this->addTransition( this->cState->higher(), this->tState );

	this->currentState = this->cState;
}

void ImageView::Private::addTransition( boost::signals2::signal< void () > & signal, std::shared_ptr< ViewState > state ) {
	signal.connect( [this, state]()->void {
		this->currentState = state;
	} );
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
	if( this->image->boundingRect().isNull() ) {
		return;
	}
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

	this->onImageChanged();
}

void ImageView::Private::animeStateChanged( QAbstractAnimation::State newState, QAbstractAnimation::State /*oldState*/ ) {
	if( newState == QAbstractAnimation::Stopped ) {
		this->imgRect = this->image->sceneBoundingRect();
	}
}

void ImageView::Private::moveBy( const QPointF & delta ) {
	if( !this->image ) {
		return;
	}
	this->image->moveBy( delta.x(), delta.y() );
	this->imgRect.translate( delta );

	auto img = this->imgRect.center();
	auto vp = this->vpRect.center();
	if( vp.x() < img.x() ) {
		this->currentState->left()();
	} else {
		this->currentState->right()();
	}
	if( vp.y() > img.y() ) {
		this->currentState->bottom()();
	} else {
		this->currentState->top()();
	}
}

void ImageView::Private::fromViewportMoveBy( QPointF delta ) {
	delta /= this->imgRatio;
	if( this->anime ) {
		this->anime->stop();
	}
	QLineF v = this->normalizeMotionVector( delta.x(), delta.y() );
	this->moveBy( v.p2() );
}

void ImageView::Private::scale( double ratio ) {
	this->owner->scale( ratio, ratio );
	this->updateViewportRectangle();
	// will move to center
	this->fromViewportMoveBy();
	this->imgRatio *= ratio;

	// update state
	if( this->imgRect.width() > this->vpRect.width() && !qFuzzyCompare( this->imgRect.width(), this->vpRect.width() ) ) {
		this->currentState->wider()();
	} else {
		this->currentState->narrower()();
	}
	if( this->imgRect.height() > this->vpRect.height() && !qFuzzyCompare( this->imgRect.height(), this->vpRect.height() ) ) {
		this->currentState->higher()();
	} else {
		this->currentState->lower()();
	}
}

void ImageView::Private::updateScaling() {
	switch( this->scaleMode ) {
	case Custom:
		this->scale( 1.0 );
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

QLineF ImageView::Private::normalizeMotionVector( double dx, double dy ) {
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
}

void ImageView::setPaused( bool paused ) {
	if( this->p_->image ) {
		this->p_->image->setPaused( paused );
	}
}

void ImageView::initialize( FileController * controller ) {
	this->p_->controller = controller;

	this->p_->connect( this->p_->controller, SIGNAL( imageLoaded( QIODevice * ) ), SLOT( addImage( QIODevice * ) ) );
}

bool ImageView::open( const QUrl & uri ) {
	return this->p_->controller->open( uri );
}

void ImageView::begin() {
	this->p_->currentState->first()();
	auto mv = this->p_->currentState->getMotionVector();
	if( !mv.isNull() ) {
		this->p_->moveBy( mv.p2() );
	}
}

void ImageView::end() {
	this->p_->currentState->last()();
	auto mv = this->p_->currentState->getMotionVector();
	if( !mv.isNull() ) {
		this->p_->moveBy( mv.p2() );
	}
}

void ImageView::fitHeight() {
	this->p_->scale( this->p_->vpRect.height() / this->p_->imgRect.height() );
	this->p_->scaleMode = Private::Height;
}

void ImageView::fitWidth() {
	this->p_->scale( this->p_->vpRect.width() / this->p_->imgRect.width() );
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

	this->p_->scale( pcRatio / 100.0 / this->p_->imgRatio );
	this->p_->scaleMode = Private::Custom;
}

void ImageView::smoothMove() {
	if( !this->p_->image ) {
		// do nothing if no image
		return;
	}
	// stop all animations
	this->p_->anime->stop();

	this->p_->currentState->next()();
	auto mv = this->p_->currentState->getMotionVector();
	if( !mv.isNull() ) {
		int t = mv.length() * this->p_->msInterval / this->p_->pixelInterval;
		this->p_->setupAnimation( t, mv.dx(), mv.dy() );
		this->p_->anime->start();
	}
}

void ImageView::smoothReversingMove() {
	if( !this->p_->image ) {
		// do nothing if no image
		return;
	}
	// stop all animations
	this->p_->anime->stop();

	this->p_->currentState->previous()();
	auto mv = this->p_->currentState->getMotionVector();
	if( !mv.isNull() ) {
		int t = mv.length() * this->p_->msInterval / this->p_->pixelInterval;
		this->p_->setupAnimation( t, mv.dx(), mv.dy() );
		this->p_->anime->start();
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
		this->p_->fromViewportMoveBy( QPoint( 0, 10 ) );
	} else if( event->key() == Qt::Key_Down ) {
		this->p_->fromViewportMoveBy( QPoint( 0, -10 ) );
	} else if( event->key() == Qt::Key_Left ) {
		this->p_->fromViewportMoveBy( QPoint( 10, 0 ) );
	} else if( event->key() == Qt::Key_Right ) {
		this->p_->fromViewportMoveBy( QPoint( -10, 0 ) );
	} else {
		// nothing
	}
}

void ImageView::mouseMoveEvent( QMouseEvent * event ) {
	if( !this->p_->image ) {
		return;
	}

	if( event->buttons() & Qt::LeftButton ) {	// left drag event
		// change cursor icon
		if( this->p_->image->cursor().shape() == Qt::BlankCursor ) {
			this->p_->image->setCursor( Qt::ClosedHandCursor );
		}

		QPoint delta = event->pos() - this->p_->pressEndPosition;
		this->p_->fromViewportMoveBy( delta );
		this->p_->pressEndPosition = event->pos();	// update end point
	} else {
		if( this->p_->image->cursor().shape() == Qt::BlankCursor ) {
			this->p_->image->setCursor( Qt::OpenHandCursor );
		}
	}
}

void ImageView::mousePressEvent( QMouseEvent * event ) {
	if( !this->p_->image ) {
		return;
	}

	this->p_->pressStartPosition = event->pos();
	this->p_->pressEndPosition = event->pos();

	if( event->button() == Qt::LeftButton ) {
		this->p_->image->setCursor( Qt::ClosedHandCursor );
	}
}

void ImageView::mouseReleaseEvent( QMouseEvent * event ) {
	if( !this->p_->image ) {
		return;
	}

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
			this->p_->fromViewportMoveBy( QPointF( delta, 0 ) );
		} else {
			this->p_->fromViewportMoveBy( QPointF( 0, delta ) );
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
