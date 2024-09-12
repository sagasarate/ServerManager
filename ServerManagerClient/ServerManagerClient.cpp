/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerClient.cpp                                  */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ServerManagerClient.cpp : 定义应用程序的类行为。
//
#include "stdafx.h"
#include "ServerManagerClient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerClientApp

BEGIN_MESSAGE_MAP(CServerManagerClientApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CServerManagerClientApp::OnAppAbout)	
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_SERVER_MANAGE, &CServerManagerClientApp::OnServerManage)
	ON_COMMAND(ID_SERVER_UPDATE, &CServerManagerClientApp::OnServerUpdate)
END_MESSAGE_MAP()


// CServerManagerClientApp 构造

CServerManagerClientApp::CServerManagerClientApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	m_ConnectionPool.Create(128, 128, 32);
}


// 唯一的一个 CServerManagerClientApp 对象

CServerManagerClientApp theApp;


CServerManagerClientApp * CServerManagerClientApp::GetInstance()
{
	return &theApp;
}


// CServerManagerClientApp 初始化

BOOL CServerManagerClientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。

	CExceptionParser::GetInstance()->Init(EXCEPTION_SET_DEFAULT_HANDLER);

	CSmartValue::INTERNAL_STRING_CODE_PAGE = CEasyString::STRING_CODE_PAGE_UTF8;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	AfxInitRichEdit2();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CServerManagerClientDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CServerManagerClientView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	CEasyString LogFileName;
	CEasyString ModulePath = CFileTools::GetModulePath(NULL);

	LogFileName.Format("%s/Log/ServerManagerClient", (LPCTSTR)ModulePath);
	CAsyncFileLogPrinter* pLog = MONITORED_NEW(_T("CServerManagerClientApp"), CAsyncFileLogPrinter, ILogPrinter::LOG_LEVEL_NORMAL | ILogPrinter::LOG_LEVEL_DEBUG, LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_CHANNEL_MAIN, pLog);
	SAFE_RELEASE(pLog);

	LogFileName.Format("%s/Log/ServerManagerClient.NetLib", (LPCTSTR)ModulePath);
	pLog = MONITORED_NEW(_T("CServerManagerClientApp"), CAsyncFileLogPrinter, ILogPrinter::LOG_LEVEL_NORMAL | ILogPrinter::LOG_LEVEL_DEBUG, LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_NET_CHANNEL, pLog);
	SAFE_RELEASE(pLog);

	m_Server.StartUp(4096, 4, 2048, 2048, 64);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生

	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CServerManagerClientView * pView = NULL;
	if (pMainFrame)
	{
		pView = (CServerManagerClientView *)pMainFrame->GetActiveView();
	}

	m_DlgServerUpdate.Create(m_DlgServerUpdate.IDD, pMainFrame);

	CDlgLogin Dlg;

	Dlg.DoModal();

	m_AccountName = Dlg.m_AccountName;
	m_Password = Dlg.m_Password;

	LoadConfig();

	
	
	return TRUE;
}


int CServerManagerClientApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	m_ConnectionPool.Destory();

	

	m_Server.ShutDown();

	CLogManager::GetInstance()->ReleaseInstance();

	return CWinApp::ExitInstance();
}

BOOL CServerManagerClientApp::OnIdle(LONG lCount)
{
	// TODO: 在此添加专用代码和/或调用基类

	CWinApp::OnIdle(lCount);

	int ProcessCount=0;

	

	void * Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CServerConnection * pConnection = m_ConnectionPool.GetNextObject(Pos);
		ProcessCount += pConnection->Update();		
	}
	if (ProcessCount == 0)
	{
		Sleep(1);
	}
	return TRUE;
}

