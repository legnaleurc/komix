#include "archivehook.hpp"
#include "archivemodel.hpp"

#include <QFileDialog>

namespace {

	inline QString archiveList() {
		QStringList tmp( QObject::tr( "All Supported Archives ( %1 )" ).arg( KomiX::ArchiveFormatsFilter().join( " " ) ) );
		tmp << KomiX::ArchiveFormatsFilter();
		return tmp.join( ";;" );
	}

}

namespace KomiX {

	ArchiveHook::ArchiveHook( QWidget * parent ) : QObject( parent ) {
		action_ = new QAction( tr( "Open A&rchive" ), parent );
		connect( action_, SIGNAL( triggered() ), this, SLOT( helper_() ) );
		connect( this, SIGNAL( opened( const QString & ) ), parent, SLOT( open( const QString & ) ) );
	}

	QAction * ArchiveHook::action() const {
		return action_;
	}

	void ArchiveHook::helper_() {
		emit opened( QFileDialog::getOpenFileName( qobject_cast< QWidget * >( this->parent() ), tr( "Open archive" ), QDir::homePath(), archiveFilter_() ) );
	}

	inline const QString & ArchiveHook::archiveFilter_() {
		static QString af = archiveList();
		return af;
	}

}
