#pragma once

class CServerManagerAckMsgHandler:public IServerManagerAck
{

public:
	CServerManagerAckMsgHandler(void);
	~CServerManagerAckMsgHandler(void);

	void InitMsgMap(CEasyMap<MSG_ID_TYPE,MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgLoginAck(const CSmartStruct& Packet);
	int HandleMsgGetServiceListAck(const CSmartStruct& Packet);
	int HandleMsgGetProcessListAck(const CSmartStruct& Packet);
	int HandleMsgGetNetAdapterListAck(const CSmartStruct& Packet);
	int HandleMsgGetServiceInfoAck(const CSmartStruct& Packet);
	int HandleMsgServiceStartupAck(const CSmartStruct& Packet);
	int HandleMsgServiceShutdownAck(const CSmartStruct& Packet);
	int HandleMsgRunProgramAck(const CSmartStruct& Packet);
	int HandleMsgProcessShutdownAck(const CSmartStruct& Packet);
	int HandleMsgExecuteScriptAck(const CSmartStruct& Packet);
	int HandleMsgBrowseServiceDirAck(const CSmartStruct& Packet);
	int HandleMsgFileDownloadStartAck(const CSmartStruct& Packet);
	int HandleMsgFileDownloadDataAck(const CSmartStruct& Packet);
	int HandleMsgFileDownloadFinishAck(const CSmartStruct& Packet);
	int HandleMsgFileUploadStartAck(const CSmartStruct& Packet);
	int HandleMsgFileUploadDataAck(const CSmartStruct& Packet);
	int HandleMsgFileUploadFinishAck(const CSmartStruct& Packet);
	int HandleMsgCreateDirAck(const CSmartStruct& Packet);
	int HandleMsgDeleteFileAck(const CSmartStruct& Packet);
	int HandleMsgChangeFileModeAck(const CSmartStruct& Packet);
	int HandleMsgAddServiceAck(const CSmartStruct& Packet);
	int HandleMsgEditServiceAck(const CSmartStruct& Packet);
	int HandleMsgDeleteServiceAck(const CSmartStruct& Packet);
	int HandleMsgSendCommandAck(const CSmartStruct& Packet);
	int HandleMsgEnableLogRecvAck(const CSmartStruct& Packet);
	int HandleMsgConsoleLogNotify(const CSmartStruct& Packet);
	int HandleMsgGetServerStatusAck(const CSmartStruct& Packet);
	int HandleMsgGetServerStatusFormatAck(const CSmartStruct& Packet);
	int HandleMsgFileCompareAck(const CSmartStruct& Packet);
	

};

