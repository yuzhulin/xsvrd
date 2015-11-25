#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include <string>

namespace xsvrd {

class TCPConnection {
public:
	TCPConnection();
	virtual ~TCPConnection();
	int Init(ConnectionEntityType entity_type, 
		int entity_id, std::string addr, unsigned short port);

private:
	unsigned short port_;
	std::string addr_;
	int entity_id_;
	ConnectionEntityType entity_type_;
};

}

#endif
