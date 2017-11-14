/**
 * @file filecontroller.cpp
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
#include "exception.hpp"
#include "filecontroller_p.hpp"
#include "global.hpp"
#include "literal.hpp"

#include <QtCore/QFileInfo>

#include <QtCore/QtDebug>


using KomiX::FileController;
using KomiX::model::FileModel;


FileController::FileController(QObject * parent)
    : QObject(parent)
    , p_(new Private(this))
{
}


// FIXME exception safety
void FileController::open(const QUrl & url) {
    try {
        this->p_->model = FileModel::createModel(url);
        if (!this->p_->model) {
            throw exception::Exception(FILEMODEL_ERROR.arg(url.toString()));
        }
        this->p_->connect(this->p_->model.get(), SIGNAL(ready()), SLOT(onModelReady()));
        this->connect(this->p_->model.get(),
                      SIGNAL(error(const QString &)),
                      SIGNAL(errorOccured(const QString &)));
        this->connect(this->p_->model.get(),
                      SIGNAL(progressUpdated(int, int)),
                      SIGNAL(progressUpdated(int, int)));
        this->p_->openingURL = url;
        this->p_->model->initialize();
    } catch (exception::Exception & e) {
        emit errorOccured(e.getMessage());
    }
}


void FileController::open(const QString & localPath) {
    if (localPath.isEmpty()) {
        return;
    }
    this->open(QUrl::fromLocalFile(localPath));
}


void FileController::open(const QModelIndex & index) {
    if (!this->isEmpty()) {
        this->p_->index = index.row();
        emit this->focus(this->p_->index);
    }
}


QModelIndex FileController::getCurrentIndex() const {
    if (!this->isEmpty()) {
        return this->p_->model->index(this->p_->index, 0);
    } else {
        return QModelIndex();
    }
}


void FileController::next() {
    if (!this->isEmpty()) {
        ++this->p_->index;
        if (this->p_->index >= this->p_->model->rowCount()) {
            this->p_->index = 0;
        }
        QModelIndex item = this->p_->model->index(this->p_->index, 0);
        //this->p_->fromIndex(item);
    }
}


void FileController::prev() {
    if (!this->isEmpty()) {
        --this->p_->index;
        if (this->p_->index < 0) {
            this->p_->index = this->p_->model->rowCount() - 1;
        }
        QModelIndex item = this->p_->model->index(this->p_->index, 0);
        //this->p_->fromIndex(item);
    }
}


bool FileController::isEmpty() const {
    if (!this->p_->model) {
        return true;
    }
    return this->p_->model->rowCount() == 0;
}


std::shared_ptr<KomiX::model::FileModel> FileController::getModel() const {
    return this->p_->model;
}


FileController::Private::Private(FileController * owner)
    : QObject()
    , owner(owner)
    , index(0)
    , openingURL()
    , model(NULL)
{
}

void FileController::Private::onModelReady() {
    if (this->owner->isEmpty()) {
        return;
    }
    QModelIndex first = this->model->index(this->openingURL);
    if (first.isValid()) {
        this->index = first.row();
    } else {
        first = this->model->index(0, 0);
        this->index = 0;
    }
    emit this->owner->modelReady();
}
