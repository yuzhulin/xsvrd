#include "thread.h"
#include "tlib_log.h"
#include "win_linux.h"
#include "base.h"
#include "ilogger.h"
#include "iconfigfile.h"


#ifndef WIN32
#define ThreadType void*
#endif

ThreadType ThreadProc( void *pvArgs )
{
	if( !pvArgs )
	{
		return NULL;
	}

	CThread *pThread = (CThread *)pvArgs;

	pThread->ThreadInit();

	if( pThread->PrepareToRun() )
	{
	    pThread->SetThreadExit();
		return NULL;
	}
	
	pThread->SetThreadRun();
	pThread->Run();
	pThread->SetThreadExit();
	return NULL;
}

CThread::CThread()
{
	m_iRunStatus    = rt_init;
    m_nNotifyStatus = normalstatus;
	m_nThreadIndex  = 0;
	memset(&m_thrLogCfg, 0, sizeof(m_thrLogCfg));
	memset(&m_stThrCfg, 0, sizeof(m_stThrCfg));
	m_pLogWrap = NULL;
	m_pThreadLogFile = NULL;
	m_bInitLogCfg = false;
	memset((void *)&m_stLogCfg, 0, sizeof(m_stLogCfg));
}

CThread::~CThread()
{
	if( m_pLogWrap )
	{
		delete m_pLogWrap;
		m_pLogWrap = NULL;
	}
}

int CThread::CreateThread()
{
#ifndef WIN32
    pthread_attr_init( &m_stAttr );
    pthread_attr_setscope( &m_stAttr, PTHREAD_SCOPE_SYSTEM );
    pthread_attr_setdetachstate( &m_stAttr, PTHREAD_CREATE_JOINABLE );
    pthread_cond_init( &m_stCond, NULL );
    pthread_mutex_init( &m_stMutex, NULL );
#else
	m_hCSEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
    pthread_create( &m_hTrd, &m_stAttr, ThreadProc, (void *)this );

    return 0;
}

int CThread::CondBlock()
{
#ifndef WIN32
	pthread_mutex_lock( &m_stMutex );

	while( IsToBeBlocked() || m_iRunStatus == rt_stopped )
	{
		if( m_iRunStatus == rt_stopped )
		{
			pthread_exit( (void *)m_abyRetVal ); 
			return 0;
		}
		m_iRunStatus = rt_blocked;
		pthread_cond_wait( &m_stCond, &m_stMutex );
	}

    m_iRunStatus = rt_running;

    pthread_mutex_unlock( &m_stMutex );
#else
	while( IsToBeBlocked() )
	{
		m_iRunStatus = rt_blocked;
		DWORD dwStatus = WaitForSingleObject(m_hCSEvent, INFINITE);
		if( dwStatus == WAIT_OBJECT_0 )
		{
			m_iRunStatus = rt_running;
			break;
		}
	}	
#endif
    return 0;
}

int CThread::WakeUp(bool bMustWakeup)
{
#ifndef WIN32
    pthread_mutex_lock( &m_stMutex );

    if(   (!IsToBeBlocked() && m_iRunStatus == rt_blocked)
		|| bMustWakeup )
    {
        pthread_cond_signal( &m_stCond );
    }

    pthread_mutex_unlock( &m_stMutex );
#else
	if(   (!IsToBeBlocked() && m_iRunStatus == rt_blocked)
		|| bMustWakeup )
	{
		SetEvent(m_hCSEvent);
	}	
#endif

    return 0;
}

int CThread::StopThread()
{
#ifndef WIN32
    pthread_mutex_lock( &m_stMutex );

    m_iRunStatus = rt_stopped;
    pthread_cond_signal( &m_stCond );

    pthread_mutex_unlock( &m_stMutex );

    //等待该线程终止
    pthread_join( m_hTrd, NULL );
#else
	m_iRunStatus = rt_stopped;
	SetEvent(m_hCSEvent);
#endif
    return 0;
}

void CThread::ThreadInit()
{
	if( m_bInitLogCfg )
	{
		ReadThreadLogCfg();
		InitLogFile();
	}
}

