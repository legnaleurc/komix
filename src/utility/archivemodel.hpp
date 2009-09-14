#ifndef KOMIX_ARCHIVEMODEL_HPP
#define KOMIX_ARCHIVEMODEL_HPP

#include "filemodel.hpp"

namespace KomiX {

	class ArchiveModel : public FileModel {
	public:
		ArchiveModel( const QString & root );
	};

}

#endif
