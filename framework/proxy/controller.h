#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "handlerthread.h"
#include "common.h"
#include "../../include/libcommon/tcpconnection.h"
#include "../../include/libcommon/x_tcpsocket.h"
#include <string>

namespace xsvrd {

class Controller {
public:
	Controller();
	virtual ~Controller();

	int Run();
	int PrepareToRun();
	int ReadConfiguration(std::string config_file);
	int Init(std::string config_file);
	int CheckConnectRequest();
	int ReadEntityList(ConnectionEntityType type, TCPConnection* connection);

private:
	TCPSocket listen_socket_;
	Configuration configuration_;
	HandlerThread* dbsvrd_handler_thread_;
	HandlerThread* mainsvrd_handler_thread_;
	HandlerThread* othersvrd_handler_thread_;
	TCPConnection dbsvrd_connection_[MAX_SVRD_NUM + 1]; 
	TCPConnection mainsvrd_connection_[MAX_SVRD_NUM + 1]; 
	TCPConnection othersvrd_connection_[MAX_SVRD_NUM + 1];
};

}

#endif
