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

	QModelIndex DirectoryModel::index( int row, int /*column*/, const QModelIndex & /*parent*/ ) const {
		if( row < 0 || row >= files_.size() ) {
			return QModelIndex();
		} else {
			// FIXME: parameter 3 needs change
			return createIndex( row, 0, row );
		}
	}

	QModelIndex DirectoryModel::parent( const QModelIndex & /*child*/ ) const {
		return QModelIndex();
	}

	int DirectoryModel::rowCount( const QModelIndex & /*parent*/ ) const {
		return files_.size();
	}

	int DirectoryModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 1;
	}

	QVariant DirectoryModel::data( const QModelIndex & index, int role ) const {
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
