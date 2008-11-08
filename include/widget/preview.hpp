#ifndef KOMIX_PREVIEW_HPP
#define KOMIX_PREVIEW_HPP

#include <QDialog>
#include <QListView>
#include <QDirModel>

namespace KomiX {

	class Preview : public QDialog {
		Q_OBJECT
	public:
		Preview( QWidget * parent = 0, Qt::WindowFlags f = 0 );
		void listDirectory( const QString & dirPath, const QString & filePath );
	signals:
		void open( const QString & path );
	private:
		QDirModel model_;
		QListView view_;
	private slots:
		void openHelper_();
	};

}

#endif
