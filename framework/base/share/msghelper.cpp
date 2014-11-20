#include "stdafx.h"
#include "../include/mywindows.h"
#include "../include/linuxtypes.h"
#include "../include/tlib_log.h"
#include "../include/base.h"
#include "msg.h"

void ConstructResMsgHead(const CMsgHead& stReqMsgHead, CMsgHead& stResMsgHead)
{
	stResMsgHead.m_sMsgID      = stReqMsgHead.m_sMsgID;
	stResMsgHead.m_sMsgType    = Response;
}

void ConstructMsgHead(CMsgHead& stMsgHead, short sMsgID, short sMsgType,
					  int nMsgSeq, char bSrcFE, char bDstFE, 
					  short sSrcID, short sDstID)
{
    stMsgHead.m_sSrcID   = sSrcID; //«Î«Û’–»ÀµƒPlayerID
    stMsgHead.m_sMsgID   = sMsgID;
    stMsgHead.m_sMsgType = sMsgType;
}


