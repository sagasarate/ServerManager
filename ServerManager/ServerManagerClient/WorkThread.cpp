#include "stdafx.h"



CWorkThread::CWorkThread()
{
	m_pParent = NULL;
	m_Status = WORK_STATUS_NONE;
	m_CompressBuffer.Create(MAX_MSG_SIZE);
	m_pTaskInfo = NULL;
	m_pFileData = NULL;
}


CWorkThread::~CWorkThread()
{
}

bool CWorkThread::Init(CTaskQueue * pParent)
{
	m_pParent = pParent;
	return true;
}

BOOL CWorkThread::OnRun()
{
	switch (m_Status)
	{
	case WORK_STATUS_LOAD:
		{
			CAutoLock Lock(m_StatusLock);
			if (m_pTaskInfo)
			{
				CWorkThread * pThread = m_pParent->AllocWorkThread();
				if(pThread)
				{
					FILE_DATA_INFO * pFileData = m_pParent->AllocFileDataInfo(FILE_TRANSFER_BLOCK_SIZE);
					if (pFileData)
					{
						UINT64 ReadLen = m_pTaskInfo->TaskFile.Read(pFileData->DataBuffer.GetBuffer(), FILE_TRANSFER_BLOCK_SIZE);

						
						pFileData->DataBuffer.SetUsedSize(ReadLen);
						pFileData->OriginSize = ReadLen;
						pFileData->Index = m_CurBlockIndex++;

						m_pTaskInfo->FinishSize += ReadLen;

						m_HashMD5.AddData((BYTE *)pFileData->DataBuffer.GetBuffer(), ReadLen);

						if (ReadLen < FILE_TRANSFER_BLOCK_SIZE)
						{
							pFileData->IsLast = true;
							m_HashMD5.MD5Final();
							m_pTaskInfo->FileMD5 = m_HashMD5.GetHashCodeString();
							m_Status = WORK_STATUS_NONE;
							LogDebug("文件%s读取完成,大小%llu,MD5=%s", (LPCTSTR)m_pTaskInfo->SourceFilePath, m_pTaskInfo->FinishSize, (LPCTSTR)m_pTaskInfo->FileMD5);
							//m_pParent->OnTaskFinish(m_pTaskInfo);
						}
						pThread->AddFileWork(m_pTaskInfo, pFileData, true);
					}				
				}
			}
			else
			{
				m_Status = WORK_STATUS_NONE;
			}
		}
		break;
	case WORK_STATUS_SAVE:
		{
			CAutoLock Lock(m_StatusLock);
			if (m_pTaskInfo)
			{
				FILE_DATA_INFO * pFileData = m_pParent->GetFinishedFileData(m_CurBlockIndex);
				bool IsLastData = false;
				if (pFileData)
				{
					IsLastData = pFileData->IsLast;
					m_CurBlockIndex++;
					m_HashMD5.AddData((BYTE *)pFileData->DataBuffer.GetBuffer(), pFileData->DataBuffer.GetUsedSize());
					UINT64 WriteLen = m_pTaskInfo->TaskFile.Write(pFileData->DataBuffer.GetBuffer(), pFileData->DataBuffer.GetUsedSize());
					m_pTaskInfo->FinishSize += WriteLen;					
					m_pTaskInfo->DecQueryCount();
					m_pParent->DeleteFileDataInfo(pFileData);
				}

				if ((m_pTaskInfo->FinishSize >= m_pTaskInfo->FileSize) || IsLastData)
				{
					m_HashMD5.MD5Final();
					m_pTaskInfo->FileMD5 = m_HashMD5.GetHashCodeString();
					LogDebug("文件%s写入完成,大小%llu,MD5=%s", (LPCTSTR)m_pTaskInfo->TargetFilePath, m_pTaskInfo->FinishSize, (LPCTSTR)m_pTaskInfo->FileMD5);
					m_pParent->OnTaskFinish(m_pTaskInfo);
					m_Status = WORK_STATUS_NONE;
				}				
			}
			else
			{
				m_Status = WORK_STATUS_NONE;
			}
		}
		break;
	case WORK_STATUS_MD5:
		{
			CAutoLock Lock(m_StatusLock);
			if (m_pTaskInfo)
			{		
				UINT64 ReadLen = m_pTaskInfo->TaskFile.Read(m_CompressBuffer.GetBuffer(), m_CompressBuffer.GetBufferSize());

				if (ReadLen)
				{
					m_HashMD5.AddData((BYTE *)m_CompressBuffer.GetBuffer(), ReadLen);
				}

				if (ReadLen < m_CompressBuffer.GetBufferSize())
				{
					m_HashMD5.MD5Final();
					m_pTaskInfo->FileMD5 = m_HashMD5.GetHashCodeString();
					m_pParent->OnTaskFinish(m_pTaskInfo);
					m_Status = WORK_STATUS_NONE;					
				}
					
			}
			else
			{
				m_Status = WORK_STATUS_NONE;
			}
		}
		break;
	case WORK_STATUS_COMPRESS:
		{
			CAutoLock Lock(m_StatusLock);
			if (m_pTaskInfo&&m_pFileData)
			{
				if(m_pFileData->DataBuffer.GetUsedSize())
				{
					size_t PackSize = m_CompressBuffer.GetBufferSize() - LZMA_PROPS_SIZE;
					size_t PackPropSize = LZMA_PROPS_SIZE;

					int ErrCode = LzmaCompress(((BYTE *)m_CompressBuffer.GetBuffer()) + LZMA_PROPS_SIZE, &PackSize,
						(BYTE *)m_pFileData->DataBuffer.GetBuffer(), m_pFileData->DataBuffer.GetUsedSize(),
						(BYTE *)m_CompressBuffer.GetBuffer(), &PackPropSize, 3,
						1 << 20, 3, 0, 2, 32, 2);

					if (ErrCode == SZ_OK)
					{
						PackSize += LZMA_PROPS_SIZE;
						if (m_pFileData->DataBuffer.GetBufferSize() < PackSize)
							m_pFileData->DataBuffer.Create(PackSize);
						m_pFileData->DataBuffer.SetUsedSize(0);
						m_pFileData->DataBuffer.PushBack(m_CompressBuffer.GetBuffer(), PackSize);
						m_pFileData->DataBuffer.SetUsedSize(PackSize);
						m_pParent->OnWorkFinish(m_pFileData);
					}
					else
					{
						m_pParent->OnTaskError(m_pTaskInfo);
					}
				}
				else
				{
					m_pFileData->DataBuffer.SetUsedSize(0);
					m_pParent->OnWorkFinish(m_pFileData);
				}
			}
			m_Status = WORK_STATUS_NONE;
		}
		break;
	case WORK_STATUS_UNCOMPRESS:
		{
			CAutoLock Lock(m_StatusLock);
			if (m_pTaskInfo&&m_pFileData)
			{
				size_t UnpackSize = m_pFileData->OriginSize;
				size_t SourceSize = m_pFileData->DataBuffer.GetUsedSize() - LZMA_PROPS_SIZE;

				int ErrCode = LzmaUncompress((BYTE *)m_CompressBuffer.GetBuffer(), &UnpackSize,
					((BYTE *)m_pFileData->DataBuffer.GetBuffer()) + LZMA_PROPS_SIZE, &SourceSize,
					(BYTE *)m_pFileData->DataBuffer.GetBuffer(), LZMA_PROPS_SIZE);
				if (ErrCode == SZ_OK)
				{
					if (m_pFileData->DataBuffer.GetBufferSize() < UnpackSize)
						m_pFileData->DataBuffer.Create(UnpackSize);
					m_pFileData->DataBuffer.SetUsedSize(0);
					m_pFileData->DataBuffer.PushBack(m_CompressBuffer.GetBuffer(), UnpackSize);
					m_pFileData->DataBuffer.SetUsedSize(UnpackSize);
					m_pParent->OnWorkFinish(m_pFileData);
				}
				else
				{
					m_pParent->OnTaskError(m_pTaskInfo);
					
				}
			}
			m_Status = WORK_STATUS_NONE;
		}
		break;
	default:
		DoSleep(1);
		break;
	}
	
	return TRUE;
}

