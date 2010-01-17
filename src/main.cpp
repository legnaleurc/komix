/**
 * @file main.cpp
 * @author Wei-Cheng Pan
 *
 * KomiX, a comics viewer.
 * Copyright (C) 2008  Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mainwindow.hpp"

#include <QtCore/QStringList>
#include <QtSingleApplication>
#include <QSettings>

#ifdef KOMIX_STATIC
# include <QtPlugin>

Q_IMPORT_PLUGIN(qgif)
Q_IMPORT_PLUGIN(qico)
Q_IMPORT_PLUGIN(qjpeg)
Q_IMPORT_PLUGIN(qmng)
Q_IMPORT_PLUGIN(qsvg)
Q_IMPORT_PLUGIN(qtiff)
#endif

/// literal to string
#define X(x) XX(x)
/// symbol to literal
#define XX(x) #x

/**
 * @brief Main entry point
 * @param argc arguments count
 * @param argv arguments
 *
 * Accept all arguments as url.
 */
int main( int argc, char * argv[] ) {
	QtSingleApplication app( argc, argv );
	QtSingleApplication::setWindowIcon( QIcon( ":/image/logo.svg" ) );
	QtSingleApplication::setOrganizationName( "FoolproofProject" );
	QtSingleApplication::setApplicationName( "KomiX" );
	QtSingleApplication::setApplicationVersion( X(KOMIX_VERSION) );

	QStringList args( QtSingleApplication::arguments() );

	if( app.sendMessage( "" ) ) {
		if( args.length() > 1 ) {
			app.sendMessage( args.at( 1 ) );
		}
		return 0;
	}

	QSettings::setDefaultFormat( QSettings::IniFormat );

	KomiX::widget::MainWindow mainWindow;
	mainWindow.setWindowTitle( QtSingleApplication::applicationName() );
	mainWindow.resize( 800, 600 );

	if( args.length() > 1 ) {
		mainWindow.open( args.at( 1 ) );
	}

	mainWindow.show();

	QObject::connect( &app, SIGNAL( messageReceived( const QString & ) ), &mainWindow, SLOT( open( const QString & ) ) );
	
	return app.exec();
}
