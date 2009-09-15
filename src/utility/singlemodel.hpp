#ifndef KOMIX_SINGLEMODEL_HPP
#define KOMIX_SINGLEMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX {

	class SingleModel : public FileModel {
	public:
		SingleModel( const QFileInfo & root );

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
