#include "handler.h"
#include "common.h"

namespace xsvrd {

Handler::Handler()
{
	configuration_ = NULL;
	read_connection_ = NULL;
	dbsvrd_connection_ = NULL;
	mainsvrd_connection_ = NULL;
	othersvrd_connection_ = NULL;
	connection_entity_type_ = CET_UNKNOWN;
}

Handler::~Handler()
{
}

int Handler::PrepareToRun()
{
	return 0;
}

int Handler::Routine()
{
	return 0;
}

bool Handler::IsToBeBlocked()
{
	return true;
}

int Handler::Init(ConnectionEntityType entity_type, 
	TCPConnection* dbsvrd_connection, 
	TCPConnection* mainsvrd_connection, 
	TCPConnection* othersvrd_connection, void* para)
{	
	connection_entity_type_ = entity_type;	
	dbsvrd_connection_ = dbsvrd_connection;
	mainsvrd_connection_ = mainsvrd_connection;
	othersvrd_connection_ = othersvrd_connection;
	if (para) {
		configuration_ 
			= static_cast<Configuration*>(para);
	}
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

}
