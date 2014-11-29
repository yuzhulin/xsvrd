#include "os.h"
#include <stdlib.h>
#include "tcpsocket.h"
#include "tlib_log.h"

CTCPSocket::CTCPSocket()
{	
	Initialize(RECVBUFLENGTH);
}

CTCPSocket::CTCPSocket(unsigned int nRecvBuffLen)
{	
	Initialize(nRecvBuffLen);
}

void CTCPSocket::Initialize(unsigned int nRecvBuffLen)
{	
	m_iSocketFD = INVALID_SOCKET;
	m_iSocketType = sot_comm;
	m_iStatus = tcs_closed;
	m_iReadBegin = 0;
	m_iReadEnd = 0;
	m_iPostBegin = m_iPostEnd = 0;
	m_abyPostBuffer = NULL;
	m_nMaxPostLength = 0;
	m_nRecvPackCount = 0;
	m_ulServerAddr = 0;
	m_nServerPort = 0;
	memset(m_szServerIPStr, 0, sizeof(m_szServerIPStr));

	m_nMaxRecvBufferLen = nRecvBuffLen;
	m_abyRecvBuffer = new unsigned char[nRecvBuffLen];
}

CTCPSocket::~CTCPSocket()
{
    if( m_iStatus == tcs_opened && m_iSocketFD != INVALID_SOCKET )
    {
        Close();
    }
    if (m_abyPostBuffer != NULL)
    {
        delete []m_abyPostBuffer;
        m_abyPostBuffer = NULL;
    }
	delete []m_abyRecvBuffer;
}

int CTCPSocket::CreateClient(char* szLocalAddr /* = NULL  */)
{
	m_nRecvPackCount = 0;
	m_ulServerAddr = 0;
	m_nServerPort = 0;
    memset(m_szServerIPStr, 0, sizeof(m_szServerIPStr));
    socklen_t iOptVal = 0;
    int iOptLen = sizeof(int);
    if(m_iStatus != tcs_closed && m_iSocketFD != INVALID_SOCKET )
    {
        Close();
    }

    m_iSocketType = sot_comm;

    m_iSocketFD = socket(AF_INET, SOCK_STREAM, 0);

    if( m_iSocketFD == INVALID_SOCKET )
    {
        m_iStatus = tcs_closed;
        return -1;
    }

    if( szLocalAddr )
    {
        sockaddr_in stTempSockAddr;
        memset((void *)&stTempSockAddr, 0, sizeof(sockaddr_in));
        stTempSockAddr.sin_family = AF_INET;
        stTempSockAddr.sin_addr.s_addr = inet_addr(szLocalAddr);
        bind(m_iSocketFD, (const struct sockaddr *)&stTempSockAddr, sizeof(stTempSockAddr));
    }

    m_iStatus = tcs_opened;
    iOptLen = sizeof(socklen_t);
    iOptVal = SENDBUFSIZE;
#ifdef WIN32
    if (setsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (const char *)&iOptVal, iOptLen))
#else
    if (setsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (const void *)&iOptVal, iOptLen))
#endif
    {
        LOG("Set Send buffer size to %d failed!\n", iOptVal);
        return -1;
    }
#ifdef WIN32
    if (!getsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (char *)&iOptVal, (socklen_t *)&iOptLen))
#else
    if (!getsockopt(m_iSocketFD, SOL_SOCKET, SO_SNDBUF, (void *)&iOptVal, (socklen_t *)&iOptLen))
#endif
    {
        LOG("Set Send buf of socket is %d.\n", iOptVal);
    }

    m_iReadBegin = m_iReadEnd = 0;
    m_iPostBegin = m_iPostEnd = 0;
    return 0;
}

