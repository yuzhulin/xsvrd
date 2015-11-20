#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "handlerthread.h"

namespace xsvrd {

class Controller {
public:
	Controller();
	virtual~Controller();

	int Run();
	int PrepareToRun();

private:
	HandlerThread* mainsvrd_handler_thread_;
	HandlerThread* dbsvrd_handler_thread_;
	HandlerThread* othersvrd_handler_thread_;
};

}

#endif
