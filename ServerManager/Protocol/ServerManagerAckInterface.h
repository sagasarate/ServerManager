#pragma once



//
class IServerManagerAck:public CBaseMsgHandler
{
public:
	
	
	
	
	/*************************************************
	函数名:	LoginAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int LoginAck(short Result ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServiceListAck
	用途:	
	参数:
		Result				
		ServiceListData				
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceListAck(short Result ,const CSmartStruct& ServiceListData ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetProcessListAck
	用途:	
	参数:
		Result				
		Page				
		PageLen				
		TotalPage				
		ProcessListData				
	返回值:无特别意义
	*************************************************/
	virtual int GetProcessListAck(short Result ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& ProcessListData ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetNetAdapterListAck
	用途:	
	参数:
		Result				
		NetAdapterListData				
	返回值:无特别意义
	*************************************************/
	virtual int GetNetAdapterListAck(short Result ,const CSmartStruct& NetAdapterListData ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServiceInfoAck
	用途:	
	参数:
		Result				
		ServiceInfoPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceInfoAck(short Result ,const CSmartStruct& ServiceInfoPacket ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ServiceStartupAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceStartupAck(short Result ,UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ServiceShutdownAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceShutdownAck(short Result ,UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	RunProgramAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int RunProgramAck(short Result ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ProcessShutdownAck
	用途:	
	参数:
		Result				
		ProcessID				
	返回值:无特别意义
	*************************************************/
	virtual int ProcessShutdownAck(short Result ,UINT ProcessID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ExecuteScriptAck
	用途:	
	参数:
		Result				
		ServiceID				
		ErrorCode				
		LastLine				
	返回值:无特别意义
	*************************************************/
	virtual int ExecuteScriptAck(short Result ,UINT ServiceID ,int ErrorCode ,int LastLine ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	BrowseServiceDirAck
	用途:	
	参数:
		Result				
		ServiceID				
		Dir				
		Page				
		PageLen				
		TotalPage				
		FileListData				
	返回值:无特别意义
	*************************************************/
	virtual int BrowseServiceDirAck(short Result ,UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& FileListData ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileDownloadStartAck
	用途:	
	参数:
		Result				
		ServiceID				
		FilePath				
		FileSize				
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadStartAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileDownloadDataAck
	用途:	
	参数:
		Result				
		Offset				
		Length				
		FileData				
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadDataAck(short Result ,UINT64 Offset ,UINT Length ,const CEasyBuffer& FileData ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileDownloadEndAck
	用途:	
	参数:
		Result				
		FileLastWriteTime				
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadEndAck(short Result ,UINT FileLastWriteTime ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileUploadStartAck
	用途:	
	参数:
		Result				
		ServiceID				
		FilePath				
		FileSize				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadStartAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileUploadDataAck
	用途:	
	参数:
		Result				
		Offset				
		Length				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadDataAck(short Result ,UINT64 Offset ,UINT Length ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileUploadEndAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadEndAck(short Result ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	CreateDirAck
	用途:	
	参数:
		Result				
		ServiceID				
		Dir				
	返回值:无特别意义
	*************************************************/
	virtual int CreateDirAck(short Result ,UINT ServiceID ,const CEasyString& Dir ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	DeleteFileAck
	用途:	
	参数:
		Result				
		ServiceID				
		FilePath				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteFileAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ChangeFileModeAck
	用途:	
	参数:
		Result				
		ServiceID				
		FilePath				
		Mode				
	返回值:无特别意义
	*************************************************/
	virtual int ChangeFileModeAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT Mode ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	AddServiceAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int AddServiceAck(short Result ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	EditServiceAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int EditServiceAck(short Result ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	DeleteServiceAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteServiceAck(short Result ,UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	SendCommandAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int SendCommandAck(short Result ,UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	EnableLogRecvAck
	用途:	
	参数:
		Result				
		ServiceID				
		Enable				
	返回值:无特别意义
	*************************************************/
	virtual int EnableLogRecvAck(short Result ,UINT ServiceID ,bool Enable ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ConsoleLogNotify
	用途:	
	参数:
		ServiceID				
		LogMsg				
	返回值:无特别意义
	*************************************************/
	virtual int ConsoleLogNotify(UINT ServiceID ,LPCTSTR LogMsg ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServerStatusAck
	用途:	
	参数:
		Result				
		ServiceID				
		StatusListPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatusAck(short Result ,UINT ServiceID ,const CSmartStruct& StatusListPacket ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServerStatusFormatAck
	用途:	
	参数:
		Result				
		ServiceID				
		StatusFormatPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatusFormatAck(short Result ,UINT ServiceID ,const CSmartStruct& StatusFormatPacket ) {return COMMON_RESULT_FAILED;}
		
	
	
protected:	
	
	enum SERVER_MANAGER_ACK_LOGIN_ACK_MEMBER_IDS
	{
		SST_LOGIN_ACK_RESULT=3,
	
	};
	enum SERVER_MANAGER_ACK_GET_SERVICE_LIST_ACK_MEMBER_IDS
	{
		SST_GET_SERVICE_LIST_ACK_RESULT=1,
		SST_GET_SERVICE_LIST_ACK_SERVICE_LIST_DATA=2,
	
	};
	enum SERVER_MANAGER_ACK_GET_PROCESS_LIST_ACK_MEMBER_IDS
	{
		SST_GET_PROCESS_LIST_ACK_RESULT=1,
		SST_GET_PROCESS_LIST_ACK_PAGE=5,
		SST_GET_PROCESS_LIST_ACK_PAGE_LEN=6,
		SST_GET_PROCESS_LIST_ACK_TOTAL_PAGE=7,
		SST_GET_PROCESS_LIST_ACK_PROCESS_LIST_DATA=2,
	
	};
	enum SERVER_MANAGER_ACK_GET_NET_ADAPTER_LIST_ACK_MEMBER_IDS
	{
		SST_GET_NET_ADAPTER_LIST_ACK_RESULT=1,
		SST_GET_NET_ADAPTER_LIST_ACK_NET_ADAPTER_LIST_DATA=2,
	
	};
	enum SERVER_MANAGER_ACK_GET_SERVICE_INFO_ACK_MEMBER_IDS
	{
		SST_GET_SERVICE_INFO_ACK_RESULT=2,
		SST_GET_SERVICE_INFO_ACK_SERVICE_INFO_PACKET=3,
	
	};
	enum SERVER_MANAGER_ACK_SERVICE_STARTUP_ACK_MEMBER_IDS
	{
		SST_SERVICE_STARTUP_ACK_RESULT=2,
		SST_SERVICE_STARTUP_ACK_SERVICE_ID=3,
	
	};
	enum SERVER_MANAGER_ACK_SERVICE_SHUTDOWN_ACK_MEMBER_IDS
	{
		SST_SERVICE_SHUTDOWN_ACK_RESULT=2,
		SST_SERVICE_SHUTDOWN_ACK_SERVICE_ID=3,
	
	};
	enum SERVER_MANAGER_ACK_RUN_PROGRAM_ACK_MEMBER_IDS
	{
		SST_RUN_PROGRAM_ACK_RESULT=4,
	
	};
	enum SERVER_MANAGER_ACK_PROCESS_SHUTDOWN_ACK_MEMBER_IDS
	{
		SST_PROCESS_SHUTDOWN_ACK_RESULT=2,
		SST_PROCESS_SHUTDOWN_ACK_PROCESS_ID=3,
	
	};
	enum SERVER_MANAGER_ACK_EXECUTE_SCRIPT_ACK_MEMBER_IDS
	{
		SST_EXECUTE_SCRIPT_ACK_RESULT=4,
		SST_EXECUTE_SCRIPT_ACK_SERVICE_ID=7,
		SST_EXECUTE_SCRIPT_ACK_ERROR_CODE=5,
		SST_EXECUTE_SCRIPT_ACK_LAST_LINE=6,
	
	};
	enum SERVER_MANAGER_ACK_BROWSE_SERVICE_DIR_ACK_MEMBER_IDS
	{
		SST_BROWSE_SERVICE_DIR_ACK_RESULT=3,
		SST_BROWSE_SERVICE_DIR_ACK_SERVICE_ID=4,
		SST_BROWSE_SERVICE_DIR_ACK_DIR=5,
		SST_BROWSE_SERVICE_DIR_ACK_PAGE=8,
		SST_BROWSE_SERVICE_DIR_ACK_PAGE_LEN=9,
		SST_BROWSE_SERVICE_DIR_ACK_TOTAL_PAGE=10,
		SST_BROWSE_SERVICE_DIR_ACK_FILE_LIST_DATA=11,
	
	};
	enum SERVER_MANAGER_ACK_FILE_DOWNLOAD_START_ACK_MEMBER_IDS
	{
		SST_FILE_DOWNLOAD_START_ACK_RESULT=3,
		SST_FILE_DOWNLOAD_START_ACK_SERVICE_ID=4,
		SST_FILE_DOWNLOAD_START_ACK_FILE_PATH=5,
		SST_FILE_DOWNLOAD_START_ACK_FILE_SIZE=6,
	
	};
	enum SERVER_MANAGER_ACK_FILE_DOWNLOAD_DATA_ACK_MEMBER_IDS
	{
		SST_FILE_DOWNLOAD_DATA_ACK_RESULT=3,
		SST_FILE_DOWNLOAD_DATA_ACK_OFFSET=4,
		SST_FILE_DOWNLOAD_DATA_ACK_LENGTH=5,
		SST_FILE_DOWNLOAD_DATA_ACK_FILE_DATA=7,
	
	};
	enum SERVER_MANAGER_ACK_FILE_DOWNLOAD_END_ACK_MEMBER_IDS
	{
		SST_FILE_DOWNLOAD_END_ACK_RESULT=1,
		SST_FILE_DOWNLOAD_END_ACK_FILE_LAST_WRITE_TIME=2,
	
	};
	enum SERVER_MANAGER_ACK_FILE_UPLOAD_START_ACK_MEMBER_IDS
	{
		SST_FILE_UPLOAD_START_ACK_RESULT=3,
		SST_FILE_UPLOAD_START_ACK_SERVICE_ID=4,
		SST_FILE_UPLOAD_START_ACK_FILE_PATH=5,
		SST_FILE_UPLOAD_START_ACK_FILE_SIZE=7,
	
	};
	enum SERVER_MANAGER_ACK_FILE_UPLOAD_DATA_ACK_MEMBER_IDS
	{
		SST_FILE_UPLOAD_DATA_ACK_RESULT=5,
		SST_FILE_UPLOAD_DATA_ACK_OFFSET=6,
		SST_FILE_UPLOAD_DATA_ACK_LENGTH=7,
	
	};
	enum SERVER_MANAGER_ACK_FILE_UPLOAD_END_ACK_MEMBER_IDS
	{
		SST_FILE_UPLOAD_END_ACK_RESULT=1,
	
	};
	enum SERVER_MANAGER_ACK_CREATE_DIR_ACK_MEMBER_IDS
	{
		SST_CREATE_DIR_ACK_RESULT=3,
		SST_CREATE_DIR_ACK_SERVICE_ID=4,
		SST_CREATE_DIR_ACK_DIR=5,
	
	};
	enum SERVER_MANAGER_ACK_DELETE_FILE_ACK_MEMBER_IDS
	{
		SST_DELETE_FILE_ACK_RESULT=3,
		SST_DELETE_FILE_ACK_SERVICE_ID=4,
		SST_DELETE_FILE_ACK_FILE_PATH=5,
	
	};
	enum SERVER_MANAGER_ACK_CHANGE_FILE_MODE_ACK_MEMBER_IDS
	{
		SST_CHANGE_FILE_MODE_ACK_RESULT=4,
		SST_CHANGE_FILE_MODE_ACK_SERVICE_ID=5,
		SST_CHANGE_FILE_MODE_ACK_FILE_PATH=6,
		SST_CHANGE_FILE_MODE_ACK_MODE=7,
	
	};
	enum SERVER_MANAGER_ACK_ADD_SERVICE_ACK_MEMBER_IDS
	{
		SST_ADD_SERVICE_ACK_RESULT=2,
	
	};
	enum SERVER_MANAGER_ACK_EDIT_SERVICE_ACK_MEMBER_IDS
	{
		SST_EDIT_SERVICE_ACK_RESULT=2,
	
	};
	enum SERVER_MANAGER_ACK_DELETE_SERVICE_ACK_MEMBER_IDS
	{
		SST_DELETE_SERVICE_ACK_RESULT=2,
		SST_DELETE_SERVICE_ACK_SERVICE_ID=3,
	
	};
	enum SERVER_MANAGER_ACK_SEND_COMMAND_ACK_MEMBER_IDS
	{
		SST_SEND_COMMAND_ACK_RESULT=3,
		SST_SEND_COMMAND_ACK_SERVICE_ID=4,
	
	};
	enum SERVER_MANAGER_ACK_ENABLE_LOG_RECV_ACK_MEMBER_IDS
	{
		SST_ENABLE_LOG_RECV_ACK_RESULT=2,
		SST_ENABLE_LOG_RECV_ACK_SERVICE_ID=5,
		SST_ENABLE_LOG_RECV_ACK_ENABLE=3,
	
	};
	enum SERVER_MANAGER_ACK_CONSOLE_LOG_NOTIFY_MEMBER_IDS
	{
		SST_CONSOLE_LOG_NOTIFY_SERVICE_ID=2,
		SST_CONSOLE_LOG_NOTIFY_LOG_MSG=1,
	
	};
	enum SERVER_MANAGER_ACK_GET_SERVER_STATUS_ACK_MEMBER_IDS
	{
		SST_GET_SERVER_STATUS_ACK_RESULT=2,
		SST_GET_SERVER_STATUS_ACK_SERVICE_ID=5,
		SST_GET_SERVER_STATUS_ACK_STATUS_LIST_PACKET=3,
	
	};
	enum SERVER_MANAGER_ACK_GET_SERVER_STATUS_FORMAT_ACK_MEMBER_IDS
	{
		SST_GET_SERVER_STATUS_FORMAT_ACK_RESULT=1,
		SST_GET_SERVER_STATUS_FORMAT_ACK_SERVICE_ID=4,
		SST_GET_SERVER_STATUS_FORMAT_ACK_STATUS_FORMAT_PACKET=2,
	
	};
};
