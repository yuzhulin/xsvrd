#ifndef _MSG_H_
#define _MSG_H_

#include "msgbase.h"
#include "msgstruct.h"

class CRequestLoginPara: public CMsgPara
{
public:
	CRequestLoginPara();
	~CRequestLoginPara(){};
	DECLARE_MSGFUNC;
	tagCSAccLoginC m_stData;
};	

class CResponseLoginPara: public CMsgPara
{
public:
	CResponseLoginPara();
	~CResponseLoginPara(){};
	DECLARE_MSGFUNC;
	tagCSAccLoginS m_stData;
};	

class CRequestLogoutPara: public CMsgPara
{
public:
	CRequestLogoutPara();
	~CRequestLogoutPara(){};
	DECLARE_MSGFUNC;
	STRequestLogoutPara m_stData;
};	

class CResponseLogoutPara: public CMsgPara
{
public:
	CResponseLogoutPara();
	~CResponseLogoutPara(){};
	DECLARE_MSGFUNC;
	STResponseLogoutPara m_stData;
};	

#endif

