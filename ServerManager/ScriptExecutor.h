/****************************************************************************/
/*                                                                          */
/*      文件名:    ScriptExecutor.h                                         */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

class CServerManagerClient;

class CScriptExecutor :
	public CNetConnection
{
protected:
	CServerManagerClient *			m_pManager;	
	CEasyBuffer						m_AssembleBuffer;		

	CESBolanStack					m_CurScript;
	CEasyScriptExecutor				m_ScriptExecutor;
	CESVariableList					m_VarList;
	CESFunctionList					m_FnList;
	CESThread						m_ESThread;

	UINT							m_Param;
	int								m_Status;
	CEasyString						m_WorkDir;
	UINT							m_DownloadServiceIndex;
	CEasyString						m_DownloadSourcePath;
	CEasyString						m_DownloadTargetPath;
	CEasyBuffer						m_TransferBuffer;
	UINT							m_TransferLeftSize;
	UINT							m_OriginSize;
	CEasyBuffer						m_PackProp;
	CEasyTime						m_CurTransferFileLastWriteTime;

	enum STATUS
	{
		ST_NONE,
		ST_DOWNLOAD,
	};

	enum SCRIPT_INTERRUPT_TYPE
	{		
		IPT_DOWNLOAD_FILE=2,		
	};

public:
	CScriptExecutor(void);
	~CScriptExecutor(void);

	BOOL Init(UINT ID,CServerManagerClient * pManager);
	virtual void Destory();

	virtual void OnConnection(bool IsSucceed);
	virtual void OnDisconnection();

	virtual void OnRecvData(const BYTE * pData, UINT DataSize);

	virtual int Update(int ProcessPacketLimit=DEFAULT_SERVER_PROCESS_PACKET_LIMIT);

	void OnMsg(CSmartStruct& Msg);
	void SendMsg(CSmartStruct& Msg);
	void SendMsg(UINT MsgID);

	bool ExcuteScript(LPCTSTR szScript,LPCTSTR szWorkDir,UINT Param);

protected:

	void ContinueScriptInterrupt(int IPTType,int Result);

	//void QueryStartDownload(UINT ServiceIndex,LPCTSTR SourceFilePath,LPCTSTR TargetFilePath);
	//void QueryDownloadData(UINT ServiceIndex);

	//void OnStartDownloadResult(int Result,UINT TotalSize,UINT OrgTotalSize,LPCVOID pPackProps,UINT PackPropSize,time_t LastWriteTime);
	//void OnDownloadData(int Result,UINT TotalSize,UINT LeftSize,LPCVOID pData,UINT DataSize);

	//int DownloadFileFN(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount);
	
};
