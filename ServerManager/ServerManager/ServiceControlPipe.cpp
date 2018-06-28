#include "stdafx.h"



CServiceControlPipe::CServiceControlPipe(UINT ServiceID)
{
	m_ServiceID = ServiceID;
	m_AssembleBuffer.Create(CONTROL_PIPE_BUFFER_SIZE * 2);
	m_MsgPacket.Create(CONTROL_PIPE_BUFFER_SIZE);
}


CServiceControlPipe::~CServiceControlPipe()
{
}



int CServiceControlPipe::Update(int ProcessLimit)
{
	if (IsDisconnected() && m_KeepAliveTimer.IsTimeOut(CONTROL_PIPE_RECREATE_TIME))
	{
		m_KeepAliveTimer.SaveTime();
		CServiceInfoEx * pServiceInfo = CMainThread::GetInstance()->GetService()->GetServiceInfo(m_ServiceID);
		if (pServiceInfo)
		{
			CEasyString PipeName;
			PipeName.Format(_T("%s(%u)"), (LPCTSTR)pServiceInfo->GetControlPipeName(), pServiceInfo->GetProcessID());
			Open(PipeName, CONTROL_PIPE_BUFFER_SIZE);
			LogDebug(_T("尝试连接管道[%s]"), (LPCTSTR)PipeName);
		}
		else
		{
			Log(_T("无法找到服务[%u]的信息"), m_ServiceID);
		}
	}
	else if (m_KeepAliveTimer.IsTimeOut(CMainConfig::GetInstance()->GetProcessInfoFetchTime() * 5))
	{
		Log(_T("控制台命名管道KeepAlive超时"));
		Destory();
	}
	return CNamedPipe::Update(ProcessLimit);
}
void CServiceControlPipe::OnConnection(bool Succeed)
{
	if (Succeed)
	{
		LogDebug(_T("控制台命名管道连接成功"));
	}
	else
	{
		Log(_T("控制台命名管道连接失败"));
	}
	m_KeepAliveTimer.SaveTime();
}
void CServiceControlPipe::OnDisconnection()
{
	LogDebug(_T("控制台命名管道连接断开"));
	m_AssembleBuffer.SetUsedSize(0);
	m_KeepAliveTimer.SaveTime();
}

