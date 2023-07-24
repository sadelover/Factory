// MetaButtonSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MetaButtonSelectDlg.h"
#include "afxdialogex.h"
#include "MetaFileSelectDlg.h"
#include "MainFrm.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "../ComponentDraw/EqmDrawProject.h"
#include "DataComConfigDlg.h"
#include "RelationItemSettingDlg.h"
#include "Tools/CustomTools/CustomTools.h"
#include "CanvasDlg.h"
// CMetaButtonSelectDlg 对话框

IMPLEMENT_DYNAMIC(CMetaButtonSelectDlg, CDialog)

CMetaButtonSelectDlg::CMetaButtonSelectDlg(BOOL bIsCreate, CWnd* pParent /*=NULL*/)
	: CDialog(CMetaButtonSelectDlg::IDD, pParent)
	,m_nIDPicComm(-1)
	,m_nIDPicOver(-1)
	,m_nIDPicDown(-1)
	,m_nIDPicDisable(-1)
	,m_nLinkPageID(-1)
	,m_nICPicChecked(-1)
	,m_pBitmapComm(NULL)
	,m_pBitmapOver(NULL)
	,m_pBitmapDown(NULL)
	,m_pBitmapDisable(NULL)
	,m_pProjectSqlite(NULL)
	,m_pBitmapChecked(NULL)
	,m_nSelectedPicId(-1)
	,m_bLBottonDown(false)
	,m_pCanvasDlg((CCanvasDlg*)pParent)
	,m_nBtnTop1(0)
	,m_nBtnTop2(0)
	,m_nBtnTop3(0)
	,m_nBtnTop4(0)
	,m_nBtnTop5(0)
	,m_strDownloadEnable(_T(""))
	,m_strDownloadName(_T(""))
	,m_bIsCreate(bIsCreate)
{
	m_nSettingValue = _T("0");
	m_nRelation = 0;
	m_strTextContent = L"开启";
	m_nTextColor = RGB(255,255,255);
	m_nTextSize = 22;
	m_nLayer = (int)E_LAYER_5;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pProjectSqlite = pFrame->m_pPrjSqlite;

	int nYSpace = 75;
	m_nBtnTop1 = 32;
	m_nBtnTop2 = m_nBtnTop1 + nYSpace;
	m_nBtnTop3 = m_nBtnTop1 + 2 * nYSpace;
	m_nBtnTop4 = m_nBtnTop1 + 3 * nYSpace;
	m_nBtnTop5 = m_nBtnTop1 + 4 * nYSpace;
}

CMetaButtonSelectDlg::~CMetaButtonSelectDlg()
{
}

void CMetaButtonSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LINK_PAGE, m_combo_link_page);
	DDX_Control(pDX, IDC_COMBO_META_BUTTON_LAYER, m_combo_layer);
	DDX_Control(pDX, IDC_LIST_RELATION, m_list_relation);
	DDX_Control(pDX, IDC_COMBO_RELATION, m_combo_relation);
	DDX_Text(pDX, IDC_EDIT_DL_ENABLE, m_strDownloadEnable);
	DDX_Text(pDX, IDC_EDIT_DL_NAME, m_strDownloadName);
	DDX_Control(pDX, IDC_COMBO_PRE_COMBINE, m_comboPreCombine);
}


BEGIN_MESSAGE_MAP(CMetaButtonSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COMMON, &CMetaButtonSelectDlg::OnBnClickedButtonCommon)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CMetaButtonSelectDlg::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_OVER, &CMetaButtonSelectDlg::OnBnClickedButtonOver)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE, &CMetaButtonSelectDlg::OnBnClickedButtonDisable)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CMetaButtonSelectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_META_BUTTON_BIND_SET_POINT_SELECT, &CMetaButtonSelectDlg::OnBnClickedButtonMetaButtonBindSetPointSelect)
	ON_BN_CLICKED(IDC_BUTTON_META_BUTTON_BIND_SET_POINT_CLEAR, &CMetaButtonSelectDlg::OnBnClickedButtonMetaButtonBindSetPointClear)
	ON_STN_CLICKED(IDC_STATIC_TEXT_COLOR, &CMetaButtonSelectDlg::OnStnClickedStaticTextColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RELATION_ADD, &CMetaButtonSelectDlg::OnBnClickedButtonRelationAdd)
	ON_BN_CLICKED(IDC_BUTTON_RELATION_DELETE, &CMetaButtonSelectDlg::OnBnClickedButtonRelationDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RELATION, &CMetaButtonSelectDlg::OnNMDblclkListRelation)
	ON_BN_CLICKED(IDC_BUTTON_CHECKED, &CMetaButtonSelectDlg::OnBnClickedButtonChecked)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_DL_ENABLE_BROWSE, &CMetaButtonSelectDlg::OnBnClickedBtnDlEnableBrowse)
	ON_BN_CLICKED(IDC_BTN_DL_NAME_BROWSE, &CMetaButtonSelectDlg::OnBnClickedBtnDlNameBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_PRE_COMBINE, &CMetaButtonSelectDlg::OnCbnSelchangeComboPreCombine)
	ON_EN_CHANGE(IDC_EDIT_DL_ENABLE, &CMetaButtonSelectDlg::OnEnChangeEditDlEnable)
	ON_EN_CHANGE(IDC_EDIT_DL_NAME, &CMetaButtonSelectDlg::OnEnChangeEditDlName)
	ON_CBN_SELCHANGE(IDC_COMBO_LINK_PAGE, &CMetaButtonSelectDlg::OnCbnSelchangeComboLinkPage)
