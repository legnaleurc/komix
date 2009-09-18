#include "archivemodel.hpp"
#include "global.hpp"
#include "archivehook.hpp"
#include "error.hpp"

#include <QProcess>
#include <QtDebug>
#include <QDir>
#include <QApplication>
#include <QPixmap>
#include <QCryptographicHash>

namespace KomiX { namespace error {

class Archive {};
typedef Error< Archive > ArchiveError;

} } // end of namespace

namespace {

QSharedPointer< KomiX::model::FileModel > create( const QFileInfo & path ) {
	if( !KomiX::model::archive::ArchiveModel::IsRunnable() ) {
		throw KomiX::error::ArchiveError( "This feature is based on 7-zip. Please install it." );
	} else if( !KomiX::model::archive::ArchiveModel::IsPrepared() ) {
		throw KomiX::error::ArchiveError( "I could not create temporary directory." );
	}
	return QSharedPointer< KomiX::model::FileModel >( new KomiX::model::archive::ArchiveModel( path ) );
}

bool check( const QFileInfo & path ) {
	if( !path.isDir() ) {
		return KomiX::model::archive::isArchiveSupported( path.suffix().toLower() );
	} else {
		return false;
	}
}

QAction * hookHelper( QWidget * parent ) {
	return ( new KomiX::model::archive::ArchiveHook( parent ) )->action();
}

static const bool registered = KomiX::registerFileMenuHook( hookHelper ) && KomiX::model::FileModel::registerModel( check, create );

// one-shot action
QDir createTmpDir() {
	qsrand( qApp->applicationPid() );
	QString tmpPath( QString( "komix_%1" ).arg( qrand() ) );
	qDebug() << tmpPath;
	QDir tmpDir( QDir::temp() );
	if( !tmpDir.mkdir( tmpPath ) ) {
		qWarning( "can not make temp dir" );
		// tmpDir will remain to tmp dir
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
				qDebug() << e.absoluteFilePath();
				++sum;
			}
		}
	}
	dir.rmdir( dir.absolutePath() );
	qDebug() << dir.absolutePath();
	return sum + 1;
}

inline QStringList archiveList() {
	QStringList a;
	a << "7z";
	a << "rar";
//		a << "tar.bz2";
//		a << "tbz2";
//		a << "tar.gz";
//		a << "tgz";
	a << "zip";
	return a;
}

} // end of namespace

namespace KomiX { namespace model { namespace archive {

const QDir & ArchiveModel::TmpDir_() {
	static QDir tmp = createTmpDir();
	return tmp;
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
	if( !TmpDir_().exists( dirName ) ) {
		TmpDir_().mkdir( dirName );
	}
	QDir tmp( TmpDir_() );
	tmp.cd( dirName );
	return tmp;
}

bool ArchiveModel::IsRunnable() {
	return QFileInfo( SevenZip_() ).isExecutable();
}

bool ArchiveModel::IsPrepared() {
	return QDir::temp() != TmpDir_();
}

ArchiveModel::ArchiveModel( const QFileInfo & root ) {
	QProcess * p = new QProcess();
	QString hash = QString::fromUtf8( QCryptographicHash::hash( root.fileName().toUtf8(), QCryptographicHash::Sha1 ).toHex() );
	qDebug() << hash;
	qDebug() << ( Arguments_( hash ) << root.absoluteFilePath() );
	p->start( SevenZip_(), ( Arguments_( hash ) << root.absoluteFilePath() ),  QIODevice::ReadOnly );
	p->waitForFinished( -1 );

	if( p->exitCode() != 0 ) {
		qWarning() << p->readAllStandardOutput();
		qWarning() << p->readAllStandardError();
		// TODO: ERROR HERE
	} else {
		root_ = ArchiveDir_( hash );
		files_ = root_.entryList( SupportedFormatsFilter(), QDir::Files );
	}
}

ArchiveModel::~ArchiveModel() {
	int ret = deltree( TmpDir_() );
	qDebug() << ret;
}

QModelIndex ArchiveModel::index( const QString & name ) const {
	int row = files_.indexOf( QFileInfo( name ).fileName() );
	return ( row < 0 ) ? QModelIndex() : createIndex( row, 0, row );
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

const QStringList & ArchiveFormats() {
	static QStringList af = archiveList();
	return af;
}

const QStringList & ArchiveFormatsFilter() {
	static QStringList sff = toNameFilter( ArchiveFormats() );
	return sff;
}

bool isArchiveSupported( const QString & suffix ) {
	foreach( QString ext, ArchiveFormats() ) {
		if( ext == suffix ) {
			return true;
		}
	}
	return false;
}

} } } // end of namespace
