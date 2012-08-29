/**
 * @file viewstate.cpp
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
#include "viewstate.hpp"

namespace KomiX {
class ViewState::Private {
public:
	std::function< QLineF () > gmv;
	boost::signals2::signal< void () > next;
	boost::signals2::signal< void () > previous;
	boost::signals2::signal< void () > first;
	boost::signals2::signal< void () > last;
	boost::signals2::signal< void () > top;
	boost::signals2::signal< void () > bottom;
	boost::signals2::signal< void () > right;
	boost::signals2::signal< void () > left;
	boost::signals2::signal< void () > wider;
	boost::signals2::signal< void () > narrower;
	boost::signals2::signal< void () > higher;
	boost::signals2::signal< void () > lower;
};
}

using KomiX::ViewState;

ViewState::ViewState( std::function< QLineF () > gmv ):
p_( new Private ) {
	this->p_->gmv = gmv;
}

QLineF ViewState::getMotionVector() const {
	return this->p_->gmv();
}

boost::signals2::signal< void () > & ViewState::next() {
	return this->p_->next;
}

boost::signals2::signal< void () > & ViewState::previous() {
	return this->p_->previous;
}

boost::signals2::signal< void () > & ViewState::first() {
	return this->p_->first;
}

boost::signals2::signal< void () > & ViewState::last() {
	return this->p_->last;
}

boost::signals2::signal< void () > & ViewState::top() {
	return this->p_->top;
}

boost::signals2::signal< void () > & ViewState::bottom() {
	return this->p_->bottom;
}

boost::signals2::signal< void () > & ViewState::right() {
	return this->p_->right;
}

boost::signals2::signal< void () > & ViewState::left() {
	return this->p_->left;
}

boost::signals2::signal< void () > & ViewState::wider() {
	return this->p_->wider;
}

boost::signals2::signal< void () > & ViewState::narrower() {
	return this->p_->narrower;
}

boost::signals2::signal< void () > & ViewState::higher() {
	return this->p_->higher;
}

boost::signals2::signal< void () > & ViewState::lower() {
	return this->p_->lower;
}
