#include "preview.hpp"
#include "global.hpp"
#include "filecontroller.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QtDebug>

namespace KomiX {

	Preview::Preview( QWidget * parent, Qt::WindowFlags f ) :
	QDialog( parent, f ),
	model_( NULL ),
	view_( NULL ),
	image_( this ) {
		setModal( true );

		view_ = new QListView( this );

		//model_ = FileController::Instance().getFileModel();
		//model_->setNameFilters( SupportedFormatsFilter() );

		view_->setModel( model_ );
		connect( view_->selectionModel(), SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( viewImage_( const QModelIndex &, const QModelIndex & ) ) );

		image_.setFixedSize( 360, 360 );
		image_.setAlignment( Qt::AlignCenter );

		QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Open | QDialogButtonBox::Cancel, Qt::Horizontal, this );
		connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
		connect( buttonBox, SIGNAL( accepted() ), this, SLOT( openHelper_() ) );

		QHBoxLayout * topFrame = new QHBoxLayout;
		topFrame->addWidget( view_ );
		topFrame->addWidget( &image_ );

		QVBoxLayout * mainFrame = new QVBoxLayout( this );
		mainFrame->addLayout( topFrame );
		mainFrame->addWidget( buttonBox );
	}

	void Preview::listDirectory() {
		// FIXME
		model_ = FileController::Instance().getFileModel();
		view_->setModel( model_ );
		view_->setRootIndex( model_->index() );
		//view_->setCurrentIndex( model_->index( 0, 1 ) );
		exec();
	}

	void Preview::openHelper_() {
		qDebug() << "Send: " << view_->currentIndex();
		emit required( view_->currentIndex() );
		accept();
	}

	void Preview::viewImage_( const QModelIndex & current, const QModelIndex & /* previous */ ) {
		qDebug( "Preview::viewImage_()" );
		qDebug() << current;
		// FIXME
		image_.setPixmap( current.data( Qt::UserRole ).value< QPixmap >().scaled( image_.size(), Qt::KeepAspectRatio ) );
	}

}
