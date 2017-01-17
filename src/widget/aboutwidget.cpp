/**
 * @file aboutwidget.cpp
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
#include "aboutwidget_p.hpp"

using KomiX::widget::AboutWidget;

AboutWidget::Private::Private(AboutWidget * owner)
    : owner(owner)
    , ui() {
}

AboutWidget::AboutWidget(QWidget * parent)
    : QWidget(parent, Qt::Dialog)
    , p_(new Private(this)) {
    this->p_->ui.setupUi(this);

    auto text = this->p_->ui.info->text();
    text = text.arg(QApplication::applicationName()).arg(QApplication::applicationVersion());
    this->p_->ui.info->setText(text);

    text = this->p_->ui.aboutMessage->text();
    text = text.arg(QApplication::organizationName());
    this->p_->ui.aboutMessage->setText(text);
}
