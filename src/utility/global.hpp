/**
 * @file global.hpp
 * @author Wei-Cheng Pan
 *
 * KomiX, a comics viewer.
 * Copyright (C) 2008  Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

	/// File menu hook
	typedef QAction * ( * FileMenuHook )( QWidget * );
	/// register file menu hook
	bool registerFileMenuHook( FileMenuHook hook );
	/// get file menu hooks
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

	/**
	 * @brief make @p exts to name filter
	 */
	QStringList toNameFilter( const QStringList & exts );

}

#endif
