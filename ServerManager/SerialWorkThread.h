#pragma once

class CTaskManager;

class CSerialWorkThread :
	public CEasyThread
{
protected:
	CTaskManager *								m_pParent;
	CThreadSafeStaticMap<UINT, CBaseTask *>		m_TaskList;
public:
	CSerialWorkThread();
	~CSerialWorkThread();

	bool Init(CTaskManager * pParent);
	bool AddTask(CBaseTask * pTask);
	virtual BOOL OnRun();
};

