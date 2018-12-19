/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerService.h                                   */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CServerManagerService :
	public CNetService
{
protected:
	CIDStorage<CServerManagerClient>			m_ClientPool;

	CProcessSnapshot							m_ProcessSnapshot;
	CProcessInfoList							m_ProcessInfoList;	
	CEasyTimer									m_ProcessInfoFetchTimer;

	CEasyArray<CServiceInfoEx>					m_ServiceInfoList;
	//UINT64										m_RecentTime;
	//UINT64										m_RecentTimeSpan;
	
	CNetAdapterInfos							m_NetAdapterInfos;
	NET_ADAPTER_INFO_LIST						m_NetAdapterInfoList;

	CEasyBuffer									m_WorkBuffer;


	CIDStorage<HTTP_REQUEST_INFO>				m_RequestPool;
	CURLM *										m_CURLMHandle;
	int											m_HaveRequestRunning;

	CEasyString									m_ServerIP;


	DECLARE_CLASS_INFO_STATIC(CServerManagerService);
public:
	CServerManagerService(void);
	~CServerManagerService(void);

	BOOL Init(CNetServer * pServer);
	virtual void Destory();

	virtual CBaseNetConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual bool DeleteConnection(CBaseNetConnection * pConnection);

	CServerManagerClient * GetConnection(UINT ConnectionID);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	UINT GetClientCount();

	CEasyArray<CServiceInfoEx>& GetServiceInfoList();
	CProcessInfoList& GetProcessInfoList();
	NET_ADAPTER_INFO_LIST& GetNetAdapterInfo();

	CServiceInfoEx * GetServiceInfo(UINT ServiceID);
	LPCTSTR GetServiceWorkDir(UINT ServiceID);

	int StartupService(UINT ServiceID);
	int ShutdownService(UINT ServiceID, int ShutdownType);

	int StartupProcess(LPCTSTR szImageFileName, LPCTSTR szWorkDir, LPCTSTR szStartupParam);
	int ShutdownProcess(UINT ProcessID, int ShutdownType);

	int CheckServiceInfo(const CServiceInfo& ServiceInfo);
	int AddService(const CServiceInfo& ServiceInfo);
	int EditService(const CServiceInfo& ServiceInfo);
	int DeleteService(UINT ServiceID);

	//int SendCommand(UINT ServiceID, LPCTSTR szCommand);
	int EnableLogRecv(UINT ServiceID, bool Enable);

	void OnGetServiceWorkStatus(UINT ServiceID, BYTE WorkStatus);
	void OnServerLogMsg(UINT ServiceID, LPCTSTR szLogMsg);
	void OnSendCommandResult(UINT ServiceID, short Result);
	void SendNotify(CServiceInfoEx * pServiceInfo, LPCTSTR szMsg);
protected:
	BOOL DoListen(CIPAddress& ListenAddress);
	void FetchProcessInfo();
#ifdef WIN32
	void FetchWinServiceInfo();
#endif
	void InitNetAdapterInfo();
	void UpdateNetAdapterInfo();
	bool StartupWinService(LPCTSTR szServiceName);
	bool ShutdownWinService(LPCTSTR szServiceName);
#ifdef WIN32
	bool PostQuitMsgToProcess(UINT ProcessID);
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
#else
	static int ProcessFN(void * pParam);
#endif

	bool AddRequest(LPCSTR szURL, int RequestType, UINT Param1, UINT Param2, LPCTSTR Param3, LPCTSTR PostContent);
	bool SendRequest(HTTP_REQUEST_INFO * pRequestInfo);
	void ProcessRequestEnd(HTTP_REQUEST_INFO * pRequestInfo, CURLcode Result, long ResponseCode);
	static size_t ReadCallback(void *ptr, size_t size, size_t nmemb, void *userp);
	int RunCURL();
	int FinishCURL();

	void OnRequestResult(int RequestType, long ResponseCode, UINT Param1, UINT Param2, LPCTSTR Param3, LPCTSTR Content);
};

inline CServerManagerClient * CServerManagerService::GetConnection(UINT ConnectionID)
{
	return m_ClientPool.GetObject(ConnectionID);
}

inline UINT CServerManagerService::GetClientCount()
{
	return m_ClientPool.GetObjectCount();
}

inline CEasyArray<CServiceInfoEx>& CServerManagerService::GetServiceInfoList()
{
	return m_ServiceInfoList;
}

inline CProcessInfoList& CServerManagerService::GetProcessInfoList()
{
	return m_ProcessInfoList;
}

inline NET_ADAPTER_INFO_LIST& CServerManagerService::GetNetAdapterInfo()
{
	return m_NetAdapterInfoList;
}