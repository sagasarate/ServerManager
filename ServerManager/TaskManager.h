#pragma once

class CServerManagerService;
class CTaskManager 
{
public:
	
protected:
	
	CServerManagerService *						m_pParent;
	CIDStorage<CFileTask>						m_FileTaskPool;
	CStaticMap<UINT, CFileTask*>				m_ActiveTaskList;
	CEasyArray<CFileTask*>						m_CanceledTaskList;

	CThreadSafeIDStorage<FILE_DATA_BLOCK>		m_FileDataPool;
	
	CEasyArray<CSerialWorkThread>				m_SerialWorkThreadList;
	CEasyArray<CParallelWorkThread>				m_ParallelWorkThreadList;

	CEasyTimer									m_TaskStatTimer;
public:
	CTaskManager();
	~CTaskManager();

	bool Init(CServerManagerService * pParent);
	void Destory();

	int AddDownloadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR FilePath, UINT64 StartOffset, CFileTask ** ppTask);
	int AddUploadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR FilePath, UINT64 StartOffset, CFileTask ** ppTask);
	int AddFileCompareTask(UINT ConnectionID, UINT ServiceID, LPCTSTR FilePath, LPCTSTR FileMD5, CFileTask ** ppTask);

	int Update(int ProcessPacketLimit);

	FILE_DATA_BLOCK * AllocFileDataInfo(UINT BufferSize);
	void DeleteFileDataInfo(FILE_DATA_BLOCK * pInfo);
	CFileTask * GetTask(UINT ConnectionID)
	{
		CFileTask** ppTask = m_ActiveTaskList.Find(ConnectionID);;
		if (ppTask)
			return *ppTask;
		return NULL;
	}
	bool CancelTask(UINT ConnectionID);
protected:
	bool AddTaskToWorkThread(CBaseTask * pTask);
	bool DeleteTask(CBaseTask* pTask);
	bool RemoveActiveTask(CFileTask* pTask);
};

