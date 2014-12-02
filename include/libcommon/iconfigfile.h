/******************************************************************************

                  版权所有 (C), 2001-2011, 金山公司

 ******************************************************************************
  文 件 名   : iconfigfile.h
  版 本 号   : 初稿
  作    者   : 高峰
  生成日期   : 2006年10月23日
  最近修改   :
  功能描述   : 读写configfile的接口文件
  函数列表   :
  修改历史   :
  1.日    期   : 2006年10月23日
    作    者   : 高峰
    修改内容   : 创建文件

******************************************************************************/

#ifndef _ICONFIGFILE_H_
#define _ICONFIGFILE_H_


#ifndef WIN32
#define CONFIGFILE_NAMESPACE_BEGIN namespace myconfigfile{
#define CONFIGFILE_NAMESPACE_END   }
#define USING_CONFFILE_NAMESPACE   using namespace myconfigfile;
#else
#define CONFIGFILE_NAMESPACE_BEGIN 
#define CONFIGFILE_NAMESPACE_END   
#define USING_CONFFILE_NAMESPACE   
#endif

#ifndef _WIN32

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#define MAX_PATH  260
#else
#include <WinSock2.h>
#include <Windows.h>
#endif

class IConfigFile
{
public:
	virtual ~IConfigFile(){};
	virtual int GetServerVersion(){return 1;};

    virtual bool GetItemValueShort( const char *pszSectionName, 
                        const char *pszKeyName, 
                        short &shReturnedValue,
						short shDefaultValue) = 0;    
    virtual bool GetItemValueInt( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue,
                        int lDefaultValue ) = 0;
    virtual bool GetItemValueString( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize, 
                        const char *pszDefaultValue) = 0;
};


extern "C" 
{
    IConfigFile* CreateConfigFile(const char* pszFileName);
}

typedef IConfigFile* (*CreateConfigFileProc)(const char* pszFileName);

#ifndef _WIN32
typedef void* HMODULE;
#endif    

static IConfigFile* CreateConfigFilePtr(char szDllFile[MAX_PATH], const char* pszFileName, HMODULE& hModule)
{
    char szFunctionName[100] = "CreateConfigFile";
	CreateConfigFileProc pfCreateConfigFile = NULL;
                    
#ifndef _WIN32	
    void *pDllHandle = dlopen(szDllFile, RTLD_NOW);
    char *pcError = dlerror();
    if( pcError )
    {
        printf("open dll %s failed, %s\n", szDllFile, pcError);
        return NULL;
    }

    pfCreateConfigFile = (CreateConfigFileProc)dlsym(pDllHandle, szFunctionName);
    pcError = dlerror();            
    if( pcError )
    {
        printf("dlsym failed, %s\n", pcError);
		dlclose(pDllHandle);
        return NULL;
    }
#else
	HMODULE pDllHandle = ::LoadLibrary(szDllFile);
	if(pDllHandle != NULL)
	{	
		pfCreateConfigFile = (CreateConfigFileProc)GetProcAddress(
			pDllHandle, szFunctionName);	
		if( !pfCreateConfigFile )
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
	
	return (*pfCreateConfigFile)(pszFileName);    
}

class CConfigFileWrap : public IConfigFile
{
public:
#ifdef _WIN32
	CConfigFileWrap(const char* pszFileName, char* szDllFile = "../dll/configfile.dll")
#else
	CConfigFileWrap(const char* pszFileName, char* szDllFile = "../dll/configfile.so")
#endif
	{
		m_hDllHandle = NULL;
		memset(m_szDllName, 0, sizeof(m_szDllName));
		strncpy(m_szDllName, szDllFile, sizeof(m_szDllName) - 1);
        m_pConfigFile = CreateConfigFilePtr(szDllFile, pszFileName, m_hDllHandle);
	}

	virtual ~CConfigFileWrap()
	{
		delete m_pConfigFile;
		m_pConfigFile = NULL;

		//卸载dll
#ifdef _WIN32
        if( m_hDllHandle )
		{
// 			FreeLibrary(m_hDllHandle);
			m_hDllHandle = NULL;
		}
#else
		if( m_hDllHandle )
		{
// 			dlclose(m_hDllHandle);
			m_hDllHandle = NULL;
		}
#endif
	}
    
	virtual bool GetItemValueShort( const char *pszSectionName, 
                        const char *pszKeyName, 
                        short &shReturnedValue,
						short shDefaultValue)
	{
        if( m_pConfigFile )
		{
			return m_pConfigFile->GetItemValueShort(pszSectionName, 
											pszKeyName, 
											shReturnedValue,
											shDefaultValue);
		}
		else
		{
			return false;
		}
	}

	int GetInteger( const char *pszSectionName, 
		const char *pszKeyName, 
		int lDefaultValue,
		int* pnReturnedValue
		 )
	{
		GetItemValueInt(pszSectionName, 
			pszKeyName, 
			*pnReturnedValue,
			lDefaultValue);
		return true;
	}

    virtual bool GetItemValueInt( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue,
                        int lDefaultValue )
	{
		if( m_pConfigFile )
		{
			return m_pConfigFile->GetItemValueInt(pszSectionName, 
											pszKeyName, 
											lReturnedValue,
											lDefaultValue);
		}
		else
		{
			return false;
		}
	}

	int GetString( const char *pszSectionName, 
		const char *pszKeyName, 
		const char *pszDefaultValue,
		char *pszReturnedString, 
		unsigned int nSize
		)
	{
		GetItemValueString( pszSectionName, 
			pszKeyName, 
			pszReturnedString, 
			nSize, 
			pszDefaultValue);
		return true;
	}
    virtual bool GetItemValueString( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize, 
                        const char *pszDefaultValue)
	{
		if( m_pConfigFile )
		{
			return m_pConfigFile->GetItemValueString(pszSectionName,
				pszKeyName, pszReturnedString, nSize, pszDefaultValue);
		}
		else
		{
			return false;
		}
	}



private:
	IConfigFile* m_pConfigFile; 
	char m_szDllName[256];
	HMODULE m_hDllHandle;
};

#endif

