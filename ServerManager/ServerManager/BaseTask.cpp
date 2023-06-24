#include "stdafx.h"

CBaseTask::CBaseTask()
{
	m_pManager = NULL;
	m_TaskType = TASK_TYPE_NONE;
	m_Status = TASK_STATUS_NONE;
	m_CallbackFN = NULL;
	m_Param = NULL;
	//m_NeedCancel = false;
}
CBaseTask::~CBaseTask()
{

}

void CBaseTask::Destory()
{
	m_pManager = NULL;
	m_TaskType = TASK_TYPE_NONE;
	m_Status = TASK_STATUS_NONE;
	m_CallbackFN = NULL;
	m_Param = NULL;
	//m_NeedCancel = false;
}