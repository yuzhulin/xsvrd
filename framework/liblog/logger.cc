#include "win_linux.h"
#include "logger.h"

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
	memset(m_szTraceNames, 0, sizeof(m_szTraceNames)); 
	memset(m_szThreadModuleName, 0, sizeof(m_szThreadModuleName));

	m_iThreadDbgLogFlag  = 0;
	m_iThreadWarnLogFlag = 0;
	m_iThreadInfoLogFlag = 0;
	
	m_nTraceNum = 0;

	max_file_size_ = 16 * 1024 * 1024;
	max_file_num_ = 10;
}

CLogFile::~CLogFile()
{
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
	//SetCurrentTime();

	//write之前先考虑备份，备份机制统一考虑。
	//Lock();
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
			//Unlock();
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
		//WriteLog(pFile, msg, args, pAddStr);
		va_end(args);
		fclose(pFile);
	}
	//Unlock();

}

void CLogFile::BinLog(const char* pszFileName, char *pBuffer, unsigned int iLength) 
{
	if(0 == m_iBinLogFlag) /*如果BinLog标志没有打开，则返回*/
	{
		return;
	}
	//SetCurrentTime();

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

	//Lock();

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
			//Unlock();
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
//	Unlock();
}


void CLogFile::LogToFileByDay(const char* pszLogFile, const char* msg, ...)
{
	if( msg == NULL || pszLogFile == NULL )
	{
		return;
	}

	//SetCurrentTime();
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
	//WriteToLogFile(m_szLogToFileName, msg, args);
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

//	SetCurrentTime();
	va_list args;
	va_start(args, msg);
	char szLogFile[MAX_PATH];
	memset(szLogFile, 0, sizeof(szLogFile));
	snprintf(szLogFile, sizeof(szLogFile) - 1,
		"player/trace_%02d_%04u_%02u_%02u.log", i + 1, m_curTime.ulYear, m_curTime.ulMonth, m_curTime.ulDay);
	//WriteToLogFile(szLogFile, msg, args);
	va_end (args);
}

void CLogFile::ThreadLog(int nThreadIndex, int nLogPriorty, const char* msg, ...)
{
	if( msg == NULL )
	{
		return;
	}
	//SetCurrentTime();
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
	//WriteToLogFile(szFileName, msg, args);
	va_end (args);
}

void CLogFile::ThreadLogToFile(const char* pszLogFile, int nLogPriorty, const char* msg, ...)
{
	if( msg == NULL || pszLogFile == NULL )
	{
		return;
	}
	//SetCurrentTime();

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
	//WriteToLogFile(szFileName, msg, args);
	va_end (args);
}

// add by: xushvai@gmail.com
////////////////////////////////////////////////////////////////////////////////
Logger::Logger()
	: info_log_switch_(SWITCH_ON), 
	warn_log_switch_(SWITCH_ON),
	error_log_switch_(SWITCH_ON),
	debug_log_switch_(SWITCH_ON),
	bin_log_switch_(SWITCH_ON),
	lock_log_switch_(SWITCH_ON)
{
	memset(&cur_time_, 0, sizeof(cur_time_));
	memset(&thread_mutex_, 0, sizeof(thread_mutex_));
	memset(info_log_name_, 0, sizeof(info_log_name_));
	memset(warn_log_name_, 0, sizeof(warn_log_name_));
	memset(debug_log_name_, 0, sizeof(debug_log_name_));
	memset(error_log_name_, 0, sizeof(error_log_name_));
	memset(log_path_, 0, sizeof(log_path_));
	memset(backup_log_path_, 0, sizeof(backup_log_path_));
	Init();
}

Logger::~Logger()
{
}

void Logger::Init()
{
	// set default log file name.
	strncpy(info_log_name_,
		DEFAULT_INFO_LOG_FILE_NAME, sizeof(info_log_name_) - 1);
	strncpy(warn_log_name_,
		DEFAULT_WARN_LOG_FILE_NAME, sizeof(warn_log_name_) - 1);
	strncpy(error_log_name_,
		DEFAULT_ERROR_LOG_FILE_NAME, sizeof(error_log_name_) - 1);
	strncpy(debug_log_name_,
		DEFAULT_DEBUG_LOG_FILE_NAME, sizeof(debug_log_name_) - 1);

	// set default log path.
	strncpy(log_path_,
		DEFAULT_LOG_PATH, sizeof(log_path_) - 1);
	strncpy(backup_log_path_,
		DEFAULT_BACKUP_LOG_PATH, sizeof(backup_log_path_) - 1);

	pthread_mutex_init(&thread_mutex_, NULL);
}

