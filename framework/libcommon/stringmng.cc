#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include <memory.h>
#endif

#include "base.h"
#include "stringmng.h"

CStringManager::CStringManager()
{
    m_szNoTipString[0] = 0;
	memset(m_szTipString, 0, sizeof(m_szTipString));
}

CStringManager::~CStringManager()
{

}

int CStringManager::LoadString(const char* pszStringFile)
{
	FILE* fpTipFile = fopen(pszStringFile, "r");

	if( fpTipFile == NULL )
	{
		return -1;
	}

	char szLineBuf[MAX_STRING_LEN];
	char szTipBuf[MAX_STRING_LEN];
	
	while( 1 )
	{
		if( feof(fpTipFile) )
		{
			break;
		}

		fgets(szLineBuf, sizeof(szLineBuf), fpTipFile);

		TrimStr( szLineBuf );

		if( szLineBuf[0] == '#' )
		{
			continue;
		}

		int nIndex;
		int iColumns = sscanf(szLineBuf, "%u%s", &nIndex, szTipBuf);

		if( iColumns < 2 )
		{
			continue;
		}
		if( nIndex < 0 && nIndex >= MAX_TIP_NUM )
		{
			continue;
		}
		strcpy(m_szTipString[nIndex], szTipBuf);
	}

	fclose(fpTipFile);
	return 0;
}

const char* CStringManager::GetStringByIndex(int iIndex)
{
	if( iIndex < 0 && iIndex >= MAX_TIP_NUM )
	{	
		return m_szNoTipString;
	}

	return m_szTipString[iIndex];
}
