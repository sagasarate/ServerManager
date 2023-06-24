#pragma once



//
class IServerManager:public CBaseMsgHandler
{
public:
	
	enum SERVER_MANAGER_METHODS
	{
		METHOD_LOGIN=23,
		METHOD_GET_SERVICE_LIST=1,
		METHOD_GET_PROCESS_LIST=2,
		METHOD_GET_NET_ADAPTER_LIST=15,
		METHOD_GET_SERVICE_INFO=30,
		METHOD_SERVICE_STARTUP=3,
		METHOD_SERVICE_SHUTDOWN=4,
		METHOD_RUN_PROGRAM=14,
		METHOD_PROCESS_SHUTDOWN=18,
		METHOD_EXECUTE_SCRIPT=16,
		METHOD_BROWSE_SERVICE_DIR=5,
		METHOD_FILE_DOWNLOAD_START=6,
		METHOD_FILE_DOWNLOAD_DATA=7,
		METHOD_FILE_DOWNLOAD_FINISH=32,
		METHOD_FILE_UPLOAD_START=9,
		METHOD_FILE_UPLOAD_DATA=10,
		METHOD_FILE_UPLOAD_FINISH=33,
		METHOD_CREATE_DIR=12,
		METHOD_DELETE_FILE=13,
		METHOD_CHANGE_FILE_MODE=19,
		METHOD_ADD_SERVICE=20,
		METHOD_EDIT_SERVICE=21,
		METHOD_DELETE_SERVICE=22,
		METHOD_SEND_COMMAND=24,
		METHOD_ENABLE_LOG_RECV=25,
		METHOD_CONSOLE_LOG_NOTIFY=26,
		METHOD_GET_SERVER_STATUS=27,
		METHOD_GET_ALL_SERVER_STATUS=28,
		METHOD_GET_SERVER_STATUS_FORMAT=29,
		METHOD_FILE_COMPARE=31,
	
	};
	
	
	
