/**
 * @file filecontroller.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KOMIX_FILECONTROLLER_HPP
#define KOMIX_FILECONTROLLER_HPP

#include "filemodel.hpp"

#include <QPixmap>
#include <QModelIndex>

#include <loki/Singleton.h>

namespace KomiX {

	namespace private_ {

		/**
		 * @brief The base file controller
		 * @warning <strong style="color: red;">DO NOT USE THIS CLASS TO CREATE OBJECT!</strong>
		 *
		 * This class is used to provide a generic open image action, and
		 * another important function: <strong>image caching</strong>.
		 */
		class FileController : public QObject {
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
			FileController( QObject * parent = 0 );

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

			QSharedPointer< FileModel > getFileModel() const;

			/**
			 * @brief get image by file path
			 * @param filePath file path
			 * @return opened image
			 * @sa getImage( const QPixmap & )
			 *
			 * This function well update cache list, that's why it's not
			 * const member function.
			 */
			//const QPixmap & getImage( const QString & filePath );

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
			void imageLoaded( const QPixmap & image );

		private:
			int index_;

			QSharedPointer< FileModel > model_;
		};

	}

	/// File controller singleton
	typedef Loki::SingletonHolder< private_::FileController > FileController;

}

#endif
