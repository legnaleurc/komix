#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDir>
#include <QStringList>

namespace KomiX {

	class ImageArea;
	class ScaleImage;
	
	class MainWindow : public QMainWindow {
		
		Q_OBJECT
		
		public:
			MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
		
		public slots:
			void nextFile();
			void prevFile();
			void whellAction( int delta );
			void updateEnvironment( const QString & name );
			void open( const QString & name );
			void openFileDialog();
			void openDirDialog();
			void toggleFullScreen();
			void about();
		
		private:
			void initMenuBar_();
			void initCentralWidget_();
			
			static const QStringList supportedFormats_;
			static const QString fileFilter_;
			
			ImageArea * imageArea_;
			ScaleImage * scaleImage_;
			int index_;
			QDir dir_;
			QStringList files_;
	};
	
}

#endif
