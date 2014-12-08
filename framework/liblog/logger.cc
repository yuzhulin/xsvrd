#include "win_linux.h"
#include "logger.h"

#include <iostream>

#if (defined(_WIN32))
#include <io.h>
#endif

#ifndef  _WIN32
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#endif

#ifdef _WIN32
#include "base.h"
#else

static void CreatePath(char szLogPath[MAX_PATH])
{
	if (0 == szLogPath[0])
	{
		printf("CreatePath Error!\n");
		return;
	}
	char szTempLogPath[MAX_PATH];
	memset(szTempLogPath, 0, sizeof(szTempLogPath));
	strncpy(szTempLogPath, szLogPath, sizeof(szTempLogPath) - 1);
	int iTempLen = strlen(szTempLogPath);

	if('/' == szTempLogPath[iTempLen - 1])
	{
		szTempLogPath[iTempLen - 1] = 0;
	}
	char *pTemp = szTempLogPath;
	char *pTemp1;
	char chTemp;
	while(1)
	{
		pTemp1 = strchr(pTemp, '/');
		if(0 == pTemp1)
		{
			MKDIR(szTempLogPath, 0777);
			break;
		}
		if(0 == *(pTemp1 + 1))
		{
			break;
		}

		chTemp = *(pTemp1 + 1);
		*(pTemp1 + 1) = 0;
		if (0 != strcmp("../", szTempLogPath) && 0 != strcmp("./", szTempLogPath))
		{
			MKDIR(szTempLogPath, 0777);
		}
		*(pTemp1 + 1) = chTemp;
		pTemp = pTemp1 + 1;
	}
}
#endif

extern "C"
{
	ILogFile* CreateLogFile()
	{
	#ifndef WIN32
		return new mylogfile::CLogFile;
	#else
		return new CLogFile;
	#endif
	}

	void* CreateObj()
	{
		return new Logger;
	}
};

LOGFILE_NAMESPACE_BEGIN

CLogFile::CLogFile()
{
	SetLogName("");

	memset(m_szTraceNames, 0, sizeof(m_szTraceNames)); 
	memset(m_szLogPath, 0, sizeof(m_szLogPath));
	memset(m_szThreadModuleName, 0, sizeof(m_szThreadModuleName));
	memset(&m_curTime, 0, sizeof(m_curTime));
	memset(m_szBakLogPath, 0, sizeof(m_szBakLogPath));
	memset(m_szLogFileName, 0, sizeof(m_szLogFileName));
	strncpy(m_szLogFileName, "tcpsvrd.log", sizeof(m_szLogFileName) - 1);

	m_bIsLockLog = FALSE;

	m_iDbgLogFlag  = 0;
	m_iWarnLogFlag = 0;
	m_iInfoLogFlag = 1;
	m_iBinLogFlag  = 0;

	m_iThreadDbgLogFlag  = 0;
	m_iThreadWarnLogFlag = 0;
	m_iThreadInfoLogFlag = 0;
	
	m_iShowMs = 0;
	m_nTraceNum = 0;
	pthread_mutex_init( &m_stMutex, NULL );

	max_file_size_ = 16 * 1024 * 1024;
	max_file_num_ = 10;
}

CLogFile::~CLogFile()
{
}

void CLogFile::SetShowTimeFormat(int ishowms)
{
	m_iShowMs = ishowms;
}

void CLogFile::SetLogPath(const char *pLogPath)
{
	if (!pLogPath)
	{
		return;
	}

	memset(m_szLogPath, 0, sizeof(m_szLogPath));
	strncpy(m_szLogPath, pLogPath, sizeof(m_szLogPath) - 2);
	int nLen = strlen(m_szLogPath);
	for( int i = 0; i < nLen; i++ )
	{
		if( m_szLogPath[i] == '\\' )
		{
			m_szLogPath[i] = '/';
		}
	}
	if( m_szLogPath[nLen - 1] != '/' )
	{
		m_szLogPath[nLen] = '/';
	}
    
	char szPath[MAX_PATH] = {0};
	snprintf(szPath, sizeof(szPath) - 1, "%sbaklogs", m_szLogPath);
	SetBakLogPath(szPath);
}

void CLogFile::SetLockFlag(int bIsLockLog)
{
	m_bIsLockLog = bIsLockLog;
}

