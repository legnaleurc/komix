#include "filecontroller.hpp"
#include "global.hpp"

#include <QMutexLocker>
#include <QtDebug>
#include <QProcess>
#include <QtGlobal>
#include <QApplication>
#include <QFileInfo>
#include <QMutex>

#include <cstdlib>

namespace {

	static QMutex lock;

}

namespace KomiX {

	namespace private_ {

		FileController::FileController( QObject * parent ) :
		QObject( parent ),
		dir_( QDir::home() ),
		index_( 0 ) {
		}

		bool FileController::open( const QString & filePath ) {
			QMutexLocker locker( &lock );
			model_ = FileModel::createModel( QFileInfo( filePath ) );
			if( !model_ ) {
				return false;
			} else {
				// FIXME
				//emit imageLoaded( fileModel_->getImage( 0 ) );
				return true;
			}
		}

		void FileController::next() {
			QMutexLocker locker( &lock );
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
			QMutexLocker locker( &lock );
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
			if( !model_ ) {
				return true;
			}
			return model_->rowCount() == 0;
		}

	}

}
