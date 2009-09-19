#ifndef KOMIX_MODEL_ARCHIVE_ARCHIVEMODEL_HPP
#define KOMIX_MODEL_ARCHIVE_ARCHIVEMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX { namespace model { namespace archive {

class ArchiveModel : public FileModel {
public:
	static bool IsRunnable();
	static bool IsPrepared();

	ArchiveModel( const QFileInfo & root );

	virtual QModelIndex index( const QString & name ) const;

	virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex & child ) const;
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

private:
	friend class ArchiveHook;

	static const QString & SevenZip_();
	static QStringList Arguments_( const QString & );
	static const QDir & TmpDir_();
	static QDir ArchiveDir_( const QString & );
	static void Extract_( const QString &, const QString & );

	QDir root_;
	QStringList files_;
};

const QStringList & ArchiveFormats();

const QStringList & ArchiveFormatsFilter();

bool isArchiveSupported( const QString & path );
int delTree( QDir dir );

} } } // end of namespace

#endif