void CLogFile::SetThreadModuleName(char szThreadModuleName[MAX_PATH])
{
	memset(m_szThreadModuleName, 0, sizeof(m_szThreadModuleName));
	strncpy(m_szThreadModuleName, szThreadModuleName, sizeof(m_szThreadModuleName) - 1);
}

void CLogFile::SetThreadDbgLogFlag(int nDbgLog)
{
	m_iThreadDbgLogFlag = nDbgLog;
}

void CLogFile::SetThreadWarnLogFlag(int nWarnLog)
{
	m_iThreadWarnLogFlag = nWarnLog;
}

void CLogFile::SetThreadInfoLogFlag(int nInfoLog)
{
	m_iThreadInfoLogFlag = nInfoLog;
}

void CLogFile::SetMaxLogFile(unsigned int nMaxSize, unsigned int nMaxFileNum)
{
    if (0 == nMaxSize || 0 == nMaxFileNum)
    {
        return;
    }
	max_file_size_ = nMaxSize;
	max_file_num_ = nMaxFileNum;
}

void CLogFile::SetLogName(const char* pLogName)
{
	if (!pLogName)
	{
		return;
	}

    memset(m_szLogName, 0, sizeof(m_szLogName));
	memset(m_szNormalLogName, 0, sizeof(m_szNormalLogName));
	memset(m_szWarnLogName, 0, sizeof(m_szWarnLogName));
	memset(m_szErrLogName, 0, sizeof(m_szErrLogName));
	memset(m_szInfoLogName, 0, sizeof(m_szInfoLogName));
	memset(m_szLogFileName, 0, sizeof(m_szLogFileName));

	strncpy(m_szLogName, pLogName, sizeof(m_szLogName) - 1);
	snprintf(m_szNormalLogName, sizeof(m_szNormalLogName) - 1, "%s_normal.log", m_szLogName);
	snprintf(m_szWarnLogName, sizeof(m_szWarnLogName) - 1, "%s_warn.log", m_szLogName);
	snprintf(m_szErrLogName, sizeof(m_szErrLogName) - 1, "%s_err.log", m_szLogName);
	snprintf(m_szInfoLogName, sizeof(m_szInfoLogName) - 1, "%s_info.log", m_szLogName);
	snprintf(m_szLogFileName, sizeof(m_szLogFileName) - 1, "%s.log", m_szLogName);
}

void  CLogFile::SetDbgLogFlag(int iLogFlag)
{
	m_iDbgLogFlag = iLogFlag;
}

void  CLogFile::SetWarnLogFlag(int iErrLogFlag)
{
	m_iWarnLogFlag = iErrLogFlag;
}

void  CLogFile::SetInfoLogFlag(int iInfoLogFlag)
{
	m_iInfoLogFlag = iInfoLogFlag;
}

void  CLogFile::SetBinLogFlag(int iBinLogFlag)
{
	m_iBinLogFlag = iBinLogFlag;
}

void CLogFile::SetTraceName(int nUinTraceNum, unsigned int pszNames[MAX_TRACENUM])
{
	if( nUinTraceNum > MAX_TRACENUM )
	{
		nUinTraceNum = MAX_TRACENUM;
	}
	m_nTraceNum = nUinTraceNum;

	for( int i = 0; i < nUinTraceNum; i++ )
	{
		m_szTraceNames[i] = pszNames[i];
	}
}

void CLogFile::Lock()
{
	if( m_bIsLockLog )
	{
		pthread_mutex_lock( &m_stMutex );
	}
}

void CLogFile::Unlock()
{
	if( m_bIsLockLog )
	{
		pthread_mutex_unlock( &m_stMutex );
	}
}

void CLogFile::SetCurrentTime()
{
	struct tm *tmpTime = NULL;

	if( m_iShowMs )
	{
		struct timeval tmNow;
#ifndef WIN32
		gettimeofday(&tmNow, NULL);
#else
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		tmNow.tv_sec  = (long)time(NULL);
		tmNow.tv_usec = sysTime.wMilliseconds*1000;
#endif
		time_t tNow = tmNow.tv_sec;
		m_curTime.ulMSecond = tmNow.tv_usec/1000;
		tmpTime = localtime(&tNow);
	}
	else
	{
		time_t tNow = time(NULL);
		tmpTime = localtime(&tNow);
	}

	m_curTime.ulYear   = tmpTime->tm_year + 1900;
	m_curTime.ulMonth  = tmpTime->tm_mon  + 1;
	m_curTime.ulDay    = tmpTime->tm_mday;
	m_curTime.ulHour   = tmpTime->tm_hour;
	m_curTime.ulMinute = tmpTime->tm_min;
	m_curTime.ulSecond = tmpTime->tm_sec;
}

