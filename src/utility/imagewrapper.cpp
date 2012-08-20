#if 0
#include "imagewrapper.hpp"

#include <QtGui/QImageReader>
#include <QtGui/QPixmap>
#include <QtGui/QMovie>

#include <functional>

namespace KomiX {

class ImageWrapper::Private {
public:
	explicit Private( QIODevice * device );

	QIODevice * device;
};

}

using KomiX::ImageWrapper;

ImageWrapper::Private::Private( QIODevice * device ):
device( device ) {
}

ImageWrapper::ImageWrapper(): p_() {
}

ImageWrapper::ImageWrapper( QIODevice * device ): p_( new Private( device ) ) {
}

void ImageWrapper::moveToLabel( QLabel * label, const QSize & size ) const {
	if( !this->p_ ) {
		return;
	}
	if( this->p_->movie && this->p_->movie->isValid() ) {
		this->p_->movie->setScaledSize( size );
		this->p_->movie->setParent( label );
		label->setMovie( this->p_->movie.release() );
		label->movie()->start();
	} else if( !this->p_->pixmap.isNull() ) {
		label->setPixmap( this->p_->pixmap.scaled( size, Qt::KeepAspectRatio ) );
	}
}

QGraphicsItem * ImageWrapper::takeGraphicsItem( QGraphicsItem * parent ) const {
	return parent;
}
#endif
