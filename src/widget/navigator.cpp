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
#include "global.hpp"
#include "navigator.hpp"
#include "ui_navigator.h"

#include <QtDebug>

using namespace KomiX::widget;
using KomiX::model::FileModelSP;

Navigator::Navigator( QWidget * parent ) :
QDialog( parent ),
ui_( new Ui::Navigator ),
model_( NULL ),
selection_( NULL ) {
	this->ui_->setupUi( this );
	this->ui_->list->setFixedSize( 160, 480 );
	this->ui_->preview->setFixedSize( 480, 480 );
	this->ui_->preview->setAlignment( Qt::AlignCenter );

	connect( this->ui_->buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );
	connect( this->ui_->buttons, SIGNAL( accepted() ), this, SLOT( openHelper_() ) );
}

Navigator::~Navigator() {
	delete this->ui_;
}

void Navigator::setModel( FileModelSP model ) {
	if( this->selection_ ) {
		disconnect( this->selection_, SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( viewImage_( const QModelIndex &, const QModelIndex & ) ) );
	}
	this->model_ = model;
	this->ui_->list->setModel( this->model_.data() );
	this->selection_ = this->ui_->list->selectionModel();
	connect( this->selection_, SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( viewImage_( const QModelIndex &, const QModelIndex & ) ) );
}

void Navigator::setCurrentIndex( const QModelIndex & index ) {
	this->ui_->list->setCurrentIndex( index );
}

void Navigator::openHelper_() {
	qDebug() << "Send: " << this->ui_->list->currentIndex();
	emit required( this->ui_->list->currentIndex() );
	accept();
}

void Navigator::viewImage_( const QModelIndex & current, const QModelIndex & /* previous */ ) {
	qDebug( "Preview::viewImage_()" );
	qDebug() << current;
	this->ui_->preview->setPixmap( current.data( Qt::UserRole ).value< QPixmap >().scaled( this->ui_->preview->size(), Qt::KeepAspectRatio ) );
}