void CServerManagerClientApp::OnServerManage()
{
	// TODO: 在此添加命令处理程序代码
	CDlgServerManage Dlg;

	
	Dlg.DoModal();
	SaveConfig();
	GetMainView()->RefreshConnection();
}
void CServerManagerClientApp::OnServerUpdate()
{
	// TODO:  在此添加命令处理程序代码

	m_DlgServerUpdate.ShowWindow(SW_SHOW);

	m_DlgServerUpdate.LoadUpdateList(m_ServerUpdateListFile);
}
//void CServerManagerClientApp::CheckServerConnections()
//{
//	for(UINT i=0;i<m_ServerList.GetCount();i++)
//	{
//		m_ServerList[i].ServerStatus=SCS_NO_CONNECTION;
//		for(UINT j=0;j<m_ConnectionList.GetCount();j++)
//		{
//			if(m_ServerList[i].ServerAddress==m_ConnectionList[j]->GetServerAddress()&&
//				m_ServerList[i].ServerPort==m_ConnectionList[j]->GetServerPort())
//			{
//				if(m_ConnectionList[j]->IsConnected())
//				{
//					m_ServerList[i].ServerStatus=SCS_CONNECTING;
//				}
//				else
//				{
//					m_ServerList[i].ServerStatus=SCS_DISCONNECTED;
//				}
//				break;
//			}
//		}
//		if(m_ServerList[i].ServerStatus==SCS_NO_CONNECTION)
//		{
//			CMainFrame * pMainFrame=(CMainFrame *)AfxGetMainWnd();
//			if(pMainFrame)
//			{
//				CServerManagerClientView * pView=(CServerManagerClientView *)pMainFrame->GetActiveView();
//				if(pView)
//				{
//					CServerConnection * pConnection=new CServerConnection(pView,m_ServerList[i].ServerAddress,m_ServerList[i].ServerPort);
//					pConnection->SetServer(&m_Server);
//					m_ConnectionList.Add(pConnection);
//				}
//			}
//			
//		}
//	}
//
//	for(int i=m_ConnectionList.GetCount()-1;i>=0;i--)
//	{
//		bool WantDelete=true;
//		for(UINT j=0;j<m_ServerList.GetCount();j++)
//		{
//			if(m_ServerList[j].ServerAddress==m_ConnectionList[i]->GetServerAddress()&&
//				m_ServerList[j].ServerPort==m_ConnectionList[i]->GetServerPort())
//			{
//				WantDelete=false;
//				break;
//			}
//		}
//		if(WantDelete)
//		{
//			SAFE_RELEASE(m_ConnectionList[i]);
//			m_ConnectionList.Delete(i);
//		}
//	}
//}

void CServerManagerClientApp::LoadConfig()
{
	

	pug::xml_parser Xml;
	if(Xml.parse_file(CFileTools::MakeModuleFullPath(NULL,SETTING_FILE_NAME),pug::parse_trim_attribute))
	{
		xml_node Setting=Xml.document();
		if(Setting.moveto_child(_T("Setting")))
		{
			if (Setting.has_attribute(_T("ServerUpdateListFile")))
				m_ServerUpdateListFile = Setting.attribute(_T("ServerUpdateListFile")).getvalue();

			if (Setting.has_attribute(_T("SystemServiceName")))
				m_SystemServiceName = Setting.attribute(_T("SystemServiceName")).getvalue();

			
			xml_node ServerList=Setting;
			if(ServerList.moveto_child(_T("ServerList")))
			{				
				for(UINT i=0;i<ServerList.children();i++)
				{
					xml_node Server=ServerList.child(i);
					
					CEasyString ServerName = Server.attribute(_T("Name")).getvalue();
					CEasyString ServerGroup = Server.attribute(_T("Group")).getvalue();
					CEasyString ServerAddress=Server.attribute(_T("Address")).getvalue();
					UINT ServerPort=Server.attribute(_T("Port"));
					CEasyString UserName = Server.attribute(_T("UserName")).getvalue();
					CEasyString Password = Server.attribute(_T("Password")).getvalue();

					if (ServerGroup == _T("其他"))
						ServerGroup.Clear();
					if (UserName.IsEmpty())
					{
						UserName = m_AccountName;
					}

					if (Password.IsEmpty())
					{
						Password = m_Password;
					}
					
					CServerConnection * pConnection = AddServerConnection(ServerName, ServerGroup, ServerAddress, ServerPort, UserName, Password);
				}
				GetMainView()->RefreshConnection();
			}			
		}
	}
	else
	{
		AfxGetMainWnd()->MessageBox(_T("装载配置失败！"));
	}
}

