/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerConnection.cpp                                     */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"


CServerConnection::CServerConnection()
{
	m_pView = NULL;
	m_ServerPort = 0;
	m_KeepAliveCount = 0;
	m_IsLogined = false;
	//m_pDataLogFile = NULL;
	m_hTreeItem = NULL;

	CServerManagerAckMsgHandler::InitMsgMap(m_MsgFnMap);
}

CServerConnection::~CServerConnection(void)
{
	//SAFE_RELEASE(m_pDataLogFile);
}

bool CServerConnection::Init(CServerManagerClientView * pView, CNetServer * pServer, LPCTSTR szName, LPCTSTR szGroup, LPCTSTR szServerAddress, UINT ServerPort, LPCTSTR UserName, LPCTSTR Password)
{
	m_pView = pView;
	SetServer(pServer);
	m_AssembleBuffer.Create(MAX_MSG_SIZE*MAX_DOWNLOAD_QUERY + NET_DATA_BLOCK_SIZE);
	m_SendBuffer.Create(MAX_MSG_SIZE + NET_DATA_BLOCK_SIZE);
	m_Name = szName;
	m_Group = szGroup;
	m_ServerAddress = szServerAddress;
	m_ServerPort = ServerPort;
	m_UserName = UserName;
	m_Password = Password;
	m_ConnectTimer.SaveTime(CEasyTimer::GetTime() + GetRand(-CONNECT_WAIT_TIME, CONNECT_WAIT_TIME));
	m_ServiceInfoFetchTimer.SaveTime(CEasyTimer::GetTime() + GetRand(-CONNECT_WAIT_TIME, CONNECT_WAIT_TIME));
	m_KeepAliveTimer.SaveTime();
	m_KeepAliveCount = 0;
	m_IsLogined = false;
	m_TaskQueue.Init(pView, this);
	m_PacketBuffer.Create(655360);
	return true;
}

bool CServerConnection::Reconnection(LPCTSTR szName, LPCTSTR szGroup, LPCTSTR szServerAddress, UINT ServerPort, LPCTSTR UserName, LPCTSTR Password)
{
	m_Name = szName;
	m_Group = szGroup;
	m_ServerAddress = szServerAddress;
	m_ServerPort = ServerPort;
	m_UserName = UserName;
	m_Password = Password;
	Disconnect();
	return true;
}