void Logger::Lock()
{
	if (!lock_log_switch_) {
		return;
	}
	pthread_mutex_lock(&thread_mutex_);
}

void Logger::Unlock()
{
	if (!lock_log_switch_) {
		pthread_mutex_unlock(&thread_mutex_);
	}
}

void Logger::SetLogPath(const char* path)
{
	if (path) {
		strncpy(log_path_,
			path, sizeof(log_path_) - 1);
	} else {
		strncpy(log_path_,
			DEFAULT_LOG_PATH,
			sizeof(log_path_) - 1);
	}
	uint32 path_str_len = strlen(log_path_);
	for (int32 i = 0; i < path_str_len; i++) {
		if ('\\' == log_path_[i]) {
			log_path_[i] = '/';
		}
	}
	if ('/' == log_path_[path_str_len - 1]) {
		log_path_[path_str_len - 1] = 0;
	}
}

void Logger::SetBackupLogPath(const char* path)
{
	if (path) {
		strncpy(backup_log_path_,
			path, sizeof(backup_log_path_) - 1);
	} else {
		strncpy(backup_log_path_,
			DEFAULT_LOG_PATH,
			sizeof(backup_log_path_) - 1);
	}
	uint32 path_str_len = strlen(backup_log_path_);
	for (int32 i = 0; i < path_str_len; i++) {
		if ('\\' == backup_log_path_[i]) {
			backup_log_path_[i] = '/';
		}
	}
	if ('/' == backup_log_path_[path_str_len - 1]) {
		backup_log_path_[path_str_len - 1] = 0;
	}
}

void Logger::SetLockSwitch(int8 on_off)
{
	lock_log_switch_ = on_off;
}

void Logger::SetBinLogSwitch(int8 on_off)
{
	bin_log_switch_ = on_off;
}

void Logger::SetInfoLogSwitch(int8 on_off)
{
	info_log_switch_ = on_off;
}

void Logger::SetWarnLogSwitch(int8 on_off)
{
	warn_log_switch_ = on_off;
}

void Logger::SetErrorLogSwitch(int8 on_off)
{
	error_log_switch_ = on_off;
}

void Logger::SetDebugLogSwitch(int8 on_off)
{
	debug_log_switch_ = on_off;
}

void Logger::SetShowMillisecondSwitch(int8 on_off)
{
	show_millisecond_switch_ = on_off;
}

void Logger::SetBinLogName(const char* name)
{
	/*if (name) {
		strncpy(bin_log_name_,
			name, sizeof(bin_log_name_) - 1);
	} else {
		strncpy(bin_log_name_,
			DEFAULT_BIN_LOG_FILE_NAME,
			sizeof(bin_log_name_) - 1);
	}*/
}

void Logger::SetInfoLogName(const char* name)
{
	if (name) {
		strncpy(info_log_name_,
			name, sizeof(info_log_name_) - 1);
	} else {
		strncpy(info_log_name_,
			DEFAULT_INFO_LOG_FILE_NAME,
				sizeof(info_log_name_) - 1);
	}
}

void Logger::SetWarnLogName(const char* name)
{
	if (name) {
		strncpy(warn_log_name_,
			name, sizeof(warn_log_name_) - 1);
	} else {
		strncpy(warn_log_name_,
			DEFAULT_WARN_LOG_FILE_NAME,
				sizeof(warn_log_name_) - 1);
	}
}

void Logger::SetErrorLogName(const char* name)
{
	if (name) {
		strncpy(error_log_name_,
			name, sizeof(error_log_name_) - 1);
	} else {
		strncpy(error_log_name_,
			DEFAULT_ERROR_LOG_FILE_NAME,
				sizeof(error_log_name_) - 1);
	}
}

void Logger::SetDebugLogName(const char* name)
{
	if (name) {
		strncpy(debug_log_name_,
			name, sizeof(debug_log_name_) - 1);
	} else {
		strncpy(debug_log_name_,
			DEFAULT_DEBUG_LOG_FILE_NAME,
				sizeof(debug_log_name_) - 1);
	}
}

