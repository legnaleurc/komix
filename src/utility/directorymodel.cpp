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

	QModelIndex DirectoryModel::index( const QString & name ) const {
		int row = files_.indexOf( QFileInfo( name ).fileName() );
		return ( row < 0 ) ? QModelIndex() : createIndex( row, 0, row );
	}


	QModelIndex DirectoryModel::index( int row, int column, const QModelIndex & parent ) const {
		if( !parent.isValid() ) {
			// query from root
			if( column == 0 && row >= 0 && row < files_.size() ) {
				return createIndex( row, 0, row );
			} else {
				return QModelIndex();
			}
		} else {
			// other node has no child
			return QModelIndex();
		}
	}

	QModelIndex DirectoryModel::parent( const QModelIndex & child ) const {
		if( !child.isValid() ) {
			// root has no parent
			return QModelIndex();
		} else {
			if( child.column() == 0 && child.row() >= 0 && child.row() < files_.size() ) {
				return QModelIndex();
			} else {
				return QModelIndex();
			}
		}
	}

	int DirectoryModel::rowCount( const QModelIndex & parent ) const {
		if( !parent.isValid() ) {
			// root row size
			return files_.size();
		} else {
			// others are leaf
			return 0;
		}
	}

	int DirectoryModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 1;
	}

	QVariant DirectoryModel::data( const QModelIndex & index, int role ) const {
		if( !index.isValid() ) {
			return QVariant();
		}
		switch( index.column() ) {
		case 0:
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
