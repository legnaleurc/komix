/**
 * @file imageproxyitem.cpp
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

#include "imageproxyitem_p.hpp"

#include <cassert>

#include <QtCore/QtDebug>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QLabel>

#include "imageloader.hpp"


using KomiX::widget::ImageProxyItem;
using KomiX::DeviceCreator;


ImageProxyItem::ImageProxyItem(int id, DeviceCreator deviceCreator, const QSize & size)
    : QGraphicsObject()
    , p_(new Private(id, deviceCreator, size, this))
{
}

void ImageProxyItem::setPaused(bool paused) {
    if (this->p_->movie) {
        this->p_->movie->setPaused(paused);
    }
}


void ImageProxyItem::activate() {
    if (this->p_->activity != Private::Activity::Deactivated) {
        return;
    }
    this->p_->activity = Private::Activity::Activating;
    auto device = this->p_->deviceCreator();
    ImageLoader::load(this->getID(), device, this->p_, SLOT(onFinished(int, const QPixmap &)), SLOT(onFinished(int, QMovie *)));
}


void ImageProxyItem::deactivate() {
    assert((this->p_->activity != Private::Activity::Activating) || !"deactivate while activating");
    if (this->p_->activity == Private::Activity::Deactivated) {
        return;
    }
    if (this->p_->item) {
        this->scene()->removeItem(this->p_->item);
        delete this->p_->item;
        this->p_->item = nullptr;
    }
    if (this->p_->movie) {
        this->p_->movie = nullptr;
    }
    this->p_->activity = Private::Activity::Deactivated;
}


QRectF ImageProxyItem::boundingRect() const {
    const auto & size = this->p_->size;
    if (!size.isValid()) {
        return QRectF();
    }
    return QRectF(QPoint(0, 0), size);
}

void ImageProxyItem::paint(QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/) {
    if (this->p_->activity == Private::Activity::Deactivated) {
        this->activate();
    }
    emit this->viewing(this->getID());
}


int ImageProxyItem::getID() const {
    return this->p_->id;
}


ImageProxyItem::Private::Private(int id, DeviceCreator deviceCreator, const QSize & size, ImageProxyItem * owner)
    : QObject(owner)
    , owner(owner)
    , id(id)
    , deviceCreator(deviceCreator)
    , size(size)
    , activity(Activity::Deactivated)
    , item(nullptr)
    , movie(nullptr)
{
}


void ImageProxyItem::Private::onFinished(int /*id*/, QMovie * movie) {
    QLabel * label = new QLabel;
    label->setMovie(movie);
    movie->setParent(label);
    movie->start();
    label->resize(movie->frameRect().size());
    QGraphicsProxyWidget * item = new QGraphicsProxyWidget(this->owner);
    item->setWidget(label);
    // HACK workaround for https://bugreports.qt.io/browse/QTBUG-55070
    item->setOpacity(0.99);

    this->movie = movie;
    this->item = item;

    this->activity = Activity::Activated;

    emit this->owner->activated(this->owner);
}


void ImageProxyItem::Private::onFinished(int /*id*/, const QPixmap & pixmap) {
    QGraphicsPixmapItem * item = new QGraphicsPixmapItem(pixmap, this->owner);
    item->setTransformationMode(Qt::SmoothTransformation);

    this->item = item;

    this->activity = Activity::Activated;

    emit this->owner->activated(this->owner);
}
