#include "handlerthread.h"

namespace xsvrd {

HandlerThread::HandlerThread()
{
	read_connection_ = NULL;
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
	connection_entity_type_ = entity_type;	
	dbsvrd_connection_ = dbsvrd_connection;
	mainsvrd_connection_ = mainsvrd_connection;
	othersvrd_connection_ = othersvrd_connection;
	switch(entity_type) {
		case CET_DBSVRD: {
			read_connection_ = dbsvrd_connection_;
			break;
		}
		case CET_MAINSVRD: {
			read_connection_ = mainsvrd_connection_;
			break;
		}
		case CET_OTHERSVRD: {
			read_connection_ = othersvrd_connection_;
			break;
		}
		default:
			return -1;
	}
	return 0;
}

};

}
