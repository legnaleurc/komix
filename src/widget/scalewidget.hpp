/**
 * @file scalewidget.hpp
 * @author Wei-Cheng Pan
 *
 * KomiX, a comics viewer.
 * Copyright (C) 2008  Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KOMIX_WIDGET_SCALEWIDGET_HPP
#define KOMIX_WIDGET_SCALEWIDGET_HPP

#include <QtGui/QWidget>

namespace Ui {
	class ScaleWidget;
}
class QButtonGroup;

namespace KomiX {
	namespace widget {

		/**
		 * @brief Widget to scale image
		 *
		 * This widget is simple ... too simple. Maybe I'll
		 * change this widget to option widget.
		 */
		class ScaleWidget : public QWidget {
			Q_OBJECT

		public:
			/**
			 * @brief default constructor
			 * @param parent parent widget
			 */
			explicit ScaleWidget( QWidget * parent );
			virtual ~ScaleWidget();

		public slots:
			/// to move slider
			void scale( int ratio );

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
			Ui::ScaleWidget * ui_;
			QButtonGroup * modes_;
		};

	}
} // end namespace

#endif
