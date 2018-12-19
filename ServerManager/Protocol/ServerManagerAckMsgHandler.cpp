#include "stdafx.h"

CServerManagerAckMsgHandler::CServerManagerAckMsgHandler(void)
{
}

CServerManagerAckMsgHandler::~CServerManagerAckMsgHandler(void)
{
}

void CServerManagerAckMsgHandler::InitMsgMap(CEasyMap<MSG_ID_TYPE,MSG_HANDLE_INFO>& MsgMap)
{
	MSG_HANDLE_INFO MsgHandleInfo;
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgLoginAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_LOGIN,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgGetServiceListAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_LIST,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgGetProcessListAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_PROCESS_LIST,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgGetNetAdapterListAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_NET_ADAPTER_LIST,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgGetServiceInfoAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_INFO,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgServiceStartupAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_STARTUP,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgServiceShutdownAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_SHUTDOWN,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgRunProgramAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_RUN_PROGRAM,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgProcessShutdownAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_PROCESS_SHUTDOWN,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgExecuteScriptAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EXECUTE_SCRIPT,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgBrowseServiceDirAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_BROWSE_SERVICE_DIR,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileDownloadStartAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_START,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileDownloadDataAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_DATA,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileDownloadEndAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_END,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileUploadStartAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_START,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileUploadDataAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_DATA,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileUploadEndAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_END,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgCreateDirAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CREATE_DIR,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgDeleteFileAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_FILE,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgChangeFileModeAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CHANGE_FILE_MODE,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgAddServiceAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ADD_SERVICE,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgEditServiceAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EDIT_SERVICE,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgDeleteServiceAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_SERVICE,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgSendCommandAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SEND_COMMAND,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgEnableLogRecvAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ENABLE_LOG_RECV,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgConsoleLogNotify;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CONSOLE_LOG_NOTIFY,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgGetServerStatusAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgGetServerStatusFormatAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS_FORMAT,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileCompareAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_COMPARE,true),MsgHandleInfo);
	
}

