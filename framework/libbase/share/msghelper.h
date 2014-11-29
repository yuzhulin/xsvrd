#ifndef _MSGHELPER_H_
#define _MSGHELPER_H_

class CProxyHead;
class CMsg;
class CMsgHead;

extern void ConstructResMsgHead(const CMsgHead& stReqMsgHead,
									 CMsgHead& stResMsgHead);
extern void ConstructMsgHead(CMsgHead& stMsgHead, short sMsgID, short sMsgType,
					  int nMsgSeq, char bSrcFE, char bDstFE, 
					  short sSrcID, short sDstID);
extern int ConvertMsgToProxyCode(int& nCodeLength, unsigned char* pCode, CProxyHead* pProxyHead, CMsg* pMsg);
extern int DBConvertProxyCodeToMsg(int nCodeLength, unsigned char* pCode, CProxyHead* pProxyHead, CMsg* pMsg);
extern int GameConvertProxyCodeToMsg(int nCodeLength, unsigned char* pCode, CProxyHead* pProxyHead, CMsgHead* pMsgHead,
									 int& nParaLength, unsigned char *pbyPara);

#endif

