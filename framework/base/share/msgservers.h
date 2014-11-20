#ifndef _MSGSERVERS_H_
#define _MSGSERVERS_H_

#include "msgbase.h"

class CInterCmdPara : public CMsgPara
{
public:
	CInterCmdPara();
	~CInterCmdPara();

	DECLARE_MSGFUNC;
	int  m_iHandleID;
	int  m_iCmdID;
	char m_szCmdBuffer[100];
};

class CRequestPlayerSecretPara : public CMsgPara
{
public:
	CRequestPlayerSecretPara();
	~CRequestPlayerSecretPara();

	DECLARE_MSGFUNC;
	char m_szAccountID[ACCOUNT_LEN];                
	unsigned int m_unTime;          
	
	int ibufflen ;
	char buffer[45 * 1024];

};

class CResponsePlayerSecretPara : public CMsgPara
{
public:
	CResponsePlayerSecretPara();
	~CResponsePlayerSecretPara();

	DECLARE_MSGFUNC;
	short m_nResultID;
	char m_szAccountID[ACCOUNT_LEN];
	char m_szPassword[MAX_PASS_LEN];
};

#define MAXTIMERPARALENGTH 32
class CInterTimeOutPara : public CMsgPara
{
public:
	CInterTimeOutPara();
	~CInterTimeOutPara();

	DECLARE_MSGFUNC;
	short m_nTimerMark;
	short m_nOtherInfoLength;
	union _UTimer
	{
		int m_aiOtherInfo[MAXTIMERPARALENGTH/sizeof(int)];
		short m_anOtherInfo[MAXTIMERPARALENGTH/sizeof(short)];
		byte m_abyOtherInfo[MAXTIMERPARALENGTH];
	} m_stOtherInfo;
};


#endif

