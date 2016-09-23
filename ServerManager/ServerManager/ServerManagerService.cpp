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
	CMainConfig::GetInstance()->LoadServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList.List);
	
	for (UINT i = 0; i<m_ServiceInfoList.List.GetCount(); i++)
	{
		m_ServiceInfoList.List[i].Status = SERVICE_STATUS_NONE;
		m_ServiceInfoList.List[i].LastStatusChangeTime = time(NULL);
	}

#ifdef WIN32
	FetchWinServiceInfo();
#endif

	m_ProcessInfoFetchTimer.SaveTime();

	//m_RecentTime = CEasyTimerEx::GetTime();

	m_ServiceKeepTimer.SaveTime();

	InitNetAdapterInfo();

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerService::Destory()
{
	FUNCTION_BEGIN;
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

	if (m_ServiceKeepTimer.IsTimeOut(SERVICE_KEEP_TIME))
	{
		m_ServiceKeepTimer.SaveTime();
		UINT CurTime = time(NULL);
		for (UINT i = 0; i < m_ServiceInfoList.List.GetCount(); i++)
		{
			SERVICE_INFO& SeriveInfo = m_ServiceInfoList.List[i];
			if (SeriveInfo.Type == SERVICE_TYPE_NORMAL && 
				SeriveInfo.Status == SERVICE_STATUS_STOP && SeriveInfo.LastOperation == SERVICE_OPERATION_STARTUP &&
				CurTime - SeriveInfo.LastStatusChangeTime >= SeriveInfo.RestartupTime)
			{
				Log("检测到服务[%u](%s)已停止，重新启动", SeriveInfo.ServiceID, (LPCTSTR)SeriveInfo.Name);
				StartupService(SeriveInfo.ServiceID);
			}
		}
	}
	return Process;
	FUNCTION_END;
	return 0;
}

SERVICE_INFO * CServerManagerService::GetServiceInfo(UINT ServiceID)
{
	for (UINT i = 0; i < m_ServiceInfoList.List.GetCount(); i++)
	{
		if (m_ServiceInfoList.List[i].ServiceID == ServiceID)
			return m_ServiceInfoList.List.GetObject(i);
	}
	return NULL;
}

int CServerManagerService::StartupService(UINT ServiceID)
{
	SERVICE_INFO * pServiceInfo = GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->Type == SERVICE_TYPE_NORMAL)
		{
			if (pServiceInfo->Status == SERVICE_STATUS_STOP)
			{
				if (StartupProcess(pServiceInfo->ImageFilePath, pServiceInfo->WorkDir, pServiceInfo->StartupParam))
				{
					Log("服务%u成功执行启动", ServiceID);
					pServiceInfo->LastOperation = SERVICE_OPERATION_STARTUP;
					pServiceInfo->Status = SERVICE_STATUS_STARTUPPING;
					pServiceInfo->LastStatusChangeTime = time(NULL);
					return MSG_RESULT_SUCCEED;
				}
				else
				{
					Log("服务%u执行启动失败", ServiceID);
				}
			}
			else
			{
				Log("服务%u状态错误%d", ServiceID, pServiceInfo->Status);
			}
		}
		else
		{
			if (StartupWinService(pServiceInfo->Name))
			{
				Log("Win服务%u成功执行启动", ServiceID);
				pServiceInfo->LastOperation = SERVICE_OPERATION_STARTUP;
				pServiceInfo->Status = SERVICE_STATUS_STARTUPPING;
				pServiceInfo->LastStatusChangeTime = time(NULL);
				return MSG_RESULT_SUCCEED;
			}
			else
			{
				Log("服务%u执行启动失败", ServiceID);
			}
		}
		
		return COMMON_RESULT_FAILED;
	}
	else
	{
		Log("服务%u不存在", ServiceID);
		return MSG_RESULT_SERVICE_NOT_EXIST;
	}
}

