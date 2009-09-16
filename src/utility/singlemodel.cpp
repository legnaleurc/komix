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

	QModelIndex SingleModel::index() const {
		return createIndex( 0, 0, -1 );
	}

	QModelIndex SingleModel::index( int row, int column, const QModelIndex & parent ) const {
		if( !parent.isValid() ) {
			return QModelIndex();
		}
		switch( parent.column() ) {
		case 0:
			return createIndex( row, 1, row );
		default:
			return QModelIndex();
		}
	}

	QModelIndex SingleModel::parent( const QModelIndex & child ) const {
		if( !child.isValid() ) {
			return QModelIndex();
		}
		switch( child.column() ) {
		case 1:
			if( child.row() >= 0 && child.row() < files_.size() ) {
				return createIndex( 0, 0, -1 );
			} else {
				return QModelIndex();
			}
		default:
			return QModelIndex();
		}
	}

	int SingleModel::rowCount( const QModelIndex & parent ) const {
		if( parent.isValid() && parent.column() == 0 ) {
			return files_.size();
		} else {
			return 0;
		}
	}

	int SingleModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 1;
	}

	QVariant SingleModel::data( const QModelIndex & index, int role ) const {
		if( !index.isValid() ) {
			return QVariant();
		}
		switch( index.column() ) {
		case 1:
			if( index.row() >= 0 && index.row() < files_.size() ) {
				switch( role ) {
				case Qt::DisplayRole:
					return files_[index.row()];
				case Qt::UserRole:
					return QPixmap( root_.filePath( files_[index.row()] ) );
				default:
					return QVariant();
				}
			} else {
				return QVariant();
			}
		default:
			return QVariant();
		}
	}

}
