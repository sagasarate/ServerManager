/****************************************************************************/
/*                                                                          */
/*      文件名:    DlgWorkDirBowser.cpp                                     */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DlgWorkDirBowser.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgWorkDirBowser.h"



#define FILE_ATTRIBUTE_DRIVER              0x10000000  

// CDlgWorkDirBowser 对话框

IMPLEMENT_DYNAMIC(CDlgWorkDirBowser, CDialog)

CDlgWorkDirBowser::CDlgWorkDirBowser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWorkDirBowser::IDD, pParent)
{
	m_ServiceID=0;
	m_pView=NULL;
	m_IsInEdit=false;
}

CDlgWorkDirBowser::~CDlgWorkDirBowser()
{
}

void CDlgWorkDirBowser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILES, m_lvFileList);
	DDX_Control(pDX, IDC_LIST_FILES_LOCAL, m_lvLocalFileList);
}


BEGIN_MESSAGE_MAP(CDlgWorkDirBowser, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILES, &CDlgWorkDirBowser::OnNMDblclkListFiles)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILES, &CDlgWorkDirBowser::OnLvnColumnclickListFiles)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILES_LOCAL, &CDlgWorkDirBowser::OnLvnColumnclickListFilesLocal)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILES_LOCAL, &CDlgWorkDirBowser::OnNMDblclkListFilesLocal)
	ON_BN_CLICKED(IDC_UPLOAD, &CDlgWorkDirBowser::OnBnClickedUpload)
	ON_BN_CLICKED(IDC_DOWNLOAD, &CDlgWorkDirBowser::OnBnClickedDownload)
	ON_COMMAND(ID_SERVER_CREATE_DIR, &CDlgWorkDirBowser::OnServerCreateDir)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILES, &CDlgWorkDirBowser::OnNMRClickListFiles)
	ON_COMMAND(ID_SERVER_EDIT_FILE, &CDlgWorkDirBowser::OnServerEditFile)
	ON_COMMAND(ID_SERVER_RUN_FILE, &CDlgWorkDirBowser::OnServerRunFile)
	ON_COMMAND(ID_SERVER_DELETE_FILE, &CDlgWorkDirBowser::OnServerDeleteFile)
	ON_COMMAND(ID_SERVER_REFRESH, &CDlgWorkDirBowser::OnServerRefresh)
	ON_COMMAND(ID_LOCAL_REFRESH, &CDlgWorkDirBowser::OnLocalRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILES_LOCAL, &CDlgWorkDirBowser::OnNMRClickListFilesLocal)
	ON_COMMAND(ID_SERVER_RUN_SCRIPT, &CDlgWorkDirBowser::OnServerRunScript)
	ON_BN_CLICKED(IDC_RETURN_UP, &CDlgWorkDirBowser::OnBnClickedReturnUp)
	ON_COMMAND(ID_SERVER_SET_TO_EXEC, &CDlgWorkDirBowser::OnServerSetToExec)
END_MESSAGE_MAP()


// CDlgWorkDirBowser 消息处理程序



