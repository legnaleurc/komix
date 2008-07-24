#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include "../utility.hpp"

#include <QScrollArea>
#include <QPointer>
#include <QTimer>
#include <QPoint>
#include <QLabel>
#include <QAction>
#include <QMouseEvent>
#include <QList>
#include <QUrl>
#include <QScrollBar>

namespace KomiX {

	class ImageArea : public QScrollArea {
		
		Q_OBJECT
		
		public:
			enum ViewPortState {
				Top = 1,
				Bottom = 2,
				Left = 4,
				Right = 8,
			};
			
			ImageArea( QWidget * parent = 0 );
			
			void openFile( const QString & fileName );
		
		public slots:
			void scale( int scalar );
			void stepTop();
			void stepBottom();
			void stepLeft();
			void stepRight();
			void autoMove();
		
		signals:
			void nextPage();
			void wheelMoved( int );
			void fileDroped( const QString & );
		
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
			
			QPointer< QLabel > image_;
			QPointer< QTimer > topTimer_;
			QPointer< QTimer > bottomTimer_;
			QPointer< QTimer > leftTimer_;
			QPointer< QTimer > rightTimer_;
			QPoint downPosition_;
			QPoint movePosition_;
			double scalar_;
			int state_;
			int step_;
			int interval_;
	};

}

#endif
