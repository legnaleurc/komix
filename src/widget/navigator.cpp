/**
 * @file navigator.cpp
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
#include "filecontroller.hpp"
#include "global.hpp"
#include "imageloader.hpp"
#include "navigator_p.hpp"


using KomiX::widget::Navigator;
using KomiX::FileController;
using KomiX::model::FileModel;


Navigator::Navigator(QWidget * parent)
    : QDialog(parent)
    , p_(new Private(this))
{
    this->p_->ui.setupUi(this);
    this->p_->ui.list->setFixedSize(160, 480);
    this->p_->ui.preview->setFixedSize(480, 480);
    this->p_->ui.preview->setAlignment(Qt::AlignCenter);

    this->connect(this->p_->ui.buttons, SIGNAL(rejected()), SLOT(reject()));
    this->p_->connect(this->p_->ui.buttons, SIGNAL(accepted()), SLOT(openHelper()));
}


void Navigator::setModel(FileModel::SP model) {
    if (this->p_->selection) {
        this->p_->selection->disconnect(SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this->p_.get(), SLOT(viewImage(const QModelIndex &, const QModelIndex &)));
    }
    this->p_->model = model;
    this->p_->ui.list->setModel(this->p_->model.get());
    this->p_->selection = this->p_->ui.list->selectionModel();
    this->p_->connect(this->p_->selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), SLOT(viewImage(const QModelIndex &, const QModelIndex &)));
}


void Navigator::setCurrentIndex(const QModelIndex & index) {
    this->p_->ui.list->setCurrentIndex(index);
}


void Navigator::setVisible(bool visible) {
    // pause movie for performance
    QMovie * movie = this->p_->ui.preview->movie();
    if (movie) {
        movie->setPaused(!visible);
    }
    this->QDialog::setVisible(visible);
}


Navigator::Private::Private(Navigator * owner)
    : QObject()
    , owner(owner)
    , ui()
    , model()
    , selection(NULL) {
}


void Navigator::Private::openHelper() {
    auto & fc = Global::instance().getFileController();
    fc.open(this->ui.list->currentIndex());
    this->owner->accept();
}


void Navigator::Private::viewImage(const QModelIndex & current, const QModelIndex & /* previous */) {
    auto deviceCreator = current.data(Qt::UserRole).value<DeviceCreator>();
    ImageLoader::load(current.row(), deviceCreator(), this, SLOT(onFinished(int, const QPixmap &)), SLOT(onFinished(int, QMovie *)));
}


void Navigator::Private::onFinished(int /*id*/, QMovie * movie) {
    QMovie * tmp = this->ui.preview->movie();

    movie->jumpToNextFrame();
    QSize size = movie->frameRect().size();
    size.scale(this->ui.preview->size(), Qt::KeepAspectRatio);
    movie->setScaledSize(size);
    this->ui.preview->setMovie(movie);
    movie->start();

    if (tmp) {
        tmp->deleteLater();
    }
}


void Navigator::Private::onFinished(int /*id*/, const QPixmap & pixmap) {
    QMovie * tmp = this->ui.preview->movie();

    this->ui.preview->setPixmap(pixmap.scaled(this->ui.preview->size(), Qt::KeepAspectRatio));

    if (tmp) {
        tmp->deleteLater();
    }
}
