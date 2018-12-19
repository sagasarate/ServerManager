/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerService.cpp                                 */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

IMPLEMENT_CLASS_INFO_STATIC(CServerManagerService,CNetService);

CServerManagerService::CServerManagerService(void)
{
	FUNCTION_BEGIN;
	//m_RecentTime = 0;
	//m_RecentTimeSpan = 0;
	m_CURLMHandle = NULL;
	m_HaveRequestRunning = 0;
	FUNCTION_END;
}

CServerManagerService::~CServerManagerService(void)
{
	FUNCTION_BEGIN;
	Destory();
	FUNCTION_END;
}


BOOL CServerManagerService::Init(CNetServer * pServer)
{
	FUNCTION_BEGIN;
	SetServer(pServer);
	if(!m_ClientPool.Create(CMainConfig::GetInstance()->GetMaxClient()))
	{
		Log("创建%u大小的客户端对象池失败",CMainConfig::GetInstance()->GetMaxClient());
		return FALSE;
	}
	Log("创建%u大小的客户端对象池成功",CMainConfig::GetInstance()->GetMaxClient());
	if(!DoListen(CMainConfig::GetInstance()->GetListenAddress()))
	{
		return FALSE;
	}
	CMainConfig::GetInstance()->LoadServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList);
	
	for (UINT i = 0; i<m_ServiceInfoList.GetCount(); i++)
	{
		m_ServiceInfoList[i].SetStatus( SERVICE_STATUS_NONE);
		m_ServiceInfoList[i].SetLastStatusChangeTime(time(NULL));		
	}

#ifdef WIN32
	FetchWinServiceInfo();
#endif

	m_ProcessInfoFetchTimer.SaveTime();

	//m_RecentTime = CEasyTimerEx::GetTime();


	InitNetAdapterInfo();


	if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
	{
		Log(_T("curl初始化失败"));
		return FALSE;
	}

	m_CURLMHandle = curl_multi_init();
	if (m_CURLMHandle == NULL)
	{
		Log(_T("curl_multi_init失败"));
		return FALSE;
	}
	
	m_HaveRequestRunning = 0;

	m_RequestPool.Create(32, 32, 32);

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerService::Destory()
{
	FUNCTION_BEGIN;
	void * Pos = m_RequestPool.GetFirstObjectPos();
	while (Pos)
	{
		HTTP_REQUEST_INFO * pRequestInfo = m_RequestPool.GetNextObject(Pos);
		if (pRequestInfo->CURLHandle)
		{
			curl_multi_remove_handle(m_CURLMHandle, pRequestInfo->CURLHandle);
			curl_easy_cleanup(pRequestInfo->CURLHandle);
			pRequestInfo->CURLHandle = NULL;
		}
	}
	m_RequestPool.Destory();
	if (m_CURLMHandle)
	{
		curl_multi_cleanup(m_CURLMHandle);
		m_CURLMHandle = NULL;
	}
	curl_global_cleanup();

	for (UINT i = 0; i < m_ServiceInfoList.GetCount(); i++)
	{
		SAFE_DELETE(m_ServiceInfoList[i].pControlPipe);
	}
	CNetService::Destory();
	FUNCTION_END;
}

CBaseNetConnection * CServerManagerService::CreateConnection(CIPAddress& RemoteAddress)
{
	FUNCTION_BEGIN;
	CServerManagerClient * pClient=NULL;
	UINT ID=m_ClientPool.NewObject(&pClient);
	if(pClient)
	{
		if(pClient->Init(ID,this))
		{
			return pClient;
		}
		else
		{
			LogDebug("初始化客户端对象失败");
		}
		m_ClientPool.DeleteObject(ID);
	}
	else
	{
		LogDebug("创建客户端对象失败");
	}

	FUNCTION_END;
	return NULL;
}

bool CServerManagerService::DeleteConnection(CBaseNetConnection * pConnection)
{
	FUNCTION_BEGIN;

	CServerManagerClient * pClient=(CServerManagerClient *)pConnection;
	pClient->Destory();
	m_ClientPool.DeleteObject(pClient->GetID());
	return TRUE;

	FUNCTION_END;
	return FALSE;
}


int CServerManagerService::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int Process=CNetService::Update(ProcessPacketLimit);

	CEasyArray<CServerManagerClient *> DeleteList;
	void * Pos=m_ClientPool.GetFirstObjectPos();
	while(Pos)
	{
		CServerManagerClient * pClient=m_ClientPool.GetNextObject(Pos);
		Process+=pClient->Update(ProcessPacketLimit);

		if(pClient->WantDelete())
		{
			DeleteList.Add(pClient);
		}
	}

	for(size_t i=0;i<DeleteList.GetCount();i++)
	{
		DeleteConnection(DeleteList[i]);
	}

	if(m_ProcessInfoFetchTimer.IsTimeOut(CMainConfig::GetInstance()->GetProcessInfoFetchTime()))
	{
		m_ProcessInfoFetchTimer.SaveTime();
		FetchProcessInfo();
		UpdateNetAdapterInfo();
	}


	UINT CurTime = time(NULL);
	for (UINT i = 0; i < m_ServiceInfoList.GetCount(); i++)
	{
		CServiceInfoEx& SeriveInfo = m_ServiceInfoList[i];
		if (SeriveInfo.GetType() == SERVICE_TYPE_NORMAL &&
			SeriveInfo.GetStatus() == SERVICE_STATUS_STOP && SeriveInfo.GetLastOperation() == SERVICE_OPERATION_STARTUP &&
			CurTime - SeriveInfo.GetLastStatusChangeTime() >= SeriveInfo.GetRestartupTime())
		{
			Log("检测到服务[%u](%s)已停止，重新启动", SeriveInfo.GetServiceID(), (LPCTSTR)SeriveInfo.GetName());
			StartupService(SeriveInfo.GetServiceID());
		}

		if (SeriveInfo.pControlPipe)
			Process += SeriveInfo.pControlPipe->Update(ProcessPacketLimit);
	}

	if (m_HaveRequestRunning)
	{
		Process += RunCURL();
	}
	if (m_RequestPool.GetObjectCount())
	{
		Process += FinishCURL();
	}


	return Process;
	FUNCTION_END;
	return 0;
}

