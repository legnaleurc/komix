/**
 * @file filecontroller.hpp
 * @author Wei-Cheng Pan
 */
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

	/**
	 * @brief The base file controller
	 * @warning <strong style="color: red;">DO NOT USE THIS CLASS TO CREATE OBJECT!</strong>
	 *
	 * This class is used to provide a generic open image action, and
	 * another important function: <strong>image caching</strong>.
	 */
	class FileControllerBase : public QObject {
		Q_OBJECT
	
	public:
		/**
		 * @brief default constructor
		 * @param pfMax max prefetch count
		 * @param limit max cache count
		 * @param parent parent widget
		 *
		 * The cache count @p limit is including prefetch count @p pfMax. \n
		 * If @p pfMax and @p limit are less than 0, it will sets to 0.\n
		 * If @p pfMax is greater than @p limit, it will sets to @p limit.
		 */
		FileControllerBase( int pfMax = 1, int limit = 8, QObject * parent = 0 );
		~FileControllerBase();

		/**
		 * @brief open a file or directory by path
		 * @param filePath file path
		 * @retval true if emited getImage( const QPixmap & )
		 * @retval false nothing happend
		 *
		 * It will emit getImage( const QPixmap & ) if necessary.
		 */
		bool open( const QString & filePath );

		/// check if there has openable files.
		bool isEmpty() const;
		/**
		 * @brief set max prefetch count
		 * @param pfMax max count
		 * @sa getPrefetchMax()
		 *
		 * If @p pfMax greater than limit, sets to limit.
		 */
		void setPrefetchMax( int pfMax );
		/**
		 * @brief get max prefetch count
		 * @return max count
		 * @sa setPrefetchMax()
		 */
		int getPrefetchMax() const;
		/**
		 * @brief set max cache count
		 * @param limit max count
		 * @sa getLimit()
		 *
		 * If @p limit less than 0, sets to 0.
		 */
		void setLimit( int limit );
		/**
		 * @brief set max cache count
		 * @return max count
		 * @sa setLimit()
		 */
		int getLimit() const;
		/**
		 * @brief get current directory path
		 * @return directory path
		 */
		QString getDirPath() const;
		/**
		 * @brief get current file path
		 * @return file path
		 */
		QString getFilePath() const;

		/**
		 * @brief get image by file path
		 * @param filePath file path
		 * @return opened image
		 * @sa getImage( const QPixmap & )
		 *
		 * This function well update cache list, that's why it's not
		 * const member function.
		 */
		const QPixmap & getImage( const QString & filePath );

	public slots:
		/**
		 * @brief go to next image
		 * @sa prev()
		 *
		 * This function well emit getImage( const QPixmap & ), and
		 * prefetch images.
		 */
		void next();
		/**
		 * @brief go to previous image
		 * @sa next()
		 *
		 * This function well emit getImage( const QPixmap & ).
		 */
		void prev();
	
	signals:
		/**
		 * @brief get cached image
		 * @param image cached image
		 * @sa getImage( const QString & )
		 */
		void getImage( const QPixmap & image );
	
	private:
		static const QString & SevenZip_();
		static QStringList Arguments_( const QString & );
		static QDir ArchiveDir_( const QString & );
		static const QDir TmpDir_;

		void prefetch_( int index );
		const QPixmap & fetch_( const QString & );
		bool update_( const QString & );

		int prefetchMax_;
		int limit_;
		QDir dir_;
		QStringList files_;
		int index_;
		QQueue< QString > history_;
		QMap< QString, QPixmap > cache_;
		QMutex lock_;
	};

	/// File controller singleton
	typedef Loki::SingletonHolder< FileControllerBase > FileController;

}

#endif