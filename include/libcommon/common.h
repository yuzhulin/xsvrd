#ifndef _BASECOMMON_H_
#define _BASECOMMON_H_

#include "iconfigfile.h"
#include "icrypt.h"
#include "imd5hash.h"

#ifdef WIN32
#define LOGFILE_DLL     "../dll/logfile.dll"
#define CONFIGFILE_DLL  "../dll/configfile.dll"
#define CRYPT_DLL       "../dll/crypt.dll"
#define MD5_DLL         "../dll/md5.dll"
#define COMPRESS_DLL    "../dll/compress.dll"
#define DIRTY_DLL       "../dll/oidirty.dll"
#else
#define LOGFILE_DLL     "../dll/logfile.so"
#define CONFIGFILE_DLL  "../dll/configfile.so"
#define CRYPT_DLL       "../dll/crypt.so"
#define MD5_DLL         "../dll/md5.so"
#define COMPRESS_DLL    "../dll/compress.so"
#define DIRTY_DLL       "../dll/oidirty.so"
#endif

/* output u64 format */
#if __WORDSIZE == 64
# define UINT64FMT "%lu"
#else   
# define UINT64FMT "%llu"
#endif
            
#if __WORDSIZE == 64
# define INT64FMT "%ld"
#else   
# define INT64FMT "%lld"
#endif  

#endif

