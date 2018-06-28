#include "stdafx.h"



CTaskQueue::CTaskQueue()
{
	m_pView = NULL;
	m_pConnection = NULL;
	m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;	
	m_IsInTransfer = false;
}


CTaskQueue::~CTaskQueue()
{
}


bool CTaskQueue::Init(CServerManagerClientView * pView, CServerConnection * pConnection)
{
	m_pView = pView;
	m_pConnection = pConnection;
	m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;
	m_TaskQueue.Create(64, 64, 64);
	m_InputBuffer.Create(MAX_MSG_SIZE);
	m_OutputBuffer.Create(MAX_MSG_SIZE);

	return Start() != FALSE;
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
	TASK_INFO * pTaskInfo = NULL;
	UINT ID = m_TaskQueue.NewObject(&pTaskInfo);
	if (pTaskInfo)
	{
		pTaskInfo->Clear();
		pTaskInfo->ID = ID;
		pTaskInfo->Type = TASK_TYPE_UPLOAD;
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

bool CTaskQueue::CancelFileTransfer(UINT ID)
{
	DeleteTask(ID);
	return true;
}
void CTaskQueue::DeleteAllTask()
{
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	while (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		DeleteTask(pTaskInfo->ID);
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
						m_pConnection->QueryStartDownload(pTaskInfo->ServiceID, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					case TASK_TYPE_UPLOAD:
						m_pConnection->QueryStartUpload(pTaskInfo->ServiceID, pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					case TASK_TYPE_DELETE_FILE:
						m_pConnection->QueryDeleteFile(pTaskInfo->ServiceID, pTaskInfo->TargetFilePath);
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					case TASK_TYPE_CREATE_DIR:
						m_pConnection->QueryCreateDir(pTaskInfo->ServiceID, pTaskInfo->TargetFilePath);
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					case TASK_TYPE_CHANGE_FILE_MODE:
						m_pConnection->QueryChangeFileMode(pTaskInfo->ServiceID, pTaskInfo->TargetFilePath, pTaskInfo->FileMode);
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					case TASK_TYPE_STARTUP_SERVICE:
						m_pConnection->QueryStartupService(pTaskInfo->ServiceID);
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					case TASK_TYPE_SHUTDOWN_SERVICE:
						m_pConnection->QueryShutDownService(pTaskInfo->ServiceID, SERVICE_SHUTDOWN_TYPE_SAFE);
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					case TASK_TYPE_RELOAD_CONFIG_DATA:
						m_pConnection->QuerySendCommand(pTaskInfo->ServiceID, _T("ReloadAllData()"));
						pTaskInfo->Status = TASK_STATUS_STARTING;
						break;
					default:
						{							
							PrintLog(_T("未知的任务类型%d"), pTaskInfo->Type);
							DeleteTask(pTaskInfo->ID);							
						}
						break;
					}					
				}
				else
				{					
					PrintLog(_T("服务器[%u]未连接，任务放弃"), (LPCTSTR)m_pConnection->GetServerAddress());
					DeleteTask(pTaskInfo->ID);
				}
			}
			break;		
		case TASK_STATUS_TRANSFER_DATA:
			{
				if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_FINISH)
				{
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;
					if (m_pConnection->IsConnected())
					{
						switch (pTaskInfo->Type)
						{
						case TASK_TYPE_DOWNLOAD:
							{
								pTaskInfo->TransferOffset += m_OutputBuffer.GetUsedSize();
								if (m_pView)
								{
									float Progress = 0;
									if (pTaskInfo->FileSize)
										Progress = (float)pTaskInfo->TransferOffset / pTaskInfo->FileSize;
									m_pView->OnTaskUpdate(m_pConnection->GetID(), pTaskInfo->ID, Progress);
								}
								DoDownload(pTaskInfo);
							}							
							break;
						case TASK_TYPE_UPLOAD:
							{
								DoUpload(pTaskInfo);
							}
							break;
						default:
							{
								PrintLog(_T("不支持的任务类型%d"), pTaskInfo->Type);
								DeleteTask(pTaskInfo->ID);
							}
							break;
						}
					}
				}
				else if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_ERROR)
				{
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;
					if (m_pConnection->IsConnected())
					{
						switch (pTaskInfo->Type)
						{
						case TASK_TYPE_DOWNLOAD:
							m_pConnection->QueryEndDownload();
							break;
						case TASK_TYPE_UPLOAD:
							m_pConnection->QueryEndUpload(0);
							break;
						default:
							{
								PrintLog(_T("不支持的任务类型%d"), pTaskInfo->Type);
								DeleteTask(pTaskInfo->ID);
							}
							break;
						}						
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
					if (GetTimeToTime(pTaskInfo->TransferStartTime, CurTime) >= SERVICE_STARTUP_TIMEOUT)
					{
						PrintLog(_T("任务超时%d"), pTaskInfo->Type);
						DeleteTask(pTaskInfo->ID);
					}
					break;
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
BOOL CTaskQueue::OnRun()
{
	switch (m_CompressWorkStatus)
	{
	case COMPRESS_WORK_STATUS_SAVE:
		{
			size_t UnpackSize = m_OutputBuffer.GetUsedSize();
			size_t SourceSize = m_InputBuffer.GetUsedSize() - LZMA_PROPS_SIZE;

			int ErrCode = LzmaUncompress((BYTE *)m_OutputBuffer.GetBuffer(), &UnpackSize,
				((BYTE *)m_InputBuffer.GetBuffer()) + LZMA_PROPS_SIZE, &SourceSize,
				(BYTE *)m_InputBuffer.GetBuffer(), LZMA_PROPS_SIZE);
			if (ErrCode == SZ_OK)
			{
				m_OutputBuffer.SetUsedSize(UnpackSize);				
				UINT64 WriteLen = m_CurTransferFile.Write(m_OutputBuffer.GetBuffer(), m_OutputBuffer.GetUsedSize());
				if (WriteLen == m_OutputBuffer.GetUsedSize())
				{
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_FINISH;
				}
				else
				{					
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_ERROR;
				}
			}
			else
			{
				m_CompressWorkStatus = COMPRESS_WORK_STATUS_ERROR;
			}
		}
		break;
	case COMPRESS_WORK_STATUS_LOAD:
		{
			UINT64 ReadLen = m_CurTransferFile.Read(m_InputBuffer.GetBuffer(), FILE_TRANSFER_BLOCK_SIZE);

			if (ReadLen)
			{
				m_InputBuffer.SetUsedSize(ReadLen);

				size_t PackSize = m_OutputBuffer.GetBufferSize() - LZMA_PROPS_SIZE;
				size_t PackPropSize = LZMA_PROPS_SIZE;

				int ErrCode = LzmaCompress(((BYTE *)m_OutputBuffer.GetBuffer()) + LZMA_PROPS_SIZE, &PackSize,
					(BYTE *)m_InputBuffer.GetBuffer(), m_InputBuffer.GetUsedSize(),
					(BYTE *)m_OutputBuffer.GetBuffer(), &PackPropSize, 3,
					1 << 20, 3, 0, 2, 32, 2);

				if (ErrCode == SZ_OK)
				{
					PackSize += LZMA_PROPS_SIZE;
					m_OutputBuffer.SetUsedSize(PackSize);
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_FINISH;
				}
				else
				{
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_ERROR;
				}
			}
			else
			{
				m_InputBuffer.SetUsedSize(0);
				m_OutputBuffer.SetUsedSize(0);
				m_CompressWorkStatus = COMPRESS_WORK_STATUS_ERROR;
				int ErrCode = GetLastError();
				PrintSystemLog(_T("读取文件错误%d"), ErrCode);
			}
		}		
		break;
	}
	return TRUE;
}

void CTaskQueue::OnStartDownloadResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize)
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
				if (m_CurTransferFile.Open(pTaskInfo->TargetFilePath, IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite))
				{
					pTaskInfo->FileSize = FileSize;
					if (pTaskInfo->ContinueTransfer)
						pTaskInfo->TransferOffset = m_CurTransferFile.GetSize();
					else
						pTaskInfo->TransferOffset = 0;
					pTaskInfo->TransferSize = 0;
					pTaskInfo->TransferStartTime = CEasyTimer::GetTime();
					pTaskInfo->Status = TASK_STATUS_TRANSFER_DATA;
					DoDownload(pTaskInfo);
				}
				else
				{
					PrintLog(_T("OnStartDownloadResult创建文件失败%s"), (LPCTSTR)pTaskInfo->TargetFilePath);
					m_pConnection->QueryEndDownload();
				}
			}
			else
			{
				PrintLog(_T("OnStartDownloadResult请求失败%d"), Result);
				DeleteTask(pTaskInfo->ID);
			}
		}
		else
		{
			PrintLog(_T("OnStartDownloadResult传输任务不相符"));
			m_pConnection->QueryEndDownload();
		}
	}
	else
	{
		PrintLog(_T("OnStartDownloadResult已无传输任务"));
		m_pConnection->QueryEndDownload();
	}	
}
void CTaskQueue::OnDownloadData(short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		void * Pos = m_TaskQueue.GetFirstObjectPos();
		if (Pos)
		{
			TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
			
			if (m_CurTransferFile.Seek(Offset, IFileAccessor::seekBegin))
			{
				if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_NONE)
				{
					pTaskInfo->TransferSize += FileData.GetUsedSize();
					m_InputBuffer.SetUsedSize(0);
					m_InputBuffer.PushBack(FileData.GetBuffer(), FileData.GetUsedSize());
					m_OutputBuffer.SetUsedSize(Length);
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_SAVE;
				}
				else
				{
					PrintLog(_T("OnDownloadData数据压缩解压状态错误%d"), m_CompressWorkStatus);
					m_pConnection->QueryEndDownload();
				}
			}
			else
			{
				PrintLog(_T("OnDownloadData文件定位失败"));
				m_pConnection->QueryEndDownload();
			}
			
		}
		else
		{
			PrintLog(_T("OnDownloadData已无传输任务"), Result);
			m_pConnection->QueryEndDownload();
		}
	}
	else
	{
		PrintLog(_T("OnDownloadData请求失败%d"), Result);
		m_pConnection->QueryEndDownload();
	}
}
void CTaskQueue::OnEndDownloadResult(short Result, UINT FileLastWriteTime)
{
	TASK_INFO * pTaskInfo = NULL;
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);

	if (Result == MSG_RESULT_SUCCEED)
	{
		m_CurTransferFile.SetLastWriteTime(FileLastWriteTime);
		m_CurTransferFile.Close();

		if (pTaskInfo)
		{
			float CompressRate = pTaskInfo->FileSize ? (float)pTaskInfo->TransferSize*100.0f / (float)pTaskInfo->FileSize : 0.0f;
			UINT TransferTime = GetTimeToTime(pTaskInfo->TransferStartTime, CEasyTimer::GetTime());
			float TransferRate = TransferTime ? (float)pTaskInfo->TransferSize*1000.0f / (float)TransferTime : 0.0f;

			PrintLog(_T("下载完成，实际传输%s,压缩率%0.2f%%,下载速度%s"),
				(LPCTSTR)FormatNumberWords(pTaskInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate));
		}
	}
	else
	{
		m_CurTransferFile.Close();
		PrintLog(_T("OnEndDownloadResult请求失败%d"), Result);		
	}

	if (pTaskInfo)
	{
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnDownloadFinish(pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
		DeleteTask(pTaskInfo->ID);
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
				if (m_CurTransferFile.Open(pTaskInfo->SourceFilePath, IFileAccessor::modeOpen | IFileAccessor::modeRead | IFileAccessor::shareShareAll))
				{
					pTaskInfo->FileSize = m_CurTransferFile.GetSize();
					if(pTaskInfo->ContinueTransfer)
						pTaskInfo->TransferOffset = FileSize;
					else
						pTaskInfo->TransferOffset = 0;
					pTaskInfo->TransferSize = 0;
					pTaskInfo->TransferStartTime = CEasyTimer::GetTime();
					if (pTaskInfo->FileSize)
					{
						if (m_CurTransferFile.Seek(pTaskInfo->TransferOffset, IFileAccessor::seekBegin))
						{
							pTaskInfo->Status = TASK_STATUS_TRANSFER_DATA;
							m_CompressWorkStatus = COMPRESS_WORK_STATUS_LOAD;
						}
						else
						{
							PrintLog(_T("OnUploadStartResult文件定位失败"));
							m_pConnection->QueryEndUpload(0);
						}
					}
					else
					{
						PrintLog(_T("文件大小为0，文件上传完毕"));
						CEasyTime LastWriteTime;
						if (!m_CurTransferFile.GetLastWriteTime(LastWriteTime))
							LastWriteTime.FetchLocalTime();
						m_pConnection->QueryEndUpload((time_t)LastWriteTime);
					}
				}
				else
				{
					PrintLog(_T("OnUploadStartResult打开文件失败%s"), (LPCTSTR)pTaskInfo->TargetFilePath);
					m_pConnection->QueryEndUpload(0);
				}
			}
			else
			{
				PrintLog(_T("OnUploadStartResult请求失败%d"), Result);
				DeleteTask(pTaskInfo->ID);
			}
		}
		else
		{
			PrintLog(_T("OnUploadStartResult传输任务不相符"));
			m_pConnection->QueryEndUpload(0);
		}
	}
	else
	{
		PrintLog(_T("OnUploadStartResult已无传输任务"));
		m_pConnection->QueryEndUpload(0);
	}
	
}
void CTaskQueue::OnUploadData(short Result, UINT64 Offset, UINT Length)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		void * Pos = m_TaskQueue.GetFirstObjectPos();
		if (Pos)
		{
			TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
			if (m_pView)
			{
				float Progress = 0;
				if (pTaskInfo->FileSize)
					Progress = (float)pTaskInfo->TransferOffset / pTaskInfo->FileSize;				
				m_pView->OnTaskUpdate(m_pConnection->GetID(), pTaskInfo->ID, Progress);
			}
				
			
			if (pTaskInfo->TransferOffset < pTaskInfo->FileSize)
			{
				if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_NONE)
				{
					if (m_CurTransferFile.Seek(pTaskInfo->TransferOffset, IFileAccessor::seekBegin))
					{
						m_CompressWorkStatus = COMPRESS_WORK_STATUS_LOAD;
					}
					else
					{
						PrintLog(_T("OnUploadData文件定位失败"));
						m_pConnection->QueryEndUpload(0);
					}
				}
				else
				{
					PrintLog(_T("OnUploadData数据压缩解压状态错误%d"), m_CompressWorkStatus);
					m_pConnection->QueryEndUpload(0);
				}
			}
			else
			{
				PrintLog(_T("文件上传完毕"));
				CEasyTime LastWriteTime;
				if (!m_CurTransferFile.GetLastWriteTime(LastWriteTime))
					LastWriteTime.FetchLocalTime();
				m_pConnection->QueryEndUpload((time_t)LastWriteTime);
			}
				
			
		}
		else
		{
			PrintLog(_T("OnUploadData已无传输任务"), Result);
			m_pConnection->QueryEndUpload(0);
		}
	}
	else
	{
		PrintLog(_T("OnUploadData请求失败%d"), Result);
		m_pConnection->QueryEndUpload(0);
	}
}
void CTaskQueue::OnEndUploadResult(short Result)
{
	m_CurTransferFile.Close();

	TASK_INFO * pTaskInfo = NULL;
	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
		pTaskInfo = m_TaskQueue.GetObject(Pos);

	if (Result == MSG_RESULT_SUCCEED)
	{
		if (pTaskInfo)
		{
			float CompressRate = pTaskInfo->FileSize ? (float)pTaskInfo->TransferSize*100.0f / (float)pTaskInfo->FileSize : 0.0f;
			UINT TransferTime = GetTimeToTime(pTaskInfo->TransferStartTime, CEasyTimer::GetTime());
			float TransferRate = TransferTime ? (float)pTaskInfo->TransferSize*1000.0f / (float)TransferTime : 0.0f;

			PrintLog(_T("上传完成，实际传输%s,压缩率%0.2f%%,上传速度%s"),
				(LPCTSTR)FormatNumberWords(pTaskInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate));
		}
	}
	else
	{
		PrintLog(_T("OnEndUploadResult请求失败%d"), Result);
	}
	
	if (pTaskInfo)
	{
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnUploadFinish(pTaskInfo->SourceFilePath, pTaskInfo->TargetFilePath);
		DeleteTask(pTaskInfo->ID);
	}
		
}

