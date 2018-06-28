/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerClientView.h                                */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ServerManagerClientView.h : CServerManagerClientView 类的接口
//


#pragma once
#include "afxcmn.h"


class CServerManagerClientDoc;

class CServerManagerClientView : public CFormView
{
protected: // 仅从序列化创建

	struct SORT_PARAM
	{
		CListCtrl *		pListCtrl;
		int				SortColumn;
	};

	CDialogItemSorter		m_DialogItemSorter;
	SORT_PARAM				m_SortParam;
	
	HTREEITEM				m_hAllServer;

	CDlgWorkDirBowser		m_DlgWorkDirBowser;
	CDlgServiceEditor		m_DlgServiceEditor;
	CDlgServerStatus		m_DlgServerStatus;
	CDlgServerConsole		m_DlgServerConsole;
	CTreeCtrl				m_tvServers;
	CListCtrl				m_lvServiceInfos;
	CListCtrl				m_lvTask;
	CEdit					m_edLog;

	bool					m_ShowHideService;
	UINT					m_SelectedConnectionID;


	CServerManagerClientView();
	DECLARE_DYNCREATE(CServerManagerClientView)

public:
	enum{ IDD = IDD_SERVERMANAGERCLIENT_FORM };

// 属性
public:
	CServerManagerClientDoc* GetDocument() const;

// 操作
public:
	void RefreshConnection();
	void PrintLogVL(LPCTSTR szFormat,va_list vl);
	void PrintLog(LPCTSTR szFormat,...);
	void SetServiceInfo(CServerConnection * pConnection, const CServiceInfo& ServiceInfo);
	void SetInternetAdapterInfo(UINT ConnectionID, float SendFlux, float RecvFlux);
	void SetIntranetAdapterInfo(UINT ConnectionID, float SendFlux, float RecvFlux);
	void ConnectNotify(CServerConnection * pConnection);
	void DisconnectNotify(CServerConnection * pConnection);
	void OnAddTask(UINT ConID, CTaskQueue::TASK_INFO& TaskInfo);
	void OnDeleteTask(UINT ConID, UINT TaskID);
	void OnTaskUpdate(UINT ConID, UINT TaskID, float Progress);
	void OnDeleteAllTask(UINT ConID);
	CDlgServiceEditor * GetServiceEditor()
	{
		return &m_DlgServiceEditor;
	}
	CDlgWorkDirBowser * GetWorkDirBrowser()
	{
		return &m_DlgWorkDirBowser;
	}
	CDlgServerStatus * GetDlgServerStatus()
	{
		return &m_DlgServerStatus;
	}
	CDlgServerConsole * GetServerConsole()
	{
		return &m_DlgServerConsole;
	}
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CServerManagerClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	static int CALLBACK ItemSortCallBack(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnNMRClickServiceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStartupService();
	afx_msg void OnShutdownServiceSafe();
	afx_msg void OnShutdownServce();
	afx_msg void OnShutDownServiceForce();
	afx_msg void OnLvnColumnclickServiceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopupBrowseWorkDir();

	afx_msg void OnShowHideService();
	afx_msg void OnUpdateShowHideService(CCmdUI *pCmdUI);
	
	afx_msg void OnNMDblclkServiceList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTypeTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPopupAddService();
	afx_msg void OnPopupEditService();
	afx_msg void OnPopupDelService();
	afx_msg void OnPopupUploadBatch();
	
public:
	afx_msg void OnReloadConfigData();
	afx_msg void OnPopupOpenServerConsole();
	afx_msg void OnPopupOpenServerStatus();
};

#ifndef _DEBUG  // ServerManagerClientView.cpp 中的调试版本
inline CServerManagerClientDoc* CServerManagerClientView::GetDocument() const
   { return reinterpret_cast<CServerManagerClientDoc*>(m_pDocument); }
#endif

