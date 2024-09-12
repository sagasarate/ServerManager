#pragma once

//<GenerateArea1Start>
//
//<GenerateArea1End>

class CServiceInfoList{
protected:
	UINT64	m_ModifyFlag;
	
//<GenerateArea2Start>	   
	CEasyArray<CServiceInfo>	m_List;
	
//<GenerateArea2End>
	
public:
//<GenerateArea3Start>
	
	enum SERVICE_INFO_LIST_MEMBER_IDS
	{
		SST_SRVIL_LIST = 401,
		
	};
	
	
	
	enum SERVICE_INFO_LIST_MODIFY_FLAGS:UINT64
	{
		MF_LIST = (((UINT64)1) << 0),
		
	};
	
	
//<GenerateArea3End>

public:
//<GenerateArea4Start>
	CServiceInfoList();
	CServiceInfoList(const CServiceInfoList& Object)
	{
		*this=Object;
	}
	virtual ~CServiceInfoList();
	virtual void Clear();
	
	void SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags);
	 
	void SetList(const CEasyArray<CServiceInfo>& Value);
	
		   
	 CEasyArray<CServiceInfo>& GetList() ;
	const CEasyArray<CServiceInfo>& GetList() const;
	

	
	virtual void GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual bool MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual void CloneFrom(const CServiceInfoList& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual UINT GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	CServiceInfoList& operator=(const CServiceInfoList& DataObject);
	
	
	
//<GenerateArea4End>

};


//<GenerateArea5Start>

inline void CServiceInfoList::SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags)
{
	m_ModifyFlag&=~RemoveFlags;
	m_ModifyFlag|=AddFlags;
}

inline void CServiceInfoList::SetList(const CEasyArray<CServiceInfo>& Value)
{
	m_List=Value;
	m_ModifyFlag|=MF_LIST;
}

		   
inline  CEasyArray<CServiceInfo>& CServiceInfoList::GetList() 
{
	return m_List;
}
inline const CEasyArray<CServiceInfo>& CServiceInfoList::GetList() const
{
	return m_List;
}


//<GenerateArea5End>