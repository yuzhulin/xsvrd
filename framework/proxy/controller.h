#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "handlerthread.h"
#include "../../include/libcommon/tcpconnection.h"
#include "../../include/libcommon/x_tcpsocket.h"
#include <string>

namespace xsvrd {

struct Configuration {
	int proxy_id;
	unsigned short proxy_port;
};

class Controller {
public:
	Controller();
	virtual~Controller();

	int Run();
	int PrepareToRun();
	int ReadConfiguration(std::string config_file);
	int Init(std::string config_file);

private:
	TCPSocket listen_socket_;
	Configuration configuration_;
	HandlerThread* dbsvrd_handler_thread_;
	HandlerThread* mainsvrd_handler_thread_;
	HandlerThread* othersvrd_handler_thread_;
	TCPConnection* dbsvrd_connection_; 
	TCPConnection* mainsvrd_connection_; 
	TCPConnection* othersvrd_connection_;
};

}

#endif
