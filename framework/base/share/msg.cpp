#include "stdafx.h"
#include "../include/mywindows.h"
#include "../include/linuxtypes.h"
#include "../include/base.h"
#include "msg.h"

CRequestLoginPara::CRequestLoginPara()
{
}
void CRequestLoginPara::StartEncode()
{
	*this << m_stData.cMajor << m_stData.cMinor;
	EncodeMemBuf((unsigned char*)m_stData.szGameSignature, m_stData.bSigLen);
    *this << m_stData.stVerifyData.lUin << m_stData.stVerifyData.ulTime << m_stData.stVerifyData.ulFlag;
	EncodeMemBuf((unsigned char*)m_stData.stVerifyData.szGameKey, sizeof(m_stData.stVerifyData.szGameKey));
	EncodeMemBuf((unsigned char*)m_stData.stVerifyData.szSvcBitmap, sizeof(m_stData.stVerifyData.szSvcBitmap));
    *this << m_stData.stQQInfo.wFace << m_stData.stQQInfo.cAge << m_stData.stQQInfo.cGender;
	EncodeMemBuf((unsigned char*)m_stData.stQQInfo.szNick, m_stData.stQQInfo.cNickLen);
}

void CRequestLoginPara::StartDecode()
{
}
void CRequestLoginPara::Print()
{
}

CResponseLoginPara::CResponseLoginPara()
{
}
void CResponseLoginPara::StartEncode()
{
//	*this << m_stData.m_nResultID;
}
void CResponseLoginPara::StartDecode()
{	
//	*this >> m_stData.m_nResultID;
}
void CResponseLoginPara::Print()
{
}

CRequestLogoutPara::CRequestLogoutPara()
{
}
void CRequestLogoutPara::StartEncode()
{
	*this << m_stData.m_dwReason;
}
void CRequestLogoutPara::StartDecode()
{
	*this >> m_stData.m_dwReason;
}
void CRequestLogoutPara::Print()
{
}

CResponseLogoutPara::CResponseLogoutPara()
{
}
void CResponseLogoutPara::StartEncode()
{
	*this << m_stData.m_dwReason;
}
void CResponseLogoutPara::StartDecode()
{
	*this >> m_stData.m_dwReason;
}
void CResponseLogoutPara::Print()
{
}

