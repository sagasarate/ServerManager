#include "stdafx.h"



CProcessInfo::CProcessInfo()
{
	Clear();
}
CProcessInfo::~CProcessInfo()
{
}

void CProcessInfo::Clear()
{
//<GenerateArea1Start>
	m_ModifyFlag=0;
	   
	m_ProcessID=0;
	m_ImageFilePath.Clear();
	m_ImageFileTime=0;
	m_CPUUsedTime=0;
	m_CPUUsed=0;
	m_MemoryUsed=0;
	m_VirtualMemoryUsed=0;
	m_DiskFree=0;
	
//<GenerateArea1End>

}

void CProcessInfo::GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea2Start>
	switch(GetType)
	{
	case GMFT_COPY:
		ModifyFlags[DATA_OBJECT_FLAG_PROCESS_INFO]=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];
		break;
	case GMFT_AND:
		ModifyFlags[DATA_OBJECT_FLAG_PROCESS_INFO]&=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];
		break;
	case GMFT_OR:
		ModifyFlags[DATA_OBJECT_FLAG_PROCESS_INFO]|=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];
		break;
	}
	
//<GenerateArea2End>
}

bool CProcessInfo::IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{

	bool IsModified=false;
//<GenerateArea3Start>	
	
	IsModified=IsModified||(m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO])!=0;
	
	IsModified=IsModified
		;
	
//<GenerateArea3End>
	return IsModified;

}

void CProcessInfo::ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea4Start>
	m_ModifyFlag&=~MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];
	
//<GenerateArea4End>
}

bool CProcessInfo::MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea5Start>
	DATA_OBJECT_MODIFY_FLAGS ModifyFlags;
	ZeroMemory(&ModifyFlags,sizeof(ModifyFlags));
	GetModifyFlag(ModifyFlags,GMFT_OR,MemberFlags);	
//<GenerateArea5End>
	return MakePacket(Packet,ModifyFlags);
}

bool CProcessInfo::MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea6Start>
	UINT FailCount=0;
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];

	if(Flag&MF_PROCESS_ID)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_PROCESS_ID,m_ProcessID),FailCount);
	}
	
	if(Flag&MF_IMAGE_FILE_PATH)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_IMAGE_FILE_PATH,m_ImageFilePath),FailCount);
	}
	
	if(Flag&MF_IMAGE_FILE_TIME)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_IMAGE_FILE_TIME,m_ImageFileTime),FailCount);
	}
	
	if(Flag&MF_PUUSED_TIME)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_PUUSED_TIME,m_CPUUsedTime),FailCount);
	}
	
	if(Flag&MF_PUUSED)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_PUUSED,m_CPUUsed),FailCount);
	}
	
	if(Flag&MF_MEMORY_USED)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_MEMORY_USED,m_MemoryUsed),FailCount);
	}
	
	if(Flag&MF_VIRTUAL_MEMORY_USED)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_VIRTUAL_MEMORY_USED,m_VirtualMemoryUsed),FailCount);
	}
	
	if(Flag&MF_DISK_FREE)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_PROCI_DISK_FREE,m_DiskFree),FailCount);
	}
	
	
//<GenerateArea6End>
	return FailCount==0;

}

