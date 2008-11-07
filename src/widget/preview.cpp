#include "preview.hpp"

#include <QHBoxLayout>

namespace KomiX {

	Preview::Preview( QWidget * parent, Qt::WindowFlags f ) :
	QDialog( parent, f ),
	model_( this ),
	view_( this ) {
		setModal( true );

		view_.setModel( &model_ );

		QHBoxLayout * mainFrame = new QHBoxLayout( this );
		mainFrame->addWidget( &view_ );
	}

	void Preview::listDirectory( const QString & path ) {
		view_.setRootIndex( model_.index( path ) );
		exec();
	}

}