CServiceInfoEx * CServerManagerService::GetServiceInfo(UINT ServiceID)
{
	for (UINT i = 0; i < m_ServiceInfoList.GetCount(); i++)
	{
		if (m_ServiceInfoList[i].GetServiceID() == ServiceID)
			return m_ServiceInfoList.GetObject(i);
	}
	return NULL;
}

LPCTSTR CServerManagerService::GetServiceWorkDir(UINT ServiceID)
{
	if (ServiceID == 0)
	{
		static CEasyString ModuleDir;

		ModuleDir = CFileTools::MakeFullPath(CFileTools::GetModulePath(NULL));

		return ModuleDir;
	}
	else
	{
		CServiceInfoEx * pServiceInfo = GetServiceInfo(ServiceID);
		if (pServiceInfo)
		{
			return pServiceInfo->GetWorkDir();
		}
	}
	return NULL;
}

int CServerManagerService::StartupService(UINT ServiceID)
{
	CServiceInfoEx * pServiceInfo = GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->GetType() == SERVICE_TYPE_NORMAL)
		{
			if (pServiceInfo->GetStatus() == SERVICE_STATUS_STOP)
			{
				if (StartupProcess(pServiceInfo->GetImageFilePath(), pServiceInfo->GetWorkDir(), pServiceInfo->GetStartupParam()))
				{
					Log("服务%u成功执行启动", ServiceID);
					pServiceInfo->SetLastOperation(SERVICE_OPERATION_STARTUP);
					pServiceInfo->SetStatus(SERVICE_STATUS_STARTUPPING);
					pServiceInfo->SetLastStatusChangeTime(time(NULL));
					SendNotify(pServiceInfo, "Service Startup");
					return MSG_RESULT_SUCCEED;
				}
				else
				{
					Log("服务%u执行启动失败", ServiceID);
				}
			}
			else
			{
				Log("服务%u状态错误%d", ServiceID, pServiceInfo->GetStatus());
			}
		}
		else
		{
			if (StartupWinService(pServiceInfo->GetName()))
			{
				Log("Win服务%u成功执行启动", ServiceID);
				pServiceInfo->SetLastOperation(SERVICE_OPERATION_STARTUP);
				pServiceInfo->SetStatus(SERVICE_STATUS_STARTUPPING);
				pServiceInfo->SetLastStatusChangeTime(time(NULL));
				return MSG_RESULT_SUCCEED;
			}
			else
			{
				Log("服务%u执行启动失败", ServiceID);
			}
		}
		
		return MSG_RESULT_FAILED;
	}
	else
	{
		Log("服务%u不存在", ServiceID);
		return MSG_RESULT_SERVICE_NOT_EXIST;
	}
}

