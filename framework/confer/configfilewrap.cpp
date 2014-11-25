#ifdef _WIN32
#include <windows.h>
#endif
#include "configfilewrap.h"
#include "configfile.h"
#include "iconfigfile.h"

CConfigfileWrap::CConfigfileWrap()
{
}

CConfigfileWrap::~CConfigfileWrap()
{
}

bool OpenFile(const char *pszFilename);

virtual bool GetItemValueShort( const char *pszSectionName, 
                        const char *pszKeyName, 
                        short &shReturnedValue,
						short shDefaultValue);    
    virtual bool GetItemValueInt( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue,
                        int lDefaultValue );
    virtual bool GetItemValueString( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize, 
                        const char *pszDefaultValue);