int CServerManagerAckMsgHandler::HandleMsgLoginAck(CSmartStruct& Packet)
{
	short		Result;
	
	
	Result=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_LOGIN_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		
		}
	}
		

	return LoginAck( Result );
}
int CServerManagerAckMsgHandler::HandleMsgGetServiceListAck(CSmartStruct& Packet)
{
	short			Result;
	CSmartStruct	ServiceListData;
	
	
	Result=0;
	ServiceListData.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_SERVICE_LIST_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_GET_SERVICE_LIST_ACK_SERVICE_LIST_DATA:
			{
				ServiceListData.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return GetServiceListAck( Result , ServiceListData );
}
int CServerManagerAckMsgHandler::HandleMsgGetProcessListAck(CSmartStruct& Packet)
{
	short			Result;
	short			Page;
	short			PageLen;
	short			TotalPage;
	CSmartStruct	ProcessListData;
	
	
	Result=0;
	Page=0;
	PageLen=0;
	TotalPage=0;
	ProcessListData.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_PROCESS_LIST_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_GET_PROCESS_LIST_ACK_PAGE:
			{
				Page=Value;
		
			}
			break;
		case SST_GET_PROCESS_LIST_ACK_PAGE_LEN:
			{
				PageLen=Value;
		
			}
			break;
		case SST_GET_PROCESS_LIST_ACK_TOTAL_PAGE:
			{
				TotalPage=Value;
		
			}
			break;
		case SST_GET_PROCESS_LIST_ACK_PROCESS_LIST_DATA:
			{
				ProcessListData.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return GetProcessListAck( Result , Page , PageLen , TotalPage , ProcessListData );
}
int CServerManagerAckMsgHandler::HandleMsgGetNetAdapterListAck(CSmartStruct& Packet)
{
	short			Result;
	CSmartStruct	NetAdapterListData;
	
	
	Result=0;
	NetAdapterListData.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_NET_ADAPTER_LIST_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_GET_NET_ADAPTER_LIST_ACK_NET_ADAPTER_LIST_DATA:
			{
				NetAdapterListData.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return GetNetAdapterListAck( Result , NetAdapterListData );
}
int CServerManagerAckMsgHandler::HandleMsgGetServiceInfoAck(CSmartStruct& Packet)
{
	short			Result;
	CSmartStruct	ServiceInfoPacket;
	
	
	Result=0;
	ServiceInfoPacket.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_SERVICE_INFO_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_GET_SERVICE_INFO_ACK_SERVICE_INFO_PACKET:
			{
				ServiceInfoPacket.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return GetServiceInfoAck( Result , ServiceInfoPacket );
}
int CServerManagerAckMsgHandler::HandleMsgServiceStartupAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ServiceID;
	
	
	Result=0;
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SERVICE_STARTUP_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_SERVICE_STARTUP_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return ServiceStartupAck( Result , ServiceID );
}
int CServerManagerAckMsgHandler::HandleMsgServiceShutdownAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ServiceID;
	
	
	Result=0;
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SERVICE_SHUTDOWN_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_SERVICE_SHUTDOWN_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return ServiceShutdownAck( Result , ServiceID );
}
int CServerManagerAckMsgHandler::HandleMsgRunProgramAck(CSmartStruct& Packet)
{
	short		Result;
	
	
	Result=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_RUN_PROGRAM_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		
		}
	}
		

	return RunProgramAck( Result );
}
int CServerManagerAckMsgHandler::HandleMsgProcessShutdownAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ProcessID;
	
	
	Result=0;
	ProcessID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_PROCESS_SHUTDOWN_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_PROCESS_SHUTDOWN_ACK_PROCESS_ID:
			{
				ProcessID=Value;
		
			}
			break;
		
		}
	}
		

	return ProcessShutdownAck( Result , ProcessID );
}
int CServerManagerAckMsgHandler::HandleMsgExecuteScriptAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ServiceID;
	int			ErrorCode;
	int			LastLine;
	
	
	Result=0;
	ServiceID=0;
	ErrorCode=0;
	LastLine=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_EXECUTE_SCRIPT_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_EXECUTE_SCRIPT_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_EXECUTE_SCRIPT_ACK_ERROR_CODE:
			{
				ErrorCode=Value;
		
			}
			break;
		case SST_EXECUTE_SCRIPT_ACK_LAST_LINE:
			{
				LastLine=Value;
		
			}
			break;
		
		}
	}
		

	return ExecuteScriptAck( Result , ServiceID , ErrorCode , LastLine );
}
int CServerManagerAckMsgHandler::HandleMsgBrowseServiceDirAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CEasyString		Dir;
	short			Page;
	short			PageLen;
	short			TotalPage;
	CSmartStruct	FileListData;
	
	
	Result=0;
	ServiceID=0;
	Dir.Clear();
	Page=0;
	PageLen=0;
	TotalPage=0;
	FileListData.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_BROWSE_SERVICE_DIR_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_ACK_DIR:
			{
				Value.GetString(Dir);
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_ACK_PAGE:
			{
				Page=Value;
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_ACK_PAGE_LEN:
			{
				PageLen=Value;
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_ACK_TOTAL_PAGE:
			{
				TotalPage=Value;
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_ACK_FILE_LIST_DATA:
			{
				FileListData.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return BrowseServiceDirAck( Result , ServiceID , Dir , Page , PageLen , TotalPage , FileListData );
}
int CServerManagerAckMsgHandler::HandleMsgFileDownloadStartAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CEasyString		FilePath;
	UINT64			FileSize;
	
	
	Result=0;
	ServiceID=0;
	FilePath.Clear();
	FileSize=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_DOWNLOAD_START_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_START_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_START_ACK_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		case SST_FILE_DOWNLOAD_START_ACK_FILE_SIZE:
			{
				FileSize=Value;
		
			}
			break;
		
		}
	}
		

	return FileDownloadStartAck( Result , ServiceID , FilePath , FileSize );
}
int CServerManagerAckMsgHandler::HandleMsgFileDownloadDataAck(CSmartStruct& Packet)
{
	short			Result;
	UINT64			Offset;
	UINT			Length;
	CEasyBuffer		FileData;
	
	
	Result=0;
	Offset=0;
	Length=0;
	FileData.Destory();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_DOWNLOAD_DATA_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_DATA_ACK_OFFSET:
			{
				Offset=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_DATA_ACK_LENGTH:
			{
				Length=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_DATA_ACK_FILE_DATA:
			{
				FileData.Create(Value.GetLength());
		FileData.PushBack((LPCSTR)Value,Value.GetLength());
		
			}
			break;
		
		}
	}
		

	return FileDownloadDataAck( Result , Offset , Length , FileData );
}
int CServerManagerAckMsgHandler::HandleMsgFileDownloadEndAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		FileLastWriteTime;
	
	
	Result=0;
	FileLastWriteTime=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_DOWNLOAD_END_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_END_ACK_FILE_LAST_WRITE_TIME:
			{
				FileLastWriteTime=Value;
		
			}
			break;
		
		}
	}
		

	return FileDownloadEndAck( Result , FileLastWriteTime );
}
int CServerManagerAckMsgHandler::HandleMsgFileUploadStartAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CEasyString		FilePath;
	UINT64			FileSize;
	
	
	Result=0;
	ServiceID=0;
	FilePath.Clear();
	FileSize=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_UPLOAD_START_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_FILE_UPLOAD_START_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_FILE_UPLOAD_START_ACK_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		case SST_FILE_UPLOAD_START_ACK_FILE_SIZE:
			{
				FileSize=Value;
		
			}
			break;
		
		}
	}
		

	return FileUploadStartAck( Result , ServiceID , FilePath , FileSize );
}
int CServerManagerAckMsgHandler::HandleMsgFileUploadDataAck(CSmartStruct& Packet)
{
	short		Result;
	UINT64		Offset;
	UINT		Length;
	
	
	Result=0;
	Offset=0;
	Length=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_UPLOAD_DATA_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_FILE_UPLOAD_DATA_ACK_OFFSET:
			{
				Offset=Value;
		
			}
			break;
		case SST_FILE_UPLOAD_DATA_ACK_LENGTH:
			{
				Length=Value;
		
			}
			break;
		
		}
	}
		

	return FileUploadDataAck( Result , Offset , Length );
}
int CServerManagerAckMsgHandler::HandleMsgFileUploadEndAck(CSmartStruct& Packet)
{
	short		Result;
	
	
	Result=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_UPLOAD_END_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		
		}
	}
		

	return FileUploadEndAck( Result );
}
int CServerManagerAckMsgHandler::HandleMsgCreateDirAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CEasyString		Dir;
	
	
	Result=0;
	ServiceID=0;
	Dir.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_CREATE_DIR_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_CREATE_DIR_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_CREATE_DIR_ACK_DIR:
			{
				Value.GetString(Dir);
		
			}
			break;
		
		}
	}
		

	return CreateDirAck( Result , ServiceID , Dir );
}
int CServerManagerAckMsgHandler::HandleMsgDeleteFileAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CEasyString		FilePath;
	
	
	Result=0;
	ServiceID=0;
	FilePath.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_DELETE_FILE_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_DELETE_FILE_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_DELETE_FILE_ACK_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		
		}
	}
		

	return DeleteFileAck( Result , ServiceID , FilePath );
}
int CServerManagerAckMsgHandler::HandleMsgChangeFileModeAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CEasyString		FilePath;
	UINT			Mode;
	
	
	Result=0;
	ServiceID=0;
	FilePath.Clear();
	Mode=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_CHANGE_FILE_MODE_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_CHANGE_FILE_MODE_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_CHANGE_FILE_MODE_ACK_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		case SST_CHANGE_FILE_MODE_ACK_MODE:
			{
				Mode=Value;
		
			}
			break;
		
		}
	}
		

	return ChangeFileModeAck( Result , ServiceID , FilePath , Mode );
}
int CServerManagerAckMsgHandler::HandleMsgAddServiceAck(CSmartStruct& Packet)
{
	short		Result;
	
	
	Result=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_ADD_SERVICE_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		
		}
	}
		

	return AddServiceAck( Result );
}
int CServerManagerAckMsgHandler::HandleMsgEditServiceAck(CSmartStruct& Packet)
{
	short		Result;
	
	
	Result=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_EDIT_SERVICE_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		
		}
	}
		

	return EditServiceAck( Result );
}
int CServerManagerAckMsgHandler::HandleMsgDeleteServiceAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ServiceID;
	
	
	Result=0;
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_DELETE_SERVICE_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_DELETE_SERVICE_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return DeleteServiceAck( Result , ServiceID );
}
int CServerManagerAckMsgHandler::HandleMsgSendCommandAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ServiceID;
	
	
	Result=0;
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SEND_COMMAND_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_SEND_COMMAND_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return SendCommandAck( Result , ServiceID );
}
int CServerManagerAckMsgHandler::HandleMsgEnableLogRecvAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ServiceID;
	bool		Enable;
	
	
	Result=0;
	ServiceID=0;
	Enable=false;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_ENABLE_LOG_RECV_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_ENABLE_LOG_RECV_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_ENABLE_LOG_RECV_ACK_ENABLE:
			{
				Enable=Value;
		
			}
			break;
		
		}
	}
		

	return EnableLogRecvAck( Result , ServiceID , Enable );
}
int CServerManagerAckMsgHandler::HandleMsgConsoleLogNotify(CSmartStruct& Packet)
{
	UINT		ServiceID;
	LPCTSTR		LogMsg;
	
	
	ServiceID=0;
	LogMsg=NULL;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_CONSOLE_LOG_NOTIFY_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_CONSOLE_LOG_NOTIFY_LOG_MSG:
			{
				LogMsg=Value;
		
			}
			break;
		
		}
	}
		

	return ConsoleLogNotify( ServiceID , LogMsg );
}
int CServerManagerAckMsgHandler::HandleMsgGetServerStatusAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CSmartStruct	StatusListPacket;
	
	
	Result=0;
	ServiceID=0;
	StatusListPacket.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_SERVER_STATUS_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_GET_SERVER_STATUS_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_GET_SERVER_STATUS_ACK_STATUS_LIST_PACKET:
			{
				StatusListPacket.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return GetServerStatusAck( Result , ServiceID , StatusListPacket );
}
int CServerManagerAckMsgHandler::HandleMsgGetServerStatusFormatAck(CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CSmartStruct	StatusFormatPacket;
	
	
	Result=0;
	ServiceID=0;
	StatusFormatPacket.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_SERVER_STATUS_FORMAT_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_GET_SERVER_STATUS_FORMAT_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_GET_SERVER_STATUS_FORMAT_ACK_STATUS_FORMAT_PACKET:
			{
				StatusFormatPacket.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return GetServerStatusFormatAck( Result , ServiceID , StatusFormatPacket );
}
int CServerManagerAckMsgHandler::HandleMsgFileCompareAck(CSmartStruct& Packet)
{
	short		Result;
	UINT		ServiceID;
	LPCTSTR		FilePath;
	
	
	Result=0;
	ServiceID=0;
	FilePath=NULL;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_COMPARE_ACK_RESULT:
			{
				Result=Value;
		
			}
			break;
		case SST_FILE_COMPARE_ACK_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_FILE_COMPARE_ACK_FILE_PATH:
			{
				FilePath=Value;
		
			}
			break;
		
		}
	}
		

	return FileCompareAck( Result , ServiceID , FilePath );
}
