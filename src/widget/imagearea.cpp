/**
 * @file imagearea.cpp
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
#include "imagearea.hpp"
#include "scalepanel.hpp"
#include "navigator.hpp"
#include "filecontroller.hpp"

#include <QTimer>
#include <QAction>
#include <QMouseEvent>
#include <QList>
#include <QUrl>
#include <QScrollBar>
#include <QtDebug>

namespace KomiX { namespace widget {

ImageArea::ImageArea( QWidget * parent ) :
QScrollArea( parent ),
scale_( new ScalePanel( this, Qt::Dialog ) ),
navi_( new Navigator( this ) ),
ctrl_( new FileController( this ) ),
image_( new QLabel( this ) ),
imageSize_(),
topTimer_( new QTimer( this ) ),
bottomTimer_( new QTimer( this ) ),
leftTimer_( new QTimer( this ) ),
rightTimer_( new QTimer( this ) ),
ratio_( 1.0 ), step_( 2 ),
interval_( 1 ) {
	setBackgroundRole( QPalette::Dark );
	setAlignment( Qt::AlignCenter );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setAcceptDrops( true );

	setWidget( image_ );
	image_->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
	image_->setScaledContents( true );
	image_->resize( 0, 0 );
	image_->setCursor( Qt::OpenHandCursor );
	QPalette p = palette();
	p.setColor( QPalette::Dark, QColor::fromRgb( 0, 0, 0 ) );
	setPalette( p );
	image_->setMouseTracking( true );
	setMouseTracking( true );

	connect( this, SIGNAL( scaled( int ) ), scale_, SLOT( scale( int ) ) );
	connect( scale_, SIGNAL( scaled( int ) ), this, SLOT( scale( int ) ) );
	connect( ctrl_, SIGNAL( imageLoaded( const QPixmap & ) ), this, SLOT( setImage( const QPixmap & ) ) );
	connect( ctrl_, SIGNAL( errorOccured( const QString & ) ), this, SIGNAL( errorOccured( const QString & ) ) );

	connect( navi_, SIGNAL( required( const QModelIndex & ) ), ctrl_, SLOT( open( const QModelIndex & ) ) );

	connect( topTimer_, SIGNAL( timeout() ), this, SLOT( stepTop() ) );
	connect( bottomTimer_, SIGNAL( timeout() ), this, SLOT( stepBottom() ) );
	connect( leftTimer_, SIGNAL( timeout() ), this, SLOT( stepLeft() ) );
	connect( rightTimer_, SIGNAL( timeout() ), this, SLOT( stepRight() ) );
}

bool ImageArea::open( const QUrl & url ) {
	return ctrl_->open( url );
}

void ImageArea::prev() {
	ctrl_->prev();
}

void ImageArea::next() {
	ctrl_->next();
}

void ImageArea::showScalePanel() {
	scale_->show();
}

void ImageArea::showNavigator() {
	if( ctrl_->isEmpty() ) {
		emit errorOccured( tr( "No openable file." ) );
		return;
	}
	navi_->setModel( ctrl_->getModel() );
	navi_->setCurrentIndex( ctrl_->getCurrentIndex() );
	navi_->exec();
}

void ImageArea::mousePressEvent( QMouseEvent * event ) {
	downPosition_ = event->pos();
	movePosition_ = event->pos();

	if( event->button() == Qt::LeftButton ) {
		image_->setCursor( Qt::ClosedHandCursor );
	}
}

void ImageArea::mouseReleaseEvent( QMouseEvent * event ) {
	switch( event->button() ) {
	case Qt::LeftButton:
		if( downPosition_ == event->pos() ) {
			emit smoothMove();
		}
		if( image_->cursor().shape() == Qt::ClosedHandCursor ) {
			image_->setCursor( Qt::OpenHandCursor );
		}
		break;
	case Qt::MidButton:
		if( downPosition_ == event->pos() ) {
			if( event->modifiers() & Qt::ControlModifier ) {
				emit scaled( 0 );
			} else {
				emit requireToogleScreen();
			}
		}
		break;
	case Qt::RightButton:
		if( downPosition_ == event->pos() ) {
			emit reverseSmoothMove();
		}
		break;
	default:
		;
	}
}

void ImageArea::mouseMoveEvent( QMouseEvent * event ) {
	if( event->buttons() & Qt::LeftButton ) {
		if( image_->cursor().shape() == Qt::BlankCursor ) {
			image_->setCursor( Qt::ClosedHandCursor );
		}
		QPoint d = movePosition_ - event->pos();
		moveScrollBars_( d.x(), d.y() );
		movePosition_ = event->pos();
	} else {
		if( image_->cursor().shape() == Qt::BlankCursor ) {
			image_->setCursor( Qt::OpenHandCursor );
		}
	}
}

void ImageArea::wheelEvent( QWheelEvent * event ) {
	int delta = event->delta();
	if( event->modifiers() & Qt::ControlModifier ) {
		if( delta < 0 ) {
			emit scaled( -10 );
		} else if( delta > 0 ) {
			emit scaled( 10 );
		}
	} else {
		if( delta < 0 ) {
			ctrl_->next();
		} else if( delta > 0 ) {
			ctrl_->prev();
		}
	}
}

void ImageArea::dragEnterEvent( QDragEnterEvent * event ) {
	if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
		event->acceptProposedAction();
	}
}

void ImageArea::dropEvent( QDropEvent * event ) {
	if( event->mimeData()->hasUrls() ) {
		QList< QUrl > urlList = event->mimeData()->urls();
		
		if( !urlList.empty() ) {
			foreach( QUrl url, urlList ) {
				emit fileDroped( url );
			}
		}
	}
	event->acceptProposedAction();
}

void ImageArea::resizeEvent( QResizeEvent * event ) {
	updateImageSize_();
	QScrollArea::resizeEvent( event );
}

void ImageArea::setImage( const QPixmap & image ) {
	stopAllStep_();
	image_->setPixmap( image );
	imageSize_ = image_->pixmap()->size();
	updateImageSize_();
	
	home();
}

void ImageArea::updateImageSize_() {
	if( image_->pixmap() ) {
		if( ratio_ >= 0.0 ) {
			image_->resize( imageSize_ * ratio_ );
		} else if( ratio_ == -1.0 ) {
			image_->resize( viewport()->width(), imageSize_.height() * ( viewport()->width() / static_cast< double >( imageSize_.width() ) ) );
		} else if( ratio_ == -2.0 ) {
			image_->resize( imageSize_.width() * ( viewport()->height() / static_cast< double >( imageSize_.height() ) ), viewport()->height() );
		} else if( ratio_ == -3.0 ) {
			QSize bound = image_->size();
			bound.scale( viewport()->size(), Qt::KeepAspectRatio );
			image_->resize( bound );
		}
	}
}

void ImageArea::scale( int ratio ) {
// 		qDebug() << "ImageArea::scale( int ):";
// 		qDebug() << "ratio: " << ratio;
// 		qDebug() << "ratio_: " << ratio_;
// 		qDebug() << "imageSize_: " << imageSize_;

	ratio_ = ( ratio >= 0 ) ? ( ratio / 100.0 ) : ( ratio );
	updateImageSize_();
}

void ImageArea::stepTop() {
	if( canMoveTop_() ) {
		moveScrollBars_( 0, step_ * -1 );
	} else {
		topTimer_->stop();
	}
}

void ImageArea::stepBottom() {
	if( canMoveBottom_() ) {
		moveScrollBars_( 0, step_ );
	} else {
		bottomTimer_->stop();
	}
}

void ImageArea::stepLeft() {
	if( canMoveLeft_() ) {
		moveScrollBars_( step_ * -1, 0 );
	} else {
		leftTimer_->stop();
	}
}

void ImageArea::stepRight() {
	if( canMoveRight_() ) {
		moveScrollBars_( step_, 0 );
	} else {
		rightTimer_->stop();
	}
}

void ImageArea::smoothMove() {
	if( image_->cursor().shape() != Qt::BlankCursor ) {
		image_->setCursor( Qt::BlankCursor );
	}
	if( image_->pixmap() ) {
		stopAllStep_();
		switch( state_ ) {
		case TopRight:
			if( canMoveBottom_() ) {
				bottomTimer_->start( interval_ );
				state_ = BottomRight;
			} else if( canMoveLeft_() ) {
				leftTimer_->start( interval_ );
				state_ = TopLeft;
			} else {
				ctrl_->next();
			}
			break;
		case BottomRight:
			if( canMoveLeft_() ) {
				leftTimer_->start( interval_ );
				if( canMoveTop_() ) {
					topTimer_->start( interval_ );
				}
				state_ = TopLeft;
			} else {
				ctrl_->next();
			}
			break;
		case TopLeft:
			if( canMoveBottom_() ) {
				bottomTimer_->start( interval_ );
				state_ = BottomLeft;
			} else {
				ctrl_->next();
			}
			break;
		case BottomLeft:
			ctrl_->next();
			break;
		}
	}
}

void ImageArea::reverseSmoothMove() {
	if( image_->cursor().shape() != Qt::BlankCursor ) {
		image_->setCursor( Qt::BlankCursor );
	}
	if( image_->pixmap() ) {
		stopAllStep_();
		switch( state_ ) {
		case BottomLeft:
			qDebug( "BottomLeft" );
			if( canMoveTop_() ) {
				topTimer_->start( interval_ );
				state_ = TopLeft;
			} else if( canMoveRight_() ) {
				rightTimer_->start( interval_ );
				state_ = BottomRight;
			} else {
				ctrl_->prev();
				end();
			}
			break;
		case TopLeft:
			qDebug( "TopLeft" );
			if( canMoveRight_() ) {
				rightTimer_->start( interval_ );
				if( canMoveBottom_() ) {
					bottomTimer_->start( interval_ );
				}
				state_ = BottomRight;
			} else {
				ctrl_->prev();
				end();
			}
			break;
		case BottomRight:
			qDebug( "BottomRight" );
			if( canMoveTop_() ) {
				topTimer_->start( interval_ );
				state_ = TopRight;
			} else {
				ctrl_->prev();
				end();
			}
			break;
		case TopRight:
			qDebug( "TopRight" );
			ctrl_->prev();
			end();
			break;
		}
	}
}

void ImageArea::home() {
	horizontalScrollBar()->setValue( horizontalScrollBar()->maximum() );
	verticalScrollBar()->setValue( verticalScrollBar()->minimum() );
	state_ = TopRight;
}

void ImageArea::end() {
	horizontalScrollBar()->setValue( horizontalScrollBar()->minimum() );
	verticalScrollBar()->setValue( verticalScrollBar()->maximum() );
	state_ = BottomLeft;
}

void ImageArea::stopAllStep_() {
	if( topTimer_->isActive() ) {
		topTimer_->stop();
	}
	if( bottomTimer_->isActive() ) {
		bottomTimer_->stop();
	}
	if( leftTimer_->isActive() ) {
		leftTimer_->stop();
	}
	if( rightTimer_->isActive() ) {
		rightTimer_->stop();
	}
}

inline void ImageArea::moveScrollBars_( int x, int y ) {
	horizontalScrollBar()->setValue( horizontalScrollBar()->value() + x );
	verticalScrollBar()->setValue( verticalScrollBar()->value() + y );
}

inline bool ImageArea::canMoveTop_() const {
	return verticalScrollBar()->value() > verticalScrollBar()->minimum();
}

inline bool ImageArea::canMoveBottom_() const {
	return verticalScrollBar()->value() < verticalScrollBar()->maximum();
}

inline bool ImageArea::canMoveLeft_() const {
	return horizontalScrollBar()->value() > horizontalScrollBar()->minimum();
}

inline bool ImageArea::canMoveRight_() const {
	return horizontalScrollBar()->value() < horizontalScrollBar()->maximum();
}

} } // end namespace
