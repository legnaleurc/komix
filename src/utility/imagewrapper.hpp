#ifndef KOMIX_UTILITY_IMAGE_HPP
#define KOMIX_UTILITY_IMAGE_HPP

#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsItem>

#include <memory>

namespace KomiX {

	class ImageWrapper {
	public:
		ImageWrapper();
		explicit ImageWrapper( const QString & path );

		QLabel * createLabel() const;
		QGraphicsItem * createGraphicsItem() const;

	private:
		class Private;
		std::shared_ptr< Private > p_;
	};

}

Q_DECLARE_METATYPE( KomiX::ImageWrapper );

#endif
