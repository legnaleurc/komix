#include "filecontroller.hpp"
#include "global.hpp"

namespace KomiX {

	FileController::FileController( unsigned int pfMax, unsigned int limit, QObject * parent ) :
	QObject( parent ),
	prefetchMax_( pfMax ),
	limit_( limit ),
	dir_(),
	files_(),
	index_( 0 ),
	history_(),
	cache_() {
	}

	bool FileController::open( const QString & filePath ) {
		if( !update_( filePath ) || files_.empty() ) {
			return false;
		} else {
			emit openImage( fetch_( files_[index_] ) );
			return true;
		}
	}

	void FileController::next() {
		if( !files_.empty() ) {
			++index_;
			if( index_ >= files_.size() ) {
				index_ = 0;
			}
			emit openImage( fetch_( files_[index_] ) );
			prefetch_( index_ );
		}
	}

	void FileController::prev() {
		if( !files_.empty() ) {
			--index_;
			if( index_ < 0 ) {
				index_ = files_.size() - 1;
			}
			emit openImage( fetch_( files_[index_] ) );
			prefetch_( index_ );
		}
	}

	inline void FileController::setPrefetchMax( unsigned int pfMax ) {
		prefetchMax_ = ( pfMax > limit_ ) ? limit_ : pfMax;
	}

	inline unsigned int FileController::getPrefetchMax() const {
		return prefetchMax_;
	}

	inline void FileController::setLimit( unsigned int limit ) {
		limit_ = limit;
	}

	inline unsigned int FileController::getLimit() const {
		return limit_;
	}

	QPixmap & FileController::fetch_( const QString & fileName ) {
		QString key = dir_.filePath( fileName );
		if( !cache_.contains( key ) ) {
			cache_.insert( key, QPixmap( key ) );
			history_.enqueue( key );
			if( cache_.size() > limit_ ) {
				cache_.remove( history_.dequeue() );
			}
		}
		return cache_[key];
	}

	void FileController::prefetch_( unsigned int index ) {
		for( unsigned int i = 0; i < prefetchMax_; ++i ) {
			QString key = dir_.filePath( files_[index+i] );
			if( !cache_.contains( key ) ) {
				cache_.insert( key, QPixmap( key ) );
				history_.enqueue( key );
				if( cache_.size() > limit_ ) {
					cache_.remove( history_.dequeue() );
				}
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
