#include "mainwindow.hpp"

#include <QApplication>
#include <QUrl>

#ifdef KOMIX_STATIC
# include <QtPlugin>

Q_IMPORT_PLUGIN(qgif)
Q_IMPORT_PLUGIN(qico)
Q_IMPORT_PLUGIN(qjpeg)
Q_IMPORT_PLUGIN(qmng)
Q_IMPORT_PLUGIN(qsvg)
Q_IMPORT_PLUGIN(qtiff)
#endif

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	QApplication::setWindowIcon( QIcon( ":/image/logo.svg" ) );
	
	KomiX::widget::MainWindow mainWindow;
	mainWindow.setWindowTitle( QObject::tr( "KomiX" ) );
	mainWindow.resize( 800, 600 );
	
	if( argc > 1 ) {
		mainWindow.open( QUrl::fromLocalFile( QApplication::arguments().at( 1 ) ) );
	}
	
	mainWindow.show();
	
	return app.exec();
}
