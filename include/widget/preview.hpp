#ifndef KOMIX_PREVIEW_HPP
#define KOMIX_PREVIEW_HPP

#include <QDialog>
#include <QListView>
#include <QDirModel>

namespace KomiX {

	class Preview : public QDialog {
	public:
		Preview( QWidget * parent = 0, Qt::WindowFlags f = 0 );
		void listDirectory( const QString & dirPath, const QString & filePath );
	private:
		QDirModel model_;
		QListView view_;
	};

}

#endif