void CLogFile::BackupFile(const char* pSrcFile, const char* pDstFile)
{
	if( access(pSrcFile, 0) != -1 )
	{
		char szBakLogPath[MAX_PATH] = {0};
		strncpy(szBakLogPath, pDstFile, MAX_PATH - 1);
		char* pEnd = strrchr(szBakLogPath, '/');
		if ( pEnd )
		{
			*pEnd = 0;
		}
		if ( access(szBakLogPath, 0) != 0)
		{
			CreatePath(szBakLogPath);
		}

		unlink(pDstFile);
		rename(pSrcFile, pDstFile);
		unlink(pSrcFile);
	}
}

void CLogFile::GetBakFileName(const char* pFileName, char szBakFileName[MAX_PATH])
{
	time_t tmOld = 0xFFFFFFFF;
	memset(szBakFileName, 0, MAX_PATH);
	for( unsigned int i = 0; i < max_file_num_; i++ )
	{
		time_t tmTemp = 0;
		char szTempName[MAX_PATH];
		memset(szTempName, 0, sizeof(szTempName));
		snprintf(
			szTempName, sizeof(szTempName) - 1, 
			"%s%s_%d.bak", 
			m_szBakLogPath, pFileName, i + 1
		);
		struct stat buf;
		if( stat(szTempName, &buf) != 0 )
		{
			tmTemp = 0;
			strncpy(szBakFileName, szTempName, MAX_PATH - 1);
			return;
		}
		else
		{
			tmTemp = buf.st_mtime;
		}
		if( tmTemp < tmOld )
		{
			tmOld = tmTemp;
			strncpy(szBakFileName, szTempName, MAX_PATH - 1);
		}
	}
}

void CLogFile::BakLogFile(const char* pFileName)
{
	char szLogFile[MAX_PATH];
	sprintf(szLogFile, "%s%s", m_szLogPath, pFileName);

	time_t tmNow = time(NULL);
	unsigned int unFileSize = 0;
	struct stat buf;
	if( stat(szLogFile, &buf) != 0 )
	{
		return;
	}
	if( buf.st_size < (int)max_file_size_ )
	{
		return;
	}
	char szBakFileName[MAX_PATH];
	GetBakFileName(pFileName, szBakFileName);
	BackupFile(szLogFile, szBakFileName);
}

void CLogFile::WriteLog(FILE* pFile, const char *msg, va_list& args, char* pAddStr)
{
	if( m_iShowMs )
	{
		fprintf(pFile,"[%.4u-%.2u-%.2u,%.2u:%.2u:%.2u:%.3u] ",
			m_curTime.ulYear ,
			m_curTime.ulMonth ,
			m_curTime.ulDay,
			m_curTime.ulHour,
			m_curTime.ulMinute,
			m_curTime.ulSecond,
			m_curTime.ulMSecond);
	}
	else
	{
		fprintf(pFile,"[%.4u-%.2u-%.2u,%.2u:%.2u:%.2u] ",
			m_curTime.ulYear ,
			m_curTime.ulMonth ,
			m_curTime.ulDay,
			m_curTime.ulHour,
			m_curTime.ulMinute,
			m_curTime.ulSecond);
	}
	if( pAddStr )
	{
		fprintf(pFile, " %s ", pAddStr);
	}

	vfprintf(pFile, msg, args);
	//fprintf(pFile,"\n");
}

void CLogFile::WriteNormalLog(const char* msg, va_list& args, char* pStrAdd)
{
	if( m_iDbgLogFlag == 0 )
	{
		return;
	}
	WriteToLogFile(m_szNormalLogName, msg, args, pStrAdd);
}

