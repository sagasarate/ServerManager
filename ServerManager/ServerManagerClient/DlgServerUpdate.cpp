// DlgServerUpdate.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgServerUpdate.h"



// CDlgServerUpdate 对话框

IMPLEMENT_DYNAMIC(CDlgServerUpdate, CDialog)

CDlgServerUpdate::CDlgServerUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerUpdate::IDD, pParent)
{

}

CDlgServerUpdate::~CDlgServerUpdate()
{
}

void CDlgServerUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgServerUpdate, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_UPDATE_LIST, &CDlgServerUpdate::OnBnClickedButtonLoadUpdateList)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_EXEC, &CDlgServerUpdate::OnBnClickedButtonUpdateExec)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_CONFIG, &CDlgServerUpdate::OnBnClickedButtonUpdateConfig)
END_MESSAGE_MAP()


// CDlgServerUpdate 消息处理程序


BOOL CDlgServerUpdate::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	m_lvList.InsertColumn(0, _T("服务器"), LVCFMT_LEFT, 150);
	m_lvList.InsertColumn(1, _T("服务"), LVCFMT_LEFT, 150);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

LPCTSTR CDlgServerUpdate::HaveUpdate(const CEasyString& ServiceName, UPDTAE_TYPE UpdateType)
{
	for (UPDATE_FILE_INFO& Info : m_UpdateFileList)
	{
		if ((Info.UpdateType&UpdateType) && (ServiceName.Find(Info.ServiceName) >= 0))
		{
			return Info.ServiceName;
		}
	}
	return NULL;
}

bool CDlgServerUpdate::LoadUpdateList(LPCTSTR szFileName)
{
	CCSVReader CSVReader;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CServerManagerClientView* pView = NULL;
	if (pMainFrame)
	{
		pView = (CServerManagerClientView*)pMainFrame->GetActiveView();
	}

	if (CSVReader.Open(szFileName))
	{
		m_UpdateFileList.Clear();
		m_ServiceList.Clear();
		bool HaveMissFile = false;
		for (UINT i = 0; i < CSVReader.GetRowCount(); i++)
		{
			int UpdateType = CSVReader.GetDataInt(i, _T("UpdateType"), 0);
			
			if (UpdateType)
			{
				UPDATE_FILE_INFO FileInfo;
				FileInfo.UpdateType = UpdateType;
				FileInfo.IncludeChildDir = CSVReader.GetDataBool(i, _T("IncludeChildDir"), false);
				FileInfo.ServiceName = CSVReader.GetDataString(i, _T("ServiceName"), _T(""));
				FileInfo.SrcDir = CSVReader.GetDataString(i, _T("SrcDir"), _T(""));
				FileInfo.DestDir = CSVReader.GetDataString(i, _T("DestDir"), _T(""));
				LPCTSTR szFileName = CSVReader.GetDataString(i, _T("FileName"), _T(""));
				LPCTSTR szExceptPatterns = CSVReader.GetDataString(i, _T("ExceptPatterns"), _T(""));

				CStringSplitter Splitter;
				Splitter.Splitter(szFileName, ';');
				for (UINT i = 0; i < Splitter.GetCount(); i++)
				{
					FileInfo.Files.Add(Splitter[i]);
				}
				Splitter.Splitter(szExceptPatterns, ';');
				for (UINT i = 0; i < Splitter.GetCount(); i++)
				{
					FileInfo.ExceptPatterns.Add(Splitter[i]);
				}
				m_UpdateFileList.Add(FileInfo);
			}
		}
		void * Pos = CServerManagerClientApp::GetInstance()->GetFirstServerConnectionPos();
		while (Pos)
		{
			CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetNextServerConnection(Pos);
			const CEasyArray<CServiceInfo>& ServiceList = pConnection->GetServiceList();
			for (UINT i = 0; i < ServiceList.GetCount(); i++)
			{
				if (HaveUpdate(ServiceList[i].GetName(), UPDTAE_TYPE_ALL))
				{
					UPDATE_SERVICE_INFO * pInfo = m_ServiceList.AddEmpty();
					pInfo->ConnectionID = pConnection->GetID();
					pInfo->ServiceID = ServiceList[i].GetServiceID();
					pInfo->ServiceName = ServiceList[i].GetName();
					pInfo->ServerAddress = pConnection->GetServerAddress();
				}
			}			
		}
		FillList();
		return true;
	}
	return false;
}

