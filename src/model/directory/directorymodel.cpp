#include "directorymodel.hpp"

namespace {

bool check( const QUrl & url ) {
	if( url.scheme() == "file" ) {
		QFileInfo fi( url.toLocalFile() );
		return fi.isDir();
	}
	return false;
}

QSharedPointer< KomiX::model::FileModel > create( const QUrl & url ) {
	return QSharedPointer< KomiX::model::FileModel >( new KomiX::model::directory::DirectoryModel( QFileInfo( url.toLocalFile() ) ) );
}

static const bool registered = KomiX::model::FileModel::registerModel( check, create );

} // end of namespace

namespace KomiX { namespace model { namespace directory {

DirectoryModel::DirectoryModel( const QFileInfo & root ): LocalFileModel( root.absoluteFilePath() ) {}

} } } // end of namespace
