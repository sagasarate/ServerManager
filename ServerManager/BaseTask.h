#pragma once

class CTaskManager;

class CBaseTask :
	public CNameObject
{
protected:
	typedef void(*TASK_DESTORY_CALLBACK_FN)(BYTE* pCallParam, CBaseTask* pTask, LPVOID Param);

	template<typename ClassName, typename FunName>
	class CCallbackDispatcherHelper
	{
	public:
		static void CallbackDispatcher(BYTE* pCallParam, CBaseTask* pTask, LPVOID Param)
		{
			ClassName* pObject = *((ClassName**)pCallParam);
			FunName pFun = *(FunName*)(pCallParam + sizeof(ClassName*));
			(pObject->*pFun)(pTask, Param);
		}
	};

	TASK_DESTORY_CALLBACK_FN	m_CallbackFN;
	BYTE						m_CallParam[32];
	LPVOID						m_Param;

	CTaskManager *				m_pManager;
	TASK_TYPE					m_TaskType;
	UINT						m_CreateTime;
	//volatile bool				m_NeedCancel;
private:
	volatile TASK_STATUS		m_Status;

public:
	CBaseTask();
	~CBaseTask();

	virtual void Destory();

	TASK_TYPE GetType()
	{
		return m_TaskType;
	}
	TASK_STATUS GetStatus()
	{
		return m_Status;
	}
	//bool NeedCancel()
	//{
	//	return m_NeedCancel;
	//}
	void InitStatus()
	{
		m_Status = TASK_STATUS_PROCESSING;
	}
	void CancelTask()
	{		
		m_Status = TASK_STATUS_CANCEL;		
	}
	template<typename ClassName, typename FunName>
	void WaitFinish(LPVOID Param, ClassName* pObject, FunName Fun)
	{		
		m_Param = Param;
		m_CallbackFN = CCallbackDispatcherHelper<ClassName, FunName>::CallbackDispatcher;
		memcpy(m_CallParam, &pObject, sizeof(pObject));
		memcpy(m_CallParam + sizeof(pObject), &Fun, sizeof(Fun));
	}
	bool IsInWaitFinish()
	{
		return (m_CallbackFN != NULL);
	}
	void CallFinishCallback()
	{
		if (m_CallbackFN != NULL)
		{
			m_CallbackFN(m_CallParam, this, m_Param);
			m_CallbackFN = NULL;
		}
	}
	void ChangeStatus(TASK_STATUS Status)
	{
		AtomicCompareAndSet((volatile unsigned int *)&m_Status, TASK_STATUS_PROCESSING, Status);
		AtomicCompareAndSet((volatile unsigned int *)&m_Status, TASK_STATUS_FINISH, Status);
	}
	bool IsTimeOut(UINT TimeOut)
	{
		return (time(NULL) - m_CreateTime) >= TimeOut;
	}
	virtual UINT GetParallelOperationCount() = 0;
	virtual int DoSerialOperation(int ProcessLimit) = 0;
	virtual int DoParallelOperation(int ProcessLimit, CEasyBuffer& CompressBuffer) = 0;
};

