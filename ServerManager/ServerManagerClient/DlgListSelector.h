#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgListSelector 对话框

class CDlgListSelector : public CDialog
{
	DECLARE_DYNAMIC(CDlgListSelector)
protected:
	CListCtrl m_lvList;
	CButton m_cbSelectAll;
	int m_PrevSelectAllState;
public:
	CString m_Title;
	CEasyArray<SELECT_ITEM_INFO>		m_ItemList;

	CDlgListSelector(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgListSelector();

// 对话框数据
	enum { IDD = IDD_DIALOG_LIST_SELECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();

	void UpdateCheckStatus();
	afx_msg void OnLvnItemchangedListFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckSelectAll();
};
