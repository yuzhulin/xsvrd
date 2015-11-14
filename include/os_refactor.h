#ifndef __OS_REFACTOR__
#define __OS_REFACTOR__

// header file
///////////////////////////////////////////////////////////
#include <stdint.h>
#include <iostream>
#include <sys/stat.h> // for struct stat

#if defined(_WIN32)
#include <windows.h>
#include <io.h> // for access func
#elif defined(__gnu_linux__)
#include <dlfcn.h>   // for dynamic linking file
#include <string.h>  // for strncpy
#endif


// const 
///////////////////////////////////////////////////////////
#ifndef TRUE
#define TRUE					1
#endif 

#ifndef FALSE
#define FALSE					0
#endif

#ifndef MAX_PATH
#define MAX_PATH				260
#endif

#ifndef MAX_NAME				
#define MAX_NAME				260
#endif

#ifndef SWITCH_ON
#define SWITCH_ON				1
#endif

#ifndef SWITCH_OFF
#define SWITCH_OFF				0
#endif

// F_OK,X_OK,W_OK,R_OK is defined in unistd.h, used for access function.
#ifndef F_OK
#define F_OK					0
#endif
#ifndef X_OK
#define X_OK					1
#endif
#ifndef W_OK
#define W_OK					2
#endif
#ifndef R_OK
#define R_OK					4
#endif

// dynamic linking file suffix
#if defined(_WIN32)
#define DLF_EXTEND_NAME			".dll"
#elif defined(__gnu_linux__)
#define DLF_EXTEND_NAME			".so"
#endif

// type
///////////////////////////////////////////////////////////
typedef uint32_t				uint32;
typedef int32_t					int32;
typedef uint8_t					uint8;
typedef int8_t					int8;

#ifndef SOCKET
#define SOCKET int	
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#if defined(__gnu_linux__)
typedef void* HMODULE;
#endif

enum TCPSocketType {
	TST_COMMON,
	TST_LISTEN
};

// ignore compile warn info
///////////////////////////////////////////////////////////
#if defined(_WIN32)
#pragma warning(disable: 4099)
#pragma warning(disable: 4996)
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)
#endif


#endif
