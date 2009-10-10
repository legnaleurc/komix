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
#include "navigator.hpp"
#include "global.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QtDebug>

namespace KomiX { namespace widget {

Navigator::Navigator( QWidget * parent ) :
QDialog( parent ),
model_( NULL ),
view_( NULL ),
selection_( NULL ),
image_( this ) {
	setModal( true );

	view_ = new QListView( this );
	view_->setFixedSize( 160, 480 );

	image_.setFixedSize( 480, 480 );
	image_.setAlignment( Qt::AlignCenter );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Open | QDialogButtonBox::Cancel, Qt::Horizontal, this );
	connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
	connect( buttonBox, SIGNAL( accepted() ), this, SLOT( openHelper_() ) );

	QHBoxLayout * topFrame = new QHBoxLayout;
	topFrame->addWidget( view_ );
	topFrame->addWidget( &image_ );

	QVBoxLayout * mainFrame = new QVBoxLayout( this );
	mainFrame->addLayout( topFrame );
	mainFrame->addWidget( buttonBox );
}

void Navigator::setModel( model::FileModelSP model ) {
	if( selection_ ) {
		disconnect( selection_, SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( viewImage_( const QModelIndex &, const QModelIndex & ) ) );
	}
	model_ = model;
	view_->setModel( model_.data() );
	selection_ = view_->selectionModel();
	connect( selection_, SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ), this, SLOT( viewImage_( const QModelIndex &, const QModelIndex & ) ) );
}

void Navigator::setCurrentIndex( const QModelIndex & index ) {
	view_->setCurrentIndex( index );
}

void Navigator::openHelper_() {
	qDebug() << "Send: " << view_->currentIndex();
	emit required( view_->currentIndex() );
	accept();
}

void Navigator::viewImage_( const QModelIndex & current, const QModelIndex & /* previous */ ) {
	qDebug( "Preview::viewImage_()" );
	qDebug() << current;
	image_.setPixmap( current.data( Qt::UserRole ).value< QPixmap >().scaled( image_.size(), Qt::KeepAspectRatio ) );
}

} } // end namespace
