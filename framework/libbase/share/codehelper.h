// HGBaseMsg.h: interface for the CHGBaseMsg class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _CODEHELPER_H_
#define _CODEHELPER_H_


#define DECODESTR(szBuffer)	        DecodeStrBuf(szBuffer, sizeof(szBuffer))
#define ENCODESTR(szBuffer)	        EncodeStrBuf(szBuffer, sizeof(szBuffer))

template< class AnyType>
class CInputMem 
{
public:
    CInputMem(byte * _mem, AnyType _length)
        :mem(_mem),length(_length)
    {
    }
    byte*    mem;
    AnyType  length;
};

template< class AnyType>
class COutputMem
{
public:
    COutputMem(byte * _mem, AnyType* _length)
        :mem(_mem),length(_length)
    {
    }
    byte     * mem;
    AnyType  *length;
};


class CInString 
{
public:
    CInString()
    {
    }
    CInString(char* _str, USHORT _maxlength)
    {
        SetStringBuf(_str, _maxlength);
    }

    void SetStringBuf(char* _str, USHORT _maxlength)
    {
        str          = _str;
        maxbufferlen = _maxlength;
    }
    
    char   *str;
    USHORT  maxbufferlen;
};

struct OutStringBuf 
{
    OutStringBuf()
    {
    }
    OutStringBuf(char* _str, USHORT _maxlength)
    {
        SetStringBuf(_str, _maxlength);
    }

    void SetStringBuf(char* _str, USHORT _maxlength)
    {
        str          = _str;
        maxbufferlen = _maxlength;
    }

    char* str;
    USHORT maxbufferlen;
};

class CBaseOperator
{
public:
    CBaseOperator() : data(NULL),cursor(NULL),m_bDecodeSucc(true),
		              m_bEncodeSucc(true),m_iMaxEncodeLen(0)
    {
    }
    //////////////////////////////////////////////////////////////////////////
    CBaseOperator& operator <<(char ucSrc)
    {
		bool bCanEncode = IsCanEncode((int)sizeof(char));
		if( bCanEncode )
		{
			*cursor = ucSrc;
			cursor += sizeof(char);
		}        
        return *this;
    }
    CBaseOperator& operator <<(short usSrc)
    {  
		bool bCanEncode = IsCanEncode((int)sizeof(short));
		if( bCanEncode )
		{
			USHORT usTemp;
			usTemp = htons( usSrc );
			memcpy((void *)(cursor), (const void *)&usTemp, sizeof(USHORT));
			cursor += sizeof(short);
		}
        return *this;
    }
    CBaseOperator& operator <<(long ulSrc)
    {   
		bool bCanEncode = IsCanEncode((int)sizeof(long));
		if( bCanEncode )
		{
			unsigned long ulTemp;
			ulTemp = htonl(ulSrc);
			memcpy((void*)cursor,(const void*)&ulTemp,sizeof(unsigned long));
			cursor += sizeof(long);
        }
		return *this;
    }
    CBaseOperator& operator <<(int uiSrc)
    {
        *this<<(long)uiSrc;
        return *this;
    }
    CBaseOperator& operator <<(unsigned char ucSrc)
    {
        *this<<(char)ucSrc;
        return *this;
    }
    CBaseOperator& operator <<(USHORT usSrc)
    {    
        *this<<(short)usSrc;
        return *this;
    }
    CBaseOperator& operator <<(unsigned long usSrc)
    {
        *this<<(long)usSrc;
        return *this;
    }
    CBaseOperator& operator <<(unsigned int uiSrc)
    {       
        *this<<(int)uiSrc;
        return *this;
    }
    
    CBaseOperator& operator <<(char* strSrc)
    {
        USHORT usTemp;
        usTemp = (USHORT)strlen(strSrc);    
        
		bool bCanEncode = IsCanEncode((int)(sizeof(short) + usTemp));
		if( bCanEncode )
		{
			*this << usTemp;
			memcpy((void *)(cursor), (const void *)strSrc, usTemp);
			cursor += usTemp;
        }
        return *this;
    }

    
    CBaseOperator& operator << (CInString &strBufSrc)
    { 
        USHORT usTemp;
        usTemp = (USHORT)strlen(strBufSrc.str); 
        if( usTemp > strBufSrc.maxbufferlen - 1)
        {
            usTemp = strBufSrc.maxbufferlen - 1;
        }
        
		bool bCanEncode = IsCanEncode((int)(sizeof(short) + usTemp));
		if( bCanEncode )
		{
			*this << usTemp;
			memcpy((void *)(cursor), (const void *)strBufSrc.str, usTemp);
			cursor += usTemp;
        }
        return *this;
    }

