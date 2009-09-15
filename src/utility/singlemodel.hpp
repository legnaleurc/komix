#ifndef KOMIX_SINGLEMODEL_HPP
#define KOMIX_SINGLEMODEL_HPP

#include "filemodel.hpp"

#include <QDir>
#include <QStringList>

namespace KomiX {

	class SingleModel : public FileModel {
	public:
		SingleModel( const QFileInfo & root );

	private:
		QDir dir_;
		QStringList files_;
		int index_;
	};

}

#endif
