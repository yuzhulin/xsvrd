#ifndef _XSVRD_INCLUDE_OS_H_
#define _XSVRD_INCLUDE_OS_H_

#include <cstdint>

typedef std::int8_t     int8;
typedef std::int16_t	int16;
typedef std::int32_t	int32;
typedef std::uint8_t	uint8;
typedef std::uint16_t	uint16;
typedef std::uint32_t	uint32;



#ifndef _WIN_LINUX_H_
#define _WIN_LINUX_H_

#if defined (__GNUC__)
#define __FUNC__ ((const char*)(__PRETTY_FUNCTION__))
#elif defined (__STDC_VERSION__) && __STDC_VERSION__ >= 19901L//TODO:这个地方可能会有问题，后面没加括号
#define __FUNC__ ((const char*)(__func__))
#else
#define __FUNC__ ((const char*)(__FUNCTION__))
#endif

typedef unsigned char ___uint8___;
typedef unsigned short ___uint16___;
typedef unsigned int   ___uint32___;
typedef unsigned long long ___uint64___;
typedef signed long long ___int64___;
typedef signed char ___int8___;
typedef signed int    ___int32___;
typedef signed short  ___int16___;

typedef struct {
	___uint32___ Data1;
	___uint16___ Data2;
	___uint16___ Data3;
	___uint8___  Data4[8];
} ___GUID___;


#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <uuid/uuid.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "signal.h"
#include "unistd.h"
#include "stdarg.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/ipc.h"
#include "sys/shm.h"
#include "sys/types.h"
#include "dirent.h"
#include "sys/stat.h"
#include "unistd.h"
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <dlfcn.h>
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif 

#ifndef SOCKET
#define SOCKET int
#endif

#define TRUE 1
#define FALSE 0
#define MAX_PATH  260

typedef char TFName [64];
#define WINCHAR_POINT void

#define closesocket close
#define MKDIR(a, b) mkdir(a, b)
#else

#pragma warning(disable: 4996)
#include <WinSock2.h>
#include <windows.h>
#include <process.h>

#define usleep(a)  Sleep(a/1000)

#ifndef __FUNCTION__
#define __FUNCTION__  "windowfunc"
#endif

#define strncasecmp strnicmp        

#define MKDIR(a, b) mkdir(a)


//typedef 
///////////////////////////////////////////////
typedef int pid_t;
typedef int key_t;
typedef HANDLE pthread_t;
typedef int pthread_attr_t;
typedef int pthread_cond_t;
typedef int socklen_t;
////////////////////////////////////////////////


typedef CRITICAL_SECTION pthread_mutex_t;
static void pthread_mutex_init(pthread_mutex_t* phMutex, void* pData)
{
	InitializeCriticalSection(phMutex);
}
static int pthread_mutex_lock(pthread_mutex_t* phMutex)
{
	EnterCriticalSection(phMutex);  
	return 0;
};

static int pthread_mutex_unlock(pthread_mutex_t* phMutex)
{
	LeaveCriticalSection(phMutex); 
	return 0;
}
static void pthread_mutex_destroy(pthread_mutex_t* phMutex)
{
	DeleteCriticalSection(phMutex);
}

#define ThreadType     UINT WINAPI
typedef unsigned int (__stdcall *thread_func)(void*);

static int pthread_create(pthread_t *pid, pthread_attr_t *pstAttr, 
						  thread_func ThreadProc, void* arglist)
{
	HANDLE hThread;
	hThread = (HANDLE)_beginthreadex(0, 0, ThreadProc,
		(void *)arglist, 0, NULL);
	CloseHandle(hThread);
	return 0;
}
static int pthread_self()
{
	return GetCurrentThreadId();
}

#endif

#endif





#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#include <direct.h>
#endif


#ifndef _WIN32
#include <pthread.h>
#endif



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
