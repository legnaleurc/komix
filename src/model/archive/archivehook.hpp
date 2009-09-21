#ifndef KOMIX_MODEL_ARCHIVE_ARCHIVEHOOK_HPP
#define KOMIX_MODEL_ARCHIVE_ARCHIVEHOOK_HPP

#include <QAction>

namespace KomiX { namespace model { namespace archive {

class ArchiveHook : public QAction {
	Q_OBJECT
public:
	ArchiveHook( QWidget * parent );

signals:
	void opened( const QUrl & name );

private slots:
	void helper_();
	void cleanup_();
};

} } } // end of namespace

#endif
