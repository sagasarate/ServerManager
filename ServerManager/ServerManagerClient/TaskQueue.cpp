#include "stdafx.h"



CTaskQueue::CTaskQueue()
{
	m_pView = NULL;
	m_pConnection = NULL;
	m_IsInTransfer = false;
}


CTaskQueue::~CTaskQueue()
{
}


bool CTaskQueue::Init(CServerManagerClientView * pView, CServerConnection * pConnection)
{
	m_pView = pView;
	m_pConnection = pConnection;
	m_TaskQueue.Create(64, 64, 64);
	m_FileDataPool.Create(32, 32, 128);
	m_WorkThreadList.Clear();
	return true;
}

CWorkThread * CTaskQueue::AllocWorkThread()
{
	if (m_WorkThreadList.GetCount() == 0)
	{
		m_WorkThreadList.Resize(6);
		for (CWorkThread& Thread : m_WorkThreadList)
		{
			Thread.Init(this);
			Thread.Start();
		}
	}
	for (CWorkThread& Thread : m_WorkThreadList)
	{
		if (Thread.IsIdle())
		{
			return &Thread;
			break;
		}
	}

	return NULL;
}
UINT CTaskQueue::GetFreeThreadCount()
{
	UINT Count = 0;
	for (CWorkThread& Thread : m_WorkThreadList)
	{
		if (Thread.IsIdle())
		{
			Count++;
		}
	}
	return Count;
}
bool CTaskQueue::IsAllWorkThreadIdle()
{
	for (CWorkThread& Thread : m_WorkThreadList)
	{
		if (!Thread.IsIdle())
			return false;
	}
	return true;
}

UINT CTaskQueue::AddDownloadTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_DOWNLOAD;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		pTaskInfo->SourceFilePath = SourceFilePath;
		pTaskInfo->TargetFilePath = TargetFilePath;
		pTaskInfo->FileSize = 0;
		pTaskInfo->TransferOffset = 0;
		pTaskInfo->ContinueTransfer = ContinueTransfer;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}
UINT CTaskQueue::AddUploadTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer)
{
	CFileInfo SourceFile;
	if(SourceFile.FetchFileInfo(SourceFilePath))
	{
		TASK_INFO* pTaskInfo = NULL;
		UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
		if (pTaskInfo)
		{
			pTaskInfo->Clear();
			pTaskInfo->ID = ID;
			pTaskInfo->Type = TASK_TYPE_UPLOAD;
			pTaskInfo->Status = TASK_STATUS_NONE;
			pTaskInfo->ServiceID = ServiceID;
			pTaskInfo->SourceFilePath = SourceFilePath;
			pTaskInfo->FileLastWriteTime = SourceFile.GetLastWriteTime();
			pTaskInfo->TargetFilePath = TargetFilePath;
			pTaskInfo->FileSize = 0;
			pTaskInfo->TransferOffset = 0;
			pTaskInfo->ContinueTransfer = ContinueTransfer;

			if (m_pView)
				m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);

			return ID;
		}
	}
	else
	{
		PrintLog(-1, "文件%s无法打开", SourceFilePath);
	}
	return 0;
}
UINT CTaskQueue::AddDeleteFileTask(UINT ServiceID, LPCTSTR TargetFilePath)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_DELETE_FILE;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		pTaskInfo->TargetFilePath = TargetFilePath;
		pTaskInfo->FileSize = 0;
		pTaskInfo->TransferOffset = 0;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}
UINT CTaskQueue::AddCreateDirTask(UINT ServiceID, LPCTSTR TargetFilePath)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_CREATE_DIR;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		pTaskInfo->TargetFilePath = TargetFilePath;
		pTaskInfo->FileSize = 0;
		pTaskInfo->TransferOffset = 0;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}
UINT CTaskQueue::AddChangeFileTask(UINT ServiceID, LPCTSTR TargetFilePath, UINT FileMode)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_CHANGE_FILE_MODE;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		pTaskInfo->TargetFilePath = TargetFilePath;
		pTaskInfo->FileMode = FileMode;
		pTaskInfo->FileSize = 0;
		pTaskInfo->TransferOffset = 0;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}
UINT CTaskQueue::AddStartupServiceTask(UINT ServiceID, bool ClearQueueOnFailed)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_STARTUP_SERVICE;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		pTaskInfo->ContinueTransfer = ClearQueueOnFailed;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}
UINT CTaskQueue::AddShutdownServiceTask(UINT ServiceID, bool ClearQueueOnFailed)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_SHUTDOWN_SERVICE;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		pTaskInfo->ContinueTransfer = ClearQueueOnFailed;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}
UINT CTaskQueue::AddReloadConfigDataTask(UINT ServiceID)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_RELOAD_CONFIG_DATA;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}

