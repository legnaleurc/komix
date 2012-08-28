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
