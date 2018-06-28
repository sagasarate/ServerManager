#include "stdafx.h"


CServerManagerAckMsgCaller::CServerManagerAckMsgCaller(INetProcessor * pNet)
{
	m_pNet=pNet;
}

CServerManagerAckMsgCaller::~CServerManagerAckMsgCaller(void)
{
}


int CServerManagerAckMsgCaller::LoginAck(short Result )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_LOGIN,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgLoginAck(Packet, Result ))
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

int CServerManagerAckMsgCaller::GetServiceListAck(short Result ,const CSmartStruct& ServiceListData )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetStructMemberSize(ServiceListData.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_LIST,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServiceListAck(Packet, Result , ServiceListData ))
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

int CServerManagerAckMsgCaller::GetProcessListAck(short Result ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& ProcessListData )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetStructMemberSize(ProcessListData.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_PROCESS_LIST,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetProcessListAck(Packet, Result , Page , PageLen , TotalPage , ProcessListData ))
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

int CServerManagerAckMsgCaller::GetNetAdapterListAck(short Result ,const CSmartStruct& NetAdapterListData )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetStructMemberSize(NetAdapterListData.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_NET_ADAPTER_LIST,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetNetAdapterListAck(Packet, Result , NetAdapterListData ))
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

int CServerManagerAckMsgCaller::GetServiceInfoAck(short Result ,const CSmartStruct& ServiceInfoPacket )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetStructMemberSize(ServiceInfoPacket.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_INFO,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServiceInfoAck(Packet, Result , ServiceInfoPacket ))
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

int CServerManagerAckMsgCaller::ServiceStartupAck(short Result ,UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_STARTUP,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgServiceStartupAck(Packet, Result , ServiceID ))
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

int CServerManagerAckMsgCaller::ServiceShutdownAck(short Result ,UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_SHUTDOWN,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgServiceShutdownAck(Packet, Result , ServiceID ))
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

int CServerManagerAckMsgCaller::RunProgramAck(short Result )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_RUN_PROGRAM,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgRunProgramAck(Packet, Result ))
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

int CServerManagerAckMsgCaller::ProcessShutdownAck(short Result ,UINT ProcessID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_PROCESS_SHUTDOWN,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgProcessShutdownAck(Packet, Result , ProcessID ))
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

int CServerManagerAckMsgCaller::ExecuteScriptAck(short Result ,UINT ServiceID ,int ErrorCode ,int LastLine )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetFixMemberSize(sizeof(int))
		+CSmartStruct::GetFixMemberSize(sizeof(int));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EXECUTE_SCRIPT,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgExecuteScriptAck(Packet, Result , ServiceID , ErrorCode , LastLine ))
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

int CServerManagerAckMsgCaller::BrowseServiceDirAck(short Result ,UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& FileListData )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize((UINT)Dir.GetLength())
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetStructMemberSize(FileListData.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_BROWSE_SERVICE_DIR,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgBrowseServiceDirAck(Packet, Result , ServiceID , Dir , Page , PageLen , TotalPage , FileListData ))
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

int CServerManagerAckMsgCaller::FileDownloadStartAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize((UINT)FilePath.GetLength())
		+CSmartStruct::GetFixMemberSize(sizeof(UINT64));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_START,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileDownloadStartAck(Packet, Result , ServiceID , FilePath , FileSize ))
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

int CServerManagerAckMsgCaller::FileDownloadDataAck(short Result ,UINT64 Offset ,UINT Length ,const CEasyBuffer& FileData )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT64))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSizeA(FileData.GetUsedSize());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_DATA,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileDownloadDataAck(Packet, Result , Offset , Length , FileData ))
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

int CServerManagerAckMsgCaller::FileDownloadEndAck(short Result ,UINT FileLastWriteTime )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_END,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileDownloadEndAck(Packet, Result , FileLastWriteTime ))
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

int CServerManagerAckMsgCaller::FileUploadStartAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize((UINT)FilePath.GetLength())
		+CSmartStruct::GetFixMemberSize(sizeof(UINT64));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_START,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileUploadStartAck(Packet, Result , ServiceID , FilePath , FileSize ))
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

int CServerManagerAckMsgCaller::FileUploadDataAck(short Result ,UINT64 Offset ,UINT Length )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT64))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_DATA,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileUploadDataAck(Packet, Result , Offset , Length ))
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

int CServerManagerAckMsgCaller::FileUploadEndAck(short Result )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_END,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgFileUploadEndAck(Packet, Result ))
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

