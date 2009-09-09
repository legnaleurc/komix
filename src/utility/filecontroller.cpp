#include "filecontroller.hpp"
#include "global.hpp"

#include <QMutexLocker>
#include <QtDebug>
#include <QProcess>
#include <QtGlobal>
#include <QApplication>

#include <cstdlib>

namespace {

	QDir createTmpDir() {
		qsrand( qApp->applicationPid() );
		QString tmpPath( QString( "komix_%1" ).arg( qrand() ) );
		QDir tmpDir( QDir::temp() );
		if( !tmpDir.mkdir( tmpPath ) ) {
			qWarning( "can not make temp dir" );
		} else {
			tmpDir.cd( tmpPath );
		}
		return tmpDir;
	}

	int rmdir( const QDir & dir ) {
#ifdef Q_OS_WIN32
		return std::system( QString( "RMDIR /S /Q %1" ).arg( QDir::toNativeSeparators( dir.absolutePath() ) ).toLocal8Bit().constData() );
#elif defined( Q_OS_UNIX )
		return QProcess::execute( "rm", QStringList() << "-rf" << dir.absolutePath() );
#endif
	}

}

namespace KomiX {

	const QDir FileControllerBase::TmpDir_ = createTmpDir();

	FileControllerBase::FileControllerBase( int pfMax, int limit, QObject * parent ) :
	QObject( parent ),
	prefetchMax_( pfMax ),
	limit_( limit ),
	dir_( QDir::home() ),
	files_(),
	index_( 0 ),
	history_(),
	cache_(),
	lock_() {
		if( limit_ < 0 ) {
			limit_ = 0;
		}
		if( prefetchMax_ < 0 ) {
			prefetchMax_ = 0;
		} else if( prefetchMax_ > limit_ ) {
			prefetchMax_ = limit_;
		}
	}

	FileControllerBase::~FileControllerBase() {
		int ret = rmdir( TmpDir_ );
		qDebug() << ret;
	}

	bool FileControllerBase::open( const QString & filePath ) {
		QMutexLocker locker( &lock_ );
		if( !update_( filePath ) || files_.empty() ) {
			return false;
		} else {
			emit getImage( fetch_( dir_.filePath( files_[index_] ) ) );
			return true;
		}
	}

	void FileControllerBase::next() {
		QMutexLocker locker( &lock_ );
		if( !files_.empty() ) {
			++index_;
			if( index_ >= files_.size() ) {
				index_ = 0;
			}
			emit getImage( fetch_( dir_.filePath( files_[index_] ) ) );
			fetch_( dir_.filePath( files_[ (index_+prefetchMax_ >= files_.size()) ? index_+prefetchMax_-files_.size() : index_+prefetchMax_ ] ) );
		}
	}

	void FileControllerBase::prev() {
		QMutexLocker locker( &lock_ );
		if( !files_.empty() ) {
			--index_;
			if( index_ < 0 ) {
				index_ = files_.size() - 1;
			}
			emit getImage( fetch_( dir_.filePath( files_[index_] ) ) );
		}
	}

	bool FileControllerBase::isEmpty() const {
		return files_.empty();
	}
	
	void FileControllerBase::setPrefetchMax( int pfMax ) {
		prefetchMax_ = ( pfMax > limit_ ? limit_ : pfMax );
	}

	int FileControllerBase::getPrefetchMax() const {
		return prefetchMax_;
	}

	void FileControllerBase::setLimit( int limit ) {
		limit_ = ( limit < 0 ? 0 : limit );
	}

	int FileControllerBase::getLimit() const {
		return limit_;
	}
	
	QString FileControllerBase::getDirPath() const {
		return dir_.path();
	}
	
	QString FileControllerBase::getFilePath() const {
		return dir_.filePath( files_[index_] );
	}

	const QPixmap & FileControllerBase::getImage( const QString & filePath ) {
		QMutexLocker locker( &lock_ );
		return fetch_( filePath );
	}

