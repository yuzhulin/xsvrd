#include "stdafx.h"
#include "../include/mywindows.h"
#include "../include/linuxtypes.h"
#include "../include/base.h"
#include "msgservers.h"

#define PRINT printf

CInterCmdPara::CInterCmdPara()
{
	m_iHandleID        = -1;
    m_iCmdID           = -1;
    memset(m_szCmdBuffer, 0 , sizeof(m_szCmdBuffer));
}
CInterCmdPara::~CInterCmdPara()
{
}
void CInterCmdPara::StartEncode()
{
	CInputMem<USHORT> inmem((byte*)m_szCmdBuffer, (USHORT)sizeof(m_szCmdBuffer));
	*this << m_iHandleID << m_iCmdID << inmem;
}
void CInterCmdPara::StartDecode()
{
	USHORT usBuffLen = sizeof(m_szCmdBuffer);
    COutputMem<USHORT> outmem((byte*)m_szCmdBuffer, &usBuffLen);
	*this >> m_iHandleID >> m_iCmdID >> outmem;
}
void CInterCmdPara::Print()
{
}

CRequestPlayerSecretPara::CRequestPlayerSecretPara()
{
}
CRequestPlayerSecretPara::~CRequestPlayerSecretPara()
{
}
void CRequestPlayerSecretPara::StartEncode()
{
	ibufflen = sizeof(buffer);
	CInputMem<int> inMem((byte*)buffer, ibufflen);
	*this << m_szAccountID << m_unTime << inMem;
}
void CRequestPlayerSecretPara::StartDecode()
{
	ibufflen = sizeof(buffer);
	COutputMem<int> outMem((byte*)buffer, &ibufflen);

	*this >> m_szAccountID >> m_unTime >> outMem;
}
void CRequestPlayerSecretPara::Print()
{
}

CResponsePlayerSecretPara::CResponsePlayerSecretPara()
{
	m_szPassword[0] = 0;
}
CResponsePlayerSecretPara::~CResponsePlayerSecretPara()
{
}
void CResponsePlayerSecretPara::StartEncode()
{
	*this << m_nResultID << m_szAccountID << m_szPassword;
}
void CResponsePlayerSecretPara::StartDecode()
{
	OutStringBuf outstrbuf(m_szPassword, sizeof(m_szPassword));
	*this >> m_nResultID >> m_szAccountID >> outstrbuf;
}
void CResponsePlayerSecretPara::Print()
{
}

CInterTimeOutPara::CInterTimeOutPara()
{
    m_nTimerMark = -1;
    m_nOtherInfoLength = 0;

}
CInterTimeOutPara::~CInterTimeOutPara()
{
}
void CInterTimeOutPara::StartEncode()
{
	CInputMem<USHORT> inmem( (byte*)m_stOtherInfo.m_abyOtherInfo, m_nOtherInfoLength);
    *this << m_nTimerMark << inmem;    
}
void CInterTimeOutPara::StartDecode()
{
	m_nOtherInfoLength = sizeof(m_stOtherInfo.m_abyOtherInfo);
	COutputMem<USHORT> outmem((byte*)m_stOtherInfo.m_abyOtherInfo, (unsigned short *)&m_nOtherInfoLength);
	*this >> m_nTimerMark >> outmem;
}
void CInterTimeOutPara::Print()
{
}

