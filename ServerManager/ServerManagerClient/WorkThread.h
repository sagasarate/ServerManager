#pragma once

class CTaskQueue;

class CWorkThread :
	public CEasyThread
{
protected:
	CTaskQueue *				m_pParent;
	volatile WORK_STATUS		m_Status;
	CEasyCriticalSection		m_StatusLock;
	CEasyBuffer					m_CompressBuffer;
	TASK_INFO *					m_pTaskInfo;
	FILE_DATA_INFO *			m_pFileData;
	CHashMD5					m_HashMD5;
	UINT						m_CurBlockIndex;
public:
	CWorkThread();
	~CWorkThread();

	bool Init(CTaskQueue * pParent);
	virtual BOOL OnRun();

	bool AddTask(TASK_INFO * pTaskInfo);
	bool AddFileWork(TASK_INFO * pTaskInfo, FILE_DATA_INFO * pFileData, bool IsCompress);
	void ClearWork()
	{
		CAutoLock Lock(m_StatusLock);
		m_Status = WORK_STATUS_NONE;
		m_pTaskInfo = NULL;
		m_pFileData = NULL;
	}

	bool IsIdle()
	{
		return m_Status == WORK_STATUS_NONE;
	}
};