END_MESSAGE_MAP()


// CMetaButtonSelectDlg 消息处理程序

//选择显示
void CMetaButtonSelectDlg::OnBnClickedButtonCommon()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	dlg.SetDivideMode(true);
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		if (m_pCanvasDlg != NULL)
		{
			int nPic = 0;
			m_pCanvasDlg->InsertImageIntoOpenS3db(dlg.m_nID, nPic);
			m_nIDPicComm = nPic;
			pFrame->m_nSelectPicID = m_nIDPicComm;
		}
		else
		{
			m_nIDPicComm = dlg.m_nID;
			pFrame->m_nSelectPicID = m_nIDPicComm;
		}
		Invalidate();
	}
}


void CMetaButtonSelectDlg::OnBnClickedButtonDown()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	dlg.SetDivideMode(true);
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		if (m_pCanvasDlg != NULL)
		{
			int nPic = 0;
			m_pCanvasDlg->InsertImageIntoOpenS3db(dlg.m_nID, nPic);
			m_nIDPicDown = nPic;
			pFrame->m_nSelectPicID = m_nIDPicDown;
		}
		else
		{
			m_nIDPicComm = dlg.m_nID;
			pFrame->m_nSelectPicID = m_nIDPicComm;
		}
		Invalidate();
	}
}


void CMetaButtonSelectDlg::OnBnClickedButtonOver()
{
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	dlg.SetDivideMode(true);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		if (m_pCanvasDlg != NULL)
		{
			int nPic = 0;
			m_pCanvasDlg->InsertImageIntoOpenS3db(dlg.m_nID, nPic);
			m_nIDPicOver = nPic;
			pFrame->m_nSelectPicID = m_nIDPicOver;
		}
		else
		{
			m_nIDPicComm = dlg.m_nID;
			pFrame->m_nSelectPicID = m_nIDPicComm;
		}
		Invalidate();
	}
}


void CMetaButtonSelectDlg::OnBnClickedButtonDisable()
{
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	dlg.SetDivideMode(true);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		if (m_pCanvasDlg != NULL)
		{
			int nPic = 0;
			m_pCanvasDlg->InsertImageIntoOpenS3db(dlg.m_nID, nPic);
			m_nIDPicDisable = nPic;
			pFrame->m_nSelectPicID = m_nIDPicDisable;
		}
		else
		{
			m_nIDPicComm = dlg.m_nID;
			pFrame->m_nSelectPicID = m_nIDPicComm;
		}
		Invalidate();
	}
}

void CMetaButtonSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Gdiplus::Graphics graphics(dc.m_hDC);
	Gdiplus::Pen penred(Gdiplus::Color(255, 0, 0));
	penred.SetWidth(2.0);
	Gdiplus::Pen penblue(Gdiplus::Color(0, 0, 255));
	if (m_nIDPicComm != -1)
	{
		m_pBitmapComm = m_pProjectSqlite->GetBitmapByIDFromPictureLib(m_nIDPicComm, true);
		if (m_pBitmapComm)
		{
			graphics.DrawImage(m_pBitmapComm, 250, m_nBtnTop1, 100, 50);
			if (m_bLBottonDown)
				graphics.DrawRectangle(&penblue,250-5,m_nBtnTop1-5,100+10,50+10);
		}
		if (m_nIDPicComm == m_nSelectedPicId)
		{
			graphics.DrawRectangle(&penred,250-5,m_nBtnTop1-5,100+10,50+10);
		}
	}
	if (m_nIDPicDown != -1)
	{
		m_pBitmapDown = m_pProjectSqlite->GetBitmapByIDFromPictureLib(m_nIDPicDown, true);
		if (m_pBitmapDown)
		{
			graphics.DrawImage(m_pBitmapDown, 250, m_nBtnTop2, 100, 50);
			if (m_bLBottonDown)
				graphics.DrawRectangle(&penblue,250-5,m_nBtnTop2-5,100+10,50+10);
		}
		if (m_nIDPicDown == m_nSelectedPicId)
		{
			graphics.DrawRectangle(&penred,250-5,m_nBtnTop2-5,100+10,50+10);
		}
	}
	if (m_nIDPicOver != -1)
	{
		m_pBitmapOver = m_pProjectSqlite->GetBitmapByIDFromPictureLib(m_nIDPicOver, true);
		if (m_pBitmapOver)
		{
			graphics.DrawImage(m_pBitmapOver, 250, m_nBtnTop3, 100, 50);
			if (m_bLBottonDown)
				graphics.DrawRectangle(&penblue,250-5,m_nBtnTop3-5,100+10,50+10);
		}
		if (m_nIDPicOver == m_nSelectedPicId)
		{
			graphics.DrawRectangle(&penred,250-5,m_nBtnTop3-5,100+10,50+10);
		}
	}
	if (m_nIDPicDisable != -1)
	{
		m_pBitmapDisable = m_pProjectSqlite->GetBitmapByIDFromPictureLib(m_nIDPicDisable, true);
		if (m_pBitmapDisable)
		{
			graphics.DrawImage(m_pBitmapDisable, 250, m_nBtnTop4, 100, 50);
			if (m_bLBottonDown)
				graphics.DrawRectangle(&penblue,250-5,m_nBtnTop4-5,100+10,50+10);
		}
		if (m_nIDPicDisable == m_nSelectedPicId)
		{
			graphics.DrawRectangle(&penred,250-5,m_nBtnTop4-5,100+10,50+10);
		}
	}
	if (m_nICPicChecked != -1)
	{
		m_pBitmapChecked = m_pProjectSqlite->GetBitmapByIDFromPictureLib(m_nICPicChecked, true);
		if (m_pBitmapChecked)
		{
			graphics.DrawImage(m_pBitmapChecked, 250, m_nBtnTop5, 100, 50);
			if (m_bLBottonDown)
				graphics.DrawRectangle(&penblue,250-5,m_nBtnTop5-5,100+10,50+10);
		}
		if (m_nICPicChecked == m_nSelectedPicId)
		{
			graphics.DrawRectangle(&penred,250-5,m_nBtnTop5-5,100+10,50+10);
		}
	}
	graphics.ReleaseHDC(dc.m_hDC);
}


