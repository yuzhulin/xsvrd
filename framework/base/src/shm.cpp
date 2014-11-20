#ifndef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "signal.h"
#include "unistd.h"
#include "stdarg.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/ipc.h"
#include "sys/shm.h"
#include "sys/types.h"
#include "dirent.h"
#include "sys/stat.h"
#include "unistd.h"
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>

#else
#include <time.h>
#endif
#include "shm.h"

unsigned char* CSharedMem::pbCurrentShm = NULL;

void* CSharedMem::operator new( size_t nSize )
{
    unsigned char* pTemp = NULL;

    if(!pbCurrentShm )
    {
        return (void*)NULL;
    }

    pTemp = pbCurrentShm;

    //printf("****************pTemp %p \n", pTemp);
    return (void * )pTemp;
}
void CSharedMem::operator delete(void* pMem )
{
}

CSharedMem::CSharedMem()
{
    m_pbCurrentSegMent = pbCurrentShm + sizeof(CSharedMem);
}

void CSharedMem::InitShm(key_t nKey, int nSize )
{
    time_t tNow;
    unsigned int nTempInt = 0;

    m_pbCurrentSegMent = pbCurrentShm + sizeof(CSharedMem);

	nTempInt = (unsigned int )m_nShmKey ^ (unsigned int )m_nShmSize ^ (unsigned int )m_tmCreateTime ^ (unsigned int )m_tmLastStamp ^ (unsigned int )m_nCRC;

	if( nTempInt != 0 )
	{
		Initialize(nKey, nSize);
		return;
	}
	if( ! (nKey == m_nShmKey && nSize == (int)m_nShmSize) )
	{
		Initialize(nKey, nSize);
		return;
	}

	time( &tNow);

	if(!((tNow - m_tmCreateTime ) > 0 && (tNow - m_tmCreateTime ) < 10*31536000 ) )
	{
		Initialize(nKey, nSize);
		return;
	}
	if(tNow - m_tmLastStamp > 3600 )
	{
        Initialize(nKey, nSize);
        return;
    }

	m_InitMode = Recover;
    SetStamp();
}

CSharedMem::CSharedMem(key_t nKey, int nSize, bool bInitFlag )
{
    if( bInitFlag )
    {
        m_pbCurrentSegMent = pbCurrentShm + sizeof(CSharedMem);
        Initialize(nKey, nSize);
    }
    else
    {
        InitShm(nKey, nSize);
    }
}

CSharedMem::~CSharedMem()
{
}

int CSharedMem::Initialize(key_t nKey, int nSize )
{
	m_InitMode = Init;

    time_t tNow;

    m_nShmKey = nKey;
    m_nShmSize = nSize;

    time( &tNow);
    m_tmCreateTime = tNow;
    m_tmLastStamp = tNow;

    m_nCRC = (unsigned int )m_nShmKey ^ (unsigned int )m_nShmSize ^ (unsigned int )m_tmCreateTime ^ (unsigned int )m_tmLastStamp;
    return 0;
}

void* CSharedMem::CreateSegment( size_t nSize )
{
    int nTempUsedLength = 0;
    unsigned char* pTemp;

    if(nSize <= 0 )
    {
        return NULL;
    }

    nTempUsedLength = ( int )(m_pbCurrentSegMent - (unsigned char * )this);
    if(m_nShmSize - nTempUsedLength < nSize )
    {
        return NULL;
    }

    pTemp = m_pbCurrentSegMent;
    m_pbCurrentSegMent += nSize;

    return (void * )pTemp;
}

void CSharedMem::SetStamp()
{
    time_t tNow;

    time( &tNow);

    m_tmLastStamp = tNow;
    m_nCRC = (unsigned int )m_nShmKey ^ (unsigned int )m_nShmSize ^ (unsigned int )m_tmCreateTime ^ (unsigned int )m_tmLastStamp;
}

EIMode CSharedMem::GetInitMode()
{
	return m_InitMode;
}
