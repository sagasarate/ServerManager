#include "stdafx.h"

CFileTask::CFileTask()
{
	m_pTaskFile = NULL;
	m_ServiceID = 0;
	m_FileMode = 0;
	m_FileSize = 0;
	m_StartOffset = 0;
	m_FinishSize = 0;
	m_ContinueTransfer = false;
	m_TransferStartTime = 0;	
	m_FileLastWriteTime = 0;
	m_SerialNumber = 0;
	m_LastProcessedSerialNumber = 0;
	m_LastFinishedSerialNumber = 0;
}
CFileTask::~CFileTask()
{
	Destory();
}

void CFileTask::Destory()
{
	//LogDebug("任务%s销毁", (LPCTSTR)m_FilePath);
	SAFE_RELEASE(m_pTaskFile);
	m_ServiceID = 0;
	m_FileMode = 0;
	m_FileSize = 0;
	m_StartOffset = 0;
	m_FinishSize = 0;
	m_ContinueTransfer = false;
	m_TransferStartTime = 0;	
	m_FileLastWriteTime = 0;
	m_SerialNumber = 0;
	m_LastProcessedSerialNumber = 0;
	m_LastFinishedSerialNumber = 0;

	for (FILE_DATA_BLOCK * pData : m_SerialOperations)
	{
		m_pManager->DeleteFileDataInfo(pData);
	}
	for (FILE_DATA_BLOCK * pData : m_FinishQueue)
	{
		m_pManager->DeleteFileDataInfo(pData);
	}
	FILE_DATA_BLOCK * pData = NULL;
	while (m_ParallelOperations.PopFront(&pData))
	{
		m_pManager->DeleteFileDataInfo(pData);
	}
	
	m_SerialOperations.Destory();
	m_ParallelOperations.Destory();
	m_FinishQueue.Destory();

	CBaseTask::Destory();
}

bool CFileTask::Create(CTaskManager * pManager, UINT ClientID, UINT ServiceID, TASK_TYPE Type, const CEasyString& FilePath, UINT64 StartOffset)
{
	Destory();
	m_pManager = pManager;
	m_ClientID = ClientID;
	m_ServiceID = ServiceID;
	m_TaskType = Type;
	m_CreateTime = time(NULL);
	m_FilePath = FilePath;
	m_StartOffset = StartOffset;
	m_FinishSize = 0;
	m_QueryCount = 0;
	InitStatus();
	if (!m_SerialOperations.Create(CMainConfig::GetInstance()->GetPoolConfigs().SerialOperationPoolConfig))
	{
		Log("串行操作池创建失败");
		return false;
	}
	if (!m_ParallelOperations.Create(CMainConfig::GetInstance()->GetPoolConfigs().ParallelOperationPoolConfig.StartSize))
	{
		Log("并行操作池创建失败");
		return false;
	}
	if (!m_FinishQueue.Create(CMainConfig::GetInstance()->GetPoolConfigs().FinishOperationPoolConfig))
	{
		Log("完成操作池创建失败");
		return false;
	}
	
	m_HashMD5.Init();
	m_pTaskFile = CFileSystemManager::GetInstance()->CreateFileAccessor(FILE_CHANNEL_NORMAL1);
	if (m_pTaskFile)
	{
		switch (m_TaskType)
		{
		case TASK_TYPE_READ:
			if (m_pTaskFile->Open(m_FilePath, IFileAccessor::modeOpen | IFileAccessor::modeRead | IFileAccessor::shareShareAll))
			{
				m_FileSize = m_pTaskFile->GetSize();
				if (m_StartOffset > m_FileSize)
					m_StartOffset = m_FileSize;
				m_pTaskFile->Seek(IFileAccessor::seekBegin, m_StartOffset);
				CEasyTime Time;
				m_pTaskFile->GetLastWriteTime(Time);
				m_FileLastWriteTime = Time;
				LogDebug("已创建文件读取任务,FileSize=%llu,StartOffset=%llu,FilePath=%s",
					m_FileSize, m_StartOffset, (LPCTSTR)m_FilePath);
				return true;
			}
			else
			{
				Log(_T("打开文件失败:%s"), (LPCTSTR)m_FilePath);
			}
			break;
		case TASK_TYPE_MAKE_MD5:
			if (m_pTaskFile->Open(m_FilePath, IFileAccessor::modeOpen | IFileAccessor::modeRead | IFileAccessor::shareShareAll))
			{
				m_FileSize = m_pTaskFile->GetSize();	
				if (m_StartOffset > m_FileSize)
					m_StartOffset = m_FileSize;
				m_pTaskFile->Seek(IFileAccessor::seekBegin, m_StartOffset);
				CEasyTime Time;
				m_pTaskFile->GetLastWriteTime(Time);
				m_FileLastWriteTime = Time;
				LogDebug("已创建文件MD5生成任务,FileSize=%llu,StartOffset=%llu,FilePath=%s",
					m_FileSize, m_StartOffset, (LPCTSTR)m_FilePath);
				return true;
			}
			else
			{
				Log(_T("打开文件失败:%s"), (LPCTSTR)m_FilePath);
			}
			break;
		case TASK_TYPE_WRITE:
			if (m_pTaskFile->Open(m_FilePath, IFileAccessor::modeCreateAlways | IFileAccessor::modeWrite))
			{
				m_FileSize = m_pTaskFile->GetSize();
				//if (m_StartOffset > m_FileSize)
				//	m_StartOffset = m_FileSize;
				//m_pTaskFile->Seek(IFileAccessor::seekBegin, m_StartOffset);
				CEasyTime Time;
				m_pTaskFile->GetLastWriteTime(Time);
				m_FileLastWriteTime = Time;
				LogDebug("已创建文件写入任务,FileSize=%llu,FilePath=%s,FileTime=%u",
					m_FileSize, (LPCTSTR)m_FilePath, m_FileLastWriteTime);
				return true;
			}
			else
			{
				Log(_T("打开文件失败:%s"), (LPCTSTR)m_FilePath);
			}
			break;
		default:
			Log(_T("未知任务类型%d"), m_TaskType);
		}
	}
	else
	{		
		Log(_T("创建文件操作器失败"));
	}
	return false;
}