BOOL CMetaButtonSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	InitComboRelation();
	m_combo_link_page.AddString(L"不指定");
	int nIndex = -1;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CEqmDrawProject& project = pFrame->m_project;
	vector<CEqmDrawPage*>& page_list = project.GetPageList();
	for (size_t i=0;i<page_list.size();++i)
	{
		if (page_list[i])
		{
			const CString strPageName = page_list[i]->GetPageName().c_str();
			const int nPageID = page_list[i]->GetID();
			if (nPageID == m_nLinkPageID)
			{
				nIndex = i;
			}
			m_combo_link_page.AddString(strPageName);
		}
	}
	m_combo_link_page.SetCurSel(nIndex+1);

	m_combo_layer.AddString(L"1层");
	m_combo_layer.AddString(L"2层");
	m_combo_layer.AddString(L"3层");
	m_combo_layer.AddString(L"4层");
	m_combo_layer.AddString(L"5层");
	m_combo_layer.AddString(L"6层");
	m_combo_layer.AddString(L"7层");
	m_combo_layer.AddString(L"8层");
	m_combo_layer.AddString(L"9层");
	m_combo_layer.AddString(L"10层");
	m_combo_layer.SetCurSel(m_nLayer);

	if (m_strBindSetPointName.GetLength() > 0)
	{
		GetDlgItem(IDC_EDIT_META_BUTTON_BIND_SET_POINT)->SetWindowTextW(m_strBindSetPointName);
	}
	if (m_strTextContent.GetLength() > 0)
	{
		GetDlgItem(IDC_EDIT_TEXT_CONTENT)->SetWindowTextW(m_strTextContent);
	}
	if (m_strOperation.GetLength() > 0)
	{
		GetDlgItem(IDC_EDIT_OPERATION)->SetWindowText(m_strOperation);
	}
	CString strTextSize;
	strTextSize.Format(L"%d",m_nTextSize);
	GetDlgItem(IDC_EDIT_TEXT_SIZE)->SetWindowText(strTextSize);
	CString strSettingValue;
	strSettingValue.Format(L"%s",m_nSettingValue);
	GetDlgItem(IDC_EDIT_SETTING_VALUE)->SetWindowText(strSettingValue);
	m_combo_relation.SetCurSel(m_nRelation);
	const size_t size_relation = m_vec.size();
	for (size_t n=0;n<size_relation;++n)
	{
		CString strTemp;
		m_list_relation.InsertItem(n,m_vec[n].strPointName);
		const int nRelationExpress = m_vec[n].nRelationExpress;
		strTemp = GetRelationTextByInt(nRelationExpress);
		m_list_relation.SetItemText(n,1,strTemp);
		const double fResult = m_vec[n].fResult;
		strTemp.Format(L"%.2f",fResult);
		m_list_relation.SetItemText(n,2,strTemp);
	}

	m_comboPreCombine.AddString(_T("普通红色按钮"));
	m_comboPreCombine.AddString(_T("普通黄色按钮"));
	m_comboPreCombine.AddString(_T("普通绿色按钮"));
	LoadPreButtonImage();
	if (m_bIsCreate)
	{
		m_comboPreCombine.SetCurSel(0);
		SetShowPicCombination(0);
	}
	else
	{
		m_comboPreCombine.SetCurSel(-1);
	}
	ASSERT(m_pCanvasDlg != NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMetaButtonSelectDlg::OnBnClickedOk()//ok
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nIDPicComm == -1)
	{
		AfxMessageBox(L"默认的图片没设置，请设置Common图片!");
		return;
	}
	if (m_nIDPicOver == -1)
	{
		AfxMessageBox(L"鼠标移上去的图片没设置，请设置Over图片!");
		return;
	}
	if (m_nIDPicDown == -1)
	{
		AfxMessageBox(L"鼠标按下的图片没设置，请设置Down图片!");
		return;
	}
	if (m_nIDPicDisable == -1)
	{
		AfxMessageBox(L"按钮禁用的图片没设置，请设置Disable图片!");
		return;
	}
	if (m_nICPicChecked == -1)
	{
		AfxMessageBox(L"按钮选中的图片没设置，请设置Checked图片!");
		return;
	}
	m_nLayer = m_combo_layer.GetCurSel();
	CString strTemp;
	GetDlgItem(IDC_EDIT_TEXT_CONTENT)->GetWindowTextW(strTemp);

	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (pMainFrm != NULL)
	{
		int nLen = strTemp.GetLength();
		if (nLen > pMainFrm->m_nItemNameLen)
		{
			CString strErr;
			strErr.Format(_T("名称长度不能大于%d！"), pMainFrm->m_nItemNameLen);
			MessageBox(strErr, _T("错误"), MB_OK|MB_ICONERROR);
			CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_TEXT_CONTENT));
			if (pEdit != NULL)
			{
				pEdit->SetSel(0, -1);
				pEdit->SetFocus();
			}
			return;
		}
	}
	m_strTextContent = strTemp;

	GetDlgItem(IDC_EDIT_TEXT_SIZE)->GetWindowTextW(strTemp);
	if (strTemp.GetLength() > 0)
	{
		m_nTextSize = _wtoi(strTemp.GetString());
	}
	GetDlgItem(IDC_EDIT_OPERATION)->GetWindowText(strTemp);
	m_strOperation = strTemp;
	GetDlgItem(IDC_EDIT_SETTING_VALUE)->GetWindowTextW(strTemp);
	if (m_strBindSetPointName.GetLength() > 0)
	{
		if (strTemp.GetLength() == 0)
		{
			AfxMessageBox(L"请设置设定值!");
			return;
		}
	}
	m_nRelation = m_combo_relation.GetCurSel();
	/*m_nSettingValue = _wtoi(strTemp.GetString());*/
	m_nSettingValue = strTemp.GetString();
	const int nCount = m_list_relation.GetItemCount();
	m_vec.clear();
	for (int k=0;k<nCount;++k)
	{
		_tagRelationExpress express;
		express.strPointName = m_list_relation.GetItemText(k,0);
		express.nRelationExpress = GetIntRelationByText(m_list_relation.GetItemText(k,1));
		express.fResult = _wtof(m_list_relation.GetItemText(k,2));
		m_vec.push_back(express);
	}
	CString strText;
	m_combo_link_page.GetLBText(m_combo_link_page.GetCurSel(),strText);
	if (strText == L"不指定")
	{
		m_nLinkPageID = -1;
	}
	else
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CEqmDrawProject& project = pFrame->m_project;
		m_nLinkPageID = project.GetPageIDByPageName(strText.GetString());
	}

	CDialog::OnOK();
}
//选择控件
void CMetaButtonSelectDlg::OnBnClickedButtonMetaButtonBindSetPointSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_META_BUTTON_BIND_SET_POINT)->SetWindowTextW(dlg.m_strPointName);
		m_strBindSetPointName = dlg.m_strPointName;
	}
}

