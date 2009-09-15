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

	QModelIndex SingleModel::index( int row, int column, const QModelIndex & /*parent*/ ) const {
		switch( column ) {
		case 0:
			return createIndex( 0, column, -1 );
			break;
		case 1:
			if( row < 0 || row >= files_.size() ) {
				return QModelIndex();
			} else {
				// FIXME: parameter 3 needs change
				return createIndex( row, column, row );
			}
		default:
			return QModelIndex();
		}
	}

	QModelIndex SingleModel::parent( const QModelIndex & child ) const {
		switch( child.column() ) {
		case 0:
			break;
		case 1:
			if( child.row() >= 0 && child.row() < files_.size() ) {
				return createIndex( 0, 0, -1 );
			}
			break;
		}
		return QModelIndex();
	}

	int SingleModel::rowCount( const QModelIndex & parent ) const {
		if( !parent.isValid() || parent.column() == 0 ) {
			return files_.size();
		} else {
			return 0;
		}
	}

	int SingleModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 0;
	}

	QVariant SingleModel::data( const QModelIndex & index, int role ) const {
		switch( index.column() ) {
		case 0:
			break;
		case 1:
			if( index.isValid() && index.row() >= 0 && index.row() < files_.size() ) {
				switch( role ) {
				case Qt::DisplayRole:
					return files_[index.row()];
				case Qt::UserRole:
					return QPixmap( root_.filePath( files_[index.row()] ) );
				}
			}
			break;
		}
		return QVariant();
	}

	bool SingleModel::hasChildren( const QModelIndex & parent ) const {
		if( !parent.isValid() || parent.column() == 0 ) {
			return !files_.empty();
		} else {
			return false;
		}
	}

}
