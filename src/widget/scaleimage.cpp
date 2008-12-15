#include "scaleimage.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>

namespace KomiX {

	ScaleImage::ScaleImage( QWidget * parent, Qt::WindowFlags f ) : QDialog( parent, f ), fitness_( new QButtonGroup( this ) ) {
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
		QRadioButton * fitWidth = new QRadioButton( tr( "Fit to width" ), this );
		fitness_->addButton( fitWidth );
		fitness_->setId( fitWidth, Width );
		fitBox->addWidget( fitWidth );
		QRadioButton * fitHeight = new QRadioButton( tr( "Fit to height" ), this );
		fitness_->addButton( fitHeight );
		fitness_->setId( fitHeight, Height );
		fitBox->addWidget( fitHeight );
		QRadioButton * fitWindow = new QRadioButton( tr( "Fit to window" ), this );
		fitness_->addButton( fitWindow );
		fitness_->setId( fitWindow, Window );
		fitBox->addWidget( fitWindow );

		fitOrigin->setChecked( true );

		QHBoxLayout * scaleBox = new QHBoxLayout;
		mainLayout->addLayout( scaleBox );

		QSlider * scaleSlider = new QSlider( Qt::Horizontal, this );
		scaleSlider->setRange( 0, 1600 );
		scaleSlider->setValue( 100 );
		scaleBox->addWidget( scaleSlider );

		QSpinBox * scaleSpin = new QSpinBox( this );
		scaleSpin->setRange( 0, 1600 );
		scaleSpin->setValue( 100 );
		scaleBox->addWidget( scaleSpin );

		connect( scaleSlider, SIGNAL( sliderMoved( int ) ), this, SIGNAL( scaled( int ) ) );
		connect( scaleSlider, SIGNAL( valueChanged( int ) ), this, SIGNAL( scaled( int ) ) );
		connect( scaleSpin, SIGNAL( valueChanged( int ) ), scaleSlider, SLOT( setValue( int ) ) );
		connect( scaleSlider, SIGNAL( valueChanged( int ) ), scaleSpin, SLOT( setValue( int ) ) );

		connect( fitOrigin, SIGNAL( toggled( bool ) ), scaleSlider, SLOT( setEnabled( bool ) ) );
		connect( fitOrigin, SIGNAL( toggled( bool ) ), scaleSpin, SLOT( setEnabled( bool ) ) );
	}

	ScaleImage::ScaleMode ScaleImage::getScaleMode() const {
		return static_cast< ScaleImage::ScaleMode >( fitness_->checkedId() );
	}

}
