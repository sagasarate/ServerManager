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
protected:
	CIPAddress					m_ListenAddress;
	UINT						m_MaxClient;
	UINT						m_RecvBufferSize;
	UINT						m_SendBufferSize;
	UINT						m_KeepAliveTime;
	UINT						m_KeepAliveCount;
	
	UINT						m_ProcessInfoFetchTime;

	
public:
	CMainConfig(void);
	~CMainConfig(void);

	bool LoadConfig(LPCTSTR FileName);	
	bool LoadServiceList(LPCTSTR FileName, CEasyArray<SERVICE_INFO>& ServiceList);
	bool SaveServiceList(LPCTSTR FileName, CEasyArray<SERVICE_INFO>& ServiceList);
	
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
};
