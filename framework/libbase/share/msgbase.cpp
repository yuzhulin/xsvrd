#include "stdafx.h"
#include "../include/mywindows.h"
#include "../include/linuxtypes.h"
#include "../include/base.h"
#include "msgbase.h"
#include "msg.h"
#include "msgservers.h"

FILE* g_fpOutPut = NULL;


unsigned long CMsgPara::ulCreateTimes = 0;
unsigned long CMsgPara::ulDestroyTimes = 0;

void xprintf(const char *szFormat, ...)
{
    va_list ag;

    if ( !g_fpOutPut )
    {
        return ;
    }
    va_start( ag, szFormat );
    vfprintf(g_fpOutPut, szFormat, ag);
    fflush(g_fpOutPut);
    va_end( ag );
    return;
}

void PrintBin(char *pBuffer, int iLength)
{
}

CMsgHead::CMsgHead()
{
    m_sMsgID = -1;
    m_sMsgType = -1;
    m_iMsgSeq = -1;
    m_bSrcFE = (unsigned char)0xff;
    m_bDstFE = (unsigned char)0xff;
    m_sSrcID = -1;
    m_sDstID = -1;
}

CMsgHead::~CMsgHead()
{
}

int CMsgHead::Encode(char *pcOutBuffer, int &sOutLength)
{
    char *pcTemp;
    int sTempLength;

    if ( !pcOutBuffer )
    {
        return -1;
    }

    pcTemp = pcOutBuffer;
    sOutLength = 0;

    sTempLength = EncodeShort( &pcTemp, (unsigned short)m_sMsgID );
    sOutLength += sTempLength;

    sTempLength = EncodeShort( &pcTemp, (unsigned short)m_sMsgType );
    sOutLength += sTempLength;

    sTempLength = EncodeInt( &pcTemp, (unsigned int)m_iMsgSeq );
    sOutLength += sTempLength;

    sTempLength = EncodeChar( &pcTemp, (unsigned char)m_bSrcFE );
    sOutLength += sTempLength;

    sTempLength = EncodeChar( &pcTemp, (unsigned char)m_bDstFE );
    sOutLength += sTempLength;

    sTempLength = EncodeShort( &pcTemp, (unsigned short)m_sSrcID );
    sOutLength += sTempLength;

    sTempLength = EncodeShort( &pcTemp, (unsigned short)m_sDstID );
    sOutLength += sTempLength;

    return 0;
}
int CMsgHead::Decode(const char *pcInBuffer, const int sInLength)
{
    char *pcTemp;
    int sLeftLength;
    int sTempLength;

    if ( !pcInBuffer || sInLength <= 0 )
    {
        return -1;
    }

    pcTemp = (char *)pcInBuffer;
    sLeftLength = sInLength;

    sTempLength = DecodeShort( &pcTemp, (unsigned short *) & m_sMsgID );
    sLeftLength -= sTempLength;

    sTempLength = DecodeShort( &pcTemp, (unsigned short *) & m_sMsgType );
    sLeftLength -= sTempLength;

    sTempLength = DecodeInt( &pcTemp, (unsigned int *) & m_iMsgSeq );
    sLeftLength -= sTempLength;

    sTempLength = DecodeChar( &pcTemp, (unsigned char *) & m_bSrcFE );
    sLeftLength -= sTempLength;

    sTempLength = DecodeChar( &pcTemp, (unsigned char *) & m_bDstFE );
    sLeftLength -= sTempLength;

    sTempLength = DecodeShort( &pcTemp, (unsigned short *) & m_sSrcID );
    sLeftLength -= sTempLength;

    sTempLength = DecodeShort( &pcTemp, (unsigned short *) & m_sDstID );
    sLeftLength -= sTempLength;

    if ( sLeftLength < 0 )
    {
        return -1;
    }

    return 0;
}

void CMsgHead::Print()
{
}

CMsg::CMsg()
{
	m_bDelete  = true;
    m_pMsgPara = NULL;
}
CMsg::~CMsg()
{
    if ( m_pMsgPara && m_bDelete )
    {
        delete m_pMsgPara;
        m_pMsgPara = NULL;
    }
}

CMsgPara* CMsg::CreateServersMsgPara(short sMsgID, short sMsgType)
{
    switch ( sMsgType )
    {
    case Request:
        {
            switch ( sMsgID )
            {
			case ID_SMD_GETPLAYERSECRETINFO:
				{
					return new CRequestPlayerSecretPara;
				}
			default:
                {
                    return NULL;
                }
            }
            break;
        }
    case Response:
        {
            switch ( sMsgID )
            {
			case ID_SMD_GETPLAYERSECRETINFO:
				{
					return new CResponsePlayerSecretPara;
				}
			default:
				{
					return NULL;
				}
            }
            break;
        }
    case Notify:
        {
            switch ( sMsgID )
            {
				default:
                {
                    return NULL;
                }
            }
            break;
        }
    case Internal:
        {
            switch ( sMsgID )
            {
            case INTER_TIMEOUT:
                {
                    return (CMsgPara *)new CInterTimeOutPara;
                }
            default:
                {
                    return NULL;
                }
            }
        }       
    case Ack:
        {
           return NULL;
        }
    default:
        {
            return NULL;
        }
    }
    return NULL;
}

