#ifndef _MSGSTRUCT_H_
#define _MSGSTRUCT_H_

#include "../include/common.h"

static const unsigned char ssk_[] = "^f098fgGHc8z7#50";

#define MAX_CLT_PKG_DATA                         	98304
#define CS_CONN_DATA_VERIFY_FAIL                 	1010031
#define CS_CONN_QQ_NOT_ALLOW                     	1010008

enum CB_TYPE
{
    CBT_CLIENT_INIT		= 0x00, // 无链接
    CBT_CLIENT_CREATED  = 0x01, // 客户端建立连接
    CBT_CLIENT_CLOSED   = 0x02, // 客户端主动关闭
    CBT_MESSAGE_TRANS   = 0x04, // 转发消息
    CBT_CLOSE_CLIENT    = 0x08, // 关闭客户端连接
    CBT_ADDMIT_CLT      = 0x16,  // 允许客户端接入
    CB_MULTICAST_CLT    = 0x32   // 多播消息
};

#define TCtrlHeadSize     (sizeof(ZONECONN_CTRL_BLOCK))   /*CtrlHead大小*/
#define MAX_SVR_PKG_DATA                         	98304
#define CS_KEY_LEN                               	16
#define CS_SVCBITMAP_LEN                         	12
#define CS_MAX_SIGN_LEN                          	128
#define CS_MAX_NAME_LEN                          	32
#define MAX_SERVER_PACKAGE_BUF_LEN                  32240


#pragma pack(1)

struct tagCSPkgHead
{
	int   iPkgLen;                
	short shCmd;                   
	unsigned int uiTick;
	unsigned short usStatus;
};

struct tagCSCltPkg
{
	tagCSPkgHead stHead;                 
	char szData[MAX_CLT_PKG_DATA];
};

struct tagCSLoginSucc
{
	char cMajor;                 
	char cMinor;                 
};

union tagCSLoginResult
{
	tagCSLoginSucc stSucc;                  	/* CSE_NOERR,   */
	unsigned int dwBanSec;                	/* CSE_BAN,   */
};

struct tagCSAccLoginS
{
	int iResult;                
	tagCSLoginResult stDesc;                 
};

struct tagCSSvrPkg
{
	tagCSPkgHead stHead;                 
	char szData[MAX_SVR_PKG_DATA];
};

struct tagCSGameSig
{
	char szGameKey[CS_KEY_LEN];  
	char szSvcBitmap[CS_SVCBITMAP_LEN];
	long lUin;                   
	unsigned long ulTime;                 
	unsigned long ulFlag;                 
};

struct tagCSQQInfo
{
	unsigned short wFace;                  
	char cAge;                   
	char cGender;                
	char cNickLen;               
	char szNick[CS_MAX_NAME_LEN];
};

struct tagCSAccLoginC
{
	char cMajor;                  	/*   协议的主版本 */
	char cMinor;                  	/*   协议的次版本 */
	unsigned char bSigLen;                
	char szGameSignature[CS_MAX_SIGN_LEN];
	tagCSGameSig stVerifyData;           
	tagCSQQInfo stQQInfo;               
};

typedef struct 
{
	UINT m_dwReason;
}STRequestLogoutPara;

typedef struct 
{
	UINT m_dwReason;
}STResponseLogoutPara;



#pragma pack()


#endif