void CMetaButtonSelectDlg::OnBnClickedButtonMetaButtonBindSetPointClear()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_META_BUTTON_BIND_SET_POINT)->SetWindowTextW(L"");
	m_strBindSetPointName = L"";
}

//文本颜色
void CMetaButtonSelectDlg::OnStnClickedStaticTextColor()
{
	// TODO: 在此添加控件通知处理程序代码
	COLORREF cusColor[16];
	memset( cusColor, 0, sizeof( COLORREF) * 16 );
	CString strSys;
	GetSysPath(strSys);
	strSys += L"\\factory.ini";
	for (int i=0;i<16;++i)
	{
		CString strTemp;
		strTemp.Format(L"usercolor%02d",i+1);
		cusColor[i] = GetPrivateProfileInt(L"USER",strTemp,0,strSys);
	}
	CHOOSECOLOR cc = {sizeof( CHOOSECOLOR ),m_hWnd,NULL,m_nTextColor,cusColor,CC_FULLOPEN,NULL,NULL,NULL};
	if (ChooseColor(&cc))
	{
		for (int i=0;i<16;++i)
		{
			CString strTemp;
			strTemp.Format(L"usercolor%02d",i+1);
			CString strColor;
			strColor.Format(L"%d",cusColor[i]);
			WritePrivateProfileString(L"USER",strTemp,strColor,strSys);
		}
		m_nTextColor = cc.rgbResult;
		Invalidate();
	}
}


HBRUSH CMetaButtonSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_nTextColor);
		if (hBrush)
		{
			return hBrush;
		}
	}
	return hbr;
}

void CMetaButtonSelectDlg::InitList()
{
	m_list_relation.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_list_relation.InsertColumn(0,L"绑定点",LVCFMT_LEFT, 80);
	m_list_relation.InsertColumn(1,L"逻辑关系",LVCFMT_LEFT, 80);
	m_list_relation.InsertColumn(2,L"匹配值",LVCFMT_LEFT, 60);
}

void CMetaButtonSelectDlg::InitComboRelation()
{
	m_combo_relation.AddString(L"与");
	m_combo_relation.AddString(L"或");
}

CString CMetaButtonSelectDlg::GetRelationTextByInt( const int nInt )
{
	CString strText;
	switch (nInt)
	{
	case 0:
		strText = L"等于";
		break;
	case 1:
		strText = L"大于";
		break;
	case 2:
		strText = L"小于";
		break;
	case 3:
		strText = L"大于等于";
		break;
	case 4:
		strText = L"小于等于";
		break;
	case 5:
		strText = L"不等于";
		break;
	default:
		strText = L"等于";
		break;
	}
	return strText;
}

int CMetaButtonSelectDlg::GetIntRelationByText( const CString strText )
{
	if (strText == L"等于")
	{
		return 0;
	}
	else if (strText == L"大于")
	{
		return 1;
	}
	else if (strText == L"小于")
	{
		return 2;
	}
	else if (strText == L"大于等于")
	{
		return 3;
	}
	else if (strText == L"小于等于")
	{
		return 4;
	}
	else if (strText == L"不等于")
	{
		return 5;
	}
	return 0;
}


void CMetaButtonSelectDlg::OnBnClickedButtonRelationAdd()
{
	const int nCount = m_list_relation.GetItemCount();
	m_list_relation.InsertItem(nCount,L"绑定点");
	m_list_relation.SetItemText(nCount,1,L"等于");
	m_list_relation.SetItemText(nCount,2,L"0");
}


void CMetaButtonSelectDlg::OnBnClickedButtonRelationDelete()
{
	if (AfxMessageBox(L"确认要删除吗?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_list_relation.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_list_relation.GetNextSelectedItem(pos);
			vec.push_back(nItem);
		}
		for (int i=vec.size()-1;i>=0;--i)
		{
			m_list_relation.DeleteItem(vec[i]);
		}
	}
}


