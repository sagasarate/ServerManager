#pragma once

//<GenerateArea1Start>
//
//<GenerateArea1End>

class CProcessInfo{
protected:
	UINT64	m_ModifyFlag;
	
//<GenerateArea2Start>	   
	UINT			m_ProcessID;
	CEasyString		m_ImageFilePath;
	UINT64			m_ImageFileTime;
	UINT64			m_CPUUsedTime;
	float			m_CPUUsed;
	UINT64			m_MemoryUsed;
	UINT64			m_VirtualMemoryUsed;
	UINT64			m_DiskFree;
	
//<GenerateArea2End>
	
public:
//<GenerateArea3Start>
	
	enum PROCESS_INFO_MEMBER_IDS
	{
		SST_PROCI_PROCESS_ID=101,
		SST_PROCI_IMAGE_FILE_PATH=103,
		SST_PROCI_IMAGE_FILE_TIME=104,
		SST_PROCI_PUUSED_TIME=109,
		SST_PROCI_PUUSED=106,
		SST_PROCI_MEMORY_USED=107,
		SST_PROCI_VIRTUAL_MEMORY_USED=108,
		SST_PROCI_DISK_FREE=110,
	
	};
	
	enum PROCESS_INFO_MODIFY_FLAGS:UINT64
	{
		MF_PROCESS_ID=(((UINT64)1)<<0),
		MF_IMAGE_FILE_PATH=(((UINT64)1)<<1),
		MF_IMAGE_FILE_TIME=(((UINT64)1)<<2),
		MF_PUUSED_TIME=(((UINT64)1)<<3),
		MF_PUUSED=(((UINT64)1)<<4),
		MF_MEMORY_USED=(((UINT64)1)<<5),
		MF_VIRTUAL_MEMORY_USED=(((UINT64)1)<<6),
		MF_DISK_FREE=(((UINT64)1)<<7),
		MF_ALL=0xFF,
	
	};
//<GenerateArea3End>

public:
//<GenerateArea4Start>
	CProcessInfo();
	CProcessInfo(const CProcessInfo& Object)
	{
		*this=Object;
	}
	virtual ~CProcessInfo();
	virtual void Clear();
	
	void SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags);
	 
	void SetProcessID(UINT Value);
	void SetImageFilePath(const CEasyString& Value);
	void SetImageFileTime(UINT64 Value);
	void SetCPUUsedTime(UINT64 Value);
	void SetCPUUsed(float Value);
	void SetMemoryUsed(UINT64 Value);
	void SetVirtualMemoryUsed(UINT64 Value);
	void SetDiskFree(UINT64 Value);
	
		   
	UINT GetProcessID() const;
	 CEasyString& GetImageFilePath() ;
	const CEasyString& GetImageFilePath() const;
	UINT64 GetImageFileTime() const;
	UINT64 GetCPUUsedTime() const;
	float GetCPUUsed() const;
	UINT64 GetMemoryUsed() const;
	UINT64 GetVirtualMemoryUsed() const;
	UINT64 GetDiskFree() const;
	

	
	virtual void GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual bool MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual void CloneFrom(const CProcessInfo& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual UINT GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	CProcessInfo& operator=(const CProcessInfo& DataObject);
	
//<GenerateArea4End>

};


//<GenerateArea5Start>

inline void CProcessInfo::SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags)
{
	m_ModifyFlag&=~RemoveFlags;
	m_ModifyFlag|=AddFlags;
}

inline void CProcessInfo::SetProcessID(UINT Value)
{
	if(m_ProcessID!=Value)
	{
		m_ProcessID=Value;
		m_ModifyFlag|=MF_PROCESS_ID;
	}
}
inline void CProcessInfo::SetImageFilePath(const CEasyString& Value)
{
	m_ImageFilePath=Value;
	m_ModifyFlag|=MF_IMAGE_FILE_PATH;
}
inline void CProcessInfo::SetImageFileTime(UINT64 Value)
{
	if(m_ImageFileTime!=Value)
	{
		m_ImageFileTime=Value;
		m_ModifyFlag|=MF_IMAGE_FILE_TIME;
	}
}
inline void CProcessInfo::SetCPUUsedTime(UINT64 Value)
{
	if(m_CPUUsedTime!=Value)
	{
		m_CPUUsedTime=Value;
		m_ModifyFlag|=MF_PUUSED_TIME;
	}
}
inline void CProcessInfo::SetCPUUsed(float Value)
{
	if(m_CPUUsed!=Value)
	{
		m_CPUUsed=Value;
		m_ModifyFlag|=MF_PUUSED;
	}
}
inline void CProcessInfo::SetMemoryUsed(UINT64 Value)
{
	if(m_MemoryUsed!=Value)
	{
		m_MemoryUsed=Value;
		m_ModifyFlag|=MF_MEMORY_USED;
	}
}
inline void CProcessInfo::SetVirtualMemoryUsed(UINT64 Value)
{
	if(m_VirtualMemoryUsed!=Value)
	{
		m_VirtualMemoryUsed=Value;
		m_ModifyFlag|=MF_VIRTUAL_MEMORY_USED;
	}
}
inline void CProcessInfo::SetDiskFree(UINT64 Value)
{
	if(m_DiskFree!=Value)
	{
		m_DiskFree=Value;
		m_ModifyFlag|=MF_DISK_FREE;
	}
}

		   
inline UINT CProcessInfo::GetProcessID() const
{
	return m_ProcessID;
}
inline  CEasyString& CProcessInfo::GetImageFilePath() 
{
	return m_ImageFilePath;
}
inline const CEasyString& CProcessInfo::GetImageFilePath() const
{
	return m_ImageFilePath;
}
inline UINT64 CProcessInfo::GetImageFileTime() const
{
	return m_ImageFileTime;
}
inline UINT64 CProcessInfo::GetCPUUsedTime() const
{
	return m_CPUUsedTime;
}
inline float CProcessInfo::GetCPUUsed() const
{
	return m_CPUUsed;
}
inline UINT64 CProcessInfo::GetMemoryUsed() const
{
	return m_MemoryUsed;
}
inline UINT64 CProcessInfo::GetVirtualMemoryUsed() const
{
	return m_VirtualMemoryUsed;
}
inline UINT64 CProcessInfo::GetDiskFree() const
{
	return m_DiskFree;
}


//<GenerateArea5End>