int CServerManagerService::ShutdownService(UINT ServiceID, int ShutdownType)
{
	CServiceInfoEx * pServiceInfo = GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->GetType() == SERVICE_TYPE_NORMAL)
		{
			if (pServiceInfo->GetStatus() == SERVICE_STATUS_RUNNING)
			{
				if (ShutdownType == SERVICE_SHUTDOWN_TYPE_SAFE)
				{
					if (!pServiceInfo->GetShutdownCmd().IsEmpty())
					{
						if (pServiceInfo->pControlPipe)
						{
							if (pServiceInfo->pControlPipe->IsConnected())
							{
								if (pServiceInfo->pControlPipe->SendCommand(0, pServiceInfo->GetShutdownCmd()))
								{
									pServiceInfo->SetLastOperation(SERVICE_OPERATION_SHUTDOWN);
									pServiceInfo->SetStatus(SERVICE_STATUS_SHUTDOWNNING);
									pServiceInfo->SetLastStatusChangeTime(time(NULL));
									SendNotify(pServiceInfo, "Service Shutdown Safety");
									return MSG_RESULT_SUCCEED;
								}
								else
								{
									return MSG_RESULT_FAILED;
								}
							}
						}
						return MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE;
					}
					else
					{
						return MSG_RESULT_SHUTDOWN_CMD_NOT_SET;
					}
				}
				else
				{
					if (ShutdownProcess(pServiceInfo->GetProcessID(), ShutdownType))
					{
						Log("服务%u成功执行关闭%d", ServiceID, ShutdownType);
						pServiceInfo->SetLastOperation(SERVICE_OPERATION_SHUTDOWN);
						pServiceInfo->SetStatus(SERVICE_STATUS_SHUTDOWNNING);
						pServiceInfo->SetLastStatusChangeTime(time(NULL));
						SendNotify(pServiceInfo, "Service Shutdown");
						return MSG_RESULT_SUCCEED;
					}
				}
			}
			else
			{
				pServiceInfo->SetLastOperation(SERVICE_OPERATION_SHUTDOWN);
				Log("服务%u已经关闭%d", ServiceID, ShutdownType);
			}
		}
		else if (pServiceInfo->GetType() == SERVICE_TYPE_WIN_SERVICE)
		{
			if (ShutdownWinService(pServiceInfo->GetName()))
			{
				Log("Win服务%u成功执行关闭", ServiceID);
				pServiceInfo->SetLastOperation(SERVICE_OPERATION_SHUTDOWN);
				pServiceInfo->SetStatus(SERVICE_STATUS_SHUTDOWNNING);
				pServiceInfo->SetLastStatusChangeTime(time(NULL));
				return MSG_RESULT_SUCCEED;
			}

		}
		Log("服务%u执行关闭失败%d", ServiceID, ShutdownType);
		return MSG_RESULT_FAILED;
	}
	else
	{
		Log("服务%u不存在", ServiceID);
		return MSG_RESULT_SERVICE_NOT_EXIST;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CServerManagerService::DoListen(CIPAddress& ListenAddress)
{
	FUNCTION_BEGIN;
	Close();
	if (!Create(IPPROTO_TCP, DEFAULT_SERVER_ACCEPT_QUEUE,
		(CMainConfig::GetInstance()->GetRecvBufferSize() / NET_DATA_BLOCK_SIZE + 1),
		(CMainConfig::GetInstance()->GetSendBufferSize() / NET_DATA_BLOCK_SIZE + 1)
		))
	{
		Log("初始化服务失败");
		return FALSE;
	}
	if(!StartListen(ListenAddress))
	{
		Log("SimpleFileService侦听于%s:%u失败",
			ListenAddress.GetIPString(),ListenAddress.GetPort());
		return FALSE;
	}
	Log("SimpleFileService侦听于%s:%u成功",
		ListenAddress.GetIPString(),ListenAddress.GetPort());
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerService::FetchProcessInfo()
{
	//UINT64 SystemTime = CEasyTimerEx::GetTime();
	//if (m_RecentTime)
	//	m_RecentTimeSpan = SystemTime - m_RecentTime;
	//m_RecentTime = SystemTime;

	UINT CurTime = time(NULL);

	m_ProcessSnapshot.Snapshot();
	const CEasyArray<CProcessSnapshot::PROCESS_INFO>& ProcessList = m_ProcessSnapshot.GetProcessList();

	m_ProcessInfoList.GetList().Resize(ProcessList.GetCount());
	for (UINT i = 0; i < ProcessList.GetCount(); i++)
	{
		m_ProcessInfoList.GetList()[i].SetProcessID(ProcessList[i].ProcessID);
		m_ProcessInfoList.GetList()[i].SetImageFilePath(ProcessList[i].ImageFile);
		m_ProcessInfoList.GetList()[i].SetCPUUsedTime(ProcessList[i].CPUUsedTime);
		m_ProcessInfoList.GetList()[i].SetCPUUsed(ProcessList[i].CPUUsedRate);
		m_ProcessInfoList.GetList()[i].SetMemoryUsed(ProcessList[i].MemoryUsed);
		m_ProcessInfoList.GetList()[i].SetVirtualMemoryUsed(ProcessList[i].VirtualMemoryUsed);
	}

	for(UINT j=0;j<m_ServiceInfoList.GetCount();j++)
	{
		CServiceInfoEx& ServiceInfo = m_ServiceInfoList[j];
		ServiceInfo.SetProcessID(0);
		ServiceInfo.SetCPUUsed(0);
		ServiceInfo.SetMemoryUsed(0);
		ServiceInfo.SetVirtualMemoryUsed(0);
		for(UINT i=0;i<m_ProcessInfoList.GetList().GetCount();i++)
		{
			CProcessInfo& ProcessInfo = m_ProcessInfoList.GetList()[i];
			if(!ServiceInfo.GetImageFilePath().IsEmpty())
			{
				if (ServiceInfo.GetImageFilePath().CompareNoCase(ProcessInfo.GetImageFilePath()) == 0)
				{
					ServiceInfo.CProcessInfo::CloneFrom(ProcessInfo, DOMF_PROCESS_INFO_FOR_STATUS_FETCH);					
				}
			}
		}
		if (ServiceInfo.GetProcessID())
		{
			if ((!ServiceInfo.GetControlPipeName().IsEmpty()) && (ServiceInfo.pControlPipe == NULL))
			{
				CEasyString PipeName;
				PipeName.Format("%s(%u)", (LPCTSTR)ServiceInfo.GetControlPipeName(), ServiceInfo.GetProcessID());
				ServiceInfo.pControlPipe = new CServiceControlPipe(ServiceInfo.GetServiceID());
			}
		}
		if (ServiceInfo.pControlPipe)
		{
			ServiceInfo.pControlPipe->QueryGetServerWorkStatus();
		}
		switch (ServiceInfo.GetStatus())
		{
		case SERVICE_STATUS_NONE:
			if (ServiceInfo.GetProcessID() == 0)
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_STOP);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
			}
			else
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_RUNNING);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
			}
			break;
		case SERVICE_STATUS_STOP:
			if (ServiceInfo.GetProcessID() != 0)
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_RUNNING);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
			}
			break;
		case SERVICE_STATUS_RUNNING:
			if (ServiceInfo.GetProcessID() == 0)
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_STOP);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
				if (ServiceInfo.GetLastOperation() == SERVICE_OPERATION_STARTUP)
					SendNotify(&ServiceInfo, "Service Shutdown Unexpected");
			}
			break;
		case SERVICE_STATUS_SHUTDOWNNING:
			if (ServiceInfo.GetProcessID() == 0)
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_STOP);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
			}
			else if (CurTime - ServiceInfo.GetLastStatusChangeTime() > MAX_OPERATION_WAIT_TIME)
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_RUNNING);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
			}
			break;
		case SERVICE_STATUS_STARTUPPING:
			if (ServiceInfo.GetProcessID() != 0)
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_RUNNING);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
			}
			else if (CurTime - ServiceInfo.GetLastStatusChangeTime() > MAX_OPERATION_WAIT_TIME)
			{
				ServiceInfo.SetStatus(SERVICE_STATUS_STOP);
				ServiceInfo.SetLastStatusChangeTime(time(NULL));
			}
			break;
		}		

		CFileInfo FileInfo;

		if (FileInfo.FetchFileInfo(ServiceInfo.GetImageFilePath()))
		{
			time_t LastExecFileTime = FileInfo.GetLastWriteTime();
			for (UINT i = 0; i < ServiceInfo.GetOtherExecFileList().GetCount(); i++)
			{
				if (FileInfo.FetchFileInfo(ServiceInfo.GetOtherExecFileList()[i]))
				{
					if (FileInfo.GetLastWriteTime() > LastExecFileTime)
						LastExecFileTime = FileInfo.GetLastWriteTime();
				}
			}
			ServiceInfo.SetImageFileTime(LastExecFileTime);
		}
	}

}

