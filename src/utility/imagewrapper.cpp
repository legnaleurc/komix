#include "imagewrapper.hpp"

#include <QtGui/QImageReader>
#include <QtGui/QPixmap>
#include <QtGui/QMovie>

namespace KomiX {

	class ImageWrapper::Private {
	public:
		Private( const QString & path );

		bool animatable;
		QString path;
	};

}

using KomiX::ImageWrapper;

ImageWrapper::Private::Private( const QString & path ):
animatable( false ),
path( path ) {
	QImageReader fin( path );
	this->animatable = fin.supportsAnimation();
}

ImageWrapper::ImageWrapper():p_() {
}

ImageWrapper::ImageWrapper( const QString & path ): p_( new Private( path ) ) {
}

QLabel * ImageWrapper::createLabel() const {
	if( !this->p_ ) {
		return NULL;
	}
	QLabel * label = new QLabel;
	if( this->p_->animatable ) {
		QMovie * movie = new QMovie( this->p_->path );
		movie->setParent( label );
		label->setMovie( movie );
		movie->start();
		label->resize( movie->frameRect().size() );
	} else {
		QPixmap pixmap( this->p_->path );
		label->setPixmap( pixmap );
		label->resize( pixmap.size() );
	}
	return label;
}
