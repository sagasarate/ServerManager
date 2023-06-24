#include "stdafx.h"



CProcessInfoList::CProcessInfoList()
{
	Clear();
}
CProcessInfoList::~CProcessInfoList()
{
}

void CProcessInfoList::Clear()
{
//<GenerateArea1Start>

	m_ModifyFlag=0;
	   
	m_List.SetTag(_T("StructData"));
	m_List.Clear();
	m_List.Create(16,8);
	
//<GenerateArea1End>

}

void CProcessInfoList::GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea2Start>
	switch(GetType)
	{
	case GMFT_COPY:
		ModifyFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST]=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];
		break;
	case GMFT_AND:
		ModifyFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST]&=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];
		break;
	case GMFT_OR:
		ModifyFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST]|=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];
		break;
	}
	
//<GenerateArea2End>
}

bool CProcessInfoList::IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{

	bool IsModified=false;
//<GenerateArea3Start>	
	
	IsModified=IsModified||(m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST])!=0;
	
	IsModified=IsModified
		;
	
//<GenerateArea3End>
	return IsModified;

}

void CProcessInfoList::ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea4Start>
	m_ModifyFlag&=~MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];
	
//<GenerateArea4End>
}

bool CProcessInfoList::MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea5Start>
	DATA_OBJECT_MODIFY_FLAGS ModifyFlags;
	ZeroMemory(&ModifyFlags,sizeof(ModifyFlags));
	GetModifyFlag(ModifyFlags,GMFT_OR,MemberFlags);	
//<GenerateArea5End>
	return MakePacket(Packet,ModifyFlags);
}

bool CProcessInfoList::MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea6Start>
	UINT FailCount=0;
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];

	if(Flag&MF_LIST)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		{
			CSmartStruct Packet(pBuffer,BufferSize,true);
			for(size_t i=0;i<m_List.GetCount();i++)
			{
				UINT BufferSize;
				void * pBuffer=Packet.PrepareMember(BufferSize);
				CSmartStruct SubPacket(pBuffer,BufferSize,true);
				if(!m_List[i].MakePacket(SubPacket,MemberFlags)) FailCount++;
				Packet.FinishMember(SST_PROCIL_LIST,SubPacket.GetDataLen());
			}
			BufferSize=Packet.GetDataLen();
		}
		Packet.FinishMember(SST_PROCIL_LIST,BufferSize);
	}
	
//<GenerateArea6End>
	return FailCount==0;

}

void CProcessInfoList::ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea7Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];
	UINT64 UpdateFlag=0;
	
	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_PROCIL_LIST:
			if(Flag&MF_LIST)
			{
				m_List.Clear();
				CSmartStruct Packet=Value;
				void * Pos=Packet.GetFirstMemberPosition();
				while(Pos)
				{
					WORD MemberID;
					CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
					switch(MemberID)
					{
					case SST_PROCIL_LIST:
						{	
							CProcessInfo	ArrayElement;
							ArrayElement.ParsePacket(Value,MemberFlags);
							UpdateFlag|=MF_LIST;
							m_List.Add(ArrayElement);
						}
						break;
					}
				}
			}
			break;
		
		
		}
	}
	
//<GenerateArea7End>
	
	m_ModifyFlag|=UpdateFlag;	

}

void CProcessInfoList::CloneFrom(const CProcessInfoList& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea8Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];
	UINT64 UpdateFlag=0;
	
	if(Flag&MF_LIST)
	{
		m_List=DataObject.m_List;
		UpdateFlag|=MF_LIST;
	}
	
	
//<GenerateArea8End>

	m_ModifyFlag|=UpdateFlag;
}

UINT CProcessInfoList::GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea9Start>
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO_LIST];
	UINT Size=0;
	if(Flag&MF_LIST)
	{
		for(size_t i=0;i<m_List.GetCount();i++)
		{
			Size+=CSmartStruct::GetStructMemberSize(m_List[i].GetSmartStructSize(MemberFlags));
		}
		Size+=CSmartStruct::GetStructMemberSize(0);
	}
	
		
//<GenerateArea9End>	
	return Size;

}

CProcessInfoList& CProcessInfoList::operator=(const CProcessInfoList& DataObject)
{
//<GenerateArea10Start>
	CloneFrom(DataObject,DOMF_PROCESS_INFO_LIST_FULL);
//<GenerateArea10End>	
	return *this;
}