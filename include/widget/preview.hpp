/**
 * @file preview.hpp
 */
#ifndef KOMIX_PREVIEW_HPP
#define KOMIX_PREVIEW_HPP

#include <QDialog>
#include <QListView>
#include <QDirModel>
#include <QLabel>

namespace KomiX {

	/**
	 * @brief Preview and goto widget
	 *
	 * This widget can preview other images in same dicrectory, and
	 * open which you want.
	 */
	class Preview : public QDialog {
		Q_OBJECT
	
	public:
		/**
		 * @brief default constructor
		 * @param parent parent widget
		 * @param f window flags
		 */
		Preview( QWidget * parent = 0, Qt::WindowFlags f = 0 );

		/**
		 * @brief list all supported files in opened directory
		 * @param dirPath current directory path
		 * @param filePath current opened file path
		 */
		void listDirectory();
	
	signals:
		/**
		 * @brief open file
		 * @param path file path
		 */
		void open( const QString & path );
	
	private:
		QDirModel model_;
		QListView view_;
		QLabel image_;
	
	private slots:
		void openHelper_();
		void viewImage_( const QModelIndex & current, const QModelIndex & previous );
	};

}

#endif
