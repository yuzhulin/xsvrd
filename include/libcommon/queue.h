#ifndef _QUEUE_
#define _QUEUE_

class CObjMng;
class CObj;

class CQueue
{
public:
	CQueue();
	CQueue(CObjMng *pObjSeg, int nIdxForNext);
	~CQueue();

	int Initialize(CObjMng *pObjSeg, int nIdxForNext);
	int Resume(CObjMng *pObjSeg);

	int	PeekHeadItem();
	int	TakeHeadItem();
	int	GetNextItem(int nItemIdx);
	int	SetNextItem(int nItemIdx, int nNextItemIdx);
	int	AppendItemToTail( int nItemIdx );
	int	InsertItemToHead( int nItemIdx );
	int	InsertItem( int nPrevItemIdx, int nItemIdx );
	int	DeleteItem( int nItemIdx );
	int		SearchItemFirst(void *pCondition, int *pnResult);
	int		SearchItemAll(void *pCondition, int *pnResult);
	int		TravelAll();
	void	SetJudgeFunction(int (*pfJudge)(CObj *pObj, void *pCondition));
	void	SetAccessFunction(int (*pfAccess)(CObj *pObj));
protected:
	int 	m_nHeadItem;
	int 	m_nTailItem;
	int (*m_pfnIsMatchObj)(CObj *pObj, void *pCondition);
	int (*m_pfnAccessItem)(CObj *pObj);
private:
	int m_nIdxForNext;
	CObjMng *m_pObjSeg;
};

#endif
