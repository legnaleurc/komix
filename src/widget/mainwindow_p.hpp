/**
 * @file mainwindow_p.hpp
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
#ifndef KOMIX_WIDGET_MAINWINDOW_P_HPP
#define KOMIX_WIDGET_MAINWINDOW_P_HPP

#include "aboutwidget.hpp"
#include "mainwindow.hpp"
#include "preference.hpp"
#include "ui_mainwindow.h"

#include <QtGui/QSystemTrayIcon>

namespace KomiX {
namespace widget {
class MainWindow::Private: public QObject {
	Q_OBJECT
public:
	Private( MainWindow * owner );

	void initTrayIcon();
	void setupCentralWidget();
	void setupEditMenu();
	void setupFileMenu();
	void setupGoMenu();
	void setupHelpMenu();
	void setupMenuBar();
	void setupViewMenu();

public slots:
	void systemTrayHelper( QSystemTrayIcon::ActivationReason reason );
	void popupError( const QString & errMsg );
	/// toggle full-screen mode
	void toggleFullScreen();
	/// toggle minimize system tray
	void toggleSystemTray();

public:
	MainWindow * owner;
	Ui::MainWindow * ui;
	Preference * preference;
	QSystemTrayIcon * trayIcon;
	AboutWidget * about;
	Qt::WindowStates dumpState;
};
}
}

#endif
