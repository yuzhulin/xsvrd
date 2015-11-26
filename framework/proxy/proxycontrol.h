#ifndef __PROXY_CONTROL_H__
#define __PROXY_CONTROL_H__

#include "handler.h"
#include "common.h"
#include "../../include/libcommon/tcpconnection.h"
#include "../../include/libcommon/x_tcpsocket.h"
#include "../../framework/svrd/control.h"
#include <string>

namespace xsvrd {

class ProxyControl : public Control {
public:
	ProxyControl();
	virtual~ProxyControl();

	virtual int Run();
	virtual int PrepareToRun();
	int ReadConfiguration(std::string config_file);
	int Init(std::string config_file);
	int CheckConnectRequest();
	int ReadEntityList(ConnectionEntityType type, TCPConnection* connection);

private:
	TCPSocket listen_socket_;
	Configuration configuration_;
	Handler* dbsvrd_handler_thread_;
	Handler* mainsvrd_handler_thread_;
	Handler* othersvrd_handler_thread_;
	TCPConnection dbsvrd_connection_[MAX_SVRD_NUM + 1]; 
	TCPConnection mainsvrd_connection_[MAX_SVRD_NUM + 1]; 
	TCPConnection othersvrd_connection_[MAX_SVRD_NUM + 1];
};

}

#endif
