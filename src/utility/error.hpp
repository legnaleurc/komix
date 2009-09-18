#ifndef KOMIX_ERROR_HPP
#define KOMIX_ERROR_HPP

#include <QString>

#include <exception>

namespace KomiX { namespace error {

class BasicError : public std::exception {
public:
	explicit BasicError( const char * msg );
	explicit BasicError( const QString & msg );
	virtual ~BasicError() throw();
	virtual const char * what() const throw();
	const QString & getMessage() const;
private:
	QString msg_;
};

template< typename Type >
class Error : public BasicError, public Type {
public:
	Error( const char * msg ) : BasicError( msg ) {}
	Error( const QString & msg ) : BasicError( msg ) {}
};

} } // end of namespace

#endif
