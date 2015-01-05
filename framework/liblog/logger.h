#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "../../include/interface/ilogger.h"
#include "logger_interface.h"

//LOGFILE_NAMESPACE_BEGIN

#define MAX_BIN_LEN             (32 * 1024)


class CLogFile : public ILogFile
{
public:
	CLogFile();
	virtual ~CLogFile();


	virtual void SetMaxLogFile(unsigned int nMaxSize, unsigned int nMaxFileNum);


	virtual void WriteLogFile(int nPriority, const char* msg, ...);

	virtual	void LogToFileByDay(const char* pszLogFile, const char* msg, ...);

	virtual	void TraceLog(unsigned int pszName, const char* msg, ...);

	virtual void SetThreadModuleName(char szThreadModuleName[MAX_PATH]);

	virtual	void SetThreadDbgLogFlag(int nDbgLog);

	virtual	void SetThreadWarnLogFlag(int nWarnLog);

	virtual	void SetThreadInfoLogFlag(int nInfoLog);

	virtual void SetTraceName(int nUinTraceNum, unsigned int pszNames[MAX_TRACENUM]);

	virtual void ThreadLog(int nThreadIndex, int nPriority, const char* msg, ...);

	virtual	void ThreadLogToFile(const char* pszLogFile, int nLogPriorty, const char* msg, ...);


private:
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
	int             m_iDbgLogFlag;              /*普通日志的打印标志*/
	int             m_iWarnLogFlag;             /*错误日志的打印标志*/
	int             m_iInfoLogFlag;             /*信息日志的打印标志*/
	int             m_iBinLogFlag;              /*缓冲区日志的打印标志*/
	char            m_szLogPath[MAX_PATH];        /*日志的路径*/
	char            m_szBakLogPath[MAX_PATH];        /*日志备份的路径*/
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

#define DEFAULT_BIN_LOG_FILE_NAME			"bin.log"
#define DEFAULT_INFO_LOG_FILE_NAME			"info.log"
#define DEFAULT_WARN_LOG_FILE_NAME			"warn.log"
#define DEFAULT_DEBUG_LOG_FILE_NAME			"debug.log"
#define DEFAULT_ERROR_LOG_FILE_NAME			"error.log"
#define DEFAULT_LOG_PATH					"../log"
#define DEFAULT_BACKUP_LOG_PATH				"../backup_log"

#define MAX_BIN_LOG_LEN						(32 * 1024)
#define BIN_LOG_BYTE_PER_LINE				16

class Logger : public LoggerInterface {
public:
	Logger();
	virtual~Logger();

	virtual void Init();

	virtual void SetLockSwitch(int8 on_off);

	virtual void SetBinLogSwitch(int8 on_off);

	virtual void SetInfoLogSwitch(int8 on_off);

	virtual void SetWarnLogSwitch(int8 on_off);

	virtual void SetErrorLogSwitch(int8 on_off);

	virtual void SetDebugLogSwitch(int8 on_off);

	virtual void SetShowMillisecondSwitch(int8 on_off);

	virtual void SetBinLogName(const char* name);

	virtual void SetInfoLogName(const char* name);

	virtual void SetWarnLogName(const char* name);

	virtual void SetErrorLogName(const char* name);

	virtual void SetDebugLogName(const char* name);

	virtual void SetLogPath(const char* path);

	virtual void SetBackupLogPath(const char* path);

	virtual void WriteContent(FILE* file_ptr, const char* format, va_list& args, char* append_string);

	virtual void WriteInfoLog(const char* format, ...);

	virtual void WriteDebugLog(const char* format, ...);

	virtual	void WriteWarnLog(const char* format, ...);

	virtual void WriteErrorLog(const char* format, ...);

	virtual void WriteBinLog(char* buffer, uint32 len);

	virtual void WriteLogToFile(const char* file_name, const char* format, ...);

	virtual void WriteToLogFile(const char* file_name, const char* format,
		va_list& variable_argument_list, char* append_string = NULL);

private:
	void SetCurTime();

	void Lock();

	void Unlock();

private:
	int8 lock_log_switch_;                // 1:on 0:off
	int8 bin_log_switch_;                 // 1:on 0:off
	int8 info_log_switch_;	              // 1:on 0:off
	int8 warn_log_switch_;                // 1:on 0:off
	int8 error_log_switch_;               // 1:on 0:off
	int8 debug_log_switch_;               // 1:on 0:off
	int8 show_millisecond_switch_;        // 1:on 0:off

	LogTime cur_time_;
	pthread_mutex_t thread_mutex_;

	char info_log_name_[MAX_NAME];
	char warn_log_name_[MAX_NAME];
	char error_log_name_[MAX_NAME];
	char debug_log_name_[MAX_NAME];
	char normal_log_name_[MAX_NAME];
	char log_path_[MAX_PATH];
	char backup_log_path_[MAX_PATH];
};

//LOGFILE_NAMESPACE_END

#endif