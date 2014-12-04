#ifndef __OS_REFACTOR__
#define __OS_REFACTOR__

// header file
///////////////////////////////////////////////////////////
#include <stdint.h>
#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#endif

// const 
///////////////////////////////////////////////////////////
#ifndef TRUE
#define TRUE					1
#endif 

#ifndef FALSE
#define FALSE					0
#endif

// dynamic linking file suffix
#if defined(_WIN32)
#define DLF_EXTEND_NAME				".dll"
#elif defined(__gnu_linux__)
#define DLF_EXTEND_NAME				".so"
#endif

// type
///////////////////////////////////////////////////////////
typedef uint32_t		uint32;
typedef int32_t			int32;

#if defined(__gnu_linux__)
typde void* HMODULE;
#endif




#endif