void CServerConnection::OnConnection(bool IsSucceed)
{
	if(IsSucceed)
	{
		PrintLog(0, _T("%s:%u已连接"),
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
		m_AssembleBuffer.SetUsedSize(0);
		m_KeepAliveTimer.SaveTime();
		m_KeepAliveCount = 0;
		m_IsLogined = false;

		if (m_pView)
		{
			m_pView->ConnectNotify(this);
		}

		CServerManagerMsgCaller MsgCaller(this);

		MsgCaller.Login(m_UserName, m_Password);
		PrintLog(0, _T("以用户[%s]登录"), (LPCTSTR)m_UserName);

		//SAFE_RELEASE(m_pDataLogFile);
		//m_pDataLogFile = CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
		//if (m_pDataLogFile)
		//{
		//	m_pDataLogFile->Open(CFileTools::MakeModuleFullPath(NULL, _T("DataLog.bin")), IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite | IFileAccessor::osWriteThrough);
		//}
	}
	else
	{
		PrintLog(-1, _T("%s:%u连接失败"),
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
	}
}

void CServerConnection::OnDisconnection()
{
	PrintLog(-1, _T("%s已断开"), (LPCTSTR)m_ServerAddress);

	m_TaskQueue.DeleteAllTask();
	//SAFE_RELEASE(m_pDataLogFile);

	if(m_pView)
	{
		m_pView->DisconnectNotify(this);
	}
}

void CServerConnection::OnRecvData(const BYTE * pData, UINT DataSize)
{
	//PrintImportantLog(_T("OnRecvData=%u"), DataSize);
	//if (m_pDataLogFile)
	//{
	//	m_pDataLogFile->Write(pData, DataSize);
	//}

	if (!m_AssembleBuffer.PushBack(pData, DataSize))
	{
		PrintLog(-1, _T("装配缓冲已满!"));
		Disconnect();
		return;
	}
	while (m_AssembleBuffer.GetUsedSize() >= CMessage::GetMsgHeaderLength())
	{
		CMessage * pMsg = (CMessage *)m_AssembleBuffer.GetBuffer();
		if (m_AssembleBuffer.GetUsedSize() >= pMsg->GetMsgLength())
		{
			if (pMsg->GetMsgFlag()&MESSAGE_FLAG_SYSTEM_MESSAGE)
				OnSystemMsg(pMsg);
			else
				OnMsg(pMsg);
			m_AssembleBuffer.PopFront(NULL, pMsg->GetMsgLength());
		}
		else
		{
			break;
		}
	}
}

int CServerConnection::Update(int ProcessPacketLimit)
{
	int ProcessCount=CNetConnection::Update(ProcessPacketLimit);
	if(IsDisconnected())
	{
		if(m_ConnectTimer.IsTimeOut(CONNECT_WAIT_TIME))
		{
			m_ConnectTimer.SaveTime();
			Destory();

			CIPAddress Address((LPCTSTR)m_ServerAddress,m_ServerPort);
			Connect(Address,CONNECT_TIME);
			PrintLog(0, _T("尝试连接%s:%u"), (LPCTSTR)m_ServerAddress, m_ServerPort);
		}
	}

	if(IsConnected())
	{
		if(m_ServiceInfoFetchTimer.IsTimeOut(SERVICE_INFO_FETCH_TIME))
		{
			m_ServiceInfoFetchTimer.SaveTime();
			QueryServiceList();
			QueryNetAdapterInfo();
		}

		if (m_KeepAliveTimer.IsTimeOut(KEEP_ALIVE_TIME))
		{
			m_KeepAliveTimer.SaveTime();
			m_KeepAliveCount++;
			CMessage KeepAliveMsg;
			KeepAliveMsg.Init();
			KeepAliveMsg.SetMsgID(SM_PROXY_KEEP_ALIVE_PING);
			KeepAliveMsg.SetMsgFlag(MESSAGE_FLAG_SYSTEM_MESSAGE);
			KeepAliveMsg.SetDataLength(0);
			DoSend(&KeepAliveMsg, KeepAliveMsg.GetMsgLength());
			
			if (m_KeepAliveCount > MAX_KEEP_ALIVE_COUNT)
			{
				PrintLog(-1, _T("连接超时!"));
				Disconnect();
			}
		}
	}
	m_TaskQueue.Update();
	return ProcessCount;
}

void CServerConnection::PrintLog(short Result, LPCTSTR szFormat, ...)
{
	if(m_pView)
	{
		va_list vl;

		va_start(vl,szFormat);
		m_pView->PrintLogVL((Result == MSG_RESULT_SUCCEED ? LOG_TYPE_NORMAL : LOG_TYPE_ERROR), szFormat, vl);
		va_end(vl);
	}
}


void CServerConnection::OnMsg(CMessage * pMsg)
{
	MSG_HANDLE_INFO * pHandleInfo = m_MsgFnMap.Find(pMsg->GetMsgID());
	if (pHandleInfo)
	{
		((pHandleInfo->pObject)->*(pHandleInfo->pFN))(pMsg->GetDataPacket());
	}
	else
	{
		PrintLog(-1, _T("无法找到消息[0x%X]的处理函数"), pMsg->GetMsgID());
	}
}
void CServerConnection::OnSystemMsg(CMessage * pMsg)
{
	if (pMsg->GetMsgID() == SM_PROXY_KEEP_ALIVE_PONG)
	{		
		m_KeepAliveCount = 0;
	}
}

CMessage * CServerConnection::NewMessage(UINT MsgDataSize)
{
	if (MsgDataSize + CMessage::GetMsgHeaderLength() < m_SendBuffer.GetBufferSize())
	{
		CMessage * pMsg = (CMessage *)m_SendBuffer.GetBuffer();
		pMsg->Init();
		return pMsg;
	}
	return NULL;
}
bool CServerConnection::ReleaseMessage(CMessage * pMsg)
{
	return true;
}
bool CServerConnection::SendMessage(CMessage * pMsg)
{
	return DoSend(pMsg, pMsg->GetMsgLength());
}

bool CServerConnection::DoSend(LPCVOID pData, UINT Size)
{
	//PrintImportantLog(_T("DoSend=%u"), Size);
	//if (m_pDataLogFile)
	//{
	//	m_pDataLogFile->Write(pData, Size);
	//}


	if (Send(pData, Size))
		return true;
	return false;
}





void CServerConnection::QueryServiceList()
{
	CServerManagerMsgCaller MsgCaller(this);
	MsgCaller.GetServiceList();
}

void CServerConnection::QueryStartupService(UINT ServiceID)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求启动服务器[%s]上的服务[%u]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID);
	MsgCaller.ServiceStartup(ServiceID);
}
void CServerConnection::QueryShutDownService(UINT ServiceID, BYTE ShutDownType)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求关闭服务器[%s]上的服务[%u]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID);
	MsgCaller.ServiceShutdown(ServiceID, ShutDownType);
}
void CServerConnection::QueryBrowseWorkDir(UINT ServiceID, LPCTSTR Dir)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求浏览服务器[%s]上的服务[%u]的目录[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,Dir);
	MsgCaller.BrowseServiceDir(ServiceID, Dir, 0, DIR_BROWSE_PAGE_LEN);
}

