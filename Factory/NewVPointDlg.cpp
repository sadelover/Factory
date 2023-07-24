// NewVPointDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewVPointDlg.h"
#include "afxdialogex.h"



// CNewVPointDlg �Ի���

IMPLEMENT_DYNAMIC(CNewVPointDlg, CDialog)

CNewVPointDlg::CNewVPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewVPointDlg::IDD, pParent)
	, m_strPointID(_T(""))
	, m_nPointType(0)
	, m_strPointDef(_T(""))
{

}

CNewVPointDlg::~CNewVPointDlg()
{
}

void CNewVPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strPointID);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nPointType);
	DDX_Text(pDX, IDC_EDIT_POINT_DEF, m_strPointDef);
}


BEGIN_MESSAGE_MAP(CNewVPointDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewVPointDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewVPointDlg ��Ϣ�������


void CNewVPointDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}
