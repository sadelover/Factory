// BEOP-DesignerDoc.cpp : implementation of the CBEOPDesignerDoc class
//
#include "stdafx.h"
#include "BEOP-Designer.h"
#include "BEOP-DesignerDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CBEOPDesignerDoc
IMPLEMENT_DYNCREATE(CBEOPDesignerDoc, CDocument)
BEGIN_MESSAGE_MAP(CBEOPDesignerDoc, CDocument)
END_MESSAGE_MAP()


// CBEOPDesignerDoc construction/destruction
CBEOPDesignerDoc::CBEOPDesignerDoc()
{
	// TODO: add one-time construction code here
}
CBEOPDesignerDoc::~CBEOPDesignerDoc()
{
}
BOOL CBEOPDesignerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}
// CBEOPDesignerDoc serialization
void CBEOPDesignerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
// CBEOPDesignerDoc diagnostics
#ifdef _DEBUG
void CBEOPDesignerDoc::AssertValid() const
{
	CDocument::AssertValid();
}
void CBEOPDesignerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
// CBEOPDesignerDoc commands


void CBEOPDesignerDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: Add your specialized code here and/or call the base class
	CBEOPDesignerApp *theApp = (CBEOPDesignerApp *)AfxGetApp();

	CDocument::SetTitle(theApp->m_wstr_app_name.c_str());
}
