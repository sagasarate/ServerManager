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

bool CMainConfig::LoadServiceList(LPCTSTR FileName, CEasyArray<SERVICE_INFO>& ServiceList)
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
						SERVICE_INFO ServiceInfo;

						ServiceInfo.ServiceID = Service.attribute("ID");
						if (Service.has_attribute("Type"))
						{
							ServiceInfo.Type = Service.attribute("Type");
						}
						if (Service.has_attribute("Name"))
						{
							ServiceInfo.Name = Service.attribute("Name").getvalue();
						}
						if (Service.has_attribute("ImageFilePath"))
						{
							ServiceInfo.ImageFilePath = CFileTools::MakeFullPath(Service.attribute("ImageFilePath").getvalue());
						}
						if (Service.has_attribute("WorkDir"))
						{
							ServiceInfo.WorkDir = Service.attribute("WorkDir").getvalue();
							ServiceInfo.WorkDir.Trim();
							if (ServiceInfo.WorkDir.IsEmpty() && ServiceInfo.Type != SERVICE_TYPE_DIRECTORY)
							{
								ServiceInfo.WorkDir = CFileTools::GetPathDirectory(ServiceInfo.ImageFilePath);
							}
							else
							{
								ServiceInfo.WorkDir = CFileTools::MakeFullPath(ServiceInfo.WorkDir);
							}
							ServiceInfo.WorkDir.TrimRight(DIR_SLASH);
						}
						if (Service.has_attribute("StartupParam"))
						{
							ServiceInfo.StartupParam = Service.attribute("StartupParam").getvalue();
						}

						if (Service.has_attribute("KeepRunning"))
						{
							bool KeepRunning = Service.attribute("KeepRunning");
							if (KeepRunning)
								ServiceInfo.LastOperation = SERVICE_OPERATION_STARTUP;
							else
								ServiceInfo.LastOperation = SERVICE_OPERATION_SHUTDOWN;
						}
						else
						{
							ServiceInfo.LastOperation = SERVICE_OPERATION_STARTUP;
						}

						if (Service.has_attribute("RestartupTime"))
						{
							ServiceInfo.RestartupTime = Service.attribute("RestartupTime");
						}

						ServiceList.Add(ServiceInfo);
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
bool CMainConfig::SaveServiceList(LPCTSTR FileName, CEasyArray<SERVICE_INFO>& ServiceList)
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

		Service.append_attribute(_T("ID"), ServiceList[i].ServiceID);
		Service.append_attribute(_T("Type"), ServiceList[i].Type);
		Service.append_attribute(_T("Name"), (LPCTSTR)ServiceList[i].Name);
		Service.append_attribute(_T("ImageFilePath"), (LPCTSTR)ServiceList[i].ImageFilePath);
		Service.append_attribute(_T("WorkDir"), (LPCTSTR)ServiceList[i].WorkDir);
		Service.append_attribute(_T("StartupParam"), (LPCTSTR)ServiceList[i].StartupParam);
		if (ServiceList[i].LastOperation == SERVICE_OPERATION_STARTUP)
			Service.append_attribute(_T("KeepRunning"), true);
		else
			Service.append_attribute(_T("KeepRunning"), false);
		Service.append_attribute(_T("RestartupTime"), ServiceList[i].RestartupTime);
	}

	return Xml.SaveToFile(Xml.document(), FileName, CP_UTF8);
}
