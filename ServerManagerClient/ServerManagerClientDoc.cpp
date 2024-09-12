/****************************************************************************/
/*                                                                          */
/*      文件名:    ServerManagerClientDoc.cpp                               */
/*      创建日期:  2010年02月09日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
// ServerManagerClientDoc.cpp : CServerManagerClientDoc 类的实现
//

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerClientDoc

IMPLEMENT_DYNCREATE(CServerManagerClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CServerManagerClientDoc, CDocument)
END_MESSAGE_MAP()


// CServerManagerClientDoc 构造/析构

CServerManagerClientDoc::CServerManagerClientDoc()
{
	// TODO: 在此添加一次性构造代码

}

CServerManagerClientDoc::~CServerManagerClientDoc()
{
}

BOOL CServerManagerClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CServerManagerClientDoc 序列化

void CServerManagerClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CServerManagerClientDoc 诊断

#ifdef _DEBUG
void CServerManagerClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServerManagerClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CServerManagerClientDoc 命令
