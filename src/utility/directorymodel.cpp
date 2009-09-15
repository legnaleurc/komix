#include "directorymodel.hpp"
#include "global.hpp"

#include <QPixmap>

namespace {

	KomiX::FileModel * create( const QFileInfo & path ) {
		return new KomiX::DirectoryModel( path );
	}

	bool check( const QFileInfo & path ) {
		return path.isDir();
	}

	static const bool registered = KomiX::FileModel::registerModel( check, create );

}

namespace KomiX {

	DirectoryModel::DirectoryModel( const QFileInfo & root ):
	root_( root.absoluteFilePath() ),
	files_( root_.entryList( SupportedFormatsFilter(), QDir::Files ) ) {
	}

	QModelIndex DirectoryModel::index( int row, int column, const QModelIndex & /*parent*/ ) const {
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

	QModelIndex DirectoryModel::parent( const QModelIndex & child ) const {
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

	int DirectoryModel::rowCount( const QModelIndex & parent ) const {
		if( !parent.isValid() || parent.column() == 0 ) {
			return files_.size();
		} else {
			return 0;
		}
	}

	int DirectoryModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 0;
	}

	QVariant DirectoryModel::data( const QModelIndex & index, int role ) const {
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

	bool DirectoryModel::hasChildren( const QModelIndex & parent ) const {
		if( !parent.isValid() || parent.column() == 0 ) {
			return !files_.empty();
		} else {
			return false;
		}
	}

}
