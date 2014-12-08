// define the interface for Logger class.
//
// Author: xushuai
// E-Mail: xushvai@gmail.com

#ifndef __LOGGER_INTERFACE_H__
#define __LOGGER_INTERFACE_H__

#include "os_refactor.h"

// Log contains 3 types: normal, warn, error
class LoggerInterface {
public:
	LoggerInterface(){};
	virtual~LoggerInterface(){};

	virtual void SetWarnLogSwitch(int8 on_off) = 0;

	virtual void SetErrorLogSwitch(int8 on_off) = 0;

	virtual void SetNormalLogSwitch(int8 on_off) = 0;

	virtual void SetNormalLogName(const char* name) = 0;

	virtual void SetLogPath(const char* log_file_path) = 0;

	virtual void Lock() = 0;

	virtual void Unlock() = 0;

	virtual void WriteContent(FILE* file_ptr, const char* msg, va_list& args, char* append_string) = 0;

	virtual	void WriteWarnLog(const char* content, ...) = 0;

	virtual void WriteNormalLog(const char* content, va_list& args, char* append_string) = 0;

	virtual void WriteToLogFile(const char* file_name, const char* content,
		va_list& variable_argument_list, char* append_string = NULL) = 0;
};

#endif
