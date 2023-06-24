#pragma once

class CTaskManager;

class CParallelWorkThread :
	public CEasyThread
{
protected:
	CTaskManager *								m_pParent;
	CThreadSafeStaticMap<UINT, CBaseTask *>		m_TaskList;
	CEasyBuffer									m_CompressBuffer;
public:
	CParallelWorkThread();
	~CParallelWorkThread();

	bool Init(CTaskManager * pParent);
	bool AddTask(CBaseTask * pTask);
	virtual BOOL OnRun();
};

