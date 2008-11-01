#ifndef SCALEIMAGE_HPP
#define SCALEIMAGE_HPP

#include <QDialog>

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