UINT CTaskQueue::AddFileCompareTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, UINT Usage)
{
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_COMPARE;
		pTaskInfo->Status = TASK_STATUS_NONE;
		pTaskInfo->ServiceID = ServiceID;
		pTaskInfo->SourceFilePath = SourceFilePath;
		pTaskInfo->TargetFilePath = TargetFilePath;
		pTaskInfo->FileSize = 0;
		pTaskInfo->TransferOffset = 0;
		pTaskInfo->Usage = Usage;
		if (m_pView)
			m_pView->OnAddTask(m_pConnection->GetID(), *pTaskInfo);
		return ID;
	}
	return 0;
}

bool CTaskQueue::CancelFileTransfer(UINT ID)
{
	TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(ID);
	if (pTaskInfo)
	{
		DeleteTask(pTaskInfo);
		return true;
	}		
	return false;
}
void CTaskQueue::DeleteAllTask()
{
	for (CWorkThread& Thread : m_WorkThreadList)
	{
		Thread.ClearWork();
	}
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	while (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetNextObject(Pos);
		DeleteTask(pTaskInfo);
	}
}
void CTaskQueue::Update()
{
	UINT CurTime = CEasyTimer::GetTime();
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		m_IsInTransfer = true;
		switch (pTaskInfo->Status)
		{
		case TASK_STATUS_NONE:
			{
				if (m_pConnection->IsConnected())
				{
					switch (pTaskInfo->Type)
					{
					case TASK_TYPE_DOWNLOAD:
						if(IsAllWorkThreadIdle())
						{
							m_pConnection->QueryStartDownload(pTaskInfo->ServiceID, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
							pTaskInfo->Status = TASK_STATUS_PROCESSING;
						}
						else
						{
							DoSleep(1);
						}
						break;
					case TASK_TYPE_UPLOAD:
						if (IsAllWorkThreadIdle())
						{
							m_pConnection->QueryStartUpload(pTaskInfo->ServiceID, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath, pTaskInfo->FileLastWriteTime);
							pTaskInfo->Status = TASK_STATUS_PROCESSING;
						}
						else
						{
							DoSleep(1);
						}
						break;
					case TASK_TYPE_DELETE_FILE:
						m_pConnection->QueryDeleteFile(pTaskInfo->ServiceID, pTaskInfo->TargetFilePath);
						pTaskInfo->Status = TASK_STATUS_PROCESSING;
						break;
					case TASK_TYPE_CREATE_DIR:
						m_pConnection->QueryCreateDir(pTaskInfo->ServiceID, pTaskInfo->TargetFilePath);
						pTaskInfo->Status = TASK_STATUS_PROCESSING;
						break;
					case TASK_TYPE_CHANGE_FILE_MODE:
						m_pConnection->QueryChangeFileMode(pTaskInfo->ServiceID, pTaskInfo->TargetFilePath, pTaskInfo->FileMode);
						pTaskInfo->Status = TASK_STATUS_PROCESSING;
						break;
					case TASK_TYPE_STARTUP_SERVICE:
						m_pConnection->QueryStartupService(pTaskInfo->ServiceID);
						pTaskInfo->Status = TASK_STATUS_PROCESSING;
						break;
					case TASK_TYPE_SHUTDOWN_SERVICE:
						m_pConnection->QueryShutDownService(pTaskInfo->ServiceID, SERVICE_SHUTDOWN_TYPE_SAFE);
						pTaskInfo->Status = TASK_STATUS_PROCESSING;
						break;
					case TASK_TYPE_RELOAD_CONFIG_DATA:
						m_pConnection->QuerySendCommand(pTaskInfo->ServiceID, _T("ReloadAllData()"));
						pTaskInfo->Status = TASK_STATUS_PROCESSING;
						break;
					case TASK_TYPE_COMPARE:
						if (IsAllWorkThreadIdle())
						{
							pTaskInfo->Status = TASK_STATUS_PROCESSING;
							if (pTaskInfo->TaskFile.Open(pTaskInfo->SourceFilePath, IFileAccessor::modeOpen | IFileAccessor::modeRead | IFileAccessor::shareShareAll))
							{
								pTaskInfo->FileSize = pTaskInfo->TaskFile.GetSize();

								pTaskInfo->Status = TASK_STATUS_TRANSFER_DATA;
								

							}
							else
							{
								PrintLog(-1, _T("OnUploadStartResult打开文件失败%s"), (LPCTSTR)pTaskInfo->TargetFilePath);
								DeleteTask(pTaskInfo);
							}
						}
						else
						{
							DoSleep(1);
						}
						break;
					default:
						{							
							PrintLog(-1, _T("未知的任务类型%d"), pTaskInfo->Type);
							DeleteTask(pTaskInfo);							
						}
						break;
					}					
				}
				else
				{					
					PrintLog(-1, _T("服务器[%u]未连接，任务放弃"), (LPCTSTR)m_pConnection->GetServerAddress());
					DeleteTask(pTaskInfo);
				}
			}
			break;		
		case TASK_STATUS_TRANSFER_DATA:			
			{
				if (m_pConnection->IsConnected())
				{
					switch (pTaskInfo->Type)
					{
					case TASK_TYPE_DOWNLOAD:
						SendDownloadQuery(pTaskInfo, GetFreeThreadCount());
						break;
					case TASK_TYPE_UPLOAD:
						if (pTaskInfo->QueryCount == 0)
						{
							FILE_DATA_INFO * pFileData = GetFinishedFileData(pTaskInfo->CurBlockIndex);
							if (pFileData)
							{
								//PrintLog(0, _T("上传数据块%u,Size=%u,OrginSize=%u"), pFileData->Index, pFileData->DataBuffer.GetUsedSize(), pFileData->OriginSize);
								pTaskInfo->CurBlockIndex++;
								m_pConnection->QueryUploadData(pFileData->OriginSize, pFileData->DataBuffer, pFileData->IsLast);
								pTaskInfo->QueryCount++;
								pTaskInfo->TransferSize += pFileData->DataBuffer.GetUsedSize();
								pTaskInfo->TransferOffset += pFileData->OriginSize;
								DeleteFileDataInfo(pFileData);
							}
						}
						break;					
					}
				}
			}
			break;
		case TASK_STATUS_WAIT_SERVICE_STATUS:
			{
				switch (pTaskInfo->Type)
				{
				case TASK_TYPE_STARTUP_SERVICE:
				case TASK_TYPE_SHUTDOWN_SERVICE:
					if (CEasyTimer::GetTimeToTime(pTaskInfo->TransferStartTime, CurTime) >= SERVICE_STARTUP_TIMEOUT)
					{
						PrintLog(-1, _T("任务超时%d"), pTaskInfo->Type);
						DeleteTask(pTaskInfo);
					}
					break;
				}
			}
			break;
		case TASK_STATUS_END:
			{
				if (m_pConnection->IsConnected())
				{
					switch (pTaskInfo->Type)
					{
					case TASK_TYPE_DOWNLOAD:
						m_pConnection->QueryDownloadFinish();
						break;					
					case TASK_TYPE_COMPARE:
						m_pConnection->QueryFileCompare(pTaskInfo->ServiceID, pTaskInfo->TargetFilePath, pTaskInfo->FileSize, pTaskInfo->FileMD5);
						break;					
					}
					pTaskInfo->Status = TASK_STATUS_PROCESSING;
				}
			}
			break;
		case TASK_STATUS_ERROR:
			{
				if (m_pConnection->IsConnected())
				{
					switch (pTaskInfo->Type)
					{
					case TASK_TYPE_DOWNLOAD:
						StopDownload(pTaskInfo);
						break;
					case TASK_TYPE_UPLOAD:
						StopUpload(pTaskInfo);
						break;					
					default:
						PrintLog(-1, _T("不支持的任务类型%d"), pTaskInfo->Type);
						DeleteTask(pTaskInfo);
						break;
					}
					pTaskInfo->Status = TASK_STATUS_PROCESSING;
				}
			}
			break;
		}
	}
	else if (m_IsInTransfer)
	{
		m_IsInTransfer = false;
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnFileTaskEmpty();
	}
}
//BOOL CTaskQueue::OnRun()
//{
//	switch (m_CompressWorkStatus)
//	{
//	case COMPRESS_WORK_STATUS_SAVE:
//		{
//			COMPRESS_WORK_INFO * pWorkInfo = NULL;
//			{
//				CAutoLock Lock(m_WorkQueueLock);
//
//				for (COMPRESS_WORK_INFO& FileData : m_CompressQueue)
//				{
//					if ((FileData.Status == COMPRESS_WORK_STATUS_FINISH) && (FileData.Index == m_CurFinishIndex))
//					{
//						pWorkInfo = &FileData;
//						m_CurFinishIndex++;
//						break;
//					}
//				}
//			}
//
//			if (pWorkInfo)
//			{
//				UINT64 WriteLen = m_CurTransferFile.Write(pWorkInfo->DataBuffer.GetBuffer(), pWorkInfo->DataBuffer.GetUsedSize());
//				if (WriteLen == pWorkInfo->DataBuffer.GetUsedSize())
//				{
//					DeleteWorkInfo(pWorkInfo);
//					m_CompressWorkFinishSize += WriteLen;
//					if (m_CompressWorkFinishSize >= m_CompressWorkTargetSize)
//					{
//						m_CompressWorkStatus = COMPRESS_WORK_STATUS_FINISH;
//					}
//				}
//				else
//				{
//					m_CompressWorkStatus = COMPRESS_WORK_STATUS_ERROR;
//				}
//			}
//			else
//			{
//				DoSleep(1);
//			}
//		}
//		break;
//	case COMPRESS_WORK_STATUS_LOAD:
//		{
//			CWorkThread * pThread = AllocWorkThread();		
//
//			if (pThread)
//			{
//				COMPRESS_WORK_INFO * pWorkInfo = AllocWorkInfo(COMPRESS_WORK_STATUS_COMPRESS);				
//
//				if (pWorkInfo)
//				{
//					UINT64 ReadLen = m_CurTransferFile.Read(pWorkInfo->DataBuffer.GetBuffer(), FILE_TRANSFER_BLOCK_SIZE);
//
//					if (ReadLen)
//					{
//						pWorkInfo->DataBuffer.SetUsedSize(ReadLen);
//						pWorkInfo->OriginSize = ReadLen;
//						pWorkInfo->Index = m_CurWorkIndex;
//						m_CurWorkIndex++;
//						pThread->AddWork(pWorkInfo);
//					}
//
//					if (ReadLen < FILE_TRANSFER_BLOCK_SIZE)
//					{
//						m_CompressWorkStatus = COMPRESS_WORK_STATUS_FINISH;
//					}
//				}
//				else
//				{
//					m_CompressWorkStatus = COMPRESS_WORK_STATUS_ERROR;
//				}
//			}
//			else
//			{
//				DoSleep(1);
//			}
//			
//		}		
//		break;
//	case COMPRESS_WORK_STATUS_MD5:
//		{
//			CHashMD5 HashMD5;
//			CEasyBuffer ReadBuffer;
//			ReadBuffer.Create(FILE_TRANSFER_BLOCK_SIZE);
//			HashMD5.Init();
//			UINT64 ReadLen = 0;
//			do {
//				ReadLen = m_CurTransferFile.Read(ReadBuffer.GetBuffer(), FILE_TRANSFER_BLOCK_SIZE);
//				HashMD5.AddData((BYTE *)ReadBuffer.GetBuffer(), ReadLen);
//			} while (ReadLen == FILE_TRANSFER_BLOCK_SIZE);
//			HashMD5.MD5Final();
//			m_MD5Str = HashMD5.GetHashCodeString();
//
//			m_CompressWorkStatus = COMPRESS_WORK_STATUS_FINISH;
//		}
//		break;
//	default:
//		DoSleep(1);
//	}
//	return TRUE;
//}

