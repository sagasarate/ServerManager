// DlgServiceEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgServiceEditor.h"





// CDlgServiceEditor 对话框

IMPLEMENT_DYNAMIC(CDlgServiceEditor, CDialog)

CDlgServiceEditor::CDlgServiceEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServiceEditor::IDD, pParent)
	, m_ExecFile(_T(""))
{
	m_pConnection = NULL;
	m_IsAddNew = true;
}

CDlgServiceEditor::~CDlgServiceEditor()
{
}

void CDlgServiceEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_SERVICE_TYPE, m_cbServiceType);

	UINT ServiceID = m_ServiceInfo.GetServiceID();
	DDX_Text(pDX, IDC_EDIT_SERVICE_ID, ServiceID);
	m_ServiceInfo.SetServiceID(ServiceID);

	CString Temp = m_ServiceInfo.GetName();
	DDX_Text(pDX, IDC_EDIT_SERVICE_NAME, Temp);
	m_ServiceInfo.SetName((LPCTSTR)Temp);

	Temp = m_ServiceInfo.GetImageFilePath();
	DDX_Text(pDX, IDC_EDIT_IMAGE_FILE_NAME, Temp);
	m_ServiceInfo.SetImageFilePath((LPCTSTR)Temp);

	Temp = m_ServiceInfo.GetWorkDir();
	DDX_Text(pDX, IDC_EDIT_WORK_DIR, Temp);
	m_ServiceInfo.SetWorkDir((LPCTSTR)Temp);

	Temp = m_ServiceInfo.GetStartupParam();
	DDX_Text(pDX, IDC_EDIT_STARTUP_PARAM, Temp);
	m_ServiceInfo.SetStartupParam((LPCTSTR)Temp);

	Temp = m_ServiceInfo.GetControlPipeName();
	DDX_Text(pDX, IDC_EDIT_CONTROL_PIPE_NAME, Temp);
	m_ServiceInfo.SetControlPipeName((LPCTSTR)Temp);

	Temp = m_ServiceInfo.GetShutdownCmd();
	DDX_Text(pDX, IDC_EDIT_SHUTDOWN_CMD, Temp);
	m_ServiceInfo.SetShutdownCmd((LPCTSTR)Temp);

	UINT RestartupTime = m_ServiceInfo.GetRestartupTime();
	DDX_Text(pDX, IDC_EDIT_RESTARTUP_TIME, RestartupTime);
	m_ServiceInfo.SetRestartupTime(RestartupTime);

	BOOL KeepRunning = m_ServiceInfo.GetKeepRunning() ? TRUE : FALSE;
	DDX_Check(pDX, IDC_CHECK_KEEP_RUNNING, KeepRunning);
	m_ServiceInfo.SetKeepRunning(KeepRunning ? true : false);

	BOOL LogServiceStatus = m_ServiceInfo.GetLogStatusToFile() ? TRUE : FALSE;
	DDX_Check(pDX, IDC_CHECK_LOG_SERVER_STATUS, LogServiceStatus);
	m_ServiceInfo.SetLogStatusToFile(LogServiceStatus != FALSE);

	int CharSet = m_ServiceInfo.GetCharSet();
	DDX_Text(pDX, IDC_EDIT_CHAR_SET, CharSet);
	m_ServiceInfo.SetCharSet(CharSet);

	if (pDX->m_bSaveAndValidate)
	{
		m_ServiceInfo.SetType(m_cbServiceType.GetCurSel());
	}
	else
	{
		m_cbServiceType.SetCurSel(m_ServiceInfo.GetType());
	}

	DDX_Text(pDX, IDC_EDIT_EXEC_FILE, m_ExecFile);
	DDX_Control(pDX, IDC_LIST, m_lvList);
}


BEGIN_MESSAGE_MAP(CDlgServiceEditor, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgServiceEditor::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD_EXEC_FILE, &CDlgServiceEditor::OnBnClickedButtonAddExecFile)
	ON_BN_CLICKED(IDC_BUTTON_DEL_EXEC_FILE, &CDlgServiceEditor::OnBnClickedButtonDelExecFile)
END_MESSAGE_MAP()


