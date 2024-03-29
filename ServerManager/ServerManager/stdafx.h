﻿/****************************************************************************/
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
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0600		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0600		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0600 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifdef WIN32

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0600	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#endif

#include "../../Libs/Utils/Utils.h"
#include "../../Libs//NetLib/NetLib.h"
#include "../../Libs/ServerFrameWork/ServerFrameWork.h"
#include "../Protocol/Protocol.h"
//#include "../../Libs/zlib/zlib.h"


#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#undef new
#endif

#include "../../Libs/rapidjson/document.h"

#if defined(USE_CRT_DETAIL_NEW) && defined(_DEBUG)
#define new new( __FILE__, __LINE__ )
#endif


#define CURL_STATICLIB
#include <curl/curl.h>

#ifdef WIN32
//#include "Tlhelp32.h"
#include "Psapi.h"
#include "Iphlpapi.h"
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Iphlpapi.lib")
//#pragma comment(lib,"Toolhelp.lib")
#else
#include <sys/vfs.h>
#endif

#define MAX_SERVER_TERMINATE_WAIT_TIME	300000
#define CONFIG_FILE_NAME				"Config.xml"
#define SERVICE_LIST_FILE_NAME			"ServiceList.xml"

#define WORK_THREAD_NUM					2
#define ASSEMBEL_BUFFER_SIZE			(NET_DATA_BLOCK_SIZE*2)
#define PACK_NEED_SIZE					4096
#define DOWNLOAD_CONNECT_TIME			30000
#define DIR_BROWSE_TIMEOUT				30000
#define SERVICE_KEEP_TIME				5000
#define MAX_OPERATION_WAIT_TIME			30
#define CONTROL_PIPE_BUFFER_SIZE		8192
#define CONTROL_PIPE_RECREATE_TIME		(5*1000)
#define SERVICE_LOG_ID_SEED				7550000

#define	MAX_FILE_READ_BLOCK				32
#define TASK_STAT_TIME					10000


//#include "Protocol.h"
//
//enum SERVICE_TYPE
//{
//	SERVICE_TYPE_NORMAL,
//	SERVICE_TYPE_WIN_SERVICE,
//	SERVICE_TYPE_DIRECTORY,
//};
//struct SERVICE_INFO
//{
//	int			Type;
//	char		Name[MAX_PATH];
//	char		ImageFilePath[MAX_PATH];
//	char		WorkDir[MAX_PATH];
//	char		StartupParam[MAX_PATH];
//	DWORD		ProcessID;
//	UINT64		RecentTime;
//	BYTE		Status;
//	float		CPUUsed;
//	UINT		MemoryUsed;
//	UINT		VirtualMemoryUsed;
//	CEasyTime	ImageFileTime;
//	SERVICE_INFO()
//	{
//		Type=SERVICE_TYPE_NORMAL;
//		Name[0]=0;
//		ImageFilePath[0]=0;
//		WorkDir[0]=0;
//		StartupParam[0]=0;
//		ProcessID=0;
//		Status=SS_STOP;
//		RecentTime=0;
//		CPUUsed=0;
//		MemoryUsed=0;
//		VirtualMemoryUsed=0;
//	}
//};

//struct FILE_INFO
//{
//	CEasyString			FileName;
//	UINT				FileAttr;
//	ULONG64				FileSize;
//	time_t				FileCreateTime;
//	time_t				FileLastAccessTime;
//	time_t				FileLastWriteTime;
//};
//
//
//struct NET_ADAPTER_INFO
//{
//	UINT		Index;
//	CEasyString	IPAddress;
//	UINT64		SendBytes;
//	float		SendFlux;
//	UINT64		RecvBytes;
//	float		RecvFlux;
//};

enum HTTP_REQUEST_TYPE
{
	HTTP_REQUEST_TYPE_NONE,
	HTTP_REQUEST_TYPE_GET_TOKEN,
	HTTP_REQUEST_TYPE_SEND_NOTIFY,
};

