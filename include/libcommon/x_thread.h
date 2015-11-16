#ifndef __THREAD_H__
#define __THREAD_H__

#include "../../include/os_refactor.h"

namespace xsvrd {

class Thread {
public:
	Thread();
	virtual ~Thread();

	virtual int Run();
	virtual int PrepareToRun();

	int Create();

};

}

#endif
