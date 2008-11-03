#include "imagearea.hpp"

#include <QTimer>
#include <QAction>
#include <QMouseEvent>
#include <QList>
#include <QUrl>
#include <QScrollBar>
#include <QtDebug>

namespace KomiX {

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
		
		connect( topTimer_, SIGNAL( timeout() ), this, SLOT( stepTop() ) );
		connect( bottomTimer_, SIGNAL( timeout() ), this, SLOT( stepBottom() ) );
		connect( leftTimer_, SIGNAL( timeout() ), this, SLOT( stepLeft() ) );
		connect( rightTimer_, SIGNAL( timeout() ), this, SLOT( stepRight() ) );
		
		QAction * space = new QAction( this );
		space->setShortcut( Qt::Key_Space );
		addAction( space );
		connect( space, SIGNAL( triggered() ), this, SLOT( autoMove() ) );
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
					emit autoMove();
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
			default:
				;
		}
	}
	
	void ImageArea::mouseMoveEvent( QMouseEvent * event ) {
		if( event->buttons() & Qt::LeftButton ) {
			QPoint d = movePosition_ - event->pos();
			moveScrollBars_( d.x(), d.y() );
			movePosition_ = event->pos();
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
					emit fileDroped( url.toLocalFile() );
				}
			}
		}
		event->acceptProposedAction();
	}
	
	void ImageArea::openFile( const QString & fileName ) {
		stopAllStep_();
		image_->setPixmap( QPixmap( fileName ) );
		imageSize_ = image_->pixmap()->size();
		image_->resize( imageSize_ * ratio_ );
		
		horizontalScrollBar()->setValue( horizontalScrollBar()->maximum() );
		verticalScrollBar()->setValue( verticalScrollBar()->minimum() );
		
		state_ = TopRight;
	}
	
	void ImageArea::scale( int ratio ) {
		qDebug() << "ImageArea::scale( int ):";
		qDebug() << "ratio: " << ratio;
		qDebug() << "ratio_: " << ratio_;
		qDebug() << "imageSize_: " << imageSize_;
		ratio_ = ratio / 100.0;
		if( image_->pixmap() ) {
			image_->resize( imageSize_ * ratio_ );
		}
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
	
	void ImageArea::autoMove() {
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

}
