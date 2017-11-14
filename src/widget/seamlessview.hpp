/**
 * @file seamlessview.hpp
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
#ifndef KOMIX_WIDGET_SEAMLESSVIEW_HPP
#define KOMIX_WIDGET_SEAMLESSVIEW_HPP

#include <QtWidgets/QGraphicsView>

#include <memory>


namespace KomiX {

class FileController;

namespace widget {

class SeamlessView : public QGraphicsView {
    Q_OBJECT
    using QGraphicsView::scale;
public:
    explicit SeamlessView(QWidget * parent);

    void setPaused(bool paused);

signals:
    void fileDropped(const QUrl & uri);
    void middleClicked();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *) override;
    virtual void dragMoveEvent(QDragMoveEvent *) override;
    virtual void dropEvent(QDropEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;
    virtual bool viewportEvent(QEvent *) override;
    virtual void wheelEvent(QWheelEvent *) override;

private:
    class Private;
    std::shared_ptr<Private> p_;
};

}
}

#endif
