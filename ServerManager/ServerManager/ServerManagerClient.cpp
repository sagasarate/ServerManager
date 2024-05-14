/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerClient.cpp                                  */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"




IMPLEMENT_CLASS_INFO_STATIC(CServerManagerClient,CNetConnection);

CServerManagerClient::CServerManagerClient(void)
{
	FUNCTION_BEGIN;
	m_pManager=NULL;
	m_WantDelete=false;
	m_IsLogined = false;
	m_IsInScriptExecute=false;
	m_EnableLogRecv = false;
	CServerManagerMsgHandler::InitMsgMap(m_MsgFnMap);

	FUNCTION_END;
}

CServerManagerClient::~CServerManagerClient(void)
{
	FUNCTION_BEGIN;
	FUNCTION_END;
}


BOOL CServerManagerClient::Init(UINT ID,CServerManagerService * pManager)
{
	FUNCTION_BEGIN;
	m_pManager=pManager;
	SetID(ID);
	SetServer(m_pManager->GetServer());

	if (m_AssembleBuffer.GetBufferSize()<CMainConfig::GetInstance()->GetRecvBufferSize() * 2)
	{
		if (!m_AssembleBuffer.Create(CMainConfig::GetInstance()->GetRecvBufferSize() * 2))
		{
			Log("创建%u大小的装配缓冲失败", CMainConfig::GetInstance()->GetRecvBufferSize() * 2);
			return FALSE;
		}
	}

	if (m_SendBuffer.GetBufferSize() < CMainConfig::GetInstance()->GetSendBufferSize())
	{
		if (!m_SendBuffer.Create(CMainConfig::GetInstance()->GetSendBufferSize()))
		{
			Log("创建%u大小的发送缓冲失败", CMainConfig::GetInstance()->GetSendBufferSize());
			return FALSE;
		}
	}

	m_PacketBuffer1.Create(CMainConfig::GetInstance()->GetSendBufferSize() - 64);

	if(!m_ScriptExecutor.Init(0,this))
	{
		Log("初始化脚本执行器失败");
		return FALSE;
	}
	m_AssembleBuffer.SetUsedSize(0);
	m_WantDelete=false;
	m_IsLogined = false;
	m_IsInScriptExecute=false;
	m_EnableLogRecv = false;	
	m_KeepAliveTimer.SaveTime();

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerClient::Destory()
{
	FUNCTION_BEGIN;

	//SAFE_RELEASE(m_pCurDownloadFile);
	//SAFE_RELEASE(m_pCurUploadFile);
	//SAFE_RELEASE(m_pDataLogFile);
	CNetConnection::Destory();

	FUNCTION_END;
}

void CServerManagerClient::OnConnection(bool IsSucceed)
{
	FUNCTION_BEGIN;
	if(IsSucceed)
	{
		Log("客户端%s:%u建立连接",
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
		m_AssembleBuffer.SetUsedSize(0);
		m_KeepAliveTimer.SaveTime();
		m_KeepAliveCount = 0;

		//SAFE_RELEASE(m_pDataLogFile);
		//m_pDataLogFile = CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
		//if (m_pDataLogFile)
		//{
		//	m_pDataLogFile->Open(CFileTools::MakeModuleFullPath(NULL, "DataLog.bin"), IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite);
		//}
	}
	else
	{
		m_WantDelete=true;
	}
	FUNCTION_END;
}

void CServerManagerClient::OnDisconnection()
{
	FUNCTION_BEGIN;
	Log("客户端%s:%u断开连接",
		GetRemoteAddress().GetIPString(),
		GetRemoteAddress().GetPort());

	CFileTask* pTask = m_pManager->GetTaskManager().GetTask(GetID());
	if (pTask)
	{
		pTask->DumpFinishQueue();
		pTask->DumpParallelQueue();
	}

	m_pManager->GetTaskManager().CancelTask(GetID());
	m_WantDelete=true;
	if (m_EnableLogRecv)
	{

	}
	//SAFE_RELEASE(m_pDataLogFile);

	FUNCTION_END;
}

void CServerManagerClient::OnRecvData(const BYTE * pData, UINT DataSize)
{
	FUNCTION_BEGIN;

	//Log("OnRecvData=%u", DataSize);
	//if (m_pDataLogFile)
	//{
	//	m_pDataLogFile->Write(pData, DataSize);
	//}

	if (!m_AssembleBuffer.PushBack(pData, DataSize))
	{
		Log("装配缓冲已满!");
		Disconnect();
		return;
	}
	int ProcessCount = 0;
	while (m_AssembleBuffer.GetUsedSize() >= CMessage::GetMsgHeaderLength())
	{
		CMessage * pMsg = (CMessage *)m_AssembleBuffer.GetBuffer();
		if (pMsg->GetMsgLength() < CMessage::GetMsgHeaderLength())
		{
			Log("消息包头异常");
			Disconnect();
			return;
		}
		if (m_AssembleBuffer.GetUsedSize() >= pMsg->GetMsgLength())
		{
			if (pMsg->GetMsgFlag()& DOS_MESSAGE_FLAG_SYSTEM_MESSAGE)
				OnSystemMsg(pMsg);
			else
				OnMsg(pMsg);
			m_AssembleBuffer.PopFront(NULL, pMsg->GetMsgLength());
		}
		else
		{
			break;
		}
		ProcessCount++;
		if (ProcessCount >= 256)
		{
			Log("异常,单次已处理256个消息,退出循环");
			break;
		}
	}
	FUNCTION_END;
}

int CServerManagerClient::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int ProcessCount=CNetConnection::Update(ProcessPacketLimit);
	ProcessCount += m_ScriptExecutor.Update(ProcessPacketLimit);
	

	if (IsConnected())
	{
		if (m_KeepAliveTimer.IsTimeOut(CMainConfig::GetInstance()->GetKeepAliveTime()))
		{
			m_KeepAliveTimer.SaveTime();

			if(m_AssembleBuffer.GetUsedSize())
			{
				LogDebug("拼包缓冲使用量%u", m_AssembleBuffer.GetUsedSize());
				if (m_AssembleBuffer.GetUsedSize() >= CMessage::GetMsgHeaderLength())
				{
					CMessage* pMsg = (CMessage*)m_AssembleBuffer.GetBuffer();
					LogDebug("包头大小%u", pMsg->GetMsgLength());
				}
			}
			m_KeepAliveCount++;
			if (!m_IsLogined)
			{
				Log("超时未登录");
				Disconnect();
			}
			else if (m_KeepAliveCount > CMainConfig::GetInstance()->GetKeepAliveCount())
			{
				Log("KeepAlive超时");
				Disconnect();
			}
		}
		
		CFileTask* pTask = m_pManager->GetTaskManager().GetTask(GetID());
		if (pTask)
		{
			switch (pTask->GetType())
			{
			case TASK_TYPE_READ:
				if (pTask->GetStatus() == TASK_STATUS_ERROR)
				{
					Log("文件读取出错");
					m_pManager->GetTaskManager().CancelTask(GetID());					
					CServerManagerAckMsgCaller MsgCaller(this);
					MsgCaller.FileDownloadDataAck(MSG_RESULT_FAILED, 0, 0, CEasyBuffer(), false);
				}
				else if(pTask->GetWorkingQueryCount())
				{
					FILE_DATA_BLOCK* pData = pTask->GetFinishOperation();
					if (pData)
					{
						CServerManagerAckMsgCaller MsgCaller(this);
						MsgCaller.FileDownloadDataAck(MSG_RESULT_SUCCEED, pData->Offset, pData->OriginSize, pData->DataBuffer, pData->IsLast);
						LogDebug("已发送文件块%u,Offset=%llu,Size=%u(%u)", pData->SerialNumber, pData->Offset, pData->OriginSize, pData->DataBuffer.GetUsedSize());
						m_pManager->GetTaskManager().DeleteFileDataInfo(pData);
					}
				}
				break;
			case TASK_TYPE_MAKE_MD5:
				if (pTask->GetStatus() == TASK_STATUS_END)
				{
					CServerManagerAckMsgCaller MsgCaller(this);

					MsgCaller.FileCompareAck((pTask->GetFileMD5().CompareNoCase(pTask->GetCompareMD5()) == 0) ? MSG_RESULT_SUCCEED : MSG_RESULT_FAILED,
						pTask->GetServiceID(), pTask->GetFilePath());
					Log("MD5比较完毕");
					m_pManager->GetTaskManager().CancelTask(GetID());
				}
				else if (pTask->GetStatus() == TASK_STATUS_ERROR)
				{
					CServerManagerAckMsgCaller MsgCaller(this);

					MsgCaller.FileCompareAck(MSG_RESULT_FAILED, pTask->GetServiceID(), pTask->GetFilePath());
					Log("MD5比较出错");
					m_pManager->GetTaskManager().CancelTask(GetID());
				}
				break;
			case TASK_TYPE_WRITE:
				if (pTask->GetStatus() == TASK_STATUS_ERROR)
				{
					Log("文件写入出错");
					m_pManager->GetTaskManager().CancelTask(GetID());
					CServerManagerAckMsgCaller MsgCaller(this);
					MsgCaller.FileUploadDataAck(MSG_RESULT_FAILED, 0, false);
				}
				else if (pTask->GetWorkingQueryCount())
				{
					FILE_DATA_BLOCK* pData = pTask->GetFinishOperation(true);
					if (pData)
					{
						CServerManagerAckMsgCaller MsgCaller(this);
						MsgCaller.FileUploadDataAck(MSG_RESULT_SUCCEED, pData->OriginSize, pData->IsLast);
						LogDebug("上传文件块%u处理完成", pData->SerialNumber);
						m_pManager->GetTaskManager().DeleteFileDataInfo(pData);
					}					
				}
				else if (pTask->IsInWaitFinish() && (pTask->GetWorkingQueryCount() == 0))
				{
					pTask->CallFinishCallback();
					LogDebug("上传任务结束,MD5=%s", (LPCTSTR)pTask->GetFileMD5());
					pTask->CancelTask();
				}
				break;
			default:
				Log("未知任务类型%d", pTask->GetType());
				m_pManager->GetTaskManager().CancelTask(GetID());
			}
		}
	}

	return ProcessCount;
	FUNCTION_END;
	return 0;
}

void CServerManagerClient::OnMsg(CMessage * pMsg)
{
	//PrintNetLog("Msg(%u)=%u", pMsg->GetMsgID(), pMsg->GetMsgLength());
	if (!m_IsLogined && (pMsg->GetMsgID() != MAKE_MSG_ID(MODULE_ID_SVR_MGR, SVR_MGR_INTERFACE_SERVER_MANAGER, IServerManager::METHOD_LOGIN, false)))
	{
		Log("未登录时收到非登录消息");
		Disconnect();
		return;
	}
	MSG_HANDLE_INFO * pHandleInfo = m_MsgFnMap.Find(pMsg->GetMsgID());
	if (pHandleInfo)
	{
		CSmartStruct Packet = pMsg->GetDataPacket();
		((pHandleInfo->pObject)->*(pHandleInfo->pFN))(Packet);
	}
	else
	{
		LogDebug("无法找到消息[0x%X]的处理函数", pMsg->GetMsgID());
	}
}
void CServerManagerClient::OnSystemMsg(CMessage * pMsg)
{
	if (pMsg->GetMsgID() == SM_PROXY_KEEP_ALIVE_PING)
	{
		m_KeepAliveTimer.SaveTime();
		m_KeepAliveCount = 0;

		pMsg->SetMsgID(SM_PROXY_KEEP_ALIVE_PONG);
		DoSend(pMsg, pMsg->GetMsgLength());
	}
}





void CServerManagerClient::OnScriptExcute(int ErrorCode, int LastLine, UINT Param)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	MsgCaller.ExecuteScriptAck(MSG_RESULT_SUCCEED, Param, ErrorCode, LastLine);
}

