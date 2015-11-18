#ifndef __THREAD_H__
#define __THREAD_H__

#include "../../include/os_refactor.h"

namespace xsvrd {

enum ThreadStatus {
	TS_INIT, 
	TS_BLOCKED, 
	TS_RUNNING, 
	TS_STOPPED
};

class Thread {
public:
	Thread();
	virtual~Thread();

	void Init();
	int Block();
	int Create();
	void SetStatus(ThreadStatus status) {
		status_ = status;
	}
	virtual int Routine();
	virtual int PrepareToRun();
	virtual bool IsToBeBlocked();

protected:
#if defined(__linux__) || defined(TARGET_OS_MAC)
	pthread_t thread_;
	pthread_cond_t cond_;
	pthread_mutex_t mutex_;
	pthread_attr_t attribute_; 
#endif
	ThreadStatus status_;
};

}

#endif
