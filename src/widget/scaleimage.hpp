#ifndef SCALEIMAGE_HPP
#define SCALEIMAGE_HPP

#include <QDialog>
#include <QVBoxLayout>
#include <QPointer>
#include <QSlider>

namespace KomiX {
	
	class ScaleImage : public QDialog {
		
		Q_OBJECT
		
		public:
			ScaleImage( QWidget * parent = 0 );
		signals:
			void scaled( int );
	};
	
}

#endif
