// SetUpDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetUpDataDlg.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "MetaFileSelectDlg.h"
#include "MainFrm.h"
#include "BEOP-DesignerView.h"
#include "PageDlg.h"
// CSetUpDataDlg dialog

IMPLEMENT_DYNAMIC(CSetUpDataDlg, CDialog)

CSetUpDataDlg::CSetUpDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetUpDataDlg::IDD, pParent)
	, m_strEditOldValue(_T(""))
	, m_strEditNewValue(_T(""))
	,m_pPage(NULL)
	,m_nPicID(0)
	,m_nStartColor(0)
	,m_nEndColor(0)
	,m_nWidth(0)
	,m_nHeight(0)
	,m_bObserverTopShow(true)
	,m_nPageType(FULLSCREEN)
	, m_pMainFrm(NULL)
	, m_pCanvasDlg(NULL)
	, m_bEnableBk(FALSE)
	, m_nPageTp(0)
	, m_strWebAddr(_T(""))
	, m_strPageId(_T(""))
	, m_nPageMinRight(0)
	, m_bBackgroundImageModified(false)
	, m_strTemplateVarsDefine(_T(""))
{

}

CSetUpDataDlg::~CSetUpDataDlg()
{
}

void CSetUpDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITOLDVALUE, m_strEditOldValue);
	DDX_Text(pDX, IDC_EDITNEWVALUE, m_strEditNewValue);
	DDX_Control(pDX, IDC_CHECK_SHOW, m_checkShow);
	DDX_Control(pDX, IDC_COMBO_PAGE_SIZE, m_combo_page_size);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Check(pDX, IDC_CHECK_BK, m_bEnableBk);
	DDX_Radio(pDX, IDC_RADIO_FIXEDPAGE, m_nPageTp);
	//DDX_Control(pDX, IDC_EDIT_WEB_ADDR, m_editWebAddr);
	//DDX_Text(pDX, IDC_EDIT_WEB_ADDR, m_strWebAddr);
	DDX_Control(pDX, IDC_COMBO_WEB_ADDR, m_comboWebAddr);
	DDX_Text(pDX, IDC_EDIT_PAGEID, m_strPageId);
	DDX_Control(pDX, IDC_COMBO_PAGE_RIGHT, m_comboPageRight);
	DDX_CBIndex(pDX, IDC_COMBO_PAGE_RIGHT, m_nPageMinRight);
	DDX_Text(pDX, IDC_EDIT_TEMPLATE_VARS_DEFINE, m_strTemplateVarsDefine);
}


BEGIN_MESSAGE_MAP(CSetUpDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetUpDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_BK_PIC, &CSetUpDataDlg::OnBnClickedButtonChooseBkPic)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_COLOR_START, &CSetUpDataDlg::OnStnClickedStaticColorStart)
	ON_STN_CLICKED(IDC_STATIC_COLOR_END, &CSetUpDataDlg::OnStnClickedStaticColorEnd)
	ON_BN_CLICKED(IDC_RADIO_FIXEDPAGE, &CSetUpDataDlg::OnBnClickedRadioFixedpage)
	ON_BN_CLICKED(IDC_RADIO_FLOTTINGPAGE, &CSetUpDataDlg::OnBnClickedRadioFlottingpage)
	ON_BN_CLICKED(IDC_RADIO_WEBPAGE, &CSetUpDataDlg::OnBnClickedRadioWebpage)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE_SIZE, &CSetUpDataDlg::OnCbnSelchangeComboPageSize)
	ON_BN_CLICKED(IDC_CHECK_BK, &CSetUpDataDlg::OnBnClickedCheckBk)
	ON_BN_CLICKED(IDC_RADIO_TEMPLATE_PAGE, &CSetUpDataDlg::OnBnClickedRadioTemplatePage)
END_MESSAGE_MAP()


// CSetUpDataDlg message handlers

void CSetUpDataDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_nWidth = GetDlgItemInt(IDC_EDIT_PAGE_WIDTH);
	m_nHeight = GetDlgItemInt(IDC_EDIT_PAGE_HEIGHT);
	if(m_checkShow.GetCheck())
		m_bObserverTopShow = true;
	else
		m_bObserverTopShow = false;
	
	UpdateData(TRUE);
	if (m_bEnableBk)
	{
		if (GetDlgItemInt(IDC_EDIT_BK_PIC_ID) <= 0)
		{
			MessageBox(_T("请选择背景图！"), _T("错误"), MB_OK|MB_ICONERROR);

			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BK_PIC_ID);
			if (pEdit != NULL)
			{
				pEdit->SetSel(0, -1);
				pEdit->SetFocus();
			}

			return;
		}
	}

	m_strEditNewValue.TrimLeft(_T(" "));
	if (_T("") == m_strEditNewValue)
	{
		MessageBox(_T("点名不能为空！"), _T("错误"), MB_OK|MB_ICONERROR);
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDITNEWVALUE));
		if (pEdit != NULL)
		{
			pEdit->SetSel(0, -1);
			pEdit->SetFocus();
		}
		return;
	}

	m_strWebAddr = GetWebComboValue();

	CDialog::OnOK();
}

void CSetUpDataDlg::OnBnClickedButtonChooseBkPic()
{
	// TODO: 在此添加控件通知处理程序代码
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
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		if (m_pCanvasDlg != NULL)
		{
			int nPic = 0;
			m_pCanvasDlg->InsertImageIntoOpenS3db(dlg.m_nID, nPic);
			m_nPicID = nPic;
			m_pPage->SetBKImageID(m_nPicID);
			pFrame->m_nSelectPicID = m_nPicID;
			SetDlgItemInt(IDC_EDIT_BK_PIC_ID, m_nPicID);

			m_bBackgroundImageModified =true;
		}
	}
}

BOOL CSetUpDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ASSERT(m_pPage);

	m_combo_page_size.AddString(L"1920*955");
	m_combo_page_size.AddString(L"1366*680");

	m_nStartColor = m_pPage->GetStartColor();
	m_nEndColor = m_pPage->GetEndColor();
	m_nPicID = m_pPage->GetBKImageID();
	m_nWidth = m_pPage->GetPageWidth();
	m_nHeight = m_pPage->GetPageHeight();

	if(m_pPage->GetPageMinRight()>=0)
		m_nPageMinRight = m_pPage->GetPageMinRight();
	else
		m_nPageMinRight = 0;

	if (m_nWidth == 1920)
	{
		m_combo_page_size.SetCurSel(0);
		m_nHeight = 955;
	}
	else if (m_nWidth == 1366)
	{
		m_combo_page_size.SetCurSel(1);
		m_nHeight = 680;
	}
	if (m_pPage)
	{
		m_bObserverTopShow = m_pPage->GetPageShowTopInObserver();
	}
	SetDlgItemInt(IDC_EDIT_BK_PIC_ID,m_nPicID);
	SetDlgItemInt(IDC_EDIT_PAGE_WIDTH,m_nWidth);
	SetDlgItemInt(IDC_EDIT_PAGE_HEIGHT,m_nHeight);

	if (m_bObserverTopShow)
	{
		m_checkShow.SetCheck(1);
	}
	else
	{
		m_checkShow.SetCheck(0);
	}
	if (m_nPageType == FULLSCREEN)
	{
		GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(TRUE);
		//m_editWebAddr.EnableWindow(FALSE);
		m_comboWebAddr.EnableWindow(FALSE);
		m_nPageTp = 0;
	}
	else if (m_nPageType == FLOATING)
	{
		GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(FALSE);
		m_checkShow.SetCheck(BST_UNCHECKED);
		//m_editWebAddr.EnableWindow(FALSE);
		m_comboWebAddr.EnableWindow(FALSE);
		m_nPageTp = 1;
	}
	else if (m_nPageType == WEBTYPE)
	{
		GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(TRUE);
		//m_editWebAddr.EnableWindow(TRUE);
		m_comboWebAddr.EnableWindow(TRUE);
		m_nPageTp = 2;
	}
	else if (m_nPageType == TEMPLATE)
	{
		GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEMPLATE_VARS_DEFINE)->ShowWindow(TRUE);
		m_comboWebAddr.EnableWindow(FALSE);
		m_nPageTp = 3;
	}
	InitWebComboBox();
	SetWebComboValue(m_strWebAddr);

	m_pMainFrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
	if (m_pMainFrm != NULL)
	{
		CBEOPDesignerView* pView = (CBEOPDesignerView*)m_pMainFrm->GetActiveView();
		m_pCanvasDlg = &(pView->m_PageDlg->m_dlgCanvas);
	}

	if (0 == m_nPicID)
	{
		m_bEnableBk = FALSE;
	}
	else
	{
		m_bEnableBk = TRUE;
	}
	UpdateData(FALSE);
	SetDlgItemInt(IDC_EDIT_BK_PIC_ID, m_nPicID);
	EnableBackgroundControls(m_bEnableBk);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CSetUpDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	HBRUSH hBrush = NULL;

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_COLOR_START)
	{
		hBrush = CreateSolidBrush(m_nStartColor);
		return hBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_COLOR_END)
	{
		hBrush = CreateSolidBrush(m_nEndColor);
		return hBrush;
	}
	return hbr;
}


