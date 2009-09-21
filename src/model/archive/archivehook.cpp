#include "archivehook.hpp"
#include "archivemodel.hpp"
#include "global.hpp"

#include <QFileDialog>
#include <QApplication>

namespace {

inline QString archiveList() {
	QStringList tmp( QObject::tr( "All Supported Archives ( %1 )" ).arg( KomiX::model::archive::ArchiveFormatsFilter().join( " " ) ) );
	tmp << KomiX::model::archive::ArchiveFormatsFilter();
	return tmp.join( ";;" );
}

QAction * hookHelper( QWidget * parent ) {
	return new KomiX::model::archive::ArchiveHook( parent );
}

static const bool registered = KomiX::registerFileMenuHook( hookHelper );

} // end of namespace

namespace KomiX { namespace model { namespace archive {

ArchiveHook::ArchiveHook( QWidget * parent ) : QAction( parent ) {
	setText( tr( "Open A&rchive" ) );
	setShortcut( tr( "Ctrl+A" ) );

	connect( this, SIGNAL( triggered() ), this, SLOT( helper_() ) );
	connect( this, SIGNAL( opened( const QUrl & ) ), parent, SLOT( open( const QUrl & ) ) );
	// for cleanup, delete temp dir
	connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( cleanup_() ) );
}

void ArchiveHook::helper_() {
	QString path = QFileDialog::getOpenFileName( this->parentWidget(), tr( "Open archive" ), QDir::homePath(), archiveFilter_() );
	if( !path.isEmpty() ) {
		emit opened( QUrl::fromLocalFile( path ) );
	}
}

void ArchiveHook::cleanup_() {
	delTree( ArchiveModel::TmpDir_() );
}

inline const QString & ArchiveHook::archiveFilter_() {
	static QString af = archiveList();
	return af;
}

} } } // end of namespace