#ifdef WIN32
void CServerManagerService::FetchWinServiceInfo()
{
	for(UINT i=0;i<m_ServiceInfoList.GetCount();i++)
	{
		CServiceInfoEx& ServiceInfo = m_ServiceInfoList[i];

		if (ServiceInfo.GetType() == SERVICE_TYPE_WIN_SERVICE)
		{
			CWinServiceController ServiceController;
			if(ServiceController.OpenService(ServiceInfo.GetName()))
			{
				TCHAR Path[MAX_PATH];
				ServiceController.GetServiceImageFilePath(Path, MAX_PATH);
				ServiceInfo.SetImageFilePath(Path);
				CEasyString WorkDir = CFileTools::GetPathDirectory(Path);
				WorkDir.TrimRight(DIR_SLASH);
				ServiceInfo.SetWorkDir(WorkDir);
				ServiceController.CloseService();
			}
		}
	}
}
#endif

int CServerManagerService::StartupProcess(LPCTSTR szImageFileName, LPCTSTR szWorkDir, LPCTSTR szStartupParam)
{
#ifdef WIN32
	CEasyString CommandLine;

	CommandLine.Format("\"%s\" %s",
		szImageFileName, szStartupParam);


	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);

	PROCESS_INFORMATION ProcessInfo;

	ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

	if (CreateProcess(szImageFileName, CommandLine, NULL, NULL, FALSE, 0, NULL, szWorkDir, &StartupInfo, &ProcessInfo))
	{
		CloseHandle( ProcessInfo.hProcess );
		CloseHandle( ProcessInfo.hThread );
		return true;
	}
	return false;
#else
	CStringSplitter Splitter(szStartupParam, ' ');

	LPTSTR * argv = new LPTSTR[Splitter.GetCount() + 2];
	argv[0] = (LPTSTR)szImageFileName;
	argv[Splitter.GetCount() + 1] = NULL;
	for (UINT i = 0; i < Splitter.GetCount(); i++)
	{
		argv[i + 1] = (LPTSTR)Splitter[i];
	}

	pid_t pid = fork();
	if (pid > 0)
	{
		delete[] argv;
		return true;
	}
	else if (pid == 0)
	{
		execv(szImageFileName, argv);
		delete[] argv;
		PrintImportantLog("运行程序失败%d", errno);
		exit(2);
	}
	else
	{
		delete[] argv;
		Log("新建进程失败%d", errno);
		return false;
	}
#endif
}

int CServerManagerService::ShutdownProcess(UINT ProcessID, int ShutdownType)
{
	switch (ShutdownType)
	{	
	case SERVICE_SHUTDOWN_TYPE_NORMAL:
#ifdef WIN32
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
			if (hProcess != NULL)
			{
				bool Ret = TerminateProcess(hProcess, 0) != FALSE;
				CloseHandle(hProcess);
				Log("终止进程%u%s(%d)", ProcessID, Ret ? "成功" : "失败", ShutdownType);
				return Ret;
			}
			else
			{
				Log("无法打开进程%u", ProcessID);
			}
		}
#else
		{
			int sig = SIGTERM;			
			if (kill(ProcessID, sig) == 0)
			{
				Log("以信号%d终止进程%u成功", sig, ProcessID);
				return true;
			}
			else
			{
				Log("以信号%d终止进程%u失败%d", sig, ProcessID, errno);
				return false;
			}
		}
#endif
		break;
	case SERVICE_SHUTDOWN_TYPE_FORCE:
#ifdef WIN32
		return PostQuitMsgToProcess(ProcessID);
#else
		{
			int sig = SIGKILL;
			if (kill(ProcessID, sig) == 0)
			{
				Log("以信号%d终止进程%u成功", sig, ProcessID);
				return true;
			}
			else
			{
				Log("以信号%d终止进程%u失败%d", sig, ProcessID, errno);
				return false;
			}
		}
#endif
		break;
	case SERVICE_SHUTDOWN_TYPE_SAFE:
		return false;
		break;
	}
	return false;
}

