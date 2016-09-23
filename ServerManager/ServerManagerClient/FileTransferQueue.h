#pragma once
class CFileTransferQueue :
	public CEasyThread
{
public:
	enum FILE_TRANSFER_TYPE
	{
		FILE_TRANSFER_TYPE_DOWNLOAD,
		FILE_TRANSFER_TYPE_UPLOAD,
		FILE_TRANSFER_TYPE_DELETE,
		FILE_TRANSFER_TYPE_CREATE_DIR,
		FILE_TRANSFER_TYPE_CHANGE_FILE_MODE,
	};
protected:
	
	enum FILE_TRANSFER_STATUS
	{
		FILE_TRANSFER_STATUS_NONE,
		FILE_TRANSFER_STATUS_STARTING,
		FILE_TRANSFER_STATUS_TRANSFER_DATA,
		FILE_TRANSFER_STATUS_END,
	};
	enum COMPRESS_WORK_STATUS
	{
		COMPRESS_WORK_STATUS_NONE,
		COMPRESS_WORK_STATUS_SAVE,
		COMPRESS_WORK_STATUS_LOAD,
		COMPRESS_WORK_STATUS_FINISH,
		COMPRESS_WORK_STATUS_ERROR,
	};
	struct FILE_INFO
	{
		UINT					ID;
		FILE_TRANSFER_TYPE		Type;
		FILE_TRANSFER_STATUS	Status;
		UINT					ConnectionID;
		UINT					ServiceID;
		CEasyString				SourceFilePath;
		CEasyString				TargetFilePath;
		UINT					FileMode;
		UINT64					FileSize;
		UINT64					TransferOffset;
		UINT64					TransferSize;
		bool					ContinueTransfer;
		UINT					TransferStartTime;
	};

	CServerManagerClientView *		m_pView;
	CIDStorage<FILE_INFO>			m_FileTransferQueue;
	volatile COMPRESS_WORK_STATUS	m_CompressWorkStatus;
	CEasyBuffer						m_InputBuffer;
	CEasyBuffer						m_OutputBuffer;
	CWinFileAccessor				m_CurTransferFile;
	bool							m_IsInTransfer;
public:
	CFileTransferQueue();
	~CFileTransferQueue();

	bool Init(CServerManagerClientView * pView);
	UINT AddTask(FILE_TRANSFER_TYPE	Type, UINT ConnectionID, UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, UINT FileMode, bool ContinueTransfer);
	bool CancelFileTransfer(UINT ID);
	void DeleteAllTask();
	void Update();
	virtual BOOL OnRun();

	

	void OnStartDownloadResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize);
	void OnDownloadData(CServerConnection * pConnection, short Result, UINT64 Offset, UINT Length, const CEasyBuffer& FileData);
	void OnEndDownloadResult(CServerConnection * pConnection, short Result, UINT FileLastWriteTime);
	void OnUploadStartResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath, UINT64 FileSize);
	void OnUploadData(CServerConnection * pConnection, short Result, UINT64 Offset, UINT Length);
	void OnEndUploadResult(CServerConnection * pConnection, short Result);
	void OnDeleteFileResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath);
	void OnCreateDirResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR Dir);
	void OnChangeFileModeResult(CServerConnection * pConnection, short Result, UINT ServiceID, LPCTSTR FilePath, UINT Mode);
protected:
	void DeleteTask(UINT ID);
	void PrintLog(LPCTSTR szFormat, ...);
	void DoDownload(CServerConnection * pConnection, FILE_INFO * pFileInfo);
	void DoUpload(CServerConnection * pConnection, FILE_INFO * pFileInfo);
};

