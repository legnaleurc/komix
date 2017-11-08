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

#include "global.hpp"
#include "filecontroller.hpp"
#include "single/singlemodel.hpp"
#include "directory/directorymodel.hpp"
#include "archive/archivemodel.hpp"

#include <QtCore/QStringList>

#include <QtWidgets/QApplication>


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
int main(int argc, char * argv[]) {
    QApplication app(argc, argv);
    QApplication::setWindowIcon(QIcon(":/image/logo.svg"));
    QApplication::setOrganizationName("FoolproofProject");
    QApplication::setApplicationName("KomiX");
    QApplication::setApplicationVersion(X(KOMIX_VERSION));

    QStringList args(QApplication::arguments());

    auto & global = KomiX::Global::instance();
    {
        using namespace KomiX::model;

        FileModel::registerModel(SingleModel::create);
        FileModel::registerModel(DirectoryModel::create);
        FileModel::registerModel(ArchiveModel::create);

        global.registerDialogFilter(SingleModel::createDialogFilter());
        global.registerDialogFilter(ArchiveModel::createDialogFilter());
    }

    KomiX::widget::MainWindow mainWindow;
    mainWindow.setWindowTitle(QApplication::applicationName());
    mainWindow.resize(800, 600);

    if (args.length() > 1) {
        global.getFileController().open(args.at(1));
    }

    mainWindow.show();

    return app.exec();
}
