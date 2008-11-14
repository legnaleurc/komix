#ifndef KOMIX_FILECONTROLLER_HPP
#define KOMIX_FILECONTROLLER_HPP

#include <QDir>
#include <QStringList>
#include <QPixmap>

namespace KomiX {

	class FileController : public QObject {
		Q_OBJECT
	public:
		FileController( unsigned int pfMax = 1, QObject * parent = 0 );
		
		bool open( const QString & filePath );
		void next();
		void prev();

		void setPrefetchMax( unsigned int pfMax );
		unsigned int getPrefetchMax() const;
	
	signals:
		void openImage( const QPixmap & );
	
	private:
		void prefetch_( unsigned int index );
		void fetch_( const QString & );
		bool update_( const QString & );

		unsigned int prefetchMax_;
		QDir dir_;
		QStringList files_;
		int index_;
		QPixmap cImage_;
	};

}

#endif
