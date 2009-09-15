#include "filecontroller.hpp"
#include "global.hpp"

#include <QMutexLocker>
#include <QtDebug>
#include <QProcess>
#include <QtGlobal>
#include <QApplication>
#include <QFileInfo>

#include <cstdlib>

namespace KomiX {

	namespace private_ {

		FileController::FileController( QObject * parent ) :
		QObject( parent ),
		dir_( QDir::home() ),
		files_(),
		index_( 0 ),
		lock_() {
		}

		bool FileController::open( const QString & filePath ) {
			QMutexLocker locker( &lock_ );
			// check if filePath has been load, to avoid reload event
			if( !updateModel_( filePath ) ) {
				return false;
			} else {
				// FIXME
				//emit imageLoaded( fileModel_->getImage( 0 ) );
				return true;
			}
		}

		void FileController::next() {
			QMutexLocker locker( &lock_ );
			//if( !fileModel_->isEmpty() ) {
				//++index_;
				//if( index_ >= files_.size() ) {
				//	index_ = 0;
				//}
				// FIXME
				//emit imageLoaded( fileModel_->next() );
				//fetch_( dir_.filePath( files_[ (index_+prefetchMax_ >= files_.size()) ? index_+prefetchMax_-files_.size() : index_+prefetchMax_ ] ) );
			//}
		}

		void FileController::prev() {
			QMutexLocker locker( &lock_ );
			//if( !files_.empty() ) {
			//	--index_;
			//	if( index_ < 0 ) {
			//		index_ = files_.size() - 1;
			//	}
			// FIXME
				//emit imageLoaded( fileModel_->prev() );
			//}
		}

		bool FileController::isEmpty() const {
			return true;
		}

		bool FileController::updateModel_( const QString & filePath ) {
			fileModel_ = FileModel::createModel( QFileInfo( filePath ) );
			//if( fileModel_ && !fileModel_->isEmpty() ) {
			//	return true;
			//} else {
				return false;
			//}
		}

	}

}