void CServerManagerClientApp::SaveConfig()
{
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("gb2312");

	xml_node Setting = Doc.append_child(node_element, _T("Setting"));

	Setting.append_attribute(_T("ServerUpdateListFile"), m_ServerUpdateListFile);
	Setting.append_attribute(_T("SystemServiceName"), m_SystemServiceName);

	xml_node ServerList = Setting.append_child(node_element, _T("ServerList"));

	void * Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CServerConnection * pConnection = m_ConnectionPool.GetNextObject(Pos);
		xml_node Server = ServerList.append_child(node_element, _T("Server"));
		Server.append_attribute(_T("Name"), pConnection->GetName());
		Server.append_attribute(_T("Group"), pConnection->GetGroup());
		Server.append_attribute(_T("Address"), pConnection->GetServerAddress());
		Server.append_attribute(_T("Port"), pConnection->GetServerPort());
		Server.append_attribute(_T("UserName"), pConnection->GetUserName());
		Server.append_attribute(_T("Password"), pConnection->GetPassword());
	}

	if (!Xml.SaveToFile(Doc, CFileTools::MakeModuleFullPath(NULL, SETTING_FILE_NAME)))
	{
		AfxGetMainWnd()->MessageBox(_T("保存配置失败！"));
	}
}
CServerConnection * CServerManagerClientApp::AddServerConnection(LPCTSTR Name, LPCTSTR Group, LPCTSTR Address, UINT Port, LPCTSTR UserName, LPCTSTR Password)
{
	CMainFrame * pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CServerManagerClientView * pView = NULL;
	if (pMainFrame)
	{
		pView = (CServerManagerClientView *)pMainFrame->GetActiveView();
	}

	CServerConnection * pConnection = m_ConnectionPool.NewObject();
	if (pConnection)
	{
		if (pConnection->Init(pView, &m_Server, Name, Group, Address, Port, UserName, Password))
		{
			return pConnection;
		}
		DeleteServerConnection(pConnection->GetID());
	}
	return false;
}
CServerConnection * CServerManagerClientApp::GetServerConnection(LPCTSTR ServerAddress)
{
	void * Pos = m_ConnectionPool.GetFirstObjectPos();
	while (Pos)
	{
		CServerConnection * pConnection = m_ConnectionPool.GetNextObject(Pos);
		if (pConnection->GetServerAddress() == ServerAddress)
			return pConnection;
	}
	return NULL;
}

CServerConnection * CServerManagerClientApp::GetServerConnection(UINT ConnectionID)
{
	return m_ConnectionPool.GetObject(ConnectionID);
}

bool CServerManagerClientApp::DeleteServerConnection(UINT ConnectionID)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		pConnection->Disconnect();
		pConnection->Destory();
		m_ConnectionPool.DeleteObject(ConnectionID);
		return true;
	}
	return false;
}

void CServerManagerClientApp::StartupService(UINT ConnectionID, UINT ServiceID)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		pConnection->QueryStartupService(ServiceID);
	}
}

void CServerManagerClientApp::ShutdownService(UINT ConnectionID, UINT ServiceID, BYTE ShutDownType)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		pConnection->QueryShutDownService(ServiceID, ShutDownType);
	}		
}

void CServerManagerClientApp::BrowseWorkDir(UINT ConnectionID, UINT ServiceID, LPCTSTR Dir)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		pConnection->QueryBrowseWorkDir(ServiceID, Dir);
	}
}


UINT CServerManagerClientApp::AddDownloadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddDownloadTask(ServiceID, SourceFilePath, TargetFilePath, ContinueTransfer);
	}
	return 0;
}
UINT CServerManagerClientApp::AddUploadTask(UINT ConnectionID, UINT ServiceID, LPCTSTR SourceFilePath, LPCTSTR TargetFilePath, bool ContinueTransfer)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddUploadTask(ServiceID, SourceFilePath, TargetFilePath, ContinueTransfer);
	}
	return 0;
}
UINT CServerManagerClientApp::AddDeleteFileTask(UINT ConnectionID, UINT ServiceID, LPCTSTR TargetFilePath)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddDeleteFileTask(ServiceID, TargetFilePath);
	}
	return 0;
}
UINT CServerManagerClientApp::AddCreateDirTask(UINT ConnectionID, UINT ServiceID, LPCTSTR TargetFilePath)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddCreateDirTask(ServiceID, TargetFilePath);
	}
	return 0;
}
UINT CServerManagerClientApp::AddChangeFileTask(UINT ConnectionID, UINT ServiceID, LPCTSTR TargetFilePath, UINT FileMode)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddChangeFileTask(ServiceID, TargetFilePath, FileMode);
	}
	return 0;
}
UINT CServerManagerClientApp::AddStartupServiceTask(UINT ConnectionID, UINT ServiceID, bool ClearQueueOnFailed)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddStartupServiceTask(ServiceID, ClearQueueOnFailed);
	}
	return 0;
}
UINT CServerManagerClientApp::AddShutdownServiceTask(UINT ConnectionID, UINT ServiceID, bool ClearQueueOnFailed)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddShutdownServiceTask(ServiceID, ClearQueueOnFailed);
	}
	return 0;
}
UINT CServerManagerClientApp::AddReloadConfigDataTask(UINT ConnectionID, UINT ServiceID)
{
	CServerConnection * pConnection = GetServerConnection(ConnectionID);
	if (pConnection)
	{
		return pConnection->GetTaskQueue().AddReloadConfigDataTask(ServiceID);
	}
	return 0;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CServerManagerClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CServerManagerClientApp 消息处理程序




