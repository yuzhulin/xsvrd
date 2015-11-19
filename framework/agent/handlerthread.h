#ifndef __HANDLERTHREAD_H__
#define __HANDLERTHREAD_H__

#include "../../include/libcommon/x_thread.h"

namespace xsvrd {

class HandlerThread : public Thread {
public:
	HandlerThread();
	virtual~HandlerThread();

	virtual int Routine();
	virtual int PrepareToRun();
	virtual bool IsToBeBlocked();
};

}

#endif
