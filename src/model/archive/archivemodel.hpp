#ifndef KOMIX_MODEL_ARCHIVE_ARCHIVEMODEL_HPP
#define KOMIX_MODEL_ARCHIVE_ARCHIVEMODEL_HPP

#include "localfilemodel.hpp"

namespace KomiX { namespace model { namespace archive {

class ArchiveModel : public LocalFileModel {
public:
	static bool IsRunnable();
	static bool IsPrepared();

	ArchiveModel( const QFileInfo & root );

private:
	friend class ArchiveHook;

	static const QString & SevenZip_();
	static QStringList Arguments_( const QString & );
	static const QDir & TmpDir_();
	static QDir ArchiveDir_( const QString & );
	static void Extract_( const QString &, const QString & );
};

const QStringList & ArchiveFormats();

const QStringList & ArchiveFormatsFilter();

bool isArchiveSupported( const QString & path );
int delTree( QDir dir );

} } } // end of namespace

#endif