	/*************************************************
	函数名:	Login
	用途:	
	参数:
		UserName				
		Password				
	返回值:无特别意义
	*************************************************/
	virtual int Login(const CEasyString& UserName ,const CEasyString& Password ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServiceList
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceList() {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetProcessList
	用途:	
	参数:
		Page				
		PageLen				
	返回值:无特别意义
	*************************************************/
	virtual int GetProcessList(short Page ,short PageLen ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetNetAdapterList
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int GetNetAdapterList() {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServiceInfo
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceInfo(UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ServiceStartup
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceStartup(UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ServiceShutdown
	用途:	
	参数:
		ServiceID				
		ShutdownType				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceShutdown(UINT ServiceID ,BYTE ShutdownType ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	RunProgram
	用途:	
	参数:
		ServiceID				
		FilePath				
		WorkDir				
		Param				
	返回值:无特别意义
	*************************************************/
	virtual int RunProgram(UINT ServiceID ,const CEasyString& FilePath ,const CEasyString& WorkDir ,const CEasyString& Param ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ProcessShutdown
	用途:	
	参数:
		ProcessID				
		ShutdownType				
	返回值:无特别意义
	*************************************************/
	virtual int ProcessShutdown(UINT ProcessID ,BYTE ShutdownType ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ExecuteScript
	用途:	
	参数:
		ServiceID				
		Script				
		FromFile				
	返回值:无特别意义
	*************************************************/
	virtual int ExecuteScript(UINT ServiceID ,const CEasyString& Script ,bool FromFile ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	BrowseServiceDir
	用途:	
	参数:
		ServiceID				
		Dir				
		Page				
		PageLen				
	返回值:无特别意义
	*************************************************/
	virtual int BrowseServiceDir(UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileDownloadStart
	用途:	
	参数:
		ServiceID				
		FilePath				
		StartOffset				
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadStart(UINT ServiceID ,const CEasyString& FilePath ,UINT64 StartOffset ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileDownloadData
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadData() {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileDownloadFinish
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadFinish() {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileUploadStart
	用途:	
	参数:
		ServiceID				
		FilePath				
		FileLastWriteTime				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadStart(UINT ServiceID ,const CEasyString& FilePath ,UINT FileLastWriteTime ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileUploadData
	用途:	
	参数:
		Length				
		FileData				
		IsLast				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadData(UINT Length ,const CEasyBuffer& FileData ,bool IsLast ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileUploadFinish
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadFinish() {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	CreateDir
	用途:	
	参数:
		ServiceID				
		Dir				
	返回值:无特别意义
	*************************************************/
	virtual int CreateDir(UINT ServiceID ,const CEasyString& Dir ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	DeleteFile
	用途:	
	参数:
		ServiceID				
		FilePath				
		IsRecursive				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteFile(UINT ServiceID ,const CEasyString& FilePath ,bool IsRecursive ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	ChangeFileMode
	用途:	
	参数:
		ServiceID				
		FilePath				
		Mode				
	返回值:无特别意义
	*************************************************/
	virtual int ChangeFileMode(UINT ServiceID ,const CEasyString& FilePath ,UINT Mode ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	AddService
	用途:	
	参数:
		ServiceInfo				
	返回值:无特别意义
	*************************************************/
	virtual int AddService(const CSmartStruct& ServiceInfo ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	EditService
	用途:	
	参数:
		ServiceInfo				
	返回值:无特别意义
	*************************************************/
	virtual int EditService(const CSmartStruct& ServiceInfo ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	DeleteService
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteService(UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	SendCommand
	用途:	
	参数:
		ServiceID				
		Command				
	返回值:无特别意义
	*************************************************/
	virtual int SendCommand(UINT ServiceID ,const CEasyString& Command ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	EnableLogRecv
	用途:	
	参数:
		ServiceID				
		Enable				
	返回值:无特别意义
	*************************************************/
	virtual int EnableLogRecv(UINT ServiceID ,bool Enable ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServerStatus
	用途:	
	参数:
		ServiceID				
		StatusListPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatus(UINT ServiceID ,const CSmartStruct& StatusListPacket ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetAllServerStatus
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int GetAllServerStatus(UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	GetServerStatusFormat
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatusFormat(UINT ServiceID ) {return COMMON_RESULT_FAILED;}
		
	
	
	/*************************************************
	函数名:	FileCompare
	用途:	
	参数:
		ServiceID				
		FilePath				
		FileSize				
		FileMD5				
	返回值:无特别意义
	*************************************************/
	virtual int FileCompare(UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize ,const CEasyString& FileMD5 ) {return COMMON_RESULT_FAILED;}
		
	
	
protected:	
	
	enum SERVER_MANAGER_LOGIN_MEMBER_IDS
	{
		SST_LOGIN_USER_NAME=1,
		SST_LOGIN_PASSWORD=2,
	
	};
	enum SERVER_MANAGER_GET_SERVICE_LIST_MEMBER_IDS
	{
		
	};
	enum SERVER_MANAGER_GET_PROCESS_LIST_MEMBER_IDS
	{
		SST_GET_PROCESS_LIST_PAGE=3,
		SST_GET_PROCESS_LIST_PAGE_LEN=4,
	
	};
	enum SERVER_MANAGER_GET_NET_ADAPTER_LIST_MEMBER_IDS
	{
		
	};
	enum SERVER_MANAGER_GET_SERVICE_INFO_MEMBER_IDS
	{
		SST_GET_SERVICE_INFO_SERVICE_ID=1,
	
	};
	enum SERVER_MANAGER_SERVICE_STARTUP_MEMBER_IDS
	{
		SST_SERVICE_STARTUP_SERVICE_ID=1,
	
	};
	enum SERVER_MANAGER_SERVICE_SHUTDOWN_MEMBER_IDS
	{
		SST_SERVICE_SHUTDOWN_SERVICE_ID=1,
		SST_SERVICE_SHUTDOWN_SHUTDOWN_TYPE=4,
	
	};
	enum SERVER_MANAGER_RUN_PROGRAM_MEMBER_IDS
	{
		SST_RUN_PROGRAM_SERVICE_ID=5,
		SST_RUN_PROGRAM_FILE_PATH=1,
		SST_RUN_PROGRAM_WORK_DIR=2,
		SST_RUN_PROGRAM_PARAM=3,
	
	};
	enum SERVER_MANAGER_PROCESS_SHUTDOWN_MEMBER_IDS
	{
		SST_PROCESS_SHUTDOWN_PROCESS_ID=1,
		SST_PROCESS_SHUTDOWN_SHUTDOWN_TYPE=4,
	
	};
	enum SERVER_MANAGER_EXECUTE_SCRIPT_MEMBER_IDS
	{
		SST_EXECUTE_SCRIPT_SERVICE_ID=2,
		SST_EXECUTE_SCRIPT_SCRIPT=1,
		SST_EXECUTE_SCRIPT_FROM_FILE=3,
	
	};
	enum SERVER_MANAGER_BROWSE_SERVICE_DIR_MEMBER_IDS
	{
		SST_BROWSE_SERVICE_DIR_SERVICE_ID=1,
		SST_BROWSE_SERVICE_DIR_DIR=2,
		SST_BROWSE_SERVICE_DIR_PAGE=6,
		SST_BROWSE_SERVICE_DIR_PAGE_LEN=7,
	
	};
	enum SERVER_MANAGER_FILE_DOWNLOAD_START_MEMBER_IDS
	{
		SST_FILE_DOWNLOAD_START_SERVICE_ID=1,
		SST_FILE_DOWNLOAD_START_FILE_PATH=2,
		SST_FILE_DOWNLOAD_START_START_OFFSET=8,
	
	};
	enum SERVER_MANAGER_FILE_DOWNLOAD_DATA_MEMBER_IDS
	{
		
	};
	enum SERVER_MANAGER_FILE_DOWNLOAD_FINISH_MEMBER_IDS
	{
		
	};
	enum SERVER_MANAGER_FILE_UPLOAD_START_MEMBER_IDS
	{
		SST_FILE_UPLOAD_START_SERVICE_ID=1,
		SST_FILE_UPLOAD_START_FILE_PATH=2,
		SST_FILE_UPLOAD_START_FILE_LAST_WRITE_TIME=8,
	
	};
	enum SERVER_MANAGER_FILE_UPLOAD_DATA_MEMBER_IDS
	{
		SST_FILE_UPLOAD_DATA_LENGTH=2,
		SST_FILE_UPLOAD_DATA_FILE_DATA=4,
		SST_FILE_UPLOAD_DATA_IS_LAST=8,
	
	};
	enum SERVER_MANAGER_FILE_UPLOAD_FINISH_MEMBER_IDS
	{
		
	};
	enum SERVER_MANAGER_CREATE_DIR_MEMBER_IDS
	{
		SST_CREATE_DIR_SERVICE_ID=1,
		SST_CREATE_DIR_DIR=2,
	
	};
	enum SERVER_MANAGER_DELETE_FILE_MEMBER_IDS
	{
		SST_DELETE_FILE_SERVICE_ID=1,
		SST_DELETE_FILE_FILE_PATH=2,
		SST_DELETE_FILE_IS_RECURSIVE=6,
	
	};
	enum SERVER_MANAGER_CHANGE_FILE_MODE_MEMBER_IDS
	{
		SST_CHANGE_FILE_MODE_SERVICE_ID=1,
		SST_CHANGE_FILE_MODE_FILE_PATH=2,
		SST_CHANGE_FILE_MODE_MODE=3,
	
	};
	enum SERVER_MANAGER_ADD_SERVICE_MEMBER_IDS
	{
		SST_ADD_SERVICE_SERVICE_INFO=1,
	
	};
	enum SERVER_MANAGER_EDIT_SERVICE_MEMBER_IDS
	{
		SST_EDIT_SERVICE_SERVICE_INFO=1,
	
	};
	enum SERVER_MANAGER_DELETE_SERVICE_MEMBER_IDS
	{
		SST_DELETE_SERVICE_SERVICE_ID=1,
	
	};
	enum SERVER_MANAGER_SEND_COMMAND_MEMBER_IDS
	{
		SST_SEND_COMMAND_SERVICE_ID=1,
		SST_SEND_COMMAND_COMMAND=2,
	
	};
	enum SERVER_MANAGER_ENABLE_LOG_RECV_MEMBER_IDS
	{
		SST_ENABLE_LOG_RECV_SERVICE_ID=4,
		SST_ENABLE_LOG_RECV_ENABLE=1,
	
	};
	enum SERVER_MANAGER_GET_SERVER_STATUS_MEMBER_IDS
	{
		SST_GET_SERVER_STATUS_SERVICE_ID=4,
		SST_GET_SERVER_STATUS_STATUS_LIST_PACKET=1,
	
	};
	enum SERVER_MANAGER_GET_ALL_SERVER_STATUS_MEMBER_IDS
	{
		SST_GET_ALL_SERVER_STATUS_SERVICE_ID=3,
	
	};
	enum SERVER_MANAGER_GET_SERVER_STATUS_FORMAT_MEMBER_IDS
	{
		SST_GET_SERVER_STATUS_FORMAT_SERVICE_ID=3,
	
	};
	enum SERVER_MANAGER_FILE_COMPARE_MEMBER_IDS
	{
		SST_FILE_COMPARE_SERVICE_ID=1,
		SST_FILE_COMPARE_FILE_PATH=2,
		SST_FILE_COMPARE_FILE_SIZE=3,
		SST_FILE_COMPARE_FILE_MD5=4,
	
	};
};