void CMetaButtonSelectDlg::OnNMDblclkListRelation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CRelationItemSettingDlg dlg;
	dlg.m_strPointName = m_list_relation.GetItemText(pNMItemActivate->iItem,0);
	dlg.m_nRelation = GetIntRelationByText(m_list_relation.GetItemText(pNMItemActivate->iItem,1));
	dlg.m_fResult = _wtof(m_list_relation.GetItemText(pNMItemActivate->iItem,2));
	if (dlg.DoModal() == IDOK)
	{
		m_list_relation.SetItemText(pNMItemActivate->iItem,0,dlg.m_strPointName);
		m_list_relation.SetItemText(pNMItemActivate->iItem,1,GetRelationTextByInt(dlg.m_nRelation));
		CString strTemp;
		strTemp.Format(L"%.2f",dlg.m_fResult);
		m_list_relation.SetItemText(pNMItemActivate->iItem,2,strTemp);
	}
	*pResult = 0;
}


void CMetaButtonSelectDlg::OnBnClickedButtonChecked()
{
	CMetaFileSelectDlg dlg(this);
	dlg.SetWndType(e_picture);
	dlg.SetDivideMode(true);
	if (dlg.DoModal() == IDOK)
	{
		if (m_pCanvasDlg != NULL)
		{
			int nPic = 0;
			m_pCanvasDlg->InsertImageIntoOpenS3db(dlg.m_nID, nPic);
			m_nICPicChecked = nPic;
		}
		else
		{
			m_nIDPicComm = dlg.m_nID;
		}
		Invalidate();
	}
}


void CMetaButtonSelectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	if (!m_bLBottonDown)
	{
		if (m_nIDPicComm != -1 && m_nSelectedPicId == -1)
		{
			if (point.x>=250 && point.x<=250+100 && point.y>=m_nBtnTop1 && point.y<=m_nBtnTop1+50)
			{
				m_nSelectedPicId = m_nIDPicComm;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
				m_bLBottonDown = true;
			}
			else
			{
				m_bLBottonDown = false;
				m_nSelectedPicId = -1;
			}
		}
		if (m_nIDPicDown != -1 && m_nSelectedPicId == -1)
		{
			if (point.x>=250 && point.x<=250+100 && point.y>=m_nBtnTop2 && point.y<=m_nBtnTop2+50)
			{
				m_nSelectedPicId = m_nIDPicDown;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
				m_bLBottonDown = true;
			}
			else
			{
				m_bLBottonDown = false;
				m_nSelectedPicId = -1;
			}
		}
		if (m_nIDPicOver != -1 && m_nSelectedPicId == -1)
		{
			if (point.x>=250 && point.x<=250+100 && point.y>=m_nBtnTop3 && point.y<=m_nBtnTop3+50)
			{
				m_nSelectedPicId = m_nIDPicOver;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
				m_bLBottonDown = true;
			}
			else
			{
				m_bLBottonDown = false;
				m_nSelectedPicId = -1;
			}
		}
		if (m_nIDPicDisable != -1 && m_nSelectedPicId == -1)
		{
			if (point.x>=250 && point.x<=250+100 && point.y>=m_nBtnTop4 && point.y<=m_nBtnTop4+50)
			{
				m_nSelectedPicId = m_nIDPicDisable;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
				m_bLBottonDown = true;
			}
			else
			{
				m_bLBottonDown = false;
				m_nSelectedPicId = -1;
			}
		}
		if (m_nICPicChecked != -1 && m_nSelectedPicId == -1)
		{
			if (point.x>=250 && point.x<=250+100 && point.y>=m_nBtnTop5 && point.y<=m_nBtnTop5+50)
			{
				m_nSelectedPicId = m_nICPicChecked;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
				m_bLBottonDown = true;
			}
			else
			{
				m_bLBottonDown = false;
				m_nSelectedPicId = -1;
			}
		}
		Invalidate();
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CMetaButtonSelectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();
	if (m_bLBottonDown && m_nSelectedPicId != -1)
	{
		if (point.x>=250 && point.x<=250+100 && point.y>=18 && point.y<=18+50)
		{
			m_nIDPicComm = m_nSelectedPicId;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		else if (point.x>=250 && point.x<=250+100 && point.y>=105 && point.y<=105+50)
		{
			m_nIDPicDown = m_nSelectedPicId;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		else if (point.x>=250 && point.x<=250+100 && point.y>=198 && point.y<=198+50)
		{
			m_nIDPicOver = m_nSelectedPicId;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		else if (point.x>=250 && point.x<=250+100 && point.y>=283 && point.y<=283+50)
		{
			m_nIDPicDisable = m_nSelectedPicId;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		else if (point.x>=250 && point.x<=250+100 && point.y>=370 && point.y<=370+50)
		{
			m_nICPicChecked = m_nSelectedPicId;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		m_bLBottonDown = false;
		m_nSelectedPicId = -1;
		Invalidate();
	}
	CDialog::OnLButtonUp(nFlags, point);
}


void CMetaButtonSelectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bLBottonDown && m_nSelectedPicId != -1)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	CDialog::OnMouseMove(nFlags, point);
}


void CMetaButtonSelectDlg::OnBnClickedBtnDlEnableBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}
	CDataComConfigDlg dlg(pMainFrm->m_plcpointmap);
	if (IDOK == dlg.DoModal())
	{
		m_strDownloadEnable = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CMetaButtonSelectDlg::OnBnClickedBtnDlNameBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm != NULL);
	if (NULL == pMainFrm)
	{
		return;
	}
	CDataComConfigDlg dlg(pMainFrm->m_plcpointmap);
	if (IDOK == dlg.DoModal())
	{
		m_strDownloadName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

void CMetaButtonSelectDlg::SetShowPicCombination(const int nType)
{
	vector<int> vecId;
	if (0 == nType)
	{	// red button
		InsertButtonGroupIntoOpenS3db(m_vecBtnRed, vecId);
	}
	else if (1 == nType)
	{	// yellow button
		InsertButtonGroupIntoOpenS3db(m_vecBtnYellow, vecId);
	}
	else if (2 == nType)
	{	// green button
		InsertButtonGroupIntoOpenS3db(m_vecBtnGreen, vecId);
	}
	else
	{
		return;
	}

	if (vecId.size() >= 5)
	{
		m_nIDPicComm	= vecId.at(0);
		m_nICPicChecked	= vecId.at(1);
		m_nIDPicOver	= vecId.at(2);
		m_nIDPicDisable	= vecId.at(3);
		m_nIDPicDown	= vecId.at(4);
		Invalidate();
	}
}


void CMetaButtonSelectDlg::OnCbnSelchangeComboPreCombine()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSel = m_comboPreCombine.GetCurSel();
	if (CB_ERR == nCurSel)
	{
		return;
	}
	SetShowPicCombination(nCurSel);
}


void CMetaButtonSelectDlg::OnEnChangeEditDlEnable()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CMetaButtonSelectDlg::OnEnChangeEditDlName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CMetaButtonSelectDlg::LoadPreButtonImage(void)
{
	CString	strSysPath;
	GetSysPath(strSysPath);
	strSysPath += _T("\\Images\\");

	CString	arrNameRed[5];
	CString arrPathRed[5];
	arrNameRed[0] = _T("btn_style1_common.png");
	arrNameRed[1] = _T("btn_style1_checked.png");
	arrNameRed[2] = _T("btn_style1_over.png");
	arrNameRed[3] = _T("btn_style1_disable.png");
	arrNameRed[4] = _T("btn_style1_down.png");
	arrPathRed[0] = strSysPath + arrNameRed[0];
	arrPathRed[1] = strSysPath + arrNameRed[1];
	arrPathRed[2] = strSysPath + arrNameRed[2];
	arrPathRed[3] = strSysPath + arrNameRed[3];
	arrPathRed[4] = strSysPath + arrNameRed[4];

	CString	arrNameYellow[5];
	CString arrPathYellow[5];
	arrNameYellow[0] = _T("btn_yellow_common.png");
	arrNameYellow[1] = _T("btn_yellow_checked.png");
	arrNameYellow[2] = _T("btn_yellow_over.png");
	arrNameYellow[3] = _T("btn_yellow_disable.png");
	arrNameYellow[4] = _T("btn_yellow_down.png");
	arrPathYellow[0] = strSysPath + arrNameYellow[0];
	arrPathYellow[1] = strSysPath + arrNameYellow[1];
	arrPathYellow[2] = strSysPath + arrNameYellow[2];
	arrPathYellow[3] = strSysPath + arrNameYellow[3];
	arrPathYellow[4] = strSysPath + arrNameYellow[4];

	CString	arrNameGreen[5];
	CString arrPathGreen[5];
	arrNameGreen[0] = _T("btn_green_common.png");
	arrNameGreen[1] = _T("btn_green_checked.png");
	arrNameGreen[2] = _T("btn_green_over.png");
	arrNameGreen[3] = _T("btn_green_disable.png");
	arrNameGreen[4] = _T("btn_green_down.png");
	arrPathGreen[0] = strSysPath + arrNameGreen[0];
	arrPathGreen[1] = strSysPath + arrNameGreen[1];
	arrPathGreen[2] = strSysPath + arrNameGreen[2];
	arrPathGreen[3] = strSysPath + arrNameGreen[3];
	arrPathGreen[4] = strSysPath + arrNameGreen[4];

	SetButtonGroupStruct(arrNameRed, arrPathRed, m_vecBtnRed);
	SetButtonGroupStruct(arrNameYellow, arrPathYellow, m_vecBtnYellow);
	SetButtonGroupStruct(arrNameGreen, arrPathGreen, m_vecBtnGreen);
}

void CMetaButtonSelectDlg::SetButtonGroupStruct(const CString arrName[5], const CString arrPath[5], vector<PictureInfo>& vecBtn)
{
	vector<PictureInfo>::const_iterator	iter;
	assert(m_pProjectSqlite != NULL);
	if (NULL == m_pProjectSqlite)
	{
		return;
	}

	vecBtn.clear();
	for (int i=0; i<5 ;i++)
	{
		FILE* pFile = NULL;
		const errno_t rs = _wfopen_s(&pFile, arrPath[i].GetString(), L"rb");
		assert(0 == rs);
		assert(pFile);
		fseek(pFile, 0, SEEK_END);
		const int file_size = ftell(pFile);
		assert(file_size > 0);
		char* pBuf = new char[file_size];
		memset(pBuf, 0, file_size);
		fseek(pFile, 0, SEEK_SET);
		fread(pBuf, sizeof(char), file_size, pFile);
		const int nPicID = m_pProjectSqlite->GetMaxLibImageNum(false)+1+INSERT_META_FILE_ID_START;
		char szFileName[MAX_PATH] = {0};
		size_t converted = 0;
		wcstombs_s(&converted, szFileName, sizeof(szFileName), arrName[i].GetString(), sizeof(szFileName));
		assert(converted > 0);
		fclose(pFile);

		{
			CMemFile   memfile;  
			memfile.Attach((BYTE*)(pBuf),file_size,1024);  
			memfile.SetLength(file_size); 
			HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, file_size);
			if (hMemBmp)
			{
				IStream* pStmBmp = NULL;
				if(CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
				{
					BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
					memfile.SeekToBegin();
					memfile.Read(pbyBmp, file_size);
					Image* pBitmap = Image::FromStream(pStmBmp);
					PictureInfo info;
					info.ID = nPicID;
					info.pBitmap = pBitmap;
					info.etype_equip = L"7";
					wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), arrName[i].GetString(), sizeof(info.szName)/sizeof(wchar_t)-1);
					info.nSize = file_size;
					info.pBlock = new char[file_size];
					if (info.pBlock != NULL)
					{
						memset(info.pBlock, 0, file_size);
						memcpy_s(info.pBlock, file_size, pBuf, file_size);
					}

					info.nBtnId = 0;
					info.nBtnSubId = i;
					vecBtn.push_back(info);

					GlobalUnlock(hMemBmp);
				}
				GlobalFree(hMemBmp);
				hMemBmp = NULL;
			}
		}
		SAFE_DELETE_ARRAY(pBuf);
	}
}

