#include "scaleimage.hpp"

namespace KomiX {
	
	ScaleImage::ScaleImage( QWidget * parent, Qt::WindowFlags f ) : QDialog( parent, f ) {
		QPointer< QVBoxLayout > mainLayout = new QVBoxLayout( this );
		setLayout( mainLayout );
		QPointer< QHBoxLayout > scaleBox = new QHBoxLayout;
		mainLayout->addLayout( scaleBox );
		
		QPointer< QSlider > scaleSlider = new QSlider( Qt::Horizontal, this );
		scaleSlider->setRange( -100, 100 );
		scaleBox->addWidget( scaleSlider );
		connect( scaleSlider, SIGNAL( sliderMoved( int ) ), this, SIGNAL( scaled( int ) ) );
		connect( scaleSlider, SIGNAL( valueChanged( int ) ), this, SIGNAL( scaled( int ) ) );

		QPointer< QSpinBox > scaleSpin = new QSpinBox( this );
		scaleSpin->setRange( -100, 100 );
		scaleBox->addWidget( scaleSpin );
		connect( scaleSpin, SIGNAL( valueChanged( int ) ), scaleSlider, SLOT( setValue( int ) ) );
		connect( scaleSlider, SIGNAL( valueChanged( int ) ), scaleSpin, SLOT( setValue( int ) ) );
	}
	
}
