// define the interface for Logger class.
//
// Author: xushuai
// E-Mail: xushvai@gmail.com

#ifndef __LOGGER_INTERFACE_H__
#define __LOGGER_INTERFACE_H__

#include "os_refactor.h"

class LoggerInterface {
public:
	LoggerInterface(){};
	virtual~LoggerInterface(){};

	virtual void SetLogPath(const char* log_file_path) = 0;

	virtual	void WriteDebugLog(const char* content, ...) = 0;
};

#endif
