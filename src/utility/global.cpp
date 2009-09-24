#include "global.hpp"

#include <QString>
#include <QImageReader>
#include <QtGlobal>
#include <QCoreApplication>

#include <algorithm>

namespace {

	inline void tl( QByteArray & s ) {
		s = s.toLower();
	}

	inline QStringList uniqueList() {
		Q_ASSERT( QCoreApplication::instance() != NULL );
		std::list< QByteArray > uniList = QImageReader::supportedImageFormats().toStdList();

		std::for_each( uniList.begin(), uniList.end(), tl );
		uniList.sort();
		uniList.unique();

		QStringList result;

		std::copy( uniList.begin(), uniList.end(), std::back_inserter( result ) );
		return result;
	}

}

namespace KomiX {

	std::list< FileMenuHook > & fileMenuHooks() {
		static std::list< FileMenuHook > hooks;
		return hooks;
	}

	const std::list< FileMenuHook > & getFileMenuHooks() {
		return fileMenuHooks();
	}

	bool registerFileMenuHook( FileMenuHook hook ) {
		fileMenuHooks().push_back( hook );
		return true;
	}

	const QStringList & SupportedFormats() {
		static QStringList sf = uniqueList();
		return sf;
	}

	const QStringList & SupportedFormatsFilter() {
		static QStringList sff = toNameFilter( KomiX::SupportedFormats() );
		return sff;
	}

	QStringList toNameFilter( const QStringList & exts ) {
		QStringList tmp;
		foreach( QString str, exts ) {
			tmp << str.prepend( "*." );
		}
		return tmp;
	}

}
