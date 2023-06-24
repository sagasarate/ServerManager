#pragma once
#include "afxcmn.h"


// CDlgServerStatus 对话框

class CDlgServerStatus : public CDialog
{
	DECLARE_DYNAMIC(CDlgServerStatus)
protected:

	struct SERVER_STATUS_FORMAT_INFO
	{
		WORD		StatusID;
		CEasyString	Name;
		int			FormatType;
	};

	CListCtrl								m_lvList;
	CEasyArray<SERVER_STATUS_FORMAT_INFO>	m_FormatInfoList;
	UINT									m_ConnectionID;
	UINT									m_ServiceID;
	int										m_CharSet;

public:
	CDlgServerStatus(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgServerStatus();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERVER_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void Open(UINT ConnectionID, UINT ServiceID, LPCTSTR ServerAddress, LPCTSTR ServiceName, int CharSet);
	void Close();
	void SetStatusFormat(UINT ConnectionID, UINT ServiceID, const CSmartStruct& ServerStatusFormat);
	void FlushStatus(UINT ConnectionID, UINT ServiceID, const CSmartStruct& ServerStatus);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
