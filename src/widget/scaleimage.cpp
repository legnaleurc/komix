#include "scaleimage.hpp"

namespace KomiX {
	
	ScaleImage::ScaleImage( QWidget * parent, Qt::WindowFlags f ) : QDialog( parent, f ) {
		setLayout( new QVBoxLayout( this ) );
		
		QPointer< QSlider > slider = new QSlider( Qt::Horizontal, this );
		slider->setRange( -100, 100 );
		layout()->addWidget( slider );
		connect( slider, SIGNAL( sliderMoved( int ) ), this, SIGNAL( scaled( int ) ) );
		connect( slider, SIGNAL( valueChanged( int ) ), this, SIGNAL( scaled( int ) ) );
	}
	
}
