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
#include "scalewidget_p.hpp"

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

using KomiX::widget::ScaleWidget;

ScaleWidget::Private::Private( ScaleWidget * owner ):
QObject(),
owner( owner ),
ui(),
modes( new QButtonGroup( owner ) ){
	this->owner->connect( this, SIGNAL( scaled( int ) ), SIGNAL( scaled( int ) ) );
}

void ScaleWidget::Private::valueHelper() {
	emit this->scaled( this->ui.scaleSlider->value() );
}

ScaleWidget::ScaleWidget( QWidget * parent ):
QWidget( parent, Qt::Dialog ),
p_( new Private( this ) ) {
	this->p_->ui.setupUi( this );

	this->p_->modes->addButton( this->p_->ui.custom );
	this->p_->modes->setId( this->p_->ui.custom, Custom );
	this->p_->connect( this->p_->ui.custom, SIGNAL( clicked() ), SLOT( valueHelper() ) );

	this->p_->modes->addButton( this->p_->ui.width );
	this->p_->modes->setId( this->p_->ui.width, Width );
	this->connect( this->p_->ui.width, SIGNAL( clicked() ), SIGNAL( fitWidth() ) );

	this->p_->modes->addButton( this->p_->ui.height );
	this->p_->modes->setId( this->p_->ui.height, Height );
	this->connect( this->p_->ui.height, SIGNAL( clicked() ), SIGNAL( fitHeight() ) );

	this->p_->modes->addButton( this->p_->ui.window );
	this->p_->modes->setId( this->p_->ui.window, Window );
	this->connect( this->p_->ui.window, SIGNAL( clicked() ), SIGNAL( fitWindow() ) );

	this->connect( this->p_->ui.scaleSlider, SIGNAL( sliderMoved( int ) ), SIGNAL( scaled( int ) ) );
	this->connect( this->p_->ui.scaleSlider, SIGNAL( valueChanged( int ) ), SIGNAL( scaled( int ) ) );
}

void ScaleWidget::scale( int ratio ) {
	this->p_->modes->button( Custom )->setChecked( true );
	if( ratio != 0 ) {
		this->p_->ui.scaleSlider->setValue( this->p_->ui.scaleSlider->value() + ratio );
	} else {
		this->p_->ui.scaleSlider->setValue( 100 );
	}
}
