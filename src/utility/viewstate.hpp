/**
 * @file viewstate.hpp
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
#ifndef KOMIX_UTILITY_VIEWSTATE_HPP
#define KOMIX_UTILITY_VIEWSTATE_HPP

#include <QtCore/QLineF>

#include <boost/signals2.hpp>

#include <functional>
#include <memory>

namespace KomiX {

class ViewState {
public:
    explicit ViewState( std::function< QLineF () > gmv );

    QLineF getMotionVector() const;

    boost::signals2::signal< void () > & next();
    boost::signals2::signal< void () > & previous();
    boost::signals2::signal< void () > & first();
    boost::signals2::signal< void () > & last();
    boost::signals2::signal< void () > & top();
    boost::signals2::signal< void () > & bottom();
    boost::signals2::signal< void () > & right();
    boost::signals2::signal< void () > & left();
    boost::signals2::signal< void () > & wider();
    boost::signals2::signal< void () > & narrower();
    boost::signals2::signal< void () > & higher();
    boost::signals2::signal< void () > & lower();

private:
    ViewState( const ViewState & );
    ViewState & operator =( const ViewState & );
    class Private;
    std::shared_ptr< Private > p_;
};

}

#endif
