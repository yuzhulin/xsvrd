#ifndef _SERVERBASE_H_
#define _SERVERBASE_H_

#ifdef WIN32
#include <WinSock2.h>
#include <time.h>
#endif

#include "win_linux.h"
#include "objmng.h"
#include "common.h"

class CSharedMem;

#ifdef WIN32
#define snprintf _snprintf
#endif

#ifndef MD5_HASH_LEN
#define MD5_HASH_LEN 16
#endif

#define KK_PRINTF "%llu"
//#define KK_PRINTF "%lld"

#ifndef IP_LENGTH
#define IP_LENGTH 20
#endif

#define KEY_LEN      16


#define CFG_STRING	(int)1
#define CFG_INT		(int)2
#define CFG_LONG		(int)3
#define CFG_DOUBLE	(int)4

#define MAX_SERVER_TRANS_LEN              (200 * 1024)
//#define MAX_SERVER_TRANS_LEN              (32 * 1024)
#define MAXOPTLENGTH                      127            //C/S协议可选部分的最大长度
#define MAXTIMERCOUNT                     32
#define ACCOUNT_LEN                       40


extern int zzUnicode2gbkbase[65535];
extern int zzGbk2utf8base[65535];

//以下定义连接的实体类型
enum eConnEntityTypes
{
	cet_mainsvr = 0,
	cet_dbsvr,
	cet_other,
	cet_proxy,
	cet_other_router,
	cet_cascade
};

struct TConnServerInfo
{
	int m_entityType;
	int m_nServerID;
};

typedef enum
{
	Block = 0,
	NBlock = 1
} TBlockFlag;

#define KEYLENGTH 16
#define CRYPTKEYLENGTH 16
typedef unsigned char TKey[KEYLENGTH];
typedef char TName [32];
typedef char StrLine [1024];
typedef char TFName [64];

typedef struct
{
    int m_iYear;
    int m_iMon;
    int m_iDay;
    int m_iHour;
    int m_iMin;
    int m_iSec;
    int m_iMSec;
} TStruTime;

typedef enum
{
    Wrong = 0,
    Free = 0,
    Right = 1,
    InUse = 1,
    False = 0,
    True = 1,
} TBool;

