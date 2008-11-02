#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDir>
#include <QStringList>
#include <QSystemTrayIcon>

namespace KomiX {

	class ImageArea;
	class ScaleImage;
	
	class MainWindow : public QMainWindow {
		
		Q_OBJECT
		
		public:
			MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
		
		public slots:
			void whellAction( int delta );
			void nextFile();
			void prevFile();
			void updateEnvironment( const QString & name );
			void open( const QString & name );
			void openFileDialog();
			void openDirDialog();
			void toggleFullScreen();
			void toggleSystemTray();
			void about();
		private slots:
			void systemTrayHelper_( QSystemTrayIcon::ActivationReason reason );
		
		private:
			void initMenuBar_();
			void initCentralWidget_();
			void initTrayIcon_();
			
			static const QStringList supportedFormats_;
			static const QString fileFilter_;
			
			ImageArea * imageArea_;
			ScaleImage * scaleImage_;
			QSystemTrayIcon * trayIcon_;
			int index_;
			QDir dir_;
			QStringList files_;
			Qt::WindowStates dumpState_;
	};
	
}

#endif