void Logger::SetCurTime()
{
	struct tm* tm_ptr = NULL;
	if (show_millisecond_switch_) {
		struct timeval timeval_now;
#ifndef _WIN32
		gettimeofday(&timeval_now, NULL);
#else
		SYSTEMTIME system_time;
		GetLocalTime(&system_time);
		timeval_now.tv_sec  = (long)time(NULL);
		timeval_now.tv_usec = system_time.wMilliseconds * 1000;
#endif
		time_t time_t_now = timeval_now.tv_sec;
		cur_time_.millisecond = timeval_now.tv_usec / 1000;
		tm_ptr = localtime(&time_t_now);
	} else {
		time_t time_t_now = time(NULL);
		tm_ptr = localtime(&time_t_now);
	}
	cur_time_.year   = tm_ptr->tm_year + 1900;
	cur_time_.month  = tm_ptr->tm_mon + 1;
	cur_time_.day    = tm_ptr->tm_mday;
	cur_time_.hour   = tm_ptr->tm_hour;
	cur_time_.minute = tm_ptr->tm_min;
	cur_time_.second = tm_ptr->tm_sec;
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

void Logger::WriteInfoLog(const char* format, ...)
{
	if (!info_log_switch_) {
		return;
	}
	SetCurTime();
	va_list variable_argument_list;
	if (debug_log_name_) {
		va_start(variable_argument_list, format);
		WriteToLogFile(debug_log_name_,
			format, variable_argument_list, (char*)"#info#");
		va_end(variable_argument_list);
	}
	va_start(variable_argument_list, format);
	WriteToLogFile(info_log_name_, format, variable_argument_list);
	va_end(variable_argument_list);
}

void Logger::WriteDebugLog(const char* format, ...)
{
	if (!debug_log_switch_) {
		return;
	}
	SetCurTime();
	va_list variable_argument_list;
	va_start(variable_argument_list, format);
	WriteToLogFile(debug_log_name_, format, variable_argument_list);
	va_end(variable_argument_list);
}

void Logger::WriteWarnLog(const char* format, ...)
{
	if (!warn_log_switch_) {
		return;
	}
	SetCurTime();
	va_list variable_argument_list;
	if (debug_log_name_) {
		va_start(variable_argument_list, format);
		WriteToLogFile(debug_log_name_,
			format, variable_argument_list, (char*)"#warn#");
		va_end(variable_argument_list);
	}
	va_start(variable_argument_list, format);
	WriteToLogFile(warn_log_name_, format, variable_argument_list);
	va_end(variable_argument_list);
}

void Logger::WriteErrorLog(const char* format, ...)
{
	if (!error_log_switch_) {
		return;
	}
	SetCurTime();
	va_list variable_argument_list;
	if (debug_log_name_) {
		va_start(variable_argument_list, format);
		WriteToLogFile(debug_log_name_,
			format, variable_argument_list, (char*)"#error#");
		va_end(variable_argument_list);
	}
	va_start(variable_argument_list, format);
	WriteToLogFile(error_log_name_, format, variable_argument_list);
	va_end(variable_argument_list);
}

void Logger::WriteBinLog(char* buffer, uint32 len)
{
	if (!bin_log_switch_) {
		return;
	}
	if (!buffer) {
		return;
	}
	if (len > MAX_BIN_LOG_LEN) {
		len = MAX_BIN_LOG_LEN;
	}
	SetCurTime();
	char log_file[MAX_PATH];
	sprintf(log_file, "%s/%s", log_path_, debug_log_name_);
	Lock();
	FILE* file_ptr = fopen(log_file, "a+");
	if (!file_ptr) {
		CreatePath(log_path_);
		file_ptr = fopen(log_file, "a+");
		if (!file_ptr) {
			Unlock();
			return;
		}
	}
	char temp_buffer[2 * MAX_BIN_LOG_LEN + MAX_BIN_LOG_LEN] = {0};
	char add_str[64] = {0};
	for (int32 i = 0; i < len; i++) {
		if (!(i % BIN_LOG_BYTE_PER_LINE)) {
			sprintf(add_str, "\n%04d>    ", i / BIN_LOG_BYTE_PER_LINE + 1);
			strcat(temp_buffer, add_str);
		}
		sprintf(add_str, "%02X ", (unsigned char)buffer[i]);
		strcat(temp_buffer, add_str);
	}
	strcat(temp_buffer, "\n");
	fprintf(file_ptr, "\n---------------- [time %02u:%02u:%02u] len %u ----------------", 
		cur_time_.hour, cur_time_.minute, cur_time_.second, len);
	fprintf(file_ptr, temp_buffer);
	fclose(file_ptr);
	Unlock();
}

void Logger::WriteLogToFile(const char* file_name, const char* format, ...)
{
	SetCurTime();
	va_list variable_argument_list;
	va_start(variable_argument_list, format);
	WriteToLogFile(file_name, format, variable_argument_list);
	va_end(variable_argument_list);
}

void Logger::WriteToLogFile(const char* file_name,
	const char* format, va_list& variable_argument_list, char* append_string)
{
	//write之前先考虑备份，备份机制统一考虑。
	Lock();
	char log_file[MAX_PATH];
	sprintf(log_file, "%s/%s", log_path_, file_name);

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