static inline bool IsStringEqual(const char *pstr1, const char *pstr2)
{
    if( strcmp(pstr1, pstr2) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

static inline bool IsStringNotEqual(const char *pstr1, const char *pstr2)
{
    if( strcmp(pstr1, pstr2) == 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

#ifdef WIN32
extern int SetNBlock(SOCKET iSock);
#else
extern int SetNBlock(int iSock);
#endif

extern bool InitNetwork();
extern int GetDateString(char *strTime);
extern int GetTimeStringOfDay(char *strTime);
extern time_t ParserTime(const char* szTime);
extern void TimeValMinus(timeval& tvA, timeval& tvB, timeval& tvResult);
extern void TrimStr(char *strInput);
extern CSharedMem* CreateProcessMem(size_t iSize);
extern CSharedMem* CreateShareMem( unsigned int nShmKey, size_t iSize, bool bInitMode);
extern int DestroyShareMem(unsigned int nShmKey);
extern void InitDaemon(void);
extern int SockAddrToString(sockaddr_in *pstSockAddr, char *szResult);
extern int EncodeChar( char **pstrEncode, unsigned char ucSrc );
extern int DecodeChar( char **pstrDecode, unsigned char *pucDest );
extern int EncodeShort( char **pstrEncode, unsigned short usSrc );
extern int DecodeShort( char **pstrDecode, unsigned short *pusDest );
extern int EncodeLong( char **pstrEncode, unsigned long ulSrc );
extern int DecodeLong( char **pstrDecode, unsigned long *pulDest );
extern int EncodeInt( char **pstrEncode, unsigned int uiSrc );
extern int DecodeInt( char **pstrDecode, unsigned int *puiDest );
extern int EncodeString( char **pstrEncode, char *strSrc, unsigned short sMaxStrLength );
extern int DecodeString( char **pstrDecode, char *strDest, unsigned short sMaxStrLength );
extern int EncodeMem( char **pstrEncode, char *pcSrc, short sMemSize );
extern int DecodeMem( char **pstrDecode, char *pcDest, short sMemSize );

extern int EncodeLongLong(char **pstrEncode, unsigned long long ullSrc);
extern int DecodeLongLong(char **pstrDecode, unsigned long long *pullDest);

extern int GetTimeString( time_t timer, char *strTime);
extern int GetTimeString_r( time_t timer, char *strTime);
extern void TraceBin(char*, int);
extern int CreateUdpSocket(int iBlockFlag , unsigned short nPort, char *szIP, int iRecvTimeoutMs = 0);
extern int SendDataTo(int iSocket, void *pData, int nDataLen, char *szDstIP, unsigned short nDstPort);
extern int RecvDataFrom(int iSocket, void *pData, int nDataLen, char *szSrcIP, unsigned short* pnSrcPort);
extern int SplitMapStr(const char *pMapStr, int iMaxItemNum, int *pItemArray, char szGap = ',');
extern int GetIfAddrs(int& riAddrNum, sockaddr_in *pstIfAddrs);
extern char* GetNextToken(const char *szSrc, char cSplitter, char *szToken);
extern int SetSockBufLen(int iSock, int iLength);
extern unsigned int GenRandSeed();
extern struct tm *GetUpperTMFromTimeString(char *strTime);
extern int GetTmFromString( struct tm &stTM, char *strTime);
extern int GetStructTime(time_t tTime, TStruTime *pstTime);
extern int CreateDir(const char *szDir);
extern int ConvertStrToHash(char *pszHash, unsigned char *pbyHash);
extern void ConvertHashToStr(unsigned char *pbyHash, char *pszHash);
extern int ConvertStrToByte(char *pszHash, unsigned char *pbyHash);

extern int CreateCacheLock(char* pLockPath, int* pLockFd);
extern int CreateLockFiles(const char* pszFilePath, int nSize);
extern int CacheLock(int nLockFd, int nType);
extern int CacheUnLock(int nLockFd, int nType);
extern void CreatePath(char szLogPath[256]);
extern int ConvertTimeFromStringToIntInt(char* sTime);
extern void SetExeCurrPath(char* pszExeName);

extern int zzIconvInit();
extern int GBK2UTF8(char* strgbk, char* strutf8);
extern int UTF82Unicode(char* utf8char, int* unicode, int* len, int leaveLen);
extern int UTF82GBK(char* strutf8, char* strgbk);

#ifdef WIN32
extern int strcasecmp(const char* s1, const char* s2);
#endif




//以下定义连接的实体类型

/*以下定义转发类型*/
enum eTransTypes
{
    trans_p2p = 0,            //点到点
    trans_p2g,                //点到组
    trans_broadcast,        //同型广播
    trans_bykey                //根据关键字转发
};

extern int GetCurStructTime(TStruTime *pstTime);
extern int TimeValPlus(timeval& tvA, timeval& tvB, timeval& tvResult);
extern bool LoadBinaryFile(const char* pszFileName,	unsigned char* pbyFileBuf, int& iFileLen);
extern int WriteBufferToFile(char* pszFileName, char* pszFileBuffer, int nFileLen);
extern void CreatePath(char szLogPath[MAX_PATH]);
extern unsigned int IPStr2Value(const char *pszIP);
extern void IPValue2Str(unsigned int uIPValue, char szIPStr[IP_LENGTH]);

class CTimerSetted
{
public:
	CTimerSetted();
	~CTimerSetted();
	CTimerSetted& operator = (CTimerSetted& stTimerSetted);

	int   m_nTimerID;
	int   m_nTimerMark;
	int   m_iWaitSeq;
};


class CMsgItem : public CObj
{
public:
	CMsgItem();
	~CMsgItem();
	virtual int GetHashKey(void *pvKey, int& iKeyLength)
	{
		return 0;
	};
	virtual int SetHashKey(const void *pvKey, int iKeyLength)
	{
		return 0;
	};
	virtual int Show(FILE *fpOut)
	{
		return 0;
	};
	int Initialize();
	int Resume();
	___uint16___ m_unCodeType;
	short m_sMsgID;
	short m_sMsgType;
	int   m_iMsgSeq;
	BYTE  m_bySrcProcessFE;
	BYTE  m_byDstProcessFE;
	int m_sSrcProcessID;
	int m_sDstProcessID;
	BYTE  m_bySrcObjFE;
	BYTE  m_byDstObjFE;
	int m_sSrcObjID;
	int m_sDstObjID;
	int   m_nParaLength;
	int   m_iParaOffset;
	DECLARE_DYN
};

class CTimerItem : public CObj
{
public:
	CTimerItem();
	~CTimerItem();
	virtual int GetHashKey(void *pvKey, int& iKeyLength)
	{
		return 0;
	};
	virtual int SetHashKey(const void *pvKey, int iKeyLength)
	{
		return 0;
	};
	virtual int Show(FILE *fpOut)
	{
		return 0;
	};
	int Initialize();
	int Resume();
	short m_nOwnFE;
	int   m_nOwnID;
	int   m_nTimerMark;
	int   m_imSeconds;
	int   m_nTimeCount;
	int   m_aiTimerInfo[MAXTIMERCOUNT];
	
	DECLARE_DYN
};




#ifdef WIN32
static int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year     = wtm.wYear - 1900;
	tm.tm_mon     = wtm.wMonth - 1;
	tm.tm_mday     = wtm.wDay;
	tm.tm_hour     = wtm.wHour;
	tm.tm_min     = wtm.wMinute;
	tm.tm_sec     = wtm.wSecond;
	tm. tm_isdst    = -1;
	clock = mktime(&tm);
	tp->tv_sec = (long)clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;

	return (0);
}
#endif

#pragma	pack(1)

struct TSocketHead
{
	int m_nSocketIndex;
	unsigned int m_nConnSocketCount;
	unsigned int m_nCreateTime;
};

typedef struct _TNetHead
{
	TSocketHead m_sockHead;
	unsigned int m_nRecvPacketCount;
	unsigned int m_nPacketLen;
 	unsigned int m_unUin;
	unsigned int   m_nServerFlag;
	unsigned char  m_byPacketReason;
	unsigned char  m_byReserve;
	unsigned int m_tStamp;
	unsigned int m_nClientAddr;
	unsigned short m_nClientPort;
}TNetHead;

#pragma pack()

#endif


