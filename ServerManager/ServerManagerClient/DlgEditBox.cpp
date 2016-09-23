/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgEditBox.cpp                                           */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgEditBox.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgEditBox.h"




// CDlgEditBox 对话框

IMPLEMENT_DYNAMIC(CDlgEditBox, CDialog)

CDlgEditBox::CDlgEditBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditBox::IDD, pParent)
	, m_Text(_T(""))
{

}

CDlgEditBox::~CDlgEditBox()
{
}

void CDlgEditBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_Text);
}


BEGIN_MESSAGE_MAP(CDlgEditBox, CDialog)
END_MESSAGE_MAP()


// CDlgEditBox 消息处理程序

BOOL CDlgEditBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
