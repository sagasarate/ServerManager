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
	PROCESS_INFO_LIST							m_ProcessInfoList;	
	CEasyTimer									m_ProcessInfoFetchTimer;

	SERVICE_INFO_LIST							m_ServiceInfoList;
	//UINT64										m_RecentTime;
	//UINT64										m_RecentTimeSpan;
	
	CNetAdapterInfos							m_NetAdapterInfos;
	NET_ADAPTER_INFO_LIST						m_NetAdapterInfoList;

	CEasyBuffer									m_WorkBuffer;

	CEasyTimer									m_ServiceKeepTimer;

	DECLARE_CLASS_INFO_STATIC(CServerManagerService);
public:
	CServerManagerService(void);
	~CServerManagerService(void);

	BOOL Init(CNetServer * pServer);
	virtual void Destory();

	virtual CBaseNetConnection * CreateConnection(CIPAddress& RemoteAddress);
	virtual bool DeleteConnection(CBaseNetConnection * pConnection);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	UINT GetClientCount();

	SERVICE_INFO_LIST& GetServiceInfoList();
	PROCESS_INFO_LIST& GetProcessInfoList();
	NET_ADAPTER_INFO_LIST& GetNetAdapterInfo();

	SERVICE_INFO * GetServiceInfo(UINT ServiceID);

	int StartupService(UINT ServiceID);
	int ShutdownService(UINT ServiceID, bool IsForce);

	int StartupProcess(LPCTSTR szImageFileName, LPCTSTR szWorkDir, LPCTSTR szStartupParam);
	int ShutdownProcess(UINT ProcessID, bool IsForce);

	int CheckServiceInfo(const SERVICE_INFO& ServiceInfo);
	int AddService(const SERVICE_INFO& ServiceInfo);
	int EditService(const SERVICE_INFO& ServiceInfo);
	int DeleteService(UINT ServiceID);

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

};


inline UINT CServerManagerService::GetClientCount()
{
	return m_ClientPool.GetObjectCount();
}

inline SERVICE_INFO_LIST& CServerManagerService::GetServiceInfoList()
{
	return m_ServiceInfoList;
}

inline PROCESS_INFO_LIST& CServerManagerService::GetProcessInfoList()
{
	return m_ProcessInfoList;
}

inline NET_ADAPTER_INFO_LIST& CServerManagerService::GetNetAdapterInfo()
{
	return m_NetAdapterInfoList;
}