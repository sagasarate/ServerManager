/****************************************************************************/
/*                                                                          */
/*      文件名:    MainApp.h                                                */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// DBProxy.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once


// CMainApp:
// 有关此类的实现，请参阅 DBProxy.cpp
//

class CMainApp : public CServerApp
{
protected:
	
public:
	CMainApp();

// 重写
	public:
	//virtual BOOL InitInstance();
	//virtual int ExitInstance();

	virtual BOOL OnStartUp();
	virtual void OnShutDown();

// 实现

	
};

//extern CMainApp theApp;
