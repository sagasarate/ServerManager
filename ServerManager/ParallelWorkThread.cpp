#include "stdafx.h"



CParallelWorkThread::CParallelWorkThread()
{
	m_pParent = NULL;
}


CParallelWorkThread::~CParallelWorkThread()
{
}

bool CParallelWorkThread::Init(CTaskManager * pParent)
{
	m_pParent = pParent;
	if (!m_TaskList.Create(CMainConfig::GetInstance()->GetPoolConfigs().FileTaskPoolConfig))
	{
		Log("任务池创建失败");
		return false;
	}
	m_CompressBuffer.Create(FILE_TRANSFER_BLOCK_SIZE * 3);
	return true;
}

BOOL CParallelWorkThread::OnRun()
{
	int ProcessCount = 0;

	for (CBaseTask * pTask : m_TaskList)
	{
		ProcessCount += pTask->DoParallelOperation(32, m_CompressBuffer);
		if (pTask->GetStatus() == TASK_STATUS_FINISH)
		{
			if (pTask->GetParallelOperationCount() == 0)
			{
				pTask->DecUseRef();
				LogDebug("任务%u已移除", pTask->GetID());
				m_TaskList.Delete(pTask->GetID());				
			}
		}
		else if (pTask->GetStatus() != TASK_STATUS_PROCESSING)
		{
			pTask->DecUseRef();
			LogDebug("任务%u(%d)已移除", pTask->GetID(), pTask->GetStatus());
			m_TaskList.Delete(pTask->GetID());
		}
	}
	
	if (ProcessCount == 0)
		DoSleep(1);
	return TRUE;
}

bool CParallelWorkThread::AddTask(CBaseTask * pTask)
{
	if (m_TaskList.Find(pTask->GetID()) == NULL)
	{
		pTask->AddUseRef();
		if (m_TaskList.Insert(pTask->GetID(), pTask))
		{
			LogDebug("任务%u已添加", pTask->GetID());
			return true;
		}
		else
		{
			pTask->DecUseRef();
			return false;
		}
	}
	else
	{
		Log("%u已经存在", pTask->GetID());
	}
	return false;
}