int CServerManagerService::CheckServiceInfo(const CServiceInfo& ServiceInfo)
{
#ifndef WIN32
	if (ServiceInfo.GetType() == SERVICE_TYPE_WIN_SERVICE)
		return MSG_RESULT_SERVICE_TYPE_NOT_SUPPORT;
#endif
	if (ServiceInfo.GetType() == SERVICE_TYPE_NORMAL || ServiceInfo.GetType() == SERVICE_TYPE_WIN_SERVICE)
	{
		if (!CFileTools::IsFileExist(ServiceInfo.GetImageFilePath()))
			return MSG_RESULT_FILE_NOT_EXIST;
	}

	if (!CFileTools::IsDirExist(ServiceInfo.GetWorkDir()))
		return MSG_RESULT_DIR_NOT_EXIST;

	return MSG_RESULT_SUCCEED;
}
int CServerManagerService::AddService(const CServiceInfo& ServiceInfo)
{
	int Result = CheckServiceInfo(ServiceInfo);
	if (Result != MSG_RESULT_SUCCEED)
		return Result;
	
	for (UINT i = 0; i < m_ServiceInfoList.GetCount(); i++)
	{
		if (m_ServiceInfoList[i].GetServiceID() == ServiceInfo.GetServiceID())
			return MSG_RESULT_SERVICE_ID_IS_USED;
	}
	
	CServiceInfoEx SrvInfo;
	SrvInfo.CloneFrom(ServiceInfo, DOMF_SERVICE_INFO_FULL);

	SrvInfo.SetImageFilePath(CFileTools::MakeFullPath(SrvInfo.GetImageFilePath()));
	SrvInfo.GetWorkDir().Trim();
	if (SrvInfo.GetWorkDir().IsEmpty() && SrvInfo.GetType() != SERVICE_TYPE_DIRECTORY)
	{
		SrvInfo.SetWorkDir(CFileTools::GetPathDirectory(SrvInfo.GetImageFilePath()));
	}
	else
	{
		SrvInfo.SetWorkDir(CFileTools::MakeFullPath(SrvInfo.GetWorkDir()));
	}
	SrvInfo.GetWorkDir().TrimRight(DIR_SLASH);

	if (CFileTools::CreateDirEx(SrvInfo.GetWorkDir()))
	{
		m_ServiceInfoList.Add(SrvInfo);
		CMainConfig::GetInstance()->SaveServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList);
		return MSG_RESULT_SUCCEED;
	}
	else
	{
		return MSG_RESULT_DIR_NOT_EXIST;
	}	
}
int CServerManagerService::EditService(const CServiceInfo& ServiceInfo)
{
	int Result = CheckServiceInfo(ServiceInfo);
	if (Result != MSG_RESULT_SUCCEED)
		return Result;

	for (UINT i = 0; i < m_ServiceInfoList.GetCount(); i++)
	{
		if (m_ServiceInfoList[i].GetServiceID() == ServiceInfo.GetServiceID())
		{
			CServiceInfoEx& SrvInfo = m_ServiceInfoList[i];
			SrvInfo.CloneFrom(ServiceInfo, DOMF_SERVICE_INFO_FOR_EDIT);
			
			
			SrvInfo.SetImageFilePath(CFileTools::MakeFullPath(SrvInfo.GetImageFilePath()));
			SrvInfo.GetWorkDir().Trim();
			if (SrvInfo.GetWorkDir().IsEmpty() && SrvInfo.GetType() != SERVICE_TYPE_DIRECTORY)
			{
				SrvInfo.SetWorkDir(CFileTools::GetPathDirectory(SrvInfo.GetImageFilePath()));
			}
			else
			{
				SrvInfo.SetWorkDir(CFileTools::MakeFullPath(SrvInfo.GetWorkDir()));
			}
			SrvInfo.GetWorkDir().TrimRight(DIR_SLASH);

			SAFE_DELETE(SrvInfo.pControlPipe);

			CMainConfig::GetInstance()->SaveServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList);
			return MSG_RESULT_SUCCEED;
		}
	}
	return MSG_RESULT_SERVICE_NOT_EXIST;
}
int CServerManagerService::DeleteService(UINT ServiceID)
{
	for (UINT i = 0; i < m_ServiceInfoList.GetCount(); i++)
	{
		if (m_ServiceInfoList[i].GetServiceID() == ServiceID)
		{
			SAFE_DELETE(m_ServiceInfoList[i].pControlPipe);
			m_ServiceInfoList.Delete(i);
			CMainConfig::GetInstance()->SaveServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList);
			return MSG_RESULT_SUCCEED;
		}
	}
	return MSG_RESULT_SERVICE_NOT_EXIST;
}

//int CServerManagerService::SendCommand(UINT ServiceID, LPCTSTR szCommand)
//{
//	SERVICE_INFO_EX * pServiceInfo = GetServiceInfo(ServiceID);
//	if (pServiceInfo)
//	{
//		if (pServiceInfo->pControlPipe)
//		{
//			if (pServiceInfo->pControlPipe->IsConnected())
//			{
//				if (pServiceInfo->pControlPipe->SendCommand(szCommand))
//				{
//					return MSG_RESULT_SUCCEED;
//				}
//				else
//				{
//					return MSG_RESULT_FAILED;
//				}
//			}
//			else
//			{
//				return MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE;
//			}
//		}
//		else
//		{
//			return MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE;
//		}
//	}
//	else
//	{
//		return MSG_RESULT_SERVICE_NOT_EXIST;
//	}
//}

int CServerManagerService::EnableLogRecv(UINT ServiceID, bool Enable)
{
	//遍历所有链接确定最终状态
	void * Pos = m_ClientPool.GetFirstObjectPos();
	while (Pos)
	{
		CServerManagerClient * pClient = m_ClientPool.GetNextObject(Pos);
		Enable = Enable | pClient->IsLogRecv(ServiceID);
	}

	CServiceInfoEx * pServiceInfo = GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->pControlPipe)
		{
			if (pServiceInfo->pControlPipe->QueryEnableLogRecv(Enable))
			{
				return MSG_RESULT_SUCCEED;
			}
			else
			{
				return MSG_RESULT_FAILED;
			}
		}
		else
		{
			return MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE;
		}
	}
	else
	{
		return MSG_RESULT_SERVICE_NOT_EXIST;
	}
}

void CServerManagerService::OnGetServiceWorkStatus(UINT ServiceID, BYTE WorkStatus)
{
	CServiceInfoEx * pServiceInfo = GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		pServiceInfo->SetWorkStatus(WorkStatus);
	}
}

