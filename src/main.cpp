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

/**
 * @brief Main entry point
 * @param argc arguments count
 * @param argv arguments
 *
 * Accept all arguments as url.
 */
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
