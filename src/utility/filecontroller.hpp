/**
 * @file filecontroller.hpp
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
#ifndef KOMIX_FILECONTROLLER_HPP
#define KOMIX_FILECONTROLLER_HPP

#include "filemodel.hpp"

class QPixmap;
class QModelIndex;
class QMovie;

namespace KomiX {

/**
 * @brief The file controller
 *
 * This class is used to provide a generic open image action.
 */
class FileController : public QObject {
	Q_OBJECT

public:
	/**
	 * @brief default constructor
	 * @param parent parent widget
	 */
	FileController( QObject * parent = 0 );

	/**
	 * @brief open a url
	 * @param url url
	 * @retval true emited getImage( const QPixmap & )
	 * @retval false nothing happend
	 *
	 * It will emit getImage( const QPixmap & ) if necessary.
	 */
	bool open( const QUrl & url );

	/// check if there has openable files.
	bool isEmpty() const;

	/// get current model
	model::FileModelSP getModel() const;
	/// get current index
	QModelIndex getCurrentIndex() const;

public slots:
	/**
	 * @brief go to next image
	 * @sa prev()
	 *
	 * This function well emit getImage( const QPixmap & ), and
	 * prefetch images.
	 */
	void next();
	/**
	 * @brief go to previous image
	 * @sa next()
	 *
	 * This function well emit getImage( const QPixmap & ).
	 */
	void prev();
	/**
	 * @brief open @p index
	 *
	 * If successful, signal imageLoaded is emitted.
	 */
	void open( const QModelIndex & index );

signals:
	/**
	 * @brief get image
	 * @param image image
	 */
	void imageLoaded( const QPixmap & image );
	void imageLoaded( QMovie * anime );
	/**
	 * @brief Some error occured
	 * @param errMsg error message
	 */
	void errorOccured( const QString & errMsg );

private:
	void fromIndex_( const QModelIndex & );
	int index_;

	model::FileModelSP model_;
};

} // end of KomiX

#endif