bool CServerManagerClient::DoSend(LPCVOID pData, UINT Size)
{
	//Log("DoSend=%u", Size);
	//if (m_pDataLogFile)
	//{
	//	m_pDataLogFile->Write(pData, Size);
	//}


	if (Send(pData, Size))
		return true;
	return false;
}

CMessage * CServerManagerClient::NewMessage(UINT MsgDataSize)
{
	if (MsgDataSize + CMessage::GetMsgHeaderLength() < m_SendBuffer.GetBufferSize())
	{
		CMessage * pMsg = (CMessage *)m_SendBuffer.GetBuffer();
		pMsg->Init();
		return pMsg;
	}
	return NULL;
}
bool CServerManagerClient::ReleaseMessage(CMessage * pMsg)
{
	return true;
}
bool CServerManagerClient::SendMessage(CMessage * pMsg)
{
	return DoSend(pMsg, pMsg->GetMsgLength());
}


void CServerManagerClient::OnTaskFinish(CBaseTask* pTask, LPVOID Param)
{
	if (pTask->GetType() == TASK_TYPE_WRITE)
	{
		CFileTask* pFileTask = dynamic_cast<CFileTask*>(pTask);
		if(pFileTask)
		{
			LogDebug("上传任务结束,MD5=%s", (LPCTSTR)pFileTask->GetFileMD5());
			CServerManagerAckMsgCaller MsgCaller(this);
			MsgCaller.FileUploadFinishAck(MSG_RESULT_SUCCEED, pFileTask->GetFileMD5());
		}
		else
		{
			Log("任务对象错误");
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CServerManagerClient::Login(const CEasyString& UserName, const CEasyString& Password)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	if (CMainConfig::GetInstance()->VerfyUser(UserName, Password))
	{
		m_IsLogined = true;
		MsgCaller.LoginAck(MSG_RESULT_SUCCEED);
	}
	else
	{
		MsgCaller.LoginAck(MSG_RESULT_USER_NAME_OR_PASSWORD_ERROR);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::GetServiceList()
{
	CServerManagerAckMsgCaller MsgCaller(this);

	m_PacketBuffer1.Clear();
	CEasyArray<CServiceInfoEx>& ServiceList = m_pManager->GetServiceInfoList();
	CSmartArray Packet = m_PacketBuffer1.PrepareSubArray();

	for (UINT i = 0; i < ServiceList.GetCount(); i++)
	{
		CSmartStruct InfoPacket = Packet.PrepareSubStruct();
		ServiceList[i].MakePacket(InfoPacket,DOMF_SERVICE_INFO_LIST_FOR_CLIENT_FETCH);
		Packet.FinishMember(InfoPacket.GetDataLen());
	}
	m_PacketBuffer1.FinishMember(CServiceInfoList::SST_SRVIL_LIST, Packet.GetDataLen());
	MsgCaller.GetServiceListAck(MSG_RESULT_SUCCEED, m_PacketBuffer1);

	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::GetProcessList(short Page, short PageLen)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	if (PageLen)
	{
		m_PacketBuffer1.Clear();
		CEasyArray<CProcessInfo>& ProcessList = m_pManager->GetProcessInfoList().GetList();
		short TotalPage = ProcessList.GetCount() / PageLen + ((ProcessList.GetCount() % PageLen) ? 1 : 0);
		UINT StartIndex = Page * PageLen;
		UINT PackCount = PageLen;

		CSmartArray Packet = m_PacketBuffer1.PrepareSubArray();
		for (size_t i = StartIndex; i < ProcessList.GetCount() && PackCount; i++, PackCount--)
		{
			CSmartStruct SubPacket = Packet.PrepareSubStruct();
			ProcessList[i].MakePacket(SubPacket,DOMF_PROCESS_INFO_FULL);
			Packet.FinishMember(SubPacket.GetDataLen());
		}
		m_PacketBuffer1.FinishMember(CProcessInfoList::SST_PROCIL_LIST, Packet.GetDataLen());

		MsgCaller.GetProcessListAck(MSG_RESULT_SUCCEED, Page, PageLen, TotalPage, m_PacketBuffer1);
	}
	else
	{
		MsgCaller.GetProcessListAck(MSG_RESULT_FAILED, Page, PageLen, 0, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::GetNetAdapterList()
{
	CServerManagerAckMsgCaller MsgCaller(this);

	m_PacketBuffer1.Clear();
	m_pManager->GetNetAdapterInfo().MakePacket(m_PacketBuffer1);
	MsgCaller.GetNetAdapterListAck(MSG_RESULT_SUCCEED, m_PacketBuffer1);

	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::GetServiceInfo(UINT ServiceID)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	CServiceInfoEx * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		m_PacketBuffer1.Clear();
		pServiceInfo->MakePacket(m_PacketBuffer1, DOMF_SERVICE_INFO_FULL);
		MsgCaller.GetServiceInfoAck(MSG_RESULT_SUCCEED, m_PacketBuffer1);
	}
	else
	{
		MsgCaller.GetServiceInfoAck(MSG_RESULT_SERVICE_NOT_EXIST, CSmartStruct());
	}

	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::ServiceStartup(UINT ServiceID)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	int Result = m_pManager->StartupService(ServiceID);
	MsgCaller.ServiceStartupAck(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::ServiceShutdown(UINT ServiceID, BYTE ShutdownType)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	int Result = m_pManager->ShutdownService(ServiceID, ShutdownType);
	MsgCaller.ServiceShutdownAck(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::RunProgram(UINT ServiceID, const CEasyString& FilePath, const CEasyString& WorkDir, const CEasyString& Param)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	CServiceInfoEx * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{

		CEasyString Path;
		Path.Format("%s%c%s", (LPCTSTR)pServiceInfo->GetWorkDir(), DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
		Path = UTF8ToLocal(Path,Path.GetLength());
#else
		Path.Replace('\\', '/');
#endif // !WIN32
		Path = CFileTools::MakeFullPath(Path);

		CEasyString RunDir;
		RunDir.Format("%s%c%s", (LPCTSTR)pServiceInfo->GetWorkDir(), DIR_SLASH, WorkDir);
#ifdef WIN32
		RunDir = UTF8ToLocal(RunDir, RunDir.GetLength());
#else
		RunDir.Replace('\\', '/');
#endif // !WIN32
		RunDir = CFileTools::MakeFullPath(RunDir);

		int Result = m_pManager->StartupProcess(FilePath, RunDir, Param);
		MsgCaller.RunProgramAck(Result);
	}
	else
	{
		MsgCaller.RunProgramAck(MSG_RESULT_SERVICE_NOT_EXIST);
	}


	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::ProcessShutdown(UINT ProcessID, BYTE ShutdownType)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	int Result = m_pManager->ShutdownProcess(ProcessID, ShutdownType);
	MsgCaller.ProcessShutdownAck(Result, ProcessID);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::ExecuteScript(UINT ServiceID, const CEasyString& Script, bool FromFile)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	if (Script)
	{
		if(!m_IsInScriptExecute)
		{
			CServiceInfoEx * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
			if (pServiceInfo)
			{

				m_IsInScriptExecute = true;
				if (FromFile)
				{
					CStringFile ScriptFile;


					CEasyString FilePath = pServiceInfo->GetWorkDir() + "\\" + Script;
#ifdef WIN32
					FilePath = UTF8ToLocal(FilePath, FilePath.GetLength());
#else
					FilePath.Replace('\\', '/');
#endif // !WIN32
					FilePath = CFileTools::MakeFullPath(FilePath);
					if (ScriptFile.LoadFile(FilePath, false))
					{
						m_ScriptExecutor.ExcuteScript(ScriptFile.GetData(), pServiceInfo->GetWorkDir(), ServiceID);
					}
					else
					{
						MsgCaller.ExecuteScriptAck(MSG_RESULT_SCRIPT_FILE_NOT_EXIST, ServiceID, 0, 0);
					}
				}
				else
				{
					m_ScriptExecutor.ExcuteScript(Script, pServiceInfo->GetWorkDir(), ServiceID);
				}
			}
			else
			{
				MsgCaller.ExecuteScriptAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, 0, 0);
			}
		}
		else
		{
			MsgCaller.ExecuteScriptAck(MSG_RESULT_ONE_SCRIPT_IS_EXCUTING, ServiceID, 0, 0);
		}
	}
	else
	{
		MsgCaller.ExecuteScriptAck(MSG_RESULT_INVALID_SCRIPT, ServiceID, 0, 0);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::BrowseServiceDir(UINT ServiceID, const CEasyString& Dir, short Page, short PageLen)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	LPCTSTR szWorkDir = m_pManager->GetServiceWorkDir(ServiceID);
	if (szWorkDir)
	{
		CEasyString SearchDir;
		SearchDir.Format("%s%c%s", szWorkDir, DIR_SLASH, (LPCTSTR)Dir);
#ifndef WIN32
		SearchDir.Replace('\\','/');
#endif // !WIN32

		SearchDir = CFileTools::MakeFullPath(SearchDir);
		if (SearchDir.Find(szWorkDir) < 0)
		{
			//要浏览的目录不在工作目录下，浏览工作目录
			SearchDir = szWorkDir;
		}
		SearchDir.TrimRight(DIR_SLASH);

		bool IsWorkDir = false;
		if (SearchDir == szWorkDir)
			IsWorkDir = true;

		CEasyString RealDir = CFileTools::GetRelativePath(szWorkDir, SearchDir);

		SearchDir.Format("%s%c*", (LPCTSTR)SearchDir, DIR_SLASH);
#ifdef WIN32
		//协议使用UTF,所以win平台要进行转换
		SearchDir = UTF8ToLocal(SearchDir, SearchDir.GetLength());
#endif

		if (Page == 0 || m_RecentBrosweDir.CompareNoCase(SearchDir) != 0 || m_DirBrosweCacheTimeout.IsTimeOut(DIR_BROWSE_TIMEOUT))
		{
			//刷新目录浏览缓冲
			CFileSearcher FileSearcher;
			if (FileSearcher.FindFirst(SearchDir))
			{
				m_RecentBrosweDir = SearchDir;
				m_DirBrosweCacheTimeout.SaveTime();
				m_DirBrosweCache.Create(64, 64);

				while (FileSearcher.FindNext())
				{
					if ((!IsWorkDir) || (!FileSearcher.IsDots()))
					{
						FILE_INFO * pFileInfo = m_DirBrosweCache.AddEmpty();
						pFileInfo->Name = FileSearcher.GetFileName();
						if (FileSearcher.IsDirectory())
							pFileInfo->Attribute = FILE_ATTRIBUTE_FLAG_DIRECTORY;
						else
							pFileInfo->Attribute = 0;

						if (FileSearcher.CanExec())
							pFileInfo->Attribute |= FILE_ATTRIBUTE_FLAG_CAN_EXEC;

						pFileInfo->Size = FileSearcher.GetFileSize();
						pFileInfo->CreateTime = (time_t)FileSearcher.GetCreationTime();
						pFileInfo->LastAccessTime = (time_t)FileSearcher.GetLastAccessTime();
						pFileInfo->LastWriteTime = (time_t)FileSearcher.GetLastWriteTime();
					}
				}
			}
			else
			{
				MsgCaller.BrowseServiceDirAck(MSG_RESULT_DIR_NOT_EXIST, ServiceID, RealDir, Page, PageLen, 0, CSmartStruct());
				return COMMON_RESULT_SUCCEED;
			}
		}

		m_PacketBuffer1.Clear();

		short TotalPage = m_DirBrosweCache.GetCount() / PageLen + ((m_DirBrosweCache.GetCount() % PageLen) ? 1 : 0);
		UINT StartIndex = Page * PageLen;
		UINT PackCount = PageLen;

		UINT BufferSize;
		void * pBuffer = m_PacketBuffer1.PrepareMember(BufferSize);
		CSmartStruct Packet(pBuffer, BufferSize, true);
		for (size_t i = StartIndex; i < m_DirBrosweCache.GetCount() && PackCount; i++, PackCount--)
		{
			pBuffer = Packet.PrepareMember(BufferSize);
			CSmartStruct SubPacket(pBuffer, BufferSize, true);
			m_DirBrosweCache[i].MakePacket(SubPacket);
			Packet.FinishMember(FILE_INFO_LIST::SST_FILE_INFO_LIST_LIST, SubPacket.GetDataLen());
		}
		m_PacketBuffer1.FinishMember(FILE_INFO_LIST::SST_FILE_INFO_LIST_LIST, Packet.GetDataLen());

		MsgCaller.BrowseServiceDirAck(MSG_RESULT_SUCCEED, ServiceID, RealDir, Page, PageLen, TotalPage, m_PacketBuffer1);
	}
	else
	{
		MsgCaller.BrowseServiceDirAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, Dir, Page, PageLen, 0, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileDownloadStart(UINT ServiceID, const CEasyString& FilePath, UINT64 StartOffset)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	LogDebug("请求下载文件%s", (LPCTSTR)FilePath);
	
	if (StartOffset)
	{
		MsgCaller.FileDownloadStartAck(MSG_RESULT_NOT_SUPPORT, ServiceID, FilePath, 0, 0);
		Log("服务%u不支持断点续传", ServiceID);
	}
	else
	{
		if (m_pManager->GetTaskManager().GetTask(GetID()) == NULL)
		{
			LPCTSTR szWorkDir = m_pManager->GetServiceWorkDir(ServiceID);
			if (szWorkDir)
			{
				CEasyString Path;
				Path.Format("%s%c%s", szWorkDir, DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
				Path = UTF8ToLocal(Path, Path.GetLength());
#else
				Path.Replace('\\', '/');
#endif // !WIN32
				Path = CFileTools::MakeFullPath(Path);
				CFileTask* pTask = NULL;
				int Result = m_pManager->GetTaskManager().AddDownloadTask(GetID(), ServiceID, Path, StartOffset, &pTask);
				if (Result == MSG_RESULT_SUCCEED)
				{
					MsgCaller.FileDownloadStartAck(MSG_RESULT_SUCCEED, ServiceID, FilePath, pTask->GetFileSize(), pTask->GetFileLastWriteTime());
					Log("已添加下载任务%s", (LPCTSTR)Path);
				}
				else
				{
					MsgCaller.FileDownloadStartAck(MSG_RESULT_FILE_NOT_EXIST, ServiceID, FilePath, 0, 0);
					Log("添加下载任务失败%s", (LPCTSTR)Path);
				}

		}
			else
			{
				MsgCaller.FileDownloadStartAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath, 0, 0);
				Log("服务%u不存在", ServiceID);
			}
	}
		else
		{
			MsgCaller.FileDownloadStartAck(MSG_RESULT_IS_BUSY, ServiceID, FilePath, 0, 0);
			Log("服务%u还有文件任务未完成", ServiceID);
		}
}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileDownloadData()
{
	CServerManagerAckMsgCaller MsgCaller(this);

	CFileTask* pTask = m_pManager->GetTaskManager().GetTask(GetID());
	if (pTask)
	{
		if((pTask->GetType() == TASK_TYPE_READ))
		{
			if (pTask->GetWorkingQueryCount() < CMainConfig::GetInstance()->GetTaskManagerConfig().MaxDownloadAcceptCount)
			{
				pTask->AddReadQuery();
			}
			else
			{
				LogDebug("队列忙");
				MsgCaller.FileDownloadDataAck(MSG_RESULT_IS_BUSY, 0, 0, CEasyBuffer(), false);
			}
		}
		else
		{
			LogDebug("当前活动任务不是下载任务");
			MsgCaller.FileDownloadDataAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, 0, 0, CEasyBuffer(), false);
		}
	}
	else
	{
		LogDebug("当前无活动任务");
		MsgCaller.FileDownloadDataAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, 0, 0, CEasyBuffer(), false);
	}

	return COMMON_RESULT_SUCCEED;
}

int CServerManagerClient::FileDownloadFinish()
{
	CServerManagerAckMsgCaller MsgCaller(this);
	LogDebug("请求完成下载");
	CFileTask* pTask = m_pManager->GetTaskManager().GetTask(GetID());
	if (pTask)
	{
		if (pTask->GetType() == TASK_TYPE_READ)
		{
			LogDebug("下载任务结束,MD5=%s,FilePath=%s", (LPCTSTR)pTask->GetFileMD5(), (LPCTSTR)pTask->GetFilePath());
			MsgCaller.FileDownloadFinishAck(MSG_RESULT_SUCCEED, pTask->GetFileMD5());
			m_pManager->GetTaskManager().CancelTask(GetID());
		}
		else
		{
			LogDebug("当前活动任务不是下载任务");
			MsgCaller.FileDownloadFinishAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, "");
		}
	}
	else
	{
		LogDebug("当前无活动任务");
		MsgCaller.FileDownloadFinishAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, "");
	}

	return COMMON_RESULT_SUCCEED;
}

int CServerManagerClient::FileUploadStart(UINT ServiceID, const CEasyString& FilePath, UINT FileLastWriteTime)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	if (m_pManager->GetTaskManager().GetTask(GetID()) == NULL)
	{
		LPCTSTR szWorkDir = m_pManager->GetServiceWorkDir(ServiceID);
		if (szWorkDir)
		{
			CEasyString Path;
			Path.Format("%s%c%s", szWorkDir, DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
			Path = UTF8ToLocal(Path, Path.GetLength());
#else
			Path.Replace('\\', '/');
#endif // !WIN32
			Path = CFileTools::MakeFullPath(Path);
			CEasyString Dir = CFileTools::GetPathDirectory(Path);
			if(CFileTools::CreateDirEx(Dir))
			{
				CFileTask* pTask = NULL;
				int Result = m_pManager->GetTaskManager().AddUploadTask(GetID(), ServiceID, Path, 0, &pTask);
				if (Result == MSG_RESULT_SUCCEED)
				{
					pTask->SetFileLastWriteTime(FileLastWriteTime);
					MsgCaller.FileUploadStartAck(MSG_RESULT_SUCCEED, ServiceID, FilePath, pTask->GetFileSize());
					Log("已添加上传任务%s,FileTime=%u", (LPCTSTR)Path, FileLastWriteTime);
				}
				else
				{
					MsgCaller.FileUploadStartAck(MSG_RESULT_FILE_NOT_EXIST, ServiceID, FilePath, 0);
					Log("添加上传任务失败%s", (LPCTSTR)Path);
				}
			}
			else
			{
				MsgCaller.FileUploadStartAck(MSG_RESULT_FILE_WRITE_ERROR, ServiceID, FilePath, 0);
				Log("创建目录失败%s", (LPCTSTR)Dir);
			}
		}
		else
		{
			MsgCaller.FileUploadStartAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath, 0);
			Log("服务%u不存在", ServiceID);
		}
	}
	else
	{
		MsgCaller.FileUploadStartAck(MSG_RESULT_IS_BUSY, ServiceID, FilePath, 0);
		Log("服务%u还有文件任务未完成", ServiceID);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileUploadData(UINT Length, const CEasyBuffer& FileData, bool IsLast)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	//LogDebug("收到上传数据%u,原大%u %s", FileData.GetUsedSize(), Length, IsLast ? "IsLast" : "");
	CFileTask * pTask = m_pManager->GetTaskManager().GetTask(GetID());
	if ((pTask && (pTask->GetType() == TASK_TYPE_WRITE)))
	{
		bool NeedAck = (pTask->GetWorkingQueryCount() >= CMainConfig::GetInstance()->GetTaskManagerConfig().MaxUploadAcceptCount);
		if (pTask->AddWriteQuery(Length, FileData, IsLast, NeedAck))
		{
			if (NeedAck)
			{
				//LogDebug("并发上传数已满%u", pTask->GetWorkingQueryCount());
			}
			else
			{
				//LogDebug("已接受上传数据");
				MsgCaller.FileUploadDataAck(MSG_RESULT_SUCCEED, 0, IsLast);
			}
		}
		else
		{
			MsgCaller.FileUploadDataAck(MSG_RESULT_QUERY_SIZE_TOO_BIG, 0, IsLast);
		}
	}
	else
	{

		MsgCaller.FileUploadDataAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, 0, IsLast);
	}

	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileUploadFinish()
{
	CServerManagerAckMsgCaller MsgCaller(this);
	LogDebug("请求完成上传");
	CFileTask* pTask = m_pManager->GetTaskManager().GetTask(GetID());
	if (pTask)
	{
		if (pTask->GetType() == TASK_TYPE_WRITE)
		{
			pTask->WaitFinish(NULL, this, &CServerManagerClient::OnTaskFinish);
		}
		else
		{
			LogDebug("当前活动任务不是上传任务");
			MsgCaller.FileUploadFinishAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, "");
		}
	}
	else
	{
		LogDebug("当前无活动任务");
		MsgCaller.FileUploadFinishAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, "");
	}

	return COMMON_RESULT_SUCCEED;
}

int CServerManagerClient::CreateDir(UINT ServiceID, const CEasyString& Dir)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	LPCTSTR szWorkDir = m_pManager->GetServiceWorkDir(ServiceID);
	if (szWorkDir)
	{

		CEasyString Path;
		Path.Format("%s%c%s", szWorkDir, DIR_SLASH, (LPCTSTR)Dir);
#ifdef WIN32
		Path = UTF8ToLocal(Path, Path.GetLength());
#else
		Path.Replace('\\', '/');
#endif // !WIN32
		Path = CFileTools::MakeFullPath(Path);
		if (CFileTools::CreateDirEx(Path))
		{
			MsgCaller.CreateDirAck(MSG_RESULT_SUCCEED, ServiceID, Dir);
		}
		else
		{
			//SAFE_RELEASE(m_pCurDownloadFile);
			MsgCaller.CreateDirAck(MSG_RESULT_FAILED, ServiceID, Dir);
		}

	}
	else
	{
		MsgCaller.CreateDirAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, Dir);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::DeleteFile(UINT ServiceID, const CEasyString& FilePath, bool IsRecursive)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	LPCTSTR szWorkDir = m_pManager->GetServiceWorkDir(ServiceID);
	if (szWorkDir)
	{

		CEasyString Path;
		Path.Format("%s%c%s", szWorkDir, DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
		Path = UTF8ToLocal(Path, Path.GetLength());
#else
		Path.Replace('\\', '/');
		signal(SIGCHLD, SIG_DFL);
#endif // !WIN32
		Path = CFileTools::MakeFullPath(Path);
		if (CFileTools::DeleteFile(Path, IsRecursive))
		{
			MsgCaller.DeleteFileAck(MSG_RESULT_SUCCEED, ServiceID, FilePath);
		}
		else
		{
			MsgCaller.DeleteFileAck(MSG_RESULT_FAILED, ServiceID, FilePath);
		}
#ifndef WIN32
		signal(SIGCHLD, SIG_IGN);
#endif
	}
	else
	{
		MsgCaller.DeleteFileAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::ChangeFileMode(UINT ServiceID, const CEasyString& FilePath, UINT Mode)
{
	CServerManagerAckMsgCaller MsgCaller(this);
#ifdef WIN32
	MsgCaller.ChangeFileModeAck(MSG_RESULT_FAILED, ServiceID, FilePath, Mode);
#else
	LPCTSTR szWorkDir = m_pManager->GetServiceWorkDir(ServiceID);
	if (szWorkDir)
	{

		CEasyString Path;
		Path.Format("%s%c%s", szWorkDir, DIR_SLASH, FilePath);
		Path.Replace('\\', '/');
		Path = CFileTools::MakeFullPath(Path);
		if (chmod(Path, Mode))
		{
			MsgCaller.ChangeFileModeAck(MSG_RESULT_SUCCEED, ServiceID, FilePath, Mode);
		}
		else
		{
			//SAFE_RELEASE(m_pCurDownloadFile);
			MsgCaller.ChangeFileModeAck(MSG_RESULT_FAILED, ServiceID, FilePath, Mode);
		}

	}
	else
	{
		MsgCaller.ChangeFileModeAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath, Mode);
	}
#endif
	return COMMON_RESULT_SUCCEED;
}


int CServerManagerClient::AddService(const CSmartStruct& ServiceInfo)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	CServiceInfo SrvInfo;
	SrvInfo.ParsePacket(ServiceInfo, DOMF_SERVICE_INFO_FOR_EDIT);
	int Result = m_pManager->AddService(SrvInfo);
	MsgCaller.AddServiceAck(Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::EditService(const CSmartStruct& ServiceInfo)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	CServiceInfo SrvInfo;
	SrvInfo.ParsePacket(ServiceInfo, DOMF_SERVICE_INFO_FOR_EDIT);
	int Result = m_pManager->EditService(SrvInfo);
	MsgCaller.EditServiceAck(Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::DeleteService(UINT ServiceID)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	int Result = m_pManager->DeleteService(ServiceID);
	MsgCaller.DeleteServiceAck(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::SendCommand(UINT ServiceID, const CEasyString& Command)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	CServiceInfoEx * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->pControlPipe)
		{
			if (!pServiceInfo->pControlPipe->SendCommand(GetID(), Command))
			{
				MsgCaller.SendCommandAck(MSG_RESULT_FAILED, ServiceID);
			}
		}
		else
		{
			MsgCaller.SendCommandAck(MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE, ServiceID);
		}
	}
	else
	{
		MsgCaller.SendCommandAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID);
	}
	return COMMON_RESULT_SUCCEED;
}

int CServerManagerClient::EnableLogRecv(UINT ServiceID, bool Enable)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	if (m_EnableLogRecv)
	{
		//关闭旧的关联
		m_pManager->EnableLogRecv(m_LogRecvServiceID, false);
	}
	m_EnableLogRecv = Enable;
	m_LogRecvServiceID = ServiceID;
	int Result = m_pManager->EnableLogRecv(ServiceID, Enable);
	MsgCaller.EnableLogRecvAck(Result, ServiceID, Enable);
	return COMMON_RESULT_SUCCEED;
}

int CServerManagerClient::GetServerStatus(UINT ServiceID, const CSmartStruct& StatusListPacket)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	CServiceInfoEx * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->pControlPipe)
		{
			if (!pServiceInfo->pControlPipe->QueryGetServerStatus(GetID(), StatusListPacket))
			{
				MsgCaller.GetServerStatusAck(MSG_RESULT_FAILED, ServiceID, CSmartStruct());
			}
		}
		else
		{
			MsgCaller.GetServerStatusAck(MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE, ServiceID, CSmartStruct());
		}
	}
	else
	{
		MsgCaller.GetServerStatusAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::GetAllServerStatus(UINT ServiceID)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	CServiceInfoEx * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->pControlPipe)
		{
			if (!pServiceInfo->pControlPipe->QueryGetAllServerStatus(GetID()))
			{
				MsgCaller.GetServerStatusAck(MSG_RESULT_FAILED, ServiceID, CSmartStruct());
			}
		}
		else
		{
			MsgCaller.GetServerStatusAck(MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE, ServiceID, CSmartStruct());
		}
	}
	else
	{
		MsgCaller.GetServerStatusAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}

int CServerManagerClient::GetServerStatusFormat(UINT ServiceID)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	CServiceInfoEx * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		if (pServiceInfo->pControlPipe)
		{
			if (!pServiceInfo->pControlPipe->QueryGetServerStatusFormat(GetID()))
			{
				MsgCaller.GetServerStatusFormatAck(MSG_RESULT_FAILED, ServiceID, CSmartStruct());
			}
		}
		else
		{
			MsgCaller.GetServerStatusFormatAck(MSG_RESULT_CONTROL_PIPE_NOT_AVAILABLE, ServiceID, CSmartStruct());
		}
	}
	else
	{
		MsgCaller.GetServerStatusFormatAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}

