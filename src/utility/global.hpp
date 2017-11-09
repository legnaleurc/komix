/**
 * @file global.hpp
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
#ifndef KOMIX_GLOBAL_HPP
#define KOMIX_GLOBAL_HPP

#include <QtCore/QMetaType>
#include <QtCore/QObject>

#include <functional>
#include <memory>


class QIODevice;
class QDir;


/**
 * @namespace KomiX
 * @brief Contains all KomiX components
 */
namespace KomiX {


class FileController;


class Global : public QObject {
public:
    static Global & instance();

    FileController & getFileController() const;

    /**
     * @brief Get temporary directory
     */
    const QDir & getTemporaryDirectory() const;
    /**
     * @brief Register file type filter for file open dialog
     */
    void registerDialogFilter(const QString & filter);
    /**
     * @brief Get file type filter for file open dialog
     */
    const QString & getDialogFilter() const;

    /**
     * @brief Get supported formats
     * @return A QStringList contains all supported formats
     *
     * The formats is read from QImageReader::supportedImageFormats().\n
     * This list transforms to lower string, sort it, and remove deprecated
     * items.
     */
    const QStringList & getSupportedFormats() const;

    /**
     * @brief Get the supported formats filter
     * @return A QStringList that can pass to name filter
     *
     * The string format is like this: <strong>"*.<ext>"</strong>.
     */
    const QStringList & getSupportedFormatsFilter() const;

private:
    Global();
    virtual ~Global();

    class Private;
    Private * p_;
};


/**
 * create a QIODevice to read the image file
 */
typedef std::function<std::shared_ptr<QIODevice> ()> DeviceCreator;

/**
 * @brief make @p exts to name filter
 */
QStringList toNameFilter(const QStringList & exts);
}

Q_DECLARE_METATYPE(KomiX::DeviceCreator)

#endif
