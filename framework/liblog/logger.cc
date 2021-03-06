#include "win_linux.h"
#include "logger.h"

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
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
	DLL_EXPORT void* CreateObj()
	{
		return new Logger;
	}
};

Logger::Logger()
	: info_log_switch_(SWITCH_ON), 
	warn_log_switch_(SWITCH_ON),
	error_log_switch_(SWITCH_ON),
	debug_log_switch_(SWITCH_ON),
	bin_log_switch_(SWITCH_ON),
	lock_log_switch_(SWITCH_ON),
	max_log_file_size_(0),
	max_log_file_num_(0)
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
	max_log_file_num_ = 10;
	max_log_file_size_ = 16 * 1024 * 1024;

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

void Logger::SetMAXLogFileSize(uint32 size)
{
	if (0 == size) {
		return;
	}
	max_log_file_size_ = size;
}

void Logger::SetMAXLogFileNum(uint32 num)
{
	if (0 == num) {
		return;
	}
	max_log_file_num_ = num;
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
	Lock();
	char log_file[MAX_PATH];
	sprintf(log_file, "%s/%s", log_path_, file_name);
	// write after backup.
	BakupLogFile(file_name);
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

void Logger::BakupLogFile(const char* file_name)
{
	char log_file[MAX_PATH];
	sprintf(log_file, "%s/%s", log_path_, file_name);

	std::clog << log_file << std::endl;

	time_t cur_time = time(NULL);
	uint32 file_size = 0;
	struct stat buf;
	if (stat(log_file, &buf) != 0) {
		return;
	}
	if (buf.st_size < (int32)(max_log_file_size_)) {
		return;
	}
	char backup_file[MAX_PATH];
	GetBackupFileName(file_name, backup_file);
	BackupFile(log_file, backup_file);
}

void Logger::GetBackupFileName(const char* file_name, char backup_file_name[MAX_PATH])
{
	time_t tmOld = 0xFFFFFFFF;
	memset(backup_file_name, 0, MAX_PATH);
	for (int32 i = 0; i < max_log_file_num_; i++) {
		time_t tmTemp = 0;
		char szTempName[MAX_PATH];
		memset(szTempName, 0, sizeof(szTempName));
		snprintf(
			szTempName, sizeof(szTempName) - 1, 
			"%s/%s_%d.bak", 
			backup_log_path_, file_name, i + 1
		);
		struct stat buf;
		if (stat(szTempName, &buf) != 0) {
			tmTemp = 0;
			strncpy(backup_file_name, szTempName, MAX_PATH - 1);
			return;
		} else {
			tmTemp = buf.st_mtime;
		}
		if (tmTemp < tmOld) {
			tmOld = tmTemp;
			strncpy(backup_file_name, szTempName, MAX_PATH - 1);
		}
	}
}

void Logger::BackupFile(const char* src_file, const char* dst_file)
{
	if (access(src_file, 0) != -1 ) {
		char backup_log_path[MAX_PATH] = {0};
		strncpy(backup_log_path, dst_file, MAX_PATH - 1);
		char* end_ptr = strrchr(backup_log_path, '/');
		if (end_ptr) {
			*end_ptr = 0;
		}
		if (access(backup_log_path, 0) != 0) {
			CreatePath(backup_log_path);
		}
		unlink(dst_file);
		// if dst and src specify different paths, the file
		// is moved to the dst location.
		rename(src_file, dst_file);
		unlink(src_file);
	}
}
