#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgServiceEditor 对话框

class CDlgServiceEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgServiceEditor)
protected:
	CServerConnection *		m_pConnection;
	CServiceInfo			m_ServiceInfo;
	CComboBox				m_cbServiceType;
	bool					m_IsAddNew;
public:
	CDlgServiceEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgServiceEditor();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERVICE_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool Init(CServerConnection * pConnection, CServiceInfo * pServiceInfo);
	
	virtual BOOL OnInitDialog();
	void FillList();
	afx_msg void OnBnClickedOk();
	CString m_ExecFile;
	CListCtrl m_lvList;
	afx_msg void OnBnClickedButtonAddExecFile();
	afx_msg void OnBnClickedButtonDelExecFile();
};
