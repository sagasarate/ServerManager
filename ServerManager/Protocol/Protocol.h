#pragma once

#define MAKE_MSG_ID(ModuleID,InterfaceID,MethodID,IsAck)		((((DWORD)ModuleID&0x7F)<<24)|(((DWORD)InterfaceID&0xFF)<<16)|(((DWORD)MethodID)&0xFFFF)|(IsAck?0x80000000:0))
#define GET_MODULE_ID(MsgID)									((((DWORD)MsgID)>>24)&0x7F)
#define GET_INTERFACE_ID(MsgID)									((((DWORD)MsgID)>>16)&0xFF)
#define GET_METHOD_ID(MsgID)									((DWORD)MsgID&0xFFFF)
#define IS_ACK_MSG(MsgID)										((DWORD)MsgID&0x7FFFFFFF)

#define MAX_MSG_SIZE				650000
#define FILE_TRANSFER_BLOCK_SIZE	300000

enum SYSTEM_MESSAGE
{
	SM_NONE = 0,
	SM_PROXY_KEEP_ALIVE_PING,
	SM_PROXY_KEEP_ALIVE_PONG,
	SM_MAX,
};

enum DOS_MESSAGE_FLAG
{
	DOS_MESSAGE_FLAG_SYSTEM_MESSAGE = 1,
	DOS_MESSAGE_FLAG_COMPRESSED = (1 << 1),
	DOS_MESSAGE_FLAG_CAN_CACHE = (1 << 2),
	DOS_MESSAGE_FLAG_ENCRYPT = (1 << 3),
	DOS_MESSAGE_FLAG_NO_COMPRESS = (1 << 4),
};

enum COMMON_RESULT_CODE
{
	COMMON_RESULT_SUCCEED = 0,
	COMMON_RESULT_FAILED = -2,
	COMMON_RESULT_MSG_PACK_ERROR = -3,
	COMMON_RESULT_MSG_UNPACK_ERROR = -4,
	COMMON_RESULT_MSG_ALLOC_ERROR = -5,
	COMMON_RESULT_MSG_SEND_ERROR = -6,
	COMMON_RESULT_MSG_NO_HANDLER = -7,
};

typedef unsigned int	MSG_ID_TYPE;
typedef unsigned int	MSG_LEN_TYPE;

#pragma pack(push)

#pragma pack(1)

class CMessage
{
public:
	struct MESSAGE_HEAD
	{
		MSG_LEN_TYPE	MsgLen;
		MSG_ID_TYPE		MsgID;
		WORD			MsgFlag;
	};
protected:
	MESSAGE_HEAD			m_MsgHeader;
	char					m_DataBuffer[4];
public:
	void Init()
	{
		m_MsgHeader.MsgLen = sizeof(MESSAGE_HEAD);
		m_MsgHeader.MsgID = 0;
		m_MsgHeader.MsgFlag = 0;
	}
	MESSAGE_HEAD& GetMsgHeader()
	{
		return m_MsgHeader;
	}
	void SetMsgID(MSG_ID_TYPE CmdID)
	{
		m_MsgHeader.MsgID = CmdID;
	}
	MSG_ID_TYPE GetMsgID() const
	{
		return m_MsgHeader.MsgID;
	}
	void SetDataLength(MSG_LEN_TYPE Length)
	{
		m_MsgHeader.MsgLen = sizeof(MESSAGE_HEAD) + Length;
	}
	MSG_LEN_TYPE GetDataLength() const
	{
		return m_MsgHeader.MsgLen - sizeof(MESSAGE_HEAD);
	}
	MSG_LEN_TYPE GetMsgLength() const
	{
		return m_MsgHeader.MsgLen;
	}
	void SetMsgFlag(WORD Flag)
	{
		m_MsgHeader.MsgFlag = Flag;
	}
	WORD GetMsgFlag()
	{
		return m_MsgHeader.MsgFlag;
	}
	void * GetDataBuffer()
	{
		return m_DataBuffer;
	}
	CSmartStruct GetDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer, GetDataLength(), false);

		return DataPacket;
	}
	void ClearDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer, GetDataLength(), true);
	}
	CSmartStruct GetEmptyDataPacket()
	{
		CSmartStruct DataPacket(m_DataBuffer, GetDataLength(), true);

		return DataPacket;
	}

	static MSG_LEN_TYPE GetMsgHeaderLength()
	{
		return sizeof(MESSAGE_HEAD);
	}
	static MSG_LEN_TYPE CaculateMsgLength(MSG_LEN_TYPE DataLength)
	{
		return sizeof(MESSAGE_HEAD) + DataLength;
	}
};

#pragma pack(pop)

class INetProcessor
{
public:
	virtual CMessage * NewMessage(UINT MsgDataSize) = 0;
	virtual bool ReleaseMessage(CMessage * pMsg) = 0;
	virtual bool SendMessage(CMessage * pMsg) = 0;
};

class CBaseMsgHandler
{
public:
	CBaseMsgHandler()
	{

	}
	virtual ~CBaseMsgHandler()
	{

	}

};

typedef int (CBaseMsgHandler::*MSG_HANDLE_FN)(CSmartStruct& MsgPacket);
struct MSG_HANDLE_INFO
{
	CBaseMsgHandler *	pObject;
	MSG_HANDLE_FN		pFN;
};

#include "SvrMgrDataStructs.h"
#include "SvrMgrInterfaces.h"

