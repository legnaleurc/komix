/**
 * @file mainwindow.cpp
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
#include "imagearea.hpp"
#include "preference.hpp"
#include "global.hpp"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTabWidget>
#include <QMessageBox>
#include <QApplication>
#include <QtDebug>
#include <QVBoxLayout>

namespace KomiX { namespace widget {

MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags f ) :
QMainWindow( parent, f ),
imageArea_( new ImageArea( this ) ),
preference_( new Preference( this ) ),
trayIcon_( new QSystemTrayIcon( QIcon( ":/image/logo.svg" ), this ) ),
about_( new QWidget( this, Qt::Dialog ) ),
dumpState_( Qt::WindowNoState ) {
	initMenuBar_();
	initCentralWidget_();
	initTrayIcon_();
	initAbout_();

	connect( imageArea_, SIGNAL( errorOccured( const QString & ) ), this, SLOT( popupError_( const QString & ) ) );
}

void MainWindow::initMenuBar_() {
	QMenuBar * menuBar = new QMenuBar( this );

	initFileMenu_( menuBar );
	initEditMenu_( menuBar );
	initViewMenu_( menuBar );
	initGoMenu_( menuBar );
	initHelpMenu_( menuBar );

	setMenuBar( menuBar );
}

void MainWindow::initFileMenu_( QMenuBar * menuBar ) {
	QMenu * fileMenu = new QMenu( tr( "&File" ), menuBar );

	foreach( FileMenuHook hook, getFileMenuHooks() ) {
		QAction * action = hook( this );
		fileMenu->addAction( action );
		addAction( action );
	}

	menuBar->addMenu( fileMenu );
}

void MainWindow::initViewMenu_( QMenuBar * menuBar ) {
	QMenu * view = new QMenu( tr( "&View" ), menuBar );

	QAction * smoothNext = new QAction( tr( "Smooth &Next" ), this );
	smoothNext->setShortcut( Qt::Key_Space );
	connect( smoothNext, SIGNAL( triggered() ), this->imageArea_, SLOT( smoothMove() ) );
	view->addAction( smoothNext );
	addAction( smoothNext );

	QAction * smoothPrev = new QAction( tr( "Smooth &Prev" ), this );
	smoothPrev->setShortcut( Qt::Key_Return );
	connect( smoothPrev, SIGNAL( triggered() ), this->imageArea_, SLOT( reverseSmoothMove() ) );
	view->addAction( smoothPrev );
	addAction( smoothPrev );

	QAction * home = new QAction( tr( "Page &Head" ), this );
	home->setShortcut( Qt::Key_Home );
	connect( home, SIGNAL( triggered() ), this->imageArea_, SLOT( home() ) );
	view->addAction( home );
	addAction( home );

	QAction * end = new QAction( tr( "Page &Tail" ), this );
	end->setShortcut( Qt::Key_End );
	connect( end, SIGNAL( triggered() ), this->imageArea_, SLOT( end() ) );
	view->addAction( end );
	addAction( end );

	view->addSeparator();

	QAction * fullScreen = new QAction( tr( "&Full Screen" ), this );
	fullScreen->setShortcut( Qt::Key_F11 );
	connect( fullScreen, SIGNAL( triggered() ), this, SLOT( toggleFullScreen() ) );
	view->addAction( fullScreen );
	addAction( fullScreen );

	QAction * hide = new QAction( tr( "&Hide window" ), this );
	hide->setShortcut( tr( "Esc" ) );
	connect( hide, SIGNAL( triggered() ), this, SLOT( toggleSystemTray() ) );
	view->addAction( hide );
	addAction( hide );

	view->addSeparator();

	QAction * scale = new QAction( tr( "&Scale image" ), this );
	scale->setShortcut( tr( "Ctrl+S" ) );
	connect( scale, SIGNAL( triggered() ), imageArea_, SLOT( showScalePanel() ) );
	view->addAction( scale );
	addAction( scale );

	menuBar->addMenu( view );
}

void MainWindow::initEditMenu_( QMenuBar * menuBar ) {
	QMenu * edit = new QMenu( tr( "&Edit" ), menuBar );

	QAction * pref = new QAction(  tr( "&Preference" ), this );
	connect( pref, SIGNAL( triggered() ), this->preference_, SLOT( exec() ) );
	connect( this->preference_, SIGNAL( accepted() ), this->imageArea_, SLOT( loadSettings() ) );
	edit->addAction( pref );
	addAction( pref );

	menuBar->addMenu( edit );
}

void MainWindow::initGoMenu_( QMenuBar * menuBar ) {
	QMenu * go = new QMenu( tr( "&Go" ), menuBar );

	QAction * jump = new QAction( tr( "&Go To..." ), this );
	jump->setShortcut( tr( "Ctrl+G" ) );
	connect( jump, SIGNAL( triggered() ), imageArea_, SLOT( showNavigator() ) );
	go->addAction( jump );
	addAction( jump );

	go->addSeparator();

	QAction * prev = new QAction( tr( "&Preverse image" ), this );
	prev->setShortcut( Qt::Key_PageUp );
	connect( prev, SIGNAL( triggered() ), imageArea_, SLOT( prev() ) );
	go->addAction( prev );
	addAction( prev );

	QAction * next = new QAction( tr( "&Next image" ), this );
	next->setShortcut( Qt::Key_PageDown );
	connect( next, SIGNAL( triggered() ), imageArea_, SLOT( next() ) );
	go->addAction( next );
	addAction( next );

	menuBar->addMenu( go );
}

void MainWindow::initHelpMenu_( QMenuBar * menuBar ) {
	QMenu * help = new QMenu( tr( "&Help" ), menuBar );

	QAction * about__ = new QAction( tr( "&About" ), this );
	connect( about__, SIGNAL( triggered() ), about_, SLOT( show() ) );

	help->addAction( about__ );

	QAction * aboutQt_ = new QAction( tr( "About &Qt" ), this );
	connect( aboutQt_, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

	help->addAction( aboutQt_ );

	menuBar->addMenu( help );
}

void MainWindow::initCentralWidget_() {
	setCentralWidget( imageArea_ );

	connect( imageArea_, SIGNAL( fileDroped( const QUrl & ) ), this, SLOT( open( const QUrl & ) ) );
	connect( imageArea_, SIGNAL( requireToogleScreen() ), this, SLOT( toggleFullScreen() ) );
}

void MainWindow::initTrayIcon_() {
	QMenu * menu = new QMenu( this );
	QAction * quit = menu->addAction( tr( "&Quit" ) );
	trayIcon_->setContextMenu( menu );

	trayIcon_->setToolTip( tr( "KomiX" ) );

	connect( quit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );
	connect( trayIcon_, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT( systemTrayHelper_( QSystemTrayIcon::ActivationReason ) ) );

	trayIcon_->show();
}

void MainWindow::initAbout_() {
	about_->setWindowTitle( tr( "About KomiX" ) );

	QVBoxLayout * outer = new QVBoxLayout( about_ );
	about_->setLayout( outer );

	QHBoxLayout * head = new QHBoxLayout();
	outer->addLayout( head );

	QLabel * logo = new QLabel( about_ );
	logo->setPixmap( QPixmap( ":/image/logo.svg" ).scaled( 60, 60 ) );
	head->addWidget( logo );

	QLabel * version = new QLabel( about_ );
	version->setText( tr(
		"<h1>%1</h1>"
		"Version: %2<br/>"
		"<a href=\"http://legnaleurc.blogspot.com/search/label/KomiX/\">More information</a>"
	).arg( QApplication::applicationName() ).arg( QApplication::applicationVersion() ) );
	version->setTextFormat( Qt::RichText );
	head->addWidget( version );

	QTabWidget * tabPages = new QTabWidget( about_ );
	outer->addWidget( tabPages );

	QLabel * about__ = new QLabel( about_ );
	about__->setText( tr(
		"KomiX - A comics viewer\n"
		"\n"
		"(c) 2008-2009 FoolproofProject\n"
		"License: GPLv3\n"
	) );
	tabPages->addTab( about__, tr( "&About" ) );

	QLabel * authors = new QLabel( about_ );
	authors->setText( tr(
		"<h6>Wei-Cheng Pan (FoolproofProject)</h6><br/>"
		"Site: <a href=\"http://legnaleurc.blogspot.com/\">legnaleurc.blogspot.com</a><br/>"
		"E-Mail: <a href=\"mailto:legnaleurc@gmail.com\">legnaleurc@gmail.com</a><br/>"
	) );
	authors->setTextFormat( Qt::RichText );
	tabPages->addTab( authors, tr( "A&uthors" ) );

	QLabel * aboutContent = new QLabel( about_ );
	aboutContent->setPixmap( QPixmap( ":/image/womm.png" ) );
	tabPages->addTab( aboutContent, tr( "&Certification" ) );
}

void MainWindow::systemTrayHelper_( QSystemTrayIcon::ActivationReason reason ) {
	switch( reason ) {
		case QSystemTrayIcon::Trigger:
			toggleSystemTray();
			break;
		default:
			;
	}
}

void MainWindow::open( const QUrl & url ) {
	if( !imageArea_->open( url ) ) {
		QMessageBox::critical( this, tr( "No file to open" ), tr( "No openable file in this directory." ) );
	}
}

void MainWindow::open( const QString & localFile ) {
	if( localFile.isEmpty() ) {
		return;
	}
	this->open( QUrl::fromLocalFile( localFile ) );
}

void MainWindow::popupError_( const QString & errMsg ) {
	QMessageBox::critical( this, tr( "Oops!" ), errMsg );
}

void MainWindow::toggleFullScreen() {
	menuBar()->setVisible( !menuBar()->isVisible() );
	setWindowState( windowState() ^ Qt::WindowFullScreen );
}

void MainWindow::toggleSystemTray() {
	if( isVisible() ) {
		dumpState_ = windowState();
		hide();
	} else {
		show();
		setWindowState( dumpState_ );
	}
}

} } // end namespace