void CServerConnection::QueryStartDownload(UINT ServiceID,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]下载文件[%s]到[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		SourceFilePath,TargetFilePath);

	MsgCaller.FileDownloadStart(ServiceID, SourceFilePath, 0);
}

void CServerConnection::QueryDownloadData()
{
	CServerManagerMsgCaller MsgCaller(this);
	//LogDebug("请求下载数据");
	MsgCaller.FileDownloadData();
}

void CServerConnection::QueryDownloadFinish()
{
	CServerManagerMsgCaller MsgCaller(this);
	LogDebug("请求完成下载");
	MsgCaller.FileDownloadFinish();
}

void CServerConnection::QueryStartUpload(UINT ServiceID,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath, UINT FileLastWriteTime)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]上传文件[%s]到[%s],文件时间%u"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		SourceFilePath,TargetFilePath, FileLastWriteTime);

	MsgCaller.FileUploadStart(ServiceID, TargetFilePath, FileLastWriteTime);
}

void CServerConnection::QueryUploadData(UINT Length, const CEasyBuffer& FileData, bool IsLast)
{
	CServerManagerMsgCaller MsgCaller(this);

	//PrintLog(0, _T("Upload%u(%u)@%llu"),
	//	FileData.GetUsedSize(), Length, Offset);
	//LogDebug("请求上传数据");
	MsgCaller.FileUploadData(Length, FileData, IsLast);
}

void CServerConnection::QueryUploadFinish()
{
	CServerManagerMsgCaller MsgCaller(this);
	LogDebug("请求完成上传");
	MsgCaller.FileUploadFinish();
}

void CServerConnection::QueryCreateDir(UINT ServiceID,LPCTSTR Dir)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]创建目录[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		Dir);

	MsgCaller.CreateDir(ServiceID, Dir);
}

void CServerConnection::QueryDeleteFile(UINT ServiceID,LPCTSTR FilePath)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]删除文件[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath);

	MsgCaller.DeleteFile(ServiceID, FilePath, true);
}
void CServerConnection::QueryChangeFileMode(UINT ServiceID, LPCTSTR FilePath, UINT Mode)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]文件[%s]更改属性为[0x%X]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath, Mode);

	MsgCaller.ChangeFileMode(ServiceID, FilePath, Mode);
}
void CServerConnection::QueryCreateProcess(UINT ServiceID,LPCTSTR FilePath,LPCTSTR WorkDir,LPCTSTR Param)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]运行程序[%s %s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath, Param);

	MsgCaller.RunProgram(ServiceID, FilePath, WorkDir, Param);	
}