void CMetaButtonSelectDlg::InsertButtonGroupIntoOpenS3db(const vector<PictureInfo>& vecPicInf, vector<int>& vecId)
{
	if (NULL == m_pProjectSqlite)
	{
		return;
	}

	USES_CONVERSION;
	vector<PictureInfo>::const_iterator iter;
	int i = 0;
	for (iter=vecPicInf.begin(); iter!=vecPicInf.end(); iter++,i++)
	{
		PictureInfo* pSrcInfo = const_cast<PictureInfo*>(&(*iter));
		if (NULL == pSrcInfo)
		{
			continue;;
		}

		PictureInfo* pDstInfo = m_pProjectSqlite->GetPictureInfo(pSrcInfo->szName, pSrcInfo->nSize, pSrcInfo->remark, pSrcInfo);
		if (NULL == pDstInfo)
		{	// 打开的s3db没有该图，直接插入数据库
			int nNewId = m_pProjectSqlite->GetMaxPictureNum(true) + 1;
			m_pProjectSqlite->InsertPictureData(nNewId, T2A(pSrcInfo->szName), Project::Tools::WideCharToAnsi(pSrcInfo->etype_equip.c_str()).c_str(), pSrcInfo->pBlock, pSrcInfo->nSize, true, 0, 0, T2A((pSrcInfo->etype_p).c_str()));

			// 插入内存结构
			m_pProjectSqlite->AddIntoPictureList(nNewId, pSrcInfo);

			vecId.push_back(nNewId);
		}
		else
		{	// 打开的s3db有该图
			vecId.push_back(pDstInfo->ID);
		}
	}
}


void CMetaButtonSelectDlg::OnCbnSelchangeComboLinkPage()
{
	// TODO: Add your control notification handler code here
}
