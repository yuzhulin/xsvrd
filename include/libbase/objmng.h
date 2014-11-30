#ifndef _OBJMNG_H_
#define _OBJMNG_H_

#include "../os.h"
#include <stdio.h>

#define MAXDSINFOITEM 4
#define IDXNULL       -1
#define DECLARE_DYN  public:    static CObj* CreateObject(void* pMem);

#define IMPLEMENT_DYN(class_name)  CObj* class_name::CreateObject( void *pMem ) {    CObj::pCurrentObj = (unsigned char *)pMem;    return (CObj *)(new class_name); }


class CIdx;
class CObjMng;
class CObj;
class CSharedMem;

class CObjMng
{
public:
	CObjMng(size_t nObjSize, int iItemCount, CObj* (*pfCreateObj)(void *));
	~CObjMng();


	int		Initialize();
	int		FormatObj();						//格式化对象区
	int		FormatIdx();						//格式化索引区
	int		RecoverIdx();						//恢复索引区
	int		CreateObject();
	int		DestroyObject(int iIdx);
	int     GetItemCount();
	int		GetUsedHead();
	int		GetFreeHead();
	int		GetUsedCount();

	CObj* GetObj(int iIdx);
	CIdx* GetIdx(int iIdx);



	void* operator new( size_t nSize );
	void  operator delete( void *pSeg );

	static CSharedMem* pCurrentShm;
	static size_t CountSegSize( size_t nObjSize, int iItemCount );

private:

	int			m_iIsFormated;
	size_t		m_nObjSize;
	CIdx*		m_pIdxs;
	CObj*		m_pObjs;
	int			m_iItemCount;
	int			m_iFreeHead;
	int			m_iUsedHead;
	int			m_iUsedCount;
	CObj*		(*m_pfNew)(void *);
};

class CIdx
{
public:
	CIdx();
	~CIdx();
	void Initialize();
	void SetFree();
	void SetUsed();
	int		IsUsed();
	int		GetNextIdx();
	void	SetNextIdx(int iIdx);
	int		GetPrevIdx();
	void	SetPrevIdx(int iIdx);
	int		GetDsInfo( int nDsIdx, int *piDsVal );
	int		SetDsInfo( int nDsIdx, int iDsVal );
	int     ClearDsInfo();
	CObj*	GetAttachedObj();
	int		SetAttachedObj( CObj *pObj );

protected:
	int		m_iUseFlag;
	int		m_iNextIdx;					//对象索引块链表指针，指向后一个闲/忙索引
	int		m_iPrevIdx;					//对象索引块链表指针，指向前一个闲/忙索引
	int		m_piDsInfo[MAXDSINFOITEM];

private:
	CObj *m_pAttachedObj;
};

class CObj
{
public:
    virtual ~CObj(){};
	virtual int Initialize() = 0;
	virtual int GetHashKey(void *pvKey, int& iKeyLength) = 0;
	virtual int SetHashKey(const void *pvKey, int iKeyLength) = 0;
    virtual int Show(FILE *fpOut) = 0;


	int GetObjectID();

	static int  nCreateMode;

protected:
    static void* operator new( size_t nSize );
    static void  operator delete( void *pMem );
    static unsigned char *pCurrentObj;

private:
	int m_iObjectID;

	friend int CObjMng::FormatObj();

	//DECLARE_DYN
};

#endif