void CServerConnection::QueryNetAdapterInfo()
{
	CServerManagerMsgCaller MsgCaller(this);
	MsgCaller.GetNetAdapterList();
}

void CServerConnection::QueryRunScript(UINT ServiceID,LPCTSTR FilePath)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]运行脚本[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath);

	MsgCaller.ExecuteScript(ServiceID, FilePath, true);	
}
void CServerConnection::QueryAddService(const CServiceInfo& ServiceInfo)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求添加服务[%s]"),
		(LPCTSTR)ServiceInfo.GetName());
	m_PacketBuffer.Clear();
	ServiceInfo.MakePacket(m_PacketBuffer, DOMF_SERVICE_INFO_FOR_EDIT);
	MsgCaller.AddService(m_PacketBuffer);
}
void CServerConnection::QueryEditService(const CServiceInfo& ServiceInfo)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求编辑服务[%s]"),
		(LPCTSTR)ServiceInfo.GetName());
	m_PacketBuffer.Clear();
	ServiceInfo.MakePacket(m_PacketBuffer, DOMF_SERVICE_INFO_FOR_EDIT);
	MsgCaller.EditService(m_PacketBuffer);
}
void CServerConnection::QueryDelService(UINT ServiceID)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求删除服务[%u]"),
		ServiceID);

	MsgCaller.DeleteService(ServiceID);
}

void CServerConnection::QueryProcessList(short Page, short PageLen)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("以%d的页长请求第%d页的进程列表"),
		PageLen, Page);

	MsgCaller.GetProcessList(Page, PageLen);
}

