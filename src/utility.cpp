#include "utility.hpp"

#include <QImageReader>
#include <QFileInfo>

namespace KomiX {

	bool isSupport( const QString & fileName ) {
		return QImageReader::supportedImageFormats().contains( QFileInfo( fileName ).suffix().toAscii() );
	}

}