bool CWorkThread::AddTask(TASK_INFO * pTaskInfo)
{
	CAutoLock Lock(m_StatusLock);
	switch (pTaskInfo->Type)
	{
	case TASK_TYPE_DOWNLOAD:
		m_pTaskInfo = pTaskInfo;
		m_CurBlockIndex = 0;
		m_HashMD5.Init();
		m_Status = WORK_STATUS_SAVE;
		break;
	case TASK_TYPE_UPLOAD:
		m_pTaskInfo = pTaskInfo;
		m_CurBlockIndex = 0;
		m_HashMD5.Init();
		m_Status = WORK_STATUS_LOAD;
		break;
	case TASK_TYPE_COMPARE:
		m_pTaskInfo = pTaskInfo;
		m_Status = WORK_STATUS_MD5;
		break;
	}
	return false;
}
bool CWorkThread::AddFileWork(TASK_INFO * pTaskInfo, FILE_DATA_INFO * pFileData, bool IsCompress)
{
	CAutoLock Lock(m_StatusLock);
	m_pTaskInfo = pTaskInfo;
	m_pFileData = pFileData;
	if(IsCompress)
		m_Status = WORK_STATUS_COMPRESS;
	else
		m_Status = WORK_STATUS_UNCOMPRESS;
	return true;
}