void CTaskQueue::OnDeleteFileResult(short Result, UINT ServiceID, LPCTSTR FilePath)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(_T("OnDeleteFileResult请求失败%d"), Result);
	}

	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnDeleteFileFinish(pTaskInfo->TargetFilePath);
		DeleteTask(pTaskInfo->ID);
	}
}

void CTaskQueue::OnCreateDirResult(short Result, UINT ServiceID, LPCTSTR Dir)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(_T("OnCreateDirResult请求失败%d"), Result);
	}

	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnCreateDirFinish(pTaskInfo->TargetFilePath);
		DeleteTask(pTaskInfo->ID);
	}
}
void CTaskQueue::OnChangeFileModeResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT Mode)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(_T("OnChangeFileModeResult请求失败%d"), Result);
	}

	void * Pos = m_TaskQueue.GetFirstObjectPos();
	if (Pos)
	{
		TASK_INFO * pTaskInfo = m_TaskQueue.GetObject(Pos);
		DeleteTask(pTaskInfo->ID);
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
				PrintLog(_T("OnServiceStartupResult请求失败%d"), Result);
				if (pTaskInfo->ContinueTransfer)
					DeleteAllTask();
				else
					DeleteTask(pTaskInfo->ID);
			}
		}
		else
		{
			PrintLog(_T("OnServiceStartupResult任务不相符"));
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
				PrintLog(_T("OnServiceShutdownResult请求失败%d"), Result);
				if (pTaskInfo->ContinueTransfer)
					DeleteAllTask();
				else
					DeleteTask(pTaskInfo->ID);
			}
		}
		else
		{
			PrintLog(_T("OnServiceShutdownResult任务不相符"));
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
				DeleteTask(pTaskInfo->ID);
			}
			else
			{
				PrintLog(_T("OnSendCommandResult请求失败%d"), Result);
				if (pTaskInfo->ContinueTransfer)
					DeleteAllTask();
				else
					DeleteTask(pTaskInfo->ID);
			}
		}
		else
		{
			PrintLog(_T("OnSendCommandResult任务不相符"));
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
					DeleteTask(pTaskInfo->ID);
				}
				break;
			case TASK_TYPE_SHUTDOWN_SERVICE:
				if (ServiceInfo.GetStatus() == SERVICE_STATUS_STOP)
				{
					DeleteTask(pTaskInfo->ID);
				}
				break;
			}
		}
	}
}
void CTaskQueue::DeleteTask(UINT ID)
{	
	m_TaskQueue.DeleteObject(ID);
	if (m_pView)
		m_pView->OnDeleteTask(m_pConnection->GetID(), ID);
}
void CTaskQueue::PrintLog(LPCTSTR szFormat, ...)
{
	if (m_pView)
	{
		va_list vl;

		va_start(vl, szFormat);
		m_pView->PrintLogVL(szFormat, vl);
		va_end(vl);
	}
}
void CTaskQueue::DoDownload(TASK_INFO * pTaskInfo)
{
	UINT QuerySize = FILE_TRANSFER_BLOCK_SIZE;
	if (QuerySize > pTaskInfo->FileSize - pTaskInfo->TransferOffset)
		QuerySize = pTaskInfo->FileSize - pTaskInfo->TransferOffset;
	if (QuerySize)
	{
		m_pConnection->QueryDownloadData(pTaskInfo->TransferOffset, QuerySize);
	}
	else
	{
		m_pConnection->QueryEndDownload();
	}
}
void CTaskQueue::DoUpload(TASK_INFO * pTaskInfo)
{
	if (m_OutputBuffer.GetUsedSize())
	{
		m_pConnection->QueryUploadData(pTaskInfo->TransferOffset, m_InputBuffer.GetUsedSize(), m_OutputBuffer);
		pTaskInfo->TransferSize += m_OutputBuffer.GetUsedSize();
		pTaskInfo->TransferOffset += m_InputBuffer.GetUsedSize();		
	}
	else
	{
		CEasyTime LastWriteTime;
		if (!m_CurTransferFile.GetLastWriteTime(LastWriteTime))
			LastWriteTime.FetchLocalTime();
		m_pConnection->QueryEndUpload((time_t)LastWriteTime);
	}
}