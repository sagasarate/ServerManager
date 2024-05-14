#pragma once

//<GenerateArea1Start>
//
//<GenerateArea1End>

class CProcessInfoList{
protected:
	UINT64	m_ModifyFlag;
	
//<GenerateArea2Start>	   
	CEasyArray<CProcessInfo>	m_List;
	
//<GenerateArea2End>
	
public:
//<GenerateArea3Start>
	
	enum PROCESS_INFO_LIST_MEMBER_IDS
	{
		SST_PROCIL_LIST = 201,
		
	};
	
	
	
	enum PROCESS_INFO_LIST_MODIFY_FLAGS:UINT64
	{
		MF_LIST = (((UINT64)1) << 0),
		
	};
	
	
//<GenerateArea3End>

public:
//<GenerateArea4Start>
	CProcessInfoList();
	CProcessInfoList(const CProcessInfoList& Object)
	{
		*this=Object;
	}
	virtual ~CProcessInfoList();
	virtual void Clear();
	
	void SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags);
	 
	void SetList(const CEasyArray<CProcessInfo>& Value);
	
		   
	 CEasyArray<CProcessInfo>& GetList() ;
	const CEasyArray<CProcessInfo>& GetList() const;
	

	
	virtual void GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual bool MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual void CloneFrom(const CProcessInfoList& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual UINT GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	CProcessInfoList& operator=(const CProcessInfoList& DataObject);
	
	
	
//<GenerateArea4End>

};


//<GenerateArea5Start>

inline void CProcessInfoList::SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags)
{
	m_ModifyFlag&=~RemoveFlags;
	m_ModifyFlag|=AddFlags;
}

inline void CProcessInfoList::SetList(const CEasyArray<CProcessInfo>& Value)
{
	m_List=Value;
	m_ModifyFlag|=MF_LIST;
}

		   
inline  CEasyArray<CProcessInfo>& CProcessInfoList::GetList() 
{
	return m_List;
}
inline const CEasyArray<CProcessInfo>& CProcessInfoList::GetList() const
{
	return m_List;
}


//<GenerateArea5End>