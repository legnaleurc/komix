/**
 * @file imageview_p.hpp
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
#ifndef KOMIX_WIDGET_IMAGEVIEW_P_HPP
#define KOMIX_WIDGET_IMAGEVIEW_P_HPP

#include "imageview.hpp"
#include "filecontroller.hpp"
#include "imageitem.hpp"
#include "viewstate.hpp"

#include <QtCore/QPropertyAnimation>

namespace KomiX {
namespace widget {

class ImageView::Private: public QObject {
	Q_OBJECT
public:
	enum ScaleMode {
		Custom,
		Width,
		Height,
		Window
	};

	explicit Private( ImageView * owner );

	void setImage( const QList< QIODevice * > & images );
	void scale( double ratio );
	void moveBy( const QPointF & );
	void fromViewportMoveBy( QPointF delta = QPointF() );
	void updateScaling();
	void updateViewportRectangle();
	QLineF normalizeMotionVector( double, double );
	void setupAnimation( int, double, double );
	void addTransition( boost::signals2::signal< void () > & signal, std::shared_ptr< ViewState > state );

public slots:
	void addImage( QIODevice * image );
	void animeStateChanged( QAbstractAnimation::State, QAbstractAnimation::State );
	void onImageChanged();

public:
	ImageView * owner;
	ImageItem * image;
	QPropertyAnimation * anime;
	FileController * controller;
	double imgRatio;
	QRectF imgRect;
	int msInterval;
	QList< QIODevice * > pageBuffer;
	int pixelInterval;
	QPoint pressEndPosition;
	QPoint pressStartPosition;
	ScaleMode scaleMode;
	QRectF vpRect;
	std::shared_ptr< ViewState > trState;
	std::shared_ptr< ViewState > brState;
	std::shared_ptr< ViewState > tlState;
	std::shared_ptr< ViewState > blState;
	std::shared_ptr< ViewState > tState;
	std::shared_ptr< ViewState > bState;
	std::shared_ptr< ViewState > rState;
	std::shared_ptr< ViewState > lState;
	std::shared_ptr< ViewState > cState;
	std::shared_ptr< ViewState > currentState;
};

}
}

#endif
