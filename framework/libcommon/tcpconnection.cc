#include "../../include/libcommon/tcpconnection.h"

namespace xsvrd {

TCPConnection::TCPConnection()
{
	port_ = 0;
	addr_.clear();
	entity_id_ = 0;
	entity_type_ = CET_UNKNOWN;
}

TCPConnection::~TCPConnection()
{
}

int TCPConnection::Init(ConnectionEntityType entity_type, 
	int entity_id, std::string addr, unsigned short port)
{
	addr_ = addr;
	port_ = port;
	entity_id_ = entity_id;
	entity_type_ = entity_type;
	return 0;
}

}
