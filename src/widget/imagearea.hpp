/**
 * @file imagearea.hpp
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
#ifndef KOMIX_WIDGET_IMAGEAREA_HPP
#define KOMIX_WIDGET_IMAGEAREA_HPP

#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtGui/QScrollArea>

class QLabel;
class QUrl;

namespace KomiX {

	class FileController;

	namespace widget {

		class ScalePanel;
		class Navigator;

		/**
		 * @brief The main display area
		 * @todo improve the performance of smoothMove()
		 *
		 * This is the pictures where you see.
		 */
		class ImageArea : public QScrollArea {
			Q_OBJECT

		public:
			/// Viewport position states
			enum ViewPortState {
				/// top-right
				TopRight,
				/// bottom-right
				BottomRight,
				/// top-left
				TopLeft,
				/// bottom-left
				BottomLeft,
			};

			/**
			 * @brief default constructor
			 * @param parent parent widget
			 */
			ImageArea( QWidget * parent = 0 );

			/**
			 * @brief open @p url
			 * @return if succeed
			 */
			bool open( const QUrl & url );

		public slots:
			/**
			 * @brief set display image
			 * @param image opened image
			 */
			void setImage( const QPixmap & image );

			/// load settings from configure file
			void loadSettings();

			/// show scale panel
			void showScalePanel();
			/// show navigator
			void showNavigator();

			/// turn to next page
			void next();
			/// turn to previous page
			void prev();

			/**
			 * @brief scale image
			 * @param ratio the scale rate
			 * @sa scale()
			 *
			 * The @p ratio means percents, so 100 actually means 100%.
			 */
			void scale( int ratio );

			/**
			 * @brief move to top by time pass
			 * @sa stepBottom(), stepLeft(), stepRight(), smoothMove()
			 */
			void stepTop();
			/**
			 * @brief move to bottom by time pass
			 * @sa stepTop(), stepLeft(), stepRight(), smoothMove()
			 */
			void stepBottom();
			/**
			 * @brief move to left by time pass
			 * @sa stepTop(), stepBottom(), stepRight(), smoothMove()
			 */
			void stepLeft();
			/**
			 * @brief move to right by time pass
			 * @sa stepTop(), stepBottom(), stepLeft(), smoothMove()
			 */
			void stepRight();
			
			/**
			 * @brief smooth move navigation
			 * @sa stepTop(), stepBottom(), stepLeft(), stepRight(), reverseSmoothMove()
			 *
			 * The initial state is top-right,
			 * then move to bottom-right,
			 * and then top-left, bottom-left,
			 * open next page finally.
			 */
			void smoothMove();
			/**
			 * @brief reverse smooth move
			 * @sa smoothMove()
			 */
			void reverseSmoothMove();

			/// move to top-right side
			void home();
			/// move to bottom-left side
			void end();

		signals:
			/// scaled to @p delta rate
			void scaled( int delta );
			/**
			 * @brief acceptable file dropped event
			 * @param url the droped url
			 *
			 * Invalid file will not emit this signal.
			 */
			void fileDroped( const QUrl & url );
			/// require toogle screen
			void requireToogleScreen();
			/// send error message
			void errorOccured( const QString & errmsg );

		protected:
			/// overrided method
			virtual void mousePressEvent( QMouseEvent * );
			/// overrided method
			virtual void mouseReleaseEvent( QMouseEvent * );
			/// overrided method
			virtual void mouseMoveEvent( QMouseEvent * );
			/// overrided method
			virtual void wheelEvent( QWheelEvent * );
			/// overrided method
			virtual void dragEnterEvent( QDragEnterEvent * );
			/// overrided method
			virtual void dropEvent( QDropEvent * );
			/// overrided method
			virtual void resizeEvent( QResizeEvent * );

		private:
			void stopAllStep_();
			void moveScrollBars_( int, int );
			bool canMoveTop_() const;
			bool canMoveBottom_() const;
			bool canMoveLeft_() const;
			bool canMoveRight_() const;
			void updateImageSize_();

			ScalePanel * scale_;
			Navigator * navi_;
			FileController * ctrl_;
			QLabel * image_;
			QSize imageSize_;
			QTimer * topTimer_;
			QTimer * bottomTimer_;
			QTimer * leftTimer_;
			QTimer * rightTimer_;
			QPoint downPosition_;
			QPoint movePosition_;
			double ratio_;
			int state_;
			int step_;
			int interval_;
		};

	}
} // end namespace

#endif
