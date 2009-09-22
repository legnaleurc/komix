#ifndef KOMIX_MODEL_SINGLE_SINGLEMODEL_HPP
#define KOMIX_MODEL_SINGLE_SINGLEMODEL_HPP

#include "localfilemodel.hpp"

namespace KomiX { namespace model { namespace single {

class SingleModel : public LocalFileModel {
public:
	SingleModel( const QFileInfo & root );
};

} } } //end of namespace

#endif
