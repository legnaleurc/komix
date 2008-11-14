#include "filecontroller.hpp"
#include "global.hpp"

#include <QPixmapCache>

namespace KomiX {

	FileController::FileController( unsigned int pfMax, QObject * parent ) :
	QObject( parent ),
	prefetchMax_( pfMax ),
	dir_(),
	files_(),
	index_( 0 ),
	cImage_() {
	}

	bool FileController::open( const QString & filePath ) {
		if( !update_( filePath ) || files_.empty() ) {
			return false;
		} else {
			fetch_( files_[index_] );
			emit openImage( cImage_ );
			return true;
		}
	}

	void FileController::next() {
		if( !files_.empty() ) {
			++index_;
			if( index_ >= files_.size() ) {
				index_ = 0;
			}
			fetch_( files_[index_] );
			emit openImage( cImage_ );
			prefetch_( index_ );
		}
	}

	void FileController::prev() {
		if( !files_.empty() ) {
			--index_;
			if( index_ < 0 ) {
				index_ = files_.size() - 1;
			}
			fetch_( files_[index_] );
			emit openImage( cImage_ );
			prefetch_( index_ );
		}
	}

	void FileController::setPrefetchMax( unsigned int pfMax ) {
		prefetchMax_ = pfMax;
	}

	inline unsigned int FileController::getPrefetchMax() const {
		return prefetchMax_;
	}

	void FileController::fetch_( const QString & fileName ) {
		if( !QPixmapCache::find( fileName, cImage_ ) ) {
			cImage_.load( dir_.filePath( fileName ) );
			QPixmapCache::insert( fileName, cImage_ );
		}
	}

	void FileController::prefetch_( unsigned int index ) {
		QPixmap pfImage;
		for( unsigned int i = 0; i < prefetchMax_; ++i ) {
			if( !QPixmapCache::find( files_[index+i], pfImage ) ) {
				pfImage.load( dir_.filePath( files_[index+i] ) );
				QPixmapCache::insert( files_[index+i], pfImage );
			}
		}
	}

	bool FileController::update_( const QString & filePath ) {
		QFileInfo tmp( filePath );
		if( tmp.isDir() ) {
			if( dir_ == tmp.absoluteFilePath() ) {
				return false;
			} else {
				dir_ = tmp.absoluteFilePath();
				files_ = dir_.entryList( SupportedFormatsFilter(), QDir::Files );
				index_ = 0;
			}
		} else {
			if( dir_ == tmp.dir() ) {
				if( files_[index_] == tmp.fileName() ) {
					return false;
				} else {
					index_ = files_.indexOf( tmp.fileName() );
				}
			} else {
				dir_ = tmp.dir();
				files_ = dir_.entryList( SupportedFormatsFilter(), QDir::Files );
				index_ = files_.indexOf( tmp.fileName() );
			}
		}
		prefetch_( index_ );
		return true;
	}

}
