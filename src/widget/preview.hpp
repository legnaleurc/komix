/**
 * @file preview.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KOMIX_PREVIEW_HPP
#define KOMIX_PREVIEW_HPP

#include "filemodel.hpp"

#include <QDialog>
#include <QListView>
#include <QItemSelectionModel>
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

	public slots:
		/**
		 * @brief list all supported files in opened directory
		 *
		 * The list is locked in current opened directory.
		 * It will focus current opened file first.
		 */
		void listDirectory();
	
	signals:
		/**
		 * @brief open file
		 * @param filePath file path
		 */
		void required( const QModelIndex & item );
	
	private:
		QSharedPointer< FileModel > model_;
		QListView * view_;
		QItemSelectionModel * selection_;
		QLabel image_;
	
	private slots:
		void openHelper_();
		void viewImage_( const QModelIndex &, const QModelIndex & );
	};

}

#endif
