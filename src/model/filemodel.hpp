/**
 * @file filemodel.hpp
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
#ifndef KOMIX_MODEL_FILEMODEL_HPP
#define KOMIX_MODEL_FILEMODEL_HPP

#include <QAbstractItemModel>
#include <QUrl>
#include <QSharedPointer>

#include <list>
#include <utility>

namespace KomiX { namespace model {

class FileModel;
/// FileModel smart pointer
typedef QSharedPointer< FileModel > FileModelSP;

/**
 * @brief Abstract file model
 * 
 * Describe how to retrive files.
 */
class FileModel : public QAbstractItemModel {
public:
	/// Functor of key comparsion
	typedef bool ( * KeyFunctor )( const QUrl & );
	/// Functor of model creation
	typedef FileModelSP ( * ValueFunctor )( const QUrl & );

	/**
	 * @brief Create concrete model
	 * @param url opening url
	 */
	static FileModelSP createModel( const QUrl & url );
	/**
	 * @brief Register model
	 * @param key compare function
	 * @param value create function
	 * @return always true
	 */
	static bool registerModel( const KeyFunctor & key, const ValueFunctor & value );

	using QAbstractItemModel::index;
	/// Query the index @p url in the model
	virtual QModelIndex index( const QUrl & url ) const = 0;

private:
	typedef std::pair< KeyFunctor, ValueFunctor > FunctorPair;
	typedef std::list< FunctorPair > FunctorList;

	class Matcher {
	public:
		Matcher( const QUrl & );
		bool operator ()( const FunctorPair & ) const;
	private:
		QUrl url_;
	};

	static FunctorList & getFunctorList_();
};

} } // end namespace

#endif
