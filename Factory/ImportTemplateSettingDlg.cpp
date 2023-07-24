// ImportTemplateSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportTemplateSettingDlg.h"
#include "afxdialogex.h"


// CImportTemplateSettingDlg dialog

IMPLEMENT_DYNAMIC(CImportTemplateSettingDlg, CDialog)

CImportTemplateSettingDlg::CImportTemplateSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportTemplateSettingDlg::IDD, pParent)
	, m_strNewPageName(_T(""))
	, m_strPrePointName(_T(""))
	, m_strReplacePointName(_T(""))
{

}

CImportTemplateSettingDlg::~CImportTemplateSettingDlg()
{
}

void CImportTemplateSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEW_NAME, m_strNewPageName);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_PRE, m_strPrePointName);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_REPLACE, m_strReplacePointName);
}


BEGIN_MESSAGE_MAP(CImportTemplateSettingDlg, CDialog)
END_MESSAGE_MAP()


// CImportTemplateSettingDlg message handlers
