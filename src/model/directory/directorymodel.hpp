#ifndef KOMIX_MODEL_DIRECTORY_DIRECTORYMODEL_HPP
#define KOMIX_MODEL_DIRECTORY_DIRECTORYMODEL_HPP

#include "localfilemodel.hpp"

namespace KomiX { namespace model { namespace directory {

class DirectoryModel : public LocalFileModel {
public:
	DirectoryModel( const QFileInfo & root );
};

} } } // end of namespace

#endif
