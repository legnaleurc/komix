/**
 * @file archivemodel.cpp
 * @author Wei-Cheng Pan
 *
 * KomiX, a comics viewer.
 * Copyright (C) 2008  Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "archivehook.hpp"
#include "archivemodel.hpp"
#include "exception.hpp"
#include "global.hpp"

#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtDebug>
#include <QtGui/QApplication>
#include <QtGui/QPixmap>

namespace KomiX {
namespace exception {

/// Private archive error class
class ArchiveException: public Exception {
public:
	ArchiveException( const char * msg ): Exception( msg ) {
	}
	ArchiveException( const QString & msg ): Exception( msg ) {
	}
};

}
} // end of namespace

namespace {

	bool check( const QUrl & url ) {
		if( url.scheme() == "file" ) {
			QFileInfo fi( url.toLocalFile() );
			if( !fi.isDir() ) {
				return KomiX::model::archive::isArchiveSupported( fi.fileName().toLower() );
			}
		}
		return false;
	}

	KomiX::model::FileModelSP create( const QUrl & url ) {
		if( !KomiX::model::archive::ArchiveModel::IsRunnable() ) {
			throw KomiX::exception::ArchiveException( "This feature is based on 7-zip. Please install it." );
		} else if( !KomiX::model::archive::ArchiveModel::IsPrepared() ) {
			throw KomiX::exception::ArchiveException( "I could not create temporary directory." );
		}
		return KomiX::model::FileModelSP( new KomiX::model::archive::ArchiveModel( QFileInfo( url.toLocalFile() ) ) );
	}

	static const bool registered = KomiX::model::FileModel::registerModel( check, create );

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

	inline const QStringList & archiveList2() {
		static QStringList a2 = QStringList() << "tar.gz" << "tgz" << "tar.bz2" << "tbz2" << "tar.lzma";
		return a2;
	}

	inline bool isTwo( const QString & name ) {
		foreach( QString ext, archiveList2() ) {
			if( name.endsWith( ext ) ) {
				return true;
			}
		}
		return false;
	}

	inline QStringList archiveList() {
		QStringList a( archiveList2() );
		a << "7z";
		a << "rar";
		a << "zip";
		a << "tar";
		return a;
	}

} // end of namespace

using namespace KomiX::model::archive;

const QDir & ArchiveModel::TmpDir_() {
	static QDir tmp = createTmpDir();
	return tmp;
}

const QString & ArchiveModel::SevenZip_() {
#ifdef Q_OS_WIN32
	static QString sz = "C:\\Program Files\\7-Zip\\7z.exe";
#elif defined( Q_OS_UNIX )
	static QString sz = "/usr/local/bin/7z";
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

void ArchiveModel::Extract_( const QString & hash, const QString & aFilePath ) {
	QSharedPointer< QProcess > p( new QProcess );
	p->start( SevenZip_(), ( Arguments_( hash ) << aFilePath ),  QIODevice::ReadOnly );
	p->waitForFinished( -1 );
	if( p->exitCode() != 0 ) {
		// delete wrong dir
		delTree( ArchiveDir_( hash ) );
		QString err = QString::fromLocal8Bit( p->readAllStandardError() );
		qWarning() << p->readAllStandardOutput();
		qWarning() << err;
		throw exception::ArchiveException( err );
	}
}

bool ArchiveModel::IsRunnable() {
	return QFileInfo( SevenZip_() ).isExecutable();
}

bool ArchiveModel::IsPrepared() {
	return QDir::temp() != TmpDir_();
}

ArchiveModel::ArchiveModel( const QFileInfo & root ) {
	QString hash = QString::fromUtf8( QCryptographicHash::hash( root.fileName().toUtf8(), QCryptographicHash::Sha1 ).toHex() );
	qDebug() << hash;

	if( !TmpDir_().exists( hash ) ) {
		Extract_( hash, root.absoluteFilePath() );
		// check if is tar-compressed
		if( isTwo( root.fileName() ) ) {
			QString name = ArchiveDir_( hash ).absoluteFilePath( root.completeBaseName() );
			Extract_( hash, name );
		}
	}

	setRoot( ArchiveDir_( hash ) );
}

namespace KomiX {
	namespace model {
		namespace archive {

			const QStringList & ArchiveFormats() {
				static QStringList af = archiveList();
				return af;
			}

			const QStringList & ArchiveFormatsFilter() {
				static QStringList sff = toNameFilter( ArchiveFormats() );
				return sff;
			}

			bool isArchiveSupported( const QString & name ) {
				foreach( QString ext, ArchiveFormats() ) {
					if( name.endsWith( ext ) ) {
						return true;
					}
				}
				return false;
			}

			int delTree( QDir dir ) {
				int sum = 0;
				QFileInfoList entry = dir.entryInfoList( QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs );
				foreach( QFileInfo e, entry ) {
					if( e.isDir() ) {
						sum += delTree( e.absoluteFilePath() );
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

		}
	}
}
