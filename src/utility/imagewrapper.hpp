#ifndef KOMIX_UTILITY_IMAGE_HPP
#define KOMIX_UTILITY_IMAGE_HPP

//#include <QtCore/QString>
#include <QtCore/QMetaType>
/*
#include <QtGui/QLabel>
#include <QtGui/QGraphicsItem>

#include <memory>

namespace KomiX {

	class ImageWrapper {
	public:
		ImageWrapper();
		explicit ImageWrapper( QIODevice * device );

		void moveToLabel( QLabel * label, const QSize & size ) const;
		QGraphicsItem * takeGraphicsItem( QGraphicsItem * parent ) const;

	private:
		class Private;
		std::shared_ptr< Private > p_;
	};

}
*/

Q_DECLARE_METATYPE( QIODevice * )

#endif