BOOL CDlgWorkDirBowser::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化



	GetSHIcon(_T(""), true, true);
	GetSHIcon(_T(""), true, false);

	m_lvFileList.SetImageList(&m_SmallIcons,LVSIL_SMALL);
	m_lvFileList.SetImageList(&m_LargeIcons,LVSIL_NORMAL);

	m_lvFileList.SetExtendedStyle(m_lvFileList.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	m_lvFileList.InsertColumn(0,_T("文件名"),LVCFMT_LEFT,200);
	m_lvFileList.InsertColumn(1, _T("类型"), LVCFMT_LEFT, 60);
	m_lvFileList.InsertColumn(2,_T("大小"),LVCFMT_RIGHT,100);
	m_lvFileList.InsertColumn(3, _T("可执行"), LVCFMT_RIGHT, 60);
	m_lvFileList.InsertColumn(4, _T("修改日期"), LVCFMT_LEFT, 200);
	m_lvFileList.InsertColumn(5, _T("创建日期"), LVCFMT_LEFT, 200);
	


	m_lvLocalFileList.SetImageList(&m_SmallIcons,LVSIL_SMALL);
	m_lvLocalFileList.SetImageList(&m_LargeIcons,LVSIL_NORMAL);

	m_lvLocalFileList.SetExtendedStyle(m_lvLocalFileList.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	m_lvLocalFileList.InsertColumn(0,_T("文件名"),LVCFMT_LEFT,200);
	m_lvLocalFileList.InsertColumn(1, _T("类型"), LVCFMT_LEFT, 60);
	m_lvLocalFileList.InsertColumn(2,_T("大小"),LVCFMT_RIGHT,100);
	m_lvLocalFileList.InsertColumn(3, _T("修改日期"), LVCFMT_LEFT, 200);
	m_lvLocalFileList.InsertColumn(4, _T("创建日期"), LVCFMT_LEFT, 200);
	

	BrowseLocal(_T(".\\"));

	m_RefreshTimer.SaveTime();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int CDlgWorkDirBowser::GetSHIcon(LPCTSTR FileType, bool IsDir, bool Large)
{
	HIMAGELIST hSysImageList;
	SHFILEINFO FileInfo;	

	DWORD Attr = 0;
	if (IsDir)
		Attr = FILE_ATTRIBUTE_DIRECTORY;
	if(Large)
	{
		hSysImageList = (HIMAGELIST)SHGetFileInfo(FileType, Attr, &FileInfo,
			sizeof(FileInfo),SHGFI_USEFILEATTRIBUTES|SHGFI_SYSICONINDEX|SHGFI_LARGEICON);
		if(m_LargeIcons.m_hImageList==NULL)
			m_LargeIcons.Attach(hSysImageList);
	}
	else
	{
		hSysImageList = (HIMAGELIST)SHGetFileInfo(FileType, Attr, &FileInfo,
			sizeof(FileInfo),SHGFI_USEFILEATTRIBUTES|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
		if(m_SmallIcons.m_hImageList==NULL)
			m_SmallIcons.Attach(hSysImageList);
	}	
	return FileInfo.iIcon;
}


void CDlgWorkDirBowser::Browse(LPCTSTR szDir)
{
	m_lvFileList.DeleteAllItems();
	CServerManagerClientApp::GetInstance()->BrowseWorkDir(m_ConnectionID,m_ServiceID,szDir);
}

void CDlgWorkDirBowser::BrowseLocal(LPCTSTR szDir)
{
	CFileSearcher FileSearcher;
	int Item=0;
	bool IsInDirectory=false;

	CEasyString SearchFolder=szDir;
	SearchFolder=CFileTools::MakeFullPath(SearchFolder+_T("\\*.*"));

	m_CurLocalDir = CFileTools::GetPathDirectory(SearchFolder);

	m_lvLocalFileList.DeleteAllItems();	

	if(FileSearcher.FindFirst(SearchFolder))
	{
		while(FileSearcher.FindNext())
		{
			int Icon;
			CEasyString Temp;
			
			Icon = GetSHIcon(FileSearcher.GetFileName(), FileSearcher.IsDirectory(), false);
			Item=m_lvLocalFileList.InsertItem(Item,FileSearcher.GetFileName(),Icon);
			m_lvLocalFileList.SetItemText(Item, 1, FileSearcher.GetFileExt());
			if(!FileSearcher.IsDirectory())
			{
				m_lvLocalFileList.SetItemText(Item,2,FormatNumberWords(FileSearcher.GetFileSize()));
			}
			FileSearcher.GetLastWriteTime().Format(Temp, _T("%Y-%m-%d %H:%M:%S"));
			m_lvLocalFileList.SetItemText(Item, 3, Temp);
			FileSearcher.GetCreationTime().Format(Temp,_T("%Y-%m-%d %H:%M:%S"));
			m_lvLocalFileList.SetItemText(Item, 4, Temp);
			if (FileSearcher.IsDirectory())
				m_lvLocalFileList.SetItemData(Item, FILE_ATTRIBUTE_FLAG_DIRECTORY);
			else
				m_lvLocalFileList.SetItemData(Item, 0);
			if(FileSearcher.IsDots())
				IsInDirectory=true;
			Item++;
		}
	}

	if(!IsInDirectory)
	{
		FileSearcher.FetchLogicalDrive();
		for(int i=0;i<FileSearcher.GetLogicalDriveCount();i++)
		{
			int Icon;
			Icon=GetSHIcon(FileSearcher.GetLogicDriveName(i),true,false);
			Item=m_lvLocalFileList.InsertItem(Item,FileSearcher.GetLogicDriveName(i),Icon);
			m_lvLocalFileList.SetItemData(Item,0xFFFFFFFF);
			Item++;			
		}
	}
	SortLocalFileList(0, false);;
	for (int i = 0; i < m_lvLocalFileList.GetHeaderCtrl()->GetItemCount(); i++)
	{
		m_lvLocalFileList.SetColumnWidth(i, LVSCW_AUTOSIZE);
	}
}

void CDlgWorkDirBowser::OnBrowseResult(LPCTSTR szDir, LPCTSTR szFileName, UINT FileAttr, ULONG64 FileSize, time_t CreateTime, time_t LastAccessTime, time_t LastWriteTime)
{
	if(m_CurDir!=szDir)
		m_CurDir=szDir;

	int Icon;
	CEasyString Temp;
	CEasyTime Time;
	int Item=m_lvFileList.GetItemCount();
	Icon = GetSHIcon(szFileName, (FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) != 0, false);
	Item=m_lvFileList.InsertItem(Item,szFileName,Icon);
	m_lvFileList.SetItemText(Item, 1, CFileTools::GetPathFileExtName(szFileName));
	if ((FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
	{
		m_lvFileList.SetItemText(Item,2,FormatNumberWords(FileSize));
	}
	m_lvFileList.SetItemText(Item, 3, (FileAttr&FILE_ATTRIBUTE_FLAG_CAN_EXEC) ? _T("是") : _T("否"));
	Time = LastWriteTime;
	Time.Format(Temp, _T("%Y-%m-%d %H:%M:%S"));
	m_lvFileList.SetItemText(Item, 4, Temp);
	Time=CreateTime;
	Time.Format(Temp,_T("%Y-%m-%d %H:%M:%S"));
	m_lvFileList.SetItemText(Item, 5, Temp);
	
	m_lvFileList.SetItemData(Item,(DWORD_PTR)FileAttr);
}

void CDlgWorkDirBowser::OnBrowseFinish()
{
	SortFileList(0, false);
	for (int i = 0; i < m_lvFileList.GetHeaderCtrl()->GetItemCount(); i++)
	{
		m_lvFileList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}


void CDlgWorkDirBowser::OnDownloadFinish(LPCTSTR szSourcePath, LPCTSTR szTargetPath)
{
	//Refresh();
	if (m_IsInEdit&&m_CurEditFileLocalPath.CompareNoCase(szTargetPath) == 0)
	{
		DoEdit();
	}
}
void CDlgWorkDirBowser::OnUploadFinish(LPCTSTR szSourcePath, LPCTSTR szTargetPath)
{
	//Refresh();
}
void CDlgWorkDirBowser::OnDeleteFileFinish(LPCTSTR szSourcePath)
{
	//Refresh();
}
void CDlgWorkDirBowser::OnCreateDirFinish(LPCTSTR szSourcePath)
{
	//Refresh();
}

void CDlgWorkDirBowser::OnFileTaskEmpty()
{
	Refresh();
}

void CDlgWorkDirBowser::OnNMDblclkListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

	if(Pos)
	{
		int Item=m_lvFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
		CString FileName=m_lvFileList.GetItemText(Item,0);
		if (FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY)
		{
			Browse(m_CurDir + DIR_SLASH + (LPCTSTR)FileName);
		}
	}
}

int CDlgWorkDirBowser::ItemSortCallBack(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	SORT_PARAM * pSortParam=(SORT_PARAM *)lParamSort;

	int Order=1;
	if(pSortParam->SortOrder==2)
	{
		Order=-1;
	}

	DWORD FileAttr1 = (DWORD)pSortParam->pList->GetItemData(lParam1)&(FILE_ATTRIBUTE_FLAG_DIRECTORY);
	DWORD FileAttr2 = (DWORD)pSortParam->pList->GetItemData(lParam2)&(FILE_ATTRIBUTE_FLAG_DIRECTORY);

	CString Text1=pSortParam->pList->GetItemText(lParam1,pSortParam->SortCol);
	CString Text2=pSortParam->pList->GetItemText(lParam2,pSortParam->SortCol);


	if(FileAttr1==FileAttr2)
	{
		if(pSortParam->SortOrder==1)
			return _tcsicmp(Text1,Text2);
		else
			return _tcsicmp(Text2, Text1);
	}
	else
	{
		return FileAttr1<FileAttr2;
	}	
}
void CDlgWorkDirBowser::OnLvnColumnclickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SortFileList(pNMLV->iSubItem, true);
	
}

void CDlgWorkDirBowser::SortFileList(int ColIndex, bool SwitchType)
{
	LVCOLUMN ColInfo;
	ColInfo.mask = LVCF_IMAGE;
	m_lvFileList.GetColumn(ColIndex, &ColInfo);

	if (SwitchType || (ColInfo.iImage != 1 && ColInfo.iImage != 2))
	{
		if (ColInfo.iImage == 1)
			ColInfo.iImage = 2;
		else
			ColInfo.iImage = 1;
	}

	m_lvFileList.SetColumn(ColIndex, &ColInfo);
	SORT_PARAM Param;
	Param.pList = &m_lvFileList;
	Param.SortCol = ColIndex;
	Param.SortOrder = ColInfo.iImage;

	m_lvFileList.SortItemsEx(ItemSortCallBack, (DWORD_PTR)&Param);
}

void CDlgWorkDirBowser::OnLvnColumnclickListFilesLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SortLocalFileList(pNMLV->iSubItem, true);;
	
}
void CDlgWorkDirBowser::SortLocalFileList(int ColIndex, bool SwitchType)
{
	LVCOLUMN ColInfo;
	ColInfo.mask = LVCF_IMAGE;
	m_lvLocalFileList.GetColumn(ColIndex, &ColInfo);

	if (SwitchType || (ColInfo.iImage != 1 && ColInfo.iImage != 2))
	{
		if (ColInfo.iImage == 1)
			ColInfo.iImage = 2;
		else
			ColInfo.iImage = 1;
	}

	m_lvLocalFileList.SetColumn(ColIndex, &ColInfo);
	SORT_PARAM Param;
	Param.pList = &m_lvLocalFileList;
	Param.SortCol = ColIndex;
	Param.SortOrder = ColInfo.iImage;

	m_lvLocalFileList.SortItemsEx(ItemSortCallBack, (DWORD_PTR)&Param);
}
void CDlgWorkDirBowser::OnNMDblclkListFilesLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	POSITION Pos=m_lvLocalFileList.GetFirstSelectedItemPosition();

	if(Pos)
	{
		int Item=m_lvLocalFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvLocalFileList.GetItemData(Item);
		CString FileName=m_lvLocalFileList.GetItemText(Item,0);
		if(FileAttr==0xFFFFFFFF)
		{
			BrowseLocal((LPCTSTR)FileName);
		}
		else if (FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY)
		{			
			BrowseLocal(m_CurLocalDir + DIR_SLASH + (LPCTSTR)FileName);
		}
	}
}

void CDlgWorkDirBowser::OnBnClickedUpload()
{
	// TODO: 在此添加控件通知处理程序代码	

	CDlgListSelector Dlg;
	CEasyArray<CEasyString>	FileList;

	POSITION Pos=m_lvLocalFileList.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvLocalFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvLocalFileList.GetItemData(Item);
		CString FileName=m_lvLocalFileList.GetItemText(Item,0);
		if ((FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
		{			
			bool IsExist=false;
			for(int i=0;i<m_lvFileList.GetItemCount();i++)
			{					
				DWORD ServerFileAttr=(DWORD)m_lvFileList.GetItemData(i);
				CString ServerFileName=m_lvFileList.GetItemText(i,0);
				if ((ServerFileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
				{
					if(FileName.CompareNoCase(ServerFileName)==0)
					{
						IsExist=true;
						break;
					}
				}
			}
			if (IsExist)
			{
				SELECT_ITEM_INFO * pInfo = Dlg.m_ItemList.AddEmpty();
				pInfo->Item = FileName;
				pInfo->IsSelected = true;
			}
			else
			{
				FileList.Add((LPCTSTR)FileName);
			}			
		}
	}	


	if (Dlg.m_ItemList.GetCount())
	{
		Dlg.m_Title = _T("请选择要覆盖的文件");
		if (Dlg.DoModal() == IDOK)
		{
			for (UINT i = 0; i < Dlg.m_ItemList.GetCount(); i++)
			{
				if (Dlg.m_ItemList[i].IsSelected)
				{
					FileList.Add(Dlg.m_ItemList[i].Item);
				}
			}
			for (UINT i = 0; i < FileList.GetCount(); i++)
			{
				CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
					AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_UPLOAD, m_ConnectionID, m_ServiceID,
					CFileTools::MakeFullPath(m_CurLocalDir + DIR_SLASH + FileList[i]), m_CurDir + DIR_SLASH + FileList[i], 0, false);
			}
		}
	}
	else
	{
		for (UINT i = 0; i < FileList.GetCount(); i++)
		{
			CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
				AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_UPLOAD, m_ConnectionID, m_ServiceID,
				CFileTools::MakeFullPath(m_CurLocalDir + DIR_SLASH + FileList[i]), m_CurDir + DIR_SLASH + FileList[i], 0, false);
		}
	}
}

void CDlgWorkDirBowser::OnBnClickedDownload()
{
	// TODO: 在此添加控件通知处理程序代码	

	CDlgListSelector Dlg;
	CEasyArray<CEasyString>	FileList;

	POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
		CString FileName=m_lvFileList.GetItemText(Item,0);
		if ((FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
		{			
			bool IsExist=false;
			for(int i=0;i<m_lvLocalFileList.GetItemCount();i++)
			{					
				DWORD LocalFileAttr=(DWORD)m_lvLocalFileList.GetItemData(i);
				CString ServerFileName=m_lvLocalFileList.GetItemText(i,0);
				if ((LocalFileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
				{
					if(FileName.CompareNoCase(ServerFileName)==0)
					{
						IsExist=true;
						break;
					}
				}
			}
			if(IsExist)
			{
				SELECT_ITEM_INFO * pInfo = Dlg.m_ItemList.AddEmpty();
				pInfo->Item = FileName;
				pInfo->IsSelected = true;
			}
			else
			{
				FileList.Add((LPCTSTR)FileName);
			}
			
		}
	}	

	if (Dlg.m_ItemList.GetCount())
	{
		Dlg.m_Title = _T("请选择要覆盖的文件");
		if (Dlg.DoModal() == IDOK)
		{
			for (UINT i = 0; i < Dlg.m_ItemList.GetCount(); i++)
			{
				if (Dlg.m_ItemList[i].IsSelected)
				{
					FileList.Add(Dlg.m_ItemList[i].Item);
				}
			}
			for (UINT i = 0; i < FileList.GetCount(); i++)
			{
				CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
					AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_DOWNLOAD, m_ConnectionID, m_ServiceID,
					m_CurDir + DIR_SLASH + FileList[i], CFileTools::MakeFullPath(m_CurLocalDir + DIR_SLASH + FileList[i]), 0, false);
			}
		}
	}
	else
	{
		for (UINT i = 0; i < FileList.GetCount(); i++)
		{
			CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
				AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_DOWNLOAD, m_ConnectionID, m_ServiceID,
				m_CurDir + DIR_SLASH + FileList[i], CFileTools::MakeFullPath(m_CurLocalDir + DIR_SLASH + FileList[i]), 0, false);
		}
	}
}



void CDlgWorkDirBowser::Refresh()
{
	Browse(m_CurDir);
	BrowseLocal(m_CurLocalDir);
}


void CDlgWorkDirBowser::OnNMRClickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	CMenu Menu;
	VERIFY(Menu.LoadMenu(IDR_TRANSFER_SERVER_LIST_POPUP));

	CMenu* pPopup = Menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();


	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePoint.x, MousePoint.y,
		pWndPopupOwner);
}

void CDlgWorkDirBowser::OnNMRClickListFilesLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	CMenu Menu;
	VERIFY(Menu.LoadMenu(IDR_TRANSFER_LOCAL_LIST_POPUP));

	CMenu* pPopup = Menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();


	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePoint.x, MousePoint.y,
		pWndPopupOwner);
}


