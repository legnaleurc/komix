#include "imagearea.hpp"

namespace KomiX {

	ImageArea::ImageArea( QWidget * parent ) : QScrollArea( parent ), image_( new QLabel( this ) ), topTimer_( new QTimer ), bottomTimer_( new QTimer ), leftTimer_( new QTimer ), rightTimer_( new QTimer ), scalar_( 1.0 ), step_( 1 ), interval_( 1 ) {
		setWidget( image_ );
		image_->setBackgroundRole( QPalette::Base );
		image_->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
		image_->setScaledContents( true );
		image_->resize( 0, 0 );
		image_->setCursor( Qt::OpenHandCursor );
		
		connect( topTimer_, SIGNAL( timeout() ), this, SLOT( stepTop() ) );
		connect( bottomTimer_, SIGNAL( timeout() ), this, SLOT( stepBottom() ) );
		connect( leftTimer_, SIGNAL( timeout() ), this, SLOT( stepLeft() ) );
		connect( rightTimer_, SIGNAL( timeout() ), this, SLOT( stepRight() ) );
		
		QPointer< QAction > space = new QAction( this );
		space->setShortcut( Qt::Key_Space );
		addAction( space );
		connect( space, SIGNAL( triggered() ), this, SLOT( autoMove() ) );
	}
	
	void ImageArea::mousePressEvent( QMouseEvent * event ) {
		if( event->button() == Qt::LeftButton ) {
			downPosition_ = event->pos();
			movePosition_ = event->pos();
			image_->setCursor( Qt::ClosedHandCursor );
		}
	}
	
	void ImageArea::mouseReleaseEvent( QMouseEvent * event ) {
		if( event->button() == Qt::LeftButton ) {
			if( downPosition_ == event->pos() ) {
				emit autoMove();
			}
			if( image_->cursor().shape() == Qt::ClosedHandCursor ) {
				image_->setCursor( Qt::OpenHandCursor );
			}
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
					if( isSupport( url.toLocalFile() ) ) {
						emit fileDroped( url.toLocalFile() );
					}
				}
			}
		}
		event->acceptProposedAction();
	}
	
	void ImageArea::openFile( const QString & fileName ) {
		stopAllStep_();
		image_->setPixmap( QPixmap( fileName ) );
		image_->resize( image_->pixmap()->size() * scalar_ );
		
		horizontalScrollBar()->setValue( horizontalScrollBar()->maximum() );
		verticalScrollBar()->setValue( verticalScrollBar()->minimum() );
		
		state_ = TopRight;
	}
	
	void ImageArea::scale( int scalar ) {
		scalar_ = 1 + scalar / 100.0;
		image_->resize( image_->pixmap()->size() *scalar_ );
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
	
	void ImageArea::moveScrollBars_( int x, int y ) {
		horizontalScrollBar()->setValue( horizontalScrollBar()->value() + x );
		verticalScrollBar()->setValue( verticalScrollBar()->value() + y );
	}
	
	bool ImageArea::canMoveTop_() const {
		return verticalScrollBar()->value() > verticalScrollBar()->minimum();
	}
	
	bool ImageArea::canMoveBottom_() const {
		return verticalScrollBar()->value() < verticalScrollBar()->maximum();
	}
	
	bool ImageArea::canMoveLeft_() const {
		return horizontalScrollBar()->value() > horizontalScrollBar()->minimum();
	}
	
	bool ImageArea::canMoveRight_() const {
		return horizontalScrollBar()->value() > horizontalScrollBar()->maximum();
	}

}