void CServerManagerService::OnServerLogMsg(UINT ServiceID, LPCTSTR szLogMsg)
{
	void * Pos = m_ClientPool.GetFirstObjectPos();
	while (Pos)
	{
		CServerManagerClient * pClient = m_ClientPool.GetNextObject(Pos);
		if (pClient->IsLogRecv(ServiceID))
		{
			CServerManagerAckMsgCaller MsgCaller(pClient);

			MsgCaller.ConsoleLogNotify(ServiceID, szLogMsg);
		}
	}
}

void CServerManagerService::OnSendCommandResult(UINT ServiceID, short Result)
{

}

void CServerManagerService::SendNotify(CServiceInfoEx * pServiceInfo, LPCTSTR szMsg)
{
	if (CMainConfig::GetInstance()->GetNotifyConfig().GetTokenURL.GetLength() &&
		CMainConfig::GetInstance()->GetNotifyConfig().SendNotifyURL.GetLength() &&
		CMainConfig::GetInstance()->GetNotifyConfig().SendTarget.GetLength())
	{
		CEasyString MsgContent;
		MsgContent.Format("{"
			"\"touser\" : \"%s\","
			"\"msgtype\" : \"text\","
			"\"agentid\" : 1000007,"
			"\"text\" : {\"content\" : \"(%s)(%s)%s\"},"
			"\"safe\" : 0",
			(LPCTSTR)CMainConfig::GetInstance()->GetNotifyConfig().SendTarget,
			(LPCTSTR)m_ServerIP,
			(LPCTSTR)pServiceInfo->GetName(),
			szMsg
			);
		AddRequest(CMainConfig::GetInstance()->GetNotifyConfig().GetTokenURL, HTTP_REQUEST_TYPE_GET_TOKEN, 0, 0, MsgContent, NULL);
	}
	else
	{
		Log("消息发送设置不正确");
	}
}

void CServerManagerService::InitNetAdapterInfo()
{
	m_NetAdapterInfos.Init();

	CEasyArray<CNetAdapterInfos::NET_ADAPTER_INFO> InfoList = m_NetAdapterInfos.GetNetAdapterInfoList();
	m_NetAdapterInfoList.List.Resize(InfoList.GetCount());

	for (UINT i = 0; i < InfoList.GetCount(); i++)
	{
		m_NetAdapterInfoList.List[i].Index = InfoList[i].Index;
		m_NetAdapterInfoList.List[i].Name = InfoList[i].Name;
		m_NetAdapterInfoList.List[i].SendBytes = InfoList[i].SendBytes;
		m_NetAdapterInfoList.List[i].SendFlux = InfoList[i].SendFlux;
		m_NetAdapterInfoList.List[i].RecvBytes = InfoList[i].RecvBytes;
		m_NetAdapterInfoList.List[i].RecvFlux = InfoList[i].RecvFlux;
		m_NetAdapterInfoList.List[i].IPAddressList.Resize(InfoList[i].IPAddressList.GetCount());
		for (UINT j = 0; j < InfoList[i].IPAddressList.GetCount(); j++)
		{
			m_NetAdapterInfoList.List[i].IPAddressList[j] = InfoList[i].IPAddressList[j].GetIPString();

			if ((!InfoList[i].IPAddressList[j].IsLocalAddress()) && (!InfoList[i].IPAddressList[j].IsLoopbackAddress()))
				m_ServerIP = InfoList[i].IPAddressList[j].GetIPString();
		}

	}
}

void CServerManagerService::UpdateNetAdapterInfo()
{
	m_NetAdapterInfos.FetchStatus();
	CEasyArray<CNetAdapterInfos::NET_ADAPTER_INFO> InfoList = m_NetAdapterInfos.GetNetAdapterInfoList();
	if (m_NetAdapterInfoList.List.GetCount() == InfoList.GetCount())
	{
		for (UINT i = 0; i < InfoList.GetCount(); i++)
		{
			m_NetAdapterInfoList.List[i].SendBytes = InfoList[i].SendBytes;
			m_NetAdapterInfoList.List[i].SendFlux = InfoList[i].SendFlux;
			m_NetAdapterInfoList.List[i].RecvBytes = InfoList[i].RecvBytes;
			m_NetAdapterInfoList.List[i].RecvFlux = InfoList[i].RecvFlux;
			m_NetAdapterInfoList.List[i].IPAddressList.Resize(InfoList[i].IPAddressList.GetCount());
		}
	}
	else
	{
		Log("异常，网卡信息不匹配");
	}
}


bool CServerManagerService::StartupWinService(LPCTSTR szServiceName)
{
#ifdef WIN32
	CWinServiceController ServiceController;
	if(ServiceController.OpenService(szServiceName))
	{
		return ServiceController.StartupService(NULL,0);
		ServiceController.CloseService();
	}
#endif
	return false;
}
bool CServerManagerService::ShutdownWinService(LPCTSTR szServiceName)
{
#ifdef WIN32
	CWinServiceController ServiceController;
	if(ServiceController.OpenService(szServiceName))
	{
		return ServiceController.ShutdownService();
		ServiceController.CloseService();
	}
#endif
	return false;
}

#ifdef WIN32
bool CServerManagerService::PostQuitMsgToProcess(UINT ProcessID)
{
	if (EnumWindows(EnumWindowsProc, ProcessID))
	{
		return true;
	}
	return false;
}

BOOL CServerManagerService::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwProcessId == lParam)
	{
		PostMessage(hWnd, WM_QUIT, 0, 0);
	}
	return true;
}
#else
int CServerManagerService::ProcessFN(void * pParam)
{

}
#endif




