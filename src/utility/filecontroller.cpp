#include "filecontroller.hpp"
#include "global.hpp"

#include <QMutexLocker>
#include <QtDebug>
#include <QProcess>
#include <QtGlobal>
#include <QApplication>
#include <QFileInfo>

#include <cstdlib>

namespace {

	//QDir createTmpDir() {
	//	qsrand( qApp->applicationPid() );
	//	QString tmpPath( QString( "komix_%1" ).arg( qrand() ) );
	//	QDir tmpDir( QDir::temp() );
	//	if( !tmpDir.mkdir( tmpPath ) ) {
	//		qWarning( "can not make temp dir" );
	//	} else {
	//		tmpDir.cd( tmpPath );
	//	}
	//	return tmpDir;
	//}

	//int rmdir( QDir dir ) {
	//	int sum = 0;
	//	QFileInfoList entry = dir.entryInfoList( QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs );
	//	foreach( QFileInfo e, entry ) {
	//		if( e.isDir() ) {
	//			sum += rmdir( e.absoluteFilePath() );
	//		} else {
	//			if( QFile::remove( e.absoluteFilePath() ) ) {
	//				++sum;
	//			}
	//		}
	//	}
	//	dir.rmdir( dir.absolutePath() );
	//	return sum + 1;
	//}

}

namespace KomiX {

	namespace private_ {

		//const QDir FileController::TmpDir_ = createTmpDir();

		FileController::FileController( /*int pfMax, int limit,*/ QObject * parent ) :
		QObject( parent ),
		//prefetchMax_( pfMax ),
		//limit_( limit ),
		dir_( QDir::home() ),
		files_(),
		index_( 0 ),
		//history_(),
		//cache_(),
		lock_() {
			//if( limit_ < 0 ) {
			//	limit_ = 0;
			//}
			//if( prefetchMax_ < 0 ) {
			//	prefetchMax_ = 0;
			//} else if( prefetchMax_ > limit_ ) {
			//	prefetchMax_ = limit_;
			//}
		}

		//FileController::~FileController() {
		//	int ret = rmdir( TmpDir_ );
		//	qDebug() << ret;
		//}

		bool FileController::open( const QString & filePath ) {
			QMutexLocker locker( &lock_ );
			// check if filePath has been load, to avoid reload event
			if( !updateModel_( filePath ) ) {
				return false;
			} else {
				emit imageLoaded( fileModel_->getImage( 0 ) );
				return true;
			}
		}

		void FileController::next() {
			QMutexLocker locker( &lock_ );
			if( !fileModel_->isEmpty() ) {
				//++index_;
				//if( index_ >= files_.size() ) {
				//	index_ = 0;
				//}
				emit imageLoaded( fileModel_->next() );
				//fetch_( dir_.filePath( files_[ (index_+prefetchMax_ >= files_.size()) ? index_+prefetchMax_-files_.size() : index_+prefetchMax_ ] ) );
			}
		}

		void FileController::prev() {
			QMutexLocker locker( &lock_ );
			//if( !files_.empty() ) {
			//	--index_;
			//	if( index_ < 0 ) {
			//		index_ = files_.size() - 1;
			//	}
				emit imageLoaded( fileModel_->prev() );
			//}
		}

		bool FileController::isEmpty() const {
			return files_.empty();
		}

		//void FileController::setPrefetchMax( int pfMax ) {
		//	prefetchMax_ = ( pfMax > limit_ ? limit_ : pfMax );
		//}

		//int FileController::getPrefetchMax() const {
		//	return prefetchMax_;
		//}

		//void FileController::setLimit( int limit ) {
		//	limit_ = ( limit < 0 ? 0 : limit );
		//}

		//int FileController::getLimit() const {
		//	return limit_;
		//}

		//QString FileController::getDirPath() const {
		//	return dir_.path();
		//}

		//QString FileController::getFilePath() const {
		//	return dir_.filePath( files_[index_] );
		//}

		//const QPixmap & FileController::getImage( const QString & filePath ) {
		//	QMutexLocker locker( &lock_ );
		//	return fetch_( filePath );
		//}

	//	const QPixmap & FileController::fetch_( const QString & key ) {
	//		if( !cache_.contains( key ) ) {
	//// 			qDebug() << "Cache miss:" << key;
	//			cache_.insert( key, QPixmap( key ) );
	//			history_.enqueue( key );
	//			if( cache_.size() > limit_ ) {
	//				cache_.remove( history_.dequeue() );
	//			}
	//		} else {
	//// 			qDebug() << "Cache hit:" << key;
	//		}
	//		return cache_[key];
	//	}

	//	void FileController::prefetch_( int index ) {
	//// 		qDebug( "<FileController::prefetch_>" );
	//		for( int i = 1; i <= prefetchMax_; ++i ) {
	//			if( index + i >= files_.size() ) {
	//				index -= files_.size();
	//			}
	//			fetch_( dir_.filePath( files_[index+i] ) );
	//		}
	//// 		qDebug( "</FileController::prefetch_>" );
	//	}

		bool FileController::updateModel_( const QString & filePath ) {
			fileModel_ = FileModel::createModel( QFileInfo( filePath ) );
			if( fileModel_ && !fileModel_->isEmpty() ) {
				return true;
			} else {
				return false;
			}
		}

	}

}
