#include "os.h"
#include "os.h"
#include "tcpconn.h"
#include "tlib_log.h"


CTCPConn::CTCPConn()
{
    m_ulIPAddr = 0;
    m_nEntityID = -1;
    m_nEntityType = -1;
#ifdef _POSIX_MT_
    pthread_mutex_init( &m_stMutex, NULL );
#endif
    m_iBlockStatus = BLK_EMPTY;
    m_iCurrentDumpSeq = 0;
    m_iCurrentRedoSeq = 0;
    m_pBlockQueue = NULL;
    m_pRedoQueue = NULL;
    m_pbyRedoBuffer  = NULL;
    m_pbyBlockBuffer = NULL;

    m_iConnState = ENTITY_OFF;
}

CTCPConn::~CTCPConn()
{
    DeleteCodeQueue(m_pBlockQueue, m_pbyBlockBuffer);
    DeleteCodeQueue(m_pRedoQueue,  m_pbyRedoBuffer);
}

int CTCPConn::Initialize(short nEntityType, short nEntityID, u_long ulIPAddr, u_short unPort)
{
    m_ulIPAddr = ulIPAddr;
    m_unPort = unPort;
    m_nEntityID = nEntityID;
    m_nEntityType = nEntityType;
    return 0;
}

int CTCPConn::ConnectToServer(char* szLocalAddr)
{
	//[Modify by xushvai@gamil.com 2013-11-25] begin
	///////////////////////////////////////////////////////////////
    //if( m_nEntityID <= 0 )
	//   ||
	//   ||
	//   \/
	if (m_nEntityID < 0)
    {
	///////////////////////////////////////////////////////////////
	//[Modify by xushvai@gamil.com 2013-11-25] end
        return -1;
    }
    if( m_stSocket.CreateClient() )
    {
        LOG("Conn create client of DBServer %d failed.\n", m_nEntityID);
        return -1;
    }

    return m_stSocket.ConnectTo(m_ulIPAddr, m_unPort);
}

CTCPSocket* CTCPConn::GetSocket()
{
    return &m_stSocket;
}

int CTCPConn::AsynConnectToServer(timeval tmConnTimeout)
{
	int nRet = -1;
	if( m_nEntityID <= 0 )
	{
		return -1;
	}
	if( m_stSocket.CreateClient() )
	{
		LOG("Conn create client of DBServer %d failed.\n", m_nEntityID);
		return -1;
	}

	nRet = m_stSocket.AsynConnectTo(m_ulIPAddr, m_unPort);
	// 返回-1表示正在连接
	if ( nRet != -1 )
	{
		return nRet;
	}

	fd_set writeSets;
    SOCKET iSocketFD = m_stSocket.GetSocketFD();
	FD_ZERO(&writeSets);

	FD_SET(iSocketFD, &writeSets);

	nRet = select(iSocketFD + 1, NULL, &writeSets, NULL, &tmConnTimeout);
	if ( nRet <= 0 )
	{
		m_stSocket.Close();
		return -4;
	}
	if ( !FD_ISSET(iSocketFD, &writeSets) )
	{
		m_stSocket.Close();
		return -5;
	}

#ifdef WIN32
	int nLen = sizeof(nRet);
	getsockopt(iSocketFD, SOL_SOCKET, SO_ERROR, (char*)&nRet, &nLen);
#else
	socklen_t nLen = sizeof(socklen_t);
	getsockopt(iSocketFD, SOL_SOCKET, SO_ERROR, (char*)&nRet, &nLen);
#endif
	if( nRet != 0 ) 
	{
		m_stSocket.Close();
		return -6;
	}

	m_stSocket.SetConnected();
	return 0;
}

int CTCPConn::GetEntityInfo(short* pnEntityType, short* pnEntityID, unsigned long* pulIpAddr)
{
    if( !pnEntityType || !pnEntityID || !pulIpAddr )
    {
        return -1;
    }

    *pnEntityType = m_nEntityType;
    *pnEntityID = m_nEntityID;
    *pulIpAddr = m_ulIPAddr;

    return 0;
}

u_long CTCPConn::GetConnAddr()
{
    return m_ulIPAddr;
}
u_short CTCPConn::GetConnPort()
{
    return m_unPort;
}

short CTCPConn::GetEntityType()
{
    return m_nEntityType;
}

