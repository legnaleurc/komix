#include "mainwindow.hpp"

#include <QApplication>

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	QApplication::setWindowIcon( QIcon( ":/image/logo.svg" ) );
	
	KomiX::MainWindow mainWindow;
	mainWindow.setWindowTitle( QObject::tr( "KomiX" ) );
	mainWindow.resize( 800, 600 );
	
	if( argc > 1 ) {
		mainWindow.open( QApplication::arguments().at( 1 ) );
	}
	
	mainWindow.show();
	
	return app.exec();
}
