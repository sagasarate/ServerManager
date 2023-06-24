#pragma once
#include "afxcmn.h"


// CDlgServerUpdate 对话框

class CDlgServerUpdate : public CDialog
{
	DECLARE_DYNAMIC(CDlgServerUpdate)
protected:
	enum UPDTAE_TYPE
	{
		UPDTAE_TYPE_EXEC = 1,
		UPDTAE_TYPE_CONFIG = (1 << 1),
		UPDTAE_TYPE_ALL = 0xFFFF,
	};
	struct UPDATE_FILE_INFO
	{
		int			UpdateType;
		bool		IncludeChildDir;
		CEasyString	ServiceName;
		CEasyString SrcDir;
		CEasyString DestDir;
		CEasyArray<CEasyString> Files;
		CEasyArray<CEasyString> ExceptPatterns;
	};	
	struct UPDATE_SERVICE_INFO
	{
		UINT			ConnectionID;
		UINT			ServiceID;
		CEasyString		ServiceName;
		CEasyString		ServerAddress;
	};

	CEasyArray<UPDATE_FILE_INFO>		m_UpdateFileList;
	CEasyArray<UPDATE_SERVICE_INFO>		m_ServiceList;
	CListCtrl							m_lvList;
public:
	CDlgServerUpdate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgServerUpdate();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERVER_UPDATE };
	bool LoadUpdateList(LPCTSTR szFileName);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	LPCTSTR HaveUpdate(const CEasyString& ServiceName, UPDTAE_TYPE UpdateType);

	virtual BOOL OnInitDialog();
	
	void FillList();
	bool IsMatch(CEasyArray<CEasyString> Patterns, const CEasyString& FileName);
	void SearchFiles(LPCTSTR SearchDir, LPCTSTR FilePattern, CEasyArray<CEasyString> ExceptPatterns, LPCTSTR DestDir, CEasyArray<SELECT_ITEM_INFO>& FileList, bool Recursion);
	void GetFiles(const CEasyArray<UPDATE_FILE_INFO>& UpdateList, CEasyArray<SELECT_ITEM_INFO>& FileList, const CEasyString& ServiceName, UPDTAE_TYPE MatchType);
	
	afx_msg void OnBnClickedButtonLoadUpdateList();
	
	afx_msg void OnBnClickedButtonUpdateExec();
	afx_msg void OnBnClickedButtonUpdateConfig();
};
