#ifndef __HANDLERTHREAD_H__
#define __HANDLERTHREAD_H__

#include "../../include/libcommon/x_thread.h"
#include "../../include/libcommon/x_base.h"

namespace xsvrd {

class TCPConnectio;

class HandlerThread : public Thread {
public:
	HandlerThread();
	virtual~HandlerThread();

	virtual int Routine();
	virtual int PrepareToRun();
	virtual bool IsToBeBlocked();

	int init(ConnectionEntityType entity_type, 
		TCPConnection* dbsvrd_connection, 
		TCPConnection* mainsvrd_connection, 
		TCPConnection* othersvrd_connection);

private:
	TCPConnection* dbsvrd_connection_; 
	TCPConnection* mainsvrd_connection_; 
	TCPConnection* othersvrd_connection_;
	ConnectionEntityType conectiont_entity_type_;
};
		
};

}

#endif