bool CFileTask::AddWriteQuery(UINT Length, const CEasyBuffer& FileData, bool IsLast, bool NeedAck)
{
	if (m_TaskType == TASK_TYPE_WRITE)
	{
		FILE_DATA_BLOCK * pData = m_pManager->AllocFileDataInfo(FILE_TRANSFER_BLOCK_SIZE * 3);
		if (pData)
		{
			pData->Offset = 0;
			pData->OriginSize = Length;
			pData->DataBuffer = FileData;
			pData->IsLast = IsLast;
			pData->NeedAck = NeedAck;
			pData->SerialNumber = ++m_SerialNumber;
			m_ParallelOperations.PushBack(&pData);
			AtomicInc(&m_QueryCount);
			LogDebug("添加写入文件块%u,Size=%u OriginSize=%u NeedAck=%s",
				pData->SerialNumber, FileData.GetUsedSize(), Length, NeedAck ? "true" : "false");
			return true;
		}		
	}
	else
	{
		Log("当前任务不是文件写入任务");
	}
	return false;
}

bool CFileTask::AddReadQuery()
{
	if (m_TaskType == TASK_TYPE_READ)
	{		
		UINT Count = AtomicInc(&m_QueryCount);
		LogDebug("已添加读取请求%u", Count);
		return true;
	}
	else
	{
		Log("当前任务不是文件读取任务");
	}
	return false;
}