int CServerManagerAckMsgCaller::CreateDirAck(short Result ,UINT ServiceID ,const CEasyString& Dir )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize((UINT)Dir.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CREATE_DIR,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgCreateDirAck(Packet, Result , ServiceID , Dir ))
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

int CServerManagerAckMsgCaller::DeleteFileAck(short Result ,UINT ServiceID ,const CEasyString& FilePath )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize((UINT)FilePath.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_FILE,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgDeleteFileAck(Packet, Result , ServiceID , FilePath ))
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

int CServerManagerAckMsgCaller::ChangeFileModeAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT Mode )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStringMemberSize((UINT)FilePath.GetLength())
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CHANGE_FILE_MODE,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgChangeFileModeAck(Packet, Result , ServiceID , FilePath , Mode ))
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

int CServerManagerAckMsgCaller::AddServiceAck(short Result )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ADD_SERVICE,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgAddServiceAck(Packet, Result ))
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

int CServerManagerAckMsgCaller::EditServiceAck(short Result )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EDIT_SERVICE,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgEditServiceAck(Packet, Result ))
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

int CServerManagerAckMsgCaller::DeleteServiceAck(short Result ,UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_SERVICE,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgDeleteServiceAck(Packet, Result , ServiceID ))
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

int CServerManagerAckMsgCaller::SendCommandAck(short Result ,UINT ServiceID )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SEND_COMMAND,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgSendCommandAck(Packet, Result , ServiceID ))
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

int CServerManagerAckMsgCaller::EnableLogRecvAck(short Result ,UINT ServiceID ,bool Enable )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetFixMemberSize(sizeof(BYTE));

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ENABLE_LOG_RECV,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgEnableLogRecvAck(Packet, Result , ServiceID , Enable ))
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

int CServerManagerAckMsgCaller::ConsoleLogNotify(UINT ServiceID ,LPCTSTR LogMsg )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+(LogMsg!=NULL?CSmartStruct::GetStringMemberSize((UINT)_tcslen(LogMsg)):0);

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CONSOLE_LOG_NOTIFY,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgConsoleLogNotify(Packet, ServiceID , LogMsg ))
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

int CServerManagerAckMsgCaller::GetServerStatusAck(short Result ,UINT ServiceID ,const CSmartStruct& StatusListPacket )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStructMemberSize(StatusListPacket.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServerStatusAck(Packet, Result , ServiceID , StatusListPacket ))
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

int CServerManagerAckMsgCaller::GetServerStatusFormatAck(short Result ,UINT ServiceID ,const CSmartStruct& StatusFormatPacket )
{
	if(m_pNet==NULL)
	{
		return COMMON_RESULT_FAILED;
	}

	UINT MsgDataSize=CSmartStruct::GetEmptyStructSize()
		+CSmartStruct::GetFixMemberSize(sizeof(short))
		+CSmartStruct::GetFixMemberSize(sizeof(UINT))
		+CSmartStruct::GetStructMemberSize(StatusFormatPacket.GetLength());

	WORD MsgFlag=0;
		
	CMessage * pMsg=m_pNet->NewMessage(MsgDataSize);
	if(pMsg==NULL)
	{
		return COMMON_RESULT_MSG_ALLOC_ERROR;
	}

	pMsg->SetMsgID(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS_FORMAT,true));
	pMsg->SetDataLength(MsgDataSize);
	pMsg->SetMsgFlag(MsgFlag);

	UINT FailCount=0;
	
	CSmartStruct Packet=pMsg->GetEmptyDataPacket();
	
	if(PackMsgGetServerStatusFormatAck(Packet, Result , ServiceID , StatusFormatPacket ))
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


