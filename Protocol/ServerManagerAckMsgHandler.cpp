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
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileDownloadFinishAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_FINISH,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileUploadStartAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_START,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileUploadDataAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_DATA,true),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerAckMsgHandler::HandleMsgFileUploadFinishAck;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_FINISH,true),MsgHandleInfo);
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

int CServerManagerAckMsgHandler::HandleMsgLoginAck(const CSmartStruct& Packet)
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
		

	return LoginAck(Result);
}
int CServerManagerAckMsgHandler::HandleMsgGetServiceListAck(const CSmartStruct& Packet)
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
				ServiceListData=Value;
			}
			break;
		
		}
	}
		

	return GetServiceListAck(Result,ServiceListData);
}
int CServerManagerAckMsgHandler::HandleMsgGetProcessListAck(const CSmartStruct& Packet)
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
				ProcessListData=Value;
			}
			break;
		
		}
	}
		

	return GetProcessListAck(Result,Page,PageLen,TotalPage,ProcessListData);
}
int CServerManagerAckMsgHandler::HandleMsgGetNetAdapterListAck(const CSmartStruct& Packet)
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
				NetAdapterListData=Value;
			}
			break;
		
		}
	}
		

	return GetNetAdapterListAck(Result,NetAdapterListData);
}
int CServerManagerAckMsgHandler::HandleMsgGetServiceInfoAck(const CSmartStruct& Packet)
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
				ServiceInfoPacket=Value;
			}
			break;
		
		}
	}
		

	return GetServiceInfoAck(Result,ServiceInfoPacket);
}
int CServerManagerAckMsgHandler::HandleMsgServiceStartupAck(const CSmartStruct& Packet)
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
		

	return ServiceStartupAck(Result,ServiceID);
}
int CServerManagerAckMsgHandler::HandleMsgServiceShutdownAck(const CSmartStruct& Packet)
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
		

	return ServiceShutdownAck(Result,ServiceID);
}
int CServerManagerAckMsgHandler::HandleMsgRunProgramAck(const CSmartStruct& Packet)
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
		

	return RunProgramAck(Result);
}
int CServerManagerAckMsgHandler::HandleMsgProcessShutdownAck(const CSmartStruct& Packet)
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
		

	return ProcessShutdownAck(Result,ProcessID);
}
int CServerManagerAckMsgHandler::HandleMsgExecuteScriptAck(const CSmartStruct& Packet)
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
		

	return ExecuteScriptAck(Result,ServiceID,ErrorCode,LastLine);
}
int CServerManagerAckMsgHandler::HandleMsgBrowseServiceDirAck(const CSmartStruct& Packet)
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
				FileListData=Value;
			}
			break;
		
		}
	}
		

	return BrowseServiceDirAck(Result,ServiceID,Dir,Page,PageLen,TotalPage,FileListData);
}
int CServerManagerAckMsgHandler::HandleMsgFileDownloadStartAck(const CSmartStruct& Packet)
{
	short			Result;
	UINT			ServiceID;
	CEasyString		FilePath;
	UINT64			FileSize;
	UINT			FileLastWriteTime;
	
	
	Result=0;
	ServiceID=0;
	FilePath.Clear();
	FileSize=0;
	FileLastWriteTime=0;
	

	
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
		case SST_FILE_DOWNLOAD_START_ACK_FILE_LAST_WRITE_TIME:
			{
				FileLastWriteTime=Value;
			}
			break;
		
		}
	}
		

	return FileDownloadStartAck(Result,ServiceID,FilePath,FileSize,FileLastWriteTime);
}
int CServerManagerAckMsgHandler::HandleMsgFileDownloadDataAck(const CSmartStruct& Packet)
{
	short			Result;
	UINT64			Offset;
	UINT			Length;
	CEasyBuffer		FileData;
	bool			IsLast;
	
	
	Result=0;
	Offset=0;
	Length=0;
	FileData.SetTag(_T("StructData"));
	FileData.Destory();
	IsLast=false;
	

	
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
		FileData.PushBack(Value.GetValueData(),Value.GetLength());
			}
			break;
		case SST_FILE_DOWNLOAD_DATA_ACK_IS_LAST:
			{
				IsLast=Value;
			}
			break;
		
		}
	}
		

	return FileDownloadDataAck(Result,Offset,Length,FileData,IsLast);
}
int CServerManagerAckMsgHandler::HandleMsgFileDownloadFinishAck(const CSmartStruct& Packet)
{
	short			Result;
	CEasyString		MD5;
	
	
	Result=0;
	MD5.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_DOWNLOAD_FINISH_ACK_RESULT:
			{
				Result=Value;
			}
			break;
		case SST_FILE_DOWNLOAD_FINISH_ACK_MD5:
			{
				Value.GetString(MD5);
			}
			break;
		
		}
	}
		

	return FileDownloadFinishAck(Result,MD5);
}
int CServerManagerAckMsgHandler::HandleMsgFileUploadStartAck(const CSmartStruct& Packet)
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
		

	return FileUploadStartAck(Result,ServiceID,FilePath,FileSize);
}
int CServerManagerAckMsgHandler::HandleMsgFileUploadDataAck(const CSmartStruct& Packet)
{
	short		Result;
	UINT		Length;
	bool		IsLast;
	
	
	Result=0;
	Length=0;
	IsLast=false;
	

	
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
		case SST_FILE_UPLOAD_DATA_ACK_LENGTH:
			{
				Length=Value;
			}
			break;
		case SST_FILE_UPLOAD_DATA_ACK_IS_LAST:
			{
				IsLast=Value;
			}
			break;
		
		}
	}
		

	return FileUploadDataAck(Result,Length,IsLast);
}
int CServerManagerAckMsgHandler::HandleMsgFileUploadFinishAck(const CSmartStruct& Packet)
{
	short			Result;
	CEasyString		MD5;
	
	
	Result=0;
	MD5.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_UPLOAD_FINISH_ACK_RESULT:
			{
				Result=Value;
			}
			break;
		case SST_FILE_UPLOAD_FINISH_ACK_MD5:
			{
				Value.GetString(MD5);
			}
			break;
		
		}
	}
		

	return FileUploadFinishAck(Result,MD5);
}
int CServerManagerAckMsgHandler::HandleMsgCreateDirAck(const CSmartStruct& Packet)
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
		

	return CreateDirAck(Result,ServiceID,Dir);
}
int CServerManagerAckMsgHandler::HandleMsgDeleteFileAck(const CSmartStruct& Packet)
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
		

	return DeleteFileAck(Result,ServiceID,FilePath);
}
int CServerManagerAckMsgHandler::HandleMsgChangeFileModeAck(const CSmartStruct& Packet)
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
		

	return ChangeFileModeAck(Result,ServiceID,FilePath,Mode);
}
int CServerManagerAckMsgHandler::HandleMsgAddServiceAck(const CSmartStruct& Packet)
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
		

	return AddServiceAck(Result);
}
int CServerManagerAckMsgHandler::HandleMsgEditServiceAck(const CSmartStruct& Packet)
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
		

	return EditServiceAck(Result);
}
int CServerManagerAckMsgHandler::HandleMsgDeleteServiceAck(const CSmartStruct& Packet)
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
		

	return DeleteServiceAck(Result,ServiceID);
}
int CServerManagerAckMsgHandler::HandleMsgSendCommandAck(const CSmartStruct& Packet)
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
		

	return SendCommandAck(Result,ServiceID);
}
int CServerManagerAckMsgHandler::HandleMsgEnableLogRecvAck(const CSmartStruct& Packet)
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
		

	return EnableLogRecvAck(Result,ServiceID,Enable);
}
int CServerManagerAckMsgHandler::HandleMsgConsoleLogNotify(const CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		LogMsg;
	
	
	ServiceID=0;
	LogMsg.Clear();
	

	
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
				Value.GetString(LogMsg);
			}
			break;
		
		}
	}
		

	return ConsoleLogNotify(ServiceID,LogMsg);
}
int CServerManagerAckMsgHandler::HandleMsgGetServerStatusAck(const CSmartStruct& Packet)
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
				StatusListPacket=Value;
			}
			break;
		
		}
	}
		

	return GetServerStatusAck(Result,ServiceID,StatusListPacket);
}
int CServerManagerAckMsgHandler::HandleMsgGetServerStatusFormatAck(const CSmartStruct& Packet)
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
				StatusFormatPacket=Value;
			}
			break;
		
		}
	}
		

	return GetServerStatusFormatAck(Result,ServiceID,StatusFormatPacket);
}
int CServerManagerAckMsgHandler::HandleMsgFileCompareAck(const CSmartStruct& Packet)
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
				Value.GetString(FilePath);
			}
			break;
		
		}
	}
		

	return FileCompareAck(Result,ServiceID,FilePath);
}
