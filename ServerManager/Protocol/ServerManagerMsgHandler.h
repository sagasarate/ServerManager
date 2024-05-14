#pragma once

class CServerManagerMsgHandler:public IServerManager
{

public:
	CServerManagerMsgHandler(void);
	~CServerManagerMsgHandler(void);

	void InitMsgMap(CEasyMap<MSG_ID_TYPE,MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgLogin(const CSmartStruct& Packet);
	int HandleMsgGetServiceList(const CSmartStruct& Packet);
	int HandleMsgGetProcessList(const CSmartStruct& Packet);
	int HandleMsgGetNetAdapterList(const CSmartStruct& Packet);
	int HandleMsgGetServiceInfo(const CSmartStruct& Packet);
	int HandleMsgServiceStartup(const CSmartStruct& Packet);
	int HandleMsgServiceShutdown(const CSmartStruct& Packet);
	int HandleMsgRunProgram(const CSmartStruct& Packet);
	int HandleMsgProcessShutdown(const CSmartStruct& Packet);
	int HandleMsgExecuteScript(const CSmartStruct& Packet);
	int HandleMsgBrowseServiceDir(const CSmartStruct& Packet);
	int HandleMsgFileDownloadStart(const CSmartStruct& Packet);
	int HandleMsgFileDownloadData(const CSmartStruct& Packet);
	int HandleMsgFileDownloadFinish(const CSmartStruct& Packet);
	int HandleMsgFileUploadStart(const CSmartStruct& Packet);
	int HandleMsgFileUploadData(const CSmartStruct& Packet);
	int HandleMsgFileUploadFinish(const CSmartStruct& Packet);
	int HandleMsgCreateDir(const CSmartStruct& Packet);
	int HandleMsgDeleteFile(const CSmartStruct& Packet);
	int HandleMsgChangeFileMode(const CSmartStruct& Packet);
	int HandleMsgAddService(const CSmartStruct& Packet);
	int HandleMsgEditService(const CSmartStruct& Packet);
	int HandleMsgDeleteService(const CSmartStruct& Packet);
	int HandleMsgSendCommand(const CSmartStruct& Packet);
	int HandleMsgEnableLogRecv(const CSmartStruct& Packet);
	int HandleMsgGetServerStatus(const CSmartStruct& Packet);
	int HandleMsgGetAllServerStatus(const CSmartStruct& Packet);
	int HandleMsgGetServerStatusFormat(const CSmartStruct& Packet);
	int HandleMsgFileCompare(const CSmartStruct& Packet);
	

};

