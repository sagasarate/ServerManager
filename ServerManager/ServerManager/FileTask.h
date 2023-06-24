#pragma once

class CFileTask :
	public CBaseTask
{
protected:
	UINT											m_ServiceID;
	UINT											m_ClientID;
	CEasyString										m_FilePath;
	IFileAccessor *									m_pTaskFile;
	UINT											m_FileMode;
	UINT64											m_FileSize;
	UINT64											m_StartOffset;
	UINT64											m_FinishSize;
	bool											m_ContinueTransfer;
	UINT											m_TransferStartTime;	
	time_t											m_FileLastWriteTime;
	CHashMD5										m_HashMD5;
	CEasyString										m_FileMD5;
	CEasyString										m_CompareMD5;
	UINT											m_SerialNumber;
	UINT											m_LastProcessedSerialNumber;
	UINT											m_LastFinishedSerialNumber;
	
	
	CIDStorage<FILE_DATA_BLOCK_POINTER>				m_SerialOperations;
	CEasyCriticalSection							m_SerialLock;
	CThreadSafeCycleQueue<FILE_DATA_BLOCK *>		m_ParallelOperations;
	CIDStorage<FILE_DATA_BLOCK_POINTER>				m_FinishQueue;
	CEasyCriticalSection							m_FinishLock;
	volatile UINT									m_QueryCount;

public:
	CFileTask();
	~CFileTask();

	virtual void Destory();

	bool Create(CTaskManager *	pManager, UINT ClientID, UINT ServiceID, TASK_TYPE Type, const CEasyString& FilePath, UINT64 StartOffset);

	bool AddWriteQuery(UINT Length, const CEasyBuffer& FileData, bool IsLast, bool NeedAck);
	bool AddReadQuery();

	virtual int DoSerialOperation(int ProcessLimit);
	virtual int DoParallelOperation(int ProcessLimit, CEasyBuffer& CompressBuffer);

	void SetCompreMD5(LPCTSTR MD5)
	{
		m_CompareMD5 = MD5;
	}
	const CEasyString& GetCompareMD5()
	{
		return m_CompareMD5;
	}
	UINT64 GetFileSize()
	{
		return m_FileSize;
	}
	time_t GetFileLastWriteTime()
	{
		return m_FileLastWriteTime;
	}
	void SetFileLastWriteTime(time_t Time)
	{
		m_FileLastWriteTime = Time;
	}
	UINT GetServiceID()
	{
		return m_ServiceID;
	}
	UINT GetClientID()
	{
		return m_ClientID;
	}
	const CEasyString& GetFilePath()
	{
		return m_FilePath;
	}
	const CEasyString& GetFileMD5()
	{
		return m_FileMD5;
	}
	UINT GetWorkingQueryCount()
	{
		return m_QueryCount;
	}

	UINT GetSerialOperationCount();
	virtual UINT GetParallelOperationCount();
	FILE_DATA_BLOCK* GetFinishOperation(bool IgnoreSequence = false);
	UINT GetFinishOperationCount();
	void DumpFinishQueue();
	void DumpParallelQueue();
protected:
	void AddSerialOperation(FILE_DATA_BLOCK * pData);
	FILE_DATA_BLOCK * GetSerialOperation(LPVOID& Pos);
	bool FinishSerialOperation(LPVOID Pos);

	void AddFinishOperation(FILE_DATA_BLOCK * pData);
	
};

