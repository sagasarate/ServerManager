#pragma once

class CServerManagerMsgHandler:public IServerManager
{

public:
	CServerManagerMsgHandler(void);
	~CServerManagerMsgHandler(void);

	void InitMsgMap(CEasyMap<MSG_ID_TYPE,MSG_HANDLE_INFO>& MsgMap);	

	int HandleMsgLogin(CSmartStruct& Packet);
	int HandleMsgGetServiceList(CSmartStruct& Packet);
	int HandleMsgGetProcessList(CSmartStruct& Packet);
	int HandleMsgGetNetAdapterList(CSmartStruct& Packet);
	int HandleMsgGetServiceInfo(CSmartStruct& Packet);
	int HandleMsgServiceStartup(CSmartStruct& Packet);
	int HandleMsgServiceShutdown(CSmartStruct& Packet);
	int HandleMsgRunProgram(CSmartStruct& Packet);
	int HandleMsgProcessShutdown(CSmartStruct& Packet);
	int HandleMsgExecuteScript(CSmartStruct& Packet);
	int HandleMsgBrowseServiceDir(CSmartStruct& Packet);
	int HandleMsgFileDownloadStart(CSmartStruct& Packet);
	int HandleMsgFileDownloadData(CSmartStruct& Packet);
	int HandleMsgFileDownloadFinish(CSmartStruct& Packet);
	int HandleMsgFileUploadStart(CSmartStruct& Packet);
	int HandleMsgFileUploadData(CSmartStruct& Packet);
	int HandleMsgFileUploadFinish(CSmartStruct& Packet);
	int HandleMsgCreateDir(CSmartStruct& Packet);
	int HandleMsgDeleteFile(CSmartStruct& Packet);
	int HandleMsgChangeFileMode(CSmartStruct& Packet);
	int HandleMsgAddService(CSmartStruct& Packet);
	int HandleMsgEditService(CSmartStruct& Packet);
	int HandleMsgDeleteService(CSmartStruct& Packet);
	int HandleMsgSendCommand(CSmartStruct& Packet);
	int HandleMsgEnableLogRecv(CSmartStruct& Packet);
	int HandleMsgGetServerStatus(CSmartStruct& Packet);
	int HandleMsgGetAllServerStatus(CSmartStruct& Packet);
	int HandleMsgGetServerStatusFormat(CSmartStruct& Packet);
	int HandleMsgFileCompare(CSmartStruct& Packet);
	

};

