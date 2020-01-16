/****************************************************************************/
/*                                                                          */
/*      文件名:    stdafx.h                                                 */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT


#define MAX_LOG_MSG_LEN				10240
#define CONNECT_WAIT_TIME			5000
#define CONNECT_TIME				60000
#define SERVICE_INFO_FETCH_TIME		5000
#define MIN_SERVER_DIR_REFRESH_TIME	3000
#define SETTING_FILE_NAME			_T("Setting.xml")
#define KEEP_ALIVE_TIME				(5000)
#define MAX_KEEP_ALIVE_COUNT		(10)
#define DIR_BROWSE_PAGE_LEN			50
#define MAX_MSG_SIZE				650000
#define FILE_TRANSFER_BLOCK_SIZE	300000
#define MAX_SERVER_STATUS_NAME_LEN	128

#define MAX_CONTROL_PANEL_MSG_LEN	5000
#define SERVICE_STARTUP_TIMEOUT		(5*60*1000)

#include "../../Libs/Utils/Utils.h"
#include "../../Libs/NetLib/NetLib.h"
#include "../../Libs/MFCExtensionLib/MFCExtensionLib.h"
//#include "../../Libs/zlib/zlib.h"
#include "../Protocol/Protocol.h"
//#include "../../Libs/ServerFrameWork/BaseProtocol.h"
#include "../../Libs/ServerFrameWork/ServerConsoleProtocol.h"



enum SERVER_CONNECTION_STATUS
{
	SCS_NO_CONNECTION,
	SCS_DISCONNECTED,
	SCS_CONNECTING,
};

enum LOG_TYPE
{
	LOG_TYPE_NORMAL,
	LOG_TYPE_ERROR,
};

//struct TRANSFER_FILE_INFO
//{
//	CEasyString	SourceFilePath;
//	CEasyString	TargetFilePath;
//};

struct SELECT_ITEM_INFO
{
	bool			IsSelected;
	CEasyString		Item;
	CEasyString		Param1;
};



#include "TaskQueue.h"
#include "ServerConnection.h"

#include "Resource.h"

#include "DlgInput.h"
#include "DlgEditBox.h"



#include "DlgServerManage.h"
#include "DlgWorkDirBowser.h"
#include "DlgListSelector.h"
#include "DlgServiceEditor.h"
#include "DlgServerStatus.h"
#include "DlgServerConsole.h"
#include "DlgServerUpdate.h"
#include "DlgLogin.h"

#include "ServerManagerClientView.h"
#include "ServerManagerClientDoc.h"
#include "MainFrm.h"

#include "ServerManagerClient.h"
#include <afxcontrolbars.h>


inline CServerManagerClientView * GetMainView()
{
	CMainFrame * pFrame = dynamic_cast<CMainFrame *>(AfxGetMainWnd());
	if (pFrame)
	{
		return dynamic_cast<CServerManagerClientView *>(pFrame->GetActiveView());
	}
	return NULL;
}

inline LPCTSTR GetResultStr(short Result)
{
	static TCHAR ResultStr[128];
	if (Result >= 0 && Result < MSG_RESULT_MAX)
	{
		return g_szMSG_RESULT[Result];
	}
	else
	{
		_tcprintf_s(ResultStr, 128, "%d", Result);
		return ResultStr;
	}
}

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


