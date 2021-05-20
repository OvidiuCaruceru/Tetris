
/*!
 * \file  exception.hpp
 * \brief
 */

#if !defined(CONSOLE_TYPES_H__)
#define CONSOLE_TYPES_H__

/**/
#include <cstddef>


//
// OS detection
// (defines the appropriate macro that is used to wrap some
// platform specific things)
//
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #define OS_WIN
#elif defined(__unix__) || defined(__unix) || defined(__linux__)
    #define OS_LINUX
#elif defined(__APPLE__)
    #define OS_MACOS
#else
    #error Unsupported platform!
#endif

#include <exception>
#include <string>


namespace xtd {

    ///
    /// \brief exception structure
    ///
    struct exception :
        std::exception
    {
    private:
        std::string what_;

    public:
        ///
        exception(char const* const what) :
            what_(what)
        {
        }

        ///
        /// \brief get the message associated to exception
        /// \return
        ///
        virtual
        const char* what() const noexcept {
            return what_.c_str();
        }
    };

}

#endif // CONSOLE_TYPES_H__