	const QPixmap & FileControllerBase::fetch_( const QString & key ) {
		if( !cache_.contains( key ) ) {
// 			qDebug() << "Cache miss:" << key;
			cache_.insert( key, QPixmap( key ) );
			history_.enqueue( key );
			if( cache_.size() > limit_ ) {
				cache_.remove( history_.dequeue() );
			}
		} else {
// 			qDebug() << "Cache hit:" << key;
		}
		return cache_[key];
	}

	void FileControllerBase::prefetch_( int index ) {
// 		qDebug( "<FileControllerBase::prefetch_>" );
		for( int i = 1; i <= prefetchMax_; ++i ) {
			if( index + i >= files_.size() ) {
				index -= files_.size();
			}
			fetch_( dir_.filePath( files_[index+i] ) );
		}
// 		qDebug( "</FileControllerBase::prefetch_>" );
	}

	bool FileControllerBase::update_( const QString & filePath ) {
		QFileInfo tmp( filePath );
		if( tmp.isDir() ) {
			if( dir_ == tmp.absoluteFilePath() ) {
				return false;
			} else {
// 				qDebug() << tmp.absoluteFilePath();
// 				qDebug() << tmp.filePath();
// 				qDebug() << tmp.absoluteDir();
// 				qDebug() << tmp.dir();
				QStringList tmpList = QDir( tmp.absoluteFilePath() ).entryList( SupportedFormatsFilter(), QDir::Files );
				if( tmpList.isEmpty() ) {
					return false;
				}
				dir_ = tmp.absoluteFilePath();
				files_ = tmpList;
				index_ = 0;
			}
		} else if( ArchiveFormats().contains( tmp.completeSuffix(), Qt::CaseInsensitive ) ) {
			QProcess * p = new QProcess();
			qDebug() << ( Arguments_( tmp.fileName() ) << tmp.absoluteFilePath() );
			p->start( SevenZip_(), ( Arguments_( tmp.fileName() ) << tmp.absoluteFilePath() ),  QIODevice::ReadOnly );
			p->waitForFinished( -1 );

			if( p->exitCode() != 0 ) {
				qWarning() << p->readAllStandardOutput();
				qWarning() << p->readAllStandardError();
				return false;
			} else {
				QDir aDir = ArchiveDir_( tmp.fileName() );
				if( dir_ == aDir ) {
					return false;
				} else {
					QStringList tmpList = aDir.entryList( SupportedFormatsFilter(), QDir::Files );
					if( tmpList.isEmpty() ) {
						return false;
					}
					dir_ = aDir;
					files_ = tmpList;
					index_ = 0;
				}
			}
		} else {
			if( dir_ == tmp.dir() ) {
				if( files_[index_] == tmp.fileName() ) {
					return false;
				} else {
					index_ = files_.indexOf( tmp.fileName() );
				}
			} else {
				dir_ = tmp.dir();
				files_ = dir_.entryList( SupportedFormatsFilter(), QDir::Files );
				index_ = files_.indexOf( tmp.fileName() );
			}
		}
		prefetch_( index_ );
		return true;
	}

	const QString & FileControllerBase::SevenZip_() {
#ifdef Q_OS_WIN32
		static QString sz = "C:\\Program Files\\7-Zip\\7z.exe";
#elif defined( Q_OS_UNIX )
		static QString sz = "/usr/bin/7z";
#endif
		return sz;
	}

	QStringList FileControllerBase::Arguments_( const QString & fileName ) {
		QStringList args( "e" );
		args << QString( "-o%1" ).arg( ArchiveDir_( fileName ).absolutePath() );
		args << "-aos";
		return args;
	}

	QDir FileControllerBase::ArchiveDir_( const QString & dirName ) {
		if( !TmpDir_.exists( dirName ) ) {
			TmpDir_.mkdir( dirName );
		}
		QDir tmp( TmpDir_ );
		tmp.cd( dirName );
		return tmp;
	}

}
