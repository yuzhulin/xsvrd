#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_

#include "base.h"
#include "os.h"
#include "codequeue.h"

#ifdef _DEBUG
#define SAY TLib_Log_LogMsg
#else
static void SAY(const char *szFormat, ...){}
#endif



#define RECVBUFLENGTH	(512 * 1024)
#define POSTBUFLENGTH	(1024 * 1024 * 2)  //默认10M
#define SENDBUFSIZE     (260 * 1024 - 2)

#ifndef MAX_USHORT_SIZE 
#define MAX_USHORT_SIZE 0xFFFF
#endif

//以下定义tcp连接状态
enum eTCPConnStates
{
	tcs_closed = 0,
	tcs_opened,
	tcs_connected,
	tcs_error
};

enum eTCPConnectResult
{
	TCP_CONNECT_SUCC = 0,
	ASYNC_TCP_CONNECT_SUCC = 1,
};

//以下定义Socket类型
enum eSockTypes
{
	sot_listen = 0,
	sot_comm
};


//以下定义接受/发送错误类型
enum eRecvErrs
{
	ERR_RECV_NOSOCK = -1,
	ERR_RECV_NOBUFF = -2,
	ERR_RECV_REMOTE = -3,
	ERR_RECV_FALIED = -4,
	CONTINUE_RECV   = 1,
};
enum eSendErrs
{
	ERR_SEND_NOSOCK = -1,
	ERR_SEND_NOTCPBUFF = -2,
	ERR_SEND_NOAPPBUFF = -3,
	ERR_SEND_FAILED = ERR_SEND_NOAPPBUFF,
};

class CTCPSocket
{
public:
	CTCPSocket();
	CTCPSocket(unsigned int nRecvBuffLen);
	~CTCPSocket();
	SOCKET GetSocketFD();
	int GetStatus();
    int SetConnected();
	int CreateClient( char* szLocalAddr = NULL );
	int CreateServer(unsigned short unPort, char* szIPAddr=NULL );
	int ConnectTo(char *szIPAddr, unsigned short unPort);
	int ConnectTo(unsigned long ulIPNetAddr, unsigned short unPort);
    int AsynConnectTo(u_long ulIPNetAddr, u_short unPort);
	int Accept( SOCKET iAcceptFD );

	int RecvData();
	int GetOneCode( int &nCodeLength, unsigned char *pCode  );
	int SendOneCode( int nCodeLength, unsigned char *pCode, bool bReserveData = true );
	// 只发送本次提交的数据，且发送失败也不放到滞留缓冲区（此时最好关闭socket）
	int SendCode( int nCodeLength, unsigned char *pCode);
    int GetAllData( int &nCodeLength, unsigned char *pCode  );
	int AddToCheckSet( fd_set *pCheckSet );
	int IsFDSetted( fd_set *pCheckSet );
	int HasReserveData();
	int CleanReserveData();
    int Close();
    bool IsConnect();
    void SetReserveBufSize(int nReserveBufSize);
	unsigned int GetRecvPackCount();
	unsigned long GetServerIP();
	const char* GetServerIPStr();
	unsigned short GetServerPort();
	void SetServerIPPort(unsigned int nServerIP, unsigned short nPort);

private:
	void Initialize(unsigned int nRecvBuffLen);
	int ReserveData(int nCodeLength, unsigned char *pCode);
	int SendReserveData();

protected:
	SOCKET m_iSocketFD;					//Socket描述子
	int m_iSocketType;					//Socket类型
	int m_iStatus;						//连接状态

private:
	int  m_iReadBegin;
	int  m_iReadEnd;
	int  m_nMaxRecvBufferLen;
	unsigned char* m_abyRecvBuffer;
	int  m_iPostBegin;
	int  m_iPostEnd;
	unsigned char* m_abyPostBuffer;
	unsigned int m_nMaxPostLength;
	unsigned int m_nRecvPackCount;
	unsigned long m_ulServerAddr;
	unsigned short m_nServerPort;
	char m_szServerIPStr[IP_LENGTH];
};

#endif
