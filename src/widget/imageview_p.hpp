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
#include "imagewrapper.hpp"
#include "scalewidget.hpp"
#include "filecontroller.hpp"

#include <QtCore/QParallelAnimationGroup>

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
	void moveBy( const QPointF & );
	void updateScaling();
	void updateViewportRectangle();
	QLineF getMotionVector( Direction );
	QLineF getMotionVector( double, double );
	void setupAnimation( int, double, double );

public slots:
	void addImage( QIODevice * image );
	void animeStateChanged( QAbstractAnimation::State, QAbstractAnimation::State );

public:
	ImageView * owner;
	QParallelAnimationGroup * anime;
	FileController * controller;
	double imgRatio;
	QRectF imgRect;
	int msInterval;
	QList< QIODevice * > pageBuffer;
	ScaleWidget * panel;
	int pixelInterval;
	QPoint pressEndPosition;
	QPoint pressStartPosition;
	ScaleMode scaleMode;
	QRectF vpRect;
	Direction vpState;
};

}
}

#endif