int CServerManagerService::ShutdownService(UINT ServiceID, bool IsForce)
{
	SERVICE_INFO * pServiceInfo = GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->Type == SERVICE_TYPE_NORMAL)
		{
			if (pServiceInfo->Status == SERVICE_STATUS_RUNNING)
			{
				if (ShutdownProcess(pServiceInfo->ProcessID, IsForce))
				{
					Log("服务%u成功执行%s关闭", ServiceID, IsForce ? "强制" : "");
					pServiceInfo->LastOperation = SERVICE_OPERATION_SHUTDOWN;
					pServiceInfo->Status = SERVICE_STATUS_SHUTDOWNNING;
					pServiceInfo->LastStatusChangeTime = time(NULL);
					return MSG_RESULT_SUCCEED;
				}
			}
		}
		else
		{
			if (ShutdownWinService(pServiceInfo->Name))
			{
				Log("Win服务%u成功执行关闭", ServiceID);
				pServiceInfo->LastOperation = SERVICE_OPERATION_SHUTDOWN;
				pServiceInfo->Status = SERVICE_STATUS_SHUTDOWNNING;
				pServiceInfo->LastStatusChangeTime = time(NULL);
				return MSG_RESULT_SUCCEED;
			}

		}
		Log("服务%u执行%s关闭失败", ServiceID, IsForce ? "强制" : "");
		return COMMON_RESULT_FAILED;
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

	m_ProcessInfoList.List.Resize(ProcessList.GetCount());
	for (UINT i = 0; i < ProcessList.GetCount(); i++)
	{
		m_ProcessInfoList.List[i].ProcessID = ProcessList[i].ProcessID;
		m_ProcessInfoList.List[i].ImageFile = ProcessList[i].ImageFile;
		m_ProcessInfoList.List[i].CPUUsedTime = ProcessList[i].CPUUsedTime;
		m_ProcessInfoList.List[i].CPUUsed = ProcessList[i].CPUUsedRate;
		m_ProcessInfoList.List[i].MemoryUsed = ProcessList[i].MemoryUsed;
		m_ProcessInfoList.List[i].VirtualMemoryUsed = ProcessList[i].VirtualMemoryUsed;
	}

	for(UINT j=0;j<m_ServiceInfoList.List.GetCount();j++)
	{
		SERVICE_INFO& ServiceInfo = m_ServiceInfoList.List[j];
		ServiceInfo.ProcessID = 0;
		ServiceInfo.CPUUsed=0;
		ServiceInfo.MemoryUsed=0;
		ServiceInfo.VirtualMemoryUsed=0;
		for(UINT i=0;i<m_ProcessInfoList.List.GetCount();i++)
		{
			PROCESS_INFO& ProcessInfo = m_ProcessInfoList.List[i];
			if(ServiceInfo.ImageFilePath[0])
			{
				if (ServiceInfo.ImageFilePath.CompareNoCase(ProcessInfo.ImageFile) == 0)
				{
					ServiceInfo.ProcessID=ProcessInfo.ProcessID;
					ServiceInfo.CPUUsed=ProcessInfo.CPUUsed;
					ServiceInfo.MemoryUsed=ProcessInfo.MemoryUsed;
					ServiceInfo.VirtualMemoryUsed=ProcessInfo.VirtualMemoryUsed;
				}
			}
		}
		switch (ServiceInfo.Status)
		{
		case SERVICE_STATUS_NONE:
			if (ServiceInfo.ProcessID == 0)
			{
				ServiceInfo.Status = SERVICE_STATUS_STOP;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			else
			{
				ServiceInfo.Status = SERVICE_STATUS_RUNNING;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			break;
		case SERVICE_STATUS_STOP:
			if (ServiceInfo.ProcessID != 0)
			{
				ServiceInfo.Status = SERVICE_STATUS_RUNNING;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			break;
		case SERVICE_STATUS_RUNNING:
			if (ServiceInfo.ProcessID == 0)
			{
				ServiceInfo.Status = SERVICE_STATUS_STOP;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			break;
		case SERVICE_STATUS_SHUTDOWNNING:
			if (ServiceInfo.ProcessID == 0)
			{
				ServiceInfo.Status = SERVICE_STATUS_STOP;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			else if (CurTime - ServiceInfo.LastStatusChangeTime > MAX_OPERATION_WAIT_TIME)
			{
				ServiceInfo.Status = SERVICE_STATUS_RUNNING;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			break;
		case SERVICE_STATUS_STARTUPPING:
			if (ServiceInfo.ProcessID != 0)
			{
				ServiceInfo.Status = SERVICE_STATUS_RUNNING;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			else if (CurTime - ServiceInfo.LastStatusChangeTime > MAX_OPERATION_WAIT_TIME)
			{
				ServiceInfo.Status = SERVICE_STATUS_STOP;
				ServiceInfo.LastStatusChangeTime = time(NULL);
			}
			break;
		}		

		CFileInfo FileInfo(ServiceInfo.ImageFilePath);

		if (FileInfo.IsOK())
		{
			ServiceInfo.ImageFileTime = FileInfo.GetLastWriteTime();
		}
	}

}

#ifdef WIN32
void CServerManagerService::FetchWinServiceInfo()
{
	for(UINT i=0;i<m_ServiceInfoList.List.GetCount();i++)
	{
		SERVICE_INFO& ServiceInfo = m_ServiceInfoList.List[i];

		if(ServiceInfo.Type==SERVICE_TYPE_WIN_SERVICE)
		{
			CWinServiceController ServiceController;
			if(ServiceController.OpenService(ServiceInfo.Name))
			{
				TCHAR Path[MAX_PATH];
				ServiceController.GetServiceImageFilePath(Path, MAX_PATH);
				ServiceInfo.ImageFilePath = Path;
				ServiceInfo.WorkDir = CFileTools::GetPathDirectory(Path);
				ServiceInfo.WorkDir.TrimRight(DIR_SLASH);
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

int CServerManagerService::ShutdownProcess(UINT ProcessID, bool IsForce)
{
#ifdef WIN32
	if (IsForce)
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
		if (hProcess != NULL)
		{
			bool Ret = TerminateProcess(hProcess, 0) != FALSE;
			CloseHandle(hProcess);
			Log("%s终止进程%u%s", IsForce ? "强制" : "", ProcessID, Ret ? "成功" : "失败");
			return Ret;
		}
		else
		{
			Log("无法打开进程%u", ProcessID);
		}
	}
	else
	{
		return PostQuitMsgToProcess(ProcessID);
	}
	return false;
#else
	int sig = SIGTERM;
	if (IsForce)
	{
		sig = SIGKILL;
	}
	if(kill(ProcessID, sig) == 0)
	{
		Log("以信号%d终止进程%u成功", sig, ProcessID);
		return true;
	}
	else
	{
		Log("以信号%d终止进程%u失败%d", sig, ProcessID, errno);
		return false;
	}
#endif
}

int CServerManagerService::CheckServiceInfo(const SERVICE_INFO& ServiceInfo)
{
#ifndef WIN32
	if (ServiceInfo.Type == SERVICE_TYPE_WIN_SERVICE)
		return MSG_RESULT_SERVICE_TYPE_NOT_SUPPORT;
#endif
	if (ServiceInfo.Type == SERVICE_TYPE_NORMAL || ServiceInfo.Type == SERVICE_TYPE_WIN_SERVICE)
	{
		if (!CFileTools::IsFileExist(ServiceInfo.ImageFilePath))
			return MSG_RESULT_FILE_NOT_EXIST;
	}

	if (!CFileTools::IsDirExist(ServiceInfo.WorkDir))
		return MSG_RESULT_DIR_NOT_EXIST;

	return MSG_RESULT_SUCCEED;
}
int CServerManagerService::AddService(const SERVICE_INFO& ServiceInfo)
{
	int Result = CheckServiceInfo(ServiceInfo);
	if (Result != MSG_RESULT_SUCCEED)
		return Result;
	
	for (UINT i = 0; i < m_ServiceInfoList.List.GetCount(); i++)
	{
		if (m_ServiceInfoList.List[i].ServiceID == ServiceInfo.ServiceID)
			return MSG_RESULT_SERVICE_ID_IS_USED;
	}
	m_ServiceInfoList.List.Add(ServiceInfo);

	SERVICE_INFO& SrvInfo = m_ServiceInfoList.List[m_ServiceInfoList.List.GetCount() - 1];
	SrvInfo.ImageFilePath = CFileTools::MakeFullPath(ServiceInfo.ImageFilePath);
	SrvInfo.WorkDir.Trim();
	if (SrvInfo.WorkDir.IsEmpty() && SrvInfo.Type != SERVICE_TYPE_DIRECTORY)
	{
		SrvInfo.WorkDir = CFileTools::GetPathDirectory(SrvInfo.ImageFilePath);
	}
	else
	{
		SrvInfo.WorkDir = CFileTools::MakeFullPath(SrvInfo.WorkDir);
	}
	SrvInfo.WorkDir.TrimRight(DIR_SLASH);

	CMainConfig::GetInstance()->SaveServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList.List);
	return MSG_RESULT_SUCCEED;
}
int CServerManagerService::EditService(const SERVICE_INFO& ServiceInfo)
{
	int Result = CheckServiceInfo(ServiceInfo);
	if (Result != MSG_RESULT_SUCCEED)
		return Result;

	for (UINT i = 0; i < m_ServiceInfoList.List.GetCount(); i++)
	{
		if (m_ServiceInfoList.List[i].ServiceID == ServiceInfo.ServiceID)
		{
			SERVICE_INFO& SrvInfo = m_ServiceInfoList.List[i];

			SrvInfo.Type = ServiceInfo.Type;
			SrvInfo.Name = ServiceInfo.Name;
			SrvInfo.ImageFilePath = CFileTools::MakeFullPath(ServiceInfo.ImageFilePath);
			SrvInfo.WorkDir = ServiceInfo.WorkDir;
			SrvInfo.StartupParam = ServiceInfo.StartupParam;
			SrvInfo.LastOperation = ServiceInfo.LastOperation;
			SrvInfo.RestartupTime = ServiceInfo.RestartupTime;
			
			SrvInfo.WorkDir.Trim();
			if (SrvInfo.WorkDir.IsEmpty() && SrvInfo.Type != SERVICE_TYPE_DIRECTORY)
			{
				SrvInfo.WorkDir = CFileTools::GetPathDirectory(SrvInfo.ImageFilePath);
			}
			else
			{
				SrvInfo.WorkDir = CFileTools::MakeFullPath(SrvInfo.WorkDir);
			}
			SrvInfo.WorkDir.TrimRight(DIR_SLASH);


			CMainConfig::GetInstance()->SaveServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList.List);
			return MSG_RESULT_SUCCEED;
		}
	}
	return MSG_RESULT_SERVICE_NOT_EXIST;
}
int CServerManagerService::DeleteService(UINT ServiceID)
{
	for (UINT i = 0; i < m_ServiceInfoList.List.GetCount(); i++)
	{
		if (m_ServiceInfoList.List[i].ServiceID == ServiceID)
		{
			m_ServiceInfoList.List.Delete(i);
			CMainConfig::GetInstance()->SaveServiceList(CFileTools::MakeModuleFullPath(NULL, SERVICE_LIST_FILE_NAME), m_ServiceInfoList.List);
			return MSG_RESULT_SUCCEED;
		}
	}
	return MSG_RESULT_SERVICE_NOT_EXIST;
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
