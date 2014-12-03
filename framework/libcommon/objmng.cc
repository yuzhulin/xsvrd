#include "win_linux.h"
#include "objmng.h"
#include "shm.h"

CSharedMem* CObjMng::pCurrentShm = NULL;

unsigned char* CObj::pCurrentObj = NULL;
int CObj::nCreateMode = 0;

void* CObj::operator new( size_t nSize )
{
    if( !pCurrentObj )
    {
        return (void*)NULL;
    }

    return (void *)pCurrentObj;
}

void  CObj::operator delete( void *pMem )
{
    return;
}

int CObj::GetObjectID()
{
	return m_iObjectID;
}

void* CObjMng::operator new(size_t nSize)
{
	char *pTemp;

	if( !pCurrentShm )
	{
		return (void*)NULL;
	}

	pTemp = (char *)(pCurrentShm->CreateSegment(nSize));

	return (void *)pTemp;
}

void CObjMng::operator delete( void *pMem )
{
	return;
}

size_t CObjMng::CountSegSize(size_t nObjSize, int iItemCount)
{
	size_t iTempSize = 0;

	iTempSize += sizeof(CObjMng);
	iTempSize += iItemCount*sizeof(CIdx);
	iTempSize += iItemCount*nObjSize;

	return iTempSize;
}

CObjMng::CObjMng(size_t nObjSize, int iItemCount, CObj* (*pfCreateObj)(void *))
{
	if( !pCurrentShm )
	{
		return;
	}

	if( pCurrentShm->GetInitMode() == Init )
	{
		m_nObjSize = nObjSize;
		m_iItemCount = iItemCount;
	}

	m_pIdxs = (CIdx*)(pCurrentShm->CreateSegment(m_iItemCount*sizeof(CIdx)));
	if (!m_pIdxs)
	{
		return;
	}
	m_pObjs = (CObj*)(pCurrentShm->CreateSegment(m_iItemCount*m_nObjSize));
	if (!m_pObjs)
	{
		return;
	}

	m_pfNew = pfCreateObj;

	if( pCurrentShm->GetInitMode() == Init )
	{
		Initialize();
		CObj::nCreateMode = 0;
	}
	else
	{
		RecoverIdx();
		CObj::nCreateMode = 1;
	}

	FormatObj();
}

CObjMng::~CObjMng()
{
}

int CObjMng::Initialize()
{
	m_iIsFormated = 0;
	m_iFreeHead = -1;
	m_iUsedHead = -1;
	m_iUsedCount = 0;


	if( FormatIdx() )
	{
		return -1;
	}


	m_iIsFormated = 1;

	return 0;
}

int CObjMng::FormatObj()
{
	CObj *pTempObj;

	if( !m_pfNew )
	{
		return -1;
	}

	for( int i = 0 ; i < m_iItemCount; i++ )
	{
		pTempObj = (*m_pfNew)((void *)((char*)m_pObjs + m_nObjSize*i));
		pTempObj->m_iObjectID = i;
		m_pIdxs[i].SetAttachedObj(pTempObj);
	}

	return 0;
}
int CObjMng::FormatIdx()
{
	m_iFreeHead = 0;
	m_iUsedHead = -1;
	int i;

	m_pIdxs[m_iFreeHead].Initialize();
	m_pIdxs[m_iFreeHead].SetPrevIdx(-1);

	for( i = 1 ; i < m_iItemCount; i++ )
	{
		m_pIdxs[i].Initialize();
		m_pIdxs[i-1].SetNextIdx(i);
		m_pIdxs[i].SetPrevIdx(i-1);
	}

	m_pIdxs[m_iItemCount-1].SetNextIdx(-1);


	return 0;
}

int CObjMng::RecoverIdx()
{
	return 0;
}

int CObjMng::GetItemCount()
{
    return m_iItemCount;
}
int CObjMng::GetUsedCount()
{
	return m_iUsedCount;
}

int CObjMng::GetUsedHead()
{
	return m_iUsedHead;
}
int CObjMng::GetFreeHead()
{
	return m_iFreeHead;
}

CIdx* CObjMng::GetIdx(int iIdx)
{
	if( iIdx < 0 || iIdx >= m_iItemCount )
	{
		return NULL;
	}

	if( !m_pIdxs[iIdx].IsUsed() )
	{
		return NULL;
	}

	return &m_pIdxs[iIdx];
}

CObj* CObjMng::GetObj(int iIdx)
{
	if( iIdx < 0 || iIdx >= m_iItemCount )
	{
		return NULL;
	}

	if( !m_pIdxs[iIdx].IsUsed() )
	{
		return NULL;
	}

	return m_pIdxs[iIdx].GetAttachedObj();
}