short CTCPConn::GetEntityID()
{
    return m_nEntityID;
}
int CTCPConn::EstConn(SOCKET iAcceptFD)
{
    if( iAcceptFD <= 0 )
    {
        return -1;
    }
#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif

    m_stSocket.Accept(iAcceptFD);

    m_iConnState = ENTITY_ON;

#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif

    return 0;
}

int CTCPConn::IsConnCanRecv()
{
    int iTempRet = 0;
    
#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif
    if( m_stSocket.GetSocketFD() != INVALID_SOCKET && m_stSocket.GetStatus() == tcs_connected )
    {
        iTempRet = True;
    }
    else
    {
        iTempRet = False;
    }
#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif
    
    return iTempRet;
}

int CTCPConn::RegToCheckSet(fd_set *pCheckSet)
{
    int iTempRet = 0;
    
#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif

    iTempRet = m_stSocket.AddToCheckSet( pCheckSet );

#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif
    
    return iTempRet;
}

int CTCPConn::IsFDSetted(fd_set *pCheckSet)
{
    int iTempRet = 0;

#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif

    iTempRet = m_stSocket.IsFDSetted( pCheckSet );

#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif

    return iTempRet;
}

int CTCPConn::RecvAllData()
{
    int iTempRet = 0;
    
#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif

    iTempRet = m_stSocket.RecvData();

#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif

    return iTempRet;
}

int CTCPConn::GetOneCode(int &nCodeLength, unsigned char *pCode)
{
    return m_stSocket.GetOneCode( nCodeLength, pCode );
}

int CTCPConn::SendCode(int nCodeLength, unsigned char *pCode, int iFlag)
{
    int iTempRet = 0;
    
#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif

    if( m_nEntityType < 0 || m_nEntityID < 0 )
    {
        //未配置节点直接返回
#ifdef _POSIX_MT_
        pthread_mutex_unlock( &m_stMutex );
#endif
        return -1;
    }

    if( iFlag == FLG_CONN_CTRL || iFlag == FLG_CONN_IGNORE )
    {
        //带外数据和只读操作优先发送,不参与排队
        if( m_stSocket.GetSocketFD() != INVALID_SOCKET && m_stSocket.GetStatus() == tcs_connected )
        {
            iTempRet = m_stSocket.SendOneCode(nCodeLength, pCode);
        }
        else
        {
            iTempRet = -1;
        }

#ifdef _POSIX_MT_
        pthread_mutex_unlock( &m_stMutex );
#endif
        return iTempRet;
    }

    if( m_iBlockStatus == BLK_EMPTY && m_iConnState == ENTITY_ON )
    {
        iTempRet = m_stSocket.SendOneCode(nCodeLength, pCode);
    }

    //如果该server曾经连接上来过，然后又断开了，再有转发给该server的数据，则存储起来
    //block队列不为空
    //发送失败
    if(    m_iBlockStatus != BLK_EMPTY 
        || iTempRet != 0  )
    {
        SAY("Now begin to reserve codes.\n");

        if( m_pBlockQueue == NULL )
        {
            SAY("Create block queue.\n");
            CreateCodeQueue(m_pBlockQueue, m_pbyBlockBuffer); 
        }
        if( m_pBlockQueue )
        {
            if( m_iBlockStatus == BLK_EMPTY )
            {
                m_iBlockStatus = BLK_SAVING;
                SAY("Now the block status of (%d,%d) is %d.\n", m_nEntityType, m_nEntityID, m_iBlockStatus);
            }
            int iRsvRet = m_pBlockQueue->AppendOneCode(pCode, nCodeLength);
            if( iRsvRet == -2 )
            {
                if( m_pRedoQueue != m_pBlockQueue )
                {
                    TFName szDumpFile;
                    sprintf(szDumpFile, "CODE%02d_%02d_%02d.tmp", m_nEntityType, m_nEntityID, m_iCurrentDumpSeq);
                    SAY("Block queue is full now, begin to dump to file %s.\n", szDumpFile);
                    m_pBlockQueue->DumpToFile(szDumpFile);
                    m_pBlockQueue->CleanQueue();
                    m_iCurrentDumpSeq++;
                }
                else
                {
                    SAY("Block queue is full and redo queue is just the same, so create another block queue.\n");
                    CreateCodeQueue(m_pBlockQueue, m_pbyBlockBuffer); 
                }
                if( m_pBlockQueue )
                {
                    m_pBlockQueue->AppendOneCode(pCode, nCodeLength);
                }
            }
            SAY("Successfully reserved code.\n");
        }
    }

#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif

    return iTempRet;
}