void CServerConnection::QuerySendCommand(UINT ServiceID, LPCTSTR szCommand)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]执行命令[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		szCommand);

	MsgCaller.SendCommand(ServiceID, szCommand);
}
void CServerConnection::QueryAllServerStatus(UINT ServiceID)
{
	CServerManagerMsgCaller MsgCaller(this);

	MsgCaller.GetAllServerStatus(ServiceID);
}
void CServerConnection::QueryServerStatusFormat(UINT ServiceID)
{
	CServerManagerMsgCaller MsgCaller(this);

	MsgCaller.GetServerStatusFormat(ServiceID);
}
void CServerConnection::QueryEnableLogRecv(UINT ServiceID, bool Enable)
{
	CServerManagerMsgCaller MsgCaller(this);

	MsgCaller.EnableLogRecv(ServiceID, Enable);
}
void CServerConnection::QueryServiceInfo(UINT ServiceID)
{
	CServerManagerMsgCaller MsgCaller(this);

	MsgCaller.GetServiceInfo(ServiceID);
}
void CServerConnection::QueryFileCompare(UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize, LPCTSTR FileMD5)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(0, _T("请求服务器[%s]上的服务[%u]对比文件[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath);
	MsgCaller.FileCompare(ServiceID, FilePath, FileSize, FileMD5);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CServerConnection::LoginAck(short Result)
{
	PrintLog(Result,_T("登录返回:%s"), GetResultStr(Result));
	if (Result == MSG_RESULT_SUCCEED)
	{
		m_IsLogined = true;
		//QueryProcessList(0, 10);
	}
	else
	{
		Disconnect();
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::GetServiceListAck(short Result, const CSmartStruct& ServiceListData)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		m_ServiceList.ParsePacket(ServiceListData,DOMF_SERVICE_INFO_LIST_FULL);
		for (UINT i = 0; i < m_ServiceList.GetList().GetCount(); i++)
		{
			CServiceInfo& ServiceInfo = m_ServiceList.GetList()[i];
			m_TaskQueue.OnServiceInfo(ServiceInfo);
			if (m_pView)
			{
				m_pView->SetServiceInfo(this, ServiceInfo);
			}
		}
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::GetProcessListAck(short Result, short Page, short PageLen, short TotalPage, const CSmartStruct& ProcessListData)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		CProcessInfoList ProcessList;
		ProcessList.ParsePacket(ProcessListData, DOMF_PROCESS_INFO_LIST_FULL);
		//Page++;
		//if (Page < TotalPage)
		//{
		//	QueryProcessList(Page, PageLen);
		//}
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::GetNetAdapterListAck(short Result, const CSmartStruct& NetAdapterListData)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		NET_ADAPTER_INFO_LIST InfoList;
		InfoList.ParsePacket(NetAdapterListData);
		for (UINT i = 0; i < InfoList.List.GetCount(); i++)
		{
			NET_ADAPTER_INFO& Info = InfoList.List[i];

			if (Info.IPAddressList.GetCount())
			{
				CIPAddress ServiceIP(m_ServerAddress, m_ServerPort);

				CIPAddress IPAddress(Info.IPAddressList[0], m_ServerPort);
				if (m_pView)
				{
					if (IPAddress == ServiceIP)
					{
						m_pView->SetInternetAdapterInfo(GetID(), Info.SendFlux, Info.RecvFlux);
					}
					else
					{
						m_pView->SetIntranetAdapterInfo(GetID(), Info.SendFlux, Info.RecvFlux);
					}
				}
			}
		}
	}
		
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::GetServiceInfoAck(short Result, const CSmartStruct& ServiceInfoPacket)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		if (m_pView)
		{
			CServiceInfo ServiceInfo;
			ServiceInfo.ParsePacket(ServiceInfoPacket, DOMF_SERVICE_INFO_FULL);
			if (m_pView->GetServiceEditor()->Init(this, &ServiceInfo))
			{
				m_pView->GetServiceEditor()->ShowWindow(SW_SHOW);
			}
		}
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ServiceStartupAck(short Result, UINT ServiceID)
{
	PrintLog(Result, _T("启动服务器[%s]上的服务[%u],结果[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		GetResultStr(Result));
	m_TaskQueue.OnServiceStartupResult(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ServiceShutdownAck(short Result, UINT ServiceID)
{
	PrintLog(Result, _T("关闭服务器[%s]上的服务[%u],结果[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		GetResultStr(Result));
	m_TaskQueue.OnServiceShutdownResult(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::RunProgramAck(short Result)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		PrintLog(Result, _T("执行程序成功"));
	}
	else
	{
		PrintLog(Result, _T("执行程序失败"));
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ProcessShutdownAck(short Result, UINT ProcessID)
{
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ExecuteScriptAck(short Result, UINT ServiceID, int ErrorCode, int LastLine)
{
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::BrowseServiceDirAck(short Result, UINT ServiceID, const CEasyString& Dir, short Page, short PageLen, short TotalPage, const CSmartStruct& FileListData)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		FILE_INFO_LIST FileList;
		FileList.ParsePacket(FileListData);
		for (UINT i = 0; i < FileList.List.GetCount(); i++)
		{
			FILE_INFO& Info = FileList.List[i];
			if (m_pView)
			{
				m_pView->GetWorkDirBrowser()->OnBrowseResult(Dir, Info.Name, Info.Attribute, Info.Size,
					Info.CreateTime, Info.LastAccessTime, Info.LastWriteTime);
			}
		}
		if (Page < TotalPage - 1)
		{
			Page++;
			CServerManagerMsgCaller MsgCaller(this);			
			MsgCaller.BrowseServiceDir(ServiceID, Dir, Page, DIR_BROWSE_PAGE_LEN);
		}
		else
		{
			m_pView->GetWorkDirBrowser()->OnBrowseFinish();
		}
	}

	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileDownloadStartAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT64 FileSize, UINT FileLastWriteTime)
{
	m_TaskQueue.OnStartDownloadResult(Result, ServiceID, FilePath, FileSize, FileLastWriteTime);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileDownloadDataAck(short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData, bool IsLast)
{
	m_TaskQueue.OnDownloadData(Result, Offset, Length, FileData, IsLast);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileDownloadFinishAck(short Result, const CEasyString& MD5)
{
	m_TaskQueue.OnDownloadFinish(Result, MD5);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileUploadStartAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT64 FileSize)
{
	m_TaskQueue.OnUploadStartResult(Result, ServiceID, FilePath, FileSize);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileUploadDataAck(short Result, UINT Length, bool IsLast)
{
	m_TaskQueue.OnUploadData(Result, Length, IsLast);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileUploadFinishAck(short Result, const CEasyString& MD5)
{
	m_TaskQueue.OnUploadFinish(Result, MD5);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::CreateDirAck(short Result, UINT ServiceID, const CEasyString& Dir)
{
	PrintLog(Result, _T("新建目录结果%s[%s]"), GetResultStr(Result), (LPCTSTR)Dir);
	m_TaskQueue.OnCreateDirResult(Result, ServiceID, Dir);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::DeleteFileAck(short Result, UINT ServiceID, const CEasyString& FilePath)
{
	PrintLog(Result, _T("删除文件结果%s[%s]"), GetResultStr(Result), (LPCTSTR)FilePath);
	m_TaskQueue.OnDeleteFileResult(Result, ServiceID, FilePath);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ChangeFileModeAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT Mode)
{
	PrintLog(Result, _T("修改文件属性结果%s[%s]"), GetResultStr(Result), (LPCTSTR)FilePath);
	m_TaskQueue.OnChangeFileModeResult(Result, ServiceID, FilePath, Mode);
	return COMMON_RESULT_SUCCEED;
}

int CServerConnection::AddServiceAck(short Result)
{
	PrintLog(Result, _T("添加服务结果%s"), GetResultStr(Result));
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::EditServiceAck(short Result)
{
	PrintLog(Result, _T("编辑服务结果%s"), GetResultStr(Result));
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::DeleteServiceAck(short Result, UINT ServiceID)
{
	PrintLog(Result, _T("删除服务结果%s"), GetResultStr(Result));
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::SendCommandAck(short Result, UINT ServiceID)
{
	PrintLog(Result, _T("向[%u]发送命令结果%s"), ServiceID, GetResultStr(Result));
	m_TaskQueue.OnSendCommandResult(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::EnableLogRecvAck(short Result, UINT ServiceID, bool Enable)
{
	PrintLog(Result, _T("向[%u]请求%s日志结果%s"), ServiceID, Enable ? _T("连接") : _T("取消连接"), GetResultStr(Result));
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ConsoleLogNotify(UINT ServiceID, const CEasyString& LogMsg)
{
	m_pView->GetServerConsole()->OnLogMsg(GetID(), ServiceID, LogMsg);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::GetServerStatusAck(short Result, UINT ServiceID, const CSmartStruct& StatusListPacket)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		m_pView->GetDlgServerStatus()->FlushStatus(GetID(), ServiceID, StatusListPacket);
	}
	else
	{
		PrintLog(Result, _T("向[%u]请求服务器状态失败%s"), ServiceID, GetResultStr(Result));
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::GetServerStatusFormatAck(short Result, UINT ServiceID, const CSmartStruct& StatusFormatPacket)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		m_pView->GetDlgServerStatus()->SetStatusFormat(GetID(), ServiceID, StatusFormatPacket);
	}
	else
	{
		PrintLog(Result, _T("向[%u]请求服务器状态格式失败%s"), ServiceID, GetResultStr(Result));
	}
	return COMMON_RESULT_SUCCEED;
}

int CServerConnection::FileCompareAck(short Result, UINT ServiceID, const CEasyString& FilePath)
{
	PrintLog(Result, _T("比较文件结果%s[%s]"), GetResultStr(Result), (LPCTSTR)FilePath);
	m_TaskQueue.OnFileCompareResult(Result, ServiceID, FilePath);
	return COMMON_RESULT_SUCCEED;
}