FILE_DATA_INFO * CTaskQueue::AllocFileDataInfo(UINT BufferSize)
{
	FILE_DATA_INFO * pFileData = m_FileDataPool.NewObject();
	if (pFileData)
	{
		if (pFileData->DataBuffer.GetBufferSize() < BufferSize)
			pFileData->DataBuffer.Create(BufferSize);
		pFileData->DataBuffer.SetUsedSize(0);
	}
	return pFileData;
}
void CTaskQueue::DeleteFileDataInfo(FILE_DATA_INFO * pInfo)
{
	m_FileDataPool.DeleteObject(pInfo->DataID);
}

bool CTaskQueue::OnWorkFinish(FILE_DATA_INFO * pData)
{
	CAutoLock Lock(m_FinishedWorkListLock);

	m_FinishedWorkList.Add(pData);

	return true;
}

FILE_DATA_INFO * CTaskQueue::GetFinishedFileData(UINT Index)
{
	CAutoLock Lock(m_FinishedWorkListLock);

	for (UINT i = 0; i < m_FinishedWorkList.GetCount(); i++)
	{
		if (m_FinishedWorkList[i]->Index == Index)
		{
			FILE_DATA_INFO * pInfo = m_FinishedWorkList[i];
			m_FinishedWorkList.Delete(i);
			return pInfo;
		}
	}
	return NULL;
}

