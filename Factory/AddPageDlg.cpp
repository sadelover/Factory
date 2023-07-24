// AddPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddPageDlg.h"
#include "MainFrm.h"
#include "MetaFileSelectDlg.h"
#include "BEOP-DesignerView.h"
#include "PageDlg.h"


// CAddPageDlg dialog

IMPLEMENT_DYNAMIC(CAddPageDlg, CSkinDlg)

CAddPageDlg::CAddPageDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CAddPageDlg::IDD, pParent)
	, m_nWidth(1920)
	, m_nHeight(955)
	,m_page_type(FULLSCREEN)
	, m_strNewPageName(_T(""))
	, m_bTop(TRUE)
	, m_pMainFrm(NULL)
	, m_pCanvasDlg(NULL)
	, m_nBkImageId(0)
	, m_bEnableBk(FALSE)
	, m_nPageType(0)
	, m_strWebAddr(_T(""))
{

}

CAddPageDlg::~CAddPageDlg()
{
}

void CAddPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_NAME, m_sticViewName);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_NEW_PAGE_NAME, m_strNewPageName);
	DDX_Check(pDX, IDC_CHECK_TOP, m_bTop);
	DDX_Check(pDX, IDC_CHECK_BK, m_bEnableBk);
	DDX_Radio(pDX, IDC_RADIO_FIX, m_nPageType);
	//DDX_Control(pDX, IDC_EDIT_WEB_ADDR, m_editWebAddr);
	//DDX_Text(pDX, IDC_EDIT_WEB_ADDR, m_strWebAddr);
	DDX_Control(pDX, IDC_COMBO_WEB_ADDR, m_comboWebAddr);
}


BEGIN_MESSAGE_MAP(CAddPageDlg, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CAddPageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_FIX, &CAddPageDlg::OnBnClickedRadioFix)
	ON_BN_CLICKED(IDC_RADIO_FLOAT, &CAddPageDlg::OnBnClickedRadioFloat)
	ON_BN_CLICKED(IDC_RADIO_WEB, &CAddPageDlg::OnBnClickedRadioWeb)
	ON_BN_CLICKED(IDC_BUTTON_ADD_IMAGE, &CAddPageDlg::OnBnClickedButtonAddImage)
	ON_BN_CLICKED(IDC_CHECK_BK, &CAddPageDlg::OnBnClickedCheckBk)
END_MESSAGE_MAP()


// CAddPageDlg message handlers

BOOL CAddPageDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	m_btnCancel.SetButtonProjectStyle(Silver);

	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIDTH)->SetWindowTextW(L"1920");
	GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowTextW(L"955");

	m_pMainFrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (m_pMainFrm != NULL)
	{
		CBEOPDesignerView* pView = (CBEOPDesignerView*)m_pMainFrm->GetActiveView();
		m_pCanvasDlg = &(pView->m_PageDlg->m_dlgCanvas);
	}

	ClickBackgroundControl();

	//m_editWebAddr.EnableWindow(FALSE);
	m_comboWebAddr.EnableWindow(FALSE);
	InitWebComboBox();

	return TRUE;
}

void CAddPageDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strNewPageName.TrimLeft(_T(" "));
	m_strNewPageName.TrimRight(_T(" "));
	if (0 == m_strNewPageName.GetLength())
	{
		MessageBox(_T("页面名称不能为空！"), _T("错误"), MB_OK|MB_ICONERROR);

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NEW_PAGE_NAME);
		if (pEdit != NULL)
		{
			pEdit->SetSel(0, -1);
			pEdit->SetFocus();
		}

		return;
	}
	if (0 == m_nPageType)
	{
		m_page_type = FULLSCREEN;
	}
	else
	{
		CString strWidth;
		CString strHeight;
		GetDlgItem(IDC_EDIT_WIDTH)->GetWindowText(strWidth);
		GetDlgItem(IDC_EDIT_HEIGHT)->GetWindowText(strHeight);
		if (strWidth.GetLength() == 0)
		{
			MessageBox(L"宽度为空，请输入!");
			return;
		}
		if (strHeight.GetLength() == 0)
		{
			MessageBox(L"高度为空，请输入!");
			return;
		}
		m_nWidth = _wtoi(strWidth.GetString());
		m_nHeight = _wtoi(strHeight.GetString());
		if (1 == m_nPageType)
		{
			m_page_type = FLOATING;
		}
		else if (2 == m_nPageType)
		{
			m_page_type = WEBTYPE;
		}
	}
	
	if (m_bEnableBk)
	{
		if (GetDlgItemInt(IDC_EDIT_BK_IMAGE) <= 0)
		{
			MessageBox(_T("请选择背景图！"), _T("错误"), MB_OK|MB_ICONERROR);

			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BK_IMAGE);
			if (pEdit != NULL)
			{
				pEdit->SetSel(0, -1);
				pEdit->SetFocus();
			}

			return;
		}
	}

	m_strWebAddr = GetWebComboValue();

	CDialog::OnOK();
}


void CAddPageDlg::OnBnClickedRadioFix()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_TOP)->EnableWindow(TRUE);
	//m_editWebAddr.EnableWindow(FALSE);
	m_comboWebAddr.EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_WIDTH)->SetWindowTextW(L"1920");
	GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowTextW(L"955");

	UpdateData(TRUE);
	m_bTop = TRUE;
	m_nPageType = 0;
	UpdateData(FALSE);
}


void CAddPageDlg::OnBnClickedRadioFloat()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_TOP)->EnableWindow(FALSE);
	//m_editWebAddr.EnableWindow(FALSE);
	m_comboWebAddr.EnableWindow(FALSE);

	UpdateData(TRUE);
	m_bTop = FALSE;
	m_nPageType = 1;
	UpdateData(FALSE);
}


void CAddPageDlg::OnBnClickedRadioWeb()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_TOP)->EnableWindow(TRUE);
	//m_editWebAddr.EnableWindow(TRUE);
	m_comboWebAddr.EnableWindow(TRUE);

	UpdateData(TRUE);
	m_bTop = TRUE;
	m_nPageType = 2;
	UpdateData(FALSE);
}


void CAddPageDlg::OnBnClickedButtonAddImage()
{
	// TODO: Add your control notification handler code here
	assert(m_pMainFrm != NULL);
	if (NULL == m_pMainFrm)
	{
		return;
	}
	if (!m_pMainFrm->m_bLoadIndependent)
	{
		m_pMainFrm->m_pPrjSqlite->LoadPictureLibSqlite(true);
		m_pMainFrm->m_pPrjSqlite->LoadPictureLibSqlite(false);
		m_pMainFrm->m_pPrjSqlite->LoadAnimationSqlite(true);
		m_pMainFrm->m_pPrjSqlite->LoadAnimationSqlite(false);
		m_pMainFrm->m_bLoadIndependent = true;
	}


	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	if (IDOK == dlg.DoModal())
	{
		if (m_pCanvasDlg != NULL)
		{
			int nPic = 0;
			m_pCanvasDlg->InsertImageIntoOpenS3db(dlg.m_nID, nPic);
			m_nBkImageId = nPic;
			SetDlgItemInt(IDC_EDIT_BK_IMAGE, nPic);
		}
	}
}


void CAddPageDlg::OnBnClickedCheckBk()
{
	// TODO: Add your control notification handler code here
	ClickBackgroundControl();
}

void CAddPageDlg::ClickBackgroundControl(void)
{
	UpdateData(TRUE);

	EnableBackgroundControls(m_bEnableBk);
	if (!m_bEnableBk)
	{
		m_nBkImageId = 0;
		SetDlgItemInt(IDC_EDIT_BK_PIC_ID, 0);
	}
}

void CAddPageDlg::EnableBackgroundControls(const BOOL bFlag)
{
	GetDlgItem(IDC_EDIT_BK_IMAGE)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_ADD_IMAGE)->EnableWindow(bFlag);
}

void CAddPageDlg::InitWebComboBox(void)
{
	m_strWebName[0] = _T("项目报表");
	m_strWebName[1] = _T("诊断");
	m_strWebName[2] = _T("数据分析");
	m_strWebValue[0] = _T("report");
	m_strWebValue[1] = _T("diagnosis");
	m_strWebValue[2] = _T("analysis");
	m_comboWebAddr.AddString(m_strWebName[0]);
	m_comboWebAddr.AddString(m_strWebName[1]);
	m_comboWebAddr.AddString(m_strWebName[2]);
	m_comboWebAddr.SetCurSel(0);
}

CString	CAddPageDlg::GetWebComboValue(void) const
{
	CString	strWebShow;
	CString	strRet;
	int nCurSel = m_comboWebAddr.GetCurSel();
	m_comboWebAddr.GetLBText(nCurSel, strWebShow);
	if (strWebShow == m_strWebName[0])
	{
		strRet = m_strWebValue[0];
	} 
	else if (strWebShow == m_strWebName[1])
	{
		strRet = m_strWebValue[1];
	}
	else if (strWebShow == m_strWebName[2])
	{
		strRet = m_strWebValue[2];
	}

	return strRet;
}
