#include "global.hpp"

#include <QString>
#include <QStringList>
#include <QImageReader>
#include <QtDebug>

#include <algorithm>

namespace {

	inline void tl( QByteArray & s ) {
		s = s.toLower();
	}

	inline QStringList uniqueList() {
		std::list< QByteArray > uniList = QImageReader::supportedImageFormats().toStdList();

		std::for_each( uniList.begin(), uniList.end(), tl );
		uniList.sort();
		uniList.unique();

		QStringList result;

		std::copy( uniList.begin(), uniList.end(), std::back_inserter( result ) );
		return result;
	}

	inline QStringList addStar() {
		QStringList temp;
		foreach( QString str, KomiX::SupportedFormats() ) {
			temp << str.prepend( "*." );
		}
		return temp;
	}

}

namespace KomiX {

	const QStringList & SupportedFormats() {
		static QStringList sf = uniqueList();
		return sf;
	}

	const QStringList & SupportedFormatsFilter() {
		static QStringList ssf = addStar();
		return ssf;
	}

}
