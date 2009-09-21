#include "directoryhook.hpp"
#include "global.hpp"

#include <QFileDialog>

namespace {

QAction * hookHelper( QWidget * parent ) {
	return new KomiX::model::directory::DirectoryHook( parent );
}

const bool registered = KomiX::registerFileMenuHook( hookHelper );

} // end of namespace

namespace KomiX { namespace model { namespace directory {

DirectoryHook::DirectoryHook( QWidget * parent ) : QAction( parent ) {
	setText( tr( "Open D&irectory" ) );
	setShortcut( tr( "Ctrl+D" ) );

	connect( this, SIGNAL( triggered() ), this, SLOT( helper_() ) );
	connect( this, SIGNAL( opened( const QUrl & ) ), parent, SLOT( open( const QUrl & ) ) );
}

void DirectoryHook::helper_() {
	QString path = QFileDialog::getExistingDirectory( this->parentWidget(), tr( "Open dicrectory" ), QDir::homePath() );
	if( !path.isEmpty() ) {
		emit opened( QUrl::fromLocalFile( path ) );
	}
}

} } } // end of namespace
