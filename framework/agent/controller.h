#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "handlerthread.h"
#include "../../include/libcommon/tcpconnection.h"

namespace xsvrd {

class Controller {
public:
	Controller();
	virtual~Controller();

	int Run();
	int PrepareToRun();

private:
	HandlerThread* dbsvrd_handler_thread_;
	HandlerThread* mainsvrd_handler_thread_;
	HandlerThread* othersvrd_handler_thread_;
	TCPConnection* dbsvrd_connection_; 
	TCPConnection* mainsvrd_connection_; 
	TCPConnection* othersvrd_connection_;
};

}

#endif
