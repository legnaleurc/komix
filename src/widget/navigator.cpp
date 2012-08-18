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
#include "navigator_p.hpp"

#include <QtCore/QtDebug>
#include <QtGui/QMovie>

using KomiX::widget::Navigator;
using KomiX::model::FileModelSP;

Navigator::Private::Private( Navigator * owner ):
QObject(),
ui( new Ui::Navigator ),
model( NULL ),
selection( NULL ) {
}

void Navigator::Private::openHelper() {
	qDebug() << "Send: " << this->ui->list->currentIndex();
	emit this->required( this->ui->list->currentIndex() );
	this->owner->accept();
}

void Navigator::Private::viewImage( const QModelIndex & current, const QModelIndex & /* previous */ ) {
	qDebug( "Preview::viewImage_()" );
	qDebug() << current;
	QString path = current.data( Qt::UserRole ).toString();
	QObject * mm = this->ui->preview->movie();
	if( path.endsWith( ".gif", Qt::CaseInsensitive ) ) {
		QMovie * m = new QMovie( path, QByteArray(), this );
		this->ui->preview->setMovie( m );
		m->start();
	} else {
		this->ui->preview->setPixmap( QPixmap( path ).scaled( this->ui->preview->size(), Qt::KeepAspectRatio ) );
	}
	if( mm ) {
		mm->deleteLater();
	}
}

Navigator::Navigator( QWidget * parent ) :
QDialog( parent ),
p_( new Private( this ) ) {
	this->p_->ui->setupUi( this );
	this->p_->ui->list->setFixedSize( 160, 480 );
	this->p_->ui->preview->setFixedSize( 480, 480 );
	this->p_->ui->preview->setAlignment( Qt::AlignCenter );

	this->connect( this->p_->ui->buttons, SIGNAL( rejected() ), SLOT( reject() ) );
	this->p_->connect( this->p_->ui->buttons, SIGNAL( accepted() ), SLOT( openHelper() ) );
	this->connect( this->p_.get(), SIGNAL( required( const QModelIndex & ) ), SIGNAL( const QModelIndex & ) );
}

void Navigator::setModel( FileModelSP model ) {
	if( this->p_->selection ) {
		this->p_->selection->disconnect( SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( viewImage( const QModelIndex &, const QModelIndex & ) ) );
	}
	this->p_->model = model;
	this->p_->ui->list->setModel( this->p_->model.data() );
	this->p_->selection = this->p_->ui->list->selectionModel();
	this->p_->connect( this->p_->selection, SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), SLOT( viewImage( const QModelIndex &, const QModelIndex & ) ) );
}

void Navigator::setCurrentIndex( const QModelIndex & index ) {
	this->p_->ui->list->setCurrentIndex( index );
}
