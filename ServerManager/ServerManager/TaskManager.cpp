#include "stdafx.h"



CTaskManager::CTaskManager()
{
	m_pParent = NULL;	
}


CTaskManager::~CTaskManager()
{
}

void CTaskManager::Destory()
{
	for (CSerialWorkThread& Thread : m_SerialWorkThreadList)
	{
		Thread.SafeTerminate();
	}
	m_SerialWorkThreadList.Clear();

	for (CParallelWorkThread& Thread : m_ParallelWorkThreadList)
	{
		Thread.SafeTerminate();
	}
	m_ParallelWorkThreadList.Clear();

	m_CanceledTaskList.Clear();
	m_ActiveTaskList.Destory();
	m_FileTaskPool.Destory();	
	m_FileDataPool.Destory();
	m_pParent = NULL;
}

bool CTaskManager::Init(CServerManagerService * pParent)
{
	m_pParent = pParent;
	if (!m_FileTaskPool.Create(CMainConfig::GetInstance()->GetPoolConfigs().FileTaskPoolConfig))
	{
		Log("文件任务对象池创建失败");
		return false;
	}
	if (!m_ActiveTaskList.Create(CMainConfig::GetInstance()->GetPoolConfigs().FileTaskPoolConfig))
	{
		Log("活动任务列表创建失败");
		return false;
	}
	if (!m_FileDataPool.Create(CMainConfig::GetInstance()->GetPoolConfigs().FileDataBlockPoolConfig))
	{
		Log("文件数据块对象池创建失败");
		return false;
	}
	m_SerialWorkThreadList.Resize(CMainConfig::GetInstance()->GetTaskManagerConfig().SerialWorkThreadCount);
	for (CSerialWorkThread& Thread : m_SerialWorkThreadList)
	{
		Thread.Init(this);
		Thread.Start();
	}
	m_ParallelWorkThreadList.Resize(CMainConfig::GetInstance()->GetTaskManagerConfig().ParallelWorkThreadCount);
	for (CParallelWorkThread& Thread : m_ParallelWorkThreadList)
	{
		Thread.Init(this);
		Thread.Start();
	}
	m_TaskStatTimer.SaveTime();
	return true;
}



int CTaskManager::AddDownloadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR FilePath, UINT64 StartOffset, CFileTask ** ppTask)
{
	if (m_ActiveTaskList.Find(ConnectionID) == NULL)
	{
		CFileTask* pTask = m_FileTaskPool.NewObject();
		if (pTask)
		{
			if (pTask->Create(this, ConnectionID, ServiceID, TASK_TYPE_READ, FilePath, StartOffset))
			{
				if (AddTaskToWorkThread(pTask))
				{
					m_ActiveTaskList.Insert(ConnectionID, pTask);
					if (ppTask)
						*ppTask = pTask;
					return MSG_RESULT_SUCCEED;
				}
				else
				{
					DeleteTask(pTask);
					LogDebug(_T("将任务加入工作线程失败"));
					return MSG_RESULT_FAILED;
				}
			}
			else
			{
				DeleteTask(pTask);
				LogDebug(_T("创建任务失败"));
				return MSG_RESULT_FAILED;
			}
		}
		else
		{
			LogDebug(_T("文件任务池已满%u/%u"), m_FileTaskPool.GetObjectCount(), m_FileTaskPool.GetBufferSize());
			return MSG_RESULT_FAILED;
		}
	}
	else
	{
		return MSG_RESULT_IS_BUSY;
	}
}
int CTaskManager::AddUploadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR FilePath, UINT64 StartOffset, CFileTask ** ppTask)
{
	if (m_ActiveTaskList.Find(ConnectionID) == NULL)
	{
		CFileTask* pTask = m_FileTaskPool.NewObject();
		if (pTask)
		{
			if (pTask->Create(this, ConnectionID, ServiceID, TASK_TYPE_WRITE, FilePath, StartOffset))
			{
				if (AddTaskToWorkThread(pTask))
				{
					m_ActiveTaskList.Insert(ConnectionID, pTask);
					if (ppTask)
						*ppTask = pTask;
					return MSG_RESULT_SUCCEED;
				}
				else
				{
					DeleteTask(pTask);
					LogDebug(_T("将任务加入工作线程失败"));
					return MSG_RESULT_FAILED;
				}
			}
			else
			{
				DeleteTask(pTask);
				LogDebug(_T("创建任务失败"));
				return MSG_RESULT_FAILED;
			}
		}
		else
		{
			LogDebug(_T("文件任务池已满%u/%u"), m_FileTaskPool.GetObjectCount(), m_FileTaskPool.GetBufferSize());
			return MSG_RESULT_FAILED;
		}
	}
	else
	{
		return MSG_RESULT_IS_BUSY;
	}
}