void CTaskQueue::OnTaskFinish(TASK_INFO * pTaskInfo)
{
	Log("任务处理完毕");
	pTaskInfo->Status = TASK_STATUS_END;
}
void CTaskQueue::OnTaskError(TASK_INFO * pTaskInfo)
{
	Log("任务出现错误");
	pTaskInfo->Status = TASK_STATUS_ERROR;
	for (CWorkThread& Thread : m_WorkThreadList)
	{
		Thread.ClearWork();
	}
}

void CTaskQueue::OnStartDownloadResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize, UINT FileLastWriteTime)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	TASK_INFO * pTaskInfo = NULL;
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);
	
	if (pTaskInfo)
	{
		if (pTaskInfo->ServiceID == ServiceID)
		{
			if (Result == MSG_RESULT_SUCCEED)
			{
				if (pTaskInfo->TaskFile.Open(pTaskInfo->TargetFilePath, IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite))
				{
					pTaskInfo->FileSize = FileSize;
					if (pTaskInfo->ContinueTransfer)
					{
						pTaskInfo->TransferOffset = pTaskInfo->TaskFile.GetSize();
						pTaskInfo->TaskFile.Seek(0, IFileAccessor::seekEnd);
					}						
					else
					{
						pTaskInfo->TransferOffset = 0;
					}
					pTaskInfo->QueryOffset = pTaskInfo->TransferOffset;
					pTaskInfo->FinishSize = pTaskInfo->TransferOffset;
					pTaskInfo->FileLastWriteTime = FileLastWriteTime;
					pTaskInfo->TransferSize = 0;
					pTaskInfo->TransferStartTime = CEasyTimer::GetTime();
					pTaskInfo->CurBlockIndex = 0;
					pTaskInfo->Status = TASK_STATUS_TRANSFER_DATA;

					CWorkThread * pThread = AllocWorkThread();
					if (pThread)
					{
						pThread->AddTask(pTaskInfo);			
						SendDownloadQuery(pTaskInfo, GetFreeThreadCount());
					}
					else
					{
						PrintLog(-1, _T("OnStartDownloadResult无可用工作线程"));
						OnTaskError(pTaskInfo);
					}
				}
				else
				{
					PrintLog(-1, _T("OnStartDownloadResult创建文件失败%s"), (LPCTSTR)pTaskInfo->TargetFilePath);
					OnTaskError(pTaskInfo);
				}
			}
			else
			{
				PrintLog(-1, _T("OnStartDownloadResult请求失败%s"), GetResultStr(Result));
				OnTaskError(pTaskInfo);
			}
		}
		else
		{
			PrintLog(-1, _T("OnStartDownloadResult传输任务不相符"));
			OnTaskError(pTaskInfo);
		}
	}
	else
	{
		PrintLog(-1, _T("OnStartDownloadResult已无传输任务"));
		OnTaskError(pTaskInfo);
	}	
}
void CTaskQueue::OnDownloadData(short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData, bool IsLast)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);

		if (Result == MSG_RESULT_SUCCEED)
		{			
			pTaskInfo->TransferSize += FileData.GetUsedSize();
			pTaskInfo->TransferOffset += Length;
			if (m_pView)
			{
				float Progress = 0;
				if (pTaskInfo->FileSize)
					Progress = (float)pTaskInfo->TransferOffset / pTaskInfo->FileSize;
				m_pView->OnTaskUpdate(m_pConnection->GetID(), pTaskInfo->ID, Progress);
			}

			CWorkThread * pThread = AllocWorkThread();			

			if (pThread)
			{
				FILE_DATA_INFO * pFileData = AllocFileDataInfo(FileData.GetUsedSize());
				if (pFileData)
				{
					pFileData->DataBuffer.PushBack(FileData.GetBuffer(), FileData.GetUsedSize());
					pFileData->OriginSize = Length;
					pFileData->Index = pTaskInfo->CurBlockIndex++;
					pFileData->IsLast = IsLast;
					pThread->AddFileWork(pTaskInfo, pFileData, false);							
				}
				else
				{
					PrintLog(-1, _T("OnDownloadData异常，无数据缓冲可分配"));
					OnTaskError(pTaskInfo);
				}
			}
			else
			{
				PrintLog(-1, _T("OnDownloadData异常，无工作线程可分配"));
				OnTaskError(pTaskInfo);
			}	
		}
		else if (Result == MSG_RESULT_IS_BUSY)
		{
			PrintLog(0, _T("OnDownloadData服务器忙"));
		}
		else
		{
			PrintLog(-1, _T("OnDownloadData请求失败%s"), GetResultStr(Result));
			OnTaskError(pTaskInfo);
		}
	}
	else
	{
		PrintLog(-1, _T("OnDownloadData已无传输任务"));
	}
	
}

