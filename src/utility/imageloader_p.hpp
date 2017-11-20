/**
 * @file imageloader_p.hpp
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
#ifndef KOMIX_IMAGELOADER_HPP_
#define KOMIX_IMAGELOADER_HPP_

#include "imageloader.hpp"

#include <QtCore/QBuffer>
#include <QtCore/QFutureWatcher>
#include <QtGui/QMovie>
#include <QtGui/QPixmap>


namespace KomiX {

class ImageLoader::Private : public QObject {
    Q_OBJECT
public:
    Private(int id, ImageLoader::DeviceSP device, QObject * parent);

    void start();

public slots:
    void onDataFinished(const QByteArray & data);
    void onImageFinished(const QImage & image);

signals:
    void finished(int id, const QPixmap & pixmap);
    void finished(int id, QMovie * movie);

public:
    int id;
    std::shared_ptr<QIODevice> device;
};


class AsynchronousDeviceLoader : public QObject {
    Q_OBJECT
public:
    AsynchronousDeviceLoader(ImageLoader::DeviceSP device, QObject * parent);

    ImageLoader::DeviceSP getDevice() const;

    virtual void start() = 0;

signals:
    void finished(const QByteArray & data);

private:
    ImageLoader::DeviceSP device_;
};


class CharacterDeviceLoader : public AsynchronousDeviceLoader {
    Q_OBJECT
public:
    CharacterDeviceLoader(ImageLoader::DeviceSP device, QObject * parent);

    void start();

private slots:
    void onReadyRead();
    void onReadFinished();

private:
    QBuffer buffer_;
};


class BlockDeviceLoader : public AsynchronousDeviceLoader {
    Q_OBJECT
public:
    BlockDeviceLoader(ImageLoader::DeviceSP device, QObject * parent);

    void start();

private slots:
    void onReadFinished();

private:
    QByteArray load();

    QFutureWatcher<QByteArray> * watcher_;
};


class ImageHelper : public QObject {
    Q_OBJECT
public:
    ImageHelper(const QByteArray & data, QObject * parent);

    void start();

signals:
    void finished(const QImage & image);

private slots:
    void onReadFinished();

private:
    QImage load();

    QByteArray data_;
    QFutureWatcher<QImage> * watcher_;
};

}

#endif
