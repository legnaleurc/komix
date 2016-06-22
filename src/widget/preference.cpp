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

#include "preference_p.hpp"

#include <QtCore/QSettings>

using KomiX::widget::Preference;

Preference::Private::Private(Preference * owner)
    : QObject()
    , owner(owner)
    , ui() {
}

void Preference::Private::dispatch(QAbstractButton * button) {
    switch (this->ui.buttons->buttonRole(button)) {
        case QDialogButtonBox::RejectRole:
            this->owner->reject();
            break;
        case QDialogButtonBox::ApplyRole:
            this->owner->accept();
            break;
        case QDialogButtonBox::AcceptRole:
            this->owner->accept();
            this->owner->hide();
            break;
        default:;
    }
}

void Preference::Private::loadSettings() {
    QSettings ini;

    this->ui.pixelInterval->setValue(ini.value("pixel_interval", 1).toInt());
    this->ui.msInterval->setValue(ini.value("ms_interval", 1).toInt());
}

void Preference::Private::saveSettings() {
    QSettings ini;

    ini.setValue("pixel_interval", this->ui.pixelInterval->value());
    ini.setValue("ms_interval", this->ui.msInterval->value());
}

Preference::Preference(QWidget * parent)
    : QDialog(parent)
    , p_(new Private(this)) {
    this->p_->ui.setupUi(this);
    this->p_->connect(this->p_->ui.buttons, SIGNAL(clicked(QAbstractButton *)), SLOT(dispatch(QAbstractButton *)));

    this->p_->loadSettings();
}

void Preference::accept() {
    this->p_->saveSettings();

    emit this->accepted();
}

void Preference::reject() {
    this->p_->loadSettings();

    this->QDialog::reject();
}
