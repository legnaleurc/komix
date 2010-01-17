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

#include <QtCore/QSettings>
#include <QtDebug>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

namespace KomiX { namespace widget {

Preference::Preference( QWidget * parent ):
QDialog( parent ),
step_( new QSpinBox( this ) ),
interval_( new QSpinBox( this ) ),
buttons_( new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel, Qt::Horizontal, this ) ) {
	this->setWindowModality( Qt::WindowModal );
	this->setWindowTitle( tr( "Preference" ) );
	QVBoxLayout * mainLayout = new QVBoxLayout( this );
	this->setLayout( mainLayout );

	this->step_->setRange( 1, 1000 );
	this->interval_->setRange( 1, 1000 );

	QGroupBox * speed = new QGroupBox( tr( "Scroll speed" ), this );
	mainLayout->addWidget( speed );
	QHBoxLayout * speedLayout = new QHBoxLayout( speed );
	speed->setLayout( speedLayout );

	QLabel * t1 = new QLabel( tr( "Move" ), speed );
	speedLayout->addWidget( t1 );
	speedLayout->addWidget( this->step_ );
	QLabel * t2 = new QLabel( tr( "pixels per" ), speed );
	speedLayout->addWidget( t2 );
	speedLayout->addWidget( this->interval_ );
	QLabel * t3 = new QLabel( tr( "millisecond(s)" ), speed );
	speedLayout->addWidget( t3 );

	mainLayout->addWidget( this->buttons_ );
	connect( this->buttons_, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( dispatch_( QAbstractButton * ) ) );

	this->loadSettings_();
}

void Preference::dispatch_( QAbstractButton * button ) {
	switch( this->buttons_->buttonRole( button ) ) {
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

	this->step_->setValue( ini.value( "step", 1 ).toInt() );
	this->interval_->setValue( ini.value( "interval", 1 ).toInt() );
}

void Preference::saveSettings_() {
	QSettings ini;

	ini.setValue( "step", this->step_->value() );
	ini.setValue( "interval", this->interval_->value() );
}

void Preference::accept() {
	this->saveSettings_();

	emit accepted();
}

void Preference::reject() {
	this->loadSettings_();

	QDialog::reject();
}

} } // end of namespace
