#ifndef __LOGGER_INTERFACE_H__
#define __LOGGER_INTERFACE_H__

class LoggerInterface {
public:
	LoggerInterface(){};
	virtual~LoggerInterface(){};

	virtual void SetLogPath(const char* log_file_path) = 0;
};

#endif
