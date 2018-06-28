#include "stdafx.h"

CServerManagerMsgHandler::CServerManagerMsgHandler(void)
{
}

CServerManagerMsgHandler::~CServerManagerMsgHandler(void)
{
}

void CServerManagerMsgHandler::InitMsgMap(CEasyMap<MSG_ID_TYPE,MSG_HANDLE_INFO>& MsgMap)
{
	MSG_HANDLE_INFO MsgHandleInfo;
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgLogin;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_LOGIN,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgGetServiceList;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_LIST,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgGetProcessList;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_PROCESS_LIST,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgGetNetAdapterList;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_NET_ADAPTER_LIST,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgGetServiceInfo;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVICE_INFO,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgServiceStartup;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_STARTUP,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgServiceShutdown;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SERVICE_SHUTDOWN,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgRunProgram;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_RUN_PROGRAM,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgProcessShutdown;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_PROCESS_SHUTDOWN,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgExecuteScript;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EXECUTE_SCRIPT,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgBrowseServiceDir;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_BROWSE_SERVICE_DIR,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgFileDownloadStart;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_START,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgFileDownloadData;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_DATA,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgFileDownloadEnd;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_DOWNLOAD_END,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgFileUploadStart;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_START,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgFileUploadData;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_DATA,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgFileUploadEnd;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_FILE_UPLOAD_END,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgCreateDir;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CREATE_DIR,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgDeleteFile;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_FILE,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgChangeFileMode;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_CHANGE_FILE_MODE,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgAddService;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ADD_SERVICE,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgEditService;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_EDIT_SERVICE,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgDeleteService;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_DELETE_SERVICE,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgSendCommand;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_SEND_COMMAND,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgEnableLogRecv;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_ENABLE_LOG_RECV,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgGetServerStatus;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgGetAllServerStatus;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_ALL_SERVER_STATUS,false),MsgHandleInfo);
	MsgHandleInfo.pObject=this;
	MsgHandleInfo.pFN=(MSG_HANDLE_FN)&CServerManagerMsgHandler::HandleMsgGetServerStatusFormat;
	MsgMap.Insert(MAKE_MSG_ID(MODULE_ID_SVR_MGR,SVR_MGR_INTERFACE_SERVER_MANAGER,IServerManager::METHOD_GET_SERVER_STATUS_FORMAT,false),MsgHandleInfo);
	
}