int CFileTask::DoSerialOperation(int ProcessLimit)
{
	int ProcessCount = 0;
		
	switch (m_TaskType)
	{
	case TASK_TYPE_READ:
	case TASK_TYPE_MAKE_MD5:
		if (m_ParallelOperations.GetUsedSize() < MAX_FILE_READ_BLOCK)
		{
			ProcessCount++;
			FILE_DATA_BLOCK * pData = m_pManager->AllocFileDataInfo(FILE_TRANSFER_BLOCK_SIZE * 3);
			if(pData)
			{
				//m_pTaskFile->Seek(IFileAccessor::seekBegin, pData->Offset);
				UINT64 ReadLen = m_pTaskFile->Read(pData->DataBuffer.GetBuffer(), FILE_TRANSFER_BLOCK_SIZE);	
				LogDebug("任务%d读取文件%u At%llu Size=%u ReadSize=%llu FinishSize=%llu",
					m_TaskType, m_SerialNumber + 1, m_StartOffset + m_FinishSize, FILE_TRANSFER_BLOCK_SIZE, ReadLen, m_FinishSize + ReadLen);				
				if (ReadLen)
				{
					pData->Offset = m_StartOffset + m_FinishSize;
					m_FinishSize += ReadLen;
					pData->DataBuffer.SetUsedSize(ReadLen);
					pData->OriginSize = ReadLen;
					if (m_FinishSize == m_FileSize)
						pData->IsLast = true;
					pData->SerialNumber = ++m_SerialNumber;
					m_HashMD5.AddData((BYTE *)pData->DataBuffer.GetBuffer(), ReadLen);

					if (m_TaskType == TASK_TYPE_READ)
					{
						if (!m_ParallelOperations.PushBack(&pData))
						{
							Log("并行队列已满", m_ParallelOperations.GetUsedSize(), m_ParallelOperations.GetBufferSize());
						}
					}
					else
					{						
						m_pManager->DeleteFileDataInfo(pData);
					}
					if (pData->IsLast)
					{
						m_HashMD5.MD5Final();
						m_FileMD5 = m_HashMD5.GetHashCodeString();
						LogDebug("读取文件完成,ReadSize=%llu,MD5=%s,FilePath=%s", m_FinishSize, (LPCTSTR)m_FileMD5, (LPCTSTR)m_FilePath);
						if (m_TaskType == TASK_TYPE_READ)
							ChangeStatus(TASK_STATUS_FINISH);
						else
							ChangeStatus(TASK_STATUS_END);
					}
				}
				else
				{
					m_pManager->DeleteFileDataInfo(pData);
					Log(_T("文件读取错误"));
					ChangeStatus(TASK_STATUS_ERROR);
				}
			}
			else
			{
				Log(_T("无法分配数据块"));
				ChangeStatus(TASK_STATUS_ERROR);
			}
		}
		break;
	case TASK_TYPE_WRITE:
		{
			LPVOID Pos = NULL;
			FILE_DATA_BLOCK * pData = GetSerialOperation(Pos);
			if (pData)
			{
				ProcessCount++;
				if (pData->DataBuffer.GetUsedSize())
				{
					m_HashMD5.AddData((BYTE *)pData->DataBuffer.GetBuffer(), pData->DataBuffer.GetUsedSize());
					pData->Offset = m_StartOffset + m_FinishSize;
					UINT64 WriteLen = m_pTaskFile->Write(pData->DataBuffer.GetBuffer(), pData->DataBuffer.GetUsedSize());
					m_FinishSize += WriteLen;
					LogDebug("写入文件%u At%llu Size=%u WriteSize=%llu FinishSize=%llu",
						pData->SerialNumber, pData->Offset, pData->DataBuffer.GetUsedSize(), WriteLen, m_FinishSize);
				}				

				if (pData->IsLast)
				{
					CEasyTime Time(m_FileLastWriteTime);
					m_pTaskFile->SetLastWriteTime(Time);
					m_HashMD5.MD5Final();
					m_FileMD5 = m_HashMD5.GetHashCodeString();
					LogDebug("写入文件完成,WriteSize=%llu,MD5=%s,FilePath=%s,FileTime=%u", m_FinishSize, (LPCTSTR)m_FileMD5, (LPCTSTR)m_FilePath, m_FileLastWriteTime);
					ChangeStatus(TASK_STATUS_END);
				}
				if (pData->NeedAck)
				{
					AddFinishOperation(pData);
				}
				else
				{
					AtomicDec(&m_QueryCount);
					m_pManager->DeleteFileDataInfo(pData);
				}
				FinishSerialOperation(Pos);
			}
		}
		break;
	}
	return ProcessCount;
}
int CFileTask::DoParallelOperation(int ProcessLimit, CEasyBuffer& CompressBuffer)
{
	int ProcessCount = 0;
	FILE_DATA_BLOCK * pData = NULL;
	if (m_ParallelOperations.PopFront(&pData))
	{
		ProcessCount++;
		LogDebug("开始处理数据块%u", pData->SerialNumber);
		switch (m_TaskType)
		{
		case TASK_TYPE_READ:
			{
				size_t PackSize = CompressBuffer.GetBufferSize() - LZMA_PROPS_SIZE;
				size_t PackPropSize = LZMA_PROPS_SIZE;

				int ErrCode = LzmaCompress(((BYTE *)CompressBuffer.GetBuffer()) + LZMA_PROPS_SIZE, &PackSize,
					(BYTE *)pData->DataBuffer.GetBuffer(), pData->DataBuffer.GetUsedSize(),
					(BYTE *)CompressBuffer.GetBuffer(), &PackPropSize, 3,
					1 << 20, 3, 0, 2, 32, 2);

				if (ErrCode == SZ_OK)
				{
					PackSize += LZMA_PROPS_SIZE;
					LogDebug(_T("%d文件块压缩成功%u->%u"), pData->SerialNumber, pData->DataBuffer.GetUsedSize(), PackSize);

					if (pData->DataBuffer.GetBufferSize() < PackSize)
						pData->DataBuffer.Create(PackSize);
					pData->DataBuffer.SetUsedSize(0);
					if (pData->DataBuffer.PushBack(CompressBuffer.GetBuffer(), PackSize))
					{
						pData->DataBuffer.SetUsedSize(PackSize);
						AddFinishOperation(pData);
						if (pData->IsLast)
							ChangeStatus(TASK_STATUS_END);
					}
					else
					{
						m_pManager->DeleteFileDataInfo(pData);
						Log("压缩时缓冲溢出");
						ChangeStatus(TASK_STATUS_ERROR);
					}
				}			
				else
				{
					m_pManager->DeleteFileDataInfo(pData);
					Log(_T("压缩失败"));
					ChangeStatus(TASK_STATUS_ERROR);
				}
			}
			break;			
		case TASK_TYPE_WRITE:
			if(pData->OriginSize)
			{
				size_t UnpackSize = pData->OriginSize;
				size_t SourceSize = pData->DataBuffer.GetUsedSize() - LZMA_PROPS_SIZE;

				int ErrCode = LzmaUncompress((BYTE *)CompressBuffer.GetBuffer(), &UnpackSize,
					((BYTE *)pData->DataBuffer.GetBuffer()) + LZMA_PROPS_SIZE, &SourceSize,
					(BYTE *)pData->DataBuffer.GetBuffer(), LZMA_PROPS_SIZE);
				if (ErrCode == SZ_OK)
				{
					if (pData->DataBuffer.GetBufferSize() < UnpackSize)
						pData->DataBuffer.Create(UnpackSize);
					pData->DataBuffer.SetUsedSize(0);
					if (pData->DataBuffer.PushBack(CompressBuffer.GetBuffer(), UnpackSize))
					{
						if (UnpackSize == pData->OriginSize)
						{
							pData->DataBuffer.SetUsedSize(UnpackSize);
							LogDebug(_T("%d文件块解压成功%u->%u"), pData->SerialNumber, SourceSize, UnpackSize);
							AddSerialOperation(pData);
						}
						else
						{
							m_pManager->DeleteFileDataInfo(pData);
							Log("%d解压后大小和原始大小不符(%u/%u)", pData->SerialNumber, UnpackSize, pData->OriginSize);
							ChangeStatus(TASK_STATUS_ERROR);
						}
					}
					else
					{
						m_pManager->DeleteFileDataInfo(pData);
						Log("%d解压时缓冲溢出", pData->SerialNumber);
						ChangeStatus(TASK_STATUS_ERROR);
					}
				}
				else
				{
					m_pManager->DeleteFileDataInfo(pData);
					Log(_T("%d解压失败"), pData->SerialNumber);
					ChangeStatus(TASK_STATUS_ERROR);
				}
			}
			else
			{
				AddSerialOperation(pData);
			}
			break;
		default:
			m_pManager->DeleteFileDataInfo(pData);
		}

	}
	return ProcessCount;
}