bool CServerManagerService::AddRequest(LPCSTR szURL, int RequestType, UINT Param1, UINT Param2, LPCTSTR Param3, LPCTSTR PostContent)
{
	HTTP_REQUEST_INFO * pRequestInfo = m_RequestPool.NewObject();
	if (pRequestInfo)
	{
		pRequestInfo->RequestURL = szURL;
		pRequestInfo->RequestType = RequestType;
		pRequestInfo->Param1 = Param1;
		pRequestInfo->Param2 = Param2;
		if (Param3)
			pRequestInfo->Param3 = Param3;
		else
			pRequestInfo->Param3.Clear();
		if (PostContent)
			pRequestInfo->RequestContent = PostContent;
		else
			pRequestInfo->RequestContent.Clear();
		if (pRequestInfo->CURLHandle)
		{
			curl_multi_remove_handle(m_CURLMHandle, pRequestInfo->CURLHandle);
			curl_easy_cleanup(pRequestInfo->CURLHandle);
			pRequestInfo->CURLHandle = NULL;
		}
		pRequestInfo->CURLHandle = curl_easy_init();
		if (pRequestInfo->CURLHandle == NULL)
		{
			m_RequestPool.DeleteObject(pRequestInfo->RequestID);
			Log(_T("curl_easy_init失败"));
			return false;
		}
		if (pRequestInfo->RecvBuffer.GetBufferSize() == 0)
		{
			pRequestInfo->RecvBuffer.Create(65536);
		}
		else
		{
			pRequestInfo->RecvBuffer.SetUsedSize(0);
		}


		if (!SendRequest(pRequestInfo))
		{
			curl_multi_remove_handle(m_CURLMHandle, pRequestInfo->CURLHandle);
			curl_easy_cleanup(pRequestInfo->CURLHandle);
			pRequestInfo->CURLHandle = NULL;
			m_RequestPool.DeleteObject(pRequestInfo->RequestID);
			return false;
		}
		else
		{
			LogDebug("发送请求[%s]", (LPCSTR)pRequestInfo->RequestURL);
		}

		return true;
	}
	else
	{
		Log(_T("分配请求缓冲数据失败(%u/%u)"), m_RequestPool.GetObjectCount(), m_RequestPool.GetBufferSize());
	}
	return false;
}

bool CServerManagerService::SendRequest(HTTP_REQUEST_INFO * pRequestInfo)
{
	int Result;
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_URL, (LPCSTR)pRequestInfo->RequestURL);
	if (Result != CURLE_OK)
	{
		Log(_T("设置URL失败%d"), Result);
		return false;
	}
	if (pRequestInfo->RequestContent.GetLength())
	{
		Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POSTFIELDS, (LPCTSTR)pRequestInfo->RequestContent);
		if (Result != CURLE_OK)
		{
			Log(_T("设置Post内容失败%d"), Result);
			return false;
		}
		Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POST, 1L);
		if (Result != CURLE_OK)
		{
			Log(_T("设置Post开关失败%d"), Result);
			return false;
		}
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_WRITEFUNCTION, CServerManagerService::ReadCallback);
	if (Result != CURLE_OK)
	{
		Log(_T("设置回调函数失败%d"), Result);
		return false;
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_WRITEDATA, pRequestInfo);
	if (Result != CURLE_OK)
	{
		Log(_T("设置回调参数失败%d"), Result);
		return false;
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_PRIVATE, pRequestInfo);
	if (Result != CURLE_OK)
	{
		Log(_T("设置参数失败%d"), Result);
		return false;
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1700.76 Safari/537.36");
	if (Result != CURLE_OK)
	{
		Log(_T("设置USERAGENT失败%d"), Result);
		return false;
	}

	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_ACCEPT_ENCODING, "gzip,deflate,sdch");
	if (Result != CURLE_OK)
	{
		Log(_T("设置ACCEPT_ENCODING失败%d"), Result);
		return false;
	}

	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_SSL_VERIFYHOST, 0);
	if (Result != CURLE_OK)
	{
		Log(_T("设置SSL参数失败%d"), Result);
		return false;
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_SSL_VERIFYPEER, 0);
	if (Result != CURLE_OK)
	{
		Log(_T("设置SSL参数失败%d"), Result);
		return false;
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_FOLLOWLOCATION, 1);
	if (Result != CURLE_OK)
	{
		Log(_T("设置FOLLOWLOCATION参数失败%d"), Result);
		return false;
	}
	//Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_USERPWD, "hjmlcs:f4fd5df438e8fd32b3b3468bdf08dc78"); 
	//Result=curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_ISSUERCERT,"curl-ca-bundle.cer"); 
	int MResult;
	MResult = curl_multi_add_handle(m_CURLMHandle, pRequestInfo->CURLHandle);
	if (MResult != CURLM_OK)
	{
		Log(_T("添加请求失败%d"), MResult);
		return false;
	}
	MResult = curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
	if (MResult != CURLM_OK&&MResult != CURLM_CALL_MULTI_PERFORM)
	{
		Log(_T("处理请求失败%d"), MResult);
		return false;
	}
	return true;
}



void CServerManagerService::ProcessRequestEnd(HTTP_REQUEST_INFO * pRequestInfo, CURLcode Result, long ResponseCode)
{
	char * pNewUrl = NULL;

	if (Result == CURLE_OK)
	{
		LPCSTR pData = (LPCSTR)pRequestInfo->RecvBuffer.GetBuffer();
		UINT DataLen = pRequestInfo->RecvBuffer.GetUsedSize();
		((BYTE *)pData)[DataLen] = 0;

		LogDebug("访问[%s]返回%d:%s", (LPCSTR)pRequestInfo->RequestURL, ResponseCode, pData);

		
		OnRequestResult(pRequestInfo->RequestType, ResponseCode, pRequestInfo->Param1, pRequestInfo->Param2, pRequestInfo->Param3, pData);
	}
	else
	{
		Log("访问[%s]失败(%d,%d)", (LPCSTR)pRequestInfo->RequestURL, Result, ResponseCode);
	}
	curl_multi_remove_handle(m_CURLMHandle, pRequestInfo->CURLHandle);
	curl_easy_cleanup(pRequestInfo->CURLHandle);
	pRequestInfo->CURLHandle = NULL;
	m_RequestPool.DeleteObject(pRequestInfo->RequestID);
}

