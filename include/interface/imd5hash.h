/******************************************************************************

                  版权所有 (C), 2001-2011, 腾讯公司

 ******************************************************************************
  文 件 名   : IMd5Hash.h
  版 本 号   : 初稿
  作    者   : 高峰
  生成日期   : 2006年10月23日
  最近修改   :
  功能描述   : MD5的接口文件
  函数列表   :
  修改历史   :
  1.日    期   : 2006年10月23日
    作    者   : 高峰
    修改内容   : 创建文件

******************************************************************************/

#ifndef _IMD5_HASH_
#define _IMD5_HASH_

#ifndef WIN32
#define MD5_NAMESPACE_BEGIN namespace mymd5hash{
#define MD5_NAMESPACE_END   }
#define USING_MD5_NAMESPACE using namespace mymd5hash;
#else
#define MD5_NAMESPACE_BEGIN 
#define MD5_NAMESPACE_END   
#define USING_MD5_NAMESPACE 
#endif

#ifndef WIN32
#include <stdio.h>
#include <dlfcn.h>
typedef unsigned char BYTE;
typedef int           BOOL;
#include <stdio.h>
#include <string.h>
#endif

#ifndef HASH_LEN
#define HASH_LEN 16
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

class IMD5Hash
{
public:
	virtual ~IMD5Hash(){};
    virtual void Md5Hash( BYTE *outBuffer, const BYTE *inBuffer, int length) = 0;
	virtual BOOL GetFileHash(char *strLocalFile, BYTE byHash[HASH_LEN]) = 0;
};


extern "C" 
{
    IMD5Hash* CreateMD5Hash();
}

typedef IMD5Hash* (*CreateMD5HashProc)();

#ifndef WIN32
typedef void*     HMODULE;
#endif

static IMD5Hash* CreateMD5HashPtr(char szDllFile[256], HMODULE& hModule)
{
	CreateMD5HashProc pfCreateMD5Hash = NULL;

#ifndef WIN32	
    void *pDllHandle = dlopen(szDllFile, RTLD_NOW);
    char *pcError = dlerror();
    if( pcError )
    {
        printf("Open Dll %s failed, %s\n", szDllFile, pcError);
        return NULL;
    }
    pfCreateMD5Hash = (CreateMD5HashProc)dlsym(pDllHandle, "CreateMD5Hash");
    pcError = dlerror();
    if( pcError )
    {
		dlclose(pDllHandle);
        printf("DL sym failed, %s\n", pcError);
        return NULL;
    }
#else
	HMODULE pDllHandle = ::LoadLibrary(szDllFile);
	if(pDllHandle != NULL)
	{	
		pfCreateMD5Hash = (CreateMD5HashProc)GetProcAddress(
			pDllHandle, "CreateMD5Hash");
		if( !pfCreateMD5Hash )
		{
			FreeLibrary(pDllHandle);
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
#endif
	hModule = pDllHandle;
	
	IMD5Hash* pMD5HashPtr = pfCreateMD5Hash();
    return pMD5HashPtr;
}

class CMD5Wrap : public IMD5Hash
{
public:
	CMD5Wrap(char szDllFile[256])
	{
		m_hDllHandle = NULL;
		strcpy(m_szDllName, szDllFile);
        m_pMD5Hash = CreateMD5HashPtr(szDllFile, m_hDllHandle);
	}

	virtual ~CMD5Wrap()
	{
		delete m_pMD5Hash;
		m_pMD5Hash = NULL;

		//卸载dll
#ifdef WIN32
        if( m_hDllHandle )
		{
			FreeLibrary(m_hDllHandle);
			m_hDllHandle = NULL;
		}
#else
		if( m_hDllHandle )
		{
			dlclose(m_hDllHandle);
			m_hDllHandle = NULL;
		}
#endif
	}
    virtual void Md5Hash( BYTE *outBuffer, const BYTE *inBuffer, int length)
	{
		if( m_pMD5Hash )
		{
			m_pMD5Hash->Md5Hash(outBuffer, inBuffer, length);
		}
	}

	virtual BOOL GetFileHash(char *strLocalFile, BYTE byHash[HASH_LEN])
	{
		if( m_pMD5Hash )
		{
			return m_pMD5Hash->GetFileHash(strLocalFile, byHash);
		}
		else
		{
			return FALSE;
		}
	}

private:
	IMD5Hash* m_pMD5Hash; 
	char m_szDllName[256];
	HMODULE m_hDllHandle;
};


#endif
