/**
 * @file imagearea.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KOMIX_IMAGEAREA_HPP
#define KOMIX_IMAGEAREA_HPP

#include <QScrollArea>
#include <QPoint>
#include <QLabel>
#include <QSize>

namespace KomiX {

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

	public slots:
		/**
		 * @brief set display image
		 * @param image opened image
		 */
		void setImage( const QPixmap & image );
		
		/**
		 * @brief scale image
		 * @param ratio the scale rate
		 *
		 * The ratio means percents, so 100 actually means 100%.
		 */
		void scale( int ratio );
		
		/// move to top by time pass
		void stepTop();
		/// move to bottom by time pass
		void stepBottom();
		/// move to left by time pass
		void stepLeft();
		/// move to right by time pass
		void stepRight();
		
		/**
		 * @brief smooth move navigation
		 *
		 * The initial state is top-right,
		 * then move to bottom-right,
		 * and then top-left, bottom-left,
		 * open next page finally.
		 */
		void smoothMove();

	signals:
		/// go to next page
		void nextPage();
		/**
		 * @brief whell scroll event
		 * @param distance delta of scrolling
		 */
		void wheelMoved( int distance );
		/**
		 * @brief acceptable file dropped event
		 * @param filePath the droped file path
		 *
		 * Invalid file will not emit this signal.
		 */
		void fileDroped( const QString & filePath );
		/// middle click event
		void middleClicked();

	protected:
		/// overrided method
		virtual void mousePressEvent( QMouseEvent * event );
		/// overrided method
		virtual void mouseReleaseEvent( QMouseEvent * event );
		/// overrided method
		virtual void mouseMoveEvent( QMouseEvent * event );
		/// overrided method
		virtual void wheelEvent( QWheelEvent * event );
		/// overrided method
		virtual void dragEnterEvent( QDragEnterEvent * event );
		/// overrided method
		virtual void dropEvent( QDropEvent * event );

	private:
		void stopAllStep_();
		void moveScrollBars_( int, int );
		bool canMoveTop_() const;
		bool canMoveBottom_() const;
		bool canMoveLeft_() const;
		bool canMoveRight_() const;

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

#endif
