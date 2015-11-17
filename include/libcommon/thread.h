#ifndef _THREAD_H_
#define _THREAD_H_


#include "win_linux.h"
#ifndef WIN32
#include <pthread.h>
#endif

enum eRunStatus
{
	rt_init = 0,
	rt_blocked = 1,
	rt_running = 2,
	rt_stopped = 3
};

typedef enum _EnThreadStatusFlag
{
    normalstatus = 0,
    thread_reloadcfg = 1,
    notify_stop = 2,
}EnThreadStatusFlag;

typedef struct  
{
	char     m_szConfigFileDllName[MAX_PATH];
	char     m_szLogFileDllName[MAX_PATH];
	char     m_szThreadCfgName[MAX_PATH];
	char     m_szThreadModuleName[MAX_PATH];
	char     m_szThreadLogPath[MAX_PATH];
}TThreadCfg;

typedef struct  
{
	unsigned int m_nDbgLog;
	unsigned int m_nWarnLog;
	unsigned int m_nInfoLog;
	unsigned int m_nShowMs;
}TThreadLogCfg;

typedef struct
{
	char sLogBaseName[200];
	int lMaxLogSize;
	int iMaxLogNum;
	int iLogInitialized;
	int iIsShow;
} TLogCfg;


class CLogWrap;
class ILogFile;

class CThread
{
public:
	CThread();
	virtual ~CThread();

    virtual int PrepareToRun() = 0;
	virtual int Run() = 0;
	virtual bool IsToBeBlocked()
	{
		return false;
	}

	bool GetThreadRunResult(bool &bRunOK)
	{
		if( m_iRunStatus == rt_init )
		{
			return false;
		}
		if( m_iRunStatus == rt_stopped )
		{
			bRunOK = false;
		}
		else
		{
			bRunOK = true;
		}

		return true; 
	}

	void SetThreadRun()
	{
		m_iRunStatus = rt_running;
	}
    
	void SetThreadCfg(TThreadCfg& stThrCfg)
	{
		m_bInitLogCfg = true;
		memcpy(&m_stThrCfg, &stThrCfg, sizeof(stThrCfg));
	}

	void SetThreadIndex(int nThreadIndex)
	{
		m_nThreadIndex = nThreadIndex; } 
    void NotifyThreadReloadCfg()
    {
        m_nNotifyStatus = thread_reloadcfg;
    }

	void NotifyThreadExit()
	{
		m_nNotifyStatus = notify_stop;
	}

    bool IsShouldReloadCfg()
    {
        bool bReloadCfg = (m_nNotifyStatus == thread_reloadcfg ? true : false);
        m_nNotifyStatus = normalstatus; 
        return bReloadCfg;
    }
    bool IsShouldExit()
    {
        bool bExit = (m_nNotifyStatus == notify_stop ? true : false);
        return bExit;
    }
    bool IsThreadExit()
    {
        return (m_iRunStatus == rt_stopped ? true : false);
    }

	int CreateThread();
	int WakeUp(bool bMustWakeup = false);
	int StopThread();
	
	void SetThreadExit()
	{
		m_iRunStatus = rt_stopped;
	}

	void SetThreadLogFilePtr(ILogFile* pThreadLogFile)
	{
		m_pThreadLogFile = pThreadLogFile;
	}

	void ThreadInit();

	void ReloadThreadLogCfg();

	void ThreadLogInit(char *sPLogBaseName, int lPMaxLogSize, int iPMaxLogNum, int iShow);

protected:
	int CondBlock();
	void ReadThreadLogCfg();
	void InitLogFile();
	void ThreadLog(const char *sFormat, ...);
	
#ifdef WIN32
	HANDLE m_hCSEvent;
#endif

    int m_nThreadIndex;
    int m_nNotifyStatus;
	int m_iRunStatus;
	pthread_t m_hTrd;
	pthread_attr_t m_stAttr;
	pthread_mutex_t m_stMutex;
	pthread_cond_t m_stCond;

	char m_abyRetVal[64];

	TThreadCfg m_stThrCfg;
	TThreadLogCfg m_thrLogCfg;
	CLogWrap* m_pLogWrap;
	ILogFile* m_pThreadLogFile;
	bool m_bInitLogCfg;
	TLogCfg m_stLogCfg;
};


#endif