int CTCPSocket::CreateServer(unsigned short unPort, char* szIPAddr/* =NULL  */)
{
    sockaddr_in stTempSockAddr;
    int iReusePortFlag = 1;

    if(m_iStatus != tcs_closed && m_iSocketFD != INVALID_SOCKET)
    {
        Close();
    }

    m_iSocketType = sot_listen;

    m_iSocketFD = socket(AF_INET, SOCK_STREAM, 0);

    if( m_iSocketFD == INVALID_SOCKET )
    {
        m_iStatus = tcs_closed;

        return -1;
    }
#ifdef WIN32
    if( setsockopt(m_iSocketFD, SOL_SOCKET, SO_REUSEADDR, (const char*)&iReusePortFlag, sizeof(int)) )
#else
    if( setsockopt(m_iSocketFD, SOL_SOCKET, SO_REUSEADDR, &iReusePortFlag, sizeof(int)) )
#endif
    {
        LOG("Set socket addr reusable failed, %s.\n", strerror(errno));
    }

    memset((void *)&stTempSockAddr, 0, sizeof(sockaddr_in));
    stTempSockAddr.sin_family = AF_INET;
    stTempSockAddr.sin_port = htons(unPort);

    if( szIPAddr )
    {
        stTempSockAddr.sin_addr.s_addr = inet_addr(szIPAddr);
    }
    else
    {
        stTempSockAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    }

    if( bind(m_iSocketFD, (const struct sockaddr *)&stTempSockAddr, sizeof(stTempSockAddr)) )
    {
        Close();
        LOG("Bind failed, %s.\n", strerror(errno));
        return -1;
    }

    if( listen(m_iSocketFD, 10) < 0 )
    {
        LOG("Listen failed, %s.\n", strerror(errno));
        Close();
        return -1;
    }

    SetNBlock( m_iSocketFD );
    m_iStatus = tcs_opened;

    return 0;
}

bool CTCPSocket::IsConnect()
{
    if( m_iSocketFD == INVALID_SOCKET || m_iStatus != tcs_connected )
    {
        return false;
    }
	return true;
}

int CTCPSocket::ConnectTo(u_long ulIPNetAddr, u_short unPort)
{
    sockaddr_in stTempAddr;

    if( m_iStatus != tcs_opened || m_iSocketFD == INVALID_SOCKET )
    {
        return -2;
    }

    memset((void *)&stTempAddr, 0, sizeof(sockaddr_in));
    stTempAddr.sin_family = AF_INET;
    stTempAddr.sin_port = htons(unPort);
    stTempAddr.sin_addr.s_addr = ulIPNetAddr;

    if( connect(m_iSocketFD, (const struct sockaddr *)&stTempAddr, sizeof(stTempAddr)) )
    {
        Close();
        return -3;
    }
	m_ulServerAddr = ulIPNetAddr;
	m_nServerPort = unPort;
	IPValue2Str(ulIPNetAddr, m_szServerIPStr);

    SetNBlock( m_iSocketFD );

    m_iReadBegin = m_iReadEnd = 0;
    m_iPostBegin = m_iPostEnd = 0;
    m_iStatus = tcs_connected;

    return 0;
}


int CTCPSocket::AsynConnectTo(u_long ulIPNetAddr, u_short unPort)
{
    sockaddr_in stTempAddr;

    m_iReadBegin = m_iReadEnd = 0;
    m_iPostBegin = m_iPostEnd = 0;

    if( m_iStatus != tcs_opened || m_iSocketFD == INVALID_SOCKET )
    {
        return -1;
    }

    SetNBlock( m_iSocketFD );

    memset((void *)&stTempAddr, 0, sizeof(sockaddr_in));
    stTempAddr.sin_family = AF_INET;
    stTempAddr.sin_port = htons(unPort);
    stTempAddr.sin_addr.s_addr = ulIPNetAddr;

	m_ulServerAddr = ulIPNetAddr;
	m_nServerPort = unPort;
	IPValue2Str(ulIPNetAddr, m_szServerIPStr);

    if( connect(m_iSocketFD, (const struct sockaddr *)&stTempAddr, sizeof(stTempAddr)) )
    {
#ifdef WIN32
        DWORD dwLastError = WSAGetLastError();
        if (dwLastError != WSAEWOULDBLOCK)
        {
            Close();
			LOG("connect to %s:%hd failed!, Connect ErrCode= %lu\n", m_szServerIPStr, unPort, dwLastError);
            return -2;
        }
#else
        if (errno != EINPROGRESS)
        {
            Close();
            LOG("connect to %s:%hd failed!, Connect ErrCode= %d\n", m_szServerIPStr, unPort, errno);
            return -2;
        }		
#endif
		return ASYNC_TCP_CONNECT_SUCC;
    }
    
	m_iStatus = tcs_connected;

    return TCP_CONNECT_SUCC;
}

