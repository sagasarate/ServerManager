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
	m_IsInScriptExecute=false;
	m_pCurDownloadFile = NULL;
	m_pCurUploadFile = NULL;
	//m_pDataLogFile = NULL;

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
	m_CompressBuffer.Create(CMainConfig::GetInstance()->GetSendBufferSize() - 64);
	m_FileTransferBuffer.Create(CMainConfig::GetInstance()->GetSendBufferSize() - 64);

	if(!m_ScriptExecutor.Init(0,this))
	{
		Log("初始化脚本执行器失败");
		return FALSE;
	}
	m_AssembleBuffer.SetUsedSize(0);	
	m_WantDelete=false;
	m_IsInScriptExecute=false;
	SAFE_RELEASE(m_pCurDownloadFile);
	SAFE_RELEASE(m_pCurUploadFile);

	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CServerManagerClient::Destory()
{
	FUNCTION_BEGIN;	

	SAFE_RELEASE(m_pCurDownloadFile);
	SAFE_RELEASE(m_pCurUploadFile);
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
	m_WantDelete=true;
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
	while(m_AssembleBuffer.GetUsedSize()>=CMessage::GetMsgHeaderLength())
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
			m_KeepAliveCount++;
			if (m_KeepAliveCount > CMainConfig::GetInstance()->GetKeepAliveCount())
			{
				Log("KeepAlive超时");
				Disconnect();
			}
		}
	}

	return ProcessCount;
	FUNCTION_END;
	return 0;
}

