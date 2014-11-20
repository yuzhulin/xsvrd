#ifndef _TCPCONN_H_
#define _TCPCONN_H_

#include "base.h"
#include "codequeue.h"
#include "tcpsocket.h"

#define _POSIX_MT_

enum enConnFlags
{
	FLG_CONN_IGNORE = 0,
	FLG_CONN_REDO	= 1,
	FLG_CONN_CTRL	= 2
};
enum enBlockStates
{
	BLK_EMPTY    = 0,
	BLK_SAVING   = 1,
	BLK_REDOING  = 2
};

enum enEntityStates
{
	ENTITY_OFF  = 0,
	ENTITY_ON   = 1
};

class CTCPConn
{
public:
	CTCPConn();
	~CTCPConn();
	enum enConnParas
	{
#ifdef WIN32
		BLOCKQUEUESIZE = 0x2000,
#else
		BLOCKQUEUESIZE = 0x1000000,
#endif
	};

	int Initialize(short nEntityType, short nEntityID, u_long ulIPAddr, u_short unPort);
	int ConnectToServer(char* szLocalAddr = NULL);
	int AsynConnectToServer(timeval tmConnTimeout);
	int GetEntityInfo(short* pnEntityType, short* pnEntityID, unsigned long* pulIpAddr);
	u_long GetConnAddr();
	u_short GetConnPort();
	short  GetEntityType();
	short  GetEntityID();

	int EstConn( SOCKET iAcceptFD );
	int IsConnCanRecv();

	int RegToCheckSet( fd_set *pCheckSet );
	int IsFDSetted( fd_set *pCheckSet );
	int RecvAllData();
	int GetOneCode( int &nCodeLength, unsigned char *pCode  );
	int SendCode(int nCodeLength, unsigned char *pCode, int iFlag);

	int CleanBlockQueue(int iQueueLength);

	int SetConnState(int iConnState);
	CTCPSocket* GetSocket();
private:
	void CreateCodeQueue(CCodeQueue* &pCodeQueue, unsigned char* &pbyBuffer);
	void DeleteCodeQueue(CCodeQueue* &pCodeQueue, unsigned char* &pbyBuffer);


private:
	short   m_nEntityType;
	short   m_nEntityID;
	CTCPSocket m_stSocket;
	u_long m_ulIPAddr;
	u_short m_unPort;

	int m_iConnState;

	CCodeQueue *m_pBlockQueue;
	CCodeQueue *m_pRedoQueue;

	unsigned char*       m_pbyRedoBuffer;
	unsigned char*       m_pbyBlockBuffer;
	int m_iCurrentDumpSeq;
	int m_iCurrentRedoSeq;
	int m_iBlockStatus;
#ifdef _POSIX_MT_
	pthread_mutex_t m_stMutex;			//操作的互斥变量
#endif
};

#endif
