/**
 * @file deviceloader.cpp
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
#include "blockdeviceloader.hpp"
#include "characterdeviceloader.hpp"
#include "deviceloader_p.hpp"

#include <QtCore/QBuffer>
#include <QtCore/QThreadPool>
#include <QtGui/QImageReader>


using KomiX::DeviceLoader;
using KomiX::AsynchronousLoader;
using KomiX::CharacterDeviceLoader;
using KomiX::BlockDeviceLoader;


void DeviceLoader::load(int id, DeviceSP device, QObject * receiver, const char * pictureLoaded, const char * animationLoaded) {
    DeviceLoader * loader = new DeviceLoader(id, device, receiver);
    receiver->connect(loader, SIGNAL(finished(int, const QPixmap &)), pictureLoaded);
    receiver->connect(loader, SIGNAL(finished(int, QMovie *)), animationLoaded);
    loader->start();
}


DeviceLoader::DeviceLoader(int id, std::shared_ptr<QIODevice> device, QObject * parent)
    : QObject(parent)
    , p_(new Private(id, device, this))
{
    this->connect(this->p_, SIGNAL(finished(int, QMovie *)), SIGNAL(finished(int, QMovie *)));
    this->connect(this->p_, SIGNAL(finished(int, const QPixmap &)), SIGNAL(finished(int, const QPixmap &)));
}


void DeviceLoader::start() const {
    AsynchronousLoader * loader = nullptr;
    if (this->p_->device->isSequential()) {
        // character device, async operation
        loader = new CharacterDeviceLoader(this->p_->device);
    } else {
        // large block device, async operation
        loader = new BlockDeviceLoader(this->p_->device);
    }
    this->p_->connect(loader, SIGNAL(finished(const QByteArray &)), SLOT(onFinished(const QByteArray &)));
    QThreadPool::globalInstance()->start(loader);
}


DeviceLoader::Private::Private(int id, std::shared_ptr<QIODevice> device, QObject * parent)
    : QObject(parent)
    , id(id)
    , device(device)
{
}


void DeviceLoader::Private::read(QIODevice * device) {
    QImageReader iin(device);
    if (iin.supportsAnimation()) {
        device->seek(0);
        QMovie * movie = new QMovie(device);
        device->setParent(movie);
        emit this->finished(this->id, movie);
    } else {
        QPixmap pixmap = QPixmap::fromImageReader(&iin);
        // we do not need this buffer anymore
        device->deleteLater();
        emit this->finished(this->id, pixmap);
    }
}


// NOTE
// somehow QFile will lost file name information while looping QMovie
// so we must wrap with a QBuffer
void DeviceLoader::Private::onFinished(const QByteArray & data) {
    QBuffer * buffer = new QBuffer(this);
    buffer->setData(data);
    buffer->open(QIODevice::ReadOnly);
    this->read(buffer);
    this->parent()->deleteLater();
}
