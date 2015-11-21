#include "handlerthread.h"

namespace xsvrd {

HandlerThread::HandlerThread()
{
	dbsvrd_connection_ = NULL;
	mainsvrd_connection_ = NULL;
	othersvrd_connection_ = NULL;
}

HandlerThread::~HandlerThread()
{
}

int HandlerThread::PrepareToRun()
{
	return 0;
}

int HandlerThread::Routine()
{
	return 0;
}

bool HandlerThread::IsToBeBlocked()
{
	return true;
}

int init(ConnectionEntityType entity_type, 
	TCPConnection* dbsvrd_connection, 
	TCPConnection* mainsvrd_connection, 
	TCPConnection* othersvrd_connection)
{	
	dbsvrd_connection_ = dbsvrd_connection;
	mainsvrd_connection_ = mainsvrd_connection;
	othersvrd_connection_ = othersvrd_connection;
	return 0;
}

};

}
