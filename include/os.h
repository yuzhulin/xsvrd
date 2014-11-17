#ifndef _XSVRD_INCLUDE_OS_H_
#define _XSVRD_INCLUDE_OS_H_

#include <cstdint>

typedef std::int8_t     int8;
typedef std::int16_t	int16;
typedef std::int32_t	int32;
typedef std::uint8_t	uint8;
typedef std::uint16_t	uint16;
typedef std::uint32_t	uint32;

// windows
////////////////////////////////////////////////////
#ifdef _WIN32
#define strcasecmp		strcmp

#define __GNUC__		1
#define __GNUC_MINOR__	0

#endif

// linux
////////////////////////////////////////////////////
#ifdef __gnu_linux__

#endif


// maxosx
////////////////////////////////////////////////////
#ifdef __APPLE__
#endif




#endif // _XSVRD_INCLUDE_OS_H_