void CTaskQueue::OnDownloadFinish(short Result, const CEasyString& MD5)
{
	void* Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO* pTaskInfo = m_TaskQueue.GetObject(Pos);
		EndDownload(pTaskInfo, Result, MD5);
	}
	else
	{
		PrintLog(-1, _T("OnDownloadFinish已无传输任务"));
	}
}

void CTaskQueue::OnUploadStartResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	TASK_INFO * pTaskInfo = NULL;
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);
	
	if (pTaskInfo)
	{
		if (pTaskInfo->ServiceID == ServiceID)
		{
			if (Result == MSG_RESULT_SUCCEED)
			{
				if (pTaskInfo->TaskFile.Open(pTaskInfo->SourceFilePath, IFileAccessor::modeOpen | IFileAccessor::modeRead | IFileAccessor::shareShareAll))
				{
					pTaskInfo->FileSize = pTaskInfo->TaskFile.GetSize();
					if(pTaskInfo->ContinueTransfer)
						pTaskInfo->TransferOffset = FileSize;
					else
						pTaskInfo->TransferOffset = 0;
					pTaskInfo->TransferSize = 0;
					pTaskInfo->TransferStartTime = CEasyTimer::GetTime();
					
					if (pTaskInfo->TaskFile.Seek(pTaskInfo->TransferOffset, IFileAccessor::seekBegin))
					{
						pTaskInfo->Status = TASK_STATUS_TRANSFER_DATA;

						CWorkThread * pThread = AllocWorkThread();
						if (pThread)
						{
							PrintLog(0, _T("开始传输文件%s,Size=%llu,StartOffset=%llu"),
								(LPCTSTR)pTaskInfo->SourceFilePath, pTaskInfo->FileSize, pTaskInfo->TransferSize);
							pThread->AddTask(pTaskInfo);
						}
						else
						{
							PrintLog(-1, _T("OnUploadStartResult无可用工作线程"));
							OnTaskError(pTaskInfo);
						}
					}
					else
					{
						PrintLog(-1, _T("OnUploadStartResult文件定位失败"));
						OnTaskError(pTaskInfo);
					}
				}
				else
				{
					PrintLog(-1, _T("OnUploadStartResult打开文件失败%s"), (LPCTSTR)pTaskInfo->TargetFilePath);
					OnTaskError(pTaskInfo);
				}
			}
			else
			{
				PrintLog(-1, _T("OnUploadStartResult请求失败%s"), GetResultStr(Result));
				DeleteTask(pTaskInfo);
			}
		}
		else
		{
			PrintLog(-1, _T("OnUploadStartResult传输任务不相符"));
			OnTaskError(pTaskInfo);
		}
	}
	else
	{
		PrintLog(-1, _T("OnUploadStartResult已无传输任务"));
	}
	
}
void CTaskQueue::OnUploadData(short Result, UINT Length, bool IsLast)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		pTaskInfo->QueryCount--;
		if (Result == MSG_RESULT_SUCCEED)
		{
			if (m_pView)
			{
				float Progress = 0;
				if (pTaskInfo->FileSize)
					Progress = (float)pTaskInfo->TransferOffset / pTaskInfo->FileSize;
				m_pView->OnTaskUpdate(m_pConnection->GetID(), pTaskInfo->ID, Progress);
			}


			if (IsLast)
			{
				m_pConnection->QueryUploadFinish();

				
			}

		}
		else
		{
			PrintLog(-1, _T("OnUploadData请求失败%s"), GetResultStr(Result));
			OnTaskError(pTaskInfo);
		}
	}
	else
	{
		PrintLog(-1, _T("OnUploadData已无传输任务"));
	}
	
}