void CServerManagerService::OnRequestResult(int RequestType, long ResponseCode, UINT Param1, UINT Param2, LPCTSTR Param3, LPCTSTR Content)
{
	switch (RequestType)
	{
	case HTTP_REQUEST_TYPE_GET_TOKEN:
		{
			rapidjson::Document Root;
			Root.Parse(Content);
			if (!Root.HasParseError())
			{
				if (Root.HasMember("errcode"))
				{
					rapidjson::Value& ErrorCode = Root["errcode"];
					if (ErrorCode.IsInt())
					{
						int ErrCode = ErrorCode.GetInt();
						if (ErrCode == 0)
						{
							if (Root.HasMember("access_token"))
							{
								rapidjson::Value& TokenValue = Root["access_token"];
								if (TokenValue.IsString())
								{
									CEasyString Token = TokenValue.GetString();

									CEasyString URL;
									URL.Format("%s?access_token=%s", 
										(LPCTSTR)CMainConfig::GetInstance()->GetNotifyConfig().SendNotifyURL,
										(LPCTSTR)Token);

									AddRequest(URL, HTTP_REQUEST_TYPE_SEND_NOTIFY, 0, 0, NULL, Param3);
								}
								else
								{
									Log("返回数据格式错误,access_token字段不是字符串");
								}
							}
							else
							{
								Log("返回数据格式错误,没有access_token字段");
							}
						}
						else
						{
							Log("GetToken错误%d", ErrCode);
						}

					}
					else
					{
						Log("返回数据格式错误,errcode字段不是数字");
					}
				}
				else
				{
					Log("返回数据格式错误,没有errcode字段");
				}
			}
			else
			{
				Log("数据解析失败");
			}

			
		}
		break;
	case HTTP_REQUEST_TYPE_SEND_NOTIFY:
		break;
	default:
		Log(_T("未知的请求类型%d"), RequestType);
	}
}
size_t CServerManagerService::ReadCallback(void *ptr, size_t size, size_t nmemb, void *userp)
{
	HTTP_REQUEST_INFO * pRequestInfo = (HTTP_REQUEST_INFO *)userp;

	size = size*nmemb;

	if (pRequestInfo->RecvBuffer.GetFreeSize() > size)
	{
		pRequestInfo->RecvBuffer.PushBack(ptr, size);
		pRequestInfo->RecvBuffer.PushConstBack(0, 1);
		pRequestInfo->RecvBuffer.PopBack(NULL, 1);
		return size;
	}
	else
		return 0;
}


int CServerManagerService::RunCURL()
{
	int ProcessCount = 0;

	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	int maxfd = -1;
	int rc;

	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);
	FD_ZERO(&fdexcep);

	/* get file descriptors from the transfers */
	int Result = curl_multi_fdset(m_CURLMHandle, &fdread, &fdwrite, &fdexcep, &maxfd);



	long TimeOut = -1;
	timeval	SelectTimeOut;
	Result = curl_multi_timeout(m_CURLMHandle, &TimeOut);
	if (TimeOut >= 0)
	{
		SelectTimeOut.tv_sec = TimeOut / 1000;
		if (SelectTimeOut.tv_sec > 0)
		{
			SelectTimeOut.tv_sec = 1;
			SelectTimeOut.tv_usec = 0;
		}
		else
		{
			SelectTimeOut.tv_usec = (TimeOut % 1000) * 1000;
		}
	}
	else
	{
		SelectTimeOut.tv_sec = 0;
		SelectTimeOut.tv_usec = 0;
	}

	/* In a real-world program you OF COURSE check the return code of the
	function calls.  On success, the value of maxfd is guaranteed to be
	greater or equal than -1.  We call select(maxfd + 1, ...), specially in
	case of (maxfd == -1), we call select(0, ...), which is basically equal
	to sleep. */
	if (maxfd >= 0)
	{
		rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &SelectTimeOut);

		switch (rc)
		{
		case -1:
			/* select error */
		{
#ifdef WIN32
			int ErrCode = WSAGetLastError();
#else
			int ErrCode = errno;
#endif
			Log("SelectError=%d", ErrCode);
		}
		break;
		case 0:
		default:
			/* timeout or readable/writable sockets */
			curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
			ProcessCount++;
			break;
		}
	}
	//else
	//{
	//	curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
	//	ProcessCount++;
	//}
	return ProcessCount;
}
int CServerManagerService::FinishCURL()
{
	int ProcessCount = 0;
	int MsgLeft = 0;
	CURLMsg * pMsg = curl_multi_info_read(m_CURLMHandle, &MsgLeft);
	if (pMsg)
	{
		if (pMsg->msg == CURLMSG_DONE)
		{
			HTTP_REQUEST_INFO * pRequestInfo = NULL;
			int Result = curl_easy_getinfo(pMsg->easy_handle, CURLINFO_PRIVATE, &pRequestInfo);
			if (Result == CURLE_OK)
			{
				long ResponseCode = 0;
				Result = curl_easy_getinfo(pMsg->easy_handle, CURLINFO_RESPONSE_CODE, &ResponseCode);
				if (Result == CURLE_OK)
				{
					if (pRequestInfo)
					{
						ProcessRequestEnd(pRequestInfo, pMsg->data.result, ResponseCode);
						ProcessCount++;
					}
				}
			}
		}
	}
	return ProcessCount;
}