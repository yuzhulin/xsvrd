#ifndef __OS_REFACTOR__
#define __OS_REFACTOR__

// header file
///////////////////////////////////////////////////////////
#include <stdint.h>
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
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

#ifndef SWITCH_ON
#define SWITCH_ON				1
#endif

#ifndef SWITCH_OFF
#define SWITCH_OFF				0
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

#if defined(__gnu_linux__)
typedef void* HMODULE;
#endif




#endif
