/**
 * @file imageview.hpp
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
#ifndef KOMIX_WIDGET_IMAGEVIEW_HPP
#define KOMIX_WIDGET_IMAGEVIEW_HPP

#include <QtGui/QGraphicsView>

#include <memory>

namespace KomiX {

class FileController;

namespace widget {

enum Direction {
	Top,
	Bottom,
	Left,
	Right,
	TopRight,
	BottomRight,
	TopLeft,
	BottomLeft
};

class ImageView : public QGraphicsView {
	Q_OBJECT
	using QGraphicsView::scale;

public:
	explicit ImageView( QWidget * parent );

	void initialize( FileController * controller );
	void moveBy( QPointF delta = QPointF() );
	bool open( const QUrl & uri );
	void scale( double ratio );
	void setPaused( bool paused );

public slots:
	void begin();
	void end();
	void fitHeight();
	void fitWidth();
	void fitWindow();
	void loadSettings();
	void nextPage();
	void previousPage();
	void scale( int pcRatio );
	void showControlPanel();
	void smoothMove();
	void smoothReversingMove();

signals:
	void fileDropped( const QUrl & uri );
	void middleClicked();
	void scaled( int ratio );

protected:
	/// overrided method
	virtual void dragEnterEvent( QDragEnterEvent * );
	/// overrided method
	virtual void dragMoveEvent( QDragMoveEvent * );
	/// overrided method
	virtual void dropEvent( QDropEvent * );
	/// overrided method
	virtual void keyPressEvent( QKeyEvent * );
	/// overrided method
	virtual void mouseMoveEvent( QMouseEvent * );
	/// overrided method
	virtual void mousePressEvent( QMouseEvent * );
	/// overrided method
	virtual void mouseReleaseEvent( QMouseEvent * );
	/// overrided method
	virtual void resizeEvent( QResizeEvent * );
	/// overrided method
	virtual void wheelEvent( QWheelEvent * );

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif
