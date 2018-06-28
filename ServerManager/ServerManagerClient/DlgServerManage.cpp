/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgServerManage.cpp                                      */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgServerManage.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgServerManage.h"




// CDlgServerManage 对话框

IMPLEMENT_DYNAMIC(CDlgServerManage, CDialog)

CDlgServerManage::CDlgServerManage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerManage::IDD, pParent)
	, m_IP(_T(""))
	, m_Port(8300)
	, m_UserName(_T(""))
	, m_Password(_T(""))
{

}

CDlgServerManage::~CDlgServerManage()
{
}

void CDlgServerManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_lvServerList);
	DDX_Text(pDX, IDC_EDIT_IP, m_IP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_UserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);
}


BEGIN_MESSAGE_MAP(CDlgServerManage, CDialog)
	ON_BN_CLICKED(IDC_ADD_SERVER, &CDlgServerManage::OnBnClickedAddServer)
	ON_BN_CLICKED(IDC_DEL_SERVER, &CDlgServerManage::OnBnClickedDelServer)
	ON_BN_CLICKED(IDC_EDIT_SERVER, &CDlgServerManage::OnBnClickedEditServer)
	ON_BN_CLICKED(IDOK, &CDlgServerManage::OnBnClickedOk)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDlgServerManage::OnHdnItemdblclickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CDlgServerManage::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDlgServerManage 消息处理程序

void CDlgServerManage::OnBnClickedAddServer()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_IP.Trim();
	if(!m_IP.IsEmpty())
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->AddServerConnection(m_IP, m_Port, m_UserName, m_Password);
		if (pConnection)
		{
			int Item = m_lvServerList.InsertItem(m_lvServerList.GetItemCount(), pConnection->GetServerAddress());
			CString Temp;
			Temp.Format(_T("%u"), pConnection->GetServerPort());
			m_lvServerList.SetItemText(Item, 1, Temp);
			m_lvServerList.SetItemText(Item, 2, _T("未连接"));
			m_lvServerList.SetItemData(Item, pConnection->GetID());
		}
		else
		{
			AfxMessageBox(_T("添加服务器地址失败"));
		}
	}
	else
	{
		AfxMessageBox(_T("请输入服务器地址"));
	}
}

void CDlgServerManage::OnBnClickedDelServer()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION Pos=m_lvServerList.GetFirstSelectedItemPosition();
	while(Pos)
	{
		int Item=m_lvServerList.GetNextSelectedItem(Pos);
		UINT ConnectionID = m_lvServerList.GetItemData(Item);
		if (CServerManagerClientApp::GetInstance()->DeleteServerConnection(ConnectionID))
		{			
			m_lvServerList.DeleteItem(Item);
		}
		else
		{
			AfxMessageBox(_T("删除服务器失败"));
		}
	}
}

void CDlgServerManage::OnBnClickedEditServer()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_IP.Trim();
	if(!m_IP.IsEmpty())
	{
		POSITION Pos=m_lvServerList.GetFirstSelectedItemPosition();
		if(Pos)
		{
			int Item=m_lvServerList.GetNextSelectedItem(Pos);
			UINT ConnectionID = m_lvServerList.GetItemData(Item);
			CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ConnectionID);
			if (pConnection)
			{
				if (pConnection->Reconnection(m_IP, m_Port, m_UserName, m_Password))
				{
					m_lvServerList.SetItemText(Item, 0, m_IP);
					CString Temp;
					Temp.Format(_T("%u"), m_Port);
					m_lvServerList.SetItemText(Item, 1, Temp);
				}
				else
				{
					AfxMessageBox(_T("编辑服务器地址失败"));
				}
			}
		}
	}
	else
	{
		AfxMessageBox(_T("请输入服务器地址"));
	}
}

void CDlgServerManage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CDlgServerManage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lvServerList.InsertColumn(0,_T("地址"),LVCFMT_LEFT,150);
	m_lvServerList.InsertColumn(1,_T("Port"),LVCFMT_LEFT,50);
	m_lvServerList.InsertColumn(2,_T("状态"),LVCFMT_LEFT,50);

	m_lvServerList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	// TODO:  在此添加额外的初始化
	void * Pos = CServerManagerClientApp::GetInstance()->GetFirstServerConnectionPos();
	while (Pos)
	{
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetNextServerConnection(Pos);
		int Item = m_lvServerList.InsertItem(m_lvServerList.GetItemCount(), pConnection->GetServerAddress());
		CString Temp;
		Temp.Format(_T("%u"), pConnection->GetServerPort());
		m_lvServerList.SetItemText(Item, 1, Temp);
		if (pConnection->IsConnected())
			m_lvServerList.SetItemText(Item, 2, _T("已连接"));
		else if (pConnection->IsDisconnected())
			m_lvServerList.SetItemText(Item, 2, _T("连接断开"));
		else
			m_lvServerList.SetItemText(Item, 2, _T("连接中"));		
		m_lvServerList.SetItemData(Item, pConnection->GetID());
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CDlgServerManage::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//
//
//	m_IP=m_lvServerList.GetItemText(phdr->iItem,0);
//	CString Temp=m_lvServerList.GetItemText(phdr->iItem,1);
//	m_Port=atoi(Temp);
//	UpdateData(false);
//}

void CDlgServerManage::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	POSITION Pos=m_lvServerList.GetFirstSelectedItemPosition();
	if(Pos)
	{
		int Item=m_lvServerList.GetNextSelectedItem(Pos);
		UINT ConnectionID = m_lvServerList.GetItemData(Item);
		CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(ConnectionID);
		if (pConnection)
		{
			m_IP = pConnection->GetServerAddress();
			m_Port = pConnection->GetServerPort();
			m_UserName = pConnection->GetUserName();
			m_Password = pConnection->GetPassword();
			UpdateData(false);
		}
	}
}