void CLogFile::WriteToLogFile(const char* pFileName, const char* msg, va_list& args, char *pAddStr)
{
	//write之前先考虑备份，备份机制统一考虑。
	Lock();
	char szLogFile[MAX_PATH];
	sprintf(szLogFile, "%s%s", m_szLogPath, pFileName);

	BakLogFile(pFileName);

	FILE *pFile = fopen(szLogFile, "a+");
	if (NULL == pFile)
	{
		char szTempLogFile[MAX_PATH];
		strncpy(szTempLogFile, szLogFile, sizeof(szTempLogFile) - 1);
		char *pTemp = strrchr(szTempLogFile, '/');
		if( pTemp == NULL )
		{
			Unlock();
			return;
		}
		*pTemp = 0;
		if ( access(szTempLogFile, 0) != 0)
		{
			CreatePath(szTempLogFile);
		}
		pFile = fopen(szLogFile, "a+");
	}
	if (pFile)
	{
		WriteLog(pFile, msg, args, pAddStr);
		fclose(pFile);
	}
	Unlock();
}

void CLogFile::WriteLogFile(int nPriority, const char* msg, ...)
{
	char* pAddStr = NULL;
	switch(nPriority)
	{
	case KGLOG_DEBUG:
		if( m_iDbgLogFlag == 0 )
		{
			return;
		}
		break;
	case KGLOG_WARNING:
		if( m_iWarnLogFlag == 0 )
		{
			return;
		}
		pAddStr = "#warn";
		break;
	case KGLOG_INFO:
		if( m_iInfoLogFlag == 0 )
		{
			return;
		}
		pAddStr = "#info#";
		break;
	case KGLOG_ERR:
		pAddStr = "#error#";
		break;
	default:
		return;
	}
	SetCurrentTime();

	//write之前先考虑备份，备份机制统一考虑。
	Lock();
	char szLogFile[MAX_PATH];
	snprintf(szLogFile, sizeof(szLogFile) - 1, "%s%s", m_szLogPath, m_szNormalLogName);
	szLogFile[sizeof(szLogFile) - 1] = 0;
	BakLogFile(m_szLogFileName);
	FILE *pFile = fopen(szLogFile, "a+");
	if (NULL == pFile)
	{
		char szTempLogFile[MAX_PATH];
		strncpy(szTempLogFile, szLogFile, sizeof(szTempLogFile) - 1);
		char *pTemp = strrchr(szTempLogFile, '/');
		if( pTemp == NULL )
		{
			Unlock();
			return;
		}
		*pTemp = 0;
		if ( access(szTempLogFile, 0) != 0)
		{
			CreatePath(szTempLogFile);
		}
		pFile = fopen(szLogFile, "a+");
	}
	if (pFile)
	{
		va_list   args;
		va_start(args, msg);
		WriteLog(pFile, msg, args, pAddStr);
		va_end(args);
		fclose(pFile);
	}
	Unlock();

}

void CLogFile::DbgLog(const char* msg, ...)
{
	if(0 == m_iDbgLogFlag) /*如果无需记录则返回*/
	{
		return;
	}

	SetCurrentTime();
	va_list   args;
	va_start(args, msg);
	WriteNormalLog(msg, args);
	va_end (args);
}

void CLogFile::WarnLog(const char* msg, ...)
{
	if(0 == m_iWarnLogFlag) /*如果WarnLog标志没有打开，则返回*/
	{
		return;
	}
	SetCurrentTime();

	va_list   args;

	va_start(args, msg);
	WriteNormalLog(msg, args, (char*)"#warn#");
	va_end(args);

	va_start(args, msg);
	WriteToLogFile(m_szWarnLogName, msg, args);
	va_end(args);
}

void CLogFile::ErrLog(const char* msg, ...)
{
	SetCurrentTime();
	va_list args;

	va_start(args, msg);
	WriteNormalLog(msg, args, (char*)"#error#");
	va_end(args);

	va_start(args, msg);
	WriteToLogFile(m_szErrLogName, msg, args);
	va_end(args);
}

