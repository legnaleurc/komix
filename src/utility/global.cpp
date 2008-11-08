#include "global.hpp"

#include <QString>
#include <QStringList>
#include <QImageReader>

namespace {

	inline QStringList addStar() {
		QStringList temp;
		foreach( QString str, QImageReader::supportedImageFormats() ) {
			temp << str.prepend( "*." );
		}
		return temp;
	}

}

namespace KomiX {

	const QStringList & SupportedFormatsFilter() {
		static QStringList ssf = addStar();
		return ssf;
	}

}
