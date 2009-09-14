#include "archivemodel.hpp"
#include "global.hpp"

namespace {

	KomiX::FileModel * create( const QFileInfo & path ) {
		return new KomiX::ArchiveModel( path.absolutePath() );
	}

	bool check( const QFileInfo & path ) {
		if( !path.isDir() ) {
			return KomiX::isArchiveSupported( path.absolutePath() );
		} else {
			return false;
		}
	}

	static const bool registered = KomiX::FileModel::registerModel( check, create );

}

namespace KomiX {

	ArchiveModel::ArchiveModel( const QString & ) {
	}

}
