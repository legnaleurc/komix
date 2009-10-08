/**
 * @file mainwindow.hpp
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
#ifndef KOMIX_WIDGET_MAINWINDOW_HPP
#define KOMIX_WIDGET_MAINWINDOW_HPP

#include <QMainWindow>
#include <QDir>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QModelIndex>

namespace KomiX { namespace widget {

class ImageArea;
//class ScaleImage;
//class Preview;

/**
 * @brief Main window
 *
 * The main window widget, all GUI components are managed by this.
 */
class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	/**
	 * @brief default constructor
	 * @param parent parent widget
	 * @param f window flags
	 */
	MainWindow( QWidget * parent = 0, Qt::WindowFlags f = 0 );

public slots:
	/**
	 * @brief open file
	 * @param filePath file path
	 */
	void open( const QUrl & url );
	/// toggle full-screen mode
	void toggleFullScreen();
	/// toggle minimize system tray
	void toggleSystemTray();

private slots:
	void systemTrayHelper_( QSystemTrayIcon::ActivationReason reason );
	void popupError_( const QString & errMsg );

private:
	void initMenuBar_();
	void initCentralWidget_();
	void initTrayIcon_();
	void initAbout_();

	ImageArea * imageArea_;
//	ScaleImage * scaleImage_;
//	Preview * preview_;
	QSystemTrayIcon * trayIcon_;
	QWidget * about_;
	Qt::WindowStates dumpState_;
};
	
} } // end namespace

#endif
