/****************************************************************************/
/*                                                                          */
/*      文件名:    MainApp.cpp                                              */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DBProxy.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR g_ProgramName="ServerManager";
LPCTSTR g_ServiceName="ServerManager";
LPCTSTR g_ServiceDesc="ServerManager";



// CMainApp 构造

CMainApp::CMainApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMainApp 对象

CMainApp theApp;


BOOL CMainApp::OnStartUp()
{
	if (!CServerApp::OnStartUp())
		return FALSE;
#ifdef WIN32
	CExceptionParser::GetInstance()->Init(EXCEPTION_SET_DEFAULT_HANDLER|EXCEPTION_USE_API_HOOK);
#endif

	if(!CMainThread::GetInstance()->StartUp())
		return FALSE;

	m_pServer=CMainThread::GetInstance();
	return TRUE;
}
void CMainApp::OnShutDown()
{
	CServerApp::OnShutDown();
	CLogManager::GetInstance()->ReleaseInstance();
}

//int main()
//{
//    //theApp.Run();
//    getchar();
//    return 0;
//}
