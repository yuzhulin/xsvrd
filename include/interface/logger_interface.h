// define the interface for Logger class.
//
// Author: xushuai
// E-Mail: xushvai@gmail.com

#ifndef __LOGGER_INTERFACE_H__
#define __LOGGER_INTERFACE_H__

#include "os_refactor.h"

struct LogTime {
	uint32 year;           // 0000-9999
	uint32 month;          // 00-12
	uint32 day;            // 01-31
	uint32 hour;           // 00-23
	uint32 minute;         // 00-59
	uint32 second;         // 00-59
	uint32 millisecond;    // 000-999
};

// Log contains 3 types: normal, warn, error
class LoggerInterface {
public:
	LoggerInterface(){};
	virtual~LoggerInterface(){};

	virtual void SetInfoLogSwitch(int8 on_off) = 0;

	virtual void SetWarnLogSwitch(int8 on_off) = 0;

	virtual void SetErrorLogSwitch(int8 on_off) = 0;

	virtual void SetNormalLogSwitch(int8 on_off) = 0;

	virtual void SetShowMillisecondSwitch(int8 on_off) = 0;

	virtual void SetNormalLogName(const char* name) = 0;

	virtual void SetLogPath(const char* log_file_path) = 0;

	virtual void Lock() = 0;

	virtual void Unlock() = 0;

	virtual void WriteContent(FILE* file_ptr, const char* msg, va_list& args, char* append_string) = 0;

	virtual void WriteInfoLog(const char* format, ...) = 0;

	virtual	void WriteWarnLog(const char* content, ...) = 0;

	virtual void WriteNormalLog(const char* content, va_list& args, char* append_string) = 0;

	virtual void WriteToLogFile(const char* file_name, const char* content,
		va_list& variable_argument_list, char* append_string = NULL) = 0;
};

#endif
