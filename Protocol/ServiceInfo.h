#pragma once

//<GenerateArea1Start>
//
//<GenerateArea1End>

class CServiceInfo
{
protected:
	UINT64	m_ModifyFlag;
	
//<GenerateArea2Start>	   
	UINT			m_ServiceID;
	CEasyString		m_Name;
	BYTE			m_Type;
	CEasyString		m_WorkDir;
	CEasyString		m_StartupParam;
	BYTE			m_Status;
	BYTE			m_WorkStatus;
	BYTE			m_LastOperation;
	UINT			m_LastStatusChangeTime;
	UINT			m_RestartupTime;
	CEasyString		m_ControlPipeName;
	CEasyString		m_ShutdownCmd;
	int				m_CharSet;
	bool			m_KeepRunning;
	bool			m_LogStatusToFile;
	CEasyArray<CEasyString>		m_OtherExecFileList;
	CEasyString		m_ImageFilePath;
	UINT64			m_ImageFileTime;
	float			m_CPUUsed;
	UINT64			m_MemoryUsed;
	UINT64			m_VirtualMemoryUsed;
	UINT64			m_DiskFree;
	CEasyArray<CProcessInfo>	m_ProcessList;
	
//<GenerateArea2End>
	
public:
//<GenerateArea3Start>
	
	enum SERVICE_INFO_MEMBER_IDS
	{
		SST_SRVI_SERVICE_ID = 301,
		SST_SRVI_NAME = 313,
		SST_SRVI_TYPE = 302,
		SST_SRVI_WORK_DIR = 304,
		SST_SRVI_STARTUP_PARAM = 305,
		SST_SRVI_STATUS = 308,
		SST_SRVI_WORK_STATUS = 319,
		SST_SRVI_LAST_OPERATION = 314,
		SST_SRVI_LAST_STATUS_CHANGE_TIME = 315,
		SST_SRVI_RESTARTUP_TIME = 316,
		SST_SRVI_CONTROL_PIPE_NAME = 317,
		SST_SRVI_SHUTDOWN_CMD = 318,
		SST_SRVI_CHAR_SET = 320,
		SST_SRVI_KEEP_RUNNING = 323,
		SST_SRVI_LOG_STATUS_TO_FILE = 322,
		SST_SRVI_OTHER_EXEC_FILE_LIST = 321,
		SST_SRVI_IMAGE_FILE_PATH = 326,
		SST_SRVI_IMAGE_FILE_TIME = 327,
		SST_SRVI_PUUSED = 328,
		SST_SRVI_MEMORY_USED = 329,
		SST_SRVI_VIRTUAL_MEMORY_USED = 330,
		SST_SRVI_DISK_FREE = 324,
		SST_SRVI_PROCESS_LIST = 325,
		
	};
	
	
	
	enum SERVICE_INFO_MODIFY_FLAGS:UINT64
	{
		MF_SERVICE_ID = (((UINT64)1) << 0),
		MF_NAME = (((UINT64)1) << 1),
		MF_TYPE = (((UINT64)1) << 2),
		MF_WORK_DIR = (((UINT64)1) << 3),
		MF_STARTUP_PARAM = (((UINT64)1) << 4),
		MF_STATUS = (((UINT64)1) << 5),
		MF_WORK_STATUS = (((UINT64)1) << 6),
		MF_LAST_OPERATION = (((UINT64)1) << 7),
		MF_LAST_STATUS_CHANGE_TIME = (((UINT64)1) << 8),
		MF_RESTARTUP_TIME = (((UINT64)1) << 9),
		MF_CONTROL_PIPE_NAME = (((UINT64)1) << 10),
		MF_SHUTDOWN_CMD = (((UINT64)1) << 11),
		MF_CHAR_SET = (((UINT64)1) << 12),
		MF_KEEP_RUNNING = (((UINT64)1) << 13),
		MF_LOG_STATUS_TO_FILE = (((UINT64)1) << 14),
		MF_OTHER_EXEC_FILE_LIST = (((UINT64)1) << 15),
		MF_IMAGE_FILE_PATH = (((UINT64)1) << 16),
		MF_IMAGE_FILE_TIME = (((UINT64)1) << 17),
		MF_PUUSED = (((UINT64)1) << 18),
		MF_MEMORY_USED = (((UINT64)1) << 19),
		MF_VIRTUAL_MEMORY_USED = (((UINT64)1) << 20),
		MF_DISK_FREE = (((UINT64)1) << 21),
		MF_PROCESS_LIST = (((UINT64)1) << 22),
		
	};
	
	
//<GenerateArea3End>

public:
//<GenerateArea4Start>
	CServiceInfo();
	CServiceInfo(const CServiceInfo& Object)
	{
		*this=Object;
	}
	virtual ~CServiceInfo();
	virtual void Clear();
	
