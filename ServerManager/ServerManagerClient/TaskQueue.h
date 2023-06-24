#pragma once

class CServerManagerClientView;
class CServerConnection;
class CTaskQueue 
{
public:
	
protected:
	
	
	

	CServerManagerClientView *					m_pView;
	CServerConnection *							m_pConnection;
	CIDStorage<TASK_INFO>						m_TaskQueue;
	CThreadSafeIDStorage<FILE_DATA_INFO>		m_FileDataPool;
	CEasyArray<FILE_DATA_INFO *>				m_FinishedWorkList;
	CEasyCriticalSection						m_FinishedWorkListLock;
	CEasyArray<CWorkThread>						m_WorkThreadList;
	
	//CWinFileAccessor							m_CurTransferFile;
	bool										m_IsInTransfer;
public:
	CTaskQueue();
	~CTaskQueue();

	bool Init(CServerManagerClientView * pView, CServerConnection * pConnection);

	CWorkThread * AllocWorkThread();
	UINT GetFreeThreadCount();
	bool IsAllWorkThreadIdle();
	UINT AddDownloadTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer);
	UINT AddUploadTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer);
	UINT AddDeleteFileTask(UINT ServiceID, LPCTSTR TargetFilePath);
	UINT AddCreateDirTask(UINT ServiceID, LPCTSTR TargetFilePath);
	UINT AddChangeFileTask(UINT ServiceID, LPCTSTR TargetFilePath, UINT FileMode);
	UINT AddStartupServiceTask(UINT ServiceID, bool ClearQueueOnFailed);
	UINT AddShutdownServiceTask(UINT ServiceID, bool ClearQueueOnFailed);
	UINT AddReloadConfigDataTask(UINT ServiceID);
	UINT AddFileCompareTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, UINT Usage);

	bool CancelFileTransfer(UINT ID);
	void DeleteAllTask();
	void Update();
	//virtual BOOL OnRun();

	FILE_DATA_INFO * AllocFileDataInfo(UINT BufferSize);
	void DeleteFileDataInfo(FILE_DATA_INFO * pInfo);
	bool OnWorkFinish(FILE_DATA_INFO * pData);
	FILE_DATA_INFO * GetFinishedFileData(UINT Index);
	void OnTaskFinish(TASK_INFO * pTaskInfo);
	void OnTaskError(TASK_INFO * pTaskInfo);

	void OnStartDownloadResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize, UINT FileLastWriteTime);
	void OnDownloadData(short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData, bool IsLast);	
	void OnDownloadFinish(short Result, const CEasyString& MD5);
	void OnUploadStartResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize);
	void OnUploadData(short Result, UINT Length, bool IsLast);
	void OnUploadFinish(short Result, const CEasyString& MD5);
	void OnDeleteFileResult(short Result, UINT ServiceID, LPCTSTR FilePath);
	void OnCreateDirResult(short Result, UINT ServiceID, LPCTSTR Dir);
	void OnChangeFileModeResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT Mode);
	void OnServiceStartupResult(short Result, UINT ServiceID);
	void OnServiceShutdownResult(short Result, UINT ServiceID);
	void OnSendCommandResult(short Result, UINT ServiceID);
	void OnServiceInfo(CServiceInfo& ServiceInfo);
	void OnFileCompareResult(short Result, UINT ServiceID, LPCTSTR FilePath);
protected:
	void DeleteTask(TASK_INFO * pTaskInfo);
	void PrintLog(short Result, LPCTSTR szFormat, ...);
	void SendDownloadQuery(TASK_INFO * pTaskInfo, UINT Count);
	void StopDownload(TASK_INFO* pTaskInfo);
	void EndDownload(TASK_INFO* pTaskInfo, int Result, const CEasyString& MD5);
	void StopUpload(TASK_INFO * pTaskInfo);
	void EndUpload(TASK_INFO* pTaskInfo, int Result, const CEasyString& MD5);
	//void DoDownload(TASK_INFO * pFileInfo);
	//void DoUpload(TASK_INFO * pFileInfo);
	//void DoCompare(TASK_INFO * pTaskInfo);
};