	CBaseOperator& operator << (CInputMem<USHORT> &memSrc)
    { 
		bool bCanEncode = IsCanEncode((int)(sizeof(USHORT) + memSrc.length));
		if( bCanEncode )
		{		
			*this << (USHORT)memSrc.length;
			memcpy((void *)(cursor), (const void *)memSrc.mem, memSrc.length);
			cursor += memSrc.length;
        }
		return *this;
    }

    CBaseOperator& operator << (CInputMem<int> &memSrc)
    { 
		bool bCanEncode = IsCanEncode((int)(sizeof(int) + memSrc.length));
		if( bCanEncode )
		{		
			*this << (int)memSrc.length;
			memcpy((void *)(cursor), (const void *)memSrc.mem, memSrc.length);
			cursor += memSrc.length;
        }
		return *this;
    }

	CBaseOperator& EncodeStrBuf(char* pstrbuf, int iMaxBuflen)
    { 
        USHORT usTemp;
        usTemp = (USHORT)strlen(pstrbuf); 
        if( usTemp > iMaxBuflen - 1)
        {
            usTemp = iMaxBuflen - 1;
        }
        
		bool bCanEncode = IsCanEncode((int)(sizeof(short) + usTemp));
		if( bCanEncode )
		{
			*this << usTemp;
			memcpy(cursor, pstrbuf, usTemp);
			cursor += usTemp;
        }
        return *this;
    }

	CBaseOperator& EncodeMemBuf(byte* pbuffer, USHORT usBuflen)
    { 
		bool bCanEncode = IsCanEncode((int)(sizeof(USHORT) + usBuflen));
		if( bCanEncode )
		{		
			*this << usBuflen;
			memcpy(cursor, pbuffer, usBuflen);
			cursor += usBuflen;
        }
		return *this;
    }

