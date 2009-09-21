#ifndef KOMIX_MODEL_SINGLE_SINGLEMODEL_HPP
#define KOMIX_MODEL_SINGLE_SINGLEMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX { namespace model { namespace single {

class SingleModel : public FileModel {
public:
	SingleModel( const QFileInfo & root );

	virtual QModelIndex index( const QUrl & url ) const;

	virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex & child ) const;
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

private:
	QDir root_;
	QStringList files_;
};

} } } //end of namespace

#endif
