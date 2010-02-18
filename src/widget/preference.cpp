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
#include "ui_preference.h"

#include <QtCore/QSettings>

using namespace KomiX::widget;

Preference::Preference( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::Preference ) {
	this->ui_->setupUi( this );
	connect( this->ui_->buttons, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( dispatch_( QAbstractButton * ) ) );

	this->loadSettings_();
}

Preference::~Preference() {
	delete this->ui_;
}

void Preference::dispatch_( QAbstractButton * button ) {
	switch( this->ui_->buttons->buttonRole( button ) ) {
	case QDialogButtonBox::RejectRole:
		this->reject();
		break;
	case QDialogButtonBox::ApplyRole:
		this->accept();
		break;
	case QDialogButtonBox::AcceptRole:
		this->accept();
		this->hide();
		break;
	default:
		;
	}
}

void Preference::loadSettings_() {
	QSettings ini;

	this->ui_->pixelInterval->setValue( ini.value( "pixel_interval", 1 ).toInt() );
	this->ui_->timeInterval->setValue( ini.value( "time_interval", 1 ).toInt() );
}

void Preference::saveSettings_() {
	QSettings ini;

	ini.setValue( "pixel_interval", this->ui_->pixelInterval->value() );
	ini.setValue( "time_interval", this->ui_->timeInterval->value() );
}

void Preference::accept() {
	this->saveSettings_();

	emit accepted();
}

void Preference::reject() {
	this->loadSettings_();

	QDialog::reject();
}
