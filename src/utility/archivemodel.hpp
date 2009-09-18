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

		virtual QModelIndex index( const QString & name ) const;

		virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
		virtual QModelIndex parent( const QModelIndex & child ) const;
		virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
		virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
		virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

	private:
		static const QString & SevenZip_();
		static QStringList Arguments_( const QString & );
		static QDir ArchiveDir_( const QString & );
		static const QDir TmpDir_;
		static bool ok_();

		QDir root_;
		QStringList files_;
	};

	const QStringList & ArchiveFormats();

	const QStringList & ArchiveFormatsFilter();

	bool isArchiveSupported( const QString & path );

}

#endif
