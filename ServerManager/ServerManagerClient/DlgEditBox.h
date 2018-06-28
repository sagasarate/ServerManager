/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgEditBox.h                                             */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


// CDlgEditBox 对话框
class CDlgWorkDirBowser;

class CDlgEditBox : public CDialog
{
protected:
	CDlgWorkDirBowser * m_pParent;
	CString m_Text;


	DECLARE_DYNAMIC(CDlgEditBox)
public:
	CDlgEditBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEditBox();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDIT_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();

	void StartEdit(CDlgWorkDirBowser * pParent, LPCTSTR Content);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
