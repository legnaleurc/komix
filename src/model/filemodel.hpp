#ifndef KOMIX_MODEL_FILEMODEL_HPP
#define KOMIX_MODEL_FILEMODEL_HPP

#include <QAbstractItemModel>
#include <QUrl>
#include <QSharedPointer>

#include <list>
#include <utility>

namespace KomiX { namespace model {

class FileModel : public QAbstractItemModel {
public:
	typedef bool ( * KeyFunctor )( const QUrl & );
	typedef QSharedPointer< FileModel > ( * ValueFunctor )( const QUrl & );

	static QSharedPointer< FileModel > createModel( const QUrl & url );
	static bool registerModel( const KeyFunctor & key, const ValueFunctor & value );

	using QAbstractItemModel::index;
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
