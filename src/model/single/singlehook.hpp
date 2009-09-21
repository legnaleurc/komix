#ifndef KOMIX_MODEL_SINGLE_SINGLEHOOK_HPP
#define KOMIX_MODEL_SINGLE_SINGLEHOOK_HPP

#include <QAction>
#include <QUrl>

namespace KomiX { namespace model { namespace single {

class SingleHook : public QAction {
	Q_OBJECT
public:
	SingleHook( QWidget * parent );
signals:
	void opened( const QUrl & url );
private slots:
	void helper_();
};

} } } // end of namespace

#endif