void CDlgServerUpdate::FillList()
{
	m_lvList.DeleteAllItems();
	for (UINT i = 0; i < m_ServiceList.GetCount(); i++)
	{
		UPDATE_SERVICE_INFO& Info = m_ServiceList[i];
		int Item = m_lvList.InsertItem(i, Info.ServerAddress);
		m_lvList.SetItemText(Item, 1, Info.ServiceName);		
		m_lvList.SetItemData(Item, i);
	}
}
bool CDlgServerUpdate::IsMatch(CEasyArray<CEasyString> Patterns, const CEasyString& FileName)
{
	for (CEasyString& Pattern : Patterns)
	{
		if (FileName.Find(Pattern, 0, true) >= 0)
			return true;
	}
	return false;
}
void CDlgServerUpdate::SearchFiles(LPCTSTR SearchDir, LPCTSTR FilePattern, CEasyArray<CEasyString> ExceptPatterns, LPCTSTR DestDir, CEasyArray<SELECT_ITEM_INFO>& FileList, bool Recursion)
{
	CEasyString SearchPattern;
	SearchPattern.Format(_T("%s\\%s"), SearchDir, FilePattern);
	CFileSearcher FileSearcher;
	FileSearcher.FindFirst(SearchPattern);
	while (FileSearcher.FindNext())
	{
		if (!FileSearcher.IsDirectory() && !IsMatch(ExceptPatterns, FileSearcher.GetFileName()))
		{
			SELECT_ITEM_INFO* pInfo = FileList.AddEmpty();
			pInfo->IsSelected = true;
			pInfo->Item = FileSearcher.GetFilePath();
			pInfo->Param1.Format(_T("%s\\%s"), DestDir, (LPCTSTR)FileSearcher.GetFileName());
		}
	}
	if (Recursion)
	{
		SearchPattern.Format(_T("%s\\*"), SearchDir);
		FileSearcher.FindFirst(SearchPattern);
		while (FileSearcher.FindNext())
		{
			if (FileSearcher.IsDirectory() && (!FileSearcher.IsDots()) && (!IsMatch(ExceptPatterns, FileSearcher.GetFileName())))
			{
				CEasyString NewDestDir;
				NewDestDir.Format(_T("%s\\%s"), DestDir, (LPCTSTR)FileSearcher.GetFileName());
				SearchFiles(FileSearcher.GetFilePath(), FilePattern, ExceptPatterns, NewDestDir, FileList, Recursion);
			}
		}
	}
}
void CDlgServerUpdate::GetFiles(const CEasyArray<UPDATE_FILE_INFO>& UpdateList, CEasyArray<SELECT_ITEM_INFO>& FileList, const CEasyString& ServiceName, UPDTAE_TYPE MatchType)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CServerManagerClientView* pView = NULL;
	if (pMainFrame)
	{
		pView = (CServerManagerClientView*)pMainFrame->GetActiveView();
	}
	CEasyString ModuleDir = CFileTools::GetModulePath(NULL);
	for (const UPDATE_FILE_INFO& UpdateInfo : UpdateList)
	{
		if ((UpdateInfo.UpdateType & MatchType) && (ServiceName.Find(UpdateInfo.ServiceName) >= 0))
		{
			for (const CEasyString& FileName : UpdateInfo.Files)
			{
				if ((FileName.Find('*') >= 0) || (FileName.Find('?') >= 0))
				{
					//带有匹配符，需要搜索
					CEasyString FilePath;
					FilePath.Format(_T("%s\\%s"), (LPCTSTR)ModuleDir, (LPCTSTR)UpdateInfo.SrcDir);
					SearchFiles(FilePath, FileName, UpdateInfo.ExceptPatterns, UpdateInfo.DestDir, FileList, UpdateInfo.IncludeChildDir);
				}
				else
				{
					CEasyString FilePath;
					FilePath.Format(_T("%s\\%s\\%s"), (LPCTSTR)ModuleDir, (LPCTSTR)UpdateInfo.SrcDir, (LPCTSTR)FileName);
					if (CFileTools::IsFileExist(FilePath))
					{
						SELECT_ITEM_INFO* pInfo = FileList.AddEmpty();
						pInfo->IsSelected = true;
						pInfo->Item = CFileTools::MakeFullPath(FilePath);
						pInfo->Param1.Format(_T("%s\\%s"), (LPCTSTR)UpdateInfo.DestDir, (LPCTSTR)FileName);
					}
					else
					{
						if (pView)
							pView->PrintLog(LOG_TYPE_ERROR, _T("文件不存在:%s"), (LPCTSTR)FilePath);
					}
				}
			}
		}
	}
}