int CTCPConn::CleanBlockQueue(int iQueueLength)
{
    int iRedoCount = 0, i, iTempRet = 0;
    int nCodeLength;
    unsigned char  abyCodeBuf[MAX_SERVER_TRANS_LEN];

#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif

    if( m_iConnState == ENTITY_ON &&
        m_stSocket.GetSocketFD() != INVALID_SOCKET && m_stSocket.GetStatus() == tcs_connected && m_iBlockStatus != BLK_EMPTY &&
        m_stSocket.CleanReserveData() == 0 )        
    {
        SAY("Now begin to redo transfer, will recover no more than %d codes.\n", iQueueLength);
        if( m_pRedoQueue == NULL )
        {
            CreateCodeQueue(m_pRedoQueue, m_pbyRedoBuffer);             
            SAY("Redo queue created now.\n");
        }
        if( m_pRedoQueue )
        {
            for( i = 0; i < iQueueLength; i++ )
            {
                if(m_stSocket.HasReserveData())
                {
                    LOG("Redo process is stopped because of data block.\n");
                    break;
                }
                m_pRedoQueue->GetHeadCode(abyCodeBuf, (unsigned int*)&nCodeLength);
                if( nCodeLength == 0 )
                {
                    SAY("The redo queue is empty now.\n");
                    if( m_iCurrentRedoSeq < m_iCurrentDumpSeq )
                    {
                        TFName szDumpFile;

                        sprintf(szDumpFile, "CODE%02d_%02d_%02d.tmp", m_nEntityType, m_nEntityID, m_iCurrentRedoSeq);
                        SAY("Load redo queue from file %s.\n", szDumpFile);
                        m_pRedoQueue->LoadFromFile(szDumpFile);
                        m_iCurrentRedoSeq++;
                        i--;
                        unlink((const char *)szDumpFile);
                    }
                    else if( m_pRedoQueue != m_pBlockQueue )
                    {
                        DeleteCodeQueue(m_pRedoQueue, m_pbyRedoBuffer);

                        m_pRedoQueue    = m_pBlockQueue;
                        m_pbyRedoBuffer = m_pbyBlockBuffer; 
                        m_pbyBlockBuffer = NULL;
                        i--;
                        SAY("Now begin to redo current block queue.\n");
                    }
                    else
                    {
                        DeleteCodeQueue(m_pRedoQueue, m_pbyRedoBuffer);

                        m_pRedoQueue    = m_pBlockQueue    = NULL;
                        m_pbyRedoBuffer = m_pbyBlockBuffer = NULL;
                        m_iBlockStatus = BLK_EMPTY;
                        m_iCurrentRedoSeq = m_iCurrentDumpSeq = 0;
                        SAY("Now all block queue is cleaned.\n");
                        break;
                    }
                }
                else
                {
                    iTempRet = m_stSocket.SendOneCode(nCodeLength, abyCodeBuf);
                    if( iTempRet == 0 )
                    {
                        iRedoCount++;
                        SAY("Redo one code OK.\n");
                    }
                    else
                    {
                        LOG("Resend one code failed of %d.\n", iTempRet);
                        break;
                    }
                }    //end if code length == 0
            }    //end for
        } //end if(pRedoQueue)
    }

#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif

    return iRedoCount;
}
int CTCPConn::SetConnState(int iConnState)
{
#ifdef _POSIX_MT_
    pthread_mutex_lock( &m_stMutex );
#endif

    m_iConnState = iConnState;

#ifdef _POSIX_MT_
    pthread_mutex_unlock( &m_stMutex );
#endif

    return 0;
}
void CTCPConn::CreateCodeQueue(CCodeQueue* &pCodeQueue, unsigned char* &pbyBuffer)
{
    int iBufferSize = CCodeQueue::CountQueueSize(BLOCKQUEUESIZE) + sizeof(CSharedMem);
	pbyBuffer = new unsigned char[iBufferSize];

	CSharedMem::pbCurrentShm = pbyBuffer;
	//CSharedMem* pSharedMem = new CSharedMem(0, iBufferSize, 1);

    printf("austin error\n");
	//CCodeQueue::pCurrentShm = pSharedMem;
	//pCodeQueue = new CCodeQueue(BLOCKQUEUESIZE);
}

void CTCPConn::DeleteCodeQueue(CCodeQueue* &pCodeQueue, unsigned char* &pbyBuffer)
{
    delete pCodeQueue;
    pCodeQueue = NULL;
    delete[] pbyBuffer;
    pbyBuffer = NULL;
}
