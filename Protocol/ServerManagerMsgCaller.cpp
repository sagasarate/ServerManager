﻿#include "stdafx.h"


CServerManagerMsgCaller::CServerManagerMsgCaller(INetProcessor * pNet)
{
	m_pNet=pNet;
}

CServerManagerMsgCaller::~CServerManagerMsgCaller(void)
{
}


int CServerManagerMsgCaller::Login(const CEasyString& UserName ,const CEasyString& Password )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetStringMemberSize(UserName)
		+CSmartStruct::GetStringMemberSize(Password);

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_LOGIN,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgLogin(Packet,UserName,Password))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::GetServiceList()
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		;

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_LIST,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServiceList(Packet))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::GetProcessList(short Page ,short PageLen )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(short));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_PROCESS_LIST,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetProcessList(Packet,Page,PageLen))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::GetNetAdapterList()
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		;

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_NET_ADAPTER_LIST,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetNetAdapterList(Packet))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::GetServiceInfo(UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_INFO,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServiceInfo(Packet,ServiceID))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::ServiceStartup(UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_STARTUP,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgServiceStartup(Packet,ServiceID))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::ServiceShutdown(UINT ServiceID ,BYTE ShutdownType )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetFixMemberSize(sizeof(BYTE));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_SHUTDOWN,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgServiceShutdown(Packet,ServiceID,ShutdownType))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::RunProgram(UINT ServiceID ,const CEasyString& FilePath ,const CEasyString& WorkDir ,const CEasyString& Param )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(FilePath)
		+CSmartStruct::GetStringMemberSize(WorkDir)
		+CSmartStruct::GetStringMemberSize(Param);

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_RUN_PROGRAM,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgRunProgram(Packet,ServiceID,FilePath,WorkDir,Param))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::ProcessShutdown(UINT ProcessID ,BYTE ShutdownType )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetFixMemberSize(sizeof(BYTE));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_PROCESS_SHUTDOWN,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgProcessShutdown(Packet,ProcessID,ShutdownType))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::ExecuteScript(UINT ServiceID ,const CEasyString& Script ,bool FromFile )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(Script)
		+CSmartStruct::GetFixMemberSize(sizeof(BYTE));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EXECUTE_SCRIPT,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgExecuteScript(Packet,ServiceID,Script,FromFile))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::BrowseServiceDir(UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(Dir)
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(short));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_BROWSE_SERVICE_DIR,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgBrowseServiceDir(Packet,ServiceID,Dir,Page,PageLen))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::FileDownloadStart(UINT ServiceID ,const CEasyString& FilePath ,UINT64 StartOffset )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(FilePath)
		+CSmartStruct::GetFixMemberSize(sizeof(UINT64));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_START,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileDownloadStart(Packet,ServiceID,FilePath,StartOffset))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::FileDownloadData()
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		;

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_DATA,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileDownloadData(Packet))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::FileDownloadFinish()
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		;

	WORD MsgFlag=0;
	MsgFlag|=DOS_MESSAGE_FLAG_CAN_CACHE;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_FINISH,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileDownloadFinish(Packet))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::FileUploadStart(UINT ServiceID ,const CEasyString& FilePath ,UINT FileLastWriteTime )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(FilePath)
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_START,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileUploadStart(Packet,ServiceID,FilePath,FileLastWriteTime))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::FileUploadData(UINT Length ,const CEasyBuffer& FileData ,bool IsLast )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetBinaryMemberSize(FileData.GetUsedSize())
		+CSmartStruct::GetFixMemberSize(sizeof(BYTE));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_DATA,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileUploadData(Packet,Length,FileData,IsLast))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::FileUploadFinish()
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		;

	WORD MsgFlag=0;
	MsgFlag|=DOS_MESSAGE_FLAG_CAN_CACHE;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_FINISH,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileUploadFinish(Packet))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::CreateDir(UINT ServiceID ,const CEasyString& Dir )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(Dir);

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CREATE_DIR,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgCreateDir(Packet,ServiceID,Dir))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::DeleteFile(UINT ServiceID ,const CEasyString& FilePath ,bool IsRecursive )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(FilePath)
		+CSmartStruct::GetFixMemberSize(sizeof(BYTE));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_FILE,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgDeleteFile(Packet,ServiceID,FilePath,IsRecursive))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::ChangeFileMode(UINT ServiceID ,const CEasyString& FilePath ,UINT Mode )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(FilePath)
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CHANGE_FILE_MODE,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgChangeFileMode(Packet,ServiceID,FilePath,Mode))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::AddService(const CSmartStruct& ServiceInfo )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetStructMemberSize(ServiceInfo.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ADD_SERVICE,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgAddService(Packet,ServiceInfo))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::EditService(const CSmartStruct& ServiceInfo )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetStructMemberSize(ServiceInfo.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EDIT_SERVICE,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgEditService(Packet,ServiceInfo))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::DeleteService(UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_SERVICE,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgDeleteService(Packet,ServiceID))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::SendCommand(UINT ServiceID ,const CEasyString& Command )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(Command);

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SEND_COMMAND,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgSendCommand(Packet,ServiceID,Command))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::EnableLogRecv(UINT ServiceID ,bool Enable )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetFixMemberSize(sizeof(BYTE));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ENABLE_LOG_RECV,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgEnableLogRecv(Packet,ServiceID,Enable))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::GetServerStatus(UINT ServiceID ,const CSmartStruct& StatusListPacket )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStructMemberSize(StatusListPacket.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServerStatus(Packet,ServiceID,StatusListPacket))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::GetAllServerStatus(UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_ALL_SERVER_STATUS,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetAllServerStatus(Packet,ServiceID))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::GetServerStatusFormat(UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS_FORMAT,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServerStatusFormat(Packet,ServiceID))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}

