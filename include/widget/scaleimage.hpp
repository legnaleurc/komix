/**
 * @file scaleimage.hpp
 */
#ifndef KOMIX_SCALEIMAGE_HPP
#define KOMIX_SCALEIMAGE_HPP

#include <QDialog>
#include <QButtonGroup>
#include <QSlider>

namespace KomiX {

	/**
	 * @brief Widget to scale image
	 * @todo "upgrade" its functionality
	 *
	 * This widget is simple ... too simple. Maybe I'll
	 * change this widget to option widget.
	 */
	class ScaleImage : public QDialog {
		Q_OBJECT
	
	public:
		enum ScaleMode {
			Origin,
			Width,
			Height,
			Window
		};
		/**
		 * @brief default constructor
		 * @param parent parent widget
		 * @param f window flags
		 */
		ScaleImage( QWidget * parent = 0, Qt::WindowFlags f = 0 );

	signals:
		/**
		 * @brief scale event
		 * @param ratio scalar ratio
		 *
		 * The ratio means percents, so 100 actually means 100%.
		 */
		void scaled( int ratio );

	private slots:
		void valueHelper_( int = -4 );

	private:
		QButtonGroup * fitness_;
		QSlider * scaleSlider_;
	};
	
}

#endif
