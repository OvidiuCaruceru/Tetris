
/*!
 * \brief C++ console framework
 * \file  console.hpp
 */

#if !defined(CONSOLE_H__)
#define CONSOLE_H__

#include "utils/system.hpp"

#if defined(OS_LINUX)
#include "linux/console.hpp"
#elif defined(OS_WIN)
#include "windows/console.hpp"
#endif //

#endif // CONSOLE_H__
