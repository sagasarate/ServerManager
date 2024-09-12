/****************************************************************************/
/*                                                                          */
/*      文件名:    ServiceController.cpp                                    */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ServiceController.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"



int _tmain(int argc, _TCHAR* argv[])
{	
	if(argc>1)
	{	
		CEasyScriptExecutor ScriptExcutor;
		CESThread ESThread;
		CESVariableList VarList;
		CESFunctionList FnList;
		CESBolanStack Script;
		CScriptFunctions Functions;

		VarList.Create(128);
		FnList.Create(128);

		ESThread.SetVariableList(&VarList);
		ESThread.SetFunctionList(&FnList);
		ESThread.SetScript(&Script);
		

		FnList.AddCFunction("StartService",1,&Functions,&CScriptFunctions::StartService);
		FnList.AddCFunction("StopService",1,&Functions,&CScriptFunctions::StopService);
		FnList.AddCFunction("CopyFile",2,&Functions,&CScriptFunctions::CopyFile);
		FnList.AddCFunction("Sleep",1,&Functions,&CScriptFunctions::Sleep);

		CWinFileAccessor FileAccessor;
		if(FileAccessor.Open(argv[1],IFileAccessor::modeOpen|IFileAccessor::modeRead|IFileAccessor::shareShareAll))
		{
			CEasyString Script;
			UINT Len=FileAccessor.GetSize();
			Script.Resize(Len);
			if(FileAccessor.Read((LPVOID)Script.GetBuffer(),Len)==Len)
			{				
				ESThread.PushScript(Script);
				if(ESThread.GetResultCode()==0)
				{
					ScriptExcutor.ExecScript(ESThread);
					if(ESThread.GetResultCode())
					{
						LPCTSTR ErrMsg=ESGetErrorMsg(ESThread.GetResultCode());
						PrintImportantLog("执行脚本失败:%s在行%d附近",
							ErrMsg,ESThread.GetLastLine());
					}
				}
				else
				{
					LPCTSTR ErrMsg=ESGetErrorMsg(ESThread.GetResultCode());
					PrintImportantLog("解析脚本失败:%s在行%d附近",
						ErrMsg,ESThread.GetLastLine());
				}
			}
			else
			{
				PrintImportantLog("读取脚本文件失败");
			}
		}
		else
		{
			PrintImportantLog(0,"无法打开脚本文件%s",argv[1]);
		}
	}
	else
	{
		printf("Usage:ServiceController ScriptFileName");
	}
	return 0;
}

