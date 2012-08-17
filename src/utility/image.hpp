#ifndef KOMIX_UTILITY_IMAGE_HPP
#define KOMIX_UTILITY_IMAGE_HPP

#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtGui/QLabel>

#include <memory>

namespace KomiX {

	class Image {
	public:
		Image();
		explicit Image( const QString & path );

		QLabel * createLabel() const;

	private:
		class Private;
		std::shared_ptr< Private > p_;
	};

}

Q_DECLARE_METATYPE( KomiX::Image );

#endif
