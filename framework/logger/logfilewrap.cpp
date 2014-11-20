#ifdef WIN32
#include <windows.h>
#endif
#include "logfilewrap.h"
#include "logfile.h"
#include "ilogfile.h"

ICrypt* CreateCrypt()
{
    return new CEnDeCryptWrap;
}


CEnDeCryptWrap::CEnDeCryptWrap()
{
}

CEnDeCryptWrap::~CEnDeCryptWrap()
{
}

int CEnDeCryptWrap::GetEnlogfileLen(int nInBufLen)
{
	return oi_symmetry_enlogfile2_len(nInBufLen);
}

void CEnDeCryptWrap::Enlogfile(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{
	oi_symmetry_enlogfile2(pInBuf, nInBufLen, pKey, pOutBuf, pOutBufLen);
}

BOOL CEnDeCryptWrap::Delogfile(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{
	return oi_symmetry_delogfile2(pInBuf, nInBufLen,pKey, pOutBuf, pOutBufLen);
}