void CLogFile::BinLog(const char* pszFileName, char *pBuffer, unsigned int iLength) 
{
	if(0 == m_iBinLogFlag) /*如果BinLog标志没有打开，则返回*/
	{
		return;
	}
	SetCurrentTime();

	if( pszFileName == NULL  || pBuffer == NULL )
	{
		return;
	}
	if( iLength >= MAX_BIN_LEN )
	{
		iLength = MAX_BIN_LEN;
	}
	char tmpBuffer[2 * MAX_BIN_LEN + 1];
	char strTemp[64];

	char szLogFile[MAX_PATH] = {0};
	sprintf(szLogFile, "%sbinlog/%04u_%02u_%02u"
		"/%02u_%s", 
		m_szLogPath, m_curTime.ulYear, m_curTime.ulMonth, m_curTime.ulDay
		, m_curTime.ulHour, pszFileName);

	Lock();

	FILE *pFile = fopen(szLogFile, "a+");
	if( pFile == NULL )
	{
		char szPath[MAX_PATH] = {0};
		sprintf(szPath, "%sbinlog/%04u_%02u_%02u", 
			m_szLogPath, m_curTime.ulYear, m_curTime.ulMonth, m_curTime.ulDay);
		CreatePath(szPath);

		pFile = fopen(szLogFile, "a+");
		if( pFile == NULL )
		{
			Unlock();
			return;
		}
	}

	tmpBuffer[0] = 0;
	for( unsigned int i = 0; i < iLength; i++ )
	{
		if(!( i%16 ) )
		{
			sprintf( strTemp, "\n%04d>    ", i/16+1);
			strcat(tmpBuffer, strTemp);
		}
		sprintf( strTemp, "%02X ", (unsigned char )pBuffer[i]);
		strcat(tmpBuffer, strTemp);
	}
	strcat(tmpBuffer, "\n");
	fprintf(pFile, "\n***************[time %02u:%02u:%02u] bufferlen %u***************", 
		m_curTime.ulHour, m_curTime.ulMinute, m_curTime.ulSecond, iLength);
	fprintf(pFile,tmpBuffer);

	fclose(pFile);
	Unlock();
}


void CLogFile::DbgBinLog(char *pBuffer, unsigned int iLength) 
{
	if(0 == m_iBinLogFlag) /*如果BinLog标志没有打开，则返回*/
	{
		return;
	}
	SetCurrentTime();

	if( pBuffer == NULL )
	{
		return;
	}
	if( iLength >= MAX_BIN_LEN )
	{
		iLength = MAX_BIN_LEN;
	}
	char tmpBuffer[2 * MAX_BIN_LEN + 1];
	char strTemp[64];

	char szLogFile[MAX_PATH] = {0};


	sprintf(szLogFile, "%s%s", m_szLogPath, m_szNormalLogName);


	Lock();

	FILE *pFile = fopen(szLogFile, "a+");
	if( pFile == NULL )
	{
		CreatePath(m_szLogPath);

		pFile = fopen(szLogFile, "a+");
		if( pFile == NULL )
		{
			Unlock();
			return;
		}
	}

	tmpBuffer[0] = 0;
	for( unsigned int i = 0; i < iLength; i++ )
	{
		if(!( i%16 ) )
		{
			sprintf( strTemp, "\n%04d>    ", i/16+1);
			strcat(tmpBuffer, strTemp);
		}
		sprintf( strTemp, "%02X ", (unsigned char )pBuffer[i]);
		strcat(tmpBuffer, strTemp);
	}
	strcat(tmpBuffer, "\n");
	//fprintf(pFile, "\n***************[time %02u:%02u:%02u] bufferlen %u***************", 
	//	m_curTime.ulHour, m_curTime.ulMinute, m_curTime.ulSecond, iLength);
	fprintf(pFile,tmpBuffer);

	fclose(pFile);
	Unlock();
}


void CLogFile::InfoLog(const char* msg, ...)
{
	if(0 == m_iInfoLogFlag) /*如果InfoLog标志没有打开，则返回*/
	{
		return;
	}
	if( 0 == m_szInfoLogName[0] )
	{
		return;
	}
	SetCurrentTime();

	va_list args;
	va_start(args, msg);
	WriteToLogFile(m_szInfoLogName, msg, args);
	va_end (args);
}

void CLogFile::LogToFile(const char* pszLogFile, const char* msg, ...)
{
	if( msg == NULL || pszLogFile == NULL )
	{
		return;
	}

	SetCurrentTime();
	va_list args;
	va_start(args, msg);
	WriteToLogFile(pszLogFile, msg, args);
	va_end (args);
}

