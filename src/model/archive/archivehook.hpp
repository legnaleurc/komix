#ifndef KOMIX_MODEL_ARCHIVE_ARCHIVEHOOK_HPP
#define KOMIX_MODEL_ARCHIVE_ARCHIVEHOOK_HPP

#include <QObject>
#include <QAction>

namespace KomiX { namespace model { namespace archive {

class ArchiveHook : public QObject {
	Q_OBJECT
public:
	ArchiveHook( QWidget * parent );

	QAction * action() const;

signals:
	void opened( const QString & name );

private slots:
	void helper_();
	void cleanup_();

private:
	const QString & archiveFilter_();

	QAction * action_;
};

} } } // end of namespace

#endif
