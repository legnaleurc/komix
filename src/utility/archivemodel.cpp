#include "archivemodel.hpp"
#include "global.hpp"

#include <QProcess>
#include <QtDebug>
#include <QDir>
#include <QApplication>
#include <QPixmap>

namespace {

	KomiX::FileModel * create( const QFileInfo & path ) {
		return new KomiX::ArchiveModel( path );
	}

	bool check( const QFileInfo & path ) {
		if( !path.isDir() ) {
			return KomiX::isArchiveSupported( path.absolutePath() );
		} else {
			return false;
		}
	}

	static const bool registered = KomiX::FileModel::registerModel( check, create );

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

	int deltree( QDir dir ) {
		int sum = 0;
		QFileInfoList entry = dir.entryInfoList( QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs );
		foreach( QFileInfo e, entry ) {
			if( e.isDir() ) {
				sum += deltree( e.absoluteFilePath() );
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

	const QDir ArchiveModel::TmpDir_ = ::createTmpDir();

	ArchiveModel::ArchiveModel( const QFileInfo & root ) {
		QProcess * p = new QProcess();
		qDebug() << ( Arguments_( root.fileName() ) << root.absoluteFilePath() );
		p->start( SevenZip_(), ( Arguments_( root.fileName() ) << root.absoluteFilePath() ),  QIODevice::ReadOnly );
		p->waitForFinished( -1 );

		if( p->exitCode() != 0 ) {
			qWarning() << p->readAllStandardOutput();
			qWarning() << p->readAllStandardError();
			// TODO: ERROR HERE
		} else {
			root_ = ArchiveDir_( root.fileName() );;
			files_ = root_.entryList( SupportedFormatsFilter(), QDir::Files );
		}
	}

	ArchiveModel::~ArchiveModel() {
		int ret = ::deltree( TmpDir_ );
		qDebug() << ret;
	}

	const QString & ArchiveModel::SevenZip_() {
#ifdef Q_OS_WIN32
		static QString sz = "C:\\Program Files\\7-Zip\\7z.exe";
#elif defined( Q_OS_UNIX )
		static QString sz = "/usr/bin/7z";
#endif
		return sz;
	}

	QStringList ArchiveModel::Arguments_( const QString & fileName ) {
		QStringList args( "e" );
		args << QString( "-o%1" ).arg( ArchiveDir_( fileName ).absolutePath() );
		args << "-aos";
		return args;
	}

	QDir ArchiveModel::ArchiveDir_( const QString & dirName ) {
		if( !TmpDir_.exists( dirName ) ) {
			TmpDir_.mkdir( dirName );
		}
		QDir tmp( TmpDir_ );
		tmp.cd( dirName );
		return tmp;
	}

	QModelIndex ArchiveModel::index( int row, int column, const QModelIndex & /*parent*/ ) const {
		switch( column ) {
		case 0:
			return createIndex( 0, column, -1 );
			break;
		case 1:
			if( row < 0 || row >= files_.size() ) {
				return QModelIndex();
			} else {
				// FIXME: parameter 3 needs change
				return createIndex( row, column, row );
			}
		default:
			return QModelIndex();
		}
	}

	QModelIndex ArchiveModel::parent( const QModelIndex & child ) const {
		switch( child.column() ) {
		case 0:
			break;
		case 1:
			if( child.row() >= 0 && child.row() < files_.size() ) {
				return createIndex( 0, 0, -1 );
			}
			break;
		}
		return QModelIndex();
	}

	int ArchiveModel::rowCount( const QModelIndex & parent ) const {
		if( !parent.isValid() || parent.column() == 0 ) {
			return files_.size();
		} else {
			return 0;
		}
	}

	int ArchiveModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 0;
	}

	QVariant ArchiveModel::data( const QModelIndex & index, int role ) const {
		switch( index.column() ) {
		case 0:
			break;
		case 1:
			if( index.isValid() && index.row() >= 0 && index.row() < files_.size() ) {
				switch( role ) {
				case Qt::DisplayRole:
					return files_[index.row()];
				case Qt::UserRole:
					return QPixmap( root_.filePath( files_[index.row()] ) );
				}
			}
			break;
		}
		return QVariant();
	}

	bool ArchiveModel::hasChildren( const QModelIndex & parent ) const {
		if( !parent.isValid() || parent.column() == 0 ) {
			return !files_.empty();
		} else {
			return false;
		}
	}

}
