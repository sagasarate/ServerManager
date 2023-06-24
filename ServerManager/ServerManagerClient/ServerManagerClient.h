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
// ServerManagerClient.h : ServerManagerClient 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif




// CServerManagerClientApp:
// 有关此类的实现，请参阅 ServerManagerClient.cpp
//


class CServerManagerClientApp : public CWinApp
{
protected:

	CNetServer						m_Server;
	CIDStorage<CServerConnection>	m_ConnectionPool;		
	CEasyString						m_ServerUpdateListFile;
	CEasyString						m_SystemServiceName;

	CEasyString						m_AccountName;
	CEasyString						m_Password;

	CDlgServerUpdate				m_DlgServerUpdate;
public:
	CServerManagerClientApp();

	static CServerManagerClientApp * GetInstance();
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

	CServerConnection * AddServerConnection(LPCTSTR Name, LPCTSTR Group, LPCTSTR Address, UINT Port, LPCTSTR UserName, LPCTSTR Password);
	CServerConnection * GetServerConnection(LPCTSTR ServerAddress);
	CServerConnection * GetServerConnection(UINT ConnectionID);

	void * GetFirstServerConnectionPos()
	{
		return m_ConnectionPool.GetFirstObjectPos();
	}
	CServerConnection * GetNextServerConnection(LPVOID& Pos)
	{
		return m_ConnectionPool.GetNextObject(Pos);
	}
	bool DeleteServerConnection(UINT ConnectionID);
	

	void StartupService(UINT ConnectionID,UINT ServiceID);
	void ShutdownService(UINT ConnectionID, UINT ServiceID, BYTE ShutDownType);
	void BrowseWorkDir(UINT ConnectionID, UINT ServiceID, LPCTSTR Dir);

	LPCTSTR GetServerUpdateListFile()
	{
		return m_ServerUpdateListFile;
	}
	LPCTSTR GetSystemServiceName()
	{
		return m_SystemServiceName;
	}
	void SetServerUpdateListFile(LPCTSTR szFileName)
	{
		m_ServerUpdateListFile = szFileName;
		SaveConfig();
	}

	UINT AddDownloadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer);
	UINT AddUploadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer);
	UINT AddDeleteFileTask(UINT ConnectionID, UINT ServiceID, LPCTSTR TargetFilePath);
	UINT AddCreateDirTask(UINT ConnectionID, UINT ServiceID, LPCTSTR TargetFilePath);
	UINT AddChangeFileTask(UINT ConnectionID, UINT ServiceID, LPCTSTR TargetFilePath, UINT FileMode);
	UINT AddStartupServiceTask(UINT ConnectionID, UINT ServiceID, bool ClearQueueOnFailed);
	UINT AddShutdownServiceTask(UINT ConnectionID, UINT ServiceID, bool ClearQueueOnFailed);
	UINT AddReloadConfigDataTask(UINT ConnectionID, UINT ServiceID);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnServerManage();

	//void CheckServerConnections();

	void LoadConfig();
	void SaveConfig();
	
public:
	afx_msg void OnServerUpdate();
};

extern CServerManagerClientApp theApp;