	CBaseOperator& EncodeLargeMemBuf(byte* pbuffer, int iBuflen)
    { 
		bool bCanEncode = IsCanEncode((int)(sizeof(int) + iBuflen));
		if( bCanEncode )
		{		
			*this << (int)iBuflen;
			memcpy(cursor, pbuffer, iBuflen);
			cursor += iBuflen;
        }
		return *this;
    }
    
//****************************Á÷³ö*************************************************//
    CBaseOperator& operator >>(char& ucDst)
    {     
        if( m_bDecodeSucc )
		{
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)sizeof(char)  )
			{
				ucDst = *cursor;
				cursor += sizeof(char);
			}
			else
			{
				m_bDecodeSucc = false;
			}
		}

        return *this;
    }
    CBaseOperator& operator >>(short& usDst)
    {
        if( m_bDecodeSucc )
		{
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)sizeof(short)  )
			{
				USHORT usTemp;
				memcpy((void *)(&usTemp), (const void *)cursor, sizeof(USHORT));
				usDst = ntohs(usTemp);
				cursor += sizeof(short);        
			}
			else
			{
				m_bDecodeSucc = false;
			}
		}        
        return *this;
    }
    CBaseOperator& operator >>(long& ulDst)
    {   
		if( m_bDecodeSucc )
		{
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)sizeof(long)  )
			{
				unsigned long ulTemp;
				memcpy((void *)(&ulTemp), (const void *)cursor, sizeof(unsigned long));
				ulDst = ntohl(ulTemp);
				cursor += sizeof(long);
			}
			else
			{
				m_bDecodeSucc = false;
			}
		}
        return *this;
    }
    CBaseOperator& operator >>(int& uiDst)
    {
        *this>>(*(long*)&uiDst);
        return *this;
    }
    CBaseOperator& operator >>(unsigned char& ucDst)
    {
            
        *this>>(*(char*)&ucDst);
        return *this;
    }
    CBaseOperator& operator >>(USHORT& usDst)
    {
        
        *this>>(*(short*)&usDst);
        return *this;
    }
    CBaseOperator& operator >>(unsigned long& ulDst)
    {
            
        *this>>(*(long*)&ulDst);
        return *this;
    }
    CBaseOperator& operator >>(unsigned int& uiDst)
    {
            
        *this>>(*(int*)&uiDst);
        return *this;
    }
    CBaseOperator& operator >> (char *strSrc)
    {
        USHORT usTemp;
        *this >> usTemp;

        if( m_bDecodeSucc )
		{
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)usTemp  )
			{
				memcpy((void *)strSrc, (void *)(cursor), usTemp);
				cursor += usTemp;
				strSrc[usTemp] = 0;
			}
			else
			{
				m_bDecodeSucc = false;
			}
		}   
        return *this;
    }

    CBaseOperator& operator >> (OutStringBuf &strBufSrc)
    {   
        USHORT usTemp = 0;
        *this >> usTemp;
 
        if( m_bDecodeSucc )
		{
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)usTemp  )
			{
				int iOffSet = usTemp;
				if( usTemp > strBufSrc.maxbufferlen - 1 )
				{
					usTemp = strBufSrc.maxbufferlen - 1;
				}
				memcpy((void *)strBufSrc.str, (void *)(cursor), usTemp);
				strBufSrc.str[usTemp] = 0;

				cursor += iOffSet;
			}
			else
			{
				m_bDecodeSucc = false;
			}
		} 
        return *this;
    }

    CBaseOperator& operator >>(COutputMem<USHORT> &memSrc)
    {
		USHORT shMaxLen = *(memSrc.length);
        *this >> (*memSrc.length);
		if( m_bDecodeSucc )
		{	
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)*memSrc.length  )
			{
				int iOffSet = *memSrc.length;
				if( (*memSrc.length) > shMaxLen )
				{
					*memSrc.length = shMaxLen;
				}

				memcpy((void *)memSrc.mem, (const void *)(cursor), *memSrc.length);
				cursor += iOffSet;
			}
			else
			{
				m_bDecodeSucc = false;
			}
		} 
        return *this;
    }

    CBaseOperator& operator >>(COutputMem<int> &memSrc)
    {
		int nMaxLen = *(memSrc.length);
        *this >> (*memSrc.length);
		if( m_bDecodeSucc )
		{	
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)*memSrc.length  )
			{
				int iOffSet = *memSrc.length;
				if( (*memSrc.length) > nMaxLen )
				{
					*memSrc.length = nMaxLen;
				}

				memcpy((void *)memSrc.mem, (const void *)(cursor), *memSrc.length);
				cursor += iOffSet;
			}
			else
			{
				m_bDecodeSucc = false;
			}
		} 
        return *this;
    }

	CBaseOperator& DecodeStrBuf(char *pstrbuf, USHORT iMaxBufflen)
    {
        USHORT usTemp = 0;
        *this >> usTemp;
 
        if( m_bDecodeSucc )
		{
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)usTemp  )
			{
				int iOffSet = usTemp;
				if( usTemp > iMaxBufflen - 1 )
				{
					usTemp = iMaxBufflen - 1;
				}
				memcpy(pstrbuf, cursor, usTemp);
				pstrbuf[usTemp] = 0;

				cursor += iOffSet;
			}
			else
			{
				m_bDecodeSucc = false;
			}
		} 
        return *this;
	}

    CBaseOperator& DecodeMemBuf(byte* pBuffer, USHORT& nLen)
    {
		USHORT  usMaxLen  = nLen;
		USHORT& usTempLen = nLen;
		*this >> usTempLen;
		if( m_bDecodeSucc )
		{	
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + (int)usTempLen )
			{
				int iOffSet = usTempLen;
				if( usTempLen > usMaxLen )
				{
					usTempLen = usMaxLen;
				}

				memcpy(pBuffer, cursor, usTempLen);
				cursor += iOffSet;
			}
			else
			{
				m_bDecodeSucc = false;
			}
		} 
        return *this;
    }
    
	CBaseOperator& DecodeLargeMemBuf(byte* pBuffer, int& iLen)
    {
		int  iMaxLen  = iLen;
		int& iTempLen = iLen;
		*this >> iTempLen;
		if( m_bDecodeSucc )
		{	
			int shTempDecodeLen = GetHandleBufferLen();
			if( m_shInBufferLen >= shTempDecodeLen + iTempLen )
			{
				int iOffSet = iTempLen;
				if( iTempLen > iMaxLen )
				{
					iTempLen = iMaxLen;
				}

				memcpy(pBuffer, cursor, iTempLen);
				cursor += iOffSet;
			}
			else
			{
				m_bDecodeSucc = false;
			}
		} 
        return *this;
    }

	

