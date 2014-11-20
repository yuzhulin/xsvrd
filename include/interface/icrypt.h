/******************************************************************************

��Ȩ���� (C), 2001-2011, ��Ѷ��˾

******************************************************************************
�� �� ��   : IMd5Hash.h
�� �� ��   : ����
��    ��   : �߷�
��������   : 2006��10��23��
����޸�   :
��������   : MD5�Ľӿ��ļ�
�����б�   :
�޸���ʷ   :
1.��    ��   : 2006��10��23��
��    ��   : �߷�
�޸�����   : �����ļ�

******************************************************************************/

#ifndef _CRYPT_H_
#define _CRYPT_H_


#ifndef WIN32
#define CRYPT_NAMESPACE_BEGIN namespace mycrypt{
#define CRYPT_NAMESPACE_END   }
#define USING_CRYPT_NAMESPACE using namespace mycrypt;
#else
#define CRYPT_NAMESPACE_BEGIN 
#define CRYPT_NAMESPACE_END   
#define USING_CRYPT_NAMESPACE
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif


#ifndef WIN32

#include <stdio.h>
#include <dlfcn.h>

typedef unsigned char byte;
typedef unsigned char BYTE;
typedef int  BOOL;
#else

#endif

class ICrypt
{
public:
	virtual ~ICrypt(){};
	virtual int  GetEncryptLen(int nInBufLen) = 0;
	virtual void Encrypt(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen) = 0;
	virtual BOOL Decrypt(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen) = 0;
};


extern "C" 
{
	ICrypt* CreateCrypt();
}

typedef ICrypt* (*CreateCryptProc)();
#ifndef WIN32
typedef void* HMODULE;
#endif    

static ICrypt* CreateCryptPtr(char szDllFile[256], HMODULE hModule)
{
	char szFunctionName[100] = "CreateCrypt";
	CreateCryptProc pfCreateCrypt;

#ifndef WIN32	
	void *pDllHandle = dlopen(szDllFile, RTLD_NOW);
	char *pcError = dlerror();
	if( pcError )
	{
		printf("Open Dll %s failed, %s\n", szDllFile, pcError);
		return NULL;
	}

	pfCreateCrypt = (CreateCryptProc)dlsym(pDllHandle, szFunctionName);
	pcError = dlerror();
	if( pcError )
	{
		dlclose(pDllHandle);
		printf("dlsym failed, %s\n", pcError);
		return NULL;
	}
#else
	HMODULE pDllHandle = ::LoadLibrary(szDllFile);
	if(pDllHandle != NULL)
	{	
		pfCreateCrypt = (CreateCryptProc)GetProcAddress(
			pDllHandle, szFunctionName);

		if( !pfCreateCrypt )
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
	return (*pfCreateCrypt)();    
}

class CCryptWrap : public ICrypt
{
public:
	CCryptWrap(char szDllFile[256])
	{
		m_hDllHandle = NULL;
		strcpy(m_szDllName, szDllFile);
		m_pCrypt = CreateCryptPtr(szDllFile, m_hDllHandle);
	}

	virtual ~CCryptWrap()
	{
		delete m_pCrypt;
		m_pCrypt = NULL;

		//ж��dll
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

	ICrypt* GetCrypt()
	{
		return m_pCrypt;
	}

	virtual int  GetEncryptLen(int nInBufLen)
	{
		if( m_pCrypt )
		{
			return m_pCrypt->GetEncryptLen(nInBufLen);
		}
		else
		{
			return 0;
		}
	}

	virtual void Encrypt(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen) 
	{
		if( m_pCrypt )
		{
			m_pCrypt->Encrypt(pInBuf, nInBufLen, 
				pKey, pOutBuf, pOutBufLen);
		}
	}
	virtual BOOL Decrypt(const BYTE* pInBuf, int nInBufLen, 
		const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
	{
		if( m_pCrypt )
		{
			return m_pCrypt->Decrypt(pInBuf, nInBufLen, 
				pKey, pOutBuf, pOutBufLen);
		}
		else
		{
			return FALSE;
		}
	}

private:
	ICrypt* m_pCrypt; 
	char m_szDllName[256];
	HMODULE m_hDllHandle;
};


#endif
