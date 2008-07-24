#include "utility.hpp"
#include "widget/mainwindow.hpp"
#include <QApplication>

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	
	KomiX::MainWindow mainWindow;
	mainWindow.setWindowTitle( QObject::tr( "KomiX" ) );
	mainWindow.resize( 800, 600 );
	
	if( argc > 1 ) {
		foreach( QString arg, app.arguments().mid( 1 ) ) {
			if( KomiX::isSupport( arg ) ) {
				mainWindow.open( arg );
				break;
			}
		}
	}
	
	mainWindow.show();
	
	return app.exec();
}