enum TASK_TYPE
{
	TASK_TYPE_NONE,
	TASK_TYPE_READ,
	TASK_TYPE_MAKE_MD5,
	TASK_TYPE_WRITE,
};
enum TASK_STATUS
{
	TASK_STATUS_NONE,
	TASK_STATUS_PROCESSING,
	TASK_STATUS_FINISH,
	TASK_STATUS_END,
	TASK_STATUS_CANCEL,
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


struct FILE_DATA_BLOCK
{
	UINT								DataID;
	volatile UINT						SerialNumber;
	UINT64								Offset;
	CEasyBuffer							DataBuffer;
	UINT								OriginSize;
	bool								IsLast;
	bool								NeedAck;

	void SetID(UINT ID)
	{
		DataID = ID;
		SerialNumber = 0;
		OriginSize = 0;
		IsLast = false;
		NeedAck = false;
	}	
};

typedef FILE_DATA_BLOCK* LPFILE_DATA_BLOCK;

struct FILE_DATA_BLOCK_POINTER
{
	LPFILE_DATA_BLOCK pDataBlock;
	FILE_DATA_BLOCK_POINTER()
	{
		pDataBlock = NULL;
	}
	FILE_DATA_BLOCK_POINTER(LPFILE_DATA_BLOCK pData)
	{
		pDataBlock = pData;
	}
	operator LPFILE_DATA_BLOCK()
	{
		return pDataBlock;
	}
	bool operator<(const FILE_DATA_BLOCK_POINTER& Block)
	{
		return pDataBlock->SerialNumber < Block.pDataBlock->SerialNumber;
	}
};



struct HTTP_REQUEST_INFO
{
	UINT						RequestID;
	CEasyStringA				RequestURL;
	CEasyStringA				RequestContent;
	int							RequestType;
	UINT						Param1;
	UINT						Param2;
	CEasyString					Param3;
	CURL *						CURLHandle;
	CEasyBuffer					RecvBuffer;
	HTTP_REQUEST_INFO()
	{
		RequestID = 0;
		RequestType = HTTP_REQUEST_TYPE_NONE;
		Param1 = 0;
		Param2 = 0;
		CURLHandle = NULL;
	}
	~HTTP_REQUEST_INFO()
	{
		if (CURLHandle)
		{
			curl_easy_cleanup(CURLHandle);
		}
	}
	void SetID(UINT ID)
	{
		RequestID = ID;
	}
};


struct USER_INFO
{
	CEasyString	UserName;
	CEasyString	Password;
};

#include "ServiceControlPipe.h"

class CServiceInfoEx :public CServiceInfo
{
public:
	CServiceControlPipe	*	pControlPipe;
	UINT					LogID;
	CServiceInfoEx()
	{
		pControlPipe = NULL;
		LogID = 0;
	}
};

inline bool ReadPoolConfig(xml_node& XMLContent, STORAGE_POOL_SETTING& Config)
{

	if (XMLContent.has_attribute(_T("StartSize")))
		Config.StartSize = XMLContent.attribute(_T("StartSize"));
	if (XMLContent.has_attribute(_T("GrowSize")))
		Config.GrowSize = XMLContent.attribute(_T("GrowSize"));
	if (XMLContent.has_attribute(_T("GrowLimit")))
		Config.GrowLimit = XMLContent.attribute(_T("GrowLimit"));

	return true;
}

#include "MainConfig.h"

#include "BaseTask.h"
#include "FileTask.h"
#include "SerialWorkThread.h"
#include "ParallelWorkThread.h"
#include "TaskManager.h"

#include "ScriptExecutor.h"
#include "ServerManagerClient.h"
#include "ServerManagerService.h"



#include "MainThread.h"

#include "MainApp.h"

#define FUNCTION_BEGIN				//static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex,__FUNCTION__);
#define FUNCTION_END


