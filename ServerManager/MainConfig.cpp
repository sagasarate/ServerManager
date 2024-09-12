/****************************************************************************/
/*                                                                          */
/*      文件名:    MainConfig.cpp                                           */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"

CMainConfig::CMainConfig(void)
{
	FUNCTION_BEGIN;
	m_MaxClient=10000;
	m_RecvBufferSize = 65536;
	m_SendBufferSize = 65536;
	m_KeepAliveTime = 5 * 1000;
	m_KeepAliveCount = 10;
	m_ProcessInfoFetchTime=5000;
	m_MaxWorkThreadCount = 5;
	FUNCTION_END;
}

CMainConfig::~CMainConfig(void)
{
}

bool CMainConfig::LoadConfig(LPCTSTR FileName)
{
	xml_parser Parser;

	if(Parser.parse_file(FileName,pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();

		if(Config.moveto_child("Main"))
		{
			if (Config.has_attribute("MaxWorkThreadCount"))
				m_MaxWorkThreadCount = Config.attribute("MaxWorkThreadCount");

			xml_node PoolConfig = Config;
			if (PoolConfig.moveto_child("PoolConfig"))
			{
				ReadPoolConfigs(PoolConfig, m_PoolConfigs);
			}

			xml_node ClientListen=Config;
			if(ClientListen.moveto_child("ClientListen"))
			{
				if(ClientListen.has_attribute("IP"))
					m_ListenAddress.SetIP((LPCTSTR)ClientListen.attribute("IP"));

				if(ClientListen.has_attribute("Port"))
					m_ListenAddress.SetPort(ClientListen.attribute("Port"));

				if(ClientListen.has_attribute("MaxClient"))
					m_MaxClient=ClientListen.attribute("MaxClient");

				if (ClientListen.has_attribute("RecvBufferSize"))
					m_RecvBufferSize = ClientListen.attribute("RecvBufferSize");

				if (ClientListen.has_attribute("SendBufferSize"))
					m_SendBufferSize = ClientListen.attribute("SendBufferSize");

				if (ClientListen.has_attribute("KeepAliveTime"))
					m_KeepAliveTime = ClientListen.attribute("KeepAliveTime");

				if (ClientListen.has_attribute("KeepAliveCount"))
					m_KeepAliveCount = ClientListen.attribute("KeepAliveCount");

			}			
			xml_node ProcessMonitor=Config;
			if(ProcessMonitor.moveto_child("ProcessMonitor"))
			{
				if(ProcessMonitor.has_attribute("ProcessInfoFetchTime"))
					m_ProcessInfoFetchTime=ProcessMonitor.attribute("ProcessInfoFetchTime");
			}			

			xml_node Notify = Config;
			if (Notify.moveto_child("Notify"))
			{
				if (Notify.has_attribute("GetTokenURL"))
					m_NotifyConfig.GetTokenURL = Notify.attribute("GetTokenURL").getvalue();
				if (Notify.has_attribute("SendNotifyURL"))
					m_NotifyConfig.SendNotifyURL = Notify.attribute("SendNotifyURL").getvalue();
				if (Notify.has_attribute("SendTarget"))
					m_NotifyConfig.SendTarget = Notify.attribute("SendTarget").getvalue();
			}
			
			xml_node UserList = Config;
			if (UserList.moveto_child("UserList"))
			{
				for (UINT i = 0; i < UserList.children(); i++)
				{
					xml_node UserInfo = UserList.child(i);
					if (_tcsicmp(UserInfo.name(), "User") == 0)
					{
						if (UserInfo.has_attribute("Name") && UserInfo.has_attribute("Password"))
						{
							USER_INFO Info;
							Info.UserName = UserInfo.attribute("Name").getvalue();
							Info.Password = UserInfo.attribute("Password").getvalue();

							m_UserList.Add(Info);
						}
					}
				}
			}

			xml_node TaskManagerConfig = Config;
			if (TaskManagerConfig.moveto_child("TaskManagerConfig"))
			{
				if (TaskManagerConfig.has_attribute("SerialWorkThreadCount"))
					m_TaskManagerConfig.SerialWorkThreadCount = TaskManagerConfig.attribute("SerialWorkThreadCount");

				if (TaskManagerConfig.has_attribute("ParallelWorkThreadCount"))
					m_TaskManagerConfig.ParallelWorkThreadCount = TaskManagerConfig.attribute("ParallelWorkThreadCount");

				if (TaskManagerConfig.has_attribute("MaxUploadAcceptCount"))
					m_TaskManagerConfig.MaxUploadAcceptCount = TaskManagerConfig.attribute("MaxUploadAcceptCount");

				if (TaskManagerConfig.has_attribute("MaxDownloadAcceptCount"))
					m_TaskManagerConfig.MaxDownloadAcceptCount = TaskManagerConfig.attribute("MaxDownloadAcceptCount");

				if (TaskManagerConfig.has_attribute("TaskTimeOut"))
					m_TaskManagerConfig.TaskTimeOut = TaskManagerConfig.attribute("TaskTimeOut");
			}
		}
		else
		{
			Log("配置文件[%s]不合法",FileName);
			return false;
		}
	}
	else
	{
		Log("无法打开配置文件[%s]",FileName);
		return false;
	}

	return true;
}

bool CMainConfig::LoadServiceList(LPCTSTR FileName, CEasyArray<CServiceInfoEx>& ServiceList)
{
	xml_parser Parser;

	if (Parser.parse_file(FileName, pug::parse_trim_attribute))
	{
		xml_node SrvList = Parser.document();

		if (SrvList.moveto_child("ServiceList"))
		{
			for (UINT i = 0; i < SrvList.children(); i++)
			{
				xml_node Service = SrvList.child(i);
				if (_stricmp(Service.name(), "Service") == 0)
				{
					if (Service.has_attribute("ID"))
					{
						CServiceInfoEx * pServiceInfo = ServiceList.AddEmpty();

						pServiceInfo->SetServiceID(Service.attribute("ID"));
						if (Service.has_attribute("Type"))
						{
							pServiceInfo->SetType(Service.attribute("Type"));
						}
						if (Service.has_attribute("Name"))
						{
							pServiceInfo->SetName(Service.attribute("Name").getvalue());
						}
						if (Service.has_attribute("ImageFilePath"))
						{
							pServiceInfo->SetImageFilePath(CFileTools::MakeFullPath(Service.attribute("ImageFilePath").getvalue()));
						}
						if (Service.has_attribute("WorkDir"))
						{
							CEasyString WorkDir = Service.attribute("WorkDir").getvalue();
							WorkDir.Trim();
							if (WorkDir.IsEmpty() && pServiceInfo->GetType() != SERVICE_TYPE_DIRECTORY)
							{
								WorkDir = CFileTools::GetPathDirectory(pServiceInfo->GetImageFilePath());
							}
							else
							{
								WorkDir = CFileTools::MakeFullPath(WorkDir);
							}
							WorkDir.TrimRight(DIR_SLASH);
							pServiceInfo->SetWorkDir(WorkDir);
						}
						if (Service.has_attribute("StartupParam"))
						{
							pServiceInfo->SetStartupParam(Service.attribute("StartupParam").getvalue());
						}

						if (Service.has_attribute("KeepRunning"))
						{
							pServiceInfo->SetKeepRunning(Service.attribute("KeepRunning"));
						}						

						if (Service.has_attribute("RestartupTime"))
						{
							pServiceInfo->SetRestartupTime(Service.attribute("RestartupTime"));
						}
						if (Service.has_attribute("ControlPipeName"))
						{
							pServiceInfo->SetControlPipeName(Service.attribute("ControlPipeName").getvalue());
						}
						if (Service.has_attribute("ShutdownCmd"))
						{
							pServiceInfo->SetShutdownCmd(Service.attribute("ShutdownCmd").getvalue());
						}
						if (Service.has_attribute("CharSet"))
						{
							pServiceInfo->SetCharSet(Service.attribute("CharSet"));
						}

						if (Service.has_attribute("LogStatusToFile"))
						{
							pServiceInfo->SetLogStatusToFile(Service.attribute("LogStatusToFile"));
						}

						xml_node OtherExecFileList = Service;
						if (OtherExecFileList.moveto_child("OtherExecFileList"))
						{
							for (UINT j = 0; j < OtherExecFileList.children(); j++)
							{
								xml_node ExecFile = OtherExecFileList.child(j);
								if (_stricmp(ExecFile.name(), "ExecFile") == 0)
								{
									if (ExecFile.has_attribute("FilePath"))
									{
										pServiceInfo->GetOtherExecFileList().Add(ExecFile.attribute("FilePath").getvalue());
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		Log("无法打开文件[%s]", FileName);
	}

	return false;
}
bool CMainConfig::SaveServiceList(LPCTSTR FileName, CEasyArray<CServiceInfoEx>& ServiceList)
{
	pug::xml_parser Xml;

	Xml.create();
	xml_node Doc;
	Doc = Xml.document();
	xml_node pi = Doc.append_child(node_pi);
	pi.name(_T("xml"));
	pi.attribute(_T("version")) = _T("1.0");
	pi.attribute(_T("encoding")) = _T("utf8");

	xml_node SrvList = Doc.append_child(node_element, _T("ServiceList"));

	for (UINT i = 0; i < ServiceList.GetCount(); i++)
	{
		xml_node Service = SrvList.append_child(node_element, _T("Service"));

		Service.append_attribute(_T("ID"), ServiceList[i].GetServiceID());
		Service.append_attribute(_T("Type"), ServiceList[i].GetType());
		Service.append_attribute(_T("Name"), (LPCTSTR)ServiceList[i].GetName());
		Service.append_attribute(_T("ImageFilePath"), (LPCTSTR)ServiceList[i].GetImageFilePath());
		Service.append_attribute(_T("WorkDir"), (LPCTSTR)ServiceList[i].GetWorkDir());
		Service.append_attribute(_T("StartupParam"), (LPCTSTR)ServiceList[i].GetStartupParam());
		Service.append_attribute(_T("KeepRunning"), ServiceList[i].GetKeepRunning());
		Service.append_attribute(_T("RestartupTime"), ServiceList[i].GetRestartupTime());
		Service.append_attribute(_T("ControlPipeName"), (LPCTSTR)ServiceList[i].GetControlPipeName());
		Service.append_attribute(_T("ShutdownCmd"), (LPCTSTR)ServiceList[i].GetShutdownCmd());
		Service.append_attribute(_T("CharSet"), ServiceList[i].GetCharSet());
		Service.append_attribute(_T("LogStatusToFile"), ServiceList[i].GetLogStatusToFile());

		xml_node OtherExecFileList = Service.append_child(node_element, _T("OtherExecFileList"));

		for (UINT j = 0; j < ServiceList[i].GetOtherExecFileList().GetCount(); j++)
		{
			xml_node ExecFile = OtherExecFileList.append_child(node_element, _T("ExecFile"));

			ExecFile.append_attribute(_T("FilePath"), ServiceList[i].GetOtherExecFileList()[j]);
		}
	}

	return Xml.SaveToFile(Xml.document(), FileName, CP_UTF8);
}

bool CMainConfig::VerfyUser(LPCTSTR UserName, LPCTSTR Password)
{
	for (UINT i = 0; i < m_UserList.GetCount(); i++)
	{
		USER_INFO& Info = m_UserList[i];
		if (Info.UserName.CompareNoCase(UserName) == 0 && Info.Password.Compare(Password) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CMainConfig::ReadPoolConfigs(xml_node& XMLContent, POOL_CONFIGS& Config)
{
	xml_node FileTaskPoolConfig = XMLContent;
	if (FileTaskPoolConfig.moveto_child("FileTaskPoolConfig"))
	{
		ReadPoolConfig(FileTaskPoolConfig, Config.FileTaskPoolConfig);
	}	
	xml_node FileDataBlockPoolConfig = XMLContent;
	if (FileDataBlockPoolConfig.moveto_child("FileDataBlockPoolConfig"))
	{
		ReadPoolConfig(FileDataBlockPoolConfig, Config.FileDataBlockPoolConfig);
	}
	xml_node SerialOperationPoolConfig = XMLContent;
	if (SerialOperationPoolConfig.moveto_child("SerialOperationPoolConfig"))
	{
		ReadPoolConfig(SerialOperationPoolConfig, Config.SerialOperationPoolConfig);
	}
	xml_node ParallelOperationPoolConfig = XMLContent;
	if (ParallelOperationPoolConfig.moveto_child("ParallelOperationPoolConfig"))
	{
		ReadPoolConfig(ParallelOperationPoolConfig, Config.ParallelOperationPoolConfig);
	}
	xml_node FinishOperationPoolConfig = XMLContent;
	if (FinishOperationPoolConfig.moveto_child("FinishOperationPoolConfig"))
	{
		ReadPoolConfig(FinishOperationPoolConfig, Config.FinishOperationPoolConfig);
	}
	return true;
}