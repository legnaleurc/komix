#include "filecontroller.hpp"
#include "global.hpp"

#include <QMutexLocker>
#include <QtDebug>
#include <QProcess>
#include <QtGlobal>
#include <QApplication>
#include <QFileInfo>

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

	int rmdir( QDir dir ) {
		int sum = 0;
		QFileInfoList entry = dir.entryInfoList( QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs );
		foreach( QFileInfo e, entry ) {
			if( e.isDir() ) {
				sum += rmdir( e.absoluteFilePath() );
			} else {
				if( QFile::remove( e.absoluteFilePath() ) ) {
					++sum;
				}
			}
		}
		dir.rmdir( dir.absolutePath() );
		return sum + 1;
	}

}

namespace KomiX {

	namespace private_ {

		const QDir FileController::TmpDir_ = createTmpDir();

		FileController::FileController( int pfMax, int limit, QObject * parent ) :
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

		FileController::~FileController() {
			int ret = rmdir( TmpDir_ );
			qDebug() << ret;
		}

		bool FileController::open( const QString & filePath ) {
			QMutexLocker locker( &lock_ );
			// check if filePath has been load, to avoid reload event
			if( !updateModel_( filePath ) || files_.empty() ) {
				return false;
			} else {
				emit imageLoaded( fetch_( dir_.filePath( files_[index_] ) ) );
				return true;
			}
		}

		void FileController::next() {
			QMutexLocker locker( &lock_ );
			if( !files_.empty() ) {
				++index_;
				if( index_ >= files_.size() ) {
					index_ = 0;
				}
				emit imageLoaded( fetch_( dir_.filePath( files_[index_] ) ) );
				fetch_( dir_.filePath( files_[ (index_+prefetchMax_ >= files_.size()) ? index_+prefetchMax_-files_.size() : index_+prefetchMax_ ] ) );
			}
		}

		void FileController::prev() {
			QMutexLocker locker( &lock_ );
			if( !files_.empty() ) {
				--index_;
				if( index_ < 0 ) {
					index_ = files_.size() - 1;
				}
				emit imageLoaded( fetch_( dir_.filePath( files_[index_] ) ) );
			}
		}

		bool FileController::isEmpty() const {
			return files_.empty();
		}

		void FileController::setPrefetchMax( int pfMax ) {
			prefetchMax_ = ( pfMax > limit_ ? limit_ : pfMax );
		}

		int FileController::getPrefetchMax() const {
			return prefetchMax_;
		}

		void FileController::setLimit( int limit ) {
			limit_ = ( limit < 0 ? 0 : limit );
		}

		int FileController::getLimit() const {
			return limit_;
		}

		QString FileController::getDirPath() const {
			return dir_.path();
		}

		QString FileController::getFilePath() const {
			return dir_.filePath( files_[index_] );
		}

		const QPixmap & FileController::getImage( const QString & filePath ) {
			QMutexLocker locker( &lock_ );
			return fetch_( filePath );
		}

		const QPixmap & FileController::fetch_( const QString & key ) {
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

		void FileController::prefetch_( int index ) {
	// 		qDebug( "<FileController::prefetch_>" );
			for( int i = 1; i <= prefetchMax_; ++i ) {
				if( index + i >= files_.size() ) {
					index -= files_.size();
				}
				fetch_( dir_.filePath( files_[index+i] ) );
			}
	// 		qDebug( "</FileController::prefetch_>" );
		}

		bool FileController::updateModel_( const QString & filePath ) {
			QFileInfo tmp( filePath );
			fileModel_ = FileModel::createModel( tmp );
			if( tmp.isDir() ) {
				// use directory model
//				archiveModel_ = new DirectoryModel( tmp.absoluteFilePath() );
//				if( dir_ == tmp.absoluteFilePath() ) {
//					return false;
//				} else {
	// 				qDebug() << tmp.absoluteFilePath();
	// 				qDebug() << tmp.filePath();
	// 				qDebug() << tmp.absoluteDir();
	// 				qDebug() << tmp.dir();
//					QStringList tmpList = QDir( tmp.absoluteFilePath() ).entryList( SupportedFormatsFilter(), QDir::Files );
//					if( tmpList.isEmpty() ) {
//						return false;
//					}
//					dir_ = tmp.absoluteFilePath();
//					files_ = tmpList;
//					index_ = 0;
//				}
			} else if( isArchiveSupported( tmp.absoluteFilePath() ) ) {
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

		const QString & FileController::SevenZip_() {
	#ifdef Q_OS_WIN32
			static QString sz = "C:\\Program Files\\7-Zip\\7z.exe";
	#elif defined( Q_OS_UNIX )
			static QString sz = "/usr/bin/7z";
	#endif
			return sz;
		}

		QStringList FileController::Arguments_( const QString & fileName ) {
			QStringList args( "e" );
			args << QString( "-o%1" ).arg( ArchiveDir_( fileName ).absolutePath() );
			args << "-aos";
			return args;
		}

		QDir FileController::ArchiveDir_( const QString & dirName ) {
			if( !TmpDir_.exists( dirName ) ) {
				TmpDir_.mkdir( dirName );
			}
			QDir tmp( TmpDir_ );
			tmp.cd( dirName );
			return tmp;
		}

	}

}
