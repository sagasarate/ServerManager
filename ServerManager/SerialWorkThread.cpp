#include "stdafx.h"



CSerialWorkThread::CSerialWorkThread()
{
	m_pParent = NULL;
}


CSerialWorkThread::~CSerialWorkThread()
{
}

bool CSerialWorkThread::Init(CTaskManager * pParent)
{
	m_pParent = pParent;
	if (!m_TaskList.Create(CMainConfig::GetInstance()->GetPoolConfigs().FileTaskPoolConfig))
	{
		Log("任务池创建失败");
		return false;
	}
	return true;
}

BOOL CSerialWorkThread::OnRun()
{
	int ProcessCount = 0;

	for (CBaseTask * pTask : m_TaskList)
	{
		pTask->DoSerialOperation(32);
		if (pTask->GetStatus() != TASK_STATUS_PROCESSING)
		{
			pTask->DecUseRef();
			m_TaskList.Delete(pTask->GetID());
		}
	}
	
	if (ProcessCount == 0)
		DoSleep(1);
	return TRUE;
}

bool CSerialWorkThread::AddTask(CBaseTask * pTask)
{
	if (m_TaskList.Find(pTask->GetID()) == NULL)
	{
		pTask->AddUseRef();
		if (m_TaskList.Insert(pTask->GetID(), pTask))
		{
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