bool CServerManagerAckMsgCaller::PackMsgLoginAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_LOGIN_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgGetServiceListAck(CSmartStruct& Packet,short Result ,const CSmartStruct& ServiceListData )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVICE_LIST_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVICE_LIST_ACK_SERVICE_LIST_DATA,ServiceListData),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgGetProcessListAck(CSmartStruct& Packet,short Result ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& ProcessListData )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_PROCESS_LIST_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_PROCESS_LIST_ACK_PAGE,Page),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_PROCESS_LIST_ACK_PAGE_LEN,PageLen),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_PROCESS_LIST_ACK_TOTAL_PAGE,TotalPage),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_PROCESS_LIST_ACK_PROCESS_LIST_DATA,ProcessListData),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgGetNetAdapterListAck(CSmartStruct& Packet,short Result ,const CSmartStruct& NetAdapterListData )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_NET_ADAPTER_LIST_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_NET_ADAPTER_LIST_ACK_NET_ADAPTER_LIST_DATA,NetAdapterListData),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgGetServiceInfoAck(CSmartStruct& Packet,short Result ,const CSmartStruct& ServiceInfoPacket )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVICE_INFO_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVICE_INFO_ACK_SERVICE_INFO_PACKET,ServiceInfoPacket),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgServiceStartupAck(CSmartStruct& Packet,short Result ,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_STARTUP_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_STARTUP_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgServiceShutdownAck(CSmartStruct& Packet,short Result ,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_SHUTDOWN_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_SHUTDOWN_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgRunProgramAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_RUN_PROGRAM_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgProcessShutdownAck(CSmartStruct& Packet,short Result ,UINT ProcessID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCESS_SHUTDOWN_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCESS_SHUTDOWN_ACK_PROCESS_ID,ProcessID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgExecuteScriptAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,int ErrorCode ,int LastLine )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EXECUTE_SCRIPT_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EXECUTE_SCRIPT_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EXECUTE_SCRIPT_ACK_ERROR_CODE,ErrorCode),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EXECUTE_SCRIPT_ACK_LAST_LINE,LastLine),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgBrowseServiceDirAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& FileListData )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_ACK_DIR,Dir),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_ACK_PAGE,Page),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_ACK_PAGE_LEN,PageLen),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_ACK_TOTAL_PAGE,TotalPage),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_BROWSE_SERVICE_DIR_ACK_FILE_LIST_DATA,FileListData),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgFileDownloadStartAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_START_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_START_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_START_ACK_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_START_ACK_FILE_SIZE,FileSize),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgFileDownloadDataAck(CSmartStruct& Packet,short Result ,UINT64 Offset ,UINT Length ,const CEasyBuffer& FileData )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_DATA_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_DATA_ACK_OFFSET,Offset),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_DATA_ACK_LENGTH,Length),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_DATA_ACK_FILE_DATA,(LPCSTR)FileData.GetBuffer(),FileData.GetUsedSize()),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgFileDownloadEndAck(CSmartStruct& Packet,short Result ,UINT FileLastWriteTime )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_END_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_DOWNLOAD_END_ACK_FILE_LAST_WRITE_TIME,FileLastWriteTime),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgFileUploadStartAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_START_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_START_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_START_ACK_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_START_ACK_FILE_SIZE,FileSize),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgFileUploadDataAck(CSmartStruct& Packet,short Result ,UINT64 Offset ,UINT Length )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_DATA_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_DATA_ACK_OFFSET,Offset),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_DATA_ACK_LENGTH,Length),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgFileUploadEndAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_UPLOAD_END_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgCreateDirAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& Dir )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CREATE_DIR_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CREATE_DIR_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CREATE_DIR_ACK_DIR,Dir),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgDeleteFileAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_FILE_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_FILE_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_FILE_ACK_FILE_PATH,FilePath),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgChangeFileModeAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT Mode )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CHANGE_FILE_MODE_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CHANGE_FILE_MODE_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CHANGE_FILE_MODE_ACK_FILE_PATH,FilePath),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CHANGE_FILE_MODE_ACK_MODE,Mode),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgAddServiceAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ADD_SERVICE_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgEditServiceAck(CSmartStruct& Packet,short Result )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_EDIT_SERVICE_ACK_RESULT,Result),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgDeleteServiceAck(CSmartStruct& Packet,short Result ,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_SERVICE_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_DELETE_SERVICE_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgSendCommandAck(CSmartStruct& Packet,short Result ,UINT ServiceID )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SEND_COMMAND_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SEND_COMMAND_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgEnableLogRecvAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,bool Enable )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ENABLE_LOG_RECV_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ENABLE_LOG_RECV_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_ENABLE_LOG_RECV_ACK_ENABLE,Enable),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgConsoleLogNotify(CSmartStruct& Packet,UINT ServiceID ,LPCTSTR LogMsg )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CONSOLE_LOG_NOTIFY_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		if( LogMsg != NULL)
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_CONSOLE_LOG_NOTIFY_LOG_MSG,LogMsg),FailCount);
		}
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgGetServerStatusAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CSmartStruct& StatusListPacket )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_ACK_STATUS_LIST_PACKET,StatusListPacket),FailCount);
	}
	
	

	return FailCount==0;
}

bool CServerManagerAckMsgCaller::PackMsgGetServerStatusFormatAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CSmartStruct& StatusFormatPacket )
{
	UINT FailCount=0;

	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_FORMAT_ACK_RESULT,Result),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_FORMAT_ACK_SERVICE_ID,ServiceID),FailCount);
	}
	
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_GET_SERVER_STATUS_FORMAT_ACK_STATUS_FORMAT_PACKET,StatusFormatPacket),FailCount);
	}
	
	

	return FailCount==0;
}


