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

CDlgServerUpdate::UPDATE_INFO * CDlgServerUpdate::GetUpdateInfo(LPCTSTR szServiceName, bool NewOnNotExist)
{
	for (UINT i = 0; i < m_UpdateList.GetCount(); i++)
	{
		if (m_UpdateList[i].ServiceName.CompareNoCase(szServiceName) == 0)
		{
			return m_UpdateList.GetObject(i);
		}
	}
	if (NewOnNotExist)
	{
		UPDATE_INFO * pUpdateInfo = m_UpdateList.AddEmpty();
		pUpdateInfo->ServiceName = szServiceName;
		return pUpdateInfo;
	}
	return NULL;
}
bool CDlgServerUpdate::LoadUpdateList(LPCTSTR szFileName)
{
	CCSVReader CSVReader;

	if (CSVReader.Open(szFileName))
	{
		m_UpdateList.Clear();
		m_ServiceList.Clear();
		bool HaveMissFile = false;
		for (UINT i = 0; i < CSVReader.GetRowCount(); i++)
		{
			LPCTSTR szServiceName = CSVReader.GetDataString(i, _T("ServiceName"), _T(""));
			int UpdateType = CSVReader.GetDataInt(i, _T("UpdateType"), 0);
			LPCTSTR szFileName = CSVReader.GetDataString(i, _T("FileName"), _T(""));
			LPCTSTR szSrcDir = CSVReader.GetDataString(i, _T("SrcDir"), _T(""));
			LPCTSTR szDestDir = CSVReader.GetDataString(i, _T("DestDir"), _T(""));

			if (UpdateType)
			{
				UPDATE_FILE_INFO FileInfo;
				FileInfo.UpdateType = UpdateType;
				if (CFileTools::IsAbsolutePath(szSrcDir))
					FileInfo.SrcPath.Format(_T("%s\\%s"), szSrcDir, szFileName);
				else
					FileInfo.SrcPath.Format(_T("%s\\%s\\%s"), (LPCTSTR)CFileTools::GetModulePath(NULL), szSrcDir, szFileName);

				FileInfo.SrcPath = CFileTools::MakeFullPath(FileInfo.SrcPath);
				FileInfo.DestPath.Format(_T("%s/%s"), szDestDir, szFileName);

				if (CFileTools::IsFileExist(FileInfo.SrcPath))
				{
					UPDATE_INFO * pUpdateInfo = GetUpdateInfo(szServiceName, true);
					if (pUpdateInfo)
					{
						pUpdateInfo->UpdateFileList.Add(FileInfo);
					}
				}
				else
				{
					HaveMissFile = true;
					AfxMessageBoxEx(MB_OK, 0, _T("文件%s不存在"), (LPCTSTR)FileInfo.SrcPath);
				}				
			}
		}
		void * Pos = CServerManagerClientApp::GetInstance()->GetFirstServerConnectionPos();
		while (Pos)
		{
			CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetNextServerConnection(Pos);
			const CEasyArray<CServiceInfo>& ServiceList = pConnection->GetServiceList();
			for (UINT i = 0; i < ServiceList.GetCount(); i++)
			{
				if (GetUpdateInfo(ServiceList[i].GetName(), false))
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
	for (int i = 0; i < m_lvList.GetItemCount(); i++)
	{
		if(m_lvList.GetCheck(i))
		{
			UINT Index = m_lvList.GetItemData(i);
			if (Index < m_ServiceList.GetCount())
			{
				UPDATE_SERVICE_INFO& ServiceInfo = m_ServiceList[Index];
				UPDATE_INFO * pUpdateInfo = GetUpdateInfo(ServiceInfo.ServiceName, false);
				CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ServiceInfo.ConnectionID);
				if (pUpdateInfo&&pConnection)
				{
					if (pUpdateInfo->UpdateFileList.GetCount())
					{
						CTaskQueue& TaskQueue = pConnection->GetTaskQueue();
						
						TaskQueue.AddShutdownServiceTask(ServiceInfo.ServiceID, true);
						
						for (UINT j = 0; j < pUpdateInfo->UpdateFileList.GetCount(); j++)
						{
							UPDATE_FILE_INFO& FileInfo = pUpdateInfo->UpdateFileList[j];
							if (FileInfo.UpdateType == UPDTAE_TYPE_EXEC)
							{
								TaskQueue.AddUploadTask(ServiceInfo.ServiceID, FileInfo.SrcPath, FileInfo.DestPath, false);
							}
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
	for (int i = 0; i < m_lvList.GetItemCount(); i++)
	{
		if (m_lvList.GetCheck(i))
		{
			UINT Index = m_lvList.GetItemData(i);
			if (Index < m_ServiceList.GetCount())
			{
				UPDATE_SERVICE_INFO& ServiceInfo = m_ServiceList[Index];
				UPDATE_INFO * pUpdateInfo = GetUpdateInfo(ServiceInfo.ServiceName, false);
				CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ServiceInfo.ConnectionID);
				if (pUpdateInfo&&pConnection)
				{
					if (pUpdateInfo->UpdateFileList.GetCount())
					{
						CTaskQueue& TaskQueue = pConnection->GetTaskQueue();

						for (UINT j = 0; j < pUpdateInfo->UpdateFileList.GetCount(); j++)
						{
							UPDATE_FILE_INFO& FileInfo = pUpdateInfo->UpdateFileList[j];
							if (FileInfo.UpdateType == UPDTAE_TYPE_CONFIG)
							{
								TaskQueue.AddUploadTask(ServiceInfo.ServiceID, FileInfo.SrcPath, FileInfo.DestPath, false);
							}
						}
						TaskQueue.AddReloadConfigDataTask(ServiceInfo.ServiceID);
					}
				}
			}
		}
	}
}
