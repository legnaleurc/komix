#include "directorymodel.hpp"
#include "global.hpp"

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
	files_( root_.entryList( SupportedFormatsFilter(), QDir::Files ) ),
	currentIndex_( 0 ) {
	}

}
