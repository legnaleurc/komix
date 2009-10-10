/**
 * @file preference.cpp
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

#include "preference.hpp"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

namespace KomiX { namespace widget {

Preference::Preference( QWidget * parent ) : QDialog( parent ) {
	this->setWindowModality( Qt::WindowModal );
	this->setWindowTitle( tr( "Preference" ) );
	QVBoxLayout * mainLayout = new QVBoxLayout( this );
	this->setLayout( mainLayout );

	QGroupBox * speed = new QGroupBox( tr( "Scroll speed" ), this );
	mainLayout->addWidget( speed );
	QHBoxLayout * speedLayout = new QHBoxLayout( speed );
	speed->setLayout( speedLayout );

	QLabel * t1 = new QLabel( tr( "Move" ), speed );
	speedLayout->addWidget( t1 );
	QSpinBox * step = new QSpinBox( this );
	step->setValue( 1 );
	speedLayout->addWidget( step );
	QLabel * t2 = new QLabel( tr( "pixels per" ), speed );
	speedLayout->addWidget( t2 );
	QSpinBox * interval = new QSpinBox( this );
	interval->setValue( 1 );
	speedLayout->addWidget( interval );
	QLabel * t3 = new QLabel( tr( "millisecond(s)" ), speed );
	speedLayout->addWidget( t3 );
}

} } // end of namespace
