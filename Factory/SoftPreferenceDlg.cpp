// SoftPreferenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoftPreferenceDlg.h"
#include "afxdialogex.h"


// CSoftPreferenceDlg dialog

IMPLEMENT_DYNAMIC(CSoftPreferenceDlg, CDialog)

CSoftPreferenceDlg::CSoftPreferenceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftPreferenceDlg::IDD, pParent)
	, m_strImageLibPath(_T(""))
{

}

CSoftPreferenceDlg::~CSoftPreferenceDlg()
{
}

void CSoftPreferenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IMAGELIBNAME, m_strImageLibPath);
}


BEGIN_MESSAGE_MAP(CSoftPreferenceDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILEPOS, &CSoftPreferenceDlg::OnBnClickedButtonFilepos)
END_MESSAGE_MAP()


// CSoftPreferenceDlg message handlers


void CSoftPreferenceDlg::OnBnClickedButtonFilepos()
{
	// TODO: Add your control notification handler code here

	CFileDialog dlgOpenFile(TRUE,NULL,L"*.s3db",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"s3db Files (*.s3db)|*.s3db|");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		m_strImageLibPath = dlgOpenFile.GetPathName();

		UpdateData(FALSE);
	}
}