void CDlgWorkDirBowser::OnServerCreateDir()
{
	// TODO: 在此添加命令处理程序代码
	
	CDlgInput Dlg;

	Dlg.m_Title=_T("请输入目录名");

	if(Dlg.DoModal()==IDOK)
	{
		CEasyString Dir = m_CurDir + DIR_SLASH + (LPCTSTR)Dlg.m_InputText;
		CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
			AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_CREATE_DIR, m_ConnectionID, m_ServiceID,
			Dir, _T(""), 0, false);
	}
	
}
void CDlgWorkDirBowser::OnServerEditFile()
{
	// TODO: 在此添加命令处理程序代码

	POSITION Pos = m_lvFileList.GetFirstSelectedItemPosition();

	if (Pos)
	{
		int Item = m_lvFileList.GetNextSelectedItem(Pos);
		DWORD FileAttr = (DWORD)m_lvFileList.GetItemData(Item);
		CString FileName = m_lvFileList.GetItemText(Item, 0);
		CString FileSize = m_lvFileList.GetItemText(Item, 2);
		if (FileSize.Find('M') >= 0)
		{
			if (AfxMessageBox(_T("文件过大,是否要继续编辑"), MB_YESNO) == IDNO)
				return;
		}
		if ((FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
		{
			TCHAR TempDir[MAX_PATH];
			if (GetTempPath(MAX_PATH, TempDir))
			{
				TCHAR TempFileName[MAX_PATH];
				if (GetTempFileName(TempDir, FileName, 0, TempFileName))
				{
					m_CurEditFileLocalPath = TempFileName;
					m_CurEditFilePath = m_CurDir + DIR_SLASH + (LPCTSTR)FileName;
					m_IsInEdit = true;

					CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
						AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_DOWNLOAD, m_ConnectionID, m_ServiceID,
						m_CurEditFilePath, m_CurEditFileLocalPath, 0, false);
				}
				else
				{
					AfxMessageBox(_T("创建临时文件失败"));
				}
			}
			else
			{
				AfxMessageBox(_T("创建临时文件失败"));
			}
		}
	}
}

void CDlgWorkDirBowser::DoEdit()
{
	CStringFile TextFile;

	if (TextFile.LoadFile(m_CurEditFileLocalPath, false))
	{
		CDlgEditBox Dlg;
		Dlg.m_Text.SetString(TextFile.GetData(), TextFile.GetDataLen());
		if (Dlg.DoModal() == IDOK)
		{
			TextFile.LoadFromString(Dlg.m_Text, -1, false);
			if (TextFile.SaveToFile(m_CurEditFileLocalPath))
			{
				CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
					AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_UPLOAD, m_ConnectionID, m_ServiceID,
					m_CurEditFileLocalPath, m_CurEditFilePath, 0, false);
			}
			else
			{
				AfxMessageBoxEx(MB_OK, 0, _T("保存文件失败%s"), m_CurEditFileLocalPath);
			}
		}
	}
	else
	{
		AfxMessageBoxEx(MB_OK, 0, _T("打开文件失败%s"), m_CurEditFileLocalPath);
	}
}


