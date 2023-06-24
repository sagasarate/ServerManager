/****************************************************************************/
/*                                                                          */
/*      文件名:    MainConfig.h                                             */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once



class CMainConfig :
	public CStaticObject<CMainConfig>
{
public:
	struct POOL_CONFIGS
	{
		STORAGE_POOL_SETTING				FileTaskPoolConfig;
		STORAGE_POOL_SETTING				FileDataBlockPoolConfig;
		STORAGE_POOL_SETTING				SerialOperationPoolConfig;
		STORAGE_POOL_SETTING				ParallelOperationPoolConfig;
		STORAGE_POOL_SETTING				FinishOperationPoolConfig;
	};
	struct TASK_MANAGER_CONFIG
	{
		UINT	SerialWorkThreadCount;
		UINT	ParallelWorkThreadCount;
		UINT	MaxUploadAcceptCount;
		UINT	MaxDownloadAcceptCount;
		UINT	TaskTimeOut;
		TASK_MANAGER_CONFIG()
		{
			SerialWorkThreadCount = 1;
			ParallelWorkThreadCount = 2;
			MaxUploadAcceptCount = 4;
			MaxDownloadAcceptCount = 4;
			TaskTimeOut = 3600;
		}
	};
protected:
	struct NOTIFY_SEND_CONFIG
	{
		CEasyString		GetTokenURL;
		CEasyString		SendNotifyURL;
		CEasyString		SendTarget;
	};
protected:
	CIPAddress					m_ListenAddress;
	UINT						m_MaxClient;
	UINT						m_RecvBufferSize;
	UINT						m_SendBufferSize;
	UINT						m_KeepAliveTime;
	UINT						m_KeepAliveCount;	
	UINT						m_ProcessInfoFetchTime;
	UINT						m_MaxWorkThreadCount;
	CEasyArray<USER_INFO>		m_UserList;
	NOTIFY_SEND_CONFIG			m_NotifyConfig;
	POOL_CONFIGS				m_PoolConfigs;
	TASK_MANAGER_CONFIG			m_TaskManagerConfig;
public:
	CMainConfig(void);
	~CMainConfig(void);

	bool LoadConfig(LPCTSTR FileName);	
	bool LoadServiceList(LPCTSTR FileName, CEasyArray<CServiceInfoEx>& ServiceList);
	bool SaveServiceList(LPCTSTR FileName, CEasyArray<CServiceInfoEx>& ServiceList);
	
	const POOL_CONFIGS& GetPoolConfigs()
	{
		return m_PoolConfigs;
	}
	CIPAddress& GetListenAddress()
	{
		return m_ListenAddress;
	}	
	UINT GetMaxClient()
	{
		return m_MaxClient;
	}
	UINT GetRecvBufferSize()
	{
		return m_RecvBufferSize;
	}
	UINT GetSendBufferSize()
	{
		return m_SendBufferSize;
	}
	UINT GetKeepAliveTime()
	{
		return m_KeepAliveTime;
	}
	UINT GetKeepAliveCount()
	{
		return m_KeepAliveCount;
	}
	UINT GetProcessInfoFetchTime()
	{
		return m_ProcessInfoFetchTime;
	}	
	UINT GetMaxWorkThreadCount()
	{
		return m_MaxWorkThreadCount;
	}
	const CEasyArray<USER_INFO>& GetUserList()
	{
		return m_UserList;
	}
	bool VerfyUser(LPCTSTR UserName, LPCTSTR Password);
	const NOTIFY_SEND_CONFIG GetNotifyConfig()
	{
		return m_NotifyConfig;
	}
	const TASK_MANAGER_CONFIG& GetTaskManagerConfig()
	{
		return m_TaskManagerConfig;
	}
protected:
	bool ReadPoolConfigs(xml_node& XMLContent, POOL_CONFIGS& Config);
};
