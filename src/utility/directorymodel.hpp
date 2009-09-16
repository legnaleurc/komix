#ifndef KOMIX_DIRECTORYMODEL_HPP
#define KOMIX_DIRECTORYMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX {

	class DirectoryModel : public FileModel {
	public:
		DirectoryModel( const QFileInfo & root );

		virtual QModelIndex index() const;

		virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
		virtual QModelIndex parent( const QModelIndex & child ) const;
		virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
		virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
		virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

	private:
		QDir root_;
		QStringList files_;
	};

}

#endif
