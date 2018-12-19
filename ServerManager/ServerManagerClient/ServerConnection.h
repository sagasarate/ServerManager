/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerConnection.h                                       */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CServerManagerClientView;

class CServerConnection :
	public CNetConnection,
	public INetProcessor,
	public CServerManagerAckMsgHandler
{
protected:
	CServerManagerClientView *				m_pView;
	CEasyBuffer								m_AssembleBuffer;
	CEasyBuffer								m_SendBuffer;
	CEasyString								m_ServerAddress;
	UINT									m_ServerPort;
	CEasyString								m_UserName;
	CEasyString								m_Password;
	CEasyTimer								m_ConnectTimer;
	CEasyTimer								m_ServiceInfoFetchTimer;

	CEasyTimer								m_KeepAliveTimer;
	UINT									m_KeepAliveCount;

	CServiceInfoList						m_ServiceList;

	CEasyMap<MSG_ID_TYPE, MSG_HANDLE_INFO>	m_MsgFnMap;

	bool									m_IsLogined;

	CTaskQueue								m_TaskQueue;

	CSmartStruct							m_PacketBuffer;

	//IFileAccessor *							m_pDataLogFile;
	
public:
	CServerConnection();
	~CServerConnection(void);

	bool Init(CServerManagerClientView * pView, CNetServer * pServer, LPCTSTR szServerAddress, UINT ServerPort, LPCTSTR UserName, LPCTSTR Password);
	bool Reconnection(LPCTSTR szServerAddress, UINT ServerPort, LPCTSTR UserName, LPCTSTR Password);

	virtual void OnConnection(bool IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const BYTE * pData, UINT DataSize);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	const CEasyString& GetServerAddress();
	UINT GetServerPort();
	LPCTSTR GetUserName();
	LPCTSTR GetPassword();

	const CEasyArray<CServiceInfo>& GetServiceList();
	const CServiceInfo * GetServiceInfo(UINT ServiceID);
	CTaskQueue& GetTaskQueue();

	void PrintLog(short Result, LPCTSTR szFormat, ...);
	

	void OnMsg(CMessage * pMsg);
	void OnSystemMsg(CMessage * pMsg);

	virtual CMessage * NewMessage(UINT MsgDataSize);
	virtual bool ReleaseMessage(CMessage * pMsg);
	virtual bool SendMessage(CMessage * pMsg);
	bool DoSend(LPCVOID pData, UINT Size);

	void QueryServiceList();
	void QueryStartupService(UINT ServiceID);
	void QueryShutDownService(UINT ServiceID, BYTE ShutDownType);
	void QueryBrowseWorkDir(UINT ServiceID, LPCTSTR Dir);

	void QueryStartDownload(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath);
	void QueryDownloadData(UINT64 Offset, UINT Length);
	void QueryEndDownload();

	void QueryStartUpload(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath);
	void QueryUploadData(UINT64 Offset, UINT Length, const CEasyBuffer& FileData);
	void QueryEndUpload(UINT FileLastWriteTime);

	void QueryCreateDir(UINT ServiceID, LPCTSTR Dir);
	void QueryDeleteFile(UINT ServiceID, LPCTSTR FilePath);
	void QueryChangeFileMode(UINT ServiceID, LPCTSTR FilePath, UINT Mode);

	void QueryCreateProcess(UINT ServiceID, LPCTSTR FilePath, LPCTSTR WorkDir, LPCTSTR Param);
	void QueryNetAdapterInfo();
	void QueryRunScript(UINT ServiceID, LPCTSTR FilePath);

	void QueryAddService(const CServiceInfo& ServiceInfo);
	void QueryEditService(const CServiceInfo& ServiceInfo);
	void QueryDelService(UINT ServiceID);

	void QueryProcessList(short Page, short PageLen);

	void QuerySendCommand(UINT ServiceID, LPCTSTR szCommand);
	void QueryAllServerStatus(UINT ServiceID);
	void QueryServerStatusFormat(UINT ServiceID);
	void QueryEnableLogRecv(UINT ServiceID, bool Enable);

	void QueryServiceInfo(UINT ServiceID);
	void QueryFileCompare(UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize, LPCTSTR FileMD5);
protected:
	virtual int LoginAck(short Result) override;
	virtual int GetServiceListAck(short Result, const CSmartStruct& ServiceListData) override;
	virtual int GetProcessListAck(short Result, short Page, short PageLen, short TotalPage, const CSmartStruct& ProcessListData) override;
	virtual int GetNetAdapterListAck(short Result, const CSmartStruct& NetAdapterListData) override;
	virtual int GetServiceInfoAck(short Result, const CSmartStruct& ServiceInfoPacket) override;
	virtual int ServiceStartupAck(short Result, UINT ServiceID) override;
	virtual int ServiceShutdownAck(short Result, UINT ServiceID) override;
	virtual int RunProgramAck(short Result) override;
	virtual int ProcessShutdownAck(short Result, UINT ProcessID) override;
	virtual int ExecuteScriptAck(short Result, UINT ServiceID, int ErrorCode, int LastLine) override;
	virtual int BrowseServiceDirAck(short Result, UINT ServiceID, const CEasyString& Dir, short Page, short PageLen, short TotalPage, const CSmartStruct& FileListData) override;
	virtual int FileDownloadStartAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT64 FileSize) override;
	virtual int FileDownloadDataAck(short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData) override;
	virtual int FileDownloadEndAck(short Result, UINT FileLastWriteTime) override;
	virtual int FileUploadStartAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT64 FileSize) override;
	virtual int FileUploadDataAck(short Result, UINT64 Offset, UINT Length) override;
	virtual int FileUploadEndAck(short Result) override;
	virtual int CreateDirAck(short Result, UINT ServiceID, const CEasyString& Dir) override;
	virtual int DeleteFileAck(short Result, UINT ServiceID, const CEasyString& FilePath) override;
	virtual int ChangeFileModeAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT Mode) override;
	virtual int AddServiceAck(short Result) override;
	virtual int EditServiceAck(short Result) override;
	virtual int DeleteServiceAck(short Result, UINT ServiceID) override;
	virtual int SendCommandAck(short Result, UINT ServiceID) override;
	virtual int EnableLogRecvAck(short Result, UINT ServiceID, bool Enable) override;
	virtual int ConsoleLogNotify(UINT ServiceID, LPCTSTR LogMsg) override;
	virtual int GetServerStatusAck(short Result, UINT ServiceID, const CSmartStruct& StatusListPacket) override;
	virtual int GetServerStatusFormatAck(short Result, UINT ServiceID, const CSmartStruct& StatusFormatPacket) override;
	virtual int FileCompareAck(short Result, UINT ServiceID, LPCTSTR FilePath) override;

};

inline const CEasyString& CServerConnection::GetServerAddress()
{
	return m_ServerAddress;
}
inline UINT CServerConnection::GetServerPort()
{
	return m_ServerPort;
}
inline LPCTSTR CServerConnection::GetUserName()
{
	return m_UserName;
}
inline LPCTSTR CServerConnection::GetPassword()
{
	return m_Password;
}
inline const CEasyArray<CServiceInfo>& CServerConnection::GetServiceList()
{
	return m_ServiceList.GetList();
}

inline const CServiceInfo * CServerConnection::GetServiceInfo(UINT ServiceID)
{
	for (UINT i = 0; i < m_ServiceList.GetList().GetCount(); i++)
	{
		if (m_ServiceList.GetList()[i].GetServiceID() == ServiceID)
		{
			return m_ServiceList.GetList().GetObject(i);
		}
	}
	return NULL;
}
inline CTaskQueue& CServerConnection::GetTaskQueue()
{
	return m_TaskQueue;
}