int CTaskManager::AddFileCompareTask(UINT ConnectionID, UINT ServiceID, LPCTSTR FilePath, LPCTSTR FileMD5, CFileTask ** ppTask)
{
	if (m_ActiveTaskList.Find(ConnectionID) == NULL)
	{
		CFileTask* pTask = m_FileTaskPool.NewObject();
		if (pTask)
		{
			if (pTask->Create(this, ConnectionID, ServiceID, TASK_TYPE_MAKE_MD5, FilePath, 0))
			{
				pTask->SetCompreMD5(FileMD5);
				if (AddTaskToWorkThread(pTask))
				{
					m_ActiveTaskList.Insert(ConnectionID, pTask);
					if (ppTask)
						*ppTask = pTask;
					return MSG_RESULT_SUCCEED;
				}
				else
				{
					DeleteTask(pTask);
					LogDebug(_T("将任务加入工作线程失败"));
					return MSG_RESULT_FAILED;
				}
			}
			else
			{
				DeleteTask(pTask);
				LogDebug(_T("创建任务失败"));
				return MSG_RESULT_FAILED;
			}
		}
		else
		{
			LogDebug(_T("文件任务池已满%u/%u"), m_FileTaskPool.GetObjectCount(), m_FileTaskPool.GetBufferSize());
			return MSG_RESULT_FAILED;
		}
	}
	else
	{
		return MSG_RESULT_IS_BUSY;
	}
}



int CTaskManager::Update(int ProcessPacketLimit)
{
	int ProcessCount = 0;
	UINT CurTime = CEasyTimer::GetTime();

	for (CFileTask* pTask : m_ActiveTaskList)
	{
		ProcessCount++;
		if (pTask->IsTimeOut(CMainConfig::GetInstance()->GetTaskManagerConfig().TaskTimeOut))
		{
			Log("任务%u超时", pTask->GetServiceID());
			pTask->CancelTask();
		}		
		if (pTask->GetStatus() == TASK_STATUS_CANCEL)
		{
			RemoveActiveTask(pTask);
		}
	}

	for (int i = (int)m_CanceledTaskList.GetCount() - 1; i >= 0; i--)
	{
		CFileTask* pTask = m_CanceledTaskList[i];
		if (pTask->GetUseRef() == 1)
		{
			DeleteTask(pTask);
			m_CanceledTaskList.Delete(i);
		}
	}

	if (ProcessCount&&m_TaskStatTimer.IsTimeOut(TASK_STAT_TIME))
	{
		m_TaskStatTimer.SaveTime();
		LogDebug("现有任务%u个，活跃任务%u个", m_FileTaskPool.GetObjectCount(), m_ActiveTaskList.GetObjectCount());
	}

	return ProcessCount;
}


FILE_DATA_BLOCK * CTaskManager::AllocFileDataInfo(UINT BufferSize)
{
	FILE_DATA_BLOCK * pFileData = m_FileDataPool.NewObject();
	if (pFileData)
	{
		if (pFileData->DataBuffer.GetBufferSize() < BufferSize)
			pFileData->DataBuffer.Create(BufferSize);
		pFileData->DataBuffer.SetUsedSize(0);
	}
	else
	{
		Log("数据缓冲已满%u/%u", m_FileDataPool.GetObjectCount(), m_FileDataPool.GetBufferSize());
	}
	return pFileData;
}
void CTaskManager::DeleteFileDataInfo(FILE_DATA_BLOCK * pInfo)
{
	m_FileDataPool.DeleteObject(pInfo->DataID);
}

bool CTaskManager::CancelTask(UINT ConnectionID)
{	
	CFileTask * pTask = GetTask(ConnectionID);
	if (pTask)
	{		
		LogDebug("放弃任务%s", (LPCTSTR)pTask->GetFilePath());
		pTask->CancelTask();
		RemoveActiveTask(pTask);
		return true;
	}
	return false;
}

bool CTaskManager::AddTaskToWorkThread(CBaseTask * pTask)
{
	UINT Index = GetRand(0, m_SerialWorkThreadList.GetCount() - 1);
	if (m_SerialWorkThreadList[Index].AddTask(pTask))
	{
		bool IsAdd = false;
		for (CParallelWorkThread& Thread : m_ParallelWorkThreadList)
		{
			if (Thread.AddTask(pTask))
			{
				IsAdd = true;
			}
		}
		if (IsAdd)
		{
			return true;
		}
		else
		{
			Log("添加任务到工作线程失败");
			pTask->CancelTask();
			return false;
		}
	}
	return false;
}

bool CTaskManager::DeleteTask(CBaseTask* pTask)
{		
	LogDebug("删除任务%s", (LPCTSTR)((CFileTask*)pTask)->GetFilePath());
	UINT TaskID = pTask->GetID();
	pTask->Destory();
	return m_FileTaskPool.DeleteObject(TaskID) != FALSE;
}

bool CTaskManager::RemoveActiveTask(CFileTask* pTask)
{
	LogDebug("移除活动任务%s", (LPCTSTR)pTask->GetFilePath());
	if (m_ActiveTaskList.Delete(pTask->GetClientID()))
	{
		m_CanceledTaskList.Add(pTask);
		return true;
	}
	return false;
}