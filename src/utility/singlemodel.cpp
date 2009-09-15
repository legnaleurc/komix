#include "singlemodel.hpp"
#include "global.hpp"

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

	SingleModel::SingleModel( const QFileInfo & root ) {
		dir_ = root.dir();
		files_ = dir_.entryList( SupportedFormatsFilter(), QDir::Files );
		index_ = files_.indexOf( root.fileName() );
	}

}