	void SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags);
	 
	void SetServiceID(UINT Value);
	void SetName(const CEasyString& Value);
	void SetType(BYTE Value);
	void SetWorkDir(const CEasyString& Value);
	void SetStartupParam(const CEasyString& Value);
	void SetStatus(BYTE Value);
	void SetWorkStatus(BYTE Value);
	void SetLastOperation(BYTE Value);
	void SetLastStatusChangeTime(UINT Value);
	void SetRestartupTime(UINT Value);
	void SetControlPipeName(const CEasyString& Value);
	void SetShutdownCmd(const CEasyString& Value);
	void SetCharSet(int Value);
	void SetKeepRunning(bool Value);
	void SetLogStatusToFile(bool Value);
	void SetOtherExecFileList(const CEasyArray<CEasyString>& Value);
	void SetImageFilePath(const CEasyString& Value);
	void SetImageFileTime(UINT64 Value);
	void SetCPUUsed(float Value);
	void SetMemoryUsed(UINT64 Value);
	void SetVirtualMemoryUsed(UINT64 Value);
	void SetDiskFree(UINT64 Value);
	void SetProcessList(const CEasyArray<CProcessInfo>& Value);
	
		   
	UINT GetServiceID() const;
	 CEasyString& GetName() ;
	const CEasyString& GetName() const;
	BYTE GetType() const;
	 CEasyString& GetWorkDir() ;
	const CEasyString& GetWorkDir() const;
	 CEasyString& GetStartupParam() ;
	const CEasyString& GetStartupParam() const;
	BYTE GetStatus() const;
	BYTE GetWorkStatus() const;
	BYTE GetLastOperation() const;
	UINT GetLastStatusChangeTime() const;
	UINT GetRestartupTime() const;
	 CEasyString& GetControlPipeName() ;
	const CEasyString& GetControlPipeName() const;
	 CEasyString& GetShutdownCmd() ;
	const CEasyString& GetShutdownCmd() const;
	int GetCharSet() const;
	bool GetKeepRunning() const;
	bool GetLogStatusToFile() const;
	 CEasyArray<CEasyString>& GetOtherExecFileList() ;
	const CEasyArray<CEasyString>& GetOtherExecFileList() const;
	 CEasyString& GetImageFilePath() ;
	const CEasyString& GetImageFilePath() const;
	UINT64 GetImageFileTime() const;
	float GetCPUUsed() const;
	UINT64 GetMemoryUsed() const;
	UINT64 GetVirtualMemoryUsed() const;
	UINT64 GetDiskFree() const;
	 CEasyArray<CProcessInfo>& GetProcessList() ;
	const CEasyArray<CProcessInfo>& GetProcessList() const;
	

	
	virtual void GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual bool MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual void CloneFrom(const CServiceInfo& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual UINT GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	CServiceInfo& operator=(const CServiceInfo& DataObject);
	
	
	
//<GenerateArea4End>

};


//<GenerateArea5Start>

inline void CServiceInfo::SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags)
{
	m_ModifyFlag&=~RemoveFlags;
	m_ModifyFlag|=AddFlags;
}

