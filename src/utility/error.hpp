/**
 * @file error.hpp
 * @author Wei-Cheng Pan
 *
 * KomiX, a comics viewer.
 * Copyright (C) 2008  Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KOMIX_ERROR_HPP
#define KOMIX_ERROR_HPP

#include <QString>

#include <exception>

namespace KomiX { namespace error {

/**
 * @brief Basic error type
 */
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

/**
 * @brief Customize error base type
 * @tparam Type customize point
 */
template< typename Type >
class Error : public BasicError, public Type {
public:
	Error( const char * msg ) : BasicError( msg ) {}
	Error( const QString & msg ) : BasicError( msg ) {}
};

} } // end of namespace

#endif
