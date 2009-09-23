#include "imagearea.hpp"

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
image_( new QLabel( this ) ),
imageSize_(),
topTimer_( new QTimer( this ) ),
bottomTimer_( new QTimer( this ) ),
leftTimer_( new QTimer( this ) ),
rightTimer_( new QTimer( this ) ),
ratio_( 1.0 ), step_( 2 ),
interval_( 1 ) {
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

	connect( topTimer_, SIGNAL( timeout() ), this, SLOT( stepTop() ) );
	connect( bottomTimer_, SIGNAL( timeout() ), this, SLOT( stepBottom() ) );
	connect( leftTimer_, SIGNAL( timeout() ), this, SLOT( stepLeft() ) );
	connect( rightTimer_, SIGNAL( timeout() ), this, SLOT( stepRight() ) );
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
			emit middleClicked();
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
	emit wheelMoved( event->delta() );
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
	scale();
	QScrollArea::resizeEvent( event );
}

void ImageArea::setImage( const QPixmap & image ) {
	stopAllStep_();
	image_->setPixmap( image );
	imageSize_ = image_->pixmap()->size();
	scale();
	
	home();
}

void ImageArea::scale() {
// 		qDebug() << "ImageArea::scale()";

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
	scale();
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
				emit nextPage();
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
				emit nextPage();
			}
			break;
		case TopLeft:
			if( canMoveBottom_() ) {
				bottomTimer_->start( interval_ );
				state_ = BottomLeft;
			} else {
				emit nextPage();
			}
			break;
		case BottomLeft:
			emit nextPage();
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
			if( canMoveTop_() ) {
				topTimer_->start( interval_ );
				state_ = TopLeft;
			} else if( canMoveRight_() ) {
				rightTimer_->start( interval_ );
				state_ = BottomRight;
			} else {
				emit prevPage();
			}
			break;
		case TopLeft:
			if( canMoveRight_() ) {
				rightTimer_->start( interval_ );
				if( canMoveBottom_() ) {
					bottomTimer_->start( interval_ );
				}
				state_ = BottomRight;
			} else {
				emit prevPage();
			}
			break;
		case BottomRight:
			if( canMoveTop_() ) {
				topTimer_->start( interval_ );
				state_ = TopRight;
			} else {
				emit prevPage();
			}
			break;
		case TopRight:
			emit prevPage();
			break;
		}
		end();
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
	return horizontalScrollBar()->value() > horizontalScrollBar()->maximum();
}

} } // end namespace
