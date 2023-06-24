#include "stdafx.h"



CServiceInfo::CServiceInfo()
{
	Clear();
}
CServiceInfo::~CServiceInfo()
{
}

void CServiceInfo::Clear()
{
//<GenerateArea1Start>
	CProcessInfo::Clear();

	m_ModifyFlag=0;
	   
	m_ServiceID=0;
	m_Name.Clear();
	m_Type=0;
	m_WorkDir.Clear();
	m_StartupParam.Clear();
	m_Status=0;
	m_WorkStatus=0;
	m_LastOperation=0;
	m_LastStatusChangeTime=0;
	m_RestartupTime=0;
	m_ControlPipeName.Clear();
	m_ShutdownCmd.Clear();
	m_CharSet=0;
	m_KeepRunning=false;
	m_LogStatusToFile=false;
	m_OtherExecFileList.SetTag(_T("StructData"));
	m_OtherExecFileList.Clear();
	m_OtherExecFileList.Create(16,8);
	
//<GenerateArea1End>

}

void CServiceInfo::GetModifyFlag(DATA_OBJECT_MODIFY_FLAGS& ModifyFlags,int GetType,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea2Start>
	CProcessInfo::GetModifyFlag(ModifyFlags,GetType,MemberFlags);
	switch(GetType)
	{
	case GMFT_COPY:
		ModifyFlags[DATA_OBJECT_FLAG_SERVICE_INFO]=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];
		break;
	case GMFT_AND:
		ModifyFlags[DATA_OBJECT_FLAG_SERVICE_INFO]&=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];
		break;
	case GMFT_OR:
		ModifyFlags[DATA_OBJECT_FLAG_SERVICE_INFO]|=m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];
		break;
	}
	
//<GenerateArea2End>
}

bool CServiceInfo::IsModified(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{

	bool IsModified=false;
//<GenerateArea3Start>	
	IsModified=IsModified||CProcessInfo::IsModified(MemberFlags);
	
	IsModified=IsModified||(m_ModifyFlag&MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO])!=0;
	
	IsModified=IsModified
		;
	
//<GenerateArea3End>
	return IsModified;

}

void CServiceInfo::ClearModifyFlag(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea4Start>
	CProcessInfo::ClearModifyFlag(MemberFlags);
	m_ModifyFlag&=~MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];
	
//<GenerateArea4End>
}

bool CServiceInfo::MakeUpdatePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea5Start>
	DATA_OBJECT_MODIFY_FLAGS ModifyFlags;
	ZeroMemory(&ModifyFlags,sizeof(ModifyFlags));
	GetModifyFlag(ModifyFlags,GMFT_OR,MemberFlags);	
//<GenerateArea5End>
	return MakePacket(Packet,ModifyFlags);
}

bool CServiceInfo::MakePacket(CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea6Start>
	UINT FailCount=0;
	
	if(!CProcessInfo::MakePacket(Packet,MemberFlags))
		FailCount++;
	
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];

	if(Flag&MF_SERVICE_ID)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_SERVICE_ID,m_ServiceID),FailCount);
	}
	
	if(Flag&MF_NAME)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_NAME,m_Name),FailCount);
	}
	
	if(Flag&MF_TYPE)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_TYPE,m_Type),FailCount);
	}
	
	if(Flag&MF_WORK_DIR)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_WORK_DIR,m_WorkDir),FailCount);
	}
	
	if(Flag&MF_STARTUP_PARAM)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_STARTUP_PARAM,m_StartupParam),FailCount);
	}
	
	if(Flag&MF_STATUS)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_STATUS,m_Status),FailCount);
	}
	
	if(Flag&MF_WORK_STATUS)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_WORK_STATUS,m_WorkStatus),FailCount);
	}
	
	if(Flag&MF_LAST_OPERATION)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_LAST_OPERATION,m_LastOperation),FailCount);
	}
	
	if(Flag&MF_LAST_STATUS_CHANGE_TIME)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_LAST_STATUS_CHANGE_TIME,m_LastStatusChangeTime),FailCount);
	}
	
	if(Flag&MF_RESTARTUP_TIME)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_RESTARTUP_TIME,m_RestartupTime),FailCount);
	}
	
	if(Flag&MF_CONTROL_PIPE_NAME)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_CONTROL_PIPE_NAME,m_ControlPipeName),FailCount);
	}
	
	if(Flag&MF_SHUTDOWN_CMD)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_SHUTDOWN_CMD,m_ShutdownCmd),FailCount);
	}
	
	if(Flag&MF_CHAR_SET)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_CHAR_SET,m_CharSet),FailCount);
	}
	
	if(Flag&MF_KEEP_RUNNING)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_KEEP_RUNNING,m_KeepRunning),FailCount);
	}
	
	if(Flag&MF_LOG_STATUS_TO_FILE)
	{
		CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_LOG_STATUS_TO_FILE,m_LogStatusToFile),FailCount);
	}
	
	if(Flag&MF_OTHER_EXEC_FILE_LIST)
	{
		UINT BufferSize;
		void * pBuffer=Packet.PrepareMember(BufferSize);
		{
			CSmartStruct Packet(pBuffer,BufferSize,true);
			for(size_t i=0;i<m_OtherExecFileList.GetCount();i++)
			{
				CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SRVI_OTHER_EXEC_FILE_LIST,m_OtherExecFileList[i]),FailCount);
			}
			BufferSize=Packet.GetDataLen();
		}
		Packet.FinishMember(SST_SRVI_OTHER_EXEC_FILE_LIST,BufferSize);
	}
	
//<GenerateArea6End>
	return FailCount==0;

}

