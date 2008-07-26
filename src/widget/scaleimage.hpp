#ifndef SCALEIMAGE_HPP
#define SCALEIMAGE_HPP

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPointer>
#include <QSlider>
#include <QSpinBox>

namespace KomiX {
	
	class ScaleImage : public QDialog {
		
		Q_OBJECT
		
		public:
			ScaleImage( QWidget * parent = 0, Qt::WindowFlags f = 0 );
		signals:
			void scaled( int );
	};
	
}

#endif