int CTCPSocket::ConnectTo(char *szIPAddr, unsigned short unPort)
{
    sockaddr_in stTempAddr;

    if( !szIPAddr )
    {
        return -1;
    }

    if( m_iStatus != tcs_opened || m_iSocketFD == INVALID_SOCKET )
    {
        return -2;
    }

    memset((void *)&stTempAddr, 0, sizeof(sockaddr_in));
    stTempAddr.sin_family = AF_INET;
    stTempAddr.sin_port = htons(unPort);
	unsigned long ulIPNetAddr = inet_addr(szIPAddr);
    stTempAddr.sin_addr.s_addr = ulIPNetAddr;

    if( connect(m_iSocketFD, (const struct sockaddr *)&stTempAddr, sizeof(stTempAddr)) )
    {
        Close();
        return -3;
    }
	m_ulServerAddr = ulIPNetAddr;
	m_nServerPort = unPort;
	IPValue2Str(ulIPNetAddr, m_szServerIPStr);



    SetNBlock( m_iSocketFD );
   
    m_iReadBegin = m_iReadEnd = 0;
    m_iPostBegin = m_iPostEnd = 0;
    
    m_iStatus = tcs_connected;

    return 0;
}

// -1 --- Invalid input fd, -2 ----- socket already connected
int CTCPSocket::Accept(SOCKET iAcceptFD)
{
    if( iAcceptFD == INVALID_SOCKET )
    {
        return -1;
    }
    int iTempRet = 0;

    if( m_iSocketFD != INVALID_SOCKET && m_iStatus == tcs_connected )
    {
        LOG("Warning, another connection request from remote, close the previous(%d).\n", m_iSocketFD);
        Close(); 
    }

    m_iSocketFD = iAcceptFD;
    m_iSocketType = sot_comm;
    m_iStatus = tcs_connected;
    m_iReadBegin = 0;
    m_iReadEnd = 0;

    
    m_iPostBegin = m_iPostEnd = 0;
    
    SetNBlock( m_iSocketFD );
    return iTempRet;
}

int CTCPSocket::Close()
{
    if( m_iSocketFD != INVALID_SOCKET )
    {
        closesocket(m_iSocketFD);
		m_iSocketFD = INVALID_SOCKET;
    }
    m_iStatus = tcs_closed;
	m_iPostBegin = m_iPostEnd = 0;

    if (m_abyPostBuffer != NULL)
    {
        delete[] m_abyPostBuffer;
        m_abyPostBuffer = NULL;
    }

    return 0;
}

SOCKET CTCPSocket::GetSocketFD()
{
    return m_iSocketFD;
}

int CTCPSocket::SetConnected()
{   
    m_iStatus = tcs_connected;
    return 0;
}
int CTCPSocket::GetStatus()
{
    int iTmpStatus = tcs_closed;
    iTmpStatus = m_iStatus;
    return iTmpStatus;
}

