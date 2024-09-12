#pragma once


class CServerManagerMsgCaller :
	public IServerManager
{
protected:
	INetProcessor *	m_pNet;
public:
	CServerManagerMsgCaller(INetProcessor * pNet);	
	~CServerManagerMsgCaller(void);
	
	
	
	/*************************************************
	函数名:	Login
	用途:	
	参数:
		UserName				
		Password				
	返回值:无特别意义
	*************************************************/
	virtual int Login(const CEasyString& UserName ,const CEasyString& Password );
		
	
	
	/*************************************************
	函数名:	GetServiceList
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceList();
		
	
	
	/*************************************************
	函数名:	GetProcessList
	用途:	
	参数:
		Page				
		PageLen				
	返回值:无特别意义
	*************************************************/
	virtual int GetProcessList(short Page ,short PageLen );
		
	
	
	/*************************************************
	函数名:	GetNetAdapterList
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int GetNetAdapterList();
		
	
	
	/*************************************************
	函数名:	GetServiceInfo
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceInfo(UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	ServiceStartup
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceStartup(UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	ServiceShutdown
	用途:	
	参数:
		ServiceID				
		ShutdownType				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceShutdown(UINT ServiceID ,BYTE ShutdownType );
		
	
	
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
	virtual int RunProgram(UINT ServiceID ,const CEasyString& FilePath ,const CEasyString& WorkDir ,const CEasyString& Param );
		
	
	
	/*************************************************
	函数名:	ProcessShutdown
	用途:	
	参数:
		ProcessID				
		ShutdownType				
	返回值:无特别意义
	*************************************************/
	virtual int ProcessShutdown(UINT ProcessID ,BYTE ShutdownType );
		
	
	
	/*************************************************
	函数名:	ExecuteScript
	用途:	
	参数:
		ServiceID				
		Script				
		FromFile				
	返回值:无特别意义
	*************************************************/
	virtual int ExecuteScript(UINT ServiceID ,const CEasyString& Script ,bool FromFile );
		
	
	
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
	virtual int BrowseServiceDir(UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen );
		
	
	
	/*************************************************
	函数名:	FileDownloadStart
	用途:	
	参数:
		ServiceID				
		FilePath				
		StartOffset				
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadStart(UINT ServiceID ,const CEasyString& FilePath ,UINT64 StartOffset );
		
	
	
	/*************************************************
	函数名:	FileDownloadData
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadData();
		
	
	
	/*************************************************
	函数名:	FileDownloadFinish
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadFinish();
		
	
	
	/*************************************************
	函数名:	FileUploadStart
	用途:	
	参数:
		ServiceID				
		FilePath				
		FileLastWriteTime				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadStart(UINT ServiceID ,const CEasyString& FilePath ,UINT FileLastWriteTime );
		
	
	
	/*************************************************
	函数名:	FileUploadData
	用途:	
	参数:
		Length				
		FileData				
		IsLast				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadData(UINT Length ,const CEasyBuffer& FileData ,bool IsLast );
		
	
	
	/*************************************************
	函数名:	FileUploadFinish
	用途:	
	参数:
		
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadFinish();
		
	
	
	/*************************************************
	函数名:	CreateDir
	用途:	
	参数:
		ServiceID				
		Dir				
	返回值:无特别意义
	*************************************************/
	virtual int CreateDir(UINT ServiceID ,const CEasyString& Dir );
		
	
	
	/*************************************************
	函数名:	DeleteFile
	用途:	
	参数:
		ServiceID				
		FilePath				
		IsRecursive				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteFile(UINT ServiceID ,const CEasyString& FilePath ,bool IsRecursive );
		
	
	
	/*************************************************
	函数名:	ChangeFileMode
	用途:	
	参数:
		ServiceID				
		FilePath				
		Mode				
	返回值:无特别意义
	*************************************************/
	virtual int ChangeFileMode(UINT ServiceID ,const CEasyString& FilePath ,UINT Mode );
		
	
	
	/*************************************************
	函数名:	AddService
	用途:	
	参数:
		ServiceInfo				
	返回值:无特别意义
	*************************************************/
	virtual int AddService(const CSmartStruct& ServiceInfo );
		
	
	
	/*************************************************
	函数名:	EditService
	用途:	
	参数:
		ServiceInfo				
	返回值:无特别意义
	*************************************************/
	virtual int EditService(const CSmartStruct& ServiceInfo );
		
	
	
	/*************************************************
	函数名:	DeleteService
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteService(UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	SendCommand
	用途:	
	参数:
		ServiceID				
		Command				
	返回值:无特别意义
	*************************************************/
	virtual int SendCommand(UINT ServiceID ,const CEasyString& Command );
		
	
	
	/*************************************************
	函数名:	EnableLogRecv
	用途:	
	参数:
		ServiceID				
		Enable				
	返回值:无特别意义
	*************************************************/
	virtual int EnableLogRecv(UINT ServiceID ,bool Enable );
		
	
	
	/*************************************************
	函数名:	GetServerStatus
	用途:	
	参数:
		ServiceID				
		StatusListPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatus(UINT ServiceID ,const CSmartStruct& StatusListPacket );
		
	
	
	/*************************************************
	函数名:	GetAllServerStatus
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int GetAllServerStatus(UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	GetServerStatusFormat
	用途:	
	参数:
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatusFormat(UINT ServiceID );
		
	
	
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
	virtual int FileCompare(UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize ,const CEasyString& FileMD5 );
		
	
	static bool PackMsgLogin(CSmartStruct& Packet,const CEasyString& UserName ,const CEasyString& Password );
	static bool PackMsgGetServiceList(CSmartStruct& Packet);
	static bool PackMsgGetProcessList(CSmartStruct& Packet,short Page ,short PageLen );
	static bool PackMsgGetNetAdapterList(CSmartStruct& Packet);
	static bool PackMsgGetServiceInfo(CSmartStruct& Packet,UINT ServiceID );
	static bool PackMsgServiceStartup(CSmartStruct& Packet,UINT ServiceID );
	static bool PackMsgServiceShutdown(CSmartStruct& Packet,UINT ServiceID ,BYTE ShutdownType );
	static bool PackMsgRunProgram(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,const CEasyString& WorkDir ,const CEasyString& Param );
	static bool PackMsgProcessShutdown(CSmartStruct& Packet,UINT ProcessID ,BYTE ShutdownType );
	static bool PackMsgExecuteScript(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Script ,bool FromFile );
	static bool PackMsgBrowseServiceDir(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen );
	static bool PackMsgFileDownloadStart(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT64 StartOffset );
	static bool PackMsgFileDownloadData(CSmartStruct& Packet);
	static bool PackMsgFileDownloadFinish(CSmartStruct& Packet);
	static bool PackMsgFileUploadStart(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT FileLastWriteTime );
	static bool PackMsgFileUploadData(CSmartStruct& Packet,UINT Length ,const CEasyBuffer& FileData ,bool IsLast );
	static bool PackMsgFileUploadFinish(CSmartStruct& Packet);
	static bool PackMsgCreateDir(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Dir );
	static bool PackMsgDeleteFile(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,bool IsRecursive );
	static bool PackMsgChangeFileMode(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT Mode );
	static bool PackMsgAddService(CSmartStruct& Packet,const CSmartStruct& ServiceInfo );
	static bool PackMsgEditService(CSmartStruct& Packet,const CSmartStruct& ServiceInfo );
	static bool PackMsgDeleteService(CSmartStruct& Packet,UINT ServiceID );
	static bool PackMsgSendCommand(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& Command );
	static bool PackMsgEnableLogRecv(CSmartStruct& Packet,UINT ServiceID ,bool Enable );
	static bool PackMsgGetServerStatus(CSmartStruct& Packet,UINT ServiceID ,const CSmartStruct& StatusListPacket );
	static bool PackMsgGetAllServerStatus(CSmartStruct& Packet,UINT ServiceID );
	static bool PackMsgGetServerStatusFormat(CSmartStruct& Packet,UINT ServiceID );
	static bool PackMsgFileCompare(CSmartStruct& Packet,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize ,const CEasyString& FileMD5 );
	
};
