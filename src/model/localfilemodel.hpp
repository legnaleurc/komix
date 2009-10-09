/**
 * @file localfilemodel.hpp
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
#ifndef KOMIX_MODEL_LOCALFILEMODEL_HPP
#define KOMIX_MODEL_LOCALFILEMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX { namespace model {

/**
 * @brief The model to retrive files in local
 */
class LocalFileModel : public FileModel {
public:
	LocalFileModel( const QDir & root = QDir() );

	virtual QModelIndex index( const QUrl & url ) const;

	virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex & child ) const;
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

protected:
	void setRoot( const QDir & root );

private:
	QDir root_;
	QStringList files_;
};

} } // end of namespace

#endif