void CFileTask::AddSerialOperation(FILE_DATA_BLOCK * pData)
{
	CAutoLock Lock(m_SerialLock);	
	m_SerialOperations.InsertSorted(pData);	
}
FILE_DATA_BLOCK * CFileTask::GetSerialOperation(LPVOID& Pos)
{
	CAutoLock Lock(m_SerialLock);
	Pos = m_SerialOperations.GetFirstObjectPos();
	if (Pos)
	{
		FILE_DATA_BLOCK * pData = *m_SerialOperations.GetObject(Pos);
		if (pData->SerialNumber == (m_LastProcessedSerialNumber + 1))
		{
			m_LastProcessedSerialNumber = pData->SerialNumber;			
			m_SerialOperations.DeleteObjectByPos(Pos);
			return pData;
		}
	}
	return NULL;
}
bool CFileTask::FinishSerialOperation(LPVOID Pos)
{
	CAutoLock Lock(m_SerialLock);
	return m_SerialOperations.DeleteObjectByPos(Pos) != FALSE;
}
void CFileTask::AddFinishOperation(FILE_DATA_BLOCK * pData)
{
	CAutoLock Lock(m_FinishLock);
	if (m_FinishQueue.InsertSorted(pData) == NULL)
	{
		Log("完成队列已满%u/%u", m_FinishQueue.GetObjectCount(), m_FinishQueue.GetBufferSize());
	}
}

