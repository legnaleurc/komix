#ifndef KOMIX_DIRECTORYMODEL_HPP
#define KOMIX_DIRECTORYMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX {

	class DirectoryModel : public FileModel {
	public:
		DirectoryModel( const QString & root );

	private:
		QDir root_;
		QStringList files_;
		int currentIndex_;
	};

}

#endif
