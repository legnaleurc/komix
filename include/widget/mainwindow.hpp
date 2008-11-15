/**
 * @file mainwindow.hpp
 */
#ifndef KOMIX_MAINWINDOW_HPP
#define KOMIX_MAINWINDOW_HPP

#include <QMainWindow>
#include <QDir>
#include <QStringList>
#include <QSystemTrayIcon>

namespace KomiX {

	class ImageArea;
	class ScaleImage;
	class Preview;

	/**
	 * @brief Main window
	 *
	 * The main window widget, all GUI components are managed by this.
	 */
	class MainWindow : public QMainWindow {
		Q_OBJECT
	
	public:
		/**
		 * @brief default constructor
		 * @param parent parent widget
		 * @param flags window flags
		 */
		MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );

	public slots:
		/**
		 * @brief whell scroll evnet
		 * @param delta distance of scrolling
		 */
		void whellAction( int delta );
		/**
		 * @brief open file
		 * @param filePath file path
		 */
		void open( const QString & filePath );
		/// open file dialog
		void openFileDialog();
		/// open directory dialog
		void openDirDialog();
		/// toggle full-screen mode
		void toggleFullScreen();
		/// toggle minimize system tray
		void toggleSystemTray();
		/// show about
		void about();

	private slots:
		void systemTrayHelper_( QSystemTrayIcon::ActivationReason reason );
		void previewHelper_();

	private:
		void initMenuBar_();
		void initCentralWidget_();
		void initTrayIcon_();

		static const QString fileFilter_;

		ImageArea * imageArea_;
		ScaleImage * scaleImage_;
		Preview * preview_;
		QSystemTrayIcon * trayIcon_;
		Qt::WindowStates dumpState_;
	};
	
}

#endif
