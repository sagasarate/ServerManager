#include "stdafx.h"



CFileTransferQueue::CFileTransferQueue()
{
	m_pView = NULL;
	m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;	
	m_IsInTransfer = false;
}


CFileTransferQueue::~CFileTransferQueue()
{
}


bool CFileTransferQueue::Init(CServerManagerClientView * pView)
{
	m_pView = pView;
	m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;
	m_FileTransferQueue.Create(64, 64, 64);
	m_InputBuffer.Create(MAX_MSG_SIZE);
	m_OutputBuffer.Create(MAX_MSG_SIZE);

	return Start() != FALSE;
}
UINT CFileTransferQueue::AddTask(FILE_TRANSFER_TYPE	Type, UINT ConnectionID, UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, UINT FileMode, bool ContinueTransfer)
{
	FILE_INFO * pFileInfo = NULL;
	UINT ID=m_FileTransferQueue.NewObject(&pFileInfo);
	if (pFileInfo)
	{
		pFileInfo->ID = ID;
		pFileInfo->Type = Type;
		pFileInfo->Status = FILE_TRANSFER_STATUS_NONE;
		pFileInfo->ConnectionID = ConnectionID;
		pFileInfo->ServiceID = ServiceID;
		pFileInfo->SourceFilePath = SourceFilePath;
		pFileInfo->TargetFilePath = TargetFilePath;
		pFileInfo->FileMode = FileMode;
		pFileInfo->FileSize = 0;
		pFileInfo->TransferOffset = 0;
		pFileInfo->ContinueTransfer = ContinueTransfer;
		if (m_pView)
			m_pView->OnAddFileTask(pFileInfo->ID, pFileInfo->Type, pFileInfo->SourceFilePath, pFileInfo->TargetFilePath);
		return ID;
	}
	return 0;
}
bool CFileTransferQueue::CancelFileTransfer(UINT ID)
{
	DeleteTask(ID);
	return true;
}
void CFileTransferQueue::DeleteAllTask()
{
	m_FileTransferQueue.Clear();
	if (m_pView)
		m_pView->OnDeleteAllFileTask();
}
void CFileTransferQueue::Update()
{
	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	if (Pos)
	{
		FILE_INFO * pFileInfo = m_FileTransferQueue.GetObject(Pos);
		m_IsInTransfer = true;
		switch (pFileInfo->Status)
		{
		case FILE_TRANSFER_STATUS_NONE:
			{
				CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(pFileInfo->ConnectionID);
				if (pConnection)
				{
					switch (pFileInfo->Type)
					{
					case FILE_TRANSFER_TYPE_DOWNLOAD:
						pConnection->QueryStartDownload(pFileInfo->ServiceID, pFileInfo->SourceFilePath, pFileInfo->TargetFilePath);
						pFileInfo->Status = FILE_TRANSFER_STATUS_STARTING;
						break;
					case FILE_TRANSFER_TYPE_UPLOAD:
						pConnection->QueryStartUpload(pFileInfo->ServiceID, pFileInfo->SourceFilePath, pFileInfo->TargetFilePath);
						pFileInfo->Status = FILE_TRANSFER_STATUS_STARTING;
						break;
					case FILE_TRANSFER_TYPE_DELETE:
						pConnection->QueryDeleteFile(pFileInfo->ServiceID, pFileInfo->SourceFilePath);
						pFileInfo->Status = FILE_TRANSFER_STATUS_STARTING;
						break;
					case FILE_TRANSFER_TYPE_CREATE_DIR:
						pConnection->QueryCreateDir(pFileInfo->ServiceID, pFileInfo->SourceFilePath);
						pFileInfo->Status = FILE_TRANSFER_STATUS_STARTING;
						break;
					case FILE_TRANSFER_TYPE_CHANGE_FILE_MODE:
						pConnection->QueryChangeFileMode(pFileInfo->ServiceID, pFileInfo->SourceFilePath, pFileInfo->FileMode);
						pFileInfo->Status = FILE_TRANSFER_STATUS_STARTING;
						break;
					default:
						{							
							PrintLog(_T("未知的传输类型%d"), pFileInfo->Type);
							DeleteTask(pFileInfo->ID);							
						}
						break;
					}					
				}
				else
				{					
					PrintLog(_T("无法找到服务器[%u]的连接，传输失败"), (LPCTSTR)pFileInfo->ConnectionID);
					DeleteTask(pFileInfo->ID);
				}
			}
			break;		
		case FILE_TRANSFER_STATUS_TRANSFER_DATA:
			{
				if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_FINISH)
				{
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;
					CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(pFileInfo->ConnectionID);
					if (pConnection)
					{
						switch (pFileInfo->Type)
						{
						case FILE_TRANSFER_TYPE_DOWNLOAD:
							{
								pFileInfo->TransferOffset += m_OutputBuffer.GetUsedSize();
								if (m_pView)
								{
									float Progress = 0;
									if (pFileInfo->FileSize)
										Progress = (float)pFileInfo->TransferOffset / pFileInfo->FileSize;
									m_pView->OnFileTaskUpdate(pFileInfo->ID, Progress);
								}
								DoDownload(pConnection, pFileInfo);
							}							
							break;
						case FILE_TRANSFER_TYPE_UPLOAD:
							{
								DoUpload(pConnection, pFileInfo);
							}
							break;
						default:
							{
								PrintLog(_T("未知的传输类型%d"), pFileInfo->Type);
								DeleteTask(pFileInfo->ID);
							}
							break;
						}
					}
				}
				else if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_ERROR)
				{
					m_CompressWorkStatus = COMPRESS_WORK_STATUS_NONE;
					CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(pFileInfo->ConnectionID);
					if (pConnection)
					{
						switch (pFileInfo->Type)
						{
						case FILE_TRANSFER_TYPE_DOWNLOAD:
							pConnection->QueryEndDownload();
							break;
						case FILE_TRANSFER_TYPE_UPLOAD:
							pConnection->QueryEndUpload(0);
							break;
						default:
							{
								PrintLog(_T("未知的传输类型%d"), pFileInfo->Type);
								DeleteTask(pFileInfo->ID);
							}
							break;
						}						
					}
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
BOOL CFileTransferQueue::OnRun()
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

void CFileTransferQueue::OnStartDownloadResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize)
{
	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	FILE_INFO * pFileInfo = NULL;
	if (Pos)
		pFileInfo = m_FileTransferQueue.GetObject(Pos);
	if (Result == MSG_RESULT_SUCCEED)
	{
		if (pFileInfo)
		{
			if (pFileInfo->ConnectionID == pConnection->GetID() && pFileInfo->ServiceID == ServiceID)
			{
				if (m_CurTransferFile.Open(pFileInfo->TargetFilePath, IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite))
				{
					pFileInfo->FileSize = FileSize;
					if (pFileInfo->ContinueTransfer)
						pFileInfo->TransferOffset = m_CurTransferFile.GetSize();
					else
						pFileInfo->TransferOffset = 0;
					pFileInfo->TransferSize = 0;
					pFileInfo->TransferStartTime = CEasyTimer::GetTime();
					pFileInfo->Status = FILE_TRANSFER_STATUS_TRANSFER_DATA;
					DoDownload(pConnection, pFileInfo);
				}
				else
				{
					PrintLog(_T("OnStartDownloadResult创建文件失败%s"), (LPCTSTR)pFileInfo->TargetFilePath);
					pConnection->QueryEndDownload();
				}
			}
			else
			{
				PrintLog(_T("OnStartDownloadResult传输任务不相符"));
				pConnection->QueryEndDownload();
			}
		}
		else
		{
			PrintLog(_T("OnStartDownloadResult已无传输任务"));
			pConnection->QueryEndDownload();
		}
	}
	else
	{
		PrintLog(_T("OnStartDownloadResult请求失败%d"), Result);
		if (pFileInfo)
			DeleteTask(pFileInfo->ID);
	}
}
void CFileTransferQueue::OnDownloadData(CServerConnection * pConnection, short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		void * Pos = m_FileTransferQueue.GetFirstObjectPos();
		if (Pos)
		{
			FILE_INFO * pFileInfo = m_FileTransferQueue.GetObject(Pos);
			if (pFileInfo->ConnectionID == pConnection->GetID())
			{
				if (m_CurTransferFile.Seek(Offset,IFileAccessor::seekBegin))
				{
					if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_NONE)
					{
						pFileInfo->TransferSize += FileData.GetUsedSize();
						m_InputBuffer.SetUsedSize(0);
						m_InputBuffer.PushBack(FileData.GetBuffer(), FileData.GetUsedSize());
						m_OutputBuffer.SetUsedSize(Length);
						m_CompressWorkStatus = COMPRESS_WORK_STATUS_SAVE;
					}
					else
					{
						PrintLog(_T("OnDownloadData数据压缩解压状态错误%d"), m_CompressWorkStatus);
						pConnection->QueryEndDownload();
					}
				}
				else
				{
					PrintLog(_T("OnDownloadData文件定位失败"));
					pConnection->QueryEndDownload();
				}
			}
			else
			{
				PrintLog(_T("OnDownloadData传输任务不相符"));
				pConnection->QueryEndDownload();
			}
		}
		else
		{
			PrintLog(_T("OnDownloadData已无传输任务"), Result);
			pConnection->QueryEndDownload();
		}
	}
	else
	{
		PrintLog(_T("OnDownloadData请求失败%d"), Result);
		pConnection->QueryEndDownload();
	}
}
void CFileTransferQueue::OnEndDownloadResult(CServerConnection * pConnection, short Result, UINT FileLastWriteTime)
{
	FILE_INFO * pFileInfo = NULL;
	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	if (Pos)
		pFileInfo = m_FileTransferQueue.GetObject(Pos);

	if (Result == MSG_RESULT_SUCCEED)
	{
		m_CurTransferFile.SetLastWriteTime(FileLastWriteTime);
		m_CurTransferFile.Close();

		if (pFileInfo)
		{
			float CompressRate = pFileInfo->FileSize ? (float)pFileInfo->TransferSize*100.0f / (float)pFileInfo->FileSize : 0.0f;
			UINT TransferTime = GetTimeToTime(pFileInfo->TransferStartTime, CEasyTimer::GetTime());
			float TransferRate = TransferTime ? (float)pFileInfo->TransferSize*1000.0f / (float)TransferTime : 0.0f;

			PrintLog(_T("下载完成，实际传输%s,压缩率%0.2f%%,下载速度%s"),
				(LPCTSTR)FormatNumberWords(pFileInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate));
		}
	}
	else
	{
		m_CurTransferFile.Close();
		PrintLog(_T("OnEndDownloadResult请求失败%d"), Result);		
	}

	if (pFileInfo)
	{
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnDownloadFinish(pFileInfo->SourceFilePath, pFileInfo->TargetFilePath);
		DeleteTask(pFileInfo->ID);
	}
}
void CFileTransferQueue::OnUploadStartResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize)
{
	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	FILE_INFO * pFileInfo = NULL;
	if (Pos)
		pFileInfo = m_FileTransferQueue.GetObject(Pos);
	if (Result == MSG_RESULT_SUCCEED)
	{
		if (pFileInfo)
		{
			if (pFileInfo->ConnectionID == pConnection->GetID() && pFileInfo->ServiceID == ServiceID)
			{
				if (m_CurTransferFile.Open(pFileInfo->SourceFilePath, IFileAccessor::modeOpen | IFileAccessor::modeRead | IFileAccessor::shareShareAll))
				{
					pFileInfo->FileSize = m_CurTransferFile.GetSize();
					if(pFileInfo->ContinueTransfer)
						pFileInfo->TransferOffset = FileSize;
					else
						pFileInfo->TransferOffset = 0;
					pFileInfo->TransferSize = 0;
					pFileInfo->TransferStartTime = CEasyTimer::GetTime();
					if (pFileInfo->FileSize)
					{
						if (m_CurTransferFile.Seek(pFileInfo->TransferOffset, IFileAccessor::seekBegin))
						{
							pFileInfo->Status = FILE_TRANSFER_STATUS_TRANSFER_DATA;
							m_CompressWorkStatus = COMPRESS_WORK_STATUS_LOAD;
						}
						else
						{
							PrintLog(_T("OnUploadStartResult文件定位失败"));
							pConnection->QueryEndUpload(0);
						}
					}
					else
					{
						PrintLog(_T("文件大小为0，文件上传完毕"));
						CEasyTime LastWriteTime;
						if (!m_CurTransferFile.GetLastWriteTime(LastWriteTime))
							LastWriteTime.FetchLocalTime();
						pConnection->QueryEndUpload((time_t)LastWriteTime);
					}
				}
				else
				{
					PrintLog(_T("OnUploadStartResult打开文件失败%s"), (LPCTSTR)pFileInfo->TargetFilePath);
					pConnection->QueryEndUpload(0);
				}
			}
			else
			{
				PrintLog(_T("OnUploadStartResult传输任务不相符"));
				pConnection->QueryEndUpload(0);
			}
		}
		else
		{
			PrintLog(_T("OnUploadStartResult已无传输任务"));
			pConnection->QueryEndUpload(0);
		}
	}
	else
	{
		PrintLog(_T("OnUploadStartResult请求失败%d"), Result);
		if (pFileInfo)
			DeleteTask(pFileInfo->ID);
	}
}
void CFileTransferQueue::OnUploadData(CServerConnection * pConnection, short Result, UINT64 Offset, UINT Length)
{
	if (Result == MSG_RESULT_SUCCEED)
	{
		void * Pos = m_FileTransferQueue.GetFirstObjectPos();
		if (Pos)
		{
			FILE_INFO * pFileInfo = m_FileTransferQueue.GetObject(Pos);
			if (m_pView)
			{
				float Progress = 0;
				if (pFileInfo->FileSize)
					Progress = (float)pFileInfo->TransferOffset / pFileInfo->FileSize;				
				m_pView->OnFileTaskUpdate(pFileInfo->ID, Progress);
			}
				
			if (pFileInfo->ConnectionID == pConnection->GetID())
			{
				if (pFileInfo->TransferOffset < pFileInfo->FileSize)
				{
					if (m_CompressWorkStatus == COMPRESS_WORK_STATUS_NONE)
					{
						if (m_CurTransferFile.Seek(pFileInfo->TransferOffset, IFileAccessor::seekBegin))
						{
							m_CompressWorkStatus = COMPRESS_WORK_STATUS_LOAD;
						}
						else
						{
							PrintLog(_T("OnUploadData文件定位失败"));
							pConnection->QueryEndUpload(0);
						}
					}
					else
					{
						PrintLog(_T("OnUploadData数据压缩解压状态错误%d"), m_CompressWorkStatus);
						pConnection->QueryEndUpload(0);
					}
				}
				else
				{
					PrintLog(_T("文件上传完毕"));
					CEasyTime LastWriteTime;
					if (!m_CurTransferFile.GetLastWriteTime(LastWriteTime))
						LastWriteTime.FetchLocalTime();
					pConnection->QueryEndUpload((time_t)LastWriteTime);
				}
				
			}
			else
			{
				PrintLog(_T("OnUploadData传输任务不相符"));
				pConnection->QueryEndUpload(0);
			}
		}
		else
		{
			PrintLog(_T("OnUploadData已无传输任务"), Result);
			pConnection->QueryEndUpload(0);
		}
	}
	else
	{
		PrintLog(_T("OnUploadData请求失败%d"), Result);
		pConnection->QueryEndUpload(0);
	}
}
void CFileTransferQueue::OnEndUploadResult(CServerConnection * pConnection, short Result)
{
	m_CurTransferFile.Close();

	FILE_INFO * pFileInfo = NULL;
	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	if (Pos)
		pFileInfo = m_FileTransferQueue.GetObject(Pos);

	if (Result == MSG_RESULT_SUCCEED)
	{
		if (pFileInfo)
		{
			float CompressRate = pFileInfo->FileSize ? (float)pFileInfo->TransferSize*100.0f / (float)pFileInfo->FileSize : 0.0f;
			UINT TransferTime = GetTimeToTime(pFileInfo->TransferStartTime, CEasyTimer::GetTime());
			float TransferRate = TransferTime ? (float)pFileInfo->TransferSize*1000.0f / (float)TransferTime : 0.0f;

			PrintLog(_T("上传完成，实际传输%s,压缩率%0.2f%%,上传速度%s"),
				(LPCTSTR)FormatNumberWords(pFileInfo->TransferSize),
				CompressRate,
				(LPCTSTR)FormatNumberWordsFloat(TransferRate));
		}
	}
	else
	{
		PrintLog(_T("OnEndUploadResult请求失败%d"), Result);
	}
	
	if (pFileInfo)
	{
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnUploadFinish(pFileInfo->SourceFilePath, pFileInfo->TargetFilePath);
		DeleteTask(pFileInfo->ID);
	}
		
}

