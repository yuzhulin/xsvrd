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

// wrap the logger interface.
/////////////////////////////////////////////////////////////////////////
#define SET_INTERFACE_INSTANCE_PTR(INSTANCE_PTR) \
	do {g_interface_instance_ptr = (INSTANCE_PTR);} while (0)

#define WRITE_DEBUG_LOG(...) \
	do {g_interface_instance_ptr->WriteDebugLog(__VA_ARGS__);} while(0)

#define WRITE_INFO_LOG(...) \
	do {g_interface_instance_ptr->WriteInfoLog(__VA_ARGS__);} while(0)

#define WRITE_WARN_LOG(...) \
	do {g_interface_instance_ptr->WriteWarnLog(__VA_ARGS__);} while(0)

#define WRITE_ERROR_LOG(...) \
	do {g_interface_instance_ptr->WriteErrorLog(__VA_ARGS__);} while(0)

#define WRITE_LOG_TO_FILE(...) \
	do {g_interface_instance_ptr->WriteLogToFile(__VA_ARGS__);} while(0)

// Log contains 4 types: debug, warn, error, info
class LoggerInterface {
public:
	LoggerInterface(){};
	virtual~LoggerInterface(){};

	virtual void Init() = 0;

	virtual void SetBinLogSwitch(int8 on_off) = 0;

	virtual void SetInfoLogSwitch(int8 on_off) = 0;

	virtual void SetWarnLogSwitch(int8 on_off) = 0;

	virtual void SetErrorLogSwitch(int8 on_off) = 0;

	virtual void SetDebugLogSwitch(int8 on_off) = 0;

	virtual void SetShowMillisecondSwitch(int8 on_off) = 0;

	virtual void SetBinLogName(const char* name) = 0;

	virtual void SetInfoLogName(const char* name) = 0;

	virtual void SetWarnLogName(const char* name) = 0;

	virtual void SetErrorLogName(const char* name) = 0;

	virtual void SetDebugLogName(const char* name) = 0;

	virtual void SetLogPath(const char* log_file_path) = 0;

	virtual void WriteContent(FILE* file_ptr, const char* msg, va_list& args, char* append_string) = 0;

	virtual void WriteInfoLog(const char* format, ...) = 0;

	virtual void WriteDebugLog(const char* format, ...) = 0;

	virtual	void WriteWarnLog(const char* format, ...) = 0;

	virtual void WriteLogToFile(const char* file_name, const char* format, ...) = 0;

	// Write log to error log file, the error means 
	// fatal problems or serious bugs.
	// If the error occur, the program must be closed so
	// as not to cause a greater impact.
	//
	// Parameters
	//   format - format string.
	//   ...    - arguments list.
	// Return
	//   void - nothing.
	virtual void WriteErrorLog(const char* format, ...) = 0;

	virtual void WriteToLogFile(const char* file_name, const char* content,
		va_list& variable_argument_list, char* append_string = NULL) = 0;
};

LoggerInterface* g_interface_instance_ptr = NULL;

#endif