void CProcessInfo::ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea7Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];
	UINT64 UpdateFlag=0;
	
	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_PROCI_PROCESS_ID:
			if(Flag&MF_PROCESS_ID)
			{
				m_ProcessID=Value;
				UpdateFlag |= MF_PROCESS_ID;
			}
			break;
		case SST_PROCI_IMAGE_FILE_PATH:
			if(Flag&MF_IMAGE_FILE_PATH)
			{
				Value.GetString(m_ImageFilePath);
				UpdateFlag |= MF_IMAGE_FILE_PATH;
			}
			break;
		case SST_PROCI_IMAGE_FILE_TIME:
			if(Flag&MF_IMAGE_FILE_TIME)
			{
				m_ImageFileTime=Value;
				UpdateFlag |= MF_IMAGE_FILE_TIME;
			}
			break;
		case SST_PROCI_PUUSED_TIME:
			if(Flag&MF_PUUSED_TIME)
			{
				m_CPUUsedTime=Value;
				UpdateFlag |= MF_PUUSED_TIME;
			}
			break;
		case SST_PROCI_PUUSED:
			if(Flag&MF_PUUSED)
			{
				m_CPUUsed=Value;
				UpdateFlag |= MF_PUUSED;
			}
			break;
		case SST_PROCI_MEMORY_USED:
			if(Flag&MF_MEMORY_USED)
			{
				m_MemoryUsed=Value;
				UpdateFlag |= MF_MEMORY_USED;
			}
			break;
		case SST_PROCI_VIRTUAL_MEMORY_USED:
			if(Flag&MF_VIRTUAL_MEMORY_USED)
			{
				m_VirtualMemoryUsed=Value;
				UpdateFlag |= MF_VIRTUAL_MEMORY_USED;
			}
			break;
		case SST_PROCI_DISK_FREE:
			if(Flag&MF_DISK_FREE)
			{
				m_DiskFree=Value;
				UpdateFlag |= MF_DISK_FREE;
			}
			break;
		
		}
	}
	
//<GenerateArea7End>
	
	m_ModifyFlag|=UpdateFlag;	

}

void CProcessInfo::CloneFrom(const CProcessInfo& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea8Start>
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];
	UINT64 UpdateFlag=0;
	
	if(Flag&MF_PROCESS_ID)
	{
		m_ProcessID=DataObject.m_ProcessID;
		UpdateFlag|=MF_PROCESS_ID;
	}
	if(Flag&MF_IMAGE_FILE_PATH)
	{
		m_ImageFilePath=DataObject.m_ImageFilePath;
		UpdateFlag|=MF_IMAGE_FILE_PATH;
	}
	if(Flag&MF_IMAGE_FILE_TIME)
	{
		m_ImageFileTime=DataObject.m_ImageFileTime;
		UpdateFlag|=MF_IMAGE_FILE_TIME;
	}
	if(Flag&MF_PUUSED_TIME)
	{
		m_CPUUsedTime=DataObject.m_CPUUsedTime;
		UpdateFlag|=MF_PUUSED_TIME;
	}
	if(Flag&MF_PUUSED)
	{
		m_CPUUsed=DataObject.m_CPUUsed;
		UpdateFlag|=MF_PUUSED;
	}
	if(Flag&MF_MEMORY_USED)
	{
		m_MemoryUsed=DataObject.m_MemoryUsed;
		UpdateFlag|=MF_MEMORY_USED;
	}
	if(Flag&MF_VIRTUAL_MEMORY_USED)
	{
		m_VirtualMemoryUsed=DataObject.m_VirtualMemoryUsed;
		UpdateFlag|=MF_VIRTUAL_MEMORY_USED;
	}
	if(Flag&MF_DISK_FREE)
	{
		m_DiskFree=DataObject.m_DiskFree;
		UpdateFlag|=MF_DISK_FREE;
	}
	
	
//<GenerateArea8End>

	m_ModifyFlag|=UpdateFlag;
}

UINT CProcessInfo::GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea9Start>
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_PROCESS_INFO];
	UINT Size=0;
	if(Flag&MF_PROCESS_ID)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
	}
	
	if(Flag&MF_IMAGE_FILE_PATH)
	{
		Size+=CSmartStruct::GetStringMemberSize(m_ImageFilePath);
	}
	
	if(Flag&MF_IMAGE_FILE_TIME)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
	}
	
	if(Flag&MF_PUUSED_TIME)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
	}
	
	if(Flag&MF_PUUSED)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(float));
	}
	
	if(Flag&MF_MEMORY_USED)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
	}
	
	if(Flag&MF_VIRTUAL_MEMORY_USED)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
	}
	
	if(Flag&MF_DISK_FREE)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
	}
	
	
		
//<GenerateArea9End>	
	return Size;

}

CProcessInfo& CProcessInfo::operator=(const CProcessInfo& DataObject)
{
//<GenerateArea10Start>
	CloneFrom(DataObject,DOMF_PROCESS_INFO_FULL);
//<GenerateArea10End>	
	return *this;
}