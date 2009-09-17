#include "global.hpp"

#include <QString>
#include <QImageReader>

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

	inline QStringList addStar( const QStringList & list ) {
		QStringList temp;
		foreach( QString str, list ) {
			temp << str.prepend( "*." );
		}
		return temp;
	}

//	inline QStringList archiveList() {
//		QStringList a;
//		a << "7z";
//		a << "rar";
////		a << "tar.bz2";
////		a << "tbz2";
////		a << "tar.gz";
////		a << "tgz";
//		a << "zip";
//		return a;
//	}

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
		static QStringList sff = addStar( KomiX::SupportedFormats() );
		return sff;
	}

//	const QStringList & ArchiveFormats() {
//		static QStringList af = archiveList();
//		return af;
//	}
//
//	const QStringList & ArchiveFormatsFilter() {
//		static QStringList sff = addStar( ArchiveFormats() );
//		return sff;
//	}
//
//	bool isArchiveSupported( const QString & path ) {
//		foreach( QString suffix, ArchiveFormats() ) {
//			if( path.endsWith( suffix, Qt::CaseInsensitive ) ) {
//				return true;
//			}
//		}
//		return false;
//	}

}
