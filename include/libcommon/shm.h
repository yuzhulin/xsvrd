#ifndef _SHM_HPP_
#define _SHM_HPP_

#ifndef WIN32
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#include "win_linux.h"

#define YEARSEC 31536000


typedef enum
{
	Init = 0,
	Recover = 1
} EIMode;

class CSharedMem
{
public:
	CSharedMem();
	CSharedMem(key_t nKey, int nSize, bool bInitFlag);
	~CSharedMem();


	int Initialize(key_t nKey, int nSize);
	void SetStamp();
	void* operator new( size_t nSize);
	void  operator delete(void *pMem);
	void*  CreateSegment( size_t nSize);
	static unsigned char* pbCurrentShm;
	EIMode GetInitMode();


private:
    void InitShm(key_t nKey, int nSize);

	key_t m_nShmKey;
	size_t m_nShmSize;
	time_t m_tmCreateTime;
	time_t m_tmLastStamp;
	unsigned int m_nCRC;
	unsigned char* m_pbCurrentSegMent;
	
	EIMode m_InitMode;
};


#endif