void CDlgWorkDirBowser::OnServerSetToExec()
{
	// TODO:  在此添加命令处理程序代码
	CServerConnection * pConnection = CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
	if (pConnection)
	{

		POSITION Pos = m_lvFileList.GetFirstSelectedItemPosition();

		if (Pos)
		{
			int Item = m_lvFileList.GetNextSelectedItem(Pos);
			DWORD FileAttr = (DWORD)m_lvFileList.GetItemData(Item);
			CString FileName = m_lvFileList.GetItemText(Item, 0);
			if ((FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
			{
				CEasyString FilePath = m_CurDir + DIR_SLASH + (LPCTSTR)FileName;
				pConnection->QueryChangeFileMode(m_ServiceID, FilePath, 0x777);
			}
		}
	}
	else
	{
		AfxMessageBox(_T("服务器连接不存在"));
	}
}
void CDlgWorkDirBowser::OnServerRunFile()
{
	// TODO: 在此添加命令处理程序代码
	CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
	if(pConnection)
	{

		POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

		if(Pos)
		{
			int Item=m_lvFileList.GetNextSelectedItem(Pos);		
			DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
			CString FileName=m_lvFileList.GetItemText(Item,0);			
			if ((FileAttr&FILE_ATTRIBUTE_FLAG_DIRECTORY) == 0)
			{	
				CDlgInput Dlg;

				Dlg.m_Title=_T("请输入运行参数");

				if(Dlg.DoModal()==IDOK)
				{
					CEasyString FilePath = m_CurDir + DIR_SLASH + (LPCTSTR)FileName;

					pConnection->QueryCreateProcess(m_ServiceID,FilePath,m_CurDir,Dlg.m_InputText);					
				}
				
			}
		}
	}
	else
	{
		AfxMessageBox(_T("服务器连接不存在"));		
	}
}

void CDlgWorkDirBowser::OnServerDeleteFile()
{
	// TODO: 在此添加命令处理程序代码
	
	CDlgListSelector Dlg;

	POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

	while(Pos)
	{
		int Item=m_lvFileList.GetNextSelectedItem(Pos);		
		DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
		CString FileName=m_lvFileList.GetItemText(Item,0);	

		SELECT_ITEM_INFO * pInfo = Dlg.m_ItemList.AddEmpty();
		pInfo->Item = FileName;
		pInfo->IsSelected = true;
	}
	if (Dlg.m_ItemList.GetCount())
	{
		Dlg.m_Title = _T("请选择要删除的文件");
		if (Dlg.DoModal() == IDOK)
		{
			for (UINT i = 0; i < Dlg.m_ItemList.GetCount(); i++)
			{
				if (Dlg.m_ItemList[i].IsSelected)
				{
					CEasyString FilePath = m_CurDir + DIR_SLASH + Dlg.m_ItemList[i].Item;

					CServerManagerClientApp::GetInstance()->GetFileTransferQueue().
						AddTask(CFileTransferQueue::FILE_TRANSFER_TYPE_DELETE, m_ConnectionID, m_ServiceID,
						FilePath, _T(""), 0, false);
				}
			}
		}
	}
}

void CDlgWorkDirBowser::OnServerRefresh()
{
	// TODO: 在此添加命令处理程序代码
	Browse(m_CurDir);
}

void CDlgWorkDirBowser::OnLocalRefresh()
{
	// TODO: 在此添加命令处理程序代码
	BrowseLocal(m_CurLocalDir);
}



void CDlgWorkDirBowser::OnServerRunScript()
{
	// TODO: 在此添加命令处理程序代码
	CServerConnection * pConnection=CServerManagerClientApp::GetInstance()->GetServerConnection(m_ConnectionID);
	if(pConnection)
	{

		POSITION Pos=m_lvFileList.GetFirstSelectedItemPosition();

		if(Pos)
		{
			int Item=m_lvFileList.GetNextSelectedItem(Pos);		
			DWORD FileAttr=(DWORD)m_lvFileList.GetItemData(Item);
			CString FileName=m_lvFileList.GetItemText(Item,0);	

			CEasyString PromptText;
			PromptText.Format(_T("是否要执行文件[%s]？"),FileName);

			int Ret=AfxMessageBox(PromptText,MB_YESNOCANCEL);
			if(Ret==IDYES)
			{
				CEasyString FilePath = m_CurDir + DIR_SLASH + (LPCTSTR)FileName;

				pConnection->QueryRunScript(m_ServiceID,FilePath);
			}				
		}
	}
	else
	{
		AfxMessageBox(_T("服务器连接不存在"));		
	}
}


void CDlgWorkDirBowser::OnBnClickedReturnUp()
{
	// TODO:  在此添加控件通知处理程序代码
	int Pos1 = m_CurDir.ReverseFind('\\');
	int Pos2 = m_CurDir.ReverseFind('/');
	int Pos = -1;
	if (Pos1 < 0)
		Pos = Pos2;
	else if (Pos2 < 0)
		Pos = Pos1;
	else
		Pos = max(Pos1, Pos2);

	m_CurDir = m_CurDir.Left(Pos);
	Browse(m_CurDir);
}




