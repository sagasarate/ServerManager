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
#define MAX_SERVER_STATUS_NAME_LEN	128

#define MAX_CONTROL_PANEL_MSG_LEN	5000
#define SERVICE_STARTUP_TIMEOUT		(5*60*1000)

#define MAX_DOWNLOAD_QUERY			5

#define LOG_CHANNEL_MAIN			76438

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

enum TASK_TYPE
{
	TASK_TYPE_NONE,
	TASK_TYPE_DOWNLOAD,
	TASK_TYPE_UPLOAD,
	TASK_TYPE_DELETE_FILE,
	TASK_TYPE_CREATE_DIR,
	TASK_TYPE_CHANGE_FILE_MODE,
	TASK_TYPE_STARTUP_SERVICE,
	TASK_TYPE_SHUTDOWN_SERVICE,
	TASK_TYPE_RELOAD_CONFIG_DATA,
	TASK_TYPE_COMPARE,
};
enum TASK_STATUS
{
	TASK_STATUS_NONE,
	TASK_STATUS_PROCESSING,
	TASK_STATUS_TRANSFER_DATA,
	TASK_STATUS_WAIT_SERVICE_STATUS,
	TASK_STATUS_END,
	TASK_STATUS_ERROR
};
enum WORK_STATUS
{
	WORK_STATUS_NONE,
	WORK_STATUS_LOAD,
	WORK_STATUS_SAVE,
	WORK_STATUS_MD5,
	WORK_STATUS_COMPRESS,
	WORK_STATUS_UNCOMPRESS,
};
enum TASK_USAGE
{
	TASK_USAGE_NONE,
	TASK_USAGE_CHECK_BEFORE_UPDATE,
	TASK_USAGE_UPDATE,
	TASK_USAGE_CHECK_AFTER_UPDATE,
};
struct TASK_INFO
{
	UINT					ID;
	TASK_TYPE				Type;
	volatile TASK_STATUS	Status;
	UINT					ServiceID;
	CEasyString				SourceFilePath;
	CEasyString				TargetFilePath;
	UINT					FileMode;
	UINT64					FileSize;
	time_t					FileLastWriteTime;
	UINT64					QueryOffset;
	UINT64					TransferOffset;	
	UINT64					TransferSize;
	UINT64					FinishSize;
	bool					ContinueTransfer;
	UINT					TransferStartTime;
	UINT					Usage;
	CWinFileAccessor		TaskFile;
	CEasyString				FileMD5;
	CEasyString				RemoteMD5;
	UINT					CurBlockIndex;
	volatile UINT			QueryCount;
	void Clear()
	{
		ID = 0;
		Type = TASK_TYPE_NONE;
		Status = TASK_STATUS_NONE;
		ServiceID = 0;
		SourceFilePath.Clear();
		TargetFilePath.Clear();
		FileMode = 0;
		FileSize = 0;
		FileLastWriteTime = 0;
		QueryOffset = 0;
		TransferOffset = 0;		
		TransferSize = 0;
		FinishSize = 0;
		ContinueTransfer = false;
		TransferStartTime = 0;
		Usage = 0;
		TaskFile.Close();
		FileMD5.Clear();
		RemoteMD5.Clear();
		CurBlockIndex = 0;
		QueryCount = 0;
	}
	void IncQueryCount()
	{
		AtomicInc(&QueryCount);
	}
	void DecQueryCount()
	{
		AtomicDec(&QueryCount);
	}
};

struct FILE_DATA_INFO
{
	UINT								DataID;
	UINT								Index;
	CEasyBuffer							DataBuffer;
	UINT								OriginSize;
	bool								IsLast;

	void SetID(UINT ID)
	{
		DataID = ID;
		OriginSize = 0;
		IsLast = false;
	}	
};

#include "WorkThread.h"
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
	static TCHAR ResultStr[256];
	if (Result >= 0 && Result < g_MSG_RESULT_COUNT)
	{
		_stprintf_s(ResultStr, 128, "%d(%s)", Result, MSG_RESULT_To_Str(Result));
	}
	else
	{
		_stprintf_s(ResultStr, 128, "%d", Result);
	}
	return ResultStr;
}

inline void LogWithTag(const char* szTag, const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(LOG_CHANNEL_MAIN, ILogPrinter::LOG_LEVEL_NORMAL, szTag, Format, vl);
	va_end(vl);
}

inline void LogDebugWithTag(const char* szTag, const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(LOG_CHANNEL_MAIN, ILogPrinter::LOG_LEVEL_DEBUG, szTag, Format, vl);
	va_end(vl);
}

#ifdef WIN32
#define Log(_Format, ...)	LogWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)
#else
#define Log(_Format, ...)	LogWithTag(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugWithTag(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#endif

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


