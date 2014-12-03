#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#include "../../include/interface/ilogger.h"

#include "logger_interface.h"

LOGFILE_NAMESPACE_BEGIN

#ifdef WIN32
#define TRUE 1
#define FALSE 0
#else
#define TRUE 1
#define FALSE 0
#endif


#define MAX_BIN_LEN             (32 * 1024)


class CLogFile : public ILogFile
{
public:
	CLogFile();
	virtual ~CLogFile();

	virtual void SetLogPath(const char *pLogPath);
	virtual void SetLogName(const char* pLogName);
	virtual void SetMaxLogFile(unsigned int nMaxSize, unsigned int nMaxFileNum);
	virtual void SetDbgLogFlag(int iLogFlag);
	virtual void SetWarnLogFlag(int iErrLogFlag);
	virtual void SetInfoLogFlag(int iInfoLogFlag);
	virtual void SetBinLogFlag(int iBinLogFlag);
	virtual void SetLockFlag(int bLogFlag);
	virtual void SetShowTimeFormat(int ishowms);

	virtual void WriteLogFile(int nPriority, const char* msg, ...);
	virtual	void DbgLog(const char* msg, ...);        /*��¼��ͨ��־*/
	virtual	void WarnLog(const char* msg, ...);     /*��¼������־*/
	virtual	void ErrLog(const char* msg, ...);   /*��¼������־*/
	virtual void BinLog(const char* pszFileName, char *pBuffer,  unsigned int iLength);
	virtual	void InfoLog(const char* msg, ...); /*ָ���ļ�����ӡ��־*/
	virtual void DbgBinLog(char *pBuffer,  unsigned int iLength);        /*��¼binlog����ͨ��־*/
	virtual	void LogToFile(const char* pszLogFile, const char* msg, ...);
	virtual	void LogToFileByDay(const char* pszLogFile, const char* msg, ...);
	virtual	void TraceLog(unsigned int pszName, const char* msg, ...);

	virtual void SetThreadModuleName(char szThreadModuleName[MAX_PATH]);
	virtual	void SetThreadDbgLogFlag(int nDbgLog);
	virtual	void SetThreadWarnLogFlag(int nWarnLog);
	virtual	void SetThreadInfoLogFlag(int nInfoLog);
	virtual void SetTraceName(int nUinTraceNum, unsigned int pszNames[MAX_TRACENUM]);

	virtual void ThreadLog(int nThreadIndex, int nPriority, const char* msg, ...);  
	virtual	void ThreadLogToFile(const char* pszLogFile, int nLogPriorty, const char* msg, ...);

	virtual	void SetBakLogPath(const char *pBakLogPath);   /* ������־���ݵ�·������ѡ��Ĭ��Ϊ����־·��/baklogs/ */

private:
	void  Lock();
	void  Unlock();
	void  SetCurrentTime();
	void  WriteNormalLog(const char* msg, va_list& args, char* pStrAdd = NULL);
	void  WriteToLogFile(const char* szFileName, const char* msg, va_list& args, char* pStrAdd = NULL);
	void  WriteLog(FILE* pFile, const char *msg, va_list& args, char* pAddStr = NULL);
	void  BakLogFile(const char* pFileName);
	void  GetBakFileName(const char* pFileName, char szBakFileName[MAX_PATH]);
	void  BackupFile(const char* pSrcFile, const char* pDstFile);
	void  WriteThreadNormalLog(int nThreadIndex, const char* msg, va_list& args, char* pStrAdd = NULL);

private:
	int             m_iThreadDbgLogFlag;              
	int             m_iThreadWarnLogFlag;     
	int             m_iThreadInfoLogFlag;  

	int             m_bIsLockLog;
	pthread_mutex_t m_stMutex;
	TTime           m_curTime;
	int             m_nTraceNum;

	int             m_iShowMs;
	int             m_iDbgLogFlag;              /*��ͨ��־�Ĵ�ӡ��־*/
	int             m_iWarnLogFlag;             /*������־�Ĵ�ӡ��־*/
	int             m_iInfoLogFlag;             /*��Ϣ��־�Ĵ�ӡ��־*/
	int             m_iBinLogFlag;              /*��������־�Ĵ�ӡ��־*/
	char            m_szLogPath[MAX_PATH];        /*��־��·��*/
	char            m_szBakLogPath[MAX_PATH];        /*��־���ݵ�·��*/
	char            m_szThreadModuleName[MAX_PATH];     
	unsigned int    m_szTraceNames[MAX_TRACENUM];
	char            m_szLogName[MAX_PATH];
	char            m_szLogFileName[MAX_PATH];
	char            m_szNormalLogName[MAX_PATH];
	char            m_szWarnLogName[MAX_PATH];
	char            m_szErrLogName[MAX_PATH];
	char            m_szInfoLogName[MAX_PATH];
	char            m_szLogToFileName[MAX_PATH];

	unsigned int max_file_size_;
	unsigned int max_file_num_;
};


// add by: xushvai@gmail.com
////////////////////////////////////////////////////////////////////////////////
class Logger : public LoggerInterface {
public:
	Logger();
	virtual~Logger();


	virtual void SetLogPath(const char* path);
};


////////////////////////////////////////////////////////////////////////////////

LOGFILE_NAMESPACE_END

#endif

