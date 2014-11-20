#include "queue.h"
#include "objmng.h"

CQueue::CQueue()
{
}

CQueue::CQueue(CObjMng *pObjSeg, int nIdxForNext)
{
    if( CObj::nCreateMode )
    {
        Resume(pObjSeg);
    }
    else
    {
        Initialize(pObjSeg, nIdxForNext);
    }
}

CQueue::~CQueue()
{
}

int CQueue::Initialize(CObjMng *pObjSeg, int nIdxForNext)
{
    if( !pObjSeg )
    {
        return -1;
    }
    
    if( nIdxForNext < 0 || nIdxForNext >= MAXDSINFOITEM )
    {
        return -1;
    }
    
    m_nIdxForNext = nIdxForNext;
    m_pObjSeg = pObjSeg;
    
    m_nHeadItem = -1;
    m_nTailItem = -1;

    m_pfnIsMatchObj = NULL;
    m_pfnAccessItem = NULL;

    return 0;
}

int CQueue::Resume(CObjMng *pObjSeg)
{
    if( !pObjSeg )
    {
        return -1;
    }

    m_pObjSeg = pObjSeg;
    m_pfnIsMatchObj = NULL;
    m_pfnAccessItem = NULL;

    return 0;
}
int CQueue::GetNextItem(int nItemIdx)
{
    CIdx *pTempIdx = NULL;
    int  nTempNext = -1;

    if( !(pTempIdx=m_pObjSeg->GetIdx(nItemIdx)) )
    {
        return -2;
    }

    pTempIdx->GetDsInfo(m_nIdxForNext, &nTempNext);

    return nTempNext;
}

int CQueue::SetNextItem(int nItemIdx, int nNextItemIdx)
{
    CIdx *pTempIdx = NULL;

    if( !(pTempIdx=m_pObjSeg->GetIdx(nItemIdx)) )
    {
        return -1;
    }

    pTempIdx->SetDsInfo(m_nIdxForNext, nNextItemIdx);

    return 0;
}

int CQueue::AppendItemToTail(int nItemIdx)
{
    CIdx *pTempIdx = NULL;

    if( !(pTempIdx=m_pObjSeg->GetIdx(nItemIdx)) )
    {
        return -1;
    }

    SetNextItem(nItemIdx, -1);
    //pTempIdx->SetDsInfo(m_nIdxForNext, -1);

    if( m_nTailItem < 0 )
    {
        m_nHeadItem = nItemIdx;
        m_nTailItem = nItemIdx;
        return 0;
    }

    SetNextItem(m_nTailItem, nItemIdx);

    m_nTailItem = nItemIdx;

    return 0;
}

int CQueue::InsertItemToHead(int nItemIdx)
{
    CIdx *pTempIdx = NULL;

    if( !(pTempIdx=m_pObjSeg->GetIdx(nItemIdx)) )
    {
        return -1;
    }
    
    SetNextItem(nItemIdx, m_nHeadItem);

    if( m_nHeadItem < 0 )
    {
        m_nTailItem = nItemIdx;
    }

    m_nHeadItem = nItemIdx;
    
    return 0;
}

int CQueue::InsertItem(int nPrevItemIdx, int nItemIdx)
{
    CIdx *pTempIdx = NULL;
    CIdx *pTempPrevIdx = NULL;
    int nTempNext = -1;

    if( !(pTempIdx=m_pObjSeg->GetIdx(nItemIdx)) || !(pTempPrevIdx=m_pObjSeg->GetIdx(nPrevItemIdx)) )
    {
        return -1;
    }

    nTempNext = GetNextItem(nPrevItemIdx);

    SetNextItem(nItemIdx, nTempNext);
    SetNextItem(nPrevItemIdx, nItemIdx);

    return 0;
}

int CQueue::PeekHeadItem()
{
    return m_nHeadItem;
}

int CQueue::TakeHeadItem()
{
    int nTempItem;

    if( m_nHeadItem < 0 )
    {
        return -1;
    }

    nTempItem = m_nHeadItem;

    DeleteItem(m_nHeadItem);

    return nTempItem;
}

int CQueue::DeleteItem(int nItemIdx)
{
    CIdx *pTempIdx = NULL;
    CIdx *pTempPrev = NULL;
    CIdx *pTempNext = NULL;
    int   nTempPrev;

    if( !(pTempIdx=m_pObjSeg->GetIdx(nItemIdx)) )
    {
        return -1;
    }
    if( nItemIdx == m_nHeadItem )
    {
        m_nHeadItem = GetNextItem(nItemIdx);
        SetNextItem(nItemIdx, -1);
        
        if( m_nHeadItem < 0 )
        {
            m_nTailItem = m_nHeadItem;
        }
        return nItemIdx;
    }

    for( nTempPrev = m_nHeadItem; nTempPrev >= 0; nTempPrev = GetNextItem(nTempPrev) )
    {
        if( GetNextItem(nTempPrev) == nItemIdx )
        {
            break;
        }
    }

    if( nTempPrev < 0 )
    {
        return -2;
    }

    SetNextItem(nTempPrev, GetNextItem(nItemIdx));
    SetNextItem(nItemIdx, -1);
    if( nItemIdx == m_nTailItem )
    {
        m_nTailItem = nTempPrev;
    }

    return 0;
}

int CQueue::SearchItemAll(void *pCondition, int *pnResult)
{
    int nTempCount = 0;
    int nTempIdx = -1;
    CObj *pTempObj = NULL;

    if( !pCondition || !pnResult || !m_pfnIsMatchObj)
    {
        return -1;
    }

    for( nTempIdx = m_nHeadItem; nTempIdx >= 0; nTempIdx = GetNextItem(nTempIdx) )
    {
        pTempObj = m_pObjSeg->GetObj(nTempIdx);
        if( (*m_pfnIsMatchObj)(pTempObj, pCondition) )
        {
            pnResult[nTempCount] = nTempIdx;
            nTempCount++;
        }
    }

    return nTempCount;
}

int CQueue::SearchItemFirst(void *pCondition, int *pnResult)
{
    int nTempCount = 0;
    int nTempIdx = -1;
    CObj *pTempObj = NULL;

    if( !pCondition || !pnResult || !m_pfnIsMatchObj)
    {
        return -1;
    }

    for( nTempIdx = m_nHeadItem; nTempIdx >= 0; nTempIdx = GetNextItem(nTempIdx) )
    {
        pTempObj = m_pObjSeg->GetObj(nTempIdx);
        if( (*m_pfnIsMatchObj)(pTempObj, pCondition) )
        {
            pnResult[nTempCount] = nTempIdx;
            nTempCount++;
            break;
        }
    }

    return nTempCount;
}

void CQueue::SetJudgeFunction(int (*pfJudge)(CObj *, void *))
{
    m_pfnIsMatchObj = pfJudge;
}

void CQueue::SetAccessFunction(int (*pfAccess)(CObj *))
{
    m_pfnAccessItem = pfAccess;
}

int CQueue::TravelAll()
{
    int nTempIdx;

    for( nTempIdx = m_nHeadItem; nTempIdx >= 0; nTempIdx = GetNextItem(nTempIdx) )
    {
        (*m_pfnAccessItem)(m_pObjSeg->GetObj(nTempIdx));
    }

    return 0;
}
