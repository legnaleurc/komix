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

	static inline QMutex * lock() {
		static QMutex m;
		return &m;
	}

}

namespace KomiX {

	namespace private_ {

		FileController::FileController( QObject * parent ) :
		QObject( parent ),
		dir_( QDir::home() ),
		index_( 0 ),
		model_( NULL ) {
		}

		bool FileController::open( const QString & filePath ) {
			QMutexLocker locker( ::lock() );
			model_ = FileModel::createModel( QFileInfo( filePath ) );
			if( isEmpty() ) {
				return false;
			} else {
				index_ = 0;
				QModelIndex first = model_->index( index_, 0 );
				emit imageLoaded( first.data( Qt::UserRole ).value< QPixmap >() );
				return true;
			}
		}

		void FileController::next() {
			QMutexLocker locker( ::lock() );
			if( !isEmpty() ) {
				++index_;
				if( index_ >= model_->rowCount() ) {
					index_ = 0;
				}
				// FIXME
				QModelIndex item = model_->index( index_, 0 );
				emit imageLoaded( item.data( Qt::UserRole ).value< QPixmap >() );
				//fetch_( dir_.filePath( files_[ (index_+prefetchMax_ >= files_.size()) ? index_+prefetchMax_-files_.size() : index_+prefetchMax_ ] ) );
			}
		}

		void FileController::prev() {
			QMutexLocker locker( ::lock() );
			if( !isEmpty() ) {
				--index_;
				if( index_ < 0 ) {
					index_ = model_->rowCount() - 1;
				}
				// FIXME
				QModelIndex item = model_->index( index_, 0 );
				emit imageLoaded( item.data( Qt::UserRole ).value< QPixmap >() );
			}
		}

		bool FileController::isEmpty() const {
			if( !model_ ) {
				return true;
			}
			return model_->rowCount() == 0;
		}

		FileModel * FileController::getFileModel() const {
			return model_;
		}

	}

}
