
/*!
 * \file  traits.hpp
 * \brief
 */

#if !defined(TRAITS_H__)
#define TRAITS_H__

/**/
//#define NOMINMAX
#include "utils/system.hpp"

#if defined(OS_LINUX)
#include "linux/traits.hpp"
#elif defined(OS_WIN)
#include "windows/traits.hpp"
#endif //

#endif //  TYPES_H__