protected:
    const char* getData()
    {
        return data;
    }
	bool IsCanEncode(int iAddSize)
	{
		if( m_bEncodeSucc )
		{
			int iHandleLen = GetHandleBufferLen();
			if( iHandleLen + iAddSize > m_iMaxEncodeLen )
			{
				m_bEncodeSucc = false;
			}
		}
		return m_bEncodeSucc;
	}
    int GetHandleBufferLen()
    {
        int nLen = (int)(cursor - data);
        return nLen;
    }    

    void InitCodeBuffer(char *pcOutBuffer)
    {
        data   = pcOutBuffer;
        cursor = pcOutBuffer;
    }
	bool m_bEncodeSucc;
    bool m_bDecodeSucc;
    char *data;
    char *cursor;
    int m_nMaxLen;

	int* m_pshOutLength;
    int m_shGetHandleBufferLen;
    int m_shInBufferLen;
	int m_iMaxEncodeLen;

    void BeginEncode(char * pcOutBuffer, int &sOutLength)
    {
		m_iMaxEncodeLen = sOutLength;
		m_pshOutLength  = &sOutLength;
        data            = pcOutBuffer;
        cursor          = pcOutBuffer + sizeof(int);
    }
    
	int EndEncode()
	{
        if( m_pshOutLength != NULL )
        {
            *m_pshOutLength = GetHandleBufferLen();
            cursor = data;
            *this << *m_pshOutLength;
        }
		return m_bEncodeSucc ? 0 : -1;
    }

    bool BeginDecode(char *pcInBuffer, int sInLength)
    {
        m_shInBufferLen         = sInLength;
        data   = pcInBuffer;
        cursor = pcInBuffer;

        *this >> m_shGetHandleBufferLen;
        bool bLengthValid = (m_shInBufferLen != m_shGetHandleBufferLen) ? false : true;
        return bLengthValid;
    }

    int EndDecode()
    {
		if( !m_bDecodeSucc )
		{
			return -1;
		}
        int shTempDecodeLen = GetHandleBufferLen();
        if( shTempDecodeLen > m_shGetHandleBufferLen )
        {
            return -1;
        }
        return 0;
    }

    friend class CEncodeHelper;
    friend class CDecodeHelper;
};

class CEncodeHelper
{
public:
    CEncodeHelper(char * pcOutBuffer, int &sOutLength, CBaseOperator * pBaseOperator)
        : m_pshLength(&sOutLength), m_pBaseOperator(pBaseOperator)
    {
        m_pBaseOperator->data   = pcOutBuffer;
        m_pBaseOperator->cursor = pcOutBuffer + sizeof(int);
    }
    virtual ~CEncodeHelper()
    {
        if( m_pshLength != NULL )
        {
            *m_pshLength = m_pBaseOperator->GetHandleBufferLen();
            m_pBaseOperator->cursor = m_pBaseOperator->data;
            *m_pBaseOperator << *m_pshLength;
        }
    }
private:
    int *m_pshLength;
    CBaseOperator *m_pBaseOperator;
};

class CDecodeHelper
{
public:
    CDecodeHelper(char *pcInBuffer, int sInLength, CBaseOperator * pBaseOperator, bool& bLengthValid)
        : m_pBaseOperator(pBaseOperator)
    {
        m_shInBufferLen         = sInLength;
        m_pBaseOperator->data   = pcInBuffer;
        m_pBaseOperator->cursor = pcInBuffer;

        *m_pBaseOperator >> m_shGetHandleBufferLen;
        bLengthValid = (m_shInBufferLen < m_shGetHandleBufferLen) ? false : true;
    }
    virtual ~CDecodeHelper()
    {
    }

    int GetDecodeResult()
    {
        int shTempDecodeLen = m_pBaseOperator->GetHandleBufferLen();
        if( shTempDecodeLen > m_shGetHandleBufferLen )
        {
            return -1;
        }
        return 0;
    }

    int GetLeftDecodeBufferLen()
    {
        int shTempDecodeLen = m_pBaseOperator->GetHandleBufferLen();
        return (m_shGetHandleBufferLen - shTempDecodeLen);
    }
protected:
    int m_shGetHandleBufferLen;
    int m_shInBufferLen;
    CBaseOperator *m_pBaseOperator;
};

#endif