void CServiceControlPipe::OnRecvData(const BYTE * pData, UINT DataSize)
{
	m_KeepAliveTimer.SaveTime();
	if (m_AssembleBuffer.PushBack(pData, DataSize))
	{
		UINT PacketSize = 0;
		UINT Pos = 1;
		while (m_AssembleBuffer.Peek(Pos, &PacketSize, sizeof(UINT)))
		{
			PacketSize += CSmartStruct::GetEmptyStructSize();
			if (m_AssembleBuffer.GetUsedSize() >= PacketSize)
			{
				CSmartStruct MsgPacket(m_AssembleBuffer.GetBuffer(), PacketSize, false);
				OnMsg(MsgPacket);
				m_AssembleBuffer.PopFront(NULL, PacketSize);
				Pos = 1;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		Log(_T("装配缓冲溢出"));
	}
}

void CServiceControlPipe::OnMsg(CSmartStruct& MsgPacket)
{
	UINT MsgID = MsgPacket.GetMember(SC_SST_MSG_ID);
	switch (MsgID)
	{
	case SC_MSG_QUERY_SERVER_STATUS_RESULT:
		{
			UINT ConnectionID = MsgPacket.GetMember(SC_SST_PARAM1);
			CServerManagerClient * pClient = CMainThread::GetInstance()->GetService()->GetConnection(ConnectionID);
			if (pClient)
			{
				CServerManagerAckMsgCaller MsgCaller(pClient);

				MsgCaller.GetServerStatusAck(MSG_RESULT_SUCCEED, m_ServiceID, MsgPacket.GetMember(SC_SST_DATA));
			}
			else
			{
				LogDebug(_T("无法找到客户端连接%u"), ConnectionID);
			}
		}
		break;
	case SC_MSG_GET_SERVER_STATUS_FORMAT_INFO_RESULT:
		{
			UINT ConnectionID = MsgPacket.GetMember(SC_SST_PARAM1);
			CServerManagerClient * pClient = CMainThread::GetInstance()->GetService()->GetConnection(ConnectionID);
			if (pClient)
			{
				CServerManagerAckMsgCaller MsgCaller(pClient);

				MsgCaller.GetServerStatusFormatAck(MSG_RESULT_SUCCEED, m_ServiceID, MsgPacket.GetMember(SC_SST_DATA));
			}
			else
			{
				LogDebug(_T("无法找到客户端连接%u"), ConnectionID);
			}
		}
		break;
	case SC_MSG_QUERY_SERVER_WORK_STATUS_RESULT:
		{
			CMainThread::GetInstance()->GetService()->OnGetServiceWorkStatus(m_ServiceID, MsgPacket.GetMember(SC_SST_DATA));
		}
		break;
	case SC_MSG_SERVER_LOG:
		{
			CMainThread::GetInstance()->GetService()->OnServerLogMsg(m_ServiceID, MsgPacket.GetMember(SC_SST_DATA));
		}
		break;
	case SC_MSG_EXEC_COMMAND_RESULT:
		{
			UINT ConnectionID = MsgPacket.GetMember(SC_SST_PARAM1);
			short Result = MsgPacket.GetMember(SC_SST_DATA);
			CServerManagerClient * pClient = CMainThread::GetInstance()->GetService()->GetConnection(ConnectionID);
			if (pClient)
			{
				CServerManagerAckMsgCaller MsgCaller(pClient);

				if (Result == 0)
					MsgCaller.SendCommandAck(MSG_RESULT_SUCCEED, m_ServiceID);
				else
					MsgCaller.SendCommandAck(MSG_RESULT_FAILED, m_ServiceID);
			}
			else
			{
				LogDebug(_T("无法找到客户端连接%u"), ConnectionID);
			}
		}
		break;
	}
}

bool CServiceControlPipe::SendCommand(UINT ConnectionID, LPCTSTR szCommand)
{
	if (IsConnected())
	{
		m_MsgPacket.Clear();
		m_MsgPacket.AddMember(SC_SST_MSG_ID, SC_MSG_EXEC_COMMAND);
		m_MsgPacket.AddMember(SC_SST_EC_COMMAND_STR, szCommand);	
		m_MsgPacket.AddMember(SC_SST_PARAM1, ConnectionID);
		if (SendData(m_MsgPacket.GetData(), m_MsgPacket.GetDataLen()))
		{
			LogDebug(_T("(%u)已发出命令:%s"), ConnectionID, szCommand);
			return true;
		}
		else
		{
			Log(_T("发出命令失败"));
			return false;
		}
	}	
	return false;
}
bool CServiceControlPipe::QueryEnableLogRecv(bool Enable)
{
	if (IsConnected())
	{
		m_MsgPacket.Clear();
		if (Enable)
			m_MsgPacket.AddMember(SC_SST_MSG_ID, SC_MSG_LINK_LOG);
		else
			m_MsgPacket.AddMember(SC_SST_MSG_ID, SC_MSG_UNLINK_LOG);
		
		return SendData(m_MsgPacket.GetData(), m_MsgPacket.GetDataLen());
	}
	return false;
}
bool CServiceControlPipe::QueryGetServerStatus(UINT ConnectionID, const CSmartStruct& StatusListPacket)
{
	if (IsConnected())
	{
		m_MsgPacket.Clear();
		m_MsgPacket.AddMember(SC_SST_MSG_ID, SC_MSG_QUERY_SERVER_STATUS);
		m_MsgPacket.AddMember(SC_SST_DATA, StatusListPacket);
		m_MsgPacket.AddMember(SC_SST_PARAM1, ConnectionID);
		return SendData(m_MsgPacket.GetData(), m_MsgPacket.GetDataLen());
	}
	return false;
}
bool CServiceControlPipe::QueryGetAllServerStatus(UINT ConnectionID)
{
	if (IsConnected())
	{
		m_MsgPacket.Clear();
		m_MsgPacket.AddMember(SC_SST_MSG_ID, SC_MSG_QUERY_ALL_SERVER_STATUS);
		m_MsgPacket.AddMember(SC_SST_PARAM1, ConnectionID);
		return SendData(m_MsgPacket.GetData(), m_MsgPacket.GetDataLen());
	}
	return false;
}
bool CServiceControlPipe::QueryGetServerStatusFormat(UINT ConnectionID)
{
	if (IsConnected())
	{
		m_MsgPacket.Clear();
		m_MsgPacket.AddMember(SC_SST_MSG_ID, SC_MSG_GET_SERVER_STATUS_FORMAT_INFO);
		m_MsgPacket.AddMember(SC_SST_PARAM1, ConnectionID);
		return SendData(m_MsgPacket.GetData(), m_MsgPacket.GetDataLen());
	}
	return false;
}

bool CServiceControlPipe::QueryGetServerWorkStatus()
{
	if (IsConnected())
	{
		m_MsgPacket.Clear();
		m_MsgPacket.AddMember(SC_SST_MSG_ID, SC_MSG_QUERY_SERVER_WORK_STATUS);
		return SendData(m_MsgPacket.GetData(), m_MsgPacket.GetDataLen());
	}
	return false;
}