int CServerManagerMsgCaller::FileCompare(UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize ,const CEasyString& FileMD5 )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize(FilePath)
		+CSmartStruct::GetFixMemberSize(sizeof(UINT64))
		+CSmartStruct::GetStringMemberSize(FileMD5);

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_COMPARE,false));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileCompare(Packet,ServiceID,FilePath,FileSize,FileMD5))
	{			
		if(m_pNet->SendMessage(pMsg))
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_SUCCEED;
		}
		else
		{
			m_pNet->ReleaseMessage(pMsg);
			return COMMON_RESULT_MSG_SEND_ERROR;
		}
	}
	else
	{
		m_pNet->ReleaseMessage(pMsg);
		return COMMON_RESULT_MSG_PACK_ERROR;
	}
	
}


bool CServerManagerMsgCaller::PackMsgLogin(CSmartStruct& Packet,const CEasyString& UserName ,const CEasyString& Password )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_LOGIN_USER_NAME,UserName),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_LOGIN_PASSWORD,Password),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgGetServiceList(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgGetProcessList(CSmartStruct& Packet,short Page ,short PageLen )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_PROCESS_LIST_PAGE,Page),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_PROCESS_LIST_PAGE_LEN,PageLen),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgGetNetAdapterList(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgGetServiceInfo(CSmartStruct& Packet,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVICE_INFO_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgServiceStartup(CSmartStruct& Packet,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_STARTUP_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgServiceShutdown(CSmartStruct& Packet,UINT ServiceID ,BYTE ShutdownType )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_SHUTDOWN_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_SHUTDOWN_SHUTDOWN_TYPE,ShutdownType),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgRunProgram(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,const CEasyString& WorkDir ,const CEasyString& Param )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_RUN_PROGRAM_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_RUN_PROGRAM_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_RUN_PROGRAM_WORK_DIR,WorkDir),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_RUN_PROGRAM_PARAM,Param),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgProcessShutdown(CSmartStruct& Packet,UINT ProcessID ,BYTE ShutdownType )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCESS_SHUTDOWN_PROCESS_ID,ProcessID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCESS_SHUTDOWN_SHUTDOWN_TYPE,ShutdownType),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgExecuteScript(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Script ,bool FromFile )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EXECUTE_SCRIPT_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EXECUTE_SCRIPT_SCRIPT,Script),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EXECUTE_SCRIPT_FROM_FILE,FromFile),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgBrowseServiceDir(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_DIR,Dir),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_PAGE,Page),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_PAGE_LEN,PageLen),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgFileDownloadStart(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT64 StartOffset )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_START_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_START_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_START_START_OFFSET,StartOffset),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgFileDownloadData(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgFileDownloadFinish(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgFileUploadStart(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT FileLastWriteTime )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_START_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_START_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_START_FILE_LAST_WRITE_TIME,FileLastWriteTime),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgFileUploadData(CSmartStruct& Packet,UINT Length ,const CEasyBuffer& FileData ,bool IsLast )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_DATA_LENGTH,Length),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_DATA_FILE_DATA,FileData),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_DATA_IS_LAST,IsLast),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgFileUploadFinish(CSmartStruct& Packet)
{
	UINT FailCount=0;

	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgCreateDir(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Dir )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CREATE_DIR_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CREATE_DIR_DIR,Dir),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgDeleteFile(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,bool IsRecursive )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_FILE_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_FILE_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_FILE_IS_RECURSIVE,IsRecursive),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgChangeFileMode(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT Mode )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CHANGE_FILE_MODE_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CHANGE_FILE_MODE_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CHANGE_FILE_MODE_MODE,Mode),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgAddService(CSmartStruct& Packet,const CSmartStruct& ServiceInfo )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ADD_SERVICE_SERVICE_INFO,ServiceInfo),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgEditService(CSmartStruct& Packet,const CSmartStruct& ServiceInfo )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EDIT_SERVICE_SERVICE_INFO,ServiceInfo),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgDeleteService(CSmartStruct& Packet,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_SERVICE_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgSendCommand(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Command )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SEND_COMMAND_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SEND_COMMAND_COMMAND,Command),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgEnableLogRecv(CSmartStruct& Packet,UINT ServiceID ,bool Enable )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ENABLE_LOG_RECV_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ENABLE_LOG_RECV_ENABLE,Enable),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgGetServerStatus(CSmartStruct& Packet,UINT ServiceID ,const CSmartStruct& StatusListPacket )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_STATUS_LIST_PACKET,StatusListPacket),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgGetAllServerStatus(CSmartStruct& Packet,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_ALL_SERVER_STATUS_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgGetServerStatusFormat(CSmartStruct& Packet,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_FORMAT_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerMsgCaller::PackMsgFileCompare(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize ,const CEasyString& FileMD5 )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_COMPARE_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_COMPARE_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_COMPARE_FILE_SIZE,FileSize),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_COMPARE_FILE_MD5,FileMD5),FailCount);
	}
	
	

	return FailCount==0;
}


