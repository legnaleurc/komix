/**
 * @file imageproxyitem_p.hpp
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
#ifndef KOMIX_WIDGET_IMAGEPROXYITEM_HPP_
#define KOMIX_WIDGET_IMAGEPROXYITEM_HPP_

#include "imageproxyitem.hpp"

#include <QtGui/QMovie>


namespace KomiX {
namespace widget {

class ImageProxyItem::Private : public QObject {
    Q_OBJECT
public:
    explicit Private(int id, DeviceCreator deviceCreator, const QSize & size, ImageProxyItem * owner);

public slots:
    void onFinished(int id, QMovie * movie);
    void onFinished(int id, const QPixmap & pixmap);

signals:
    void changed();

public:
    ImageProxyItem * owner;
    int id;
    DeviceCreator deviceCreator;
    QSize size;
    bool activated;
    bool activating;
    QGraphicsItem * item;
    QMovie * movie;
};

}
}

#endif
