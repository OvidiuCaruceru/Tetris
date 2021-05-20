#if !defined (EXCEPTION_H__)
#define EXCEPTION_H__

#include <exception>

namespace asd {
	struct exception : 
		public std::exception {
		exception(char const* what) :
			std::exception (what) {					// E derivat public din except si apelez explicit constructorul
		}
	};
}

#endif