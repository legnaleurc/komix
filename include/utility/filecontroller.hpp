#ifndef KOMIX_FILECONTROLLER_HPP
#define KOMIX_FILECONTROLLER_HPP

#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QQueue>
#include <QMap>

namespace KomiX {

	class FileController : public QObject {
		Q_OBJECT
	
	public:
		FileController( unsigned int pfMax = 1, unsigned int limit = 8, QObject * parent = 0 );
		
		bool open( const QString & filePath );
		void next();
		void prev();

		void setPrefetchMax( unsigned int pfMax );
		unsigned int getPrefetchMax() const;
		void setLimit( unsigned int limit );
		unsigned int getLimit() const;
	
	signals:
		void openImage( const QPixmap & );
	
	private:
		void prefetch_( unsigned int index );
		QPixmap & fetch_( const QString & );
		bool update_( const QString & );

		unsigned int prefetchMax_;
		unsigned int limit_;
		QDir dir_;
		QStringList files_;
		int index_;
		QQueue< QString > history_;
		QMap< QString, QPixmap > cache_;
	};

}

#endif