void CThread::ReadThreadLogCfg()
{	
	int i;
	char szLogThreadIDs[MAX_PATH];
	memset(szLogThreadIDs, 0, sizeof(szLogThreadIDs));
	CConfigFileWrap stCfgWrap(m_stThrCfg.m_szThreadCfgName, m_stThrCfg.m_szConfigFileDllName);
	stCfgWrap.GetItemValueInt(m_stThrCfg.m_szThreadModuleName,  "DbgFlag",
		(int&)m_thrLogCfg.m_nDbgLog, 0);	
	stCfgWrap.GetItemValueInt(m_stThrCfg.m_szThreadModuleName,  "WarnFlag",
		(int&)m_thrLogCfg.m_nWarnLog, 1);
	stCfgWrap.GetItemValueInt(m_stThrCfg.m_szThreadModuleName,  "InfoFlag",
		(int&)m_thrLogCfg.m_nInfoLog, 1);	
	stCfgWrap.GetItemValueInt(m_stThrCfg.m_szThreadModuleName,  "ShowMs",
		(int&)m_thrLogCfg.m_nShowMs, 0);
	stCfgWrap.GetItemValueString(m_stThrCfg.m_szThreadModuleName,  "logthreadid",
		szLogThreadIDs, sizeof(szLogThreadIDs), "-1");

	TrimStr(szLogThreadIDs);

	bool bNeedThreadLog = false;
	if( strcmp(szLogThreadIDs, "-1") != 0 )
	{
		int nThreadIDs[100];
		for( i = 0; i < 100; i++ )
		{
			nThreadIDs[i] = -1;
		}
		int nNum = SplitMapStr(szLogThreadIDs, 100, nThreadIDs, ',');
		for( i = 0; i < nNum; i++ )
		{
			if( nThreadIDs[i] == m_nThreadIndex )
			{
				bNeedThreadLog = true;
			}
		}
	}
	else
	{
		bNeedThreadLog = true;
	}

	if( bNeedThreadLog != true )
	{
		m_thrLogCfg.m_nDbgLog  = 0;
		m_thrLogCfg.m_nWarnLog = 0;
		m_thrLogCfg.m_nInfoLog = 0;
	}
	
	stCfgWrap.GetItemValueString(m_stThrCfg.m_szThreadModuleName,  "LogPath",
		m_stThrCfg.m_szThreadLogPath, sizeof(m_stThrCfg.m_szThreadLogPath), "");

	int nStrLen = strlen(m_stThrCfg.m_szThreadLogPath);
	if( m_stThrCfg.m_szThreadLogPath[nStrLen - 1] == '\\' )
	{
		m_stThrCfg.m_szThreadLogPath[nStrLen - 1] = 0;
	}
	if( m_stThrCfg.m_szThreadLogPath[nStrLen - 1] == '/' )
	{
		m_stThrCfg.m_szThreadLogPath[nStrLen - 1] = 0;
	}
}


void CThread::InitLogFile()
{
	// TODO: comment
	/*if( m_pLogWrap == NULL )
	{
		m_pLogWrap = new CLogWrap(m_stThrCfg.m_szLogFileDllName);
		m_pThreadLogFile = m_pLogWrap->GetLogFile();
	}
	if( m_pThreadLogFile )
	{
		char szLogPath[MAX_PATH];
		memset(szLogPath, 0, sizeof(szLogPath));
		snprintf(szLogPath, sizeof(szLogPath) - 1,
			"%s/%s", m_stThrCfg.m_szThreadLogPath, m_stThrCfg.m_szThreadModuleName);
		m_pThreadLogFile->SetLogPath(szLogPath);
		m_pThreadLogFile->SetThreadModuleName(m_stThrCfg.m_szThreadModuleName);
		m_pThreadLogFile->SetThreadDbgLogFlag(m_thrLogCfg.m_nDbgLog);
		m_pThreadLogFile->SetThreadWarnLogFlag(m_thrLogCfg.m_nWarnLog);
		m_pThreadLogFile->SetThreadInfoLogFlag(m_thrLogCfg.m_nInfoLog);
		m_pThreadLogFile->SetShowTimeFormat(m_thrLogCfg.m_nShowMs);
	}*/
}

void CThread::ReloadThreadLogCfg()
{
	ReadThreadLogCfg();
	if( m_pThreadLogFile )
	{
		m_pThreadLogFile->SetThreadDbgLogFlag(m_thrLogCfg.m_nDbgLog);
		m_pThreadLogFile->SetThreadWarnLogFlag(m_thrLogCfg.m_nWarnLog);
		m_pThreadLogFile->SetThreadInfoLogFlag(m_thrLogCfg.m_nInfoLog);
		m_pThreadLogFile->SetShowTimeFormat(m_thrLogCfg.m_nShowMs);
	}
}

void CThread::ThreadLogInit(char *sPLogBaseName, int lPMaxLogSize, int iPMaxLogNum, int iShow)
{
	memset(m_stLogCfg.sLogBaseName, 0, sizeof(m_stLogCfg.sLogBaseName));
	strncpy(m_stLogCfg.sLogBaseName, sPLogBaseName, sizeof(m_stLogCfg.sLogBaseName)-1);
	m_stLogCfg.lMaxLogSize = lPMaxLogSize;
	m_stLogCfg.iMaxLogNum = iPMaxLogNum;
	m_stLogCfg.iIsShow = iShow;
	m_stLogCfg.iLogInitialized = 1;
}


void CThread::ThreadLog(const char *sFormat, ...)
{
	va_list ap;

	if (m_stLogCfg.iIsShow != 0)
	{
		printf("[%s] ", TLib_Tools_GetCurDateTimeStr());

		va_start(ap, sFormat);
		vprintf(sFormat, ap);
		va_end(ap);

		printf("\n");
	}

	if (m_stLogCfg.iLogInitialized != 0)
	{
		va_start(ap, sFormat);
		TLib_Log_VWriteLog(m_stLogCfg.sLogBaseName, m_stLogCfg.lMaxLogSize,
			m_stLogCfg.iMaxLogNum, NULL, sFormat, ap);
		va_end(ap);
	}
}