UINT CFileTask::GetSerialOperationCount()
{
	CAutoLock Lock(m_SerialLock);
	return m_SerialOperations.GetObjectCount();
}
UINT CFileTask::GetParallelOperationCount()
{
	return m_ParallelOperations.GetUsedSize();
}

FILE_DATA_BLOCK * CFileTask::GetFinishOperation(bool IgnoreSequence)
{
	CAutoLock Lock(m_FinishLock);
	LPVOID Pos = m_FinishQueue.GetFirstObjectPos();
	if (Pos)
	{
		FILE_DATA_BLOCK * pData = *m_FinishQueue.GetObject(Pos);
		if ((pData->SerialNumber == (m_LastFinishedSerialNumber + 1)) || IgnoreSequence)
		{
			if (!IgnoreSequence)
				m_LastFinishedSerialNumber = pData->SerialNumber;
			m_FinishQueue.DeleteObjectByPos(Pos);
			AtomicDec(&m_QueryCount);
			return pData;
		}
	}
	return NULL;
}

UINT CFileTask::GetFinishOperationCount()
{
	CAutoLock Lock(m_FinishLock);
	return m_FinishQueue.GetObjectCount();
}

void CFileTask::DumpFinishQueue()
{
	CAutoLock Lock(m_FinishLock);
	LPVOID Pos = m_FinishQueue.GetFirstObjectPos();
	while (Pos)
	{
		FILE_DATA_BLOCK* pData = *m_FinishQueue.GetNextObject(Pos);
		Log("NO.=%u,Offset=%llu,Size=%u(%u)", pData->SerialNumber, pData->Offset, pData->OriginSize, pData->DataBuffer.GetUsedSize());
	}
	Log("一共%u个数据块", m_FinishQueue.GetObjectCount());
}

void CFileTask::DumpParallelQueue()
{
	Log("一共%u个数据块", m_ParallelOperations.GetUsedSize());
	FILE_DATA_BLOCK* pData = NULL;
	while (m_ParallelOperations.PopFront(&pData))
	{
		Log("NO.=%u,Offset=%llu,Size=%u(%u)", pData->SerialNumber, pData->Offset, pData->OriginSize, pData->DataBuffer.GetUsedSize());
		m_pManager->DeleteFileDataInfo(pData);
	}
}