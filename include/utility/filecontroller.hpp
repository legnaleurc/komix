#ifndef KOMIX_FILECONTROLLER_HPP
#define KOMIX_FILECONTROLLER_HPP

#include <QDir>
#include <QStringList>
#include <QPixmap>
#include <QQueue>
#include <QMap>
#include <QMutex>

#include <loki/Singleton.h>

namespace KomiX {

	class FileControllerBase : public QObject {
		Q_OBJECT
	
	public:
		FileControllerBase( unsigned int pfMax = 1, unsigned int limit = 8, QObject * parent = 0 );
		
		bool open( const QString & filePath );

		bool isEmpty() const;
		void setPrefetchMax( unsigned int pfMax );
		unsigned int getPrefetchMax() const;
		void setLimit( unsigned int limit );
		unsigned int getLimit() const;
		QString getDirPath() const;
		QString getFilePath() const;

		const QPixmap & getImage( const QString & filePath );

	public slots:
		void next();
		void prev();
	
	signals:
		void getImage( const QPixmap & );
	
	private:
		void prefetch_( unsigned int index );
		const QPixmap & fetch_( const QString & );
		bool update_( const QString & );

		unsigned int prefetchMax_;
		unsigned int limit_;
		QDir dir_;
		QStringList files_;
		int index_;
		QQueue< QString > history_;
		QMap< QString, QPixmap > cache_;
		QMutex lock;
	};

	typedef Loki::SingletonHolder< FileControllerBase > FileController;

}

#endif
