/**
 * @file mainwindow.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KOMIX_MAINWINDOW_HPP
#define KOMIX_MAINWINDOW_HPP

#include <QMainWindow>
#include <QDir>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QModelIndex>

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
		 * @param f window flags
		 */
		MainWindow( QWidget * parent = 0, Qt::WindowFlags f = 0 );

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
		void open( const QString & path );
		void open( const QModelIndex & item );
		/// open file dialog
		void openFileDialog();
		/// open directory dialog
		void openDirDialog();
//		void openArchiveDialog();
		/// toggle full-screen mode
		void toggleFullScreen();
		/// toggle minimize system tray
		void toggleSystemTray();
		/// show about
		void about();

	private slots:
		void systemTrayHelper_( QSystemTrayIcon::ActivationReason reason );

	private:
		void initMenuBar_();
		void initCentralWidget_();
		void initTrayIcon_();
		void initAbout_();

		static const QString & fileFilter_();
//		static const QString & archiveFilter_();

		ImageArea * imageArea_;
		ScaleImage * scaleImage_;
		Preview * preview_;
		QSystemTrayIcon * trayIcon_;
		QWidget * about_;
		Qt::WindowStates dumpState_;
	};
	
}

#endif
