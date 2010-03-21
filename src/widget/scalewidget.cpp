/**
 * @file scalewidget.cpp
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
#include "scalewidget.hpp"
#include "ui_scalewidget.h"

#include <QtGui/QButtonGroup>

namespace {

	/// native enum
	enum ScaleMode {
		Custom,
		Width,
		Height,
		Window
	};

} // end of namespace

using namespace KomiX::widget;

ScaleWidget::ScaleWidget( QWidget * parent ):
QWidget( parent, Qt::Dialog ),
ui_( new Ui::ScaleWidget ),
modes_( new QButtonGroup( this ) ) {
	this->ui_->setupUi( this );

	this->modes_->addButton( this->ui_->custom );
	this->modes_->setId( this->ui_->custom, Custom );
	connect( this->ui_->custom, SIGNAL( clicked() ), this, SLOT( valueHelper_() ) );

	this->modes_->addButton( this->ui_->width );
	this->modes_->setId( this->ui_->width, Width );
	connect( this->ui_->width, SIGNAL( clicked() ), this, SLOT( fitWidth() ) );

	this->modes_->addButton( this->ui_->height );
	this->modes_->setId( this->ui_->height, Height );
	connect( this->ui_->height, SIGNAL( clicked() ), this, SLOT( fitHeight() ) );

	this->modes_->addButton( this->ui_->window );
	this->modes_->setId( this->ui_->window, Window );
	connect( this->ui_->window, SIGNAL( clicked() ), this, SLOT( fitWindow() ) );

	connect( this->ui_->scaleSlider, SIGNAL( sliderMoved( int ) ), this, SIGNAL( scaled( int ) ) );
	connect( this->ui_->scaleSlider, SIGNAL( valueChanged( int ) ), this, SIGNAL( scaled( int ) ) );
}

ScaleWidget::~ScaleWidget() {
	delete this->ui_;
}


void ScaleWidget::scale( int ratio ) {
	this->modes_->button( Custom )->setChecked( true );
	if( ratio != 0 ) {
		this->ui_->scaleSlider->setValue( this->ui_->scaleSlider->value() + ratio );
	} else {
		this->ui_->scaleSlider->setValue( 100 );
	}
}

void ScaleWidget::valueHelper_() {
	qDebug( "ScaleImage::valueHelper_" );
	qDebug( "%d", this->ui_->scaleSlider->value() );
	emit scaled( this->ui_->scaleSlider->value() );
}
