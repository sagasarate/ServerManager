#include "stdafx.h"



CDataObjectEmpty::CDataObjectEmpty()
{
	Clear();
}
CDataObjectEmpty::~CDataObjectEmpty()
{
}

void CDataObjectEmpty::Clear()
{
//<GenerateArea1Start>

	m_ModifyFlag=0;
	   
	
//<GenerateArea1End>

}

void CDataObjectEmpty::GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea2Start>
	switch(GetType)
	{
	case GMFT_COPY:
		ModifyFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY]=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY];
		break;
	case GMFT_AND:
		ModifyFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY]&=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY];
		break;
	case GMFT_OR:
		ModifyFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY]|=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY];
		break;
	}
	
//<GenerateArea2End>
}

bool CDataObjectEmpty::IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{

	bool IsModified=false;
//<GenerateArea3Start>	
	
	IsModified=IsModified||(m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY])!=0;
	
	IsModified=IsModified
		||false;
	
//<GenerateArea3End>
	return IsModified;

}

void CDataObjectEmpty::ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea4Start>
	m_ModifyFlag&=~MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY];
	
//<GenerateArea4End>
}

bool CDataObjectEmpty::MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea5Start>
	DATA_OBJECT_MODIFY_FLAGS ModifyFlags;
	ZeroMemory(&ModifyFlags,sizeof(ModifyFlags));
	GetModifyFlag(ModifyFlags,GMFT_OR,MemberFlags);	
//<GenerateArea5End>
	return MakePacket(Packet,ModifyFlags);
}

bool CDataObjectEmpty::MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea6Start>
	UINT FailCount=0;
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY];

	
//<GenerateArea6End>
	return FailCount==0;

}

void CDataObjectEmpty::ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea7Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY];
	UINT64 UpdateFlag=0;
	
//<GenerateArea7End>
	
	m_ModifyFlag|=UpdateFlag;	

}

void CDataObjectEmpty::CloneFrom(const CDataObjectEmpty& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea8Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_DATA_OBJECT_EMPTY];
	UINT64 UpdateFlag=0;
	
	
	
//<GenerateArea8End>

	m_ModifyFlag|=UpdateFlag;
}

UINT CDataObjectEmpty::GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea9Start>
	UINT Size=0;
	
		
//<GenerateArea9End>	
	return Size;

}

CDataObjectEmpty& CDataObjectEmpty::operator=(const CDataObjectEmpty& DataObject)
{
//<GenerateArea10Start>
	CloneFrom(DataObject,DOMF_DATA_OBJECT_EMPTY_FULL);
//<GenerateArea10End>	
	return *this;
}