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
#include "literal.hpp"
#include "filemodel.hpp"

#include <QtCore/QtDebug>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>


using KomiX::widget::MainWindow;
using KomiX::model::FileModel;


/**
 * @brief default constructor
 * @param parent parent widget
 * @param f window flags
 */
MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags f)
    : QMainWindow(parent, f)
    , p_(new Private(this)) {
}


MainWindow::Private::Private(MainWindow * owner)
    : QObject()
    , owner(owner)
    , ui()
    , progress(new QProgressDialog(owner))
    , navigator(new Navigator(owner))
    , about(new AboutWidget(owner))
    , dumpState(Qt::WindowNoState)
{
    this->ui.setupUi(this->owner);

    this->setupMenuBar();
    this->setupCentralWidget();
    //this->initTrayIcon();
    this->setupProgressDialog();

    auto & fc = Global::instance().getFileController();
    this->connect(&fc,
                  SIGNAL(errorOccured(const QString &)),
                  SLOT(popupError(const QString &)));
    this->connect(&fc,
                  SIGNAL(progressUpdated(int, int)),
                  SLOT(onProgressUpdated(int, int)));
    this->connect(&fc,
                  SIGNAL(modelReady()),
                  SLOT(onModelReady()));
}


void MainWindow::Private::setupMenuBar() {
    this->setupFileMenu();
    this->setupViewMenu();
    this->setupGoMenu();
    this->setupHelpMenu();
}


void MainWindow::Private::setupFileMenu() {
    this->connect(this->ui.action_Open, SIGNAL(triggered()), SLOT(showOpenDialog()));
    this->connect(this->ui.actionOpen_Directory, SIGNAL(triggered()), SLOT(showOpenDirectoryDialog()));
}


void MainWindow::Private::setupViewMenu() {
    this->connect(this->ui.action_Fullscreen, SIGNAL(triggered()), SLOT(toggleFullScreen()));
}


void MainWindow::Private::setupGoMenu() {
    this->connect(this->ui.action_Go_To, SIGNAL(triggered()), SLOT(showNavigator()));
}


void MainWindow::Private::setupHelpMenu() {
    this->about->connect(this->ui.action_About, SIGNAL(triggered()), SLOT(show()));
    qApp->connect(this->ui.actionAbout_Qt, SIGNAL(triggered()), SLOT(aboutQt()));
}


void MainWindow::Private::setupCentralWidget() {
    this->connect(this->ui.graphicsView, SIGNAL(middleClicked()), SLOT(toggleFullScreen()));
}


void MainWindow::Private::setupProgressDialog() {
    this->progress->setModal(true);
    this->progress->setCancelButton(nullptr);
    auto flags = this->progress->windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags &= ~Qt::WindowCloseButtonHint;
    this->progress->setWindowFlags(flags);

    // HACK Reset it immediately or it will popup after minimumDuration.
    // Probably a Qt bug.
    this->progress->reset();
}


void MainWindow::Private::popupError(const QString & errMsg) {
    this->progress->reset();
    QMessageBox::critical(this->owner, MAINWINDOW_ERROR_DIALOG_TITLE, errMsg);
}


void MainWindow::Private::toggleFullScreen() {
    this->owner->menuBar()->setVisible(!this->owner->menuBar()->isVisible());
    this->owner->setWindowState(this->owner->windowState() ^ Qt::WindowFullScreen);
}


void MainWindow::Private::showOpenDialog() {
    const auto & global = Global::instance();
    QString path = QFileDialog::getOpenFileName(this->owner, OPEN_DIALOG_TITLE, QDir::homePath(), global.getDialogFilter());
    if (path.isEmpty()) {
        return;
    }
    auto & fc = global.getFileController();
    auto url = QUrl::fromLocalFile(path);
    fc.open(url);
}


void MainWindow::Private::showOpenDirectoryDialog() {
    QString path = QFileDialog::getExistingDirectory(this->owner, OPEN_DIRECTORY_DIALOG_TITLE, QDir::homePath());
    if (path.isEmpty()) {
        return;
    }
    auto & fc = Global::instance().getFileController();
    auto url = QUrl::fromLocalFile(path);
    fc.open(url);
}


// TODO
void MainWindow::Private::showNavigator() {
    auto & fc = Global::instance().getFileController();

    if (fc.isEmpty()) {
        this->popupError(NO_OPENABLE_FILE);
        return;
    }

    this->navigator->setModel(fc.getModel());
    this->navigator->setCurrentIndex(fc.getCurrentIndex());
    this->ui.graphicsView->setPaused(true);
    this->navigator->exec();
    this->ui.graphicsView->setPaused(false);
}


void MainWindow::Private::onProgressUpdated(int current, int total) {
    if (current == total) {
        this->progress->reset();
        return;
    }
    this->progress->setRange(0, total);
    this->progress->setValue(current);
}


void MainWindow::Private::onModelReady() {
    this->progress->reset();
}
