#ifndef KOMIX_MODEL_DIRECTORY_DIRECTORYHOOK_HPP
#define KOMIX_MODEL_DIRECTORY_DIRECTORYHOOK_HPP

#include <QAction>
#include <QUrl>

namespace KomiX { namespace model { namespace directory {

class DirectoryHook : public QAction {
	Q_OBJECT
public:
	DirectoryHook( QWidget * parent );
signals:
	void opened( const QUrl & url );
private slots:
	void helper_();
};

} } } // end of namespace

#endif
