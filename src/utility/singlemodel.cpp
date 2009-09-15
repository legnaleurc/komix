#include "singlemodel.hpp"
#include "global.hpp"

#include <QPixmap>

namespace {

	bool check( const QFileInfo & path ) {
		if( !path.isDir() ) {
			return true;
		} else {
			return false;
		}
	}

	KomiX::FileModel * create( const QFileInfo & path ) {
		return new KomiX::SingleModel( path );
	}

	static const bool registered = KomiX::FileModel::registerModel( check, create );

}

namespace KomiX {

	SingleModel::SingleModel( const QFileInfo & root ):
	root_( root.dir() ),
	files_( root_.entryList( SupportedFormatsFilter(), QDir::Files ) ) {
	}

	QModelIndex SingleModel::index( int row, int /*column*/, const QModelIndex & /*parent*/ ) const {
		if( row < 0 || row >= files_.size() ) {
			return QModelIndex();
		} else {
			// FIXME: parameter 3 needs change
			return createIndex( row, 0, row );
		}
	}

	QModelIndex SingleModel::parent( const QModelIndex & /*child*/ ) const {
		return QModelIndex();
	}

	int SingleModel::rowCount( const QModelIndex & /*parent*/ ) const {
		return files_.size();
	}

	int SingleModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 1;
	}

	QVariant SingleModel::data( const QModelIndex & index, int role ) const {
		if( !index.isValid() || index.row() < 0 || index.row() >= files_.size() ) {
			return QVariant();
		} else {
			switch( role ) {
			case Qt::DisplayRole:
				return files_[index.row()];
			case Qt::UserRole:
				return QPixmap( root_.filePath( files_[index.row()] ) );
			default:
				return QVariant();
			}
		}
	}

}