inline void CServiceInfo::SetServiceID(UINT Value)
{
	if(m_ServiceID!=Value)
	{
		m_ServiceID=Value;
		m_ModifyFlag|=MF_SERVICE_ID;
	}
}
inline void CServiceInfo::SetName(const CEasyString& Value)
{
	m_Name=Value;
	m_ModifyFlag|=MF_NAME;
}
inline void CServiceInfo::SetType(BYTE Value)
{
	if(m_Type!=Value)
	{
		m_Type=Value;
		m_ModifyFlag|=MF_TYPE;
	}
}
inline void CServiceInfo::SetWorkDir(const CEasyString& Value)
{
	m_WorkDir=Value;
	m_ModifyFlag|=MF_WORK_DIR;
}
inline void CServiceInfo::SetStartupParam(const CEasyString& Value)
{
	m_StartupParam=Value;
	m_ModifyFlag|=MF_STARTUP_PARAM;
}
inline void CServiceInfo::SetStatus(BYTE Value)
{
	if(m_Status!=Value)
	{
		m_Status=Value;
		m_ModifyFlag|=MF_STATUS;
	}
}
inline void CServiceInfo::SetWorkStatus(BYTE Value)
{
	if(m_WorkStatus!=Value)
	{
		m_WorkStatus=Value;
		m_ModifyFlag|=MF_WORK_STATUS;
	}
}
inline void CServiceInfo::SetLastOperation(BYTE Value)
{
	if(m_LastOperation!=Value)
	{
		m_LastOperation=Value;
		m_ModifyFlag|=MF_LAST_OPERATION;
	}
}
inline void CServiceInfo::SetLastStatusChangeTime(UINT Value)
{
	if(m_LastStatusChangeTime!=Value)
	{
		m_LastStatusChangeTime=Value;
		m_ModifyFlag|=MF_LAST_STATUS_CHANGE_TIME;
	}
}
inline void CServiceInfo::SetRestartupTime(UINT Value)
{
	if(m_RestartupTime!=Value)
	{
		m_RestartupTime=Value;
		m_ModifyFlag|=MF_RESTARTUP_TIME;
	}
}
inline void CServiceInfo::SetControlPipeName(const CEasyString& Value)
{
	m_ControlPipeName=Value;
	m_ModifyFlag|=MF_CONTROL_PIPE_NAME;
}
inline void CServiceInfo::SetShutdownCmd(const CEasyString& Value)
{
	m_ShutdownCmd=Value;
	m_ModifyFlag|=MF_SHUTDOWN_CMD;
}
inline void CServiceInfo::SetCharSet(int Value)
{
	if(m_CharSet!=Value)
	{
		m_CharSet=Value;
		m_ModifyFlag|=MF_CHAR_SET;
	}
}
inline void CServiceInfo::SetKeepRunning(bool Value)
{
	if(m_KeepRunning!=Value)
	{
		m_KeepRunning=Value;
		m_ModifyFlag|=MF_KEEP_RUNNING;
	}
}
inline void CServiceInfo::SetLogStatusToFile(bool Value)
{
	if(m_LogStatusToFile!=Value)
	{
		m_LogStatusToFile=Value;
		m_ModifyFlag|=MF_LOG_STATUS_TO_FILE;
	}
}
inline void CServiceInfo::SetOtherExecFileList(const CEasyArray<CEasyString>& Value)
{
	m_OtherExecFileList=Value;
	m_ModifyFlag|=MF_OTHER_EXEC_FILE_LIST;
}
inline void CServiceInfo::SetImageFilePath(const CEasyString& Value)
{
	m_ImageFilePath=Value;
	m_ModifyFlag|=MF_IMAGE_FILE_PATH;
}
inline void CServiceInfo::SetImageFileTime(UINT64 Value)
{
	if(m_ImageFileTime!=Value)
	{
		m_ImageFileTime=Value;
		m_ModifyFlag|=MF_IMAGE_FILE_TIME;
	}
}
inline void CServiceInfo::SetCPUUsed(float Value)
{
	if(m_CPUUsed!=Value)
	{
		m_CPUUsed=Value;
		m_ModifyFlag|=MF_PUUSED;
	}
}
inline void CServiceInfo::SetMemoryUsed(UINT64 Value)
{
	if(m_MemoryUsed!=Value)
	{
		m_MemoryUsed=Value;
		m_ModifyFlag|=MF_MEMORY_USED;
	}
}
inline void CServiceInfo::SetVirtualMemoryUsed(UINT64 Value)
{
	if(m_VirtualMemoryUsed!=Value)
	{
		m_VirtualMemoryUsed=Value;
		m_ModifyFlag|=MF_VIRTUAL_MEMORY_USED;
	}
}
inline void CServiceInfo::SetDiskFree(UINT64 Value)
{
	if(m_DiskFree!=Value)
	{
		m_DiskFree=Value;
		m_ModifyFlag|=MF_DISK_FREE;
	}
}
inline void CServiceInfo::SetProcessList(const CEasyArray<CProcessInfo>& Value)
{
	m_ProcessList=Value;
	m_ModifyFlag|=MF_PROCESS_LIST;
}

		   
inline UINT CServiceInfo::GetServiceID() const
{
	return m_ServiceID;
}
inline  CEasyString& CServiceInfo::GetName() 
{
	return m_Name;
}
inline const CEasyString& CServiceInfo::GetName() const
{
	return m_Name;
}
inline BYTE CServiceInfo::GetType() const
{
	return m_Type;
}
inline  CEasyString& CServiceInfo::GetWorkDir() 
{
	return m_WorkDir;
}
inline const CEasyString& CServiceInfo::GetWorkDir() const
{
	return m_WorkDir;
}
inline  CEasyString& CServiceInfo::GetStartupParam() 
{
	return m_StartupParam;
}
inline const CEasyString& CServiceInfo::GetStartupParam() const
{
	return m_StartupParam;
}
inline BYTE CServiceInfo::GetStatus() const
{
	return m_Status;
}
inline BYTE CServiceInfo::GetWorkStatus() const
{
	return m_WorkStatus;
}
inline BYTE CServiceInfo::GetLastOperation() const
{
	return m_LastOperation;
}
inline UINT CServiceInfo::GetLastStatusChangeTime() const
{
	return m_LastStatusChangeTime;
}
inline UINT CServiceInfo::GetRestartupTime() const
{
	return m_RestartupTime;
}
inline  CEasyString& CServiceInfo::GetControlPipeName() 
{
	return m_ControlPipeName;
}
inline const CEasyString& CServiceInfo::GetControlPipeName() const
{
	return m_ControlPipeName;
}
inline  CEasyString& CServiceInfo::GetShutdownCmd() 
{
	return m_ShutdownCmd;
}
inline const CEasyString& CServiceInfo::GetShutdownCmd() const
{
	return m_ShutdownCmd;
}
inline int CServiceInfo::GetCharSet() const
{
	return m_CharSet;
}
inline bool CServiceInfo::GetKeepRunning() const
{
	return m_KeepRunning;
}
inline bool CServiceInfo::GetLogStatusToFile() const
{
	return m_LogStatusToFile;
}
inline  CEasyArray<CEasyString>& CServiceInfo::GetOtherExecFileList() 
{
	return m_OtherExecFileList;
}
inline const CEasyArray<CEasyString>& CServiceInfo::GetOtherExecFileList() const
{
	return m_OtherExecFileList;
}
inline  CEasyString& CServiceInfo::GetImageFilePath() 
{
	return m_ImageFilePath;
}
inline const CEasyString& CServiceInfo::GetImageFilePath() const
{
	return m_ImageFilePath;
}
inline UINT64 CServiceInfo::GetImageFileTime() const
{
	return m_ImageFileTime;
}
inline float CServiceInfo::GetCPUUsed() const
{
	return m_CPUUsed;
}
inline UINT64 CServiceInfo::GetMemoryUsed() const
{
	return m_MemoryUsed;
}
inline UINT64 CServiceInfo::GetVirtualMemoryUsed() const
{
	return m_VirtualMemoryUsed;
}
inline UINT64 CServiceInfo::GetDiskFree() const
{
	return m_DiskFree;
}
inline  CEasyArray<CProcessInfo>& CServiceInfo::GetProcessList() 
{
	return m_ProcessList;
}
inline const CEasyArray<CProcessInfo>& CServiceInfo::GetProcessList() const
{
	return m_ProcessList;
}


//<GenerateArea5End>