//返回值：-1 ：Socket状态错误；-2 ：接收缓冲区已满；-3 ：对端断开连接；-4 ：接收错误
int CTCPSocket::RecvData()
{
    int iRecvedBytes = 0;
    int iTempRet = 0;


    char szPeerAddr[32];
    sockaddr stPeerAddr;
    socklen_t iPeerAddrLen = sizeof(stPeerAddr);
    memset(&stPeerAddr, 0, iPeerAddrLen);

    if( m_iSocketFD == INVALID_SOCKET || m_iStatus != tcs_connected )
    {
        return ERR_RECV_NOSOCK;
    }

    if( m_iReadEnd == m_iReadBegin )
    {
        m_iReadBegin = 0;
        m_iReadEnd = 0;
    }
    
    do
    {      
        if( m_iReadEnd == m_nMaxRecvBufferLen )
        {
            LOG("The recv buffer is full now(%d, %d), stop recv data, fd = %d.\n", m_iReadBegin, m_iReadEnd, m_iSocketFD);
            iTempRet = ERR_RECV_NOBUFF;
            break;
        }
        
        
#ifdef WIN32
        iRecvedBytes = recv(m_iSocketFD, (char*)&m_abyRecvBuffer[m_iReadEnd],
                            m_nMaxRecvBufferLen - m_iReadEnd, 0);
#else
        errno = 0;
        iRecvedBytes = recv(m_iSocketFD, &m_abyRecvBuffer[m_iReadEnd],
                            m_nMaxRecvBufferLen - m_iReadEnd, 0);
#endif        
        if( iRecvedBytes > 0 )
        {
            m_iReadEnd += iRecvedBytes;
			++m_nRecvPackCount;
        }
        else if( iRecvedBytes == 0 )
        {
            getpeername(m_iSocketFD, &stPeerAddr, &iPeerAddrLen);
            SockAddrToString((sockaddr_in *)&stPeerAddr, szPeerAddr);
			int errcode = errno;
            LOG("The remote site %s may closed this conn, fd = %d, errno = %d.\n", szPeerAddr, m_iSocketFD, errcode);
            Close();
            iTempRet = ERR_RECV_REMOTE;
            break;
        }
#ifndef WIN32
        else if( errno != EAGAIN )
#else
		else if( WSAGetLastError() !=  WSAEWOULDBLOCK)
#endif
        {	
			getpeername(m_iSocketFD, &stPeerAddr, &iPeerAddrLen);
			SockAddrToString((sockaddr_in *)&stPeerAddr, szPeerAddr);
			LOG("Error in read, %s, socket fd = %d, remote site %s.\n", strerror(errno), m_iSocketFD, szPeerAddr);
			Close();
			iTempRet = ERR_RECV_FALIED;
            break;
        }
#ifndef WIN32
    } while( iRecvedBytes > 0 );
#else
    } while( 0 );
#endif

    return iTempRet;
}

int CTCPSocket::GetAllData( int &nCodeLength, unsigned char *pCode  )
{
	int shMaxBufferLen = nCodeLength;
	int iDataLength = 0;
	//int nTempLength;
	nCodeLength = 0;

	if( !pCode )
	{
		return -1;
	}

	iDataLength = m_iReadEnd - m_iReadBegin;
	if( iDataLength <= 0 )
	{
		return -2;
	}
	if( iDataLength > shMaxBufferLen )
	{
	    return -3;
	}
	memcpy((void *)pCode, (const void *)&m_abyRecvBuffer[m_iReadBegin], iDataLength);
	nCodeLength = iDataLength;
	m_iReadBegin = m_iReadEnd = 0;
	return 0;
}

int CTCPSocket::GetOneCode(int &nCodeLength, unsigned char *pCode)
{
    int nMaxBufferLen = nCodeLength;
    int iDataLength = 0;
    if( !pCode )
    {
        return -1;
    }

    iDataLength = m_iReadEnd - m_iReadBegin;

    if( iDataLength <= (int)sizeof(unsigned int) )
    {
        return -2;
    }

    nCodeLength = ntohl( *((int *)&m_abyRecvBuffer[m_iReadBegin]) );
    if( nCodeLength <= 0 )
    {
		//按理说<=0不可能，最少也是4
        m_iReadBegin = m_iReadEnd = 0;
        return -3;
    }	
	if( iDataLength < nCodeLength )
	{
		//还没有收全包，判断是否有足够空间来收包
		if( m_iReadBegin + nCodeLength >= m_nMaxRecvBufferLen )
		{
			memmove(&m_abyRecvBuffer[0], &m_abyRecvBuffer[m_iReadBegin], iDataLength);
			m_iReadBegin = 0;
			m_iReadEnd = iDataLength;

			//memmvoe以后再判断一次是否可以接收
			if( nCodeLength > m_nMaxRecvBufferLen )
			{
				//已经超过了接收缓冲区最大长度，所以关闭socket
				Close();
				return -901;
			}
		}
		return -4;
	}
	
	memcpy(pCode, &m_abyRecvBuffer[m_iReadBegin], nCodeLength);
	m_iReadBegin += nCodeLength;
    if( m_iReadBegin == m_iReadEnd )
    {
        m_iReadBegin = m_iReadEnd = 0;
    }
    return 0;
}

