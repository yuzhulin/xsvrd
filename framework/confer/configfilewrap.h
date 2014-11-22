
#ifndef _CRYPTWRAP_H_
#define _CRYPTWRAP_H_

#include "iconfigfile.h"



class CConfigfileWrap : public IConfigfile 
{
public:
    CConfigfileWrap();
    virtual ~CConfigfileWrap();

    virtual bool OpenFile(const char *pszFilename);

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

};

#endif