void CLogFile::LogToFileByDay(const char* pszLogFile, const char* msg, ...)
{
	if( msg == NULL || pszLogFile == NULL )
	{
		return;
	}

	SetCurrentTime();
	int nRet = snprintf(
		m_szLogToFileName, sizeof(m_szLogToFileName) - 1, "%04u-%02u-%02u/%s",
		m_curTime.ulYear, m_curTime.ulMonth, m_curTime.ulDay, pszLogFile
	);
	if ( nRet <= 0 )
	{
		return;
	}
	m_szLogToFileName[nRet] = '\0';

	va_list args;
	va_start(args, msg);
	WriteToLogFile(m_szLogToFileName, msg, args);
	va_end (args);
}

void CLogFile::TraceLog(unsigned int pszName, const char* msg, ...)
{
	if( m_nTraceNum <= 0 )
	{
		return;
	}
	int i;
	for( i = 0; i < m_nTraceNum; i++ )
	{
		if( pszName = m_szTraceNames[i] )
		{
			break;
		}
	}
	if( i == m_nTraceNum )
	{
		return;
	}

	SetCurrentTime();
	va_list args;
	va_start(args, msg);
	char szLogFile[MAX_PATH];
	memset(szLogFile, 0, sizeof(szLogFile));
	snprintf(szLogFile, sizeof(szLogFile) - 1,
		"player/trace_%02d_%04u_%02u_%02u.log", i + 1, m_curTime.ulYear, m_curTime.ulMonth, m_curTime.ulDay);
	WriteToLogFile(szLogFile, msg, args);
	va_end (args);
}

void CLogFile::ThreadLog(int nThreadIndex, int nLogPriorty, const char* msg, ...)
{
	if( msg == NULL )
	{
		return;
	}
	SetCurrentTime();
	char szFileName[MAX_PATH] = {0};
	snprintf(szFileName, sizeof(szFileName) - 1, "thread_%d.log", nThreadIndex);
	switch(nLogPriorty)
	{
	case KGLOG_DEBUG:
		if( m_iThreadDbgLogFlag == 0 )
		{
			return;
		}
	case KGLOG_WARNING:
		if( m_iThreadWarnLogFlag == 0 )
		{
			return;
		}
	case KGLOG_INFO:
		if( m_iThreadInfoLogFlag == 0 )
		{
			return;
		}
	default:
		break;
	}
	va_list args;
	va_start(args, msg);
	WriteToLogFile(szFileName, msg, args);
	va_end (args);
}

void CLogFile::ThreadLogToFile(const char* pszLogFile, int nLogPriorty, const char* msg, ...)
{
	if( msg == NULL || pszLogFile == NULL )
	{
		return;
	}
	SetCurrentTime();

	switch(nLogPriorty)
	{
	case KGLOG_DEBUG:
		if( m_iThreadDbgLogFlag == 0 )
		{
			return;
		}
	case KGLOG_WARNING:
		if( m_iThreadWarnLogFlag == 0 )
		{
			return;
		}
	case KGLOG_INFO:
		if( m_iThreadInfoLogFlag == 0 )
		{
			return;
		}
	default:
		break;
	}

	char szFileName[MAX_PATH];
	memset(szFileName, 0, sizeof(szFileName));
	snprintf(
		szFileName, sizeof(szFileName) - 1, "%04u-%02u-%02u/%s",
		m_curTime.ulYear, m_curTime.ulMonth, m_curTime.ulDay, pszLogFile
	);

	va_list args;
	va_start(args, msg);
	WriteToLogFile(szFileName, msg, args);
	va_end (args);
}

void CLogFile::SetBakLogPath(const char *pBakLogPath)
{
	if (!pBakLogPath)
	{
		return;
	}
	memset(m_szBakLogPath, 0, sizeof(m_szBakLogPath));
	strncpy(m_szBakLogPath, pBakLogPath, sizeof(m_szBakLogPath) - 2);
	int nLen = strlen(m_szBakLogPath);
	for( int i = 0; i < nLen; i++ )
	{
		if( m_szBakLogPath[i] == '\\' )
		{
			m_szBakLogPath[i] = '/';
		}
	}
	if( m_szBakLogPath[nLen - 1] != '/' )
	{
		m_szBakLogPath[nLen] = '/';
	}
}




