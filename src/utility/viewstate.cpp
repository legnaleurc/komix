#include "viewstate.hpp"

namespace KomiX {
class ViewState::Private {
public:
	std::function< QLineF () > gmv;
	boost::signals2::signal< void () > next;
	boost::signals2::signal< void () > previous;
	boost::signals2::signal< void () > first;
	boost::signals2::signal< void () > last;
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