void CFileTransferQueue::OnDeleteFileResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(_T("OnDeleteFileResult请求失败%d"), Result);
	}

	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	if (Pos)
	{
		FILE_INFO * pFileInfo = m_FileTransferQueue.GetObject(Pos);
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnDeleteFileFinish(pFileInfo->SourceFilePath);
		DeleteTask(pFileInfo->ID);
	}
}

void CFileTransferQueue::OnCreateDirResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR Dir)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(_T("OnCreateDirResult请求失败%d"), Result);
	}

	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	if (Pos)
	{
		FILE_INFO * pFileInfo = m_FileTransferQueue.GetObject(Pos);
		if (m_pView)
			m_pView->GetWorkDirBrowser()->OnCreateDirFinish(pFileInfo->SourceFilePath);
		DeleteTask(pFileInfo->ID);
	}
}
void CFileTransferQueue::OnChangeFileModeResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath, UINT Mode)
{
	if (Result == MSG_RESULT_SUCCEED)
	{

	}
	else
	{
		PrintLog(_T("OnChangeFileModeResult请求失败%d"), Result);
	}

	void * Pos = m_FileTransferQueue.GetFirstObjectPos();
	if (Pos)
	{
		FILE_INFO * pFileInfo = m_FileTransferQueue.GetObject(Pos);
		DeleteTask(pFileInfo->ID);
	}
}
void CFileTransferQueue::DeleteTask(UINT ID)
{
	m_FileTransferQueue.DeleteObject(ID);
	if (m_pView)
		m_pView->OnDeleteFileTask(ID);
}
void CFileTransferQueue::PrintLog(LPCTSTR szFormat, ...)
{
	if (m_pView)
	{
		va_list vl;

		va_start(vl, szFormat);
		m_pView->PrintLogVL(szFormat, vl);
		va_end(vl);
	}
}
void CFileTransferQueue::DoDownload(CServerConnection * pConnection, FILE_INFO * pFileInfo)
{
	UINT QuerySize = FILE_TRANSFER_BLOCK_SIZE;
	if (QuerySize > pFileInfo->FileSize - pFileInfo->TransferOffset)
		QuerySize = pFileInfo->FileSize - pFileInfo->TransferOffset;
	if (QuerySize)
	{
		pConnection->QueryDownloadData(pFileInfo->TransferOffset, QuerySize);
	}
	else
	{
		pConnection->QueryEndDownload();
	}
}
void CFileTransferQueue::DoUpload(CServerConnection * pConnection, FILE_INFO * pFileInfo)
{
	if (m_OutputBuffer.GetUsedSize())
	{
		pConnection->QueryUploadData(pFileInfo->TransferOffset, m_InputBuffer.GetUsedSize(), m_OutputBuffer);
		pFileInfo->TransferSize += m_OutputBuffer.GetUsedSize();
		pFileInfo->TransferOffset += m_InputBuffer.GetUsedSize();		
	}
	else
	{
		CEasyTime LastWriteTime;
		if (!m_CurTransferFile.GetLastWriteTime(LastWriteTime))
			LastWriteTime.FetchLocalTime();
		pConnection->QueryEndUpload((time_t)LastWriteTime);
	}
}