// CDlgServiceEditor 消息处理程序
bool CDlgServiceEditor::Init(CServerConnection * pConnection, CServiceInfo * pServiceInfo)
{
	m_pConnection = pConnection;
	if (pServiceInfo)
	{
		m_ServiceInfo = *pServiceInfo;		
		m_IsAddNew = false;
		CWnd * pedServiceID = GetDlgItem(IDC_EDIT_SERVICE_ID);
		if (pedServiceID)
		{
			pedServiceID->EnableWindow(false);
		}
		UpdateData(false);
		FillList();
		return true;
	}
	else
	{
		m_ServiceInfo.Clear();
		m_IsAddNew = true;
		CWnd * pedServiceID = GetDlgItem(IDC_EDIT_SERVICE_ID);
		if (pedServiceID)
		{
			pedServiceID->EnableWindow(true);
		}
		const CEasyArray<CServiceInfo>& ServiceList = m_pConnection->GetServiceList();
		m_ServiceInfo.SetServiceID(0);
		for (UINT i = 0; i < ServiceList.GetCount(); i++)
		{
			if (m_ServiceInfo.GetServiceID() == 0 || m_ServiceInfo.GetServiceID() < ServiceList[i].GetServiceID())
				m_ServiceInfo.SetServiceID(ServiceList[i].GetServiceID());
		}
		m_ServiceInfo.SetServiceID(m_ServiceInfo.GetServiceID() + 1);
		UpdateData(false);
		FillList();
		return true;
	}
}

BOOL CDlgServiceEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lvList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lvList.InsertColumn(0, _T("文件路径"), LVCFMT_LEFT, 500);

	m_cbServiceType.ResetContent();
	for (UINT i = 0; i < g_SERVICE_TYPE_COUNT; i++)
	{
		m_cbServiceType.AddString(SERVICE_TYPE_To_Str(i));
	}
	m_cbServiceType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgServiceEditor::FillList()
{
	m_lvList.DeleteAllItems();
	for (UINT i = 0; i < m_ServiceInfo.GetOtherExecFileList().GetCount(); i++)
	{
		int Item = m_lvList.InsertItem(i, m_ServiceInfo.GetOtherExecFileList()[i]);
		m_lvList.SetItemData(Item, i);
	}
}

void CDlgServiceEditor::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_ServiceInfo.GetName().IsEmpty())
	{
		AfxMessageBox(_T("请输入服务名称"));
		return;
	}
	if ((m_ServiceInfo.GetType() == SERVICE_TYPE_NORMAL || m_ServiceInfo.GetType() == SERVICE_TYPE_WIN_SERVICE) && m_ServiceInfo.GetImageFilePath().IsEmpty())
	{
		AfxMessageBox(_T("请输入服务可执行文件名称"));
		return;
	}
	if (m_IsAddNew)
	{
		if (m_ServiceInfo.GetServiceID() == 0)
		{
			AfxMessageBox(_T("请输入服务ID"));
			return;
		}
		const CEasyArray<CServiceInfo>& ServiceList = m_pConnection->GetServiceList();
		for (UINT i = 0; i < ServiceList.GetCount(); i++)
		{
			if (m_ServiceInfo.GetServiceID() == ServiceList[i].GetServiceID())
			{
				AfxMessageBox(_T("服务ID已被使用，请更换"));
				return;
			}
		}

		//if (m_ServiceInfo.GetCharSet() == CP_UTF8)
		//{
		//	char Buffer[1024];
		//	UINT Len = AnsiToUTF8(m_ServiceInfo.GetName(), m_ServiceInfo.GetName().GetLength(), Buffer, 1000);
		//	Buffer[Len] = 0;
		//	m_ServiceInfo.SetName(Buffer);
		//}
		m_pConnection->QueryAddService(m_ServiceInfo);
	}
	else
	{
		//if (m_ServiceInfo.GetCharSet() == CP_UTF8)
		//{
		//	char Buffer[1024];
		//	UINT Len = AnsiToUTF8(m_ServiceInfo.GetName(), m_ServiceInfo.GetName().GetLength(), Buffer, 1000);
		//	Buffer[Len] = 0;
		//	m_ServiceInfo.SetName(Buffer);
		//}
		m_pConnection->QueryEditService(m_ServiceInfo);
	}

	

	CDialog::OnOK();
}


void CDlgServiceEditor::OnBnClickedButtonAddExecFile()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	m_ServiceInfo.GetOtherExecFileList().Add((LPCTSTR)m_ExecFile);
	FillList();
}


void CDlgServiceEditor::OnBnClickedButtonDelExecFile()
{
	// TODO:  在此添加控件通知处理程序代码
	POSITION Pos = m_lvList.GetFirstSelectedItemPosition();
	if (Pos)
	{
		int Item = m_lvList.GetNextSelectedItem(Pos);
		UINT Index = m_lvList.GetItemData(Item);
		if (Index < m_ServiceInfo.GetOtherExecFileList().GetCount())
		{
			if (AfxMessageBoxEx(MB_YESNO, 0, _T("是否要删除[%s]?"), m_ServiceInfo.GetOtherExecFileList()[Index]) == IDYES)
			{
				m_ServiceInfo.GetOtherExecFileList().Delete(Index);
				FillList();
			}
		}
	}
}
