#include "scaleimage.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>

namespace KomiX {

	ScaleImage::ScaleImage( QWidget * parent, Qt::WindowFlags f ):
	QDialog( parent, f ),
	fitness_( new QButtonGroup( this ) ),
	scaleSlider_( new QSlider( Qt::Horizontal, this ) ) {
		QVBoxLayout * mainLayout = new QVBoxLayout( this );
		setLayout( mainLayout );

		QGroupBox * fit = new QGroupBox( tr( "Fitness" ), this );
		mainLayout->addWidget( fit );

		QVBoxLayout * fitBox = new QVBoxLayout;
		fit->setLayout( fitBox );

		QRadioButton * fitOrigin = new QRadioButton( tr( "Original size" ), this );
		fitness_->addButton( fitOrigin );
		fitness_->setId( fitOrigin, Origin );
		fitBox->addWidget( fitOrigin );
		connect( fitOrigin, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );
		QRadioButton * fitWidth = new QRadioButton( tr( "Fit to width" ), this );
		fitness_->addButton( fitWidth );
		fitness_->setId( fitWidth, Width );
		fitBox->addWidget( fitWidth );
		connect( fitWidth, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );
		QRadioButton * fitHeight = new QRadioButton( tr( "Fit to height" ), this );
		fitness_->addButton( fitHeight );
		fitness_->setId( fitHeight, Height );
		fitBox->addWidget( fitHeight );
		connect( fitHeight, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );
		QRadioButton * fitWindow = new QRadioButton( tr( "Fit to window" ), this );
		fitness_->addButton( fitWindow );
		fitness_->setId( fitWindow, Window );
		fitBox->addWidget( fitWindow );
		connect( fitWindow, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );

		fitOrigin->setChecked( true );

		QHBoxLayout * scaleBox = new QHBoxLayout;
		mainLayout->addLayout( scaleBox );

		scaleSlider_->setRange( 0, 1600 );
		scaleSlider_->setValue( 100 );
		scaleBox->addWidget( scaleSlider_ );

		QSpinBox * scaleSpin = new QSpinBox( this );
		scaleSpin->setRange( 0, 1600 );
		scaleSpin->setValue( 100 );
		scaleBox->addWidget( scaleSpin );

		connect( scaleSlider_, SIGNAL( sliderMoved( int ) ), this, SLOT( valueHelper_( int ) ) );
		connect( scaleSlider_, SIGNAL( valueChanged( int ) ), this, SLOT( valueHelper_( int ) ) );
		connect( scaleSpin, SIGNAL( valueChanged( int ) ), scaleSlider_, SLOT( setValue( int ) ) );
		connect( scaleSlider_, SIGNAL( valueChanged( int ) ), scaleSpin, SLOT( setValue( int ) ) );

		connect( fitOrigin, SIGNAL( toggled( bool ) ), scaleSlider_, SLOT( setEnabled( bool ) ) );
		connect( fitOrigin, SIGNAL( toggled( bool ) ), scaleSpin, SLOT( setEnabled( bool ) ) );
	}

	void ScaleImage::valueHelper_( int ) {
		qDebug( "ScaleImage::valueHelper_" );
		switch( fitness_->checkedId() ) {
		case Origin:
			emit scaled( scaleSlider_->value() );
			break;
		case Width:
			emit scaled( -1 );
			break;
		case Height:
			emit scaled( -2 );
			break;
		case Window:
			emit scaled( -3 );
			break;
		default:
			;
		}
	}

	ScaleImage::ScaleMode ScaleImage::getScaleMode() const {
		return static_cast< ScaleImage::ScaleMode >( fitness_->checkedId() );
	}

}
