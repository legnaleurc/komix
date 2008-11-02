#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include <QScrollArea>
#include <QPoint>
#include <QLabel>
#include <QSize>

namespace KomiX {

	class ImageArea : public QScrollArea {
		
		Q_OBJECT
		
		public:
			enum ViewPortState {
				TopRight,
				BottomRight,
				TopLeft,
				BottomLeft,
			};
			
			ImageArea( QWidget * parent = 0 );
			
			void openFile( const QString & fileName );
		
		public slots:
			void scale( int ratio );
			void stepTop();
			void stepBottom();
			void stepLeft();
			void stepRight();
			void autoMove();
		
		signals:
			void nextPage();
			void wheelMoved( int );
			void fileDroped( const QString & );
			void middleClicked();
		
		protected:
			virtual void mousePressEvent( QMouseEvent * event );
			virtual void mouseReleaseEvent( QMouseEvent * event );
			virtual void mouseMoveEvent( QMouseEvent * event );
			virtual void wheelEvent( QWheelEvent * event );
			virtual void dragEnterEvent( QDragEnterEvent * event );
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
