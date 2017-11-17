/**
 * @file imageloader.cpp
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
#include "imageloader_p.hpp"

#include <cassert>

#include <QtConcurrent>
#include <QtGui/QImageReader>

#include <QtCore/QtDebug>


using KomiX::ImageLoader;
using KomiX::AsynchronousDeviceLoader;
using KomiX::CharacterDeviceLoader;
using KomiX::BlockDeviceLoader;
using KomiX::ImageHelper;


void ImageLoader::load(int id, DeviceSP device, QObject * receiver,
                       const char * pictureLoaded, const char * animationLoaded)
{
    auto loader = new ImageLoader(id, device, receiver);
    receiver->connect(loader, SIGNAL(finished(int, const QPixmap &)), pictureLoaded);
    receiver->connect(loader, SIGNAL(finished(int, QMovie *)), animationLoaded);
    loader->start();
}


ImageLoader::ImageLoader(int id, DeviceSP device, QObject * parent)
    : QObject(parent)
    , p_(new Private(id, device, this))
{
    this->connect(this->p_,
                  SIGNAL(finished(int, QMovie *)),
                  SIGNAL(finished(int, QMovie *)));
    this->connect(this->p_,
                  SIGNAL(finished(int, const QPixmap &)),
                  SIGNAL(finished(int, const QPixmap &)));
}


void ImageLoader::start() const {
    AsynchronousDeviceLoader * loader = nullptr;
    if (this->p_->device->isSequential()) {
        // character device, async operation
        loader = new CharacterDeviceLoader(this->p_->device, this->p_);
    } else {
        // large block device, async operation
        loader = new BlockDeviceLoader(this->p_->device, this->p_);
    }
    this->p_->connect(loader,
                      SIGNAL(finished(const QByteArray &)),
                      SLOT(onDataFinished(const QByteArray &)));
    loader->start();
}


ImageLoader::Private::Private(int id, std::shared_ptr<QIODevice> device, QObject * parent)
    : QObject(parent)
    , id(id)
    , device(device)
{
}


// NOTE
// somehow QFile will lost file name information while looping QMovie
// so we must wrap with a QBuffer
void ImageLoader::Private::onDataFinished(const QByteArray & data) {
    QBuffer * buffer = new QBuffer(this);
    buffer->setData(data);
    buffer->open(QIODevice::ReadOnly);
    QImageReader iin(buffer);
    if (iin.supportsAnimation()) {
        buffer->seek(0);
        QMovie * movie = new QMovie(buffer);
        buffer->setParent(movie);
        emit this->finished(this->id, movie);
    } else {
        buffer->deleteLater();
        auto loader = new ImageHelper(data, this);
        this->connect(loader,
                      SIGNAL(finished(const QImage &)),
                      SLOT(onImageFinished(const QImage &)));
        loader->start();
    }
}


void ImageLoader::Private::onImageFinished(const QImage & image) {
    auto pixmap = QPixmap::fromImage(image);
    emit this->finished(this->id, pixmap);

    // delete the image loader and all helpers
    this->parent()->deleteLater();
}


AsynchronousDeviceLoader::AsynchronousDeviceLoader(ImageLoader::DeviceSP device,
                                                   QObject * parent)
    : QObject(parent)
    , device_(device)
{
}


ImageLoader::DeviceSP AsynchronousDeviceLoader::getDevice() const {
    return this->device_;
}


CharacterDeviceLoader::CharacterDeviceLoader(ImageLoader::DeviceSP device,
                                             QObject * parent)
    : AsynchronousDeviceLoader(device, parent)
    , buffer_()
{
}


void CharacterDeviceLoader::start() {
    auto device = this->getDevice();

    this->connect(device.get(), SIGNAL(readyRead()), SLOT(onReadyRead()));
    this->connect(device.get(),
                  SIGNAL(readChannelFinished()),
                  SLOT(onReadFinished()));

    bool ok = this->buffer_.open(QIODevice::ReadWrite);
    assert(ok || !"cannot open buffer");
}


void CharacterDeviceLoader::onReadyRead() {
    auto device = this->getDevice();
    auto chunk = device->readAll();
    this->buffer_.write(chunk);
}


void CharacterDeviceLoader::onReadFinished() {
    auto device = this->getDevice();
    // ensure there is not data remain
    auto chunk = device->readAll();
    this->buffer_.write(chunk);

    this->buffer_.seek(0);
    const auto & data = this->buffer_.data();
    emit this->finished(data);
}


BlockDeviceLoader::BlockDeviceLoader(ImageLoader::DeviceSP device,
                                     QObject * parent)
    : AsynchronousDeviceLoader(device, parent)
    , watcher_(new QFutureWatcher<QByteArray>(this))
{
    this->connect(this->watcher_, SIGNAL(finished()), SLOT(onReadFinished()));
}


void BlockDeviceLoader::start() {
    // should use bind, but VS has bug VSO#246001
    auto fn = [this]() -> QByteArray {
        return this->load();
    };
    QFuture<QByteArray> future = QtConcurrent::run(fn);
    this->watcher_->setFuture(future);
}


QByteArray BlockDeviceLoader::load() {
    auto device = this->getDevice();
    if (!device->open(QIODevice::ReadOnly)) {
        // TODO emit error signal
        assert(!"cannot open file");
    }
    auto chunk = device->readAll();
    device->close();
    return chunk;
}


void BlockDeviceLoader::onReadFinished() {
    auto future = this->watcher_->future();
    auto data = future.result();
    emit this->finished(data);
}


ImageHelper::ImageHelper(const QByteArray & data, QObject * parent)
    : QObject(parent)
    , data_(data)
    , watcher_(new QFutureWatcher<QImage>(this))
{
    this->connect(this->watcher_, SIGNAL(finished()), SLOT(onReadFinished()));
}


void ImageHelper::start() {
    // should use bind, but VS has bug VSO#246001
    auto fn = [this]() -> QImage {
        return this->load();
    };
    QFuture<QImage> future = QtConcurrent::run(fn);
    this->watcher_->setFuture(future);
}


QImage ImageHelper::load() {
    QBuffer in;
    in.setData(this->data_);
    if (!in.open(QIODevice::ReadOnly)) {
        assert(!"cannot open buffer");
    }
    QImageReader ir(&in);
    return ir.read();
}


void ImageHelper::onReadFinished() {
    auto future = this->watcher_->future();
    auto image = future.result();
    emit this->finished(image);
}