int CServerManagerClient::FileCompare(UINT ServiceID, const CEasyString& FilePath, UINT64 FileSize, const CEasyString& FileMD5)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	LogDebug("请求对比文件%s,MD5=%s", (LPCTSTR)FilePath, (LPCTSTR)FileMD5);

	if (m_pManager->GetTaskManager().GetTask(GetID()) == NULL)
	{
		LPCTSTR szWorkDir = m_pManager->GetServiceWorkDir(ServiceID);
		if (szWorkDir)
		{

			CEasyString Path;
			Path.Format("%s%c%s", szWorkDir, DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
			Path = UTF8ToLocal(Path, Path.GetLength());
#else
			Path.Replace('\\', '/');
#endif // !WIN32
			Path = CFileTools::MakeFullPath(Path);
			CFileInfo FileInfo;
			if (FileInfo.FetchFileInfo(Path))
			{
				if (FileInfo.GetFileSize() == FileSize)
				{
					CFileTask * pTask = NULL;
					int Result = m_pManager->GetTaskManager().AddFileCompareTask(GetID(), ServiceID, Path, FileMD5, &pTask);
					if (Result != MSG_RESULT_SUCCEED)
					{
						MsgCaller.FileCompareAck(Result, ServiceID, FilePath);
					}					
				}
				else
				{
					MsgCaller.FileCompareAck(MSG_RESULT_FAILED, ServiceID, FilePath);
				}
			}
			else
			{
				MsgCaller.FileCompareAck(MSG_RESULT_FILE_NOT_EXIST, ServiceID, FilePath);
			}
		}
		else
		{
			MsgCaller.FileCompareAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath);
		}
	}
	else
	{
		MsgCaller.FileCompareAck(MSG_RESULT_IS_BUSY, ServiceID, FilePath);
	}
	return COMMON_RESULT_SUCCEED;
}
