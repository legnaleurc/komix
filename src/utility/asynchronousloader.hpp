/**
 * @file asynchronousloader.hpp
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
#ifndef KOMIX_WIDGET_ASYNCHRONOUSLOADER_HPP
#define KOMIX_WIDGET_ASYNCHRONOUSLOADER_HPP

#include <QtCore/QIODevice>
#include <QtCore/QRunnable>

#include <memory>


namespace KomiX {

class AsynchronousLoader : public QObject, public QRunnable {
    Q_OBJECT
public:
    AsynchronousLoader(std::shared_ptr<QIODevice> device);

protected:
    QIODevice * getDevice() const;

signals:
    void finished(const QByteArray & data);

private:
    class Private;
    std::shared_ptr<Private> p_;
};

}

#endif
