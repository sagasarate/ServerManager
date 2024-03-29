﻿#pragma once

class CServerManagerAckMsgHandler:public IServerManagerAck
{

public:
	CServerManagerAckMsgHandler(void);
	~CServerManagerAckMsgHandler(void);

	void InitMsgMap(CEasyMap<MSG_ID_TYPE,MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgLoginAck(CSmartStruct& Packet);
	int HandleMsgGetServiceListAck(CSmartStruct& Packet);
	int HandleMsgGetProcessListAck(CSmartStruct& Packet);
	int HandleMsgGetNetAdapterListAck(CSmartStruct& Packet);
	int HandleMsgGetServiceInfoAck(CSmartStruct& Packet);
	int HandleMsgServiceStartupAck(CSmartStruct& Packet);
	int HandleMsgServiceShutdownAck(CSmartStruct& Packet);
	int HandleMsgRunProgramAck(CSmartStruct& Packet);
	int HandleMsgProcessShutdownAck(CSmartStruct& Packet);
	int HandleMsgExecuteScriptAck(CSmartStruct& Packet);
	int HandleMsgBrowseServiceDirAck(CSmartStruct& Packet);
	int HandleMsgFileDownloadStartAck(CSmartStruct& Packet);
	int HandleMsgFileDownloadDataAck(CSmartStruct& Packet);
	int HandleMsgFileDownloadFinishAck(CSmartStruct& Packet);
	int HandleMsgFileUploadStartAck(CSmartStruct& Packet);
	int HandleMsgFileUploadDataAck(CSmartStruct& Packet);
	int HandleMsgFileUploadFinishAck(CSmartStruct& Packet);
	int HandleMsgCreateDirAck(CSmartStruct& Packet);
	int HandleMsgDeleteFileAck(CSmartStruct& Packet);
	int HandleMsgChangeFileModeAck(CSmartStruct& Packet);
	int HandleMsgAddServiceAck(CSmartStruct& Packet);
	int HandleMsgEditServiceAck(CSmartStruct& Packet);
	int HandleMsgDeleteServiceAck(CSmartStruct& Packet);
	int HandleMsgSendCommandAck(CSmartStruct& Packet);
	int HandleMsgEnableLogRecvAck(CSmartStruct& Packet);
	int HandleMsgConsoleLogNotify(CSmartStruct& Packet);
	int HandleMsgGetServerStatusAck(CSmartStruct& Packet);
	int HandleMsgGetServerStatusFormatAck(CSmartStruct& Packet);
	int HandleMsgFileCompareAck(CSmartStruct& Packet);
	

};