void CTaskQueue::OnUploadFinish(short Result, const CEasyString& MD5)
{
	void* Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO* pTaskInfo = m_TaskQueue.GetObject(Pos);
		EndUpload(pTaskInfo, Result, MD5);
	}
	else
	{
		PrintLog(-1, _T("OnDownloadFinish已无传输任务"));
	}
}

void CTaskQueue::OnDeleteFileResult(short Result, UINT ServiceID, LPCTSTR FilePath)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(-1, _T("OnDeleteFileResult请求失败%d"), Result);
	}

	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnDeleteFileFinish(pTaskInfo->TargetFilePath);
		DeleteTask(pTaskInfo);
	}
}

void CTaskQueue::OnCreateDirResult(short Result, UINT ServiceID, LPCTSTR Dir)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(-1, _T("OnCreateDirResult请求失败%d"), Result);
	}

	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnCreateDirFinish(pTaskInfo->TargetFilePath);
		DeleteTask(pTaskInfo);
	}
}
void CTaskQueue::OnChangeFileModeResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT Mode)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(-1, _T("OnChangeFileModeResult请求失败%s"), GetResultStr(Result));
	}

	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		DeleteTask(pTaskInfo);
	}
}
void CTaskQueue::OnServiceStartupResult(short Result, UINT ServiceID)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	TASK_INFO * pTaskInfo = NULL;
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);

	if (pTaskInfo)
	{
		if ((pTaskInfo->Type == TASK_TYPE_STARTUP_SERVICE) && (pTaskInfo->ServiceID == ServiceID))
		{
			if (Result == MSG_RESULT_SUCCEED)
			{
				pTaskInfo->Status = TASK_STATUS_WAIT_SERVICE_STATUS;
				pTaskInfo->TransferStartTime = CEasyTimer::GetTime();
			}
			else
			{
				PrintLog(-1, _T("OnServiceStartupResult请求失败%s"), GetResultStr(Result));
				if (pTaskInfo->ContinueTransfer)
					DeleteAllTask();
				else
					DeleteTask(pTaskInfo);
			}
		}
		else
		{
			PrintLog(-1, _T("OnServiceStartupResult任务不相符"));
		}
	}	
}
void CTaskQueue::OnServiceShutdownResult(short Result, UINT ServiceID)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	TASK_INFO * pTaskInfo = NULL;
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);

	if (pTaskInfo)
	{
		if ((pTaskInfo->Type == TASK_TYPE_SHUTDOWN_SERVICE) && (pTaskInfo->ServiceID == ServiceID))
		{
			if (Result == MSG_RESULT_SUCCEED)
			{
				pTaskInfo->Status = TASK_STATUS_WAIT_SERVICE_STATUS;
				pTaskInfo->TransferStartTime = CEasyTimer::GetTime();
			}
			else
			{
				PrintLog(-1, _T("OnServiceShutdownResult请求失败%s"), GetResultStr(Result));
				if (pTaskInfo->ContinueTransfer)
					DeleteAllTask();
				else
					DeleteTask(pTaskInfo);
			}
		}
		else
		{
			PrintLog(-1, _T("OnServiceShutdownResult任务不相符"));
		}
	}
}
void CTaskQueue::OnSendCommandResult(short Result, UINT ServiceID)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	TASK_INFO * pTaskInfo = NULL;
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);

	if (pTaskInfo)
	{
		if ((pTaskInfo->Type == TASK_TYPE_RELOAD_CONFIG_DATA) && (pTaskInfo->ServiceID == ServiceID))
		{
			if (Result == MSG_RESULT_SUCCEED)
			{
				DeleteTask(pTaskInfo);
			}
			else
			{
				PrintLog(-1, _T("OnSendCommandResult请求失败%s"), GetResultStr(Result));
				if (pTaskInfo->ContinueTransfer)
					DeleteAllTask();
				else
					DeleteTask(pTaskInfo);
			}
		}
		else
		{
			PrintLog(-1, _T("OnSendCommandResult任务不相符"));
		}
	}
}
void CTaskQueue::OnServiceInfo(CServiceInfo& ServiceInfo)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	TASK_INFO * pTaskInfo = NULL;
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);

	if (pTaskInfo)
	{
		if (pTaskInfo->ServiceID == ServiceInfo.GetServiceID())
		{
			switch (pTaskInfo->Type)
			{
			case TASK_TYPE_STARTUP_SERVICE:
				if (ServiceInfo.GetWorkStatus() == 2)
				{
					DeleteTask(pTaskInfo);
				}
				break;
			case TASK_TYPE_SHUTDOWN_SERVICE:
				if (ServiceInfo.GetStatus() == SERVICE_STATUS_STOP)
				{
					DeleteTask(pTaskInfo);
				}
				break;
			}
		}
	}
}
void CTaskQueue::OnFileCompareResult(short Result, UINT ServiceID, LPCTSTR FilePath)
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);		
		if (pTaskInfo->Usage == TASK_USAGE_CHECK_BEFORE_UPDATE)
		{
			if (Result != MSG_RESULT_SUCCEED)
			{
				PrintLog(0, _T("%s文件比较结果不相同，需要上传"), (LPCTSTR)pTaskInfo->TargetFilePath);
				AddUploadTask(ServiceID, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath, false);
				AddFileCompareTask(ServiceID, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath, TASK_USAGE_CHECK_AFTER_UPDATE);
			}
		}
		else if (pTaskInfo->Usage == TASK_USAGE_CHECK_AFTER_UPDATE)
		{
			if (Result != MSG_RESULT_SUCCEED)
			{
				PrintLog(-1, _T("%s文件比较结果不相同，更新没有成功"), (LPCTSTR)pTaskInfo->TargetFilePath);
			}
		}
		else
		{
			if (Result != MSG_RESULT_SUCCEED)
			{
				PrintLog(-1, _T("%s文件比较结果不相同，更新没有成功"), (LPCTSTR)pTaskInfo->TargetFilePath);
			}
			if (m_pView)
				m_pView->GetWorkDirBrowser()->OnFileCompareFinish(Result, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
		}
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnFileCompareFinish(Result, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
		DeleteTask(pTaskInfo);
	}
}
void CTaskQueue::DeleteTask(TASK_INFO * pTaskInfo)
{		
	pTaskInfo->TaskFile.Close();
	if (m_pView)
		m_pView->OnDeleteTask(m_pConnection->GetID(), pTaskInfo->ID);
	m_TaskQueue.DeleteObject(pTaskInfo->ID);
}
void CTaskQueue::PrintLog(short Result, LPCTSTR szFormat, ...)
{
	if (m_pView)
	{
		va_list vl;

		va_start(vl, szFormat);
		m_pView->PrintLogVL((Result == MSG_RESULT_SUCCEED ? LOG_TYPE_NORMAL : LOG_TYPE_ERROR), szFormat, vl);
		va_end(vl);
	}
}

