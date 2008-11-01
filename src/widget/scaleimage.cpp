#include "scaleimage.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>

namespace KomiX {
	
	ScaleImage::ScaleImage( QWidget * parent, Qt::WindowFlags f ) : QDialog( parent, f ) {
		QVBoxLayout * mainLayout = new QVBoxLayout( this );
		setLayout( mainLayout );
		QHBoxLayout * scaleBox = new QHBoxLayout;
		mainLayout->addLayout( scaleBox );
		
		QSlider * scaleSlider = new QSlider( Qt::Horizontal, this );
		scaleSlider->setRange( -100, 100 );
		scaleBox->addWidget( scaleSlider );
		connect( scaleSlider, SIGNAL( sliderMoved( int ) ), this, SIGNAL( scaled( int ) ) );
		connect( scaleSlider, SIGNAL( valueChanged( int ) ), this, SIGNAL( scaled( int ) ) );

		QSpinBox * scaleSpin = new QSpinBox( this );
		scaleSpin->setRange( -100, 100 );
		scaleBox->addWidget( scaleSpin );
		connect( scaleSpin, SIGNAL( valueChanged( int ) ), scaleSlider, SLOT( setValue( int ) ) );
		connect( scaleSlider, SIGNAL( valueChanged( int ) ), scaleSpin, SLOT( setValue( int ) ) );
	}
	
}
