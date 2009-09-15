#ifndef KOMIX_ARCHIVEMODEL_HPP
#define KOMIX_ARCHIVEMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX {

	class ArchiveModel : public FileModel {
	public:
		ArchiveModel( const QFileInfo & root );
		virtual ~ArchiveModel();

	private:
		static const QString & SevenZip_();
		static QStringList Arguments_( const QString & );
		static QDir ArchiveDir_( const QString & );
		static const QDir TmpDir_;

		QDir dir_;
		QStringList files_;
		int index_;
	};

}

#endif
