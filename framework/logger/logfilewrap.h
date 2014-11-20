
#ifndef _CRYPTWRAP_H_
#define _CRYPTWRAP_H_

#include "ilogfile.h"

class CEnDeCryptWrap : public ICrypt 
{
public:
    CEnDeCryptWrap();
    virtual ~CEnDeCryptWrap();

	virtual int  GetEncryptLen(int nInBufLen);
	virtual void Encrypt(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);
	virtual BOOL Decrypt(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);
};

#endif
