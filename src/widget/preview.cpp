#include "preview.hpp"
#include "global.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>

namespace KomiX {

	Preview::Preview( QWidget * parent, Qt::WindowFlags f ) :
	QDialog( parent, f ),
	model_( SupportedFormatsFilter(), QDir::Files, QDir::Name, this ),
	view_( this ) {
		setModal( true );

		model_.setNameFilters( SupportedFormatsFilter() );
		view_.setModel( &model_ );

		QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Open | QDialogButtonBox::Cancel, Qt::Horizontal, this );
		connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

		QHBoxLayout * topFrame = new QHBoxLayout;
		topFrame->addWidget( &view_ );

		QVBoxLayout * mainFrame = new QVBoxLayout( this );
		mainFrame->addLayout( topFrame );
		mainFrame->addWidget( buttonBox );
	}

	void Preview::listDirectory( const QString & dirPath, const QString & filePath ) {
		view_.setRootIndex( model_.index( dirPath ) );
		view_.setCurrentIndex( model_.index( filePath ) );
		exec();
	}

}
