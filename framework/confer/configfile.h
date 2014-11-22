/************************************************************
  Copyright ©,  2002-2003,
  FileName:     ConfigFile.h
  Author:       ¸ß·å
  Version:      1.0
  Date:         2002Äê5ÔÂ8ÈÕ
  Description:
    ÊµÏÖ¶ÔÅäÖÃÎÄ¼şµÄ¶ÁÈ¡¡£
    Ê¹ÓÃ·½·¨ÈçÏÂ£º
      1 ´´½¨±¾ÀàµÄ¶ÔÏóÊµÀı£¬ÔÚ¹¹Ôìº¯ÊıÖĞ´«ÈëÅäÖÃÎÄ¼şµÄÎÄ¼şÃû
      2 µ÷ÓÃ³ÉÔ±º¯ÊıIsOpen()ÅĞ¶ÏÊÇ·ñ¶ÁÈ¡³É¹¦
      3 µ÷ÓÃGetItemValueµÃµ½¾ßÌåµÄÖµ
  Others: 
  Function List:
    1. CConfigFile::CConfigFile
       ¹¹Ôìº¯Êı£¬³õÊ¼»¯ÀàµÄ³ÉÔ±£¬¶ÁÈ¡ÅäÖÃÎÄ¼ş
    2. CConfigFile::~CConfigFile
       Îö¹¹º¯Êı£¬ÊÍ·Å×ÊÔ´
    3. CConfigFile::IsOpen
       ·µ»ØÅäÖÃÎÄ¼ş¶ÁÈ¡×´Ì¬
    4. CConfigFile::GetItemValue×Ö·û´®
       È¡Ö¸¶¨µÄ¼üÖµ
    5. CConfigFile::LocateSection
       ¶¨Î»sectionµÄ¿ªÊ¼µØÖ·ºÍ½áÊøµØÖ·
    6. CConfigFile::LocateKeyValue
       ÔÚÖ¸¶¨µÄ»º³åÇø·¶Î§ÖĞËÑË÷Key£¬·µ»ØÓëKeyÆ¥ÅäµÄÖµµÄ¿ªÊ¼µØÖ·ºÍ½áÊøµØÖ·
    7. CConfigFile::LocateStr
       ÔÚÒ»¸ö×Ö·û´®ÖĞËÑË÷ÁíÒ»¸ö×Ö·û´®
    8. CConfigFile::MapToContent
       ½«ShadowÖĞµÄµØÖ·Ó³Éäµ½ContentÖĞ
    9. CConfigFile::MapToShadow
       ½«ContentÖĞµÄµØÖ·Ó³Éäµ½ShadowÖĞ
    10.CConfigFile::uT_main
       ÀàµÄ²âÊÔÓÃ³ÉÔ±Ö÷º¯Êı¡£¾²Ì¬º¯Êı
    11.CConfigFile::uT_GetContentBuffer
    12.CConfigFile::uT_SetBuffer
    13.CConfigFile::uT_ClearBuffer
    14.CConfigFile::uT_SetContent
    15.CConfigFile::uT_SetShadow
    16.CConfigFile::uT_SetIsOpenFlag
    17.ToLower
       ½«×Ö·û´®ÖĞµÄ´óĞ´×ÖÄ¸×ª»»³ÉĞ¡Ğ´×ÖÄ¸
    18.CConfigFile::OpenFile
       ¼ÓÔØÅäÖÃÎÄ¼şµ½ÄÚ´æ
    19.CConfigFile::CloseFile
       ÊÍ·ÅÅäÖÃÎÄ¼ş¼ÓÔØµ½ÄÚ´æºóÕ¼ÓÃµÄ×ÊÔ´
    20.CConfigFile::SetItemValue×Ö·û´®
       ´ÓÄÚ´æ»º³åÇøÖĞÕÒµ½KeyName£¬½«Öµ¿½±´µ½Ö¸¶¨µÄ¿Õ¼ä
    21.CConfigFile::LocateKeyRange
       ÔÚÖ¸¶¨µÄ»º³åÇø·¶Î§ÖĞËÑË÷Key£¬·µ»ØÓëKeyÆ¥ÅäµÄÖµµÄ¿ªÊ¼µØÖ·ºÍ½áÊøµØÖ·
    22.CConfigFile::SetItemValueÊı×Ö
    23.CConfigFile::GetItemValueÊı×Ö
    24.CConfigFile::GetItemValue´øÈ±Ê¡ÖµÊı×Ö
    25.CConfigFile::GetItemValue´øÈ±Ê¡Öµ×Ö·û´®
  History: 
      ¸ß·å      2002/05/08        1.0        ½¨Á¢±¾Ä£¿é
      ¸ß·å      2002/05/13        1.0        Ôö¼ÓSetItemValue×Ö·û´®,
                                             OpenFile, CloseFile,
                                             LocateKeyRange
      ¸ß·å      2002/06/08        1.0        Ôö¼ÓSetItemValueÊı×Ö,
                                             Ôö¼ÓGetItemValueÊı×Ö
      ¸ß·å      2002/06/10        1.0        Ê¹GetItemValue, SetItemValueÖ§³Ö×ÊÔ´Ëø¶¨
      ¸ß·å      2002/06/11        1.0        Ôö¼Ó´øÈ±Ê¡ÖµµÄGetItemValueÊı×Ö, GetItemValue×Ö·û´®
***********************************************************/

#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

CONFIGFILE_NAMESPACE_BEGIN

class CConfigFile : public IConfigFile
{
public:
    CConfigFile(const char *pszFilename);  
    virtual ~CConfigFile();
    
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

private:
	char    *m_pszFilename;     /* ´æ·ÅĞèÒª¶ÁÈ¡µÄÅäÖÃÎÄ¼şÃû */
	
    unsigned int IsOpen();
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        const char *pszKeyValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue );
	unsigned int GetItemValue( const char *pszSectionName, 
					const char *pszKeyName, 
					short &lReturnedValue );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int lKeyValue );

	void CloseFile();

private:
    bool OpenFile(const char *pszFilename);

    unsigned int LocateSection(const char *pszSectionName, 
                        char * &pszSectionBegin, 
                        char * &pszSectionEnd);
    unsigned int LocateKeyRange(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszKeyBegin, 
                        char * &pszKeyEnd);
    unsigned int LocateKeyValue(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszValueBegin, 
                        char * &pszValueEnd);
    char *LocateStr(    const char *pszCharSet, 
                        const char *pszBegin, 
                        const char *pszEnd );
	char *SearchMarchStr(const char *pszBegin, const char *pszCharSet);

    char *MapToContent(const char *p);
    char *MapToShadow(const char *p);

	void ToLower( char * pszSrc, size_t len);

private:
    char    *m_pszContent;      /* ÅäÖÃÎÄ¼şµÄÔ­Ê¼ÄÚÈİ */
    char    *m_pszShadow;       /* ÅäÖÃÎÄ¼şµÄÄÚÈİÈ«²¿×ª»»³ÉĞ¡Ğ´ */
    size_t  m_nSize;            /* ÅäÖÃÎÄ¼şÄÚÈİµÄ³¤¶È£¬²»°üÀ¨×îºóµÄNULL */
    short   m_bIsOpen;          /* ÅäÖÃÎÄ¼şÊÇ·ñ´ò¿ª³É¹¦µÄ±êÖ¾ */

};

CONFIGFILE_NAMESPACE_END
#endif