CMsgPara* CMsg::CreateClientServerMsgPara(short sMsgID, short sMsgType)
{

    return NULL;
}

int CMsg::CreatePara()
{
    if ( m_pMsgPara )
    {
        delete m_pMsgPara;
        m_pMsgPara = NULL;
    }
	if( m_stHead.m_sMsgID >= ID_INTERSERVERS_BASEMSGID )
	{
		m_pMsgPara = CreateServersMsgPara( m_stHead.m_sMsgID, m_stHead.m_sMsgType );
	}
	else
	{
		m_pMsgPara = CreateClientServerMsgPara( m_stHead.m_sMsgID, m_stHead.m_sMsgType );
	}

    if ( !m_pMsgPara )
    {
        return -1;
    }
    return 0;
}

int CMsg::Encode(char *pcOutBuffer, int &sOutLength)
{
	int iMaxLength = sOutLength;

    char *pcTemp;
    int sTempLength;
    if ( !pcOutBuffer )
    {
        return -1;
    }

    pcTemp = pcOutBuffer;
    sOutLength = 0;

    if ( m_stHead.Encode(pcTemp, sTempLength) )
    {
        return -1;
    }

    sOutLength += sTempLength;
    pcTemp += sTempLength;

    if ( !m_pMsgPara )
    {
        return 0;
    }

	sTempLength = iMaxLength - sTempLength;
    if ( m_pMsgPara->Encode(pcTemp, sTempLength) )
    {
        return -1;
    }

    sOutLength += sTempLength;
    pcTemp += sTempLength;

    return 0;
}
int CMsg::Decode(const char *pcInBuffer, const int sInLength)
{
    char *pcTemp;
    int sLeftLength;

    if ( !pcInBuffer || sInLength <= 0 )
    {
        return -1;
    }

    pcTemp = (char *)pcInBuffer;
    sLeftLength = sInLength;

    if ( m_stHead.Decode((const char *)pcTemp, (const int)sLeftLength) )
    {
        return -2;
    }

    sLeftLength -= MSGHEADLENGTH;
    pcTemp += MSGHEADLENGTH;

    if ( sLeftLength <= 0 )
    {
        if ( m_pMsgPara )
        {
            delete m_pMsgPara;
            m_pMsgPara = NULL;
        }
        if ( sLeftLength == 0 )
        {
            return 0;
        }
        else
        {
            return -2;
        }
    }

    if ( CreatePara() )
    {
        return -3;
    }

    if ( m_pMsgPara->Decode((const char *)pcTemp, (const int)sLeftLength) )
    {
        return -4;
    }

    return 0;
}

void CMsg::Print()
{
    m_stHead.Print();

    if ( m_pMsgPara )
    {
        m_pMsgPara->Print();
    }
}

#ifdef _MULTHREAD_MSG_	
unsigned char CMsgPara::ms_byMsgAllocFlag[MAX_MSG_COUNT] = {0};
unsigned char CMsgPara::ms_byBuffer[MAX_MSG_COUNT][MAX_MSG_BUF_LEN];

#ifndef WIN32
pthread_mutex_t CMsgPara::ms_Mutex = PTHREAD_MUTEX_INITIALIZER;
#else
pthread_mutex_t CMsgPara::ms_Mutex;
void CMsgPara::InitMutex()
{
	pthread_mutex_init(&CMsgPara::ms_Mutex, NULL);
}
#endif

#endif

void* CMsgPara::operator new(size_t nSize)
{
#ifndef _MULTHREAD_MSG_	
    static unsigned char s_byBuffer[MAX_MSG_BUF_LEN];
	return (void*)s_byBuffer;
#else    
	int i;
	void* pBuffer = NULL;
    pthread_mutex_lock(&CMsgPara::ms_Mutex);
	for( i = 0; i < MAX_MSG_COUNT; i++ )
	{
		if( ms_byMsgAllocFlag[i] == 0 )
		{
			pBuffer = ms_byBuffer[i];
			ms_byMsgAllocFlag[i] = 1; 
			break;
		}
	}
    pthread_mutex_unlock(&CMsgPara::ms_Mutex);
	return pBuffer;
#endif
}

void CMsgPara::operator delete(void *pMem)
{
#ifdef _MULTHREAD_MSG_	
	int i;
    pthread_mutex_lock(&CMsgPara::ms_Mutex);
	for( i = 0; i < MAX_MSG_COUNT; i++ )
	{
		if( ms_byBuffer[i] == pMem )
		{
			ms_byMsgAllocFlag[i] = 0; 
			break;
		}
	}
    pthread_mutex_unlock(&CMsgPara::ms_Mutex);
#endif
}

CMsgPara::CMsgPara()
{
    ulCreateTimes++;
}
CMsgPara::~CMsgPara()
{
    ulDestroyTimes++;
}


