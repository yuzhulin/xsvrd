#ifndef _STRINGMANAGER_H_
#define _STRINGMANAGER_H_

#define MAX_TIP_NUM          1000
#define MAX_STRING_LEN       200

class CStringManager
{
public:
	CStringManager();
	~CStringManager();

	int LoadString(const char* pszStringFile);
	const char* GetStringByIndex(int iIndex);

private:
	char m_szNoTipString[1];
	char m_szTipString[MAX_TIP_NUM][MAX_STRING_LEN];
};

#endif 

