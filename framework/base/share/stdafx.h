#ifndef __STDAFX_H__
#define __STDAFX_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifdef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <winsock2.h>
#include <io.h>
#include <assert.h>
#include <errno.h>
#include <pdh.h>
#include <time.h>
#include <Winreg.h>
#include <tchar.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <crtdbg.h>
#include <errno.h>
#include <stdlib.h>
#include <process.h>

#else

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

typedef unsigned char BYTE;

#endif

#define ERRPACKETLENGTH               (-2)
#define MY_NAMESPACE_BEGIN
#define MY_NAMESPACE_END

#define __LINUXCONFIG__

#endif /* __STDAFX_H__ */
