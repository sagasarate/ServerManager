/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerClient.h                                    */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CServerManagerService;



class CServerManagerClient :
	public CNetConnection,
	public INetProcessor,
	public CServerManagerMsgHandler
{
protected:
	CServerManagerService *					m_pManager;
	CEasyBuffer								m_AssembleBuffer;
	CEasyBuffer								m_SendBuffer;
	bool									m_WantDelete;

	CEasyTimer								m_KeepAliveTimer;
	UINT									m_KeepAliveCount;

	CEasyMap<MSG_ID_TYPE, MSG_HANDLE_INFO>	m_MsgFnMap;

	CSmartStruct							m_PacketBuffer1;

	CEasyString								m_RecentBrosweDir;
	CEasyArray<FILE_INFO>					m_DirBrosweCache;
	CEasyTimer								m_DirBrosweCacheTimeout;

	CEasyBuffer								m_FileTransferBuffer;
	CEasyBuffer								m_CompressBuffer;	
	IFileAccessor *							m_pCurDownloadFile;
	IFileAccessor *							m_pCurUploadFile;
	//IFileAccessor *							m_pDataLogFile;

	//CEasyString				m_TransferFilePath;
	//CEasyBuffer				m_TransferBuffer;
	//CEasyBuffer				m_PackProp;
	//UINT					m_OriginSize;
	//UINT					m_TransferLeftSize;
	//CEasyTime				m_CurTransferFileLastWriteTime;

	CScriptExecutor			m_ScriptExecutor;
	bool					m_IsInScriptExecute;

	

	DECLARE_CLASS_INFO_STATIC(CServerManagerClient);
public:
	CServerManagerClient(void);
	~CServerManagerClient(void);

	BOOL Init(UINT ID,CServerManagerService * pManager);
	virtual void Destory();

	virtual void OnConnection(bool IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const BYTE * pData, UINT DataSize);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	void OnMsg(CMessage * pMsg);
	void OnSystemMsg(CMessage * pMsg);
	bool WantDelete();

	//virtual void OnGetFile(FILE_CACHE * pFileCache);
	void OnScriptExcute(int ErrorCode, int LastLine, UINT Param);

	bool DoSend(LPCVOID pData, UINT Size);
	virtual CMessage * NewMessage(UINT MsgDataSize);
	virtual bool ReleaseMessage(CMessage * pMsg);
	virtual bool SendMessage(CMessage * pMsg);
protected:
	virtual int GetServiceList() override;
	virtual int GetProcessList(short Page, short PageLen) override;
	virtual int GetNetAdapterList() override;
	virtual int ServiceStartup(UINT ServiceID) override;
	virtual int ServiceShutdown(UINT ServiceID, bool IsForce) override;
	virtual int RunProgram(UINT ServiceID, const CEasyString& FilePath, const CEasyString& WorkDir, const CEasyString& Param) override;
	virtual int ProcessShutdown(UINT ProcessID, bool IsForce) override;
	virtual int ExecuteScript(UINT ServiceID, const CEasyString& Script, bool FromFile) override;
	virtual int BrowseServiceDir(UINT ServiceID, const CEasyString& Dir, short Page, short PageLen) override;
	virtual int FileDownloadStart(UINT ServiceID, const CEasyString& FilePath) override;
	virtual int FileDownloadData(UINT64 Offset, UINT Length) override;
	virtual int FileDownloadEnd() override;
	virtual int FileUploadStart(UINT ServiceID, const CEasyString& FilePath) override;
	virtual int FileUploadData(UINT64 Offset, UINT Length, const CEasyBuffer& FileData) override;
	virtual int FileUploadEnd(UINT FileLastWriteTime) override;
	virtual int CreateDir(UINT ServiceID, const CEasyString& Dir) override;
	virtual int DeleteFile(UINT ServiceID, const CEasyString& FilePath, bool IsRecursive) override;
	virtual int ChangeFileMode(UINT ServiceID, const CEasyString& FilePath, UINT Mode) override;
	virtual int AddService(const SERVICE_INFO& ServiceInfo) override;
	virtual int EditService(const SERVICE_INFO& ServiceInfo) override;
	virtual int DeleteService(UINT ServiceID) override;


protected:

	//UINT PackServiceInfo(CEasyArray<SERVICE_INFO>& ServiceList,UINT StartIndex,CSmartStruct& Packet,WORD MemberID);
	//void PackNetAdapterInfo(NET_ADAPTER_INFO& NetAdapterInfo,CSmartStruct& Packet,WORD MemberI);
	//bool SendFileList(UINT ServiceIndex,CEasyString& WorkDir,CEasyString& SearchDir);
	
};

inline bool CServerManagerClient::WantDelete()
{
	return m_WantDelete;
}
