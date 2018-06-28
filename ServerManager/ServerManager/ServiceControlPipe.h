#pragma once

class CServiceControlPipe :
	public CNamedPipe
{
protected:
	UINT			m_ServiceID;
	CEasyBuffer		m_AssembleBuffer;
	CSmartStruct	m_MsgPacket;
	CEasyTimer		m_KeepAliveTimer;
public:
	CServiceControlPipe(UINT ServiceID);
	~CServiceControlPipe();


	virtual int Update(int ProcessLimit = 32);
	virtual void OnConnection(bool Succeed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const BYTE * pData, UINT DataSize);

	void OnMsg(CSmartStruct& MsgPacket);

	bool SendCommand(UINT ConnectionID, LPCTSTR szCommand);
	bool QueryEnableLogRecv(bool Enable);
	bool QueryGetServerStatus(UINT ConnectionID, const CSmartStruct& StatusListPacket);
	bool QueryGetAllServerStatus(UINT ConnectionID);
	bool QueryGetServerStatusFormat(UINT ConnectionID);
	bool QueryGetServerWorkStatus();
};

