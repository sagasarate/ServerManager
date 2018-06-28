#pragma once


class CServerManagerAckMsgCaller :
	public IServerManagerAck
{
protected:
	INetProcessor *	m_pNet;
public:
	CServerManagerAckMsgCaller(INetProcessor * pNet);	
	~CServerManagerAckMsgCaller(void);
	
	
	
	/*************************************************
	函数名:	LoginAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int LoginAck(short Result );
		
	
	
	/*************************************************
	函数名:	GetServiceListAck
	用途:	
	参数:
		Result				
		ServiceListData				
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceListAck(short Result ,const CSmartStruct& ServiceListData );
		
	
	
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
	virtual int GetProcessListAck(short Result ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& ProcessListData );
		
	
	
	/*************************************************
	函数名:	GetNetAdapterListAck
	用途:	
	参数:
		Result				
		NetAdapterListData				
	返回值:无特别意义
	*************************************************/
	virtual int GetNetAdapterListAck(short Result ,const CSmartStruct& NetAdapterListData );
		
	
	
	/*************************************************
	函数名:	GetServiceInfoAck
	用途:	
	参数:
		Result				
		ServiceInfoPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServiceInfoAck(short Result ,const CSmartStruct& ServiceInfoPacket );
		
	
	
	/*************************************************
	函数名:	ServiceStartupAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceStartupAck(short Result ,UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	ServiceShutdownAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int ServiceShutdownAck(short Result ,UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	RunProgramAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int RunProgramAck(short Result );
		
	
	
	/*************************************************
	函数名:	ProcessShutdownAck
	用途:	
	参数:
		Result				
		ProcessID				
	返回值:无特别意义
	*************************************************/
	virtual int ProcessShutdownAck(short Result ,UINT ProcessID );
		
	
	
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
	virtual int ExecuteScriptAck(short Result ,UINT ServiceID ,int ErrorCode ,int LastLine );
		
	
	
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
	virtual int BrowseServiceDirAck(short Result ,UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& FileListData );
		
	
	
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
	virtual int FileDownloadStartAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize );
		
	
	
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
	virtual int FileDownloadDataAck(short Result ,UINT64 Offset ,UINT Length ,const CEasyBuffer& FileData );
		
	
	
	/*************************************************
	函数名:	FileDownloadEndAck
	用途:	
	参数:
		Result				
		FileLastWriteTime				
	返回值:无特别意义
	*************************************************/
	virtual int FileDownloadEndAck(short Result ,UINT FileLastWriteTime );
		
	
	
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
	virtual int FileUploadStartAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize );
		
	
	
	/*************************************************
	函数名:	FileUploadDataAck
	用途:	
	参数:
		Result				
		Offset				
		Length				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadDataAck(short Result ,UINT64 Offset ,UINT Length );
		
	
	
	/*************************************************
	函数名:	FileUploadEndAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int FileUploadEndAck(short Result );
		
	
	
	/*************************************************
	函数名:	CreateDirAck
	用途:	
	参数:
		Result				
		ServiceID				
		Dir				
	返回值:无特别意义
	*************************************************/
	virtual int CreateDirAck(short Result ,UINT ServiceID ,const CEasyString& Dir );
		
	
	
	/*************************************************
	函数名:	DeleteFileAck
	用途:	
	参数:
		Result				
		ServiceID				
		FilePath				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteFileAck(short Result ,UINT ServiceID ,const CEasyString& FilePath );
		
	
	
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
	virtual int ChangeFileModeAck(short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT Mode );
		
	
	
	/*************************************************
	函数名:	AddServiceAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int AddServiceAck(short Result );
		
	
	
	/*************************************************
	函数名:	EditServiceAck
	用途:	
	参数:
		Result				
	返回值:无特别意义
	*************************************************/
	virtual int EditServiceAck(short Result );
		
	
	
	/*************************************************
	函数名:	DeleteServiceAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int DeleteServiceAck(short Result ,UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	SendCommandAck
	用途:	
	参数:
		Result				
		ServiceID				
	返回值:无特别意义
	*************************************************/
	virtual int SendCommandAck(short Result ,UINT ServiceID );
		
	
	
	/*************************************************
	函数名:	EnableLogRecvAck
	用途:	
	参数:
		Result				
		ServiceID				
		Enable				
	返回值:无特别意义
	*************************************************/
	virtual int EnableLogRecvAck(short Result ,UINT ServiceID ,bool Enable );
		
	
	
	/*************************************************
	函数名:	ConsoleLogNotify
	用途:	
	参数:
		ServiceID				
		LogMsg				
	返回值:无特别意义
	*************************************************/
	virtual int ConsoleLogNotify(UINT ServiceID ,LPCTSTR LogMsg );
		
	
	
	/*************************************************
	函数名:	GetServerStatusAck
	用途:	
	参数:
		Result				
		ServiceID				
		StatusListPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatusAck(short Result ,UINT ServiceID ,const CSmartStruct& StatusListPacket );
		
	
	
	/*************************************************
	函数名:	GetServerStatusFormatAck
	用途:	
	参数:
		Result				
		ServiceID				
		StatusFormatPacket				
	返回值:无特别意义
	*************************************************/
	virtual int GetServerStatusFormatAck(short Result ,UINT ServiceID ,const CSmartStruct& StatusFormatPacket );
		
	
	static bool PackMsgLoginAck(CSmartStruct& Packet,short Result );
	static bool PackMsgGetServiceListAck(CSmartStruct& Packet,short Result ,const CSmartStruct& ServiceListData );
	static bool PackMsgGetProcessListAck(CSmartStruct& Packet,short Result ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& ProcessListData );
	static bool PackMsgGetNetAdapterListAck(CSmartStruct& Packet,short Result ,const CSmartStruct& NetAdapterListData );
	static bool PackMsgGetServiceInfoAck(CSmartStruct& Packet,short Result ,const CSmartStruct& ServiceInfoPacket );
	static bool PackMsgServiceStartupAck(CSmartStruct& Packet,short Result ,UINT ServiceID );
	static bool PackMsgServiceShutdownAck(CSmartStruct& Packet,short Result ,UINT ServiceID );
	static bool PackMsgRunProgramAck(CSmartStruct& Packet,short Result );
	static bool PackMsgProcessShutdownAck(CSmartStruct& Packet,short Result ,UINT ProcessID );
	static bool PackMsgExecuteScriptAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,int ErrorCode ,int LastLine );
	static bool PackMsgBrowseServiceDirAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& Dir ,short Page ,short PageLen ,short TotalPage ,const CSmartStruct& FileListData );
	static bool PackMsgFileDownloadStartAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize );
	static bool PackMsgFileDownloadDataAck(CSmartStruct& Packet,short Result ,UINT64 Offset ,UINT Length ,const CEasyBuffer& FileData );
	static bool PackMsgFileDownloadEndAck(CSmartStruct& Packet,short Result ,UINT FileLastWriteTime );
	static bool PackMsgFileUploadStartAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT64 FileSize );
	static bool PackMsgFileUploadDataAck(CSmartStruct& Packet,short Result ,UINT64 Offset ,UINT Length );
	static bool PackMsgFileUploadEndAck(CSmartStruct& Packet,short Result );
	static bool PackMsgCreateDirAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& Dir );
	static bool PackMsgDeleteFileAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath );
	static bool PackMsgChangeFileModeAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CEasyString& FilePath ,UINT Mode );
	static bool PackMsgAddServiceAck(CSmartStruct& Packet,short Result );
	static bool PackMsgEditServiceAck(CSmartStruct& Packet,short Result );
	static bool PackMsgDeleteServiceAck(CSmartStruct& Packet,short Result ,UINT ServiceID );
	static bool PackMsgSendCommandAck(CSmartStruct& Packet,short Result ,UINT ServiceID );
	static bool PackMsgEnableLogRecvAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,bool Enable );
	static bool PackMsgConsoleLogNotify(CSmartStruct& Packet,UINT ServiceID ,LPCTSTR LogMsg );
	static bool PackMsgGetServerStatusAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CSmartStruct& StatusListPacket );
	static bool PackMsgGetServerStatusFormatAck(CSmartStruct& Packet,short Result ,UINT ServiceID ,const CSmartStruct& StatusFormatPacket );
	
};
