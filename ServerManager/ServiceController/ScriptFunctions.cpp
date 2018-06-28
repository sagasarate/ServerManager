#include "StdAfx.h"
#include "ScriptFunctions.h"

CScriptFunctions::CScriptFunctions(void)
{
}

CScriptFunctions::~CScriptFunctions(void)
{
}


int CScriptFunctions::StartService(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	pResult->ValueType=VALUE_TYPE_INT;
	pResult->ValueInt=0;
	CWinServiceController ServiceController;

	if(ServiceController.OpenService(pParams[0]))
	{
		if(ServiceController.StartupService(NULL,0,5*60*1000))
		{
			pResult->ValueInt=1;
			PrintImportantLog("启动服务%s成功",(LPCTSTR)pParams[0]);
			return 0;
		}
	}
	PrintImportantLog("启动服务%s失败",(LPCTSTR)pParams[0]);
	return 0;
}

int CScriptFunctions::StopService(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	pResult->ValueType=VALUE_TYPE_INT;
	pResult->ValueInt=0;
	CWinServiceController ServiceController;

	if(ServiceController.OpenService(pParams[0]))
	{
		if(ServiceController.ShutdownService(5*60*1000))
		{
			pResult->ValueInt=1;
			PrintImportantLog("关闭服务%s成功",(LPCTSTR)pParams[0]);
			return 0;
		}
	}
	PrintImportantLog("关闭服务%s失败",(LPCTSTR)pParams[0]);
	return 0;
}

int CScriptFunctions::CopyFile(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	SHFILEOPSTRUCT Operation;
	char szFileFrom[MAX_PATH];
	char szFileTo[MAX_PATH];

	strncpy_0(szFileFrom,MAX_PATH,CFileTools::MakeFullPath(pParams[0]),MAX_PATH);
	szFileFrom[strlen(szFileFrom)+1]=0;

	strncpy_0(szFileTo,MAX_PATH,CFileTools::MakeFullPath(pParams[1]),MAX_PATH);
	szFileTo[strlen(szFileTo)+1]=0;

	Operation.hwnd=NULL;
	Operation.wFunc=FO_COPY;
	Operation.pFrom=szFileFrom;
	Operation.pTo=szFileTo;
	Operation.fAnyOperationsAborted=FALSE;
	Operation.hNameMappings=NULL;
	Operation.lpszProgressTitle=NULL;
	Operation.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;

	pResult->ValueType=VALUE_TYPE_INT;
	UINT Code=SHFileOperation(&Operation);
	if(Code==0)
	{
		PrintImportantLog("复制文件[%s]到[%s]成功",
			(LPCTSTR)pParams[0],
			(LPCTSTR)pParams[1]);
		pResult->ValueInt=1;
	}
	else
	{		
		PrintImportantLog("复制文件[%s]到[%s]失败0x%X",
			(LPCTSTR)pParams[0],
			(LPCTSTR)pParams[1],
			Code);
		pResult->ValueInt=0;
	}


	return 0;
}

int CScriptFunctions::Sleep(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	PrintImportantLog("Sleep%u毫秒",(UINT)pParams[0].ValueInt);
	DoSleep(pParams[0]);
	return 0;
}