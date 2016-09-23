/****************************************************************************/
/*                                                                          */
/*      文件名:    MainThread.h                                             */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

enum SIMPLE_FILE_SERVER_STATUS
{
	SFSS_WORKING,
	SFSS_INITIALIZING,
	SFSS_BUSY,
	SFSS_TERMINATING,
};

class CMainThread :
	public CServerThread,public CStaticObject<CMainThread>
{
protected:	
	
	CServerManagerService *	m_pServerManagerService;
	int						m_Status;
public:
	CMainThread(void);
	virtual ~CMainThread(void);

	virtual BOOL OnStart();

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);
	
	virtual void OnBeginTerminate();
	virtual BOOL OnTerminating();	

	virtual void OnTerminate();

	

	virtual int GetClientCount();
	
	virtual LPCTSTR GetConfigFileName();

	void SetServerStatus(int Status);
	int GetServerStatus();

	
protected:
	

};


inline void CMainThread::SetServerStatus(int Status)
{
	m_Status=Status;
}
inline int CMainThread::GetServerStatus()
{
	return m_Status;
}

