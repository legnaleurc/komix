#include "archivehook.hpp"
#include "archivemodel.hpp"

#include <QFileDialog>
#include <QApplication>

namespace {

inline QString archiveList() {
	QStringList tmp( QObject::tr( "All Supported Archives ( %1 )" ).arg( KomiX::model::archive::ArchiveFormatsFilter().join( " " ) ) );
	tmp << KomiX::model::archive::ArchiveFormatsFilter();
	return tmp.join( ";;" );
}

} // end of namespace

namespace KomiX { namespace model { namespace archive {

ArchiveHook::ArchiveHook( QWidget * parent ) : QObject( parent ) {
	action_ = new QAction( tr( "Open A&rchive" ), parent );
	action_->setShortcut( tr( "Ctrl+A" ) );
	connect( action_, SIGNAL( triggered() ), this, SLOT( helper_() ) );
	connect( this, SIGNAL( opened( const QString & ) ), parent, SLOT( open( const QString & ) ) );
	// for cleanup, delete temp dir
	connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( cleanup_() ) );
}

QAction * ArchiveHook::action() const {
	return action_;
}

void ArchiveHook::helper_() {
	QString name = QFileDialog::getOpenFileName( qobject_cast< QWidget * >( this->parent() ), tr( "Open archive" ), QDir::homePath(), archiveFilter_() );
	if( !name.isEmpty() ) {
		emit opened( name );
	}
}

void ArchiveHook::cleanup_() {
	delTree( ArchiveModel::TmpDir_() );
}

inline const QString & ArchiveHook::archiveFilter_() {
	static QString af = archiveList();
	return af;
}

} } } // end of namespace
