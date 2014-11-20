#include "stdafx.h"
#include "../include/mywindows.h"
#include "../include/linuxtypes.h"
#include "../include/common.h"
#include "../include/base.h"
#include "../../dll/crypt/src/icrypt.h"
#include "../../dll/compress/src/icompress.h"

CCompressWrap g_compresswrap(COMPRESS_DLL);

bool LoadAndZipFile(const char* pszFileName,
					BYTE* pbyUnZipBuf, int& iUnzipLen,
					bool bShouldZip,
					BYTE* pbyZipBuf, int& iZipLen)
{
    bool bRet = LoadBinaryFile(pszFileName, pbyUnZipBuf, iUnzipLen);
	if( !bRet )
	{
		return bRet;
	}

	if( bShouldZip )
	{
		unsigned long ulOutLen = iZipLen;
		int	iZipResult = g_compresswrap.Compress(pbyZipBuf, &ulOutLen,
			                                     pbyUnZipBuf, iUnzipLen);
		if( iZipResult != 0 )
		{
			return false;
		}
		iZipLen = ulOutLen;
	}
	return true;
}