void CSetUpDataDlg::OnStnClickedStaticColorStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_nStartColor = dlg.GetColor();
		m_pPage->SetStartColor(m_nStartColor);
		Invalidate();
	}
}


void CSetUpDataDlg::OnStnClickedStaticColorEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_nEndColor = dlg.GetColor();
		m_pPage->SetEndColor(m_nEndColor);
		Invalidate();
	}
}

void CSetUpDataDlg::SetPage( CEqmDrawPage* pPage )
{
	m_pPage = pPage;
}


void CSetUpDataDlg::OnBnClickedRadioFixedpage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPageType = FULLSCREEN;
	GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TEMPLATE_VARS_DEFINE)->ShowWindow(FALSE);
	//m_editWebAddr.EnableWindow(FALSE);
	m_comboWebAddr.EnableWindow(FALSE);

	m_combo_page_size.SetCurSel(0);
	OnCbnSelchangeComboPageSize();

	m_nPageTp = 0;
	UpdateData(FALSE);
}


void CSetUpDataDlg::OnBnClickedRadioFlottingpage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPageType = FLOATING;
	GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TEMPLATE_VARS_DEFINE)->ShowWindow(FALSE);
	m_checkShow.SetCheck(BST_UNCHECKED);
	//m_editWebAddr.EnableWindow(FALSE);
	m_comboWebAddr.EnableWindow(FALSE);

	m_nPageTp = 1;
	UpdateData(FALSE);
}


void CSetUpDataDlg::OnBnClickedRadioWebpage()
{
	// TODO: Add your control notification handler code here
	m_nPageType = WEBTYPE;
	GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TEMPLATE_VARS_DEFINE)->ShowWindow(FALSE);
	m_checkShow.SetCheck(BST_CHECKED);
	//m_editWebAddr.EnableWindow(TRUE);
	m_comboWebAddr.EnableWindow(TRUE);

	m_nPageTp = 2;
	UpdateData(FALSE);
}


void CSetUpDataDlg::OnCbnSelchangeComboPageSize()
{
	if (m_combo_page_size.GetCurSel() == 0)
	{
		GetDlgItem(IDC_EDIT_PAGE_WIDTH)->SetWindowText(L"1920");
		GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->SetWindowText(L"955");
	}
	if (m_combo_page_size.GetCurSel() == 1)
	{
		GetDlgItem(IDC_EDIT_PAGE_WIDTH)->SetWindowText(L"1366");
		GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->SetWindowText(L"680");
	}
}


void CSetUpDataDlg::OnBnClickedCheckBk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	EnableBackgroundControls(m_bEnableBk);
	if (!m_bEnableBk)
	{
		m_nPicID = 0;
		SetDlgItemInt(IDC_EDIT_BK_PIC_ID, 0);
	}
}

void CSetUpDataDlg::EnableBackgroundControls(const BOOL bFlag)
{
	GetDlgItem(IDC_EDIT_BK_PIC_ID)->EnableWindow(bFlag);
	GetDlgItem(IDC_BUTTON_CHOOSE_BK_PIC)->EnableWindow(bFlag);
}

void CSetUpDataDlg::InitWebComboBox(void)
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
}

CString	CSetUpDataDlg::GetWebComboValue(void) const
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

void CSetUpDataDlg::SetWebComboValue(const CString strWebShow)
{
	if (strWebShow == m_strWebValue[0])
	{
		m_comboWebAddr.SetCurSel(0);
	} 
	else if (strWebShow == m_strWebValue[1])
	{
		m_comboWebAddr.SetCurSel(1);
	}
	else if (strWebShow == m_strWebValue[2])
	{
		m_comboWebAddr.SetCurSel(2);
	}
	else
	{
		m_comboWebAddr.SetCurSel(0);
	}
}


void CSetUpDataDlg::OnBnClickedRadioTemplatePage()
{
	// TODO: Add your control notification handler code here
	m_nPageType = TEMPLATE;
	GetDlgItem(IDC_COMBO_PAGE_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PAGE_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PAGE_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SHOW)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_TEMPLATE_VARS_DEFINE)->ShowWindow(TRUE);
	m_checkShow.SetCheck(BST_CHECKED);
	//m_editWebAddr.EnableWindow(TRUE);
	m_comboWebAddr.EnableWindow(FALSE);

	m_nPageTp = 3;
	UpdateData(FALSE);
}
