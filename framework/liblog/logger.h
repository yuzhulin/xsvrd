#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "logger_interface.h"

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

	virtual void SetMAXLogFileNum(uint32 num);

	virtual void SetMAXLogFileSize(uint32 size);

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

	void BakupLogFile(const char* file_name);

	void BackupFile(const char* src_file, const char* dst_file);

	void GetBackupFileName(const char* file_name, char backup_file_name[MAX_PATH]);

private:
	int8 lock_log_switch_;                // 1:on 0:off
	int8 bin_log_switch_;                 // 1:on 0:off
	int8 info_log_switch_;	              // 1:on 0:off
	int8 warn_log_switch_;                // 1:on 0:off
	int8 error_log_switch_;               // 1:on 0:off
	int8 debug_log_switch_;               // 1:on 0:off
	int8 show_millisecond_switch_;        // 1:on 0:off

	uint32 max_log_file_size_;
	uint32 max_log_file_num_;

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

#endif