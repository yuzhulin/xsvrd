#ifndef _CRYPTHELPER_H_
#define _CRYPTHELPER_H_

extern bool LoadAndZipFile(const char* pszFileName,
					BYTE* pbyUnZipBuf, int& iUnzipLen,
					bool bShouldZip,
					BYTE* pbyZipBuf, int& iZipLen);


#endif