void CTaskQueue::SendDownloadQuery(TASK_INFO* pTaskInfo, UINT Count)
{
	while (Count && pTaskInfo->QueryCount < MAX_DOWNLOAD_QUERY)
	{
		m_pConnection->QueryDownloadData();
		pTaskInfo->IncQueryCount();
		Count--;
	}
}
void CTaskQueue::StopDownload(TASK_INFO* pTaskInfo)
{
	m_pConnection->QueryDownloadFinish();
	PrintLog(-1, _T("下载失败"));
	//if (m_pView)
	//	m_pView->GetWorkDirBrowser()->OnUploadFinish(pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
	//DeleteTask(pTaskInfo);
}
void CTaskQueue::EndDownload(TASK_INFO * pTaskInfo, int Result, const CEasyString& MD5)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		pTaskInfo->TaskFile.SetLastWriteTime(pTaskInfo->FileLastWriteTime);
		pTaskInfo->TaskFile.Close();

		float CompressRate = pTaskInfo->FileSize ? (float)pTaskInfo->TransferSize*100.0f / (float)pTaskInfo->FileSize : 0.0f;
		UINT TransferTime = CEasyTimer::GetTimeToTime(pTaskInfo->TransferStartTime, CEasyTimer::GetTime());
		float TransferRate = TransferTime ? (float)pTaskInfo->TransferSize*1000.0f / (float)TransferTime : 0.0f;

		if (pTaskInfo->FileMD5.CompareNoCase(MD5) == 0)
		{
			PrintLog(0, _T("下载完成，实际传输%s,压缩率%0.2f%%,下载速度%s"),
				(LPCTSTR)FormatNumberWords(pTaskInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate));
		}
		else
		{
			PrintLog(-1, _T("下载完成，实际传输%s,压缩率%0.2f%%,下载速度%s,MD5校验失败Local=%s,Server=%s"),
				(LPCTSTR)FormatNumberWords(pTaskInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate),
				(LPCTSTR)pTaskInfo->FileMD5,
				(LPCTSTR)MD5);
		}
	}
	else
	{
		pTaskInfo->TaskFile.Close();
		PrintLog(-1, _T("完成下载失败%s"), GetResultStr(Result));
	}

	if (m_pView)
		m_pView->GetWorkDirBrowser()->OnDownloadFinish(pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
	DeleteTask(pTaskInfo);
}