void CServerManagerClient::OnMsg(CMessage * pMsg)
{
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
//{
//	FUNCTION_BEGIN;
//	UINT MsgID=Msg.GetMember(SSMM_MSG_TYPE_ID);
//	switch(MsgID)
//	{
//	case SM_MSG_GET_SERVICE_INFO_LIST:	
//		{
//			CEasyArray<SERVICE_INFO>& ServiceList=m_pManager->GetServiceInfoList();
//			char Buffer[NET_DATA_BLOCK_SIZE];
//			CSmartStruct Packet(Buffer,NET_DATA_BLOCK_SIZE,true);
//			UINT StartIndex=0;
//			do{
//				Packet.Clear();
//				Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_SERVICE_INFO_LIST_RESULT);
//				Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//				Packet.AddMember(SGSILR_TOTAL_COUNT,ServiceList.GetCount());
//				Packet.AddMember(SGSILR_START_INDEX,StartIndex);
//				StartIndex+=PackServiceInfo(ServiceList,StartIndex,Packet,SGSILR_SERVICE_INFO);
//				SendMsg(Packet);
//			}while(StartIndex<ServiceList.GetCount());
//		}
//		break;	
//	case SM_MSG_GET_PROCESS_INFO_LIST:
//		{
//			CEasyArray<SERVICE_INFO>& ProcessList=m_pManager->GetProcessInfoList();
//			char Buffer[NET_DATA_BLOCK_SIZE];
//			CSmartStruct Packet(Buffer,NET_DATA_BLOCK_SIZE,true);
//			UINT StartIndex=0;
//			do{
//				Packet.Clear();
//				Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_PROCESS_INFO_LIST_RESULT);
//				Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//				Packet.AddMember(SGPILR_TOTAL_COUNT,ProcessList.GetCount());
//				Packet.AddMember(SGPILR_START_INDEX,StartIndex);
//				StartIndex+=PackServiceInfo(ProcessList,StartIndex,Packet,SGPILR_SERVICE_INFO);
//				SendMsg(Packet);
//			}while(StartIndex<ProcessList.GetCount());
//		}
//		break;
//	case SM_MSG_STARTUP_SERVICE:
//		{
//			UINT ServiceIndex=Msg.GetMember(SSUS_SERVICE_INDEX);
//			Log("收到请求启动服务[%u]",ServiceIndex);
//			char Buffer[32];
//			CSmartStruct Result(Buffer,32,true);
//			Result.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_STARTUP_SERVICE_RESULT);
//			Result.AddMember(SSUS_SERVICE_INDEX,ServiceIndex);
//			if(m_pManager->StartupService(ServiceIndex))
//			{
//				Result.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);				
//			}
//			else
//			{
//				Result.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//			}
//			SendMsg(Result);
//		}
//		break;
//	case SM_MSG_SHUTDOWN_SERVICE:
//		{
//			UINT ServiceIndex=Msg.GetMember(SSDS_SERVICE_INDEX);
//			Log("收到请求关闭服务[%u]",ServiceIndex);
//			char Buffer[32];
//			CSmartStruct Result(Buffer,32,true);
//			Result.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_SHUTDOWN_SERVICE_RESULT);
//			Result.AddMember(SSDS_SERVICE_INDEX,ServiceIndex);
//			if(m_pManager->ShutdownService(ServiceIndex))
//			{
//				Result.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);				
//			}
//			else
//			{
//				Result.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//			}
//			SendMsg(Result);
//		}
//		break;
//	case SM_MSG_BROWSE_SERVICE_DIR:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMBSD_SERVICE_INDEX);
//			LPCTSTR QueryDir=Msg.GetMember(SST_SMBSD_DIRECTORY_NAME);
//			if(QueryDir&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
//			{
//				Log("请求服务[%u]目录[%s]的信息",
//					ServiceIndex,
//					QueryDir);
//
//				SERVICE_INFO& ServiceInfo=m_pManager->GetServiceInfoList()[ServiceIndex];
//
//				CEasyString SearchDir;
//				CEasyString WorkDir=ServiceInfo.WorkDir;
//				WorkDir=MakeFullPath(WorkDir+"\\");
//				WorkDir.MakeUpper();
//				SearchDir=WorkDir+QueryDir+"\\";
//				SearchDir=MakeFullPath(SearchDir);
//				SearchDir.MakeUpper();
//				if(SearchDir.Find(WorkDir)!=-1)
//				{
//					if(SendFileList(ServiceIndex,WorkDir,SearchDir))
//					{
//						return;
//					}
//					else
//					{
//						Log("请求服务[%u]目录[%s]的信息,读取目录失败",
//							ServiceIndex,
//							(LPCTSTR)SearchDir);
//					}
//				}
//				else
//				{
//					Log("请求服务[%u]目录[%s]的信息,目录非法",
//						ServiceIndex,
//						(LPCTSTR)SearchDir);
//				}
//			}
//			Log("请求服务[%u]目录信息失败",
//				ServiceIndex);
//
//			char SendBuffer[64];
//			CSmartStruct ReturnMsg(SendBuffer,64,true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_BROWSE_SERVICE_DIR_RESULT);
//			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMBSD_DIRECTORY_NOT_EXIST);
//			ReturnMsg.AddMember(SST_SMBSD_SERVICE_INDEX,ServiceIndex);			
//			SendMsg(ReturnMsg);
//			return;
//		}
//		break;
//	case SM_MSG_START_DOWNLOAD_SERVICE_FILE:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMSDSF_SERVICE_INDEX);
//			LPCTSTR szFilePath=(LPCTSTR)Msg.GetMember(SST_SMSDSF_FILE_PATH);
//
//			char SendBuffer[MAX_PATH+128];
//			CSmartStruct ReturnMsg(SendBuffer,sizeof(SendBuffer),true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_DOWNLOAD_SERVICE_FILE_RESULT);			
//			ReturnMsg.AddMember(SST_SMSDSFR_SERVICE_INDEX,ServiceIndex);
//			ReturnMsg.AddMember(SST_SMSDSFR_FILE_PATH,szFilePath);
//
//			if(szFilePath&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
//			{
//				CEasyString FilePath=m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir;
//				FilePath=FilePath+"\\"+szFilePath;
//
//				CMainThread::GetInstance()->GetFileCache()->QueryGetFile(FilePath,this,ServiceIndex);
//
//				
//			}
//			else
//			{
//				Log("请求服务[%u]下载文件[%s]失败",
//					ServiceIndex,szFilePath);
//				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//				SendMsg(ReturnMsg);
//			}
//			
//		}
//		break;
//	case SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMQDSFD_SERVICE_INDEX);
//
//
//			UINT DataSize=MAX_DATA_TRANSFER_SIZE;
//			if(m_TransferLeftSize<DataSize)
//				DataSize=m_TransferLeftSize;
//			BYTE * pDataBuffer=(BYTE *)m_TransferBuffer.GetBuffer()+m_TransferBuffer.GetUsedSize()-m_TransferLeftSize;
//			m_TransferLeftSize-=DataSize;
//
//
//			char SendBuffer[MAX_DATA_TRANSFER_SIZE+128];
//			CSmartStruct ReturnMsg(SendBuffer,sizeof(SendBuffer),true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_QUERY_DOWNLOAD_SERVICE_FILE_DATA_RESULT);
//			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//			ReturnMsg.AddMember(SST_SMQDSFDR_SERVICE_INDEX,ServiceIndex);
//			ReturnMsg.AddMember(SST_SMQDSFDR_TOTAL_SIZE,m_TransferBuffer.GetUsedSize());
//			ReturnMsg.AddMember(SST_SMQDSFDR_LEFT_SIZE,m_TransferLeftSize);
//			ReturnMsg.AddMember(SST_SMQDSFDR_DATA,(LPCTSTR)pDataBuffer,DataSize);
//			SendMsg(ReturnMsg);
//		}
//		break;
//	case SM_MSG_START_UPLOAD_SERVICE_FILE:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMSUSF_SERVICE_INDEX);
//			LPCTSTR szFilePath=(LPCTSTR)Msg.GetMember(SST_SMSUSF_FILE_PATH);
//			UINT TotalSize=Msg.GetMember(SST_SMSUSF_TOTAL_SIZE);
//			m_TransferBuffer.Create(TotalSize);
//			m_OriginSize=Msg.GetMember(SST_SMSUSF_ORIGIN_TOTAL_SIZE);
//			UINT PackPropSize=Msg.GetMember(SST_SMSUSF_PACK_PROP).GetLength();
//			m_PackProp.Create(PackPropSize);
//			m_PackProp.PushBack((LPCTSTR)Msg.GetMember(SST_SMSUSF_PACK_PROP),PackPropSize);
//			if(Msg.IsMemberExist(SST_SMSUSF_FILE_LAST_WRITE_TIME))
//			{
//				m_CurTransferFileLastWriteTime=(time_t)Msg.GetMember(SST_SMSUSF_FILE_LAST_WRITE_TIME);
//			}
//			else
//			{
//				m_CurTransferFileLastWriteTime.FetchSystemTime();
//			}
//
//			
//			
//
//			char SendBuffer[128+MAX_PATH];
//			CSmartStruct ReturnMsg(SendBuffer,128+MAX_PATH,true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_UPLOAD_SERVICE_FILE_RESULT);			
//			ReturnMsg.AddMember(SST_SMSUSFR_SERVICE_INDEX,ServiceIndex);
//			ReturnMsg.AddMember(SST_SMSUSFR_FILE_PATH,szFilePath);
//
//			if(szFilePath&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
//			{
//				m_TransferFilePath=m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir;
//				m_TransferFilePath=m_TransferFilePath+"\\"+szFilePath;
//				CEasyString FileDir=GetPathDirectory(m_TransferFilePath);
//				if(CreateDirEx(FileDir))				
//				{
//					CWinFileAccessor FileAccessor;
//					if(FileAccessor.Open(m_TransferFilePath,CWinFileAccessor::modeCreateAlways|CWinFileAccessor::modeWrite|CWinFileAccessor::shareShareAll))
//					{
//
//						FileAccessor.Close();
//						
//						Log("请求服务[%u]开始上传文件[%s]成功,数据传输大小%u,原始数据大小%u",
//							ServiceIndex,szFilePath,
//							TotalSize,m_OriginSize);
//
//						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//					}
//					else
//					{
//						int Err=GetLastError();
//						Log("请求服务[%u]开始上传文件[%s]打开失败%d",
//							ServiceIndex,szFilePath,Err);
//
//						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//					}
//				}
//				else
//				{
//					Log("请求服务[%u]开始上传文件[%s]创建目录失败",
//						ServiceIndex,szFilePath);
//
//					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//				}	
//			}
//			else
//			{
//				Log("请求服务[%u]开始上传文件[%s]失败",
//					ServiceIndex,szFilePath);
//
//				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//			}
//
//			SendMsg(ReturnMsg);
//		}
//		break;
//	case SM_MSG_SEND_UPLOAD_SERVICE_FILE_DATA:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMSUSFD_SERVICE_INDEX);
//			UINT TotalSize=Msg.GetMember(SST_SMSUSFD_TOTAL_SIZE);
//			UINT LeftSize=Msg.GetMember(SST_SMSUSFD_LEFT_SIZE);
//			CSmartValue FileData=Msg.GetMember(SST_SMSUSFD_DATA);
//
//			char SendBuffer[64];
//			CSmartStruct ReturnMsg(SendBuffer,64,true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_SEND_UPLOAD_SERVICE_FILE_DATA_RESULT);			
//			ReturnMsg.AddMember(SST_SMSUSFDR_SERVICE_INDEX,ServiceIndex);
//
//
//			m_TransferBuffer.PushBack((LPCTSTR)FileData,FileData.GetLength());
//			
//			
//			//Log("请求服务[%u]上传文件写入数据[%u]成功,未解压大小[%u]",
//			//	ServiceIndex,UnpackSize,FileData.GetLength());
//			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);				
//			
//			SendMsg(ReturnMsg);
//
//			if(LeftSize==0)
//			{
//				Log("请求服务[%u]上传文件完毕",
//					ServiceIndex);
//
//				CWinFileAccessor FileAccessor;
//				if(FileAccessor.Open(m_TransferFilePath,CWinFileAccessor::modeCreateAlways|CWinFileAccessor::modeWrite))
//				{
//					CEasyBuffer WriteBuffer;
//					WriteBuffer.Create(m_OriginSize);
//
//					size_t UnpackSize=m_OriginSize;
//					size_t SourceSize=m_TransferBuffer.GetUsedSize();
//
//					if(LzmaUncompress((BYTE *)WriteBuffer.GetBuffer(),&UnpackSize,
//						(BYTE *)m_TransferBuffer.GetBuffer(),&SourceSize,
//						(BYTE *)m_PackProp.GetBuffer(),m_PackProp.GetUsedSize())==SZ_OK)
//					{
//						Log("请求服务[%u]上传文件,数据解压完毕,大小%u",
//							ServiceIndex,UnpackSize);
//						if(FileAccessor.Write(WriteBuffer.GetBuffer(),UnpackSize)==UnpackSize)
//						{
//							FileAccessor.SetLastWriteTime(m_CurTransferFileLastWriteTime);
//							Log("请求服务[%u]上传文件,数据写入完毕,大小%u",
//								ServiceIndex,UnpackSize);
//						}
//						else
//						{
//							Log("请求服务[%u]上传文件,数据写入失败",
//								ServiceIndex);
//						}
//					}
//					else
//					{
//						Log("请求服务[%u]上传文件,数据解压失败",
//							ServiceIndex);
//					}
//
//					FileAccessor.Close();
//				}
//			}
//		}
//		break;
//	case SM_MSG_CREATE_DIR:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMCD_SERVICE_INDEX);
//			LPCTSTR szDir=(LPCTSTR)Msg.GetMember(SST_SMCD_DIR_NAME);
//
//			char SendBuffer[64];
//			CSmartStruct ReturnMsg(SendBuffer,64,true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_CREATE_DIR_RESULT);			
//			ReturnMsg.AddMember(SST_SMCDR_SERVICE_INDEX,ServiceIndex);
//
//			if(szDir&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
//			{
//				CEasyString Dir=m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir;
//				Dir=MakeFullPath(Dir+"\\"+szDir);	
//
//				Log("请求服务[%u]创建目录[%s]",
//					ServiceIndex,(LPCTSTR)Dir);
//				if(CreateDirEx(Dir))				
//				{
//					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//				}
//				else
//				{
//					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//				}
//			}
//			else
//			{
//				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//			}
//
//			SendMsg(ReturnMsg);
//		}
//		break;
//	case SM_MSG_DELETE_FILE:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMDF_SERVICE_INDEX);
//			LPCTSTR szFilePath=(LPCTSTR)Msg.GetMember(SST_SMDF_FILE_PATH);
//
//			char SendBuffer[64];
//			CSmartStruct ReturnMsg(SendBuffer,64,true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_DELETE_FILE_RESULT);			
//			ReturnMsg.AddMember(SST_SMDFR_SERVICE_INDEX,ServiceIndex);
//
//			if(szFilePath&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
//			{
//				CEasyString WorkDir=MakeFullPath(m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir);
//				CEasyString FilePath=MakeFullPath(WorkDir+"\\"+szFilePath);	
//
//				Log("请求服务[%u]删除文件[%s]",
//					ServiceIndex,(LPCTSTR)FilePath);
//
//				WorkDir.MakeUpper();
//				FilePath.MakeUpper();
//
//				if(FilePath.Find(WorkDir)>=0)				
//				{
//					char szFileFrom[MAX_PATH];
//					strncpy_0(szFileFrom,MAX_PATH,FilePath,MAX_PATH);
//					if(FilePath.GetLength()<MAX_PATH-1)
//						szFileFrom[FilePath.GetLength()+1]=0;
//
//					SHFILEOPSTRUCT Operation;
//					Operation.hwnd=NULL;
//					Operation.wFunc=FO_DELETE;
//					Operation.pFrom=szFileFrom;
//					Operation.pTo=NULL;
//					Operation.fAnyOperationsAborted=FALSE;
//					Operation.hNameMappings=NULL;
//					Operation.lpszProgressTitle=NULL;
//					Operation.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;
//
//					UINT Code=SHFileOperation(&Operation);
//					if(Code==0)
//					{
//						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//					}
//					else
//					{
//						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//					}
//				}
//				else
//				{
//					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//				}
//			}
//			else
//			{
//				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//			}
//
//			SendMsg(ReturnMsg);
//		}
//		break;
//	case SM_MSG_CREATE_PROCESS:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMCP_SERVICE_INDEX);
//			LPCTSTR szImageFilePath=(LPCTSTR)Msg.GetMember(SST_SMCP_IMAGE_FILE_PATH);
//			LPCTSTR szWorkDir=(LPCTSTR)Msg.GetMember(SST_SMCP_WORK_DIR);
//			LPCTSTR szParam=(LPCTSTR)Msg.GetMember(SST_SMCP_PARAM);
//
//			char SendBuffer[64];
//			CSmartStruct ReturnMsg(SendBuffer,64,true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_CREATE_PROCESS_RESULT);			
//			ReturnMsg.AddMember(SST_SMCPR_SERVICE_INDEX,ServiceIndex);
//
//			if(szImageFilePath&&szWorkDir&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
//			{
//				
//				CEasyString ServiceDir=MakeFullPath(m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir);
//				CEasyString ImageFilePath=MakeFullPath(ServiceDir+"\\"+szImageFilePath);
//				CEasyString WorkDir=MakeFullPath(ServiceDir+"\\"+szWorkDir);
//				
//
//				Log("请求服务[%u]启动程序[%s %s]在[%s]",
//					ServiceIndex,
//					(LPCTSTR)ImageFilePath,
//					szParam,
//					WorkDir);
//
//				ServiceDir.MakeUpper();
//				ImageFilePath.MakeUpper();
//
//				if(ImageFilePath.Find(ServiceDir)>=0)				
//				{					
//
//					SERVICE_INFO ProcessInfo;
//
//					strncpy_0(ProcessInfo.ImageFilePath,MAX_PATH,ImageFilePath,MAX_PATH);
//					strncpy_0(ProcessInfo.WorkDir,MAX_PATH,WorkDir,MAX_PATH);
//					if(szParam)
//						strncpy_0(ProcessInfo.StartupParam,MAX_PATH,szParam,MAX_PATH);
//
//					if(m_pManager->StartupProcess(ProcessInfo))				
//					{
//						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//					}
//					else
//					{
//						ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//					}
//				}
//				else
//				{
//					ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//				}
//			}
//			else
//			{
//				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//			}
//
//			SendMsg(ReturnMsg);
//		}
//		break;
//	case SM_MSG_GET_NET_ADAPTER_INFO:
//		{
//			CEasyArray<NET_ADAPTER_INFO>& AdapterInfoList=m_pManager->GetNetAdapterInfo();
//
//			char Buffer[NET_DATA_BLOCK_SIZE];
//			CSmartStruct Packet(Buffer,NET_DATA_BLOCK_SIZE,true);
//			Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_NET_ADAPTER_INFO_RESULT);
//			Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//			Packet.AddMember(SST_SMGNAIR_TOTAL_COUNT,AdapterInfoList.GetCount());
//			Packet.AddMember(SST_SMGNAIR_START_INDEX,0);
//
//			for(UINT i=0;i<AdapterInfoList.GetCount();i++)
//			{
//				if(Packet.GetDataLen()>=MAX_DATA_TRANSFER_SIZE)
//				{
//					SendMsg(Packet);
//					Packet.Clear();
//					Packet.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_GET_NET_ADAPTER_INFO_RESULT);
//					Packet.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//					Packet.AddMember(SST_SMGNAIR_TOTAL_COUNT,AdapterInfoList.GetCount());
//					Packet.AddMember(SST_SMGNAIR_START_INDEX,i);
//				}
//				PackNetAdapterInfo(AdapterInfoList[i],Packet,SST_SMGNAIR_NET_ADAPTER_INFO);				
//				
//			}
//			SendMsg(Packet);
//		}
//		break;
//	case SM_MSG_EXECUTE_SCRIPT:
//		{
//			UINT ServiceIndex=Msg.GetMember(SST_SMES_SERVICE_INDEX);
//			int ScriptType=Msg.GetMember(SST_SMES_SCRIPT_TYPE);
//			LPCTSTR szScript=(LPCTSTR)Msg.GetMember(SST_SMES_SCRIPT);
//
//			char SendBuffer[64];
//			CSmartStruct ReturnMsg(SendBuffer,64,true);
//			ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_EXECUTE_SCRIPT_RESULT);			
//			ReturnMsg.AddMember(SST_SMESR_SERVICE_INDEX,ServiceIndex);
//			if(szScript&&ServiceIndex<m_pManager->GetServiceInfoList().GetCount())
//			{
//				if(!m_IsInScriptExecute)
//				{
//					CEasyString WorkDir=MakeFullPath(m_pManager->GetServiceInfoList()[ServiceIndex].WorkDir);
//					m_IsInScriptExecute=true;
//					if(ScriptType==SCRIPT_TYPE_FROM_FILE)
//					{
//						CWinFileAccessor FileAccessor;
//
//						
//						CEasyString FilePath=MakeFullPath(WorkDir+"\\"+szScript);	
//						if(FileAccessor.Open(FilePath,CWinFileAccessor::modeOpen|CWinFileAccessor::modeRead|CWinFileAccessor::shareShareAll))
//						{
//							CEasyBuffer ReadBuffer;
//							UINT Size=FileAccessor.GetSize();
//							ReadBuffer.Create(Size+1);
//							if(FileAccessor.Read(ReadBuffer.GetBuffer(),Size)==Size)
//							{
//								ReadBuffer.SetUsedSize(Size);
//								ReadBuffer.PushConstBack(0,1);
//								m_ScriptExecutor.ExcuteScript((LPCTSTR)ReadBuffer.GetBuffer(),WorkDir,ServiceIndex);
//							}
//							else
//							{
//								ReturnMsg.AddMember(SSMM_MSG_RESULT,SMESRT_SCRIPT_FILE_READ_ERROR);
//								SendMsg(ReturnMsg);
//							}
//						}
//						else
//						{
//							ReturnMsg.AddMember(SSMM_MSG_RESULT,SMESRT_SCRIPT_FILE_NOT_EXIST);
//							SendMsg(ReturnMsg);
//						}
//
//					}
//					else
//					{
//						m_ScriptExecutor.ExcuteScript(szScript,WorkDir,ServiceIndex);
//					}
//				}
//				else
//				{
//					ReturnMsg.AddMember(SSMM_MSG_RESULT,SMESRT_ONE_SCRIPT_IS_EXCUTING);
//					SendMsg(ReturnMsg);
//				}
//			}
//			else
//			{
//				ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_FAILED);
//				SendMsg(ReturnMsg);
//			}
//
//		}
//		break;
//	}
//	
//	FUNCTION_END;
//}


//void CServerManagerClient::OnGetFile(FILE_CACHE * pFileCache)
//{
//	char SendBuffer[MAX_PATH+128];
//	CSmartStruct ReturnMsg(SendBuffer,sizeof(SendBuffer),true);
//	ReturnMsg.AddMember(SSMM_MSG_TYPE_ID,SM_MSG_START_DOWNLOAD_SERVICE_FILE_RESULT);			
//	ReturnMsg.AddMember(SST_SMSDSFR_SERVICE_INDEX,pFileCache->QueryParam);
//	ReturnMsg.AddMember(SST_SMSDSFR_FILE_PATH,(LPCTSTR)pFileCache->FilePath);
//	if(pFileCache->IsReady)
//	{
//		m_FileTransferBuffer.SetUsedSize(0);
//		m_OriginSize=pFileCache->FileSize;
//
//		if(m_OriginSize)
//		{	
//			m_FileTransferBuffer=pFileCache->FileData;
//			m_PackProp=pFileCache->PackProp;
//			m_TransferLeftSize=pFileCache->PackSize;			
//
//			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//			ReturnMsg.AddMember(SST_SMSDSFR_TOTAL_SIZE,pFileCache->PackSize);
//			ReturnMsg.AddMember(SST_SMSDSFR_ORIGIN_TOTAL_SIZE,m_OriginSize);
//			ReturnMsg.AddMember(SST_SMSDSFR_PACK_PROP,(LPCTSTR)m_PackProp.GetBuffer(),m_PackProp.GetUsedSize());
//			ReturnMsg.AddMember(SST_SMSDSFR_FILE_LAST_WRITE_TIME,(time_t)pFileCache->LastWriteTime);
//
//
//			SendMsg(ReturnMsg);
//					
//		}
//		else
//		{
//			Log("请求服务[%u]下载文件[%s]长度为0",
//				pFileCache->QueryParam,
//				(LPCTSTR)pFileCache->FilePath);
//
//			ReturnMsg.AddMember(SST_SMSDSFR_TOTAL_SIZE,0);
//			ReturnMsg.AddMember(SST_SMSDSFR_ORIGIN_TOTAL_SIZE,0);						
//			ReturnMsg.AddMember(SSMM_MSG_RESULT,SSMRT_SUCCEED);
//			SendMsg(ReturnMsg);
//
//		}
//		
//	}
//	else
//	{
//		Log("请求服务[%u]下载文件[%s]读取失败失败",
//			pFileCache->QueryParam,
//			(LPCTSTR)pFileCache->FilePath);
//		ReturnMsg.AddMember(SSMM_MSG_RESULT,SMSDSFRT_FILE_NOT_EXIST);
//		SendMsg(ReturnMsg);
//	}
//}

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CServerManagerClient::GetServiceList()
{
	CServerManagerAckMsgCaller MsgCaller(this);

	m_PacketBuffer1.Clear();
	m_pManager->GetServiceInfoList().MakePacket(m_PacketBuffer1);
	MsgCaller.GetServiceListAck(COMMON_RESULT_SUCCEED, m_PacketBuffer1);

	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::GetProcessList(short Page, short PageLen)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	if (PageLen)
	{
		m_PacketBuffer1.Clear();
		CEasyArray<PROCESS_INFO>& ProcessList = m_pManager->GetProcessInfoList().List;
		short TotalPage = ProcessList.GetCount() / PageLen + ((ProcessList.GetCount() % PageLen) ? 1 : 0);
		UINT StartIndex = Page * PageLen;
		UINT PackCount = PageLen;

		UINT BufferSize;
		void * pBuffer = m_PacketBuffer1.PrepareMember(BufferSize);
		CSmartStruct Packet(pBuffer, BufferSize, true);
		for (size_t i = StartIndex; i < ProcessList.GetCount() && PackCount; i++, PackCount--)
		{
			pBuffer = Packet.PrepareMember(BufferSize);
			CSmartStruct SubPacket(pBuffer, BufferSize, true);
			ProcessList[i].MakePacket(SubPacket);
			Packet.FinishMember(PROCESS_INFO_LIST::SST_SERVICE_INFO_LIST_LIST, SubPacket.GetDataLen());
		}
		m_PacketBuffer1.FinishMember(PROCESS_INFO_LIST::SST_SERVICE_INFO_LIST_LIST, Packet.GetDataLen());

		MsgCaller.GetProcessListAck(COMMON_RESULT_SUCCEED, Page, PageLen, TotalPage, m_PacketBuffer1);
	}
	else
	{
		MsgCaller.GetProcessListAck(COMMON_RESULT_FAILED, Page, PageLen, 0, CSmartStruct());
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::GetNetAdapterList()
{
	CServerManagerAckMsgCaller MsgCaller(this);

	m_PacketBuffer1.Clear();
	m_pManager->GetNetAdapterInfo().MakePacket(m_PacketBuffer1);
	MsgCaller.GetNetAdapterListAck(COMMON_RESULT_SUCCEED, m_PacketBuffer1);

	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::ServiceStartup(UINT ServiceID)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	int Result = m_pManager->StartupService(ServiceID);
	MsgCaller.ServiceStartupAck(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::ServiceShutdown(UINT ServiceID, bool IsForce)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	int Result = m_pManager->ShutdownService(ServiceID, IsForce);
	MsgCaller.ServiceShutdownAck(Result, ServiceID);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::RunProgram(UINT ServiceID, const CEasyString& FilePath, const CEasyString& WorkDir, const CEasyString& Param)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{

		CEasyString Path;
		Path.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
		Path = UTF8ToLocal(Path,Path.GetLength());
#else
		Path.Replace('\\', '/');		
#endif // !WIN32
		Path = CFileTools::MakeFullPath(Path);

		CEasyString RunDir;
		RunDir.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, WorkDir);
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
int CServerManagerClient::ProcessShutdown(UINT ProcessID, bool IsForce)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	int Result = m_pManager->ShutdownProcess(ProcessID, IsForce);
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
			SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
			if (pServiceInfo)
			{
				
				m_IsInScriptExecute = true;
				if (FromFile)
				{
					CStringFile ScriptFile;


					CEasyString FilePath = pServiceInfo->WorkDir + "\\" + Script;
#ifdef WIN32
					FilePath = UTF8ToLocal(FilePath, FilePath.GetLength());
#else
					FilePath.Replace('\\', '/');
#endif // !WIN32
					FilePath = CFileTools::MakeFullPath(FilePath);
					if (ScriptFile.LoadFile(FilePath, false))
					{
						m_ScriptExecutor.ExcuteScript(ScriptFile.GetData(), pServiceInfo->WorkDir, ServiceID);
					}
					else
					{
						MsgCaller.ExecuteScriptAck(MSG_RESULT_SCRIPT_FILE_NOT_EXIST, ServiceID, 0, 0);
					}
				}
				else
				{
					m_ScriptExecutor.ExcuteScript(Script, pServiceInfo->WorkDir, ServiceID);
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

	SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		CEasyString SearchDir;
		SearchDir.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, (LPCTSTR)Dir);
#ifndef WIN32
		SearchDir.Replace('\\','/');
#endif // !WIN32

		SearchDir = CFileTools::MakeFullPath(SearchDir);
		if (SearchDir.Find(pServiceInfo->WorkDir) < 0)
		{
			//要浏览的目录不在工作目录下，浏览工作目录
			SearchDir = pServiceInfo->WorkDir;
		}
		SearchDir.TrimRight(DIR_SLASH);

		bool IsWorkDir = false;
		if (SearchDir == pServiceInfo->WorkDir)
			IsWorkDir = true;

		CEasyString RealDir = CFileTools::GetRelativePath(pServiceInfo->WorkDir, SearchDir);

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
						//文件名统一使用UTF8，所以WIN平台需要转码
#ifdef WIN32
						pFileInfo->Name = LocalToUTF8(FileSearcher.GetFileName(),FileSearcher.GetFileName().GetLength());
#else
						pFileInfo->Name = FileSearcher.GetFileName();
#endif
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
int CServerManagerClient::FileDownloadStart(UINT ServiceID, const CEasyString& FilePath)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		SAFE_RELEASE(m_pCurDownloadFile);
		m_pCurDownloadFile = CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
		if (m_pCurDownloadFile)
		{
			CEasyString Path;
			Path.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
			Path = UTF8ToLocal(Path, Path.GetLength());
#else
			Path.Replace('\\', '/');
#endif // !WIN32
			Path = CFileTools::MakeFullPath(Path);
			if (m_pCurDownloadFile->Open(Path, IFileAccessor::modeOpen | IFileAccessor::modeRead | IFileAccessor::shareShareAll))
			{
				MsgCaller.FileDownloadStartAck(MSG_RESULT_SUCCEED, ServiceID, FilePath, m_pCurDownloadFile->GetSize());
			}
			else
			{
				SAFE_RELEASE(m_pCurDownloadFile);
				MsgCaller.FileDownloadStartAck(MSG_RESULT_FILE_NOT_EXIST, ServiceID, FilePath, 0);
			}
		}
		else
		{
			MsgCaller.FileDownloadStartAck(COMMON_RESULT_FAILED, ServiceID, FilePath, 0);
		}
	}
	else
	{
		MsgCaller.FileDownloadStartAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath, 0);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileDownloadData(UINT64 Offset, UINT Length)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	if (m_pCurDownloadFile)
	{
		if (Length <= m_FileTransferBuffer.GetBufferSize())
		{
			if(m_pCurDownloadFile->Seek(Offset, IFileAccessor::seekBegin))
			{
				UINT64 ReadLen = m_pCurDownloadFile->Read(m_FileTransferBuffer.GetBuffer(), Length);
				if (ReadLen)
				{
					size_t PackSize = m_CompressBuffer.GetBufferSize() - LZMA_PROPS_SIZE;
					size_t PackPropSize = LZMA_PROPS_SIZE;

					int ErrCode = LzmaCompress(((BYTE *)m_CompressBuffer.GetBuffer()) + LZMA_PROPS_SIZE, &PackSize,
						(BYTE *)m_FileTransferBuffer.GetBuffer(), ReadLen,
						(BYTE *)m_CompressBuffer.GetBuffer(), &PackPropSize, 5,
						1 << 24, 3, 0, 2, 32, 2);
					if (ErrCode == SZ_OK)
					{
						PackSize += LZMA_PROPS_SIZE;
						m_CompressBuffer.SetUsedSize(PackSize);

						MsgCaller.FileDownloadDataAck(MSG_RESULT_SUCCEED, Offset, ReadLen, m_CompressBuffer);
					}
					else
					{
						MsgCaller.FileDownloadDataAck(MSG_RESULT_DATA_COMPRESS_ERROR, Offset, 0, CEasyBuffer());
					}
				}
				else
				{
					MsgCaller.FileDownloadDataAck(MSG_RESULT_SUCCEED, Offset, 0, CEasyBuffer());
				}
			}
			else
			{
				MsgCaller.FileDownloadDataAck(MSG_RESULT_FILE_READ_ERROR, Offset, 0, CEasyBuffer());
			}
		}
		else
		{
			MsgCaller.FileDownloadDataAck(MSG_RESULT_QUERY_SIZE_TOO_BIG, Offset, 0, CEasyBuffer());
		}
	}
	else
	{
		
		MsgCaller.FileDownloadDataAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, Offset, 0, CEasyBuffer());
	}
	
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileDownloadEnd()
{
	CServerManagerAckMsgCaller MsgCaller(this);
	if (m_pCurDownloadFile)
	{
		CEasyTime LastWriteTime;
		m_pCurDownloadFile->GetLastWriteTime(LastWriteTime);
		MsgCaller.FileDownloadEndAck(MSG_RESULT_SUCCEED, (time_t)LastWriteTime);
		m_pCurDownloadFile->Close();
		SAFE_RELEASE(m_pCurDownloadFile);		
	}
	else
	{
		MsgCaller.FileDownloadEndAck(COMMON_RESULT_FAILED, 0);
	}	
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileUploadStart(UINT ServiceID, const CEasyString& FilePath)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		SAFE_RELEASE(m_pCurUploadFile);
		m_pCurUploadFile = CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
		if (m_pCurUploadFile)
		{
			CEasyString Path;
			Path.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, (LPCTSTR)FilePath);
#ifdef WIN32
			Path = UTF8ToLocal(Path, Path.GetLength());
#else
			Path.Replace('\\', '/');
#endif // !WIN32
			Path = CFileTools::MakeFullPath(Path);
			if (m_pCurUploadFile->Open(Path, IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite))
			{
				MsgCaller.FileUploadStartAck(MSG_RESULT_SUCCEED, ServiceID, FilePath, m_pCurUploadFile->GetSize());
			}
			else
			{
				SAFE_RELEASE(m_pCurUploadFile);
				MsgCaller.FileUploadStartAck(MSG_RESULT_FILE_NOT_EXIST, ServiceID, FilePath, 0);
			}
		}
		else
		{
			MsgCaller.FileUploadStartAck(COMMON_RESULT_FAILED, ServiceID, FilePath, 0);
		}
	}
	else
	{
		MsgCaller.FileUploadStartAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath, 0);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileUploadData(UINT64 Offset, UINT Length, const CEasyBuffer& FileData)
{	
	CServerManagerAckMsgCaller MsgCaller(this);
	if (m_pCurUploadFile)
	{
		if (Length <= m_CompressBuffer.GetBufferSize())
		{
			size_t UnpackSize = Length;
			size_t SourceSize = FileData.GetUsedSize() - LZMA_PROPS_SIZE;

			int ErrCode = LzmaUncompress((BYTE *)m_CompressBuffer.GetBuffer(), &UnpackSize,
				((BYTE *)FileData.GetBuffer()) + LZMA_PROPS_SIZE, &SourceSize,
				(BYTE *)FileData.GetBuffer(), LZMA_PROPS_SIZE);
			if (ErrCode == SZ_OK)
			{
				m_CompressBuffer.SetUsedSize(UnpackSize);
				if (m_pCurUploadFile->Seek(Offset, IFileAccessor::seekBegin))
				{
					UINT64 WriteSize = m_pCurUploadFile->Write(m_CompressBuffer.GetBuffer(), UnpackSize);
					if (WriteSize == UnpackSize)
					{
						MsgCaller.FileUploadDataAck(MSG_RESULT_SUCCEED, Offset, WriteSize);
					}
					else
					{
						MsgCaller.FileUploadDataAck(MSG_RESULT_FILE_WRITE_ERROR, Offset, 0);
					}
				}
				else
				{
					MsgCaller.FileUploadDataAck(MSG_RESULT_FILE_WRITE_ERROR, Offset, 0);
				}
			}
			else
			{
				MsgCaller.FileUploadDataAck(MSG_RESULT_DATA_COMPRESS_ERROR, Offset, 0);
			}
		}
		else
		{
			MsgCaller.FileUploadDataAck(MSG_RESULT_QUERY_SIZE_TOO_BIG, Offset, 0);
		}
	}
	else
	{

		MsgCaller.FileUploadDataAck(MSG_RESULT_QUERY_SEQUENCE_ERROR, Offset, 0);
	}

	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::FileUploadEnd(UINT FileLastWriteTime)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	if (m_pCurUploadFile)
	{
		m_pCurUploadFile->SetLastWriteTime((time_t)FileLastWriteTime);
		m_pCurUploadFile->Close();
		SAFE_RELEASE(m_pCurUploadFile);
		MsgCaller.FileUploadEndAck(MSG_RESULT_SUCCEED);
	}
	else
	{
		MsgCaller.FileUploadEndAck(COMMON_RESULT_FAILED);
	}
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::CreateDir(UINT ServiceID, const CEasyString& Dir)
{
	CServerManagerAckMsgCaller MsgCaller(this);

	SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{
		
		CEasyString Path;
		Path.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, (LPCTSTR)Dir);
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
			SAFE_RELEASE(m_pCurDownloadFile);
			MsgCaller.CreateDirAck(COMMON_RESULT_FAILED, ServiceID, Dir);
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

	SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{

		CEasyString Path;
		Path.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, (LPCTSTR)FilePath);
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
			MsgCaller.DeleteFileAck(COMMON_RESULT_FAILED, ServiceID, FilePath);
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
	MsgCaller.ChangeFileModeAck(COMMON_RESULT_FAILED, ServiceID, FilePath, Mode);
#else
	SERVICE_INFO * pServiceInfo = m_pManager->GetServiceInfo(ServiceID);
	if (pServiceInfo)
	{

		CEasyString Path;
		Path.Format("%s%c%s", (LPCTSTR)pServiceInfo->WorkDir, DIR_SLASH, FilePath);
		Path.Replace('\\', '/');
		Path = CFileTools::MakeFullPath(Path);
		if (chmod(Path, Mode))
		{
			MsgCaller.ChangeFileModeAck(MSG_RESULT_SUCCEED, ServiceID, FilePath, Mode);
		}
		else
		{
			SAFE_RELEASE(m_pCurDownloadFile);
			MsgCaller.ChangeFileModeAck(COMMON_RESULT_FAILED, ServiceID, FilePath, Mode);
		}

	}
	else
	{
		MsgCaller.ChangeFileModeAck(MSG_RESULT_SERVICE_NOT_EXIST, ServiceID, FilePath, Mode);
	}
#endif
	return COMMON_RESULT_SUCCEED;
}


int CServerManagerClient::AddService(const SERVICE_INFO& ServiceInfo)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	int Result = m_pManager->AddService(ServiceInfo);
	MsgCaller.AddServiceAck(Result);
	return COMMON_RESULT_SUCCEED;
}
int CServerManagerClient::EditService(const SERVICE_INFO& ServiceInfo)
{
	CServerManagerAckMsgCaller MsgCaller(this);
	int Result = m_pManager->EditService(ServiceInfo);
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