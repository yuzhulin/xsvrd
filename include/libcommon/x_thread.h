#ifndef __THREAD_H__
#define __THREAD_H__

#include "../../include/os_refactor.h"

namespace xsvrd {

class Thread {
public:
	Thread();
	virtual ~Thread();

	void Init();
	int Create();
	virtual int Run();
	virtual int PrepareToRun();

protected:
#if defined(__linux__) || defined(TARGET_OS_MAC)
	pthread_t thread_;
	pthread_cond_t cond_;
	pthread_mutex_t mutex_;
	pthread_attr_t attribute_; 
#endif

};

}

#endif