int CServerManagerMsgHandler::HandleMsgLogin(CSmartStruct& Packet)
{
	LPCTSTR		UserName;
	LPCTSTR		Password;
	
	
	UserName=NULL;
	Password=NULL;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_LOGIN_USER_NAME:
			{
				UserName=Value;
		
			}
			break;
		case SST_LOGIN_PASSWORD:
			{
				Password=Value;
		
			}
			break;
		
		}
	}
		

	return Login( UserName , Password );
}
int CServerManagerMsgHandler::HandleMsgGetServiceList(CSmartStruct& Packet)
{
	
	
	

	

	return GetServiceList();
}
int CServerManagerMsgHandler::HandleMsgGetProcessList(CSmartStruct& Packet)
{
	short		Page;
	short		PageLen;
	
	
	Page=0;
	PageLen=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_PROCESS_LIST_PAGE:
			{
				Page=Value;
		
			}
			break;
		case SST_GET_PROCESS_LIST_PAGE_LEN:
			{
				PageLen=Value;
		
			}
			break;
		
		}
	}
		

	return GetProcessList( Page , PageLen );
}
int CServerManagerMsgHandler::HandleMsgGetNetAdapterList(CSmartStruct& Packet)
{
	
	
	

	

	return GetNetAdapterList();
}
int CServerManagerMsgHandler::HandleMsgGetServiceInfo(CSmartStruct& Packet)
{
	UINT	ServiceID;
	
	
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_SERVICE_INFO_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return GetServiceInfo( ServiceID );
}
int CServerManagerMsgHandler::HandleMsgServiceStartup(CSmartStruct& Packet)
{
	UINT	ServiceID;
	
	
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SERVICE_STARTUP_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return ServiceStartup( ServiceID );
}
int CServerManagerMsgHandler::HandleMsgServiceShutdown(CSmartStruct& Packet)
{
	UINT	ServiceID;
	BYTE	ShutdownType;
	
	
	ServiceID=0;
	ShutdownType=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SERVICE_SHUTDOWN_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_SERVICE_SHUTDOWN_SHUTDOWN_TYPE:
			{
				ShutdownType=Value;
		
			}
			break;
		
		}
	}
		

	return ServiceShutdown( ServiceID , ShutdownType );
}
int CServerManagerMsgHandler::HandleMsgRunProgram(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		FilePath;
	CEasyString		WorkDir;
	CEasyString		Param;
	
	
	ServiceID=0;
	FilePath.Clear();
	WorkDir.Clear();
	Param.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_RUN_PROGRAM_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_RUN_PROGRAM_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		case SST_RUN_PROGRAM_WORK_DIR:
			{
				Value.GetString(WorkDir);
		
			}
			break;
		case SST_RUN_PROGRAM_PARAM:
			{
				Value.GetString(Param);
		
			}
			break;
		
		}
	}
		

	return RunProgram( ServiceID , FilePath , WorkDir , Param );
}
int CServerManagerMsgHandler::HandleMsgProcessShutdown(CSmartStruct& Packet)
{
	UINT	ProcessID;
	BYTE	ShutdownType;
	
	
	ProcessID=0;
	ShutdownType=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_PROCESS_SHUTDOWN_PROCESS_ID:
			{
				ProcessID=Value;
		
			}
			break;
		case SST_PROCESS_SHUTDOWN_SHUTDOWN_TYPE:
			{
				ShutdownType=Value;
		
			}
			break;
		
		}
	}
		

	return ProcessShutdown( ProcessID , ShutdownType );
}
int CServerManagerMsgHandler::HandleMsgExecuteScript(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		Script;
	bool			FromFile;
	
	
	ServiceID=0;
	Script.Clear();
	FromFile=false;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_EXECUTE_SCRIPT_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_EXECUTE_SCRIPT_SCRIPT:
			{
				Value.GetString(Script);
		
			}
			break;
		case SST_EXECUTE_SCRIPT_FROM_FILE:
			{
				FromFile=Value;
		
			}
			break;
		
		}
	}
		

	return ExecuteScript( ServiceID , Script , FromFile );
}
int CServerManagerMsgHandler::HandleMsgBrowseServiceDir(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		Dir;
	short			Page;
	short			PageLen;
	
	
	ServiceID=0;
	Dir.Clear();
	Page=0;
	PageLen=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_BROWSE_SERVICE_DIR_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_DIR:
			{
				Value.GetString(Dir);
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_PAGE:
			{
				Page=Value;
		
			}
			break;
		case SST_BROWSE_SERVICE_DIR_PAGE_LEN:
			{
				PageLen=Value;
		
			}
			break;
		
		}
	}
		

	return BrowseServiceDir( ServiceID , Dir , Page , PageLen );
}
int CServerManagerMsgHandler::HandleMsgFileDownloadStart(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		FilePath;
	
	
	ServiceID=0;
	FilePath.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_DOWNLOAD_START_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_START_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		
		}
	}
		

	return FileDownloadStart( ServiceID , FilePath );
}
int CServerManagerMsgHandler::HandleMsgFileDownloadData(CSmartStruct& Packet)
{
	UINT64		Offset;
	UINT		Length;
	
	
	Offset=0;
	Length=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_DOWNLOAD_DATA_OFFSET:
			{
				Offset=Value;
		
			}
			break;
		case SST_FILE_DOWNLOAD_DATA_LENGTH:
			{
				Length=Value;
		
			}
			break;
		
		}
	}
		

	return FileDownloadData( Offset , Length );
}
int CServerManagerMsgHandler::HandleMsgFileDownloadEnd(CSmartStruct& Packet)
{
	
	
	

	

	return FileDownloadEnd();
}
int CServerManagerMsgHandler::HandleMsgFileUploadStart(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		FilePath;
	
	
	ServiceID=0;
	FilePath.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_UPLOAD_START_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_FILE_UPLOAD_START_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		
		}
	}
		

	return FileUploadStart( ServiceID , FilePath );
}
int CServerManagerMsgHandler::HandleMsgFileUploadData(CSmartStruct& Packet)
{
	UINT64			Offset;
	UINT			Length;
	CEasyBuffer		FileData;
	
	
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
		case SST_FILE_UPLOAD_DATA_OFFSET:
			{
				Offset=Value;
		
			}
			break;
		case SST_FILE_UPLOAD_DATA_LENGTH:
			{
				Length=Value;
		
			}
			break;
		case SST_FILE_UPLOAD_DATA_FILE_DATA:
			{
				FileData.Create(Value.GetLength());
		FileData.PushBack((LPCSTR)Value,Value.GetLength());
		
			}
			break;
		
		}
	}
		

	return FileUploadData( Offset , Length , FileData );
}
int CServerManagerMsgHandler::HandleMsgFileUploadEnd(CSmartStruct& Packet)
{
	UINT	FileLastWriteTime;
	
	
	FileLastWriteTime=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_FILE_UPLOAD_END_FILE_LAST_WRITE_TIME:
			{
				FileLastWriteTime=Value;
		
			}
			break;
		
		}
	}
		

	return FileUploadEnd( FileLastWriteTime );
}
int CServerManagerMsgHandler::HandleMsgCreateDir(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		Dir;
	
	
	ServiceID=0;
	Dir.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_CREATE_DIR_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_CREATE_DIR_DIR:
			{
				Value.GetString(Dir);
		
			}
			break;
		
		}
	}
		

	return CreateDir( ServiceID , Dir );
}
int CServerManagerMsgHandler::HandleMsgDeleteFile(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		FilePath;
	bool			IsRecursive;
	
	
	ServiceID=0;
	FilePath.Clear();
	IsRecursive=false;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_DELETE_FILE_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_DELETE_FILE_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		case SST_DELETE_FILE_IS_RECURSIVE:
			{
				IsRecursive=Value;
		
			}
			break;
		
		}
	}
		

	return DeleteFile( ServiceID , FilePath , IsRecursive );
}
int CServerManagerMsgHandler::HandleMsgChangeFileMode(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CEasyString		FilePath;
	UINT			Mode;
	
	
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
		case SST_CHANGE_FILE_MODE_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_CHANGE_FILE_MODE_FILE_PATH:
			{
				Value.GetString(FilePath);
		
			}
			break;
		case SST_CHANGE_FILE_MODE_MODE:
			{
				Mode=Value;
		
			}
			break;
		
		}
	}
		

	return ChangeFileMode( ServiceID , FilePath , Mode );
}
int CServerManagerMsgHandler::HandleMsgAddService(CSmartStruct& Packet)
{
	CSmartStruct	ServiceInfo;
	
	
	ServiceInfo.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_ADD_SERVICE_SERVICE_INFO:
			{
				ServiceInfo.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return AddService( ServiceInfo );
}
int CServerManagerMsgHandler::HandleMsgEditService(CSmartStruct& Packet)
{
	CSmartStruct	ServiceInfo;
	
	
	ServiceInfo.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_EDIT_SERVICE_SERVICE_INFO:
			{
				ServiceInfo.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return EditService( ServiceInfo );
}
int CServerManagerMsgHandler::HandleMsgDeleteService(CSmartStruct& Packet)
{
	UINT	ServiceID;
	
	
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_DELETE_SERVICE_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return DeleteService( ServiceID );
}
int CServerManagerMsgHandler::HandleMsgSendCommand(CSmartStruct& Packet)
{
	UINT		ServiceID;
	LPCTSTR		Command;
	
	
	ServiceID=0;
	Command=NULL;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SEND_COMMAND_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_SEND_COMMAND_COMMAND:
			{
				Command=Value;
		
			}
			break;
		
		}
	}
		

	return SendCommand( ServiceID , Command );
}
int CServerManagerMsgHandler::HandleMsgEnableLogRecv(CSmartStruct& Packet)
{
	UINT	ServiceID;
	bool	Enable;
	
	
	ServiceID=0;
	Enable=false;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_ENABLE_LOG_RECV_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_ENABLE_LOG_RECV_ENABLE:
			{
				Enable=Value;
		
			}
			break;
		
		}
	}
		

	return EnableLogRecv( ServiceID , Enable );
}
int CServerManagerMsgHandler::HandleMsgGetServerStatus(CSmartStruct& Packet)
{
	UINT			ServiceID;
	CSmartStruct	StatusListPacket;
	
	
	ServiceID=0;
	StatusListPacket.Clear();
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_SERVER_STATUS_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		case SST_GET_SERVER_STATUS_STATUS_LIST_PACKET:
			{
				StatusListPacket.CloneFrom(Value);
		
			}
			break;
		
		}
	}
		

	return GetServerStatus( ServiceID , StatusListPacket );
}
int CServerManagerMsgHandler::HandleMsgGetAllServerStatus(CSmartStruct& Packet)
{
	UINT	ServiceID;
	
	
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_ALL_SERVER_STATUS_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return GetAllServerStatus( ServiceID );
}
int CServerManagerMsgHandler::HandleMsgGetServerStatusFormat(CSmartStruct& Packet)
{
	UINT	ServiceID;
	
	
	ServiceID=0;
	

	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_GET_SERVER_STATUS_FORMAT_SERVICE_ID:
			{
				ServiceID=Value;
		
			}
			break;
		
		}
	}
		

	return GetServerStatusFormat( ServiceID );
}
