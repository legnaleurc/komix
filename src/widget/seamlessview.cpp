/**
 * @file seamlessview.cpp
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
#include "seamlessview_p.hpp"

#include <algorithm>
#include <cmath>

#include <QtConcurrent/QtConcurrentMap>
#include <QtCore/QMimeData>
#include <QtCore/QSettings>
#include <QtCore/QtDebug>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QImageReader>
#include <QtWidgets/QGestureEvent>
#include <QtWidgets/QPinchGesture>
#include <QtWidgets/QScrollBar>

#include "filecontroller.hpp"
#include "global.hpp"


namespace {

const qreal RECYCLE_DISTANCE = 8192.;

}


using KomiX::widget::SeamlessView;
using KomiX::widget::ImageProxyItem;
using KomiX::FileController;


SeamlessView::SeamlessView(QWidget * parent)
    : QGraphicsView(parent)
    , p_(new Private(this))
{
#ifdef Q_OS_MAC
    this->viewport()->grabGesture(Qt::PinchGesture);
#endif
    this->setScene(new QGraphicsScene(this));

    auto & fc = Global::instance().getFileController();
    fc.connect(this, SIGNAL(fileDropped(const QUrl &)), SLOT(open(const QUrl &)));
    this->p_->connect(&fc, SIGNAL(modelReady()), SLOT(resolveBounds()));
    this->p_->connect(&fc, SIGNAL(focus(int)), SLOT(focusTo(int)));
}


void SeamlessView::setPaused(bool paused) {
    // TODO should be all?
    for (auto proxyItem : this->p_->activeItems) {
        proxyItem->setPaused(paused);
    }
}


void SeamlessView::dragEnterEvent(QDragEnterEvent * event) {
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}


void SeamlessView::dragMoveEvent(QDragMoveEvent * event) {
    event->acceptProposedAction();
}


void SeamlessView::dropEvent(QDropEvent * event) {
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();

        if (!urlList.empty()) {
            foreach(QUrl url, urlList) {
                emit this->fileDropped(url);
            }
        }
    }
    event->acceptProposedAction();
}


void SeamlessView::mouseMoveEvent(QMouseEvent * event) {
    this->QGraphicsView::mouseMoveEvent(event);
    this->p_->showCursor();
}


bool SeamlessView::viewportEvent(QEvent * event) {
    if (event->type() == QEvent::Gesture) {
        QGestureEvent * gEvent = static_cast<QGestureEvent *>(event);
        this->p_->handleGesture(gEvent);
        return true;
    }
    return this->QGraphicsView::viewportEvent(event);
}


void SeamlessView::wheelEvent(QWheelEvent * event) {
#ifndef Q_OS_MAC
    bool withCtrl = event->modifiers() & Qt::ControlModifier;
    if (withCtrl) {
        int delta = event->delta();
        this->p_->adjustScaleFactor(delta >= 0 ? 1.1 : 0.9);
        return;
    }
#endif
    this->QGraphicsView::wheelEvent(event);
    this->p_->hideCursor();
}


SeamlessView::Private::Private(SeamlessView * owner)
    : QObject()
    , owner(owner)
    , items()
    , activeItems()
    , recycleTimer()
    , scaleFactor(1.)
    , baseScaleFactor(1.)
    , watcher(new QFutureWatcher<QSize>(this))
    , cursorVisible(true)
{
    // setup recycle timer
    this->recycleTimer.setInterval(1000);
    this->connect(&this->recycleTimer, SIGNAL(timeout()), SLOT(tryRecycle()));
    this->recycleTimer.start();

    // watch bound resolver
    this->connect(this->watcher, SIGNAL(finished()), SLOT(createProxies()));
}


void SeamlessView::Private::adjustScaleFactor(qreal df) {
    this->setScaleFactor(this->scaleFactor * df);
}


void SeamlessView::Private::setScaleFactor(qreal factor) {
    if (factor < .1 || factor >= 8.) {
        return;
    }
    auto matrix = QTransform::fromScale(factor, factor);
    auto tmp = this->owner->transformationAnchor();
    this->owner->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->owner->setTransform(matrix);
    this->owner->setTransformationAnchor(tmp);
    this->scaleFactor = factor;
    this->hideCursor();
}


void SeamlessView::Private::handleGesture(QGestureEvent * event) {
    auto gs = event->gestures();
    for (auto g : gs) {
        if (g->gestureType() != Qt::PinchGesture) {
            continue;
        }
        auto pg = static_cast<QPinchGesture *>(g);
        if (pg->state() == Qt::GestureStarted) {
            this->beginRelativeScaling();
        } else if (pg->state() == Qt::GestureFinished) {
            this->endRelativeScaling();
        } else {
            auto changeFlags = pg->changeFlags();
            bool sfc = changeFlags & QPinchGesture::ScaleFactorChanged;
            if (!sfc) {
                continue;
            }
            auto factor = pg->totalScaleFactor();
            this->updateRelativeScaling(factor);
        }
    }
}


void SeamlessView::Private::beginRelativeScaling() {
    this->baseScaleFactor = this->scaleFactor;
}


void SeamlessView::Private::updateRelativeScaling(qreal factor) {
    factor = this->baseScaleFactor * factor;
    this->setScaleFactor(factor);
}


void SeamlessView::Private::endRelativeScaling() {
    this->baseScaleFactor = 1.;
}


void SeamlessView::Private::showCursor() {
    if (!this->cursorVisible) {
        this->owner->viewport()->setCursor(Qt::OpenHandCursor);
        this->cursorVisible = true;
    }
}


void SeamlessView::Private::hideCursor() {
    if (this->cursorVisible) {
        this->owner->viewport()->setCursor(Qt::BlankCursor);
        this->cursorVisible = false;
    }
}


void SeamlessView::Private::resolveBounds() {
    auto & fc = Global::instance().getFileController();
    auto model = fc.getModel();
    auto nImages = model->rowCount();
    // somehow std::vector will out of bound
    QList<DeviceCreator> devices;
    for (int row = 0; row < nImages; ++row) {
        auto item = model->index(row, 0);
        auto dc = item.data(Qt::UserRole).value<DeviceCreator>();
        devices.push_back(dc);
    }
    // somehow VC cannot resolve the return type of lambda expression
    std::function<QSize (DeviceCreator)> helper = [](DeviceCreator dc) -> QSize {
        auto device = dc();
        QImageReader reader(device.get());
        return reader.size();
    };
    QFuture<QSize> sizes = QtConcurrent::mapped(devices, helper);
    this->watcher->setFuture(sizes);
}


void SeamlessView::Private::createProxies() {
    // remove all items
    this->activeItems.clear();
    auto scene = this->owner->scene();
    for (auto item : this->items) {
        scene->removeItem(item);
        item->deleteLater();
    }
    this->items.clear();

    // setup scaling paramaters
    this->setScaleFactor(1.);
    auto globalRect = this->owner->viewport()->geometry();
    auto sceneRect = this->owner->mapToScene(globalRect).boundingRect();
    auto viewportWidth = sceneRect.width();

    // make placeholder images
    auto & fc = Global::instance().getFileController();
    auto model = fc.getModel();
    auto nImages = model->rowCount();
    qreal offset = 0.0;
    for (int row = 0; row < nImages; ++row) {
        auto item = model->index(row, 0);
        auto dc = item.data(Qt::UserRole).value<DeviceCreator>();
        auto proxyItem = new ImageProxyItem(row, dc, this->watcher->resultAt(row));
        this->items.push_back(proxyItem);
        scene->addItem(proxyItem);

        // adjust item width
        auto itemRect = proxyItem->boundingRect();
        auto itemRectInScene = proxyItem->mapToScene(itemRect).boundingRect();
        auto factor = viewportWidth / itemRectInScene.width();
        proxyItem->setScale(factor);
        itemRectInScene = proxyItem->mapToScene(itemRect).boundingRect();

        // move to the next position
        proxyItem->moveBy(0.0, offset);
        offset += itemRectInScene.height();

        // connect events
        this->connect(proxyItem, SIGNAL(activated(ImageProxyItem *)), SLOT(addActiveItem(ImageProxyItem *)));
    }

    scene->setSceneRect(scene->itemsBoundingRect());
    this->owner->verticalScrollBar()->setValue(0);
}


void SeamlessView::Private::addActiveItem(ImageProxyItem * proxyItem) {
    this->activeItems.push_back(proxyItem);
}


void SeamlessView::Private::tryRecycle() {
    auto globalRect = this->owner->viewport()->geometry();
    auto sceneRect = this->owner->mapToScene(globalRect).boundingRect();
    auto center = sceneRect.center();

    auto b = std::begin(this->activeItems);
    auto e = std::end(this->activeItems);
    auto it = std::partition(b, e, [&sceneRect, &center](ImageProxyItem * proxyItem) -> bool {
        auto itemRect = proxyItem->mapRectToScene(proxyItem->boundingRect());
        if (sceneRect.intersects(itemRect)) {
            return true;
        }
        auto line = QLineF(center, itemRect.center());
        auto dist = pow(pow(line.dx(), 2) + pow(line.dy(), 2), 0.5);
        return dist < RECYCLE_DISTANCE;
    });

    e = std::end(this->activeItems);
    std::for_each(it, e, [](ImageProxyItem * proxyItem) -> void {
        proxyItem->deactivate();
    });
    this->activeItems.erase(it, e);
}


void SeamlessView::Private::focusTo(int index) {
    auto item = this->items.at(index);
    this->owner->ensureVisible(item, 0, 0);
}