void CTaskQueue::StopUpload(TASK_INFO * pTaskInfo)
{
	m_pConnection->QueryUploadFinish();
	PrintLog(-1, _T("上传失败"));
	//if (m_pView)
	//	m_pView->GetWorkDirBrowser()->OnUploadFinish(pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
	//DeleteTask(pTaskInfo);
}

void CTaskQueue::EndUpload(TASK_INFO* pTaskInfo, int Result, const CEasyString& MD5)
{
	if(Result == MSG_RESULT_SUCCEED)
	{
		pTaskInfo->TaskFile.Close();

		float CompressRate = pTaskInfo->FileSize ? (float)pTaskInfo->TransferSize * 100.0f / (float)pTaskInfo->FileSize : 0.0f;
		UINT TransferTime = CEasyTimer::GetTimeToTime(pTaskInfo->TransferStartTime, CEasyTimer::GetTime());
		float TransferRate = TransferTime ? (float)pTaskInfo->FileSize * 1000.0f / (float)TransferTime : 0.0f;
		float RealTransferRate = TransferTime ? (float)pTaskInfo->TransferSize * 1000.0f / (float)TransferTime : 0.0f;

		if (pTaskInfo->FileMD5.CompareNoCase(MD5) == 0)
		{
			PrintLog(0, _T("上传完成，实际传输%s,压缩率%0.2f%%,上传速度%s(%s)"),
				(LPCTSTR)FormatNumberWords(pTaskInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate),
				(LPCTSTR)FormatNumberWordsFloat(RealTransferRate));
		}
		else
		{
			PrintLog(-1, _T("上传完成，实际传输%s,压缩率%0.2f%%,上传速度%s,MD5校验失败Local=%s,Server=%s"),
				(LPCTSTR)FormatNumberWords(pTaskInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate),
				(LPCTSTR)pTaskInfo->FileMD5,
				(LPCTSTR)MD5);
		}
	}
	else
	{
		pTaskInfo->TaskFile.Close();
		PrintLog(-1, _T("完成上传失败%d"), Result);
	}
	if (m_pView)
		m_pView->GetWorkDirBrowser()->OnUploadFinish(pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
	DeleteTask(pTaskInfo);
}