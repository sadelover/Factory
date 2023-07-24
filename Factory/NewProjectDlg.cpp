// NewProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewProjectDlg.h"
#include "pathdialog.h"
#include "Tools/CustomTools/CustomTools.h"

// CNewProjectDlg dialog

IMPLEMENT_DYNAMIC(CNewProjectDlg, CSkinDlg)

CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CNewProjectDlg::IDD, pParent)
	, m_ProjectName(_T(""))
	, m_strFileDir(_T(""))
	, m_nDeviceType(0)
{
}

CNewProjectDlg::~CNewProjectDlg()
{
}

void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITPROJECTNAME, m_ProjectName);
	DDX_Control(pDX, IDC_PROHECT_NAME, m_stiProjectName);
	DDX_Control(pDX, IDOK, m_btnNewProject);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_EDITPROJECTNAME2, m_strFileDir);
	DDX_Control(pDX, IDC_COMBO_DEVICE_TYPE, m_comboDeviceType);
	DDX_Control(pDX, IDC_COMBO_FILE_TYPE, m_comboFileType);
}


BEGIN_MESSAGE_MAP(CNewProjectDlg, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CNewProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FILEPOS, &CNewProjectDlg::OnBnClickedButtonFilepos)
END_MESSAGE_MAP()


BOOL CNewProjectDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();
	m_cancel.SetButtonProjectStyle(Silver);

	m_comboDeviceType.AddString(_T("用于显示在桌面版应用程序PC设备"));
	m_comboDeviceType.AddString(_T("用于显示在iPhone手机端设备"));
	m_comboDeviceType.SetCurSel(0);

	m_comboFileType.AddString(_T("s3db"));
	m_comboFileType.AddString(_T("4db"));
	m_comboFileType.SetCurSel(1);

	GetSysPath(m_strFileDir);
	m_strFileDir += _T("\\config");

	UpdateData(FALSE);
	
	return TRUE;
}
// CNewProjectDlg message handlers

void CNewProjectDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  // add by hun 7.28
    const wstring c_str_space = _T(" ");
	int spacePos = -1;
	wstring wstr = m_ProjectName;
	int	nNameLen = m_ProjectName.GetLength();


	if (nNameLen <= 0)
	{
		::MessageBox(NULL, _T("项目名不能为空!"), _T("提示"), MB_OK);
		return;
	}
	else if (nNameLen > 20)
	{
		::MessageBox(NULL, _T("文件名长度限20字符以内"), _T("提示"), MB_OK);
		return;
	}

	if( 0!= m_ProjectName.Find(' '))
	{
		if( -1 != m_ProjectName.Find('@')||
			-1 != m_ProjectName.Find('*')|| 
			-1 != m_ProjectName.Find('#')||
			-1 != m_ProjectName.Find('!')||
			-1 != m_ProjectName.Find('$')||
			-1 != m_ProjectName.Find('&')||
			-1 != m_ProjectName.Find('?')||
			-1 != m_ProjectName.Find('+')||
			-1 != m_ProjectName.Find(' ')
			)
			{
				::MessageBox(NULL, _T("文件名命名不合法(包含非法字符)！\r\n (例如：@，空格，#,!,$,?,+,&)"), _T("提示"), MB_OK);
				return;
			}
	}
	else
		{	
		 ::MessageBox(NULL, _T("文件名首字母不能为空格！"), _T("提示"), MB_OK);
		   return ;
		}


	// deny Chinese char
	const CString strFlag(_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
	CString	strChar;

	for (int i=0; i<nNameLen; ++i)
	{
		strChar = m_ProjectName.Mid(i, 1);
		if (-1 == strFlag.Find(strChar))
		{	// find other char
			MessageBox(_T("项目名称只能使用字母和数字！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}
	}

	m_nDeviceType = m_comboDeviceType.GetCurSel();
	m_nFileType = m_comboFileType.GetCurSel();


	OnOK();
}


void CNewProjectDlg::OnBnClickedButtonFilepos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CPathDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_strFileDir = dlg.GetPathName();
		UpdateData(FALSE);
	}
	
}

int CNewProjectDlg::GetDeviceType(void)
{
	return m_nDeviceType;
}

int CNewProjectDlg::GetFileType(void)
{
	return m_nFileType;
}
