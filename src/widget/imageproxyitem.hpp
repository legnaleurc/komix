/**
 * @file imageproxyitem.hpp
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
#ifndef KOMIX_WIDGET_IMAGEPROXYITEM_HPP
#define KOMIX_WIDGET_IMAGEPROXYITEM_HPP

#include "global.hpp"

#include <QtWidgets/QGraphicsObject>


namespace KomiX {
namespace widget {

// TODO just QGraphicsItem?
class ImageProxyItem : public QGraphicsObject {
    Q_OBJECT

public:
    ImageProxyItem(int id, DeviceCreator deviceCreator, const QSize & size);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void setPaused(bool paused);
    void activate();
    void deactivate();
    int getID() const;

signals:
    void activated(ImageProxyItem * item);
    void viewing(int id);

private:
    class Private;
    Private * p_;
};

}
}

#endif
