/**
 * @file navigator.hpp
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
#ifndef KOMIX_WIDGET_PREVIEW_HPP
#define KOMIX_WIDGET_PREVIEW_HPP

#include "filemodel.hpp"

#include <QDialog>
#include <QListView>
#include <QItemSelectionModel>
#include <QLabel>

namespace KomiX { namespace widget {

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
	void popup();

signals:
	/**
	 * @brief open file
	 * @param filePath file path
	 */
	void required( const QModelIndex & item );

private:
	QSharedPointer< model::FileModel > model_;
	QListView * view_;
	QItemSelectionModel * selection_;
	QLabel image_;

private slots:
	void openHelper_();
	void viewImage_( const QModelIndex &, const QModelIndex & );
};

} } // end namespace

#endif
