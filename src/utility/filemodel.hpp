#ifndef KOMIX_FILEMODEL_HPP
#define KOMIX_FILEMODEL_HPP

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QSharedPointer>

#include <list>
#include <utility>

namespace KomiX {

	class FileModel : public QAbstractItemModel {
	public:
		typedef bool ( * KeyFunctor )( const QFileInfo & );
		typedef QSharedPointer< FileModel > ( * ValueFunctor )( const QFileInfo & );

		static QSharedPointer< FileModel > createModel( const QFileInfo & path );
		static bool registerModel( const KeyFunctor & key, const ValueFunctor & value );

		using QAbstractItemModel::index;
		virtual QModelIndex index( const QString & name ) const = 0;

	private:
		typedef std::pair< KeyFunctor, ValueFunctor > FunctorPair;
		typedef std::list< FunctorPair > FunctorList;

		class Matcher {
		public:
			Matcher( const QFileInfo & );
			bool operator ()( const FunctorPair & ) const;
		private:
			QFileInfo path_;
		};

		static FunctorList & getFunctorList_();
	};

}

#endif
