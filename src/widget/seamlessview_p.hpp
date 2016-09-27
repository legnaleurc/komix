/**
 * @file seamlessview_p.hpp
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
#ifndef KOMIX_WIDGET_SEAMLESSVIEW_HPP_
#define KOMIX_WIDGET_SEAMLESSVIEW_HPP_

#include "seamlessview.hpp"

#include <QtCore/QFutureWatcher>
#include <QtCore/QTimer>
#include <QtWidgets/QGestureEvent>

#include "imageproxyitem.hpp"


namespace KomiX {
namespace widget {

class SeamlessView::Private : public QObject {
    Q_OBJECT

public:
    explicit Private(SeamlessView * owner);

    void adjustScaleFactor(qreal df);
    void setScaleFactor(qreal factor);
    void handleGesture(QGestureEvent * event);
    void beginRelativeScaling();
    void updateRelativeScaling(qreal factor);
    void endRelativeScaling();
    void showCursor();
    void hideCursor();

public slots:
    void resolveBounds();
    void createProxies();
    void tryRecycle();
    void addActiveItem(ImageProxyItem *);
    void focusTo(int index);

public:
    SeamlessView * owner;
    std::vector<ImageProxyItem *> items;
    std::list<ImageProxyItem *> activeItems;
    QTimer recycleTimer;
    qreal scaleFactor;
    // only used in pinch gesture
    qreal baseScaleFactor;
    QFutureWatcher<QSize> * watcher;
    bool cursorVisible;
};

}
}


#endif
