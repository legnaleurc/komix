#include "error.hpp"

namespace KomiX { namespace error {

BasicError::BasicError( const char * msg ) : msg_( QString::fromUtf8( msg ) ) {}

BasicError::BasicError( const QString & msg ) : msg_( msg ) {}

BasicError::~BasicError() throw() {}

const char * BasicError::what() const throw() {
	return msg_.toLocal8Bit().constData();
}

const QString & BasicError::getMessage() const {
	return msg_;
}

} } // end of namespace
