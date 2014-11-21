
#ifndef _ILOGFILE_H_
#define _ILOGFILE_H_



#ifndef WIN32
#define LOGFILE_NAMESPACE_BEGIN namespace mylogfile{
#define LOGFILE_NAMESPACE_END   }
#define USING_LOGFILE_NAMESPACE using namespace mylogfile;

#define LOGFILE_API
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
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

#define LOGFILE_API

#endif

#ifndef WIN32

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#else

#include <windows.h>
#pragma warning(disable: 4099)
#pragma warning(disable: 4996)
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)

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

typedef struct _TTime/*��ǰʱ��*/
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
	virtual void SetLogName(const char* pLogName) = 0;
	virtual void SetMaxLogFile(unsigned int nMaxSize, unsigned int nMaxFileNum) = 0;
	virtual void SetDbgLogFlag(int iLogFlag) = 0;
	virtual void SetWarnLogFlag(int iErrLogFlag) = 0;
	virtual void SetInfoLogFlag(int iInfoLogFlag) = 0;
	virtual void SetBinLogFlag(int iBinLogFlag) = 0;
	virtual void SetLockFlag(int bLogFlag) = 0;
	virtual void SetShowTimeFormat(int ishowms) = 0;

	virtual void WriteLogFile(int nPriority, const char* msg, ...) = 0;

	virtual	void DbgLog(const char* msg, ...) = 0;        /*��¼��ͨ��־*/
	virtual	void WarnLog(const char* msg, ...) = 0;     /*��¼������־*/
	virtual	void ErrLog(const char* msg, ...) = 0;   /*��¼������־*/
	virtual	void InfoLog(const char* msg, ...) = 0; /*ָ���ļ�����ӡ��־*/

	virtual void BinLog(const char* pszFileName, char *pBuffer,  unsigned int iLength) = 0;
	virtual void DbgBinLog(char *pBuffer,  unsigned int iLength) = 0;        /*��¼binlog����ͨ��־*/
	virtual	void LogToFile(const char* pszLogFile, const char* msg, ...) = 0;
	virtual	void TraceLog(unsigned int pszName, const char* msg, ...) = 0;
	virtual void SetTraceName(int nUinTraceNum, unsigned int pszNames[MAX_TRACENUM]) = 0;	

	virtual void SetThreadModuleName(char szThreadModuleName[MAX_PATH]) = 0;
	virtual	void SetThreadDbgLogFlag(int nDbgLog) = 0;
	virtual	void SetThreadWarnLogFlag(int nWarnLog) = 0;
	virtual	void SetThreadInfoLogFlag(int nInfoLog) = 0;
	virtual void ThreadLog(int nThreadIndex, int nPriority, const char* msg, ...) = 0;        
	virtual	void ThreadLogToFile(const char* pszLogFile, int nLogPriorty, const char* msg, ...) = 0;

	virtual	void SetBakLogPath(const char *pBakLogPath) = 0;   /* ������־���ݵ�·������ѡ�Ĭ��Ϊ����־·��/baklogs/ */
	virtual	void LogToFileByDay(const char* pszLogFile, const char* msg, ...) = 0;
};

#ifdef WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C"
{
	DLL_EXPORT ILogFile* CreateLogFile();
}

typedef ILogFile* (*CreateLogFileProc)();
#ifndef WIN32
typedef void* HMODULE;
#endif

static ILogFile* CreateLogFilePtr(char szDllFile[MAX_PATH], HMODULE &hModule)
{
	char szFunctionName[100] = "CreateLogFile";
	CreateLogFileProc pfCreateLogFile;

#ifndef WIN32
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

class LOGFILE_API CLogWrap
{
public:
	CLogWrap()
	{
		m_hDllHandle = NULL;
		m_pLogFile = NULL;
	}
	CLogWrap(char szDllFile[MAX_PATH])
	{
		m_pLogFile = NULL;
		m_hDllHandle = NULL;
		InitDll(szDllFile);
	}

#ifndef WIN32
	void InitDll(char* pszDllFile = "../dll/logfile.so")
#else
	void InitDll(char* pszDllFile = "../dll/logfile.dll")
#endif
	{
		if( m_pLogFile == NULL )
		{
			memset(m_szDllName, 0, sizeof(m_szDllName));
			strncpy(m_szDllName, pszDllFile, sizeof(m_szDllName) - 1);
			m_pLogFile = CreateLogFilePtr(pszDllFile, m_hDllHandle);
		}
	}

	~CLogWrap()
	{
#ifndef NO_FREEDLL
		delete m_pLogFile;
#endif
		m_pLogFile = NULL;

		//ж��dll
#ifdef WIN32
		if( m_hDllHandle )
		{
			FreeLibrary(m_hDllHandle);
			m_hDllHandle = NULL;
		}
#else
		if( m_hDllHandle )
		{
			dlclose(m_hDllHandle);
			m_hDllHandle = NULL;
		}
#endif
	}

	ILogFile* GetLogFile()
	{
		return m_pLogFile;
	}

	ILogFile* operator ->()
	{
		return m_pLogFile;
	}


private:
	ILogFile* m_pLogFile;
	char m_szDllName[MAX_PATH];
	HMODULE m_hDllHandle;
};


#endif