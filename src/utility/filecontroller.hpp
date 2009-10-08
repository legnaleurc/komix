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

#include <QPixmap>
#include <QModelIndex>

namespace KomiX {

/**
 * @brief The base file controller
 * @warning <strong style="color: red;">DO NOT USE THIS CLASS TO CREATE OBJECT!</strong>
 *
 * This class is used to provide a generic open image action, and
 * another important function: <strong>image caching</strong>.
 */
class FileController : public QObject {
	Q_OBJECT

public:
	/**
	 * @brief default constructor
	 * @param pfMax max prefetch count
	 * @param limit max cache count
	 * @param parent parent widget
	 *
	 * The cache count @p limit is including prefetch count @p pfMax. \n
	 * If @p pfMax and @p limit are less than 0, it will sets to 0.\n
	 * If @p pfMax is greater than @p limit, it will sets to @p limit.
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

	QSharedPointer< model::FileModel > getModel() const;
	QModelIndex getCurrentIndex() const;

	/**
	 * @brief get image by file path
	 * @param filePath file path
	 * @return opened image
	 * @sa getImage( const QPixmap & )
	 *
	 * This function well update cache list, that's why it's not
	 * const member function.
	 */
	//const QPixmap & getImage( const QString & filePath );

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
	void open( const QModelIndex & index );

signals:
	/**
	 * @brief get cached image
	 * @param image cached image
	 * @sa getImage( const QString & )
	 */
	void imageLoaded( const QPixmap & image );
	void errorOccured( const QString & errMsg );

private:
	int index_;

	QSharedPointer< model::FileModel > model_;
};

} // end of KomiX

#endif
