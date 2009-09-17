#ifndef KOMIX_ARCHIVEHOOK_HPP
#define KOMIX_ARCHIVEHOOK_HPP

#include <QObject>
#include <QAction>

namespace KomiX {

	class ArchiveHook : public QObject {
		Q_OBJECT
	public:
		ArchiveHook( QWidget * parent );

		QAction * action() const;

	signals:
		void opened( const QString & name );

	private slots:
		void helper_();

	private:
		const QString & archiveFilter_();

		QAction * action_;
	};

}

#endif
