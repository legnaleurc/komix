/**
 * @file scalepanel.cpp
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
#include "scalepanel.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>

namespace {

/// native enum
enum ScaleMode {
	Origin,
	Width,
	Height,
	Window
};

} // end of namespace

namespace KomiX { namespace widget {

ScalePanel::ScalePanel( QWidget * parent ):
QWidget( parent, Qt::Dialog ),
fitness_( new QButtonGroup( this ) ),
scaleSlider_( new QSlider( Qt::Horizontal, this ) ) {
	setWindowTitle( tr( "Scale Image" ) );

	QVBoxLayout * mainLayout = new QVBoxLayout( this );
	setLayout( mainLayout );

	QGroupBox * fit = new QGroupBox( tr( "Fitness" ), this );
	mainLayout->addWidget( fit );

	QVBoxLayout * fitBox = new QVBoxLayout;
	fit->setLayout( fitBox );

	QRadioButton * fitOrigin = new QRadioButton( tr( "&Customize" ), this );
	fitness_->addButton( fitOrigin );
	fitness_->setId( fitOrigin, Origin );
	fitBox->addWidget( fitOrigin );
	connect( fitOrigin, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );
	QRadioButton * fitWidth = new QRadioButton( tr( "Fit to &Width" ), this );
	fitness_->addButton( fitWidth );
	fitness_->setId( fitWidth, Width );
	fitBox->addWidget( fitWidth );
	connect( fitWidth, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );
	QRadioButton * fitHeight = new QRadioButton( tr( "Fit to &Height" ), this );
	fitness_->addButton( fitHeight );
	fitness_->setId( fitHeight, Height );
	fitBox->addWidget( fitHeight );
	connect( fitHeight, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );
	QRadioButton * fitWindow = new QRadioButton( tr( "&Fit to Window" ), this );
	fitness_->addButton( fitWindow );
	fitness_->setId( fitWindow, Window );
	fitBox->addWidget( fitWindow );
	connect( fitWindow, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );

	fitOrigin->setChecked( true );

	QHBoxLayout * scaleBox = new QHBoxLayout;
	mainLayout->addLayout( scaleBox );

	scaleSlider_->setRange( 0, 1600 );
	scaleSlider_->setValue( 100 );
	scaleBox->addWidget( scaleSlider_ );

	QSpinBox * scaleSpin = new QSpinBox( this );
	scaleSpin->setRange( 0, 1600 );
	scaleSpin->setValue( 100 );
	scaleBox->addWidget( scaleSpin );

	connect( scaleSlider_, SIGNAL( sliderMoved( int ) ), this, SLOT( valueHelper_( int ) ) );
	connect( scaleSlider_, SIGNAL( valueChanged( int ) ), this, SLOT( valueHelper_( int ) ) );
	connect( scaleSpin, SIGNAL( valueChanged( int ) ), scaleSlider_, SLOT( setValue( int ) ) );
	connect( scaleSlider_, SIGNAL( valueChanged( int ) ), scaleSpin, SLOT( setValue( int ) ) );

	connect( fitOrigin, SIGNAL( toggled( bool ) ), scaleSlider_, SLOT( setEnabled( bool ) ) );
	connect( fitOrigin, SIGNAL( toggled( bool ) ), scaleSpin, SLOT( setEnabled( bool ) ) );
}

void ScalePanel::scale( int ratio ) {
	fitness_->button( Origin )->setChecked( true );
	if( ratio != 0 ) {
		scaleSlider_->setValue( scaleSlider_->value() + ratio );
	} else {
		scaleSlider_->setValue( 100 );
	}
}

void ScalePanel::valueHelper_( int ) {
	qDebug( "ScaleImage::valueHelper_" );
	switch( fitness_->checkedId() ) {
	case Origin:
		emit scaled( scaleSlider_->value() );
		break;
	case Width:
		emit scaled( -1 );
		break;
	case Height:
		emit scaled( -2 );
		break;
	case Window:
		emit scaled( -3 );
		break;
	default:
		;
	}
}

} } // end namespace
