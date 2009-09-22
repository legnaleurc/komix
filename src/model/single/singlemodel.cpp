#include "singlemodel.hpp"
#include "global.hpp"

namespace {

bool check( const QUrl & url ) {
	if( url.scheme() == "file" ) {
		QFileInfo fi( url.toLocalFile() );
		if( !fi.isDir() ) {
			QString suffix = fi.suffix().toLower();
			foreach( QString ext, KomiX::SupportedFormats() ) {
				if( suffix == ext ) {
					return true;
				}
			}
		}
	}
	return false;
}

QSharedPointer< KomiX::model::FileModel > create( const QUrl & url ) {
	return QSharedPointer< KomiX::model::FileModel >( new KomiX::model::single::SingleModel( QFileInfo( url.toLocalFile() ) ) );
}

static const bool registered = KomiX::model::FileModel::registerModel( check, create );

} // end of namespace

namespace KomiX { namespace model { namespace single {

SingleModel::SingleModel( const QFileInfo & root ) : LocalFileModel( root.dir() ) {}

} } } // end of namespace
