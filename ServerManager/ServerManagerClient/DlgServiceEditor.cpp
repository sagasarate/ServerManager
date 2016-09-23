// DlgServiceEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgServiceEditor.h"





// CDlgServiceEditor 对话框

IMPLEMENT_DYNAMIC(CDlgServiceEditor, CDialog)

CDlgServiceEditor::CDlgServiceEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServiceEditor::IDD, pParent)
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
	DDX_Text(pDX, IDC_EDIT_SERVICE_ID, m_ServiceInfo.ServiceID);
	DDX_Text(pDX, IDC_EDIT_SERVICE_NAME, m_ServiceInfo.Name);
	DDX_Text(pDX, IDC_EDIT_IMAGE_FILE_NAME, m_ServiceInfo.ImageFilePath);
	DDX_Text(pDX, IDC_EDIT_WORK_DIR, m_ServiceInfo.WorkDir);
	DDX_Text(pDX, IDC_EDIT_STARTUP_PARAM, m_ServiceInfo.StartupParam);
	DDX_Text(pDX, IDC_EDIT_RESTARTUP_TIME, m_ServiceInfo.RestartupTime);
	BOOL KeepRunning = m_ServiceInfo.LastOperation == SERVICE_OPERATION_STARTUP ? TRUE : FALSE;
	DDX_Check(pDX, IDC_CHECK_KEEP_RUNNING, KeepRunning);
	m_ServiceInfo.LastOperation = KeepRunning ? SERVICE_OPERATION_STARTUP : SERVICE_OPERATION_SHUTDOWN;

	if (pDX->m_bSaveAndValidate)
	{
		m_ServiceInfo.Type = m_cbServiceType.GetCurSel();
	}
	else
	{
		m_cbServiceType.SetCurSel(m_ServiceInfo.Type);
	}
	
}


BEGIN_MESSAGE_MAP(CDlgServiceEditor, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgServiceEditor::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgServiceEditor 消息处理程序
bool CDlgServiceEditor::Init(CServerConnection * pConnection, UINT ServiceID)
{
	m_pConnection = pConnection;
	if (ServiceID)
	{
		const SERVICE_INFO * pServiceInfo = m_pConnection->GetServiceInfo(ServiceID);
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
			return true;
		}
		else
		{
			return false;
		}		
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
		const CEasyArray<SERVICE_INFO>& ServiceList = m_pConnection->GetServiceList();
		m_ServiceInfo.ServiceID = 0;
		for (UINT i = 0; i < ServiceList.GetCount(); i++)
		{
			if (m_ServiceInfo.ServiceID == 0 || m_ServiceInfo.ServiceID<ServiceList[i].ServiceID)
				m_ServiceInfo.ServiceID = ServiceList[i].ServiceID;
		}
		m_ServiceInfo.ServiceID++;
		UpdateData(false);
		return true;
	}
}

BOOL CDlgServiceEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_cbServiceType.ResetContent();
	for (UINT i = 0; i < SERVICE_TYPE_MAX; i++)
	{
		m_cbServiceType.AddString(g_szSERVICE_TYPE[i]);
	}
	m_cbServiceType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgServiceEditor::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_ServiceInfo.Name.IsEmpty())
	{
		AfxMessageBox(_T("请输入服务名称"));
		return;
	}
	if ((m_ServiceInfo.Type == SERVICE_TYPE_NORMAL || m_ServiceInfo.Type == SERVICE_TYPE_WIN_SERVICE) && m_ServiceInfo.ImageFilePath.IsEmpty())
	{
		AfxMessageBox(_T("请输入服务可执行文件名称"));
		return;
	}
	if (m_IsAddNew)
	{
		if (m_ServiceInfo.ServiceID == 0)
		{
			AfxMessageBox(_T("请输入服务ID"));
			return;
		}
		const CEasyArray<SERVICE_INFO>& ServiceList = m_pConnection->GetServiceList();
		for (UINT i = 0; i < ServiceList.GetCount(); i++)
		{
			if (m_ServiceInfo.ServiceID == ServiceList[i].ServiceID)
			{
				AfxMessageBox(_T("服务ID已被使用，请更换"));
				return;
			}
		}
		m_pConnection->QueryAddService(m_ServiceInfo);
	}
	else
	{
		m_pConnection->QueryEditService(m_ServiceInfo);
	}
	CDialog::OnOK();
}
