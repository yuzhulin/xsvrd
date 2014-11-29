#ifndef _MSGBASE_H_
#define _MSGBASE_H_


#include "codehelper.h"
#include "../include/common.h"

#define PRINT printf

#define BEGIN_ENCODE\
	if( !pcOutBuffer )\
	{\
		return -1;\
	}\
	BeginEncode(pcOutBuffer, sOutLength);

#define END_ENCODE\
	return EndEncode(); 

#define BEGIN_DECODE \
	if( !pcInBuffer || sInLength <= 0 ) \
	{ \
		return -1; \
	} \
	bool bLengthValid = BeginDecode((char*)pcInBuffer, sInLength); \
	if( !bLengthValid ) \
	{ \
		return -1; \
	}

#define END_DECODE \
    int iDecodeResult = EndDecode(); \
	return iDecodeResult;


#define DECLARE_MSGFUNC\
	virtual int Encode(char *pcOutBuffer, int &sOutLength)\
	{\
		BEGIN_ENCODE;\
		StartEncode();\
		END_ENCODE;\
	}\
	void StartEncode();\
	virtual int Decode(const char *pcInBuffer, const int sInLength)\
	{\
		BEGIN_DECODE;\
		StartDecode();\
		END_DECODE;\
	}\
	void StartDecode();\
	virtual void Print();

	

extern FILE* g_fpOutPut;
extern void PrintBin(char *pBuffer, int iLength);




#define MAX_MSG_COUNT    32
#define MAX_MSG_BUF_LEN  (100 * 1024)
class CMsgPara : public CBaseOperator
{
public:
	CMsgPara();
	virtual ~CMsgPara();
	virtual int Encode( char *pcOutBuffer, int &sOutLength ) = 0;
	virtual int Decode( const char *pcInBuffer, const int sInLength ) = 0;
	virtual void Print() = 0;

	static unsigned long ulCreateTimes;
	static unsigned long ulDestroyTimes;

	static void* operator new( size_t nSize );
	static void  operator delete( void *pMem );


#ifdef _MULTHREAD_MSG_

  #ifdef WIN32
    static void InitMutex();	
  #endif

	static pthread_mutex_t ms_Mutex;
	static unsigned char  ms_byMsgAllocFlag[MAX_MSG_COUNT];
	static unsigned char  ms_byBuffer[MAX_MSG_COUNT][MAX_MSG_BUF_LEN];
#endif
};


class CMsgHead
{
public:
    CMsgHead();
    ~CMsgHead();

    int  Encode( char *pcOutBuffer, int &sOutLength );
    int  Decode( const char *pcInBuffer, const int sInLength );
    void Print();

    short m_sMsgID;
	short m_sMsgType;
	int   m_iMsgSeq;
	char  m_bSrcFE;
	char  m_bDstFE;
	short m_sSrcID;
	short m_sDstID;
};

class CMsg
{
public:
	CMsg();
	~CMsg();

	static CMsgPara* CreateServersMsgPara( short sMsgID, short sMsgType);
	static CMsgPara* CreateClientServerMsgPara( short sMsgID, short sMsgType);
	int  CreatePara();
	int  Encode( char *pcOutBuffer, int &sOutLength );
	int  Decode( const char *pcInBuffer, const int sInLength );
	void Print();

	CMsgHead m_stHead;
	CMsgPara *m_pMsgPara;

public:
	void SetMsgPara(CMsgPara* pMsgPara, bool bDelete)
	{
		m_pMsgPara = pMsgPara;
		m_bDelete  = bDelete;
	}
private:
	bool m_bDelete;
};

#endif