int CTCPSocket::SendReserveData()
{
	int iBytesSent = 0;
	int iBytesLeft = m_iPostEnd - m_iPostBegin;
	if( iBytesLeft == 0 )
	{
		return 0;
	}
	unsigned char* pbyTemp = &(m_abyPostBuffer[m_iPostBegin]);

	while(1)
	{
		iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);
		if( iBytesSent > 0 )
		{
			pbyTemp += iBytesSent;
			iBytesLeft -= iBytesSent;
			m_iPostBegin += iBytesSent;
			if( iBytesLeft == 0 )
			{
				m_iPostBegin = 0;
				m_iPostEnd = 0;
                return 0;
			}
		}
#ifndef WIN32
		else if( iBytesSent < 0 )
		{
			if( errno == EAGAIN )
			{
#else
		else if( iBytesSent == SOCKET_ERROR )
		{
			if( WSAGetLastError() == WSAEWOULDBLOCK )
			{
#endif
                //未发送出的数据已经在滞留缓冲区了，不需要再保留了？
				/*int nTempRet = ReserveData(iBytesLeft, pbyTemp);
				if( nTempRet != 0 )
				{
					return ERR_SEND_FAILED;
				}
				else
				{
					return ERR_SEND_NOTCPBUFF;
				}*/
                return ERR_SEND_NOTCPBUFF;
			}
			else
			{
				m_iStatus = tcs_error;
				return ERR_SEND_FAILED;
			}
		}
		else
		{
			m_iStatus = tcs_error;
			return ERR_SEND_FAILED;
		}
	}
    return 0;
}

int CTCPSocket::ReserveData(int iBytesLeft, unsigned char *pbyTemp)
{
	//Socket发送缓冲区满，则将剩余的数据放到缓存中,这个地方应该判断下缓冲区是否足够长，
	//否则会发生缓冲区越界的问题。
	if( m_abyPostBuffer == NULL )
	{
		if (m_nMaxPostLength <= 0)
		{
            m_nMaxPostLength = POSTBUFLENGTH;
		}
		m_abyPostBuffer = new unsigned char[m_nMaxPostLength];
	}
	int nBodyLen = m_iPostEnd - m_iPostBegin;
	if( nBodyLen + iBytesLeft < (int)m_nMaxPostLength )
	{
		if( m_iPostBegin > 0 )
		{
			memmove(&m_abyPostBuffer[0], &m_abyPostBuffer[m_iPostBegin], nBodyLen);
		}
		m_iPostBegin = 0;
		m_iPostEnd = nBodyLen;
		memcpy((void *)&(m_abyPostBuffer[m_iPostEnd]), (const void *)pbyTemp, iBytesLeft);
		m_iPostEnd += iBytesLeft;
		return 0;
	}
	else
	{
		//原有的缓冲区不够大，可以考虑new一个新的缓冲区或者直接失败。
		m_iStatus = tcs_error;
		return -1;
	}
	return 0;
}

//返回值说明：-1：参数错误或状态非法；-2：发送缓冲区满；-3：发送系统错误；
int CTCPSocket::SendOneCode(int nCodeLength, unsigned char *pCode, bool bReserveData)
{
    int iBytesSent = 0;
    int iBytesLeft = nCodeLength;
    unsigned char *pbyTemp = NULL;
    int iTempRet = 0;
    
    if( !pCode )
    {
        return ERR_SEND_NOSOCK;
    }

    if( m_iSocketFD == INVALID_SOCKET || m_iStatus != tcs_connected )
    {
        return ERR_SEND_NOSOCK;
    }
    //首先检查是否有滞留数据
	iTempRet = SendReserveData();
    if( iTempRet == ERR_SEND_NOTCPBUFF)
	{
        //发送滞留数据失败，把本次需要发送的数据放入滞留缓冲区
        if (!bReserveData)
        {
            return ERR_SEND_NOTCPBUFF;
        }
        int nTempRet = ReserveData(nCodeLength, pCode);
        if( nTempRet != 0 )
        {
            return ERR_SEND_FAILED;
        }
        return ERR_SEND_NOTCPBUFF;
	}

    //发送本次提交的数据
    iBytesLeft = nCodeLength;
    pbyTemp = pCode;

    while( iBytesLeft > 0 )
    {
        iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);        
        if( iBytesSent > 0 )
        {
            pbyTemp += iBytesSent;
            iBytesLeft -= iBytesSent;
        }
#ifndef WIN32
		else if( iBytesSent < 0 )
		{
			if( errno == EAGAIN )
#else
		else if( iBytesSent == SOCKET_ERROR )
		{
			if( WSAGetLastError() == WSAEWOULDBLOCK )
#endif
			{
				if( !bReserveData )
				{
					if( iBytesLeft == nCodeLength )
					{
						return ERR_SEND_NOTCPBUFF;
					}
				}
				int nTempRet = ReserveData(iBytesLeft, pbyTemp);
				if( nTempRet != 0 )
				{
					return ERR_SEND_FAILED;
				}
				else
				{
					return ERR_SEND_NOTCPBUFF;
				}				
			}
			else
			{
				m_iStatus = tcs_error;
				return ERR_SEND_FAILED;
			}
		}
		else
		{
			m_iStatus = tcs_error;
			return ERR_SEND_FAILED;
		}
    }
    return iTempRet;
}

int CTCPSocket::SendCode( int nCodeLength, unsigned char *pCode)
{
	int iBytesSent = 0;
	int iBytesLeft = nCodeLength;
	unsigned char *pbyTemp = NULL;

	if( !pCode )
	{
		return ERR_SEND_NOSOCK;
	}

	if( m_iSocketFD == INVALID_SOCKET || m_iStatus != tcs_connected )
	{
		return ERR_SEND_NOSOCK;
	}

	iBytesLeft = nCodeLength;
	pbyTemp = pCode;

	while( iBytesLeft > 0 )
	{
		iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);        
		if( iBytesSent > 0 )
		{
			pbyTemp += iBytesSent;
			iBytesLeft -= iBytesSent;
		}
#ifndef WIN32
		else if( iBytesSent < 0 )
		{
			if( errno == EAGAIN )
#else
		else if( iBytesSent == SOCKET_ERROR )
		{
			if( WSAGetLastError() == WSAEWOULDBLOCK )
#endif
			{
				return ERR_SEND_NOTCPBUFF;	
			}
			else
			{
				m_iStatus = tcs_error;
				return ERR_SEND_FAILED;
			}
		}
		else
		{
			m_iStatus = tcs_error;
			return ERR_SEND_FAILED;
		}
	}
	return 0;
}

int CTCPSocket::AddToCheckSet(fd_set *pCheckSet)
{
    int iTempRet = 0;

    if( !pCheckSet )
    {
        return -1;
    }

    if( m_iSocketFD != INVALID_SOCKET && m_iStatus == tcs_connected )
    {
        FD_SET( m_iSocketFD, pCheckSet );
    }
    else if( m_iSocketFD != INVALID_SOCKET )
    {
        Close();
        iTempRet = -2;
    }



    return iTempRet;
}

int CTCPSocket::IsFDSetted(fd_set *pCheckSet)
{
    int iTempRet = False;

    if( !pCheckSet )
    {
        return False;
    }

    if( m_iSocketFD != INVALID_SOCKET && m_iStatus == tcs_connected )
    {
        iTempRet = FD_ISSET( m_iSocketFD, pCheckSet );
    }
    else
    {
        iTempRet = False;
    }

    return iTempRet;
}
int CTCPSocket::HasReserveData()
{
    if(m_iPostEnd - m_iPostBegin > 0)
    {
        return True;
    }
    else
    {
        return False;
    }
}

int CTCPSocket::CleanReserveData()
{
    int iBytesSent = 0, iBytesLeft = 0, iBytesCleaned = 0, iTempRet = 0;
    unsigned char *pbyTemp = NULL;

    if( m_iSocketFD == INVALID_SOCKET || m_iStatus != tcs_connected )
    {
        return ERR_SEND_NOSOCK;
    }
    iBytesLeft = m_iPostEnd - m_iPostBegin;
    pbyTemp = &(m_abyPostBuffer[m_iPostBegin]);
    while( iBytesLeft > 0 )
    {
        iBytesSent = send(m_iSocketFD, (const char *)pbyTemp, iBytesLeft, 0);
        if( iBytesSent > 0 )
        {
            pbyTemp += iBytesSent;
            iBytesLeft -= iBytesSent;
            m_iPostBegin += iBytesSent;
            iBytesCleaned += iBytesSent;
        }
#ifndef WIN32
		else if( iBytesSent < 0 )
		{
			if( errno == EAGAIN )
#else
		else if( iBytesSent == SOCKET_ERROR )
		{
			if( WSAGetLastError() == WSAEWOULDBLOCK )
#endif		
			{
				iTempRet = ERR_SEND_NOTCPBUFF;
				break;
			}
			else
			{
				m_iStatus = tcs_error;
				iTempRet = ERR_SEND_FAILED;
				break;
			}
		}
		else
		{
			m_iStatus = tcs_error;
			iTempRet = ERR_SEND_FAILED;
			break;
		}
    }
    if( iBytesLeft == 0 )
    {
        m_iPostBegin = m_iPostEnd = 0;
        iTempRet = 0;
    }
    SAY("CleanReserveData %d bytes is cleaned, left %d bytes.\n", iBytesCleaned, iBytesLeft);
    return iTempRet;
}

void CTCPSocket::SetReserveBufSize(int nReserveBufSize)
{
    if (nReserveBufSize <= 0)
    {
        m_nMaxPostLength = POSTBUFLENGTH; //默认10M
    }
    else
    {
        m_nMaxPostLength = nReserveBufSize;
    }
}

unsigned int CTCPSocket::GetRecvPackCount()
{
	return m_nRecvPackCount;
}

unsigned long CTCPSocket::GetServerIP()
{
    return m_ulServerAddr;
}

unsigned short CTCPSocket::GetServerPort()
{
    return m_nServerPort;
}

const char* CTCPSocket::GetServerIPStr()
{
	return m_szServerIPStr;
}

void CTCPSocket::SetServerIPPort(unsigned int nServerIP, unsigned short nPort)
{
    m_ulServerAddr = nServerIP;
	m_nServerPort = nPort;
	IPValue2Str(m_ulServerAddr, m_szServerIPStr);
}


