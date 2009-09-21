#include "filemodel.hpp"

#include <QMutex>
#include <QMutexLocker>

#include <algorithm>

namespace {

static inline QMutex * lock() {
	static QMutex m;
	return &m;
}

} // end of namespace

namespace KomiX { namespace model {

FileModel::FunctorList & FileModel::getFunctorList_() {
	static FileModel::FunctorList fl;
	return fl;
}

QSharedPointer< FileModel > FileModel::createModel( const QUrl & url ) {
	QMutexLocker locker( ::lock() );
	FunctorList::const_iterator it = find_if( getFunctorList_().begin(), getFunctorList_().end(), Matcher( url ) );
	if( it == getFunctorList_().end() ) {
		return QSharedPointer< FileModel >();
	} else {
		return it->second( url );
	}
}

bool FileModel::registerModel( const KeyFunctor & key, const ValueFunctor & value ) {
	QMutexLocker locker( ::lock() );
	getFunctorList_().push_back( std::make_pair( key, value ) );
	return true;
}

FileModel::Matcher::Matcher( const QUrl & url ) : url_( url ) {
}

bool FileModel::Matcher::operator ()( const FileModel::FunctorPair & that ) const {
	return that.first( url_ );
}

} } // end of namespace
