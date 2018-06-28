#pragma once

class CServerManagerClientView;
class CServerConnection;
class CTaskQueue :
	public CEasyThread
{
public:
	enum TASK_TYPE
	{
		TASK_TYPE_NONE,
		TASK_TYPE_DOWNLOAD,
		TASK_TYPE_UPLOAD,
		TASK_TYPE_DELETE_FILE,
		TASK_TYPE_CREATE_DIR,
		TASK_TYPE_CHANGE_FILE_MODE,
		TASK_TYPE_STARTUP_SERVICE,
		TASK_TYPE_SHUTDOWN_SERVICE,
		TASK_TYPE_RELOAD_CONFIG_DATA,
	};
	enum TASK_STATUS
	{
		TASK_STATUS_NONE,
		TASK_STATUS_STARTING,
		TASK_STATUS_TRANSFER_DATA,
		TASK_STATUS_WAIT_SERVICE_STATUS,
		TASK_STATUS_END,
	};
	enum COMPRESS_WORK_STATUS
	{
		COMPRESS_WORK_STATUS_NONE,
		COMPRESS_WORK_STATUS_SAVE,
		COMPRESS_WORK_STATUS_LOAD,
		COMPRESS_WORK_STATUS_FINISH,
		COMPRESS_WORK_STATUS_ERROR,
	};
	struct TASK_INFO
	{
		UINT					ID;
		TASK_TYPE				Type;
		TASK_STATUS				Status;
		UINT					ServiceID;
		CEasyString				SourceFilePath;
		CEasyString				TargetFilePath;
		UINT					FileMode;
		UINT64					FileSize;
		UINT64					TransferOffset;
		UINT64					TransferSize;
		bool					ContinueTransfer;
		UINT					TransferStartTime;
		void Clear()
		{
			ID = 0;
			Type = TASK_TYPE_NONE;
			Status = TASK_STATUS_NONE;
			ServiceID = 0;
			SourceFilePath.Clear();
			TargetFilePath.Clear();
			FileMode = 0;
			FileSize = 0;
			TransferOffset = 0;
			TransferSize = 0;
			ContinueTransfer = false;
			TransferStartTime = 0;
		}
	};
protected:
	
	
	

	CServerManagerClientView *		m_pView;
	CServerConnection *				m_pConnection;
	CIDStorage<TASK_INFO>			m_TaskQueue;
	volatile COMPRESS_WORK_STATUS	m_CompressWorkStatus;
	CEasyBuffer						m_InputBuffer;
	CEasyBuffer						m_OutputBuffer;
	CWinFileAccessor				m_CurTransferFile;
	bool							m_IsInTransfer;
public:
	CTaskQueue();
	~CTaskQueue();

	bool Init(CServerManagerClientView * pView, CServerConnection * pConnection);
	UINT AddDownloadTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer);
	UINT AddUploadTask(UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer);
	UINT AddDeleteFileTask(UINT ServiceID, LPCTSTR TargetFilePath);
	UINT AddCreateDirTask(UINT ServiceID, LPCTSTR TargetFilePath);
	UINT AddChangeFileTask(UINT ServiceID, LPCTSTR TargetFilePath, UINT FileMode);
	UINT AddStartupServiceTask(UINT ServiceID, bool ClearQueueOnFailed);
	UINT AddShutdownServiceTask(UINT ServiceID, bool ClearQueueOnFailed);
	UINT AddReloadConfigDataTask(UINT ServiceID);

	bool CancelFileTransfer(UINT ID);
	void DeleteAllTask();
	void Update();
	virtual BOOL OnRun();

	

	void OnStartDownloadResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize);
	void OnDownloadData(short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData);
	void OnEndDownloadResult(short Result, UINT FileLastWriteTime);
	void OnUploadStartResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize);
	void OnUploadData(short Result, UINT64 Offset, UINT Length);
	void OnEndUploadResult(short Result);
	void OnDeleteFileResult(short Result, UINT ServiceID, LPCTSTR FilePath);
	void OnCreateDirResult(short Result, UINT ServiceID, LPCTSTR Dir);
	void OnChangeFileModeResult(short Result, UINT ServiceID, LPCTSTR FilePath, UINT Mode);
	void OnServiceStartupResult(short Result, UINT ServiceID);
	void OnServiceShutdownResult(short Result, UINT ServiceID);
	void OnSendCommandResult(short Result, UINT ServiceID);
	void OnServiceInfo(CServiceInfo& ServiceInfo);
protected:
	void DeleteTask(UINT ID);
	void PrintLog(LPCTSTR szFormat, ...);
	void DoDownload(TASK_INFO * pFileInfo);
	void DoUpload(TASK_INFO * pFileInfo);
};

