#ifndef __BASE_H__
#define __BASE_H__

enum ConnectionEntityType {
	CET_UNKNOWN   = 0,
	CET_PROXY     = 1,
	CET_MAINSVRD  = 2,
	CET_DBSVRD    = 3,
	CET_OTHERSVRD = 4 
};

struct ConnectServerInfo {
	int entity_type;
	int server_id;
};

#endif
