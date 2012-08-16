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
#include "aboutwidget.hpp"
#include "global.hpp"
#include "mainwindow.hpp"
#include "preference.hpp"
#include "ui_mainwindow.h"

#include <QtCore/QUrl>
#include <QtDebug>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>

using namespace KomiX::widget;

MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags f ) :
QMainWindow( parent, f ),
ui_( new Ui::MainWindow ),
preference_( new Preference( this ) ),
trayIcon_( new QSystemTrayIcon( QIcon( ":/image/logo.svg" ), this ) ),
about_( new AboutWidget( this ) ),
dumpState_( Qt::WindowNoState ) {
	this->ui_->setupUi( this );

	this->setupMenuBar_();
	this->setupCentralWidget_();
	initTrayIcon_();
}

MainWindow::~MainWindow() {
	delete this->ui_;
}

void MainWindow::setupMenuBar_() {
	this->setupFileMenu_();
	this->setupEditMenu_();
	this->setupViewMenu_();
	this->setupGoMenu_();
	this->setupHelpMenu_();
}

void MainWindow::setupFileMenu_() {
	QMenu * fileMenu = this->ui_->menu_File;

	foreach( FileMenuHook hook, getFileMenuHooks() ) {
		QAction * action = hook( this );
		fileMenu->addAction( action );
		addAction( action );
	}
}

void MainWindow::setupEditMenu_() {
	connect( this->ui_->action_Preference, SIGNAL( triggered() ), this->preference_, SLOT( exec() ) );
	connect( this->preference_, SIGNAL( accepted() ), this->ui_->graphicsView, SLOT( loadSettings() ) );
}

void MainWindow::setupViewMenu_() {
	this->addAction( this->ui_->actionSmooth_Next );
	connect( this->ui_->actionSmooth_Next, SIGNAL( triggered() ), this->ui_->graphicsView, SLOT( smoothMove() ) );
	this->addAction( this->ui_->actionSmooth_Previous );
	connect( this->ui_->actionSmooth_Previous, SIGNAL( triggered() ), this->ui_->graphicsView, SLOT( smoothReversingMove() ) );
	this->addAction( this->ui_->actionPage_Head );
	connect( this->ui_->actionPage_Head, SIGNAL( triggered() ), this->ui_->graphicsView, SLOT( begin() ) );
	this->addAction( this->ui_->actionPage_Tail );
	connect( this->ui_->actionPage_Tail, SIGNAL( triggered() ), this->ui_->graphicsView, SLOT( end() ) );
	this->addAction( this->ui_->action_Fullscreen );
	connect( this->ui_->action_Fullscreen, SIGNAL( triggered() ), this, SLOT( toggleFullScreen() ) );
	this->addAction( this->ui_->action_Hide_Window );
	connect( this->ui_->action_Hide_Window, SIGNAL( triggered() ), this, SLOT( toggleSystemTray() ) );
	this->addAction( this->ui_->action_Scale_Image );
	connect( this->ui_->action_Scale_Image, SIGNAL( triggered() ), ui_->graphicsView, SLOT( showControlPanel() ) );
}

void MainWindow::setupGoMenu_() {
	this->addAction( this->ui_->action_Go_To );
	connect( this->ui_->action_Go_To, SIGNAL( triggered() ), this->ui_->graphicsView, SLOT( showNavigator() ) );
	this->addAction( this->ui_->action_Previous_Image );
	connect( this->ui_->action_Previous_Image, SIGNAL( triggered() ), this->ui_->graphicsView, SLOT( previousPage() ) );
	this->addAction( this->ui_->action_Next_Image );
	connect( this->ui_->action_Next_Image, SIGNAL( triggered() ), this->ui_->graphicsView, SLOT( nextPage() ) );
}

void MainWindow::setupHelpMenu_() {
	connect( this->ui_->action_About, SIGNAL( triggered() ), about_, SLOT( show() ) );

	connect( this->ui_->actionAbout_Qt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
}

void MainWindow::setupCentralWidget_() {
	connect( this->ui_->graphicsView, SIGNAL( errorOccured( const QString & ) ), this, SLOT( popupError_( const QString & ) ) );
	connect( this->ui_->graphicsView, SIGNAL( fileDropped( const QUrl & ) ), this, SLOT( open( const QUrl & ) ) );
	connect( this->ui_->graphicsView, SIGNAL( middleClicked() ), this, SLOT( toggleFullScreen() ) );
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
	if( !this->ui_->graphicsView->open( url ) ) {
		QMessageBox::critical( this, tr( "Error" ), tr( "No openable file in this directory." ) );
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
