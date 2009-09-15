#include "filemodel.hpp"

#include <QMutex>
#include <QMutexLocker>

#include <algorithm>

namespace {

	static QMutex lock;

}

namespace KomiX {

	FileModel::FunctorList FileModel::functorList_;

	FileModel * FileModel::createModel( const QFileInfo & path ) {
		QMutexLocker locker( &::lock );
		FunctorList::const_iterator it = find_if( functorList_.begin(), functorList_.end(), Matcher( path ) );
		return it->second( path );
	}

	bool FileModel::registerModel( const KeyFunctor & key, const ValueFunctor & value ) {
		QMutexLocker locker( &::lock );
		functorList_.push_back( std::make_pair( key, value ) );
		return true;
	}

	FileModel::Matcher::Matcher( const QFileInfo & path ) : path_( path ) {
	}

	bool FileModel::Matcher::operator ()( const FileModel::FunctorPair & that ) const {
		return that.first( path_ );
	}

	bool FileModel::isEmpty() const {
		return files_.empty();
	}

	void FileModel::setFileList( const QStringList & files ) {
		files_ = files;
	}

	const QStringList & FileModel::getFileList() const {
		return files_;
	}

}
