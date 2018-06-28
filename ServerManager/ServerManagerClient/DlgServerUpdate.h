#pragma once
#include "afxcmn.h"


// CDlgServerUpdate 对话框

class CDlgServerUpdate : public CDialog
{
	DECLARE_DYNAMIC(CDlgServerUpdate)
protected:
	enum UPDTAE_TYPE
	{
		UPDTAE_TYPE_NONE,
		UPDTAE_TYPE_EXEC,
		UPDTAE_TYPE_CONFIG,
	};
	struct UPDATE_FILE_INFO
	{
		int			UpdateType;
		CEasyString SrcPath;
		CEasyString DestPath;
	};
	struct UPDATE_INFO
	{
		CEasyString						ServiceName;
		CEasyArray<UPDATE_FILE_INFO>	UpdateFileList;
	};
	struct UPDATE_SERVICE_INFO
	{
		UINT			ConnectionID;
		UINT			ServiceID;
		CEasyString		ServiceName;
		CEasyString		ServerAddress;
	};

	CEasyArray<UPDATE_INFO>				m_UpdateList;
	CEasyArray<UPDATE_SERVICE_INFO>		m_ServiceList;
	CListCtrl							m_lvList;
public:
	CDlgServerUpdate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgServerUpdate();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERVER_UPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	UPDATE_INFO * GetUpdateInfo(LPCTSTR szServiceName, bool NewOnNotExist);
public:
	virtual BOOL OnInitDialog();
	bool LoadUpdateList(LPCTSTR szFileName);
	void FillList();
	
	afx_msg void OnBnClickedButtonLoadUpdateList();
	
	afx_msg void OnBnClickedButtonUpdateExec();
	afx_msg void OnBnClickedButtonUpdateConfig();
};
