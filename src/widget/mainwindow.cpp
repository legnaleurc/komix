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
/**
 * @class KomiX::widget::MainWindow
 * @brief Main window
 *
 * The main window widget, all GUI components are managed by this.
 */

#include "global.hpp"
#include "mainwindow_p.hpp"

#include <QtCore/QtDebug>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>

using KomiX::widget::MainWindow;

MainWindow::Private::Private( MainWindow * owner ):
QObject(),
owner( owner ),
ui(),
controller( new FileController( this ) ),
navigator( new Navigator( this->controller, owner ) ),
preference( new Preference( owner ) ),
trayIcon( new QSystemTrayIcon( QIcon( ":/image/logo.svg" ), owner ) ),
about( new AboutWidget( owner ) ),
dumpState( Qt::WindowNoState ) {
	this->connect( this->controller, SIGNAL( errorOccured( const QString & ) ), SLOT( popupError( const QString & ) ) );
}

void MainWindow::Private::setupMenuBar() {
	this->setupFileMenu();
	this->setupEditMenu();
	this->setupViewMenu();
	this->setupGoMenu();
	this->setupHelpMenu();
}

void MainWindow::Private::setupFileMenu() {
	QMenu * fileMenu = this->ui.menu_File;

	foreach( FileMenuHook hook, getFileMenuHooks() ) {
		QAction * action = hook( this->owner );
		fileMenu->addAction( action );
		this->owner->addAction( action );
	}
}

void MainWindow::Private::setupEditMenu() {
	this->preference->connect( this->ui.action_Preference, SIGNAL( triggered() ), SLOT( exec() ) );
	this->ui.graphicsView->connect( this->preference, SIGNAL( accepted() ), SLOT( loadSettings() ) );
}

void MainWindow::Private::setupViewMenu() {
	this->owner->addAction( this->ui.actionSmooth_Next );
	this->ui.graphicsView->connect( this->ui.actionSmooth_Next, SIGNAL( triggered() ), SLOT( smoothMove() ) );

	this->owner->addAction( this->ui.actionSmooth_Previous );
	this->ui.graphicsView->connect( this->ui.actionSmooth_Previous, SIGNAL( triggered() ), SLOT( smoothReversingMove() ) );

	this->owner->addAction( this->ui.actionPage_Head );
	this->ui.graphicsView->connect( this->ui.actionPage_Head, SIGNAL( triggered() ), SLOT( begin() ) );

	this->owner->addAction( this->ui.actionPage_Tail );
	this->ui.graphicsView->connect( this->ui.actionPage_Tail, SIGNAL( triggered() ), SLOT( end() ) );

	this->owner->addAction( this->ui.action_Fullscreen );
	this->connect( this->ui.action_Fullscreen, SIGNAL( triggered() ), SLOT( toggleFullScreen() ) );

	this->owner->addAction( this->ui.action_Hide_Window );
	this->connect( this->ui.action_Hide_Window, SIGNAL( triggered() ), SLOT( toggleSystemTray() ) );

	this->owner->addAction( this->ui.action_Scale_Image );
	this->ui.graphicsView->connect( this->ui.action_Scale_Image, SIGNAL( triggered() ), SLOT( showControlPanel() ) );
}

void MainWindow::Private::setupGoMenu() {
	this->owner->addAction( this->ui.action_Go_To );
	this->connect( this->ui.action_Go_To, SIGNAL( triggered() ), SLOT( showNavigator() ) );

	this->owner->addAction( this->ui.action_Previous_Image );
	this->ui.graphicsView->connect( this->ui.action_Previous_Image, SIGNAL( triggered() ), SLOT( previousPage() ) );

	this->owner->addAction( this->ui.action_Next_Image );
	this->ui.graphicsView->connect( this->ui.action_Next_Image, SIGNAL( triggered() ), SLOT( nextPage() ) );
}

void MainWindow::Private::setupHelpMenu() {
	this->about->connect( this->ui.action_About, SIGNAL( triggered() ), SLOT( show() ) );

	qApp->connect( this->ui.actionAbout_Qt, SIGNAL( triggered() ), SLOT( aboutQt() ) );
}

void MainWindow::Private::setupCentralWidget() {
	this->owner->connect( this->ui.graphicsView, SIGNAL( fileDropped( const QUrl & ) ), SLOT( open( const QUrl & ) ) );
	this->connect( this->ui.graphicsView, SIGNAL( middleClicked() ), SLOT( toggleFullScreen() ) );
}

void MainWindow::Private::initTrayIcon() {
	QMenu * menu = new QMenu( this->owner );
	QAction * quit = menu->addAction( tr( "&Quit" ) );
	this->trayIcon->setContextMenu( menu );

	this->trayIcon->setToolTip( tr( "KomiX" ) );

	qApp->connect( quit, SIGNAL( triggered() ), SLOT( quit() ) );
	this->connect( this->trayIcon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), SLOT( systemTrayHelper( QSystemTrayIcon::ActivationReason ) ) );

	this->trayIcon->show();
}

void MainWindow::Private::systemTrayHelper( QSystemTrayIcon::ActivationReason reason ) {
	switch( reason ) {
		case QSystemTrayIcon::Trigger:
			toggleSystemTray();
			break;
		default:
			;
	}
}

void MainWindow::Private::popupError( const QString & errMsg ) {
	QMessageBox::critical( this->owner, tr( "Oops!" ), errMsg );
}

void MainWindow::Private::toggleFullScreen() {
	this->owner->menuBar()->setVisible( !this->owner->menuBar()->isVisible() );
	this->owner->setWindowState( this->owner->windowState() ^ Qt::WindowFullScreen );
}

void MainWindow::Private::toggleSystemTray() {
	if( this->owner->isVisible() ) {
		this->dumpState = this->owner->windowState();
		this->owner->hide();
	} else {
		this->owner->show();
		this->owner->setWindowState( this->dumpState );
	}
}

void MainWindow::Private::showNavigator() {
	if( this->controller->isEmpty() ) {
		this->popupError( tr( "No openable file." ) );
		return;
	}
	this->navigator->setModel( this->controller->getModel() );
	this->navigator->setCurrentIndex( this->controller->getCurrentIndex() );
	this->navigator->exec();
}

/**
 * @brief default constructor
 * @param parent parent widget
 * @param f window flags
 */
MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags f ) :
QMainWindow( parent, f ),
p_( new Private( this ) ) {
	this->p_->ui.setupUi( this );

	this->p_->ui.graphicsView->initialize( this->p_->controller );

	this->p_->setupMenuBar();
	this->p_->setupCentralWidget();
	this->p_->initTrayIcon();
}

/**
 * @brief open url
 * @param url file url
 */
void MainWindow::open( const QUrl & url ) {
	if( !this->p_->ui.graphicsView->open( url ) ) {
		QMessageBox::critical( this, tr( "Error" ), tr( "No openable file in this directory." ) );
	}
}

void MainWindow::open( const QString & localFile ) {
	if( localFile.isEmpty() ) {
		return;
	}
	this->open( QUrl::fromLocalFile( localFile ) );
}