// add by: xushvai@gmail.com
////////////////////////////////////////////////////////////////////////////////
Logger::Logger()
	: warn_log_switch_(0), error_log_switch_(0), normal_log_switch_(0)
{
	memset(&cur_time_, 0, sizeof(cur_time_));
	memset(warn_log_name_, 0, sizeof(warn_log_name_));
	memset(default_output_path_, 0, sizeof(default_output_path_));
}

Logger::~Logger()
{

}

void Logger::Lock()
{
}

void Logger::Unlock()
{

}

void Logger::SetLogPath(const char* path)
{
	strncpy(default_output_path_,
		path, sizeof(default_output_path_) - 1);
}

void Logger::SetWarnLogSwitch(int8 on_off)
{
	warn_log_switch_ = on_off;
}

void Logger::SetErrorLogSwitch(int8 on_off)
{
	error_log_switch_ = on_off;
}

void Logger::SetNormalLogSwitch(int8 on_off)
{
	normal_log_switch_ = on_off;
}

void Logger::SetShowMillisecondSwitch(int8 on_off)
{
	show_millisecond_switch_ = on_off;
}

void Logger::SetNormalLogName(const char* name)
{
	strncpy(normal_log_name_,
		name, sizeof(normal_log_name_) - 1);
}

void Logger::WriteContent(FILE* file_ptr, const char* format, va_list& args, char* append_string)
{
	if (show_millisecond_switch_) { // show millisecond.
		fprintf(file_ptr, "[%.4u-%.2u-%.2u_%.2u:%.2u:%.2u:%.3u] ",
			cur_time_.year,
			cur_time_.month,
			cur_time_.day,
			cur_time_.hour,
			cur_time_.minute,
			cur_time_.second,
			cur_time_.millisecond);
	} else {                        // do not show millisecond.
		fprintf(file_ptr, "[%.4u-%.2u-%.2u_%.2u:%.2u:%.2u] ",
			cur_time_.year,
			cur_time_.month,
			cur_time_.day,
			cur_time_.hour,
			cur_time_.minute,
			cur_time_.second);
	}
	if (append_string) {
		fprintf(file_ptr, "%s ", append_string);
	}
	vfprintf(file_ptr, format, args);
}

void Logger::WriteNormalLog(const char* format, va_list& args, char* append_string)
{
	if (!normal_log_switch_) {
		return;
	}
	WriteToLogFile(normal_log_name_, format, args, append_string);
}

void Logger::WriteWarnLog(const char* format, ...)
{
	if (!warn_log_switch_) {
		return;
	}
	//SetCurrentTime();

	va_list variable_argument_list;
	va_start(variable_argument_list, format);
	WriteNormalLog(format, variable_argument_list, (char*)"#warn#");
	va_end(variable_argument_list);

	va_start(variable_argument_list, format);
	WriteToLogFile(warn_log_name_, format, variable_argument_list);
	va_end(variable_argument_list);
}

void Logger::WriteToLogFile(const char* file_name,
	const char* format, va_list& variable_argument_list, char* append_string)
{
	//write之前先考虑备份，备份机制统一考虑。
	Lock();
	char log_file[MAX_PATH];
	sprintf(log_file, "%s/%s", default_output_path_, file_name);

	//BakLogFile(pFileName);

	FILE* file_ptr = fopen(log_file, "a+");
	if (!file_ptr) {
		// open file failed
		// 1. check if the path is error.
		// 2. path ok, create the path, then create the file.
		char temp_log_file[MAX_PATH];
		strncpy(temp_log_file, log_file, sizeof(temp_log_file) - 1);
		char* last_match_character_ptr = strrchr(temp_log_file, '/');
		if (!last_match_character_ptr) { // path format error.
			Unlock();
			return;
		}
		*last_match_character_ptr = 0; // just save path.
		if (access(temp_log_file, F_OK)) { // return is not 0, path is not exist.
			CreatePath(temp_log_file);
		}
		file_ptr = fopen(log_file, "a+");
	}
	if (file_ptr) {
		WriteContent(file_ptr, format, variable_argument_list, append_string);
		fclose(file_ptr);
		file_ptr = NULL;
	}
	Unlock();
}

////////////////////////////////////////////////////////////////////////////////
LOGFILE_NAMESPACE_END
