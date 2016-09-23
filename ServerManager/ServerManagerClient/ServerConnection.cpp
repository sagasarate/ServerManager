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
	//m_pDataLogFile = NULL;

	CServerManagerAckMsgHandler::InitMsgMap(m_MsgFnMap);
}

CServerConnection::~CServerConnection(void)
{
	//SAFE_RELEASE(m_pDataLogFile);
}

bool CServerConnection::Init(CServerManagerClientView * pView, CNetServer * pServer, LPCTSTR szServerAddress, UINT ServerPort)
{
	m_pView = pView;
	SetServer(pServer);
	m_AssembleBuffer.Create(MAX_MSG_SIZE + NET_DATA_BLOCK_SIZE);
	m_SendBuffer.Create(MAX_MSG_SIZE + NET_DATA_BLOCK_SIZE);
	m_ServerAddress = szServerAddress;
	m_ServerPort = ServerPort;
	m_ConnectTimer.SaveTime(CEasyTimer::GetTime() + GetRand(-CONNECT_WAIT_TIME, CONNECT_WAIT_TIME));
	m_ServiceInfoFetchTimer.SaveTime(CEasyTimer::GetTime() + GetRand(-CONNECT_WAIT_TIME, CONNECT_WAIT_TIME));
	m_KeepAliveTimer.SaveTime();
	m_KeepAliveCount = 0;
	return true;
}

bool CServerConnection::Reconnection(LPCTSTR szServerAddress, UINT ServerPort)
{
	m_ServerAddress = szServerAddress;
	m_ServerPort = ServerPort;
	Disconnect();
	return true;
}

void CServerConnection::OnConnection(bool IsSucceed)
{
	if(IsSucceed)
	{
		PrintLog(_T("%s:%u已连接"),
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
		m_AssembleBuffer.SetUsedSize(0);
		m_KeepAliveTimer.SaveTime();
		m_KeepAliveCount = 0;

		if (m_pView)
		{
			m_pView->ConnectNotify(this);
		}

		//SAFE_RELEASE(m_pDataLogFile);
		//m_pDataLogFile = CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
		//if (m_pDataLogFile)
		//{
		//	m_pDataLogFile->Open(CFileTools::MakeModuleFullPath(NULL, _T("DataLog.bin")), IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite | IFileAccessor::osWriteThrough);
		//}
	}
	else
	{
		PrintLog(_T("%s:%u连接失败"),
			GetRemoteAddress().GetIPString(),
			GetRemoteAddress().GetPort());
	}
}

void CServerConnection::OnDisconnection()
{
	PrintLog(_T("%s:%u已断开"),
		GetRemoteAddress().GetIPString(),
		GetRemoteAddress().GetPort());

	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().DeleteAllTask();
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
		PrintLog(_T("装配缓冲已满!"));
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
			PrintLog(_T("尝试连接%s:%u"), (LPCTSTR)m_ServerAddress, m_ServerPort);
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
				PrintLog(_T("连接超时!"));
				Disconnect();
			}
		}
	}

	return ProcessCount;
}

void CServerConnection::PrintLog(LPCTSTR szFormat,...)
{
	if(m_pView)
	{
		va_list vl;

		va_start(vl,szFormat);
		m_pView->PrintLogVL(szFormat,vl);
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
		PrintLog(_T("无法找到消息[0x%X]的处理函数"), pMsg->GetMsgID());
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
//void CServerConnection::OnMsg(CSmartStruct& Msg)
//{
//	UINT MsgID=Msg.GetMember(SSMM_MSG_TYPE_ID);
//	switch(MsgID)
//	{
//	case SM_MSG_GET_SERVICE_INFO_LIST_RESULT:
//		{
//			void * Pos=Msg.GetFirstMemberPosition();
//			while(Pos)
//			{
//				WORD MemberID;
//				CSmartValue Value=Msg.GetNextMember(Pos,MemberID);
//				switch(MemberID)
//				{
//				case SGSILR_TOTAL_COUNT:
//					break;
//				case SGSILR_START_INDEX:
//					break;
//				case SGSILR_SERVICE_INFO:
//					{
//						CSmartStruct ServiceInfo=Value;
//
//						LPCTSTR ServiceName=ServiceInfo.GetMember(SSIL_NAME);
//						UINT Index=ServiceInfo.GetMember(SSIL_INDEX);
//						LPCTSTR ImageFilePath=ServiceInfo.GetMember(SSIL_IMAGE_FILE_PATH);
//						UINT Status=ServiceInfo.GetMember(SSIL_STATUS);
//						float CPUUsed=ServiceInfo.GetMember(SSIL_CPU_USED);
//						UINT MemoryUsed=ServiceInfo.GetMember(SSIL_MEMORY);
//						UINT VirtualMemoryUsed=ServiceInfo.GetMember(SSIL_VIRTUAL_MEMORY);
//						time_t ImageFileTime=ServiceInfo.GetMember(SSIL_IMAGE_FILE_TIME);
//						if(m_pView)
//						{
//							m_pView->SetServiceInfo(Index,m_ServerAddress,ServiceName,ImageFilePath,ImageFileTime,Status,CPUUsed,MemoryUsed,VirtualMemoryUsed);
//						}
//					}
//					break;
//				}
//			}
//		}
//		break;
//	case SM_MSG_GET_PROCESS_INFO_LIST_RESULT:
//		
//		break;
//	case SM_MSG_STARTUP_SERVICE_RESULT:
//		{
//			UINT ServiceID=Msg.GetMember(SSUS_SERVICE_INDEX);
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			PrintLog(_T("启动服务器[%s]上的服务[%u],结果[%d]"),
//				(LPCTSTR)m_ServerAddress,
//				ServiceID,
//				Result);
//		}		
//		break;
//	case SM_MSG_SHUTDOWN_SERVICE_RESULT:
//		{
//			UINT ServiceID=Msg.GetMember(SSDS_SERVICE_INDEX);
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			PrintLog(_T("关闭服务器[%s]上的服务[%u],结果[%d]"),
//				(LPCTSTR)m_ServerAddress,
//				ServiceID,
//				Result);
//		}
//		break;
//	case SM_MSG_BROWSE_SERVICE_DIR_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			if(Result==SSMRT_SUCCEED)
//			{
//				LPVOID Pos=Msg.GetFirstMemberPosition();
//				LPCTSTR szDir;
//				bool IsFinish=false;
//
//				while(Pos)
//				{
//					WORD MemberID;
//					CSmartValue Value=Msg.GetNextMember(Pos,MemberID);
//					switch(MemberID)
//					{
//					case SST_SMBSDR_LEFT_COUNT:
//						{
//							UINT LeftCount=Value;
//							if(LeftCount==0)
//							{
//								IsFinish=true;
//							}
//						}
//						break;
//					case SST_SMBSDR_DIRECTORY:
//						{
//							szDir=(LPCTSTR)Value;						
//						}
//						break;
//					case SST_SMBSDR_FILE_INFO:
//						{
//							CSmartStruct FileInfo=Value;
//							if(m_pView)
//							{
//								m_pView->OnBrowseResult(szDir,
//									FileInfo.GetMember(SST_SMFI_FILE_NAME),
//									FileInfo.GetMember(SST_SMFI_FILE_ATTRIBUTE),
//									FileInfo.GetMember(SST_SMFI_FILE_SIZE),
//									FileInfo.GetMember(SST_SMFI_FILE_CREATE_TIME),
//									FileInfo.GetMember(SST_SMFI_FILE_LAST_ACCESS_TIME),
//									FileInfo.GetMember(SST_SMFI_FILE_LAST_WRITE_TIME));
//							}
//						}
//						break;
//					}
//				}
//				if(IsFinish)
//				{
//					m_pView->OnBrowseFinish();
//				}
//			}		
//		}
//		break;
//	case SM_MSG_START_DOWNLOAD_SERVICE_FILE_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			UINT TotalSize=Msg.GetMember(SST_SMSDSFR_TOTAL_SIZE);
//			UINT OriginSize=Msg.GetMember(SST_SMSDSFR_ORIGIN_TOTAL_SIZE);
//			LPCTSTR pPackProp=Msg.GetMember(SST_SMSDSFR_PACK_PROP);
//			UINT PackPropSize=Msg.GetMember(SST_SMSDSFR_PACK_PROP).GetLength();
//			time_t LastWriteTime=Msg.GetMember(SST_SMSDSFR_FILE_LAST_WRITE_TIME);
//
//			m_pView->GetWorkDirBrowser()->OnStartDownloadResult(Result,TotalSize,OriginSize,pPackProp,PackPropSize,LastWriteTime);
//		}
//		break;
//	case SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			UINT TotalSize=Msg.GetMember(SST_SMQDSFDR_TOTAL_SIZE);
//			UINT LeftSize=Msg.GetMember(SST_SMQDSFDR_LEFT_SIZE);
//			CSmartValue FileData=Msg.GetMember(SST_SMQDSFDR_DATA);
//			m_pView->GetWorkDirBrowser()->OnDownloadData(Result,TotalSize,LeftSize,(LPCTSTR)FileData,FileData.GetLength());
//		}
//		break;
//	case SM_MSG_START_UPLOAD_SERVICE_FILE_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			if(Result==SSMRT_SUCCEED)
//			{
//				m_pView->GetWorkDirBrowser()->ContinueUploadFile(true);
//			}
//			else
//			{
//				m_pView->GetWorkDirBrowser()->ContinueUploadFile(false);
//			}
//		}
//		break;
//	case SM_MSG_SEND_UPLOAD_SERVICE_FILE_DATA_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			if(Result==SSMRT_SUCCEED)
//			{
//				m_pView->GetWorkDirBrowser()->ContinueUploadFile(true);
//			}
//			else
//			{
//				m_pView->GetWorkDirBrowser()->ContinueUploadFile(false);
//			}
//		}
//		break;
//	case SM_MSG_CREATE_DIR_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//
//			m_pView->GetWorkDirBrowser()->OnCreateDirResult(Result);
//		}
//		break;
//	case SM_MSG_DELETE_FILE_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//
//			if(Result==SSMRT_SUCCEED)
//			{
//				PrintLog(_T("删除文件成功"));
//			}
//			else
//			{
//				PrintLog(_T("删除文件失败"));
//			}
//			m_pView->GetWorkDirBrowser()->OnDeleteFileResult(Result);
//		}
//		break;
//	case SM_MSG_CREATE_PROCESS_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//
//			if(Result==SSMRT_SUCCEED)
//			{
//				PrintLog(_T("执行程序成功"));
//			}
//			else
//			{
//				PrintLog(_T("执行程序失败"));
//			}
//		}
//		break;
//	case SM_MSG_GET_NET_ADAPTER_INFO_RESULT:
//		{
//			void * Pos=Msg.GetFirstMemberPosition();
//			while(Pos)
//			{
//				WORD MemberID;
//				CSmartValue Value=Msg.GetNextMember(Pos,MemberID);
//				switch(MemberID)
//				{
//				case SST_SMGNAIR_TOTAL_COUNT:
//					break;
//				case SST_SMGNAIR_START_INDEX:
//					break;
//				case SST_SMGNAIR_NET_ADAPTER_INFO:
//					{
//						CSmartStruct AdapterInfo=Value;
//						CIPAddress ServiceIP(m_ServerAddress,m_ServerPort);
//
//						CIPAddress IPAddress((LPCTSTR)AdapterInfo.GetMember(SST_NAI_IP_ADDRESS), m_ServerPort);
//						UINT AdapterIndex=AdapterInfo.GetMember(SST_NAI_INDEX);						
//						float SendFlux=AdapterInfo.GetMember(SST_NAI_SEND_FLUX);
//						float ResvFlux=AdapterInfo.GetMember(SST_NAI_RECV_FLUX);
//						
//						if(m_pView)
//						{
//							if(IPAddress==ServiceIP)
//							{
//								m_pView->SetInternetAdapterInfo(m_ServerAddress,SendFlux,ResvFlux);
//							}
//							else
//							{
//								m_pView->SetIntranetAdapterInfo(m_ServerAddress,SendFlux,ResvFlux);
//							}
//						}
//					}
//					break;
//				}
//			}
//		}
//		break;
//	case SM_MSG_EXECUTE_SCRIPT_RESULT:
//		{
//			int Result=Msg.GetMember(SSMM_MSG_RESULT);
//			PrintLog(_T("运行脚本结果[%d]"),Result);
//		}
//		break;
//
//	}
//}




void CServerConnection::QueryServiceList()
{
	CServerManagerMsgCaller MsgCaller(this);
	MsgCaller.GetServiceList();
}

void CServerConnection::QueryStartupService(UINT ServiceID)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求启动服务器[%s]上的服务[%u]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID);
	MsgCaller.ServiceStartup(ServiceID);
}
void CServerConnection::QueryShutDownService(UINT ServiceID, bool IsForce)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求关闭服务器[%s]上的服务[%u]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID);
	MsgCaller.ServiceShutdown(ServiceID, IsForce);
}
void CServerConnection::QueryBrowseWorkDir(UINT ServiceID, LPCTSTR Dir)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求浏览服务器[%s]上的服务[%u]的目录[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,Dir);
	MsgCaller.BrowseServiceDir(ServiceID, Dir, 0, DIR_BROWSE_PAGE_LEN);
}

void CServerConnection::QueryStartDownload(UINT ServiceID,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求服务器[%s]上的服务[%u]下载文件[%s]到[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		SourceFilePath,TargetFilePath);

	MsgCaller.FileDownloadStart(ServiceID, SourceFilePath);
}

void CServerConnection::QueryDownloadData(UINT64 Offset, UINT Length)
{
	CServerManagerMsgCaller MsgCaller(this);

	MsgCaller.FileDownloadData(Offset, Length);
}

void CServerConnection::QueryEndDownload()
{
	CServerManagerMsgCaller MsgCaller(this);

	MsgCaller.FileDownloadEnd();
}

void CServerConnection::QueryStartUpload(UINT ServiceID,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求服务器[%s]上的服务[%u]上传文件[%s]到[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		SourceFilePath,TargetFilePath);

	MsgCaller.FileUploadStart(ServiceID, TargetFilePath);
}

void CServerConnection::QueryUploadData(UINT64 Offset, UINT Length, const CEasyBuffer& FileData)
{
	CServerManagerMsgCaller MsgCaller(this);

	//PrintLog(_T("请求服务器[%s]上的服务[%u]上传文件数据总长=%u,剩余=%u,原始=%u,当前=%u"),
	//	(LPCTSTR)m_ServerAddress,
	//	ServiceID,
	//	TotalSize,LeftSize,OrgDataSize,DataSize);

	MsgCaller.FileUploadData(Offset, Length, FileData);
}
void CServerConnection::QueryEndUpload(UINT FileLastWriteTime)
{
	CServerManagerMsgCaller MsgCaller(this);

	MsgCaller.FileUploadEnd(FileLastWriteTime);
}

void CServerConnection::QueryCreateDir(UINT ServiceID,LPCTSTR Dir)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求服务器[%s]上的服务[%u]创建目录[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		Dir);

	MsgCaller.CreateDir(ServiceID, Dir);
}

void CServerConnection::QueryDeleteFile(UINT ServiceID,LPCTSTR FilePath)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求服务器[%s]上的服务[%u]删除文件[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath);

	MsgCaller.DeleteFile(ServiceID, FilePath, true);
}
void CServerConnection::QueryChangeFileMode(UINT ServiceID, LPCTSTR FilePath, UINT Mode)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求服务器[%s]上的服务[%u]文件[%s]更改属性为[0x%X]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath, Mode);

	MsgCaller.ChangeFileMode(ServiceID, FilePath, Mode);
}
void CServerConnection::QueryCreateProcess(UINT ServiceID,LPCTSTR FilePath,LPCTSTR WorkDir,LPCTSTR Param)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求服务器[%s]上的服务[%u]运行程序[%s %s]"),
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

	PrintLog(_T("请求服务器[%s]上的服务[%u]运行脚本[%s]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		FilePath);

	MsgCaller.ExecuteScript(ServiceID, FilePath, true);	
}
void CServerConnection::QueryAddService(const SERVICE_INFO& ServiceInfo)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求添加服务[%s]"),
		(LPCTSTR)ServiceInfo.Name);

	MsgCaller.AddService(ServiceInfo);
}
void CServerConnection::QueryEditService(const SERVICE_INFO& ServiceInfo)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求编辑服务[%s]"),
		(LPCTSTR)ServiceInfo.Name);

	MsgCaller.EditService(ServiceInfo);
}
void CServerConnection::QueryDelService(UINT ServiceID)
{
	CServerManagerMsgCaller MsgCaller(this);

	PrintLog(_T("请求删除服务[%u]"),
		ServiceID);

	MsgCaller.DeleteService(ServiceID);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CServerConnection::GetServiceListAck(short Result, const CSmartStruct& ServiceListData)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		m_ServiceList.ParsePacket(ServiceListData);
		for (UINT i = 0; i < m_ServiceList.List.GetCount(); i++)
		{
			SERVICE_INFO& ServiceInfo = m_ServiceList.List[i];

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
int CServerConnection::ServiceStartupAck(short Result, UINT ServiceID)
{
	PrintLog(_T("启动服务器[%s]上的服务[%u],结果[%d]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ServiceShutdownAck(short Result, UINT ServiceID)
{
	PrintLog(_T("关闭服务器[%s]上的服务[%u],结果[%d]"),
		(LPCTSTR)m_ServerAddress,
		ServiceID,
		Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::RunProgramAck(short Result)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		PrintLog(_T("执行程序成功"));
	}
	else
	{
		PrintLog(_T("执行程序失败"));
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
int CServerConnection::FileDownloadStartAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT64 FileSize)
{
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
		OnStartDownloadResult(this, Result, ServiceID, FilePath, FileSize);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileDownloadDataAck(short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData)
{
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
		OnDownloadData(this, Result, Offset, Length, FileData);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileDownloadEndAck(short Result, UINT FileLastWriteTime)
{
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
		OnEndDownloadResult(this, Result, FileLastWriteTime);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileUploadStartAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT64 FileSize)
{
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
		OnUploadStartResult(this, Result, ServiceID, FilePath, FileSize);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileUploadDataAck(short Result, UINT64 Offset, UINT Length)
{
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
		OnUploadData(this, Result, Offset, Length);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::FileUploadEndAck(short Result)
{
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().OnEndUploadResult(this, Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::CreateDirAck(short Result, UINT ServiceID, const CEasyString& Dir)
{
	PrintLog(_T("新建目录结果%d[%s]"), Result, (LPCTSTR)Dir);
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().OnCreateDirResult(this, Result, ServiceID, Dir);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::DeleteFileAck(short Result, UINT ServiceID, const CEasyString& FilePath)
{
	PrintLog(_T("删除文件结果%d[%s]"), Result, (LPCTSTR)FilePath);
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().OnDeleteFileResult(this, Result, ServiceID, FilePath);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::ChangeFileModeAck(short Result, UINT ServiceID, const CEasyString& FilePath, UINT Mode)
{
	PrintLog(_T("修改文件属性结果%d[%s]"), Result, (LPCTSTR)FilePath);
	CServerManagerClientApp::GetInstance()->GetFileTransferQueue().OnChangeFileModeResult(this, Result, ServiceID, FilePath, Mode);
	return COMMON_RESULT_SUCCEED;
}

int CServerConnection::AddServiceAck(short Result)
{
	PrintLog(_T("添加服务结果%d"), Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::EditServiceAck(short Result)
{
	PrintLog(_T("编辑服务结果%d"), Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerConnection::DeleteServiceAck(short Result, UINT ServiceID)
{
	PrintLog(_T("删除服务结果%d"), Result);
	return COMMON_RESULT_SUCCEED;
}