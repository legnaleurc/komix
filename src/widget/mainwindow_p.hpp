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


#include "mainwindow.hpp"

#include <QProgressDialog>

#include "aboutwidget.hpp"
#include "filecontroller.hpp"
#include "navigator.hpp"
#include "ui_mainwindow.h"


namespace KomiX {
namespace widget {

class MainWindow::Private : public QObject {
    Q_OBJECT
public:
    Private(MainWindow * owner);

    void setupCentralWidget();
    void setupFileMenu();
    void setupGoMenu();
    void setupHelpMenu();
    void setupMenuBar();
    void setupProgressDialog();
    void setupViewMenu();

public slots:
    void showOpenDialog();
    void showOpenDirectoryDialog();
    void showNavigator();
    void onProgressUpdated(int current, int total);
    void popupError(const QString & errMsg);
    /// toggle full-screen mode
    void toggleFullScreen();

public:
    MainWindow * owner;
    Ui::MainWindow ui;
    QProgressDialog * progress;
    Navigator * navigator;
    AboutWidget * about;
    Qt::WindowStates dumpState;
};

}
}

#endif