void CServiceInfo::ParsePacket(const CSmartStruct& Packet,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea7Start>
	
	CProcessInfo::ParsePacket(Packet,MemberFlags);
	
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];
	UINT64 UpdateFlag=0;
	
	
	void * Pos=Packet.GetFirstMemberPosition();
	while(Pos)
	{
		WORD MemberID;
		CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
		switch(MemberID)
		{
		case SST_SRVI_SERVICE_ID:
			if(Flag&MF_SERVICE_ID)
			{
				m_ServiceID=Value;
				UpdateFlag|=MF_SERVICE_ID;
			}
			break;
		case SST_SRVI_NAME:
			if(Flag&MF_NAME)
			{
				Value.GetString(m_Name);
				UpdateFlag|=MF_NAME;
			}
			break;
		case SST_SRVI_TYPE:
			if(Flag&MF_TYPE)
			{
				m_Type=Value;
				UpdateFlag|=MF_TYPE;
			}
			break;
		case SST_SRVI_WORK_DIR:
			if(Flag&MF_WORK_DIR)
			{
				Value.GetString(m_WorkDir);
				UpdateFlag|=MF_WORK_DIR;
			}
			break;
		case SST_SRVI_STARTUP_PARAM:
			if(Flag&MF_STARTUP_PARAM)
			{
				Value.GetString(m_StartupParam);
				UpdateFlag|=MF_STARTUP_PARAM;
			}
			break;
		case SST_SRVI_STATUS:
			if(Flag&MF_STATUS)
			{
				m_Status=Value;
				UpdateFlag|=MF_STATUS;
			}
			break;
		case SST_SRVI_WORK_STATUS:
			if(Flag&MF_WORK_STATUS)
			{
				m_WorkStatus=Value;
				UpdateFlag|=MF_WORK_STATUS;
			}
			break;
		case SST_SRVI_LAST_OPERATION:
			if(Flag&MF_LAST_OPERATION)
			{
				m_LastOperation=Value;
				UpdateFlag|=MF_LAST_OPERATION;
			}
			break;
		case SST_SRVI_LAST_STATUS_CHANGE_TIME:
			if(Flag&MF_LAST_STATUS_CHANGE_TIME)
			{
				m_LastStatusChangeTime=Value;
				UpdateFlag|=MF_LAST_STATUS_CHANGE_TIME;
			}
			break;
		case SST_SRVI_RESTARTUP_TIME:
			if(Flag&MF_RESTARTUP_TIME)
			{
				m_RestartupTime=Value;
				UpdateFlag|=MF_RESTARTUP_TIME;
			}
			break;
		case SST_SRVI_CONTROL_PIPE_NAME:
			if(Flag&MF_CONTROL_PIPE_NAME)
			{
				Value.GetString(m_ControlPipeName);
				UpdateFlag|=MF_CONTROL_PIPE_NAME;
			}
			break;
		case SST_SRVI_SHUTDOWN_CMD:
			if(Flag&MF_SHUTDOWN_CMD)
			{
				Value.GetString(m_ShutdownCmd);
				UpdateFlag|=MF_SHUTDOWN_CMD;
			}
			break;
		case SST_SRVI_CHAR_SET:
			if(Flag&MF_CHAR_SET)
			{
				m_CharSet=Value;
				UpdateFlag|=MF_CHAR_SET;
			}
			break;
		case SST_SRVI_KEEP_RUNNING:
			if(Flag&MF_KEEP_RUNNING)
			{
				m_KeepRunning=Value;
				UpdateFlag|=MF_KEEP_RUNNING;
			}
			break;
		case SST_SRVI_LOG_STATUS_TO_FILE:
			if(Flag&MF_LOG_STATUS_TO_FILE)
			{
				m_LogStatusToFile=Value;
				UpdateFlag|=MF_LOG_STATUS_TO_FILE;
			}
			break;
		case SST_SRVI_OTHER_EXEC_FILE_LIST:
			if(Flag&MF_OTHER_EXEC_FILE_LIST)
			{
				m_OtherExecFileList.Clear();
				CSmartStruct Packet=Value;
				void * Pos=Packet.GetFirstMemberPosition();
				while(Pos)
				{
					WORD MemberID;
					CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
					switch(MemberID)
					{
					case SST_SRVI_OTHER_EXEC_FILE_LIST:
						{	
							CEasyString	ArrayElement;
							Value.GetString(ArrayElement);
							UpdateFlag|=MF_OTHER_EXEC_FILE_LIST;
							m_OtherExecFileList.Add(ArrayElement);
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

void CServiceInfo::CloneFrom(const CServiceInfo& DataObject,const DATA_OBJECT_MODIFY_FLAGS& MemberFlags)
{
//<GenerateArea8Start>
	
	CProcessInfo::CloneFrom(DataObject,MemberFlags);
	
	
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];
	UINT64 UpdateFlag=0;
	
	if(Flag&MF_SERVICE_ID)
	{
		m_ServiceID=DataObject.m_ServiceID;
		UpdateFlag|=MF_SERVICE_ID;
	}
	if(Flag&MF_NAME)
	{
		m_Name=DataObject.m_Name;
		UpdateFlag|=MF_NAME;
	}
	if(Flag&MF_TYPE)
	{
		m_Type=DataObject.m_Type;
		UpdateFlag|=MF_TYPE;
	}
	if(Flag&MF_WORK_DIR)
	{
		m_WorkDir=DataObject.m_WorkDir;
		UpdateFlag|=MF_WORK_DIR;
	}
	if(Flag&MF_STARTUP_PARAM)
	{
		m_StartupParam=DataObject.m_StartupParam;
		UpdateFlag|=MF_STARTUP_PARAM;
	}
	if(Flag&MF_STATUS)
	{
		m_Status=DataObject.m_Status;
		UpdateFlag|=MF_STATUS;
	}
	if(Flag&MF_WORK_STATUS)
	{
		m_WorkStatus=DataObject.m_WorkStatus;
		UpdateFlag|=MF_WORK_STATUS;
	}
	if(Flag&MF_LAST_OPERATION)
	{
		m_LastOperation=DataObject.m_LastOperation;
		UpdateFlag|=MF_LAST_OPERATION;
	}
	if(Flag&MF_LAST_STATUS_CHANGE_TIME)
	{
		m_LastStatusChangeTime=DataObject.m_LastStatusChangeTime;
		UpdateFlag|=MF_LAST_STATUS_CHANGE_TIME;
	}
	if(Flag&MF_RESTARTUP_TIME)
	{
		m_RestartupTime=DataObject.m_RestartupTime;
		UpdateFlag|=MF_RESTARTUP_TIME;
	}
	if(Flag&MF_CONTROL_PIPE_NAME)
	{
		m_ControlPipeName=DataObject.m_ControlPipeName;
		UpdateFlag|=MF_CONTROL_PIPE_NAME;
	}
	if(Flag&MF_SHUTDOWN_CMD)
	{
		m_ShutdownCmd=DataObject.m_ShutdownCmd;
		UpdateFlag|=MF_SHUTDOWN_CMD;
	}
	if(Flag&MF_CHAR_SET)
	{
		m_CharSet=DataObject.m_CharSet;
		UpdateFlag|=MF_CHAR_SET;
	}
	if(Flag&MF_KEEP_RUNNING)
	{
		m_KeepRunning=DataObject.m_KeepRunning;
		UpdateFlag|=MF_KEEP_RUNNING;
	}
	if(Flag&MF_LOG_STATUS_TO_FILE)
	{
		m_LogStatusToFile=DataObject.m_LogStatusToFile;
		UpdateFlag|=MF_LOG_STATUS_TO_FILE;
	}
	if(Flag&MF_OTHER_EXEC_FILE_LIST)
	{
		m_OtherExecFileList=DataObject.m_OtherExecFileList;
		UpdateFlag|=MF_OTHER_EXEC_FILE_LIST;
	}
	
	
//<GenerateArea8End>

	m_ModifyFlag|=UpdateFlag;
}

UINT CServiceInfo::GetSmartStructSize(const DATA_OBJECT_MODIFY_FLAGS& MemberFlags) const
{
//<GenerateArea9Start>
	UINT64 Flag=MemberFlags[DATA_OBJECT_FLAG_SERVICE_INFO];
	UINT Size=0;
	if(Flag&MF_SERVICE_ID)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
	}
	
	if(Flag&MF_NAME)
	{
		Size+=CSmartStruct::GetStringMemberSize(m_Name);
	}
	
	if(Flag&MF_TYPE)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
	}
	
	if(Flag&MF_WORK_DIR)
	{
		Size+=CSmartStruct::GetStringMemberSize(m_WorkDir);
	}
	
	if(Flag&MF_STARTUP_PARAM)
	{
		Size+=CSmartStruct::GetStringMemberSize(m_StartupParam);
	}
	
	if(Flag&MF_STATUS)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
	}
	
	if(Flag&MF_WORK_STATUS)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
	}
	
	if(Flag&MF_LAST_OPERATION)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
	}
	
	if(Flag&MF_LAST_STATUS_CHANGE_TIME)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
	}
	
	if(Flag&MF_RESTARTUP_TIME)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
	}
	
	if(Flag&MF_CONTROL_PIPE_NAME)
	{
		Size+=CSmartStruct::GetStringMemberSize(m_ControlPipeName);
	}
	
	if(Flag&MF_SHUTDOWN_CMD)
	{
		Size+=CSmartStruct::GetStringMemberSize(m_ShutdownCmd);
	}
	
	if(Flag&MF_CHAR_SET)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(int));
	}
	
	if(Flag&MF_KEEP_RUNNING)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
	}
	
	if(Flag&MF_LOG_STATUS_TO_FILE)
	{
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
	}
	
	if(Flag&MF_OTHER_EXEC_FILE_LIST)
	{
		for(size_t i=0;i<m_OtherExecFileList.GetCount();i++)
		{
			Size+=CSmartStruct::GetStringMemberSize(m_OtherExecFileList[i]);
		}
		Size+=CSmartStruct::GetStructMemberSize(0);
	}
	
		
	
	Size+=CProcessInfo::GetSmartStructSize(MemberFlags);
	
//<GenerateArea9End>	
	return Size;

}

CServiceInfo& CServiceInfo::operator=(const CServiceInfo& DataObject)
{
//<GenerateArea10Start>
	CloneFrom(DataObject,DOMF_SERVICE_INFO_FULL);
//<GenerateArea10End>	
	return *this;
}