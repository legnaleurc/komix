#include "filemodel.hpp"

#include <QMutex>
#include <QMutexLocker>

#include <algorithm>

namespace {

	static inline QMutex * lock() {
		static QMutex m;
		return &m;
	}

}

namespace KomiX {

	FileModel::FunctorList & FileModel::getFunctorList_() {
		static FileModel::FunctorList fl;
		return fl;
	}

	FileModel * FileModel::createModel( const QFileInfo & path ) {
		QMutexLocker locker( ::lock() );
		FunctorList::const_iterator it = find_if( getFunctorList_().begin(), getFunctorList_().end(), Matcher( path ) );
		if( it == getFunctorList_().end() ) {
			return NULL;
		} else {
			return it->second( path );
		}
	}

	bool FileModel::registerModel( const KeyFunctor & key, const ValueFunctor & value ) {
		QMutexLocker locker( ::lock() );
		getFunctorList_().push_back( std::make_pair( key, value ) );
		return true;
	}

	FileModel::Matcher::Matcher( const QFileInfo & path ) : path_( path ) {
	}

	bool FileModel::Matcher::operator ()( const FileModel::FunctorPair & that ) const {
		return that.first( path_ );
	}

}