void CDlgServerUpdate::OnBnClickedButtonLoadUpdateList()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog Dlg(true, _T("*.csv"), _T("*.csv"));

	if (Dlg.DoModal() == IDOK)
	{
		if (LoadUpdateList(Dlg.GetPathName()))
		{
			CServerManagerClientApp::GetInstance()->SetServerUpdateListFile(Dlg.GetPathName());
		}
	}
}


void CDlgServerUpdate::OnBnClickedButtonUpdateExec()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgListSelector Dlg;
	CEasyString RecentMatchName;
	for (int i = 0; i < m_lvList.GetItemCount(); i++)
	{
		if(m_lvList.GetCheck(i))
		{
			UINT Index = m_lvList.GetItemData(i);
			if (Index < m_ServiceList.GetCount())
			{
				UPDATE_SERVICE_INFO& ServiceInfo = m_ServiceList[Index];				
				CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ServiceInfo.ConnectionID);
				if (pConnection)
				{
					const CServiceInfo * pServiceInfo = pConnection->GetServiceInfo(ServiceInfo.ServiceID);
					if (pServiceInfo)
					{
						CTaskQueue& TaskQueue = pConnection->GetTaskQueue();
						LPCTSTR MatchName = HaveUpdate(ServiceInfo.ServiceName, UPDTAE_TYPE_EXEC);
						if (MatchName && (RecentMatchName != MatchName))
						{
							Dlg.m_ItemList.Clear();
							GetFiles(m_UpdateFileList, Dlg.m_ItemList, ServiceInfo.ServiceName, UPDTAE_TYPE_EXEC);
							RecentMatchName = MatchName;
							if (Dlg.DoModal() != IDOK)
							{
								break;
							}
						}
						if (pServiceInfo->GetStatus() != SERVICE_STATUS_STOP)
							TaskQueue.AddShutdownServiceTask(ServiceInfo.ServiceID, true);

						for (SELECT_ITEM_INFO& ItemInfo : Dlg.m_ItemList)
						{
							if (ItemInfo.IsSelected)
								TaskQueue.AddUploadTask(ServiceInfo.ServiceID, ItemInfo.Item, ItemInfo.Param1, false);
						}
						TaskQueue.AddStartupServiceTask(ServiceInfo.ServiceID, false);
					}
				}
			}
		}
	}
}


void CDlgServerUpdate::OnBnClickedButtonUpdateConfig()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgListSelector Dlg;
	CEasyString RecentMatchName;
	for (int i = 0; i < m_lvList.GetItemCount(); i++)
	{
		if (m_lvList.GetCheck(i))
		{
			UINT Index = m_lvList.GetItemData(i);
			if (Index < m_ServiceList.GetCount())
			{
				UPDATE_SERVICE_INFO& ServiceInfo = m_ServiceList[Index];
				CServerConnection* pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ServiceInfo.ConnectionID);
				if (pConnection)
				{
					const CServiceInfo* pServiceInfo = pConnection->GetServiceInfo(ServiceInfo.ServiceID);
					if (pServiceInfo)
					{
						CTaskQueue& TaskQueue = pConnection->GetTaskQueue();
						LPCTSTR MatchName = HaveUpdate(ServiceInfo.ServiceName, UPDTAE_TYPE_CONFIG);
						if (MatchName && (RecentMatchName != MatchName))
						{
							Dlg.m_ItemList.Clear();
							GetFiles(m_UpdateFileList, Dlg.m_ItemList, ServiceInfo.ServiceName, UPDTAE_TYPE_CONFIG);
							RecentMatchName = MatchName;
							if (Dlg.DoModal() != IDOK)
							{
								break;
							}
						}
						for (SELECT_ITEM_INFO& ItemInfo : Dlg.m_ItemList)
						{
							if (ItemInfo.IsSelected)
								TaskQueue.AddUploadTask(ServiceInfo.ServiceID, ItemInfo.Item, ItemInfo.Param1, false);
						}
					}
				}
			}
		}
	}
}
