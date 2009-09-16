#include "filecontroller.hpp"

#include <QMutexLocker>
#include <QFileInfo>
#include <QMutex>

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
		index_( 0 ),
		model_( NULL ) {
		}

		bool FileController::open( const QString & filePath ) {
			QMutexLocker locker( ::lock() );
			model_ = FileModel::createModel( QFileInfo( filePath ) );
			if( isEmpty() ) {
				return false;
			} else {
				QModelIndex first = model_->index( filePath );
				if( first.isValid() ) {
					index_ = first.row();
				} else {
					first = model_->index( 0, 0 );
					index_ = 0;
				}
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
