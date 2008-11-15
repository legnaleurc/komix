#include "filecontroller.hpp"
#include "global.hpp"

#include <QMutexLocker>

namespace KomiX {

	FileControllerBase::FileControllerBase( unsigned int pfMax, unsigned int limit, QObject * parent ) :
	QObject( parent ),
	prefetchMax_( pfMax ),
	limit_( limit ),
	dir_(),
	files_(),
	index_( 0 ),
	history_(),
	cache_(),
	lock() {
	}

	bool FileControllerBase::open( const QString & filePath ) {
		QMutexLocker locker( &lock );
		if( !update_( filePath ) || files_.empty() ) {
			return false;
		} else {
			emit getImage( fetch_( dir_.filePath( files_[index_] ) ) );
			return true;
		}
	}

	void FileControllerBase::next() {
		QMutexLocker locker( &lock );
		if( !files_.empty() ) {
			++index_;
			if( index_ >= files_.size() ) {
				index_ = 0;
			}
			emit getImage( fetch_( dir_.filePath( files_[index_] ) ) );
			prefetch_( index_ );
		}
	}

	void FileControllerBase::prev() {
		QMutexLocker locker( &lock );
		if( !files_.empty() ) {
			--index_;
			if( index_ < 0 ) {
				index_ = files_.size() - 1;
			}
			emit getImage( fetch_( dir_.filePath( files_[index_] ) ) );
			prefetch_( index_ );
		}
	}

	bool FileControllerBase::isEmpty() const {
		return files_.empty();
	}
	
	void FileControllerBase::setPrefetchMax( unsigned int pfMax ) {
		prefetchMax_ = ( pfMax > limit_ ) ? limit_ : pfMax;
	}

	unsigned int FileControllerBase::getPrefetchMax() const {
		return prefetchMax_;
	}

	void FileControllerBase::setLimit( unsigned int limit ) {
		limit_ = limit;
	}

	unsigned int FileControllerBase::getLimit() const {
		return limit_;
	}
	
	QString FileControllerBase::getDirPath() const {
		return dir_.path();
	}
	
	QString FileControllerBase::getFilePath() const {
		return dir_.filePath( files_[index_] );
	}

	const QPixmap & FileControllerBase::getImage( const QString & filePath ) {
		QMutexLocker locker( &lock );
		return fetch_( filePath );
	}

	const QPixmap & FileControllerBase::fetch_( const QString & key ) {
		if( !cache_.contains( key ) ) {
			cache_.insert( key, QPixmap( key ) );
			history_.enqueue( key );
			if( static_cast< unsigned int >( cache_.size() ) > limit_ ) {
				cache_.remove( history_.dequeue() );
			}
		}
		return cache_[key];
	}

	void FileControllerBase::prefetch_( unsigned int index ) {
		for( unsigned int i = 0; i < prefetchMax_; ++i ) {
			fetch_( dir_.filePath( files_[index+i] ) );
		}
	}

	bool FileControllerBase::update_( const QString & filePath ) {
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
