
#ifndef _ILOGFILE_H_
#define _ILOGFILE_H_

#include "os_refactor.h"

#ifndef _WIN32
#define LOGFILE_NAMESPACE_BEGIN namespace mylogfile{
#define LOGFILE_NAMESPACE_END   }
#define USING_LOGFILE_NAMESPACE using namespace mylogfile;

#define LOGFILE_API
#else
#define LOGFILE_NAMESPACE_BEGIN
#define LOGFILE_NAMESPACE_END
#define USING_LOGFILE_NAMESPACE
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the LOGFILE_EXPORT
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// LOGFILE_EXPORT functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.


#endif

#define MAX_TRACENUM       20
enum KGLOG_PRIORITY
{
	KGLOG_DEBUG	    =   1,  // debug-level messages
	KGLOG_WARNING	=   2,  // warning conditions
	KGLOG_ERR		=   3,  // error conditions
	KGLOG_INFO	    =   4,  // informational 
	KGLOG_PRIORITY_MAX = 4
};

typedef struct _TTime/*当前时间*/
{
	unsigned int      ulYear;           /*0000-9999*/
	unsigned int      ulMonth;          /*00-12*/
	unsigned int      ulDay;            /*01-31*/
	unsigned int      ulHour;           /*00-23*/
	unsigned int      ulMinute;         /*00-59*/
	unsigned int      ulSecond;         /*00-59*/
	unsigned int      ulMSecond;        /*000-999*/
}TTime;

class ILogFile
{
public:
	virtual ~ILogFile()
	{
	};
	virtual int GetServerVersion(){return 1;};
	virtual void SetLogPath(const char *pLogPath) = 0;
	virtual void SetMaxLogFile(unsigned int nMaxSize, unsigned int nMaxFileNum) = 0;


	virtual void WriteLogFile(int nPriority, const char* msg, ...) = 0;

	virtual void BinLog(const char* pszFileName, char *pBuffer,  unsigned int iLength) = 0;
	virtual void DbgBinLog(char *pBuffer,  unsigned int iLength) = 0;        /*记录binlog到普通日志*/
	virtual	void TraceLog(unsigned int pszName, const char* msg, ...) = 0;
	virtual void SetTraceName(int nUinTraceNum, unsigned int pszNames[MAX_TRACENUM]) = 0;	

	virtual void SetThreadModuleName(char szThreadModuleName[MAX_PATH]) = 0;
	virtual	void SetThreadDbgLogFlag(int nDbgLog) = 0;
	virtual	void SetThreadWarnLogFlag(int nWarnLog) = 0;
	virtual	void SetThreadInfoLogFlag(int nInfoLog) = 0;
	virtual void ThreadLog(int nThreadIndex, int nPriority, const char* msg, ...) = 0;        
	virtual	void ThreadLogToFile(const char* pszLogFile, int nLogPriorty, const char* msg, ...) = 0;

	virtual	void SetBakLogPath(const char *pBakLogPath) = 0;   /* 设置日志备份的路径，可选项，默认为：日志路径/baklogs/ */
	virtual	void LogToFileByDay(const char* pszLogFile, const char* msg, ...) = 0;
};

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C"
{
	DLL_EXPORT ILogFile* CreateLogFile();
}

typedef ILogFile* (*CreateLogFileProc)();
static ILogFile* CreateLogFilePtr(char szDllFile[MAX_PATH], HMODULE &hModule)
{
	char szFunctionName[100] = "CreateLogFile";
	CreateLogFileProc pfCreateLogFile;

#ifndef _WIN32
	void *pDllHandle = dlopen(szDllFile, RTLD_NOW);
	char *pcError = dlerror();
	if( pcError )
	{
		printf("Open Dll %s failed, %s\n", szDllFile, pcError);
		return NULL;
	}

	pfCreateLogFile = (CreateLogFileProc)dlsym(pDllHandle, szFunctionName);
	pcError = dlerror();
	if( pcError )
	{
		dlclose(pDllHandle);
		printf("dlsym failed, %s\n", pcError);
		return NULL;
	}
#else
	HMODULE pDllHandle = ::LoadLibrary(szDllFile);
	if(pDllHandle != NULL)
	{
		pfCreateLogFile = (CreateLogFileProc)GetProcAddress(
			pDllHandle, szFunctionName);

		if( !pfCreateLogFile )
		{
			FreeLibrary(pDllHandle);
			return NULL;
		}
	}
    else
    {
        return NULL;
    }
#endif
	hModule = pDllHandle;
	return (*pfCreateLogFile)();
}

#endif
