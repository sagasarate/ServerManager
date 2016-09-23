/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgServerManage.h                                        */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once
#include "afxcmn.h"


// CDlgServerManage 对话框

class CDlgServerManage : public CDialog
{
protected:
	CListCtrl					m_lvServerList;
	CString						m_IP;
	UINT						m_Port;


	DECLARE_DYNAMIC(CDlgServerManage)

public:
	CDlgServerManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgServerManage();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERVER_MANAGE };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	
	afx_msg void OnBnClickedAddServer();
	afx_msg void OnBnClickedDelServer();
	afx_msg void OnBnClickedEditServer();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
//	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
