#pragma once

//<GenerateArea1Start>
//
//<GenerateArea1End>

class CDataObjectEmpty{
protected:
	UINT64	m_ModifyFlag;
	
//<GenerateArea2Start>	   
	
//<GenerateArea2End>
	
public:
//<GenerateArea3Start>
	
	enum DATA_OBJECT_EMPTY_MEMBER_IDS
	{
		
	};
	
	enum DATA_OBJECT_EMPTY_MODIFY_FLAGS:UINT64
	{
		MF_ALL=0x0,
	
	};
//<GenerateArea3End>

public:
//<GenerateArea4Start>
	CDataObjectEmpty();
	CDataObjectEmpty(const CDataObjectEmpty& Object)
	{
		*this=Object;
	}
	virtual ~CDataObjectEmpty();
	virtual void Clear();
	
	void SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags);
	 
	
		   
	

	
	virtual void GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual bool MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual bool MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	virtual void ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual void CloneFrom(const CDataObjectEmpty& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags);
	virtual UINT GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const;
	CDataObjectEmpty& operator=(const CDataObjectEmpty& DataObject);
	
//<GenerateArea4End>

};


//<GenerateArea5Start>

inline void CDataObjectEmpty::SetModifyFlag(UINT64 RemoveFlags,UINT64 AddFlags)
{
	m_ModifyFlag&=~RemoveFlags;
	m_ModifyFlag|=AddFlags;
}


		   


//<GenerateArea5End>