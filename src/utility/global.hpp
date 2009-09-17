/**
 * @file global.hpp
 */
#ifndef KOMIX_GLOBAL_HPP
#define KOMIX_GLOBAL_HPP

#include <QStringList>
#include <QAction>

/**
 * @namespace KomiX
 * @brief Contains all KomiX components
 */
namespace KomiX {

	typedef QAction * ( * FileMenuHook )( QWidget * );
	bool registerFileMenuHook( FileMenuHook hook );
	const std::list< FileMenuHook > & getFileMenuHooks();

	/**
	 * @brief Get supported formats
	 * @return A QStringList contains all supported formats
	 * @note Not thread-safe on initialization. Do not initialize before QApplication.
	 *
	 * The formats is read from QImageReader::supportedImageFormats().\n
	 * This list transforms to lower string, sort it, and remove deprecated
	 * items.
	 */
	const QStringList & SupportedFormats();

	/**
	 * @brief Get the supported formats filter
	 * @return A QStringList that can pass to name filter
	 * @note Not thread-safe on initialization.
	 *
	 * The string format is like this: <strong>"*.<ext>"</strong>.
	 */
	const QStringList & SupportedFormatsFilter();

//	const QStringList & ArchiveFormats();
//
//	const QStringList & ArchiveFormatsFilter();
//
//	bool isArchiveSupported( const QString & path );

}

#endif
