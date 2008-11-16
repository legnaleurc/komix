/**
 * @file global.hpp
 */
#ifndef KOMIX_GLOBAL_HPP
#define KOMIX_GLOBAL_HPP

#include <QtGlobal>

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

/**
 * @namespace KomiX
 * @brief Contains all KomiX components
 */
namespace KomiX {

	/**
	 * @brief Get the supported formats filter
	 * @return A QStringList that can pass to name filter
	 *
	 * The string format is like this: <strong>"*.<ext>"</strong>.\n
	 * This list is storing as static local variable,
	 * and initialized at start-up routine,
	 * so do not worry about race condition.
	 */
	const QStringList & SupportedFormatsFilter();

}

#endif
