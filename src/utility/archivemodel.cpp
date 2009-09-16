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

	QModelIndex ArchiveModel::index() const {
		return QModelIndex();
	}


	QModelIndex ArchiveModel::index( int row, int column, const QModelIndex & parent ) const {
		if( !parent.isValid() ) {
			// query from root
			if( column == 0 && row >= 0 && row < files_.size() ) {
				return createIndex( row, 0, row );
			} else {
				return QModelIndex();
			}
		} else {
			// other node has no child
			return QModelIndex();
		}
	}

	QModelIndex ArchiveModel::parent( const QModelIndex & child ) const {
		if( !child.isValid() ) {
			// root has no parent
			return QModelIndex();
		} else {
			if( child.column() == 0 && child.row() >= 0 && child.row() < files_.size() ) {
				return QModelIndex();
			} else {
				return QModelIndex();
			}
		}
	}

	int ArchiveModel::rowCount( const QModelIndex & parent ) const {
		if( !parent.isValid() ) {
			// root row size
			return files_.size();
		} else {
			// others are leaf
			return 0;
		}
	}

	int ArchiveModel::columnCount( const QModelIndex & /*parent*/ ) const {
		return 1;
	}

	QVariant ArchiveModel::data( const QModelIndex & index, int role ) const {
		if( !index.isValid() ) {
			return QVariant();
		}
		switch( index.column() ) {
		case 0:
			if( index.row() >= 0 && index.row() < files_.size() ) {
				switch( role ) {
				case Qt::DisplayRole:
					return files_[index.row()];
				case Qt::UserRole:
					return QPixmap( root_.filePath( files_[index.row()] ) );
				default:
					return QVariant();
				}
			} else {
				return QVariant();
			}
		default:
			return QVariant();
		}
	}

}