int CObjMng::CreateObject()
{
	int iTempIdx;

	if( m_iUsedCount >= m_iItemCount )
	{
		return -1;
	}

	iTempIdx = m_iFreeHead;
	m_iFreeHead = m_pIdxs[iTempIdx].GetNextIdx();
	if( m_iFreeHead >= 0 )
	{
		m_pIdxs[m_iFreeHead].SetPrevIdx(-1);
	}

	m_pIdxs[iTempIdx].SetUsed();
	m_pIdxs[iTempIdx].SetNextIdx(m_iUsedHead);
	m_pIdxs[iTempIdx].SetPrevIdx(-1);
	if( m_iUsedHead >= 0 )
	{
		m_pIdxs[m_iUsedHead].SetPrevIdx(iTempIdx);
	}
	m_iUsedHead = iTempIdx;

	m_pIdxs[iTempIdx].ClearDsInfo();
	m_pIdxs[iTempIdx].GetAttachedObj()->Initialize();

	m_iUsedCount++;

	return iTempIdx;
}

int CObjMng::DestroyObject(int iIdx)
{
	if( iIdx >= m_iItemCount || iIdx < 0 || m_iItemCount <= 0 )
	{
		return -1;
	}

	if( !m_pIdxs[iIdx].IsUsed() )
	{
		return -2;
	}

	//从使用队列中删除
	if( iIdx == m_iUsedHead )
	{
		m_iUsedHead = m_pIdxs[iIdx].GetNextIdx();
	}
	if( m_pIdxs[iIdx].GetNextIdx() >= 0 )
	{
		m_pIdxs[m_pIdxs[iIdx].GetNextIdx()].SetPrevIdx(m_pIdxs[iIdx].GetPrevIdx());
	}
	if( m_pIdxs[iIdx].GetPrevIdx() >= 0 )
	{
		m_pIdxs[m_pIdxs[iIdx].GetPrevIdx()].SetNextIdx(m_pIdxs[iIdx].GetNextIdx());
	}

	//挂入空闲队列待分配
	m_pIdxs[iIdx].SetFree();
	m_pIdxs[iIdx].SetPrevIdx(-1);
	m_pIdxs[iIdx].SetNextIdx(m_iFreeHead);
	if( m_iFreeHead >= 0 )
	{
		m_pIdxs[m_iFreeHead].SetPrevIdx(iIdx);
	}
	m_iFreeHead = iIdx;

	m_iUsedCount--;

	return iIdx;
}


CIdx::CIdx()
{
	Initialize();
}

CIdx::~CIdx()
{
}

void CIdx::Initialize()
{
	m_iNextIdx = -1;
	m_iPrevIdx = -1;


	m_iUseFlag = 0;

	m_pAttachedObj = NULL;
    ClearDsInfo();

}

int CIdx::GetNextIdx()
{
	return m_iNextIdx;
}

void CIdx::SetNextIdx(int iIdx)
{
	m_iNextIdx = iIdx;
	return;
}

int CIdx::GetPrevIdx()
{
	return m_iPrevIdx;
}

void CIdx::SetPrevIdx(int iIdx)
{
	m_iPrevIdx = iIdx;
	return;
}

void CIdx::SetUsed()
{
	m_iUseFlag = 1;
	return;
}
void CIdx::SetFree()
{
	m_iUseFlag = 0;
	return;
}
int CIdx::IsUsed()
{
	return m_iUseFlag;
}

CObj* CIdx::GetAttachedObj()
{
	return m_pAttachedObj;
}

int CIdx::SetAttachedObj(CObj *pObj)
{
	if( !pObj )
	{
		return -1;
	}

	m_pAttachedObj = pObj;

	return 0;
}

int CIdx::GetDsInfo(int nDsIdx, int *piDsVal)
{
	if( nDsIdx < 0 || nDsIdx >= MAXDSINFOITEM || piDsVal == NULL )
	{
		return -1;
	}
	*piDsVal = m_piDsInfo[nDsIdx];
	return 0;
}

int CIdx::SetDsInfo(int nDsIdx, int iDsVal)
{
	if( nDsIdx < 0 || nDsIdx >= MAXDSINFOITEM )
	{
		return -1;
	}

	m_piDsInfo[nDsIdx] = iDsVal;
	return 0;
}

int CIdx::ClearDsInfo()
{
    int i;

    for( i = 0; i < MAXDSINFOITEM; i++ )
    {
        m_piDsInfo[i] = IDXNULL;
    }

    return 0;
}
