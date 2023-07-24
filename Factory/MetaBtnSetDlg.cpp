// MetaBtnSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MetaBtnSetDlg.h"
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


// CMetaBtnSetDlg dialog

IMPLEMENT_DYNAMIC(CMetaBtnSetDlg, CDialogEx)

CMetaBtnSetDlg::CMetaBtnSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMetaBtnSetDlg::IDD, pParent)
	, m_nIDPicComm(-1)
	, m_nIDPicDown(-1)
	, m_nIDPicOver(-1)
	, m_nIDPicDisable(-1)
	, m_nICPicChecked(-1)
	, m_nLinkPageID(-1)
	, m_pBitmapComm(NULL)
	, m_pBitmapDown(NULL)
	, m_pBitmapOver(NULL)
	, m_pBitmapDisable(NULL)
	, m_pProjectSqlite(NULL)
	, m_pBitmapChecked(NULL)
	, m_nSelectedPicId(-1)
	, m_bLBottonDown(false)
	, m_pCanvasDlg((CCanvasDlg*)pParent)
	, m_pMainFrm(NULL)
	, m_nBtnStyleSel(-1)
	, m_nBtnCurGroup(0)
{
	m_nSettingValue = 0;
	m_nRelation = 0;
	m_strTextContent = L"开启";
	m_nTextColor = RGB(255,255,255);
	m_nTextSize = 22;
	m_nLayer = (int)E_LAYER_5;
	m_pMainFrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
	assert(m_pMainFrm != NULL);
	if (m_pMainFrm != NULL)
	{
		m_pProjectSqlite = m_pMainFrm->m_pPrjSqlite;
		assert(m_pProjectSqlite != NULL);
	}
}

CMetaBtnSetDlg::~CMetaBtnSetDlg()
{
}

void CMetaBtnSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LINK_PAGE, m_combo_link_page);
	DDX_Control(pDX, IDC_COMBO_META_BUTTON_LAYER, m_combo_layer);
	DDX_Control(pDX, IDC_LIST_RELATION, m_list_relation);
	DDX_Control(pDX, IDC_COMBO_RELATION, m_combo_relation);
	DDX_Control(pDX, IDC_BTN_STYLE_UP, m_btnStyleUp);
	DDX_Control(pDX, IDC_BTN_STYLE_NEXT, m_btnStyleNext);
}


BEGIN_MESSAGE_MAP(CMetaBtnSetDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CMetaBtnSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_META_BUTTON_BIND_SET_POINT_SELECT, &CMetaBtnSetDlg::OnBnClickedButtonMetaButtonBindSetPointSelect)
	ON_BN_CLICKED(IDC_BUTTON_META_BUTTON_BIND_SET_POINT_CLEAR, &CMetaBtnSetDlg::OnBnClickedButtonMetaButtonBindSetPointClear)
	ON_STN_CLICKED(IDC_STATIC_TEXT_COLOR, &CMetaBtnSetDlg::OnStnClickedStaticTextColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RELATION_ADD, &CMetaBtnSetDlg::OnBnClickedButtonRelationAdd)
	ON_BN_CLICKED(IDC_BUTTON_RELATION_DELETE, &CMetaBtnSetDlg::OnBnClickedButtonRelationDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RELATION, &CMetaBtnSetDlg::OnNMDblclkListRelation)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_STYLE_ADD, &CMetaBtnSetDlg::OnBnClickedBtnStyleAdd)
	ON_BN_CLICKED(IDC_BTN_STYLE_DEL, &CMetaBtnSetDlg::OnBnClickedBtnStyleDel)
	ON_BN_CLICKED(IDC_BTN_STYLE_UP, &CMetaBtnSetDlg::OnBnClickedBtnStyleUp)
	ON_BN_CLICKED(IDC_BTN_STYLE_NEXT, &CMetaBtnSetDlg::OnBnClickedBtnStyleNext)
	ON_EN_CHANGE(IDC_EDIT_TEXT_CONTENT, &CMetaBtnSetDlg::OnEnChangeEditTextContent)
	ON_EN_CHANGE(IDC_EDIT_TEXT_SIZE, &CMetaBtnSetDlg::OnEnChangeEditTextSize)
END_MESSAGE_MAP()


// CMetaBtnSetDlg message handlers

void CMetaBtnSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Gdiplus::Graphics graphics(dc.m_hDC);
	Gdiplus::Pen penred(Gdiplus::Color(255, 0, 0));
	penred.SetWidth(2.0);
	Gdiplus::Pen penblue(Gdiplus::Color(0, 0, 255));

	// for ensure rectangle position
	//graphics.DrawRectangle(&penblue, m_rtStyle[0]);
	//graphics.DrawRectangle(&penblue, m_rtStyle[1]);
	//graphics.DrawRectangle(&penblue, m_rtStyle[2]);
	//graphics.DrawRectangle(&penblue, m_rtStyle[3]);
	//graphics.DrawRectangle(&penblue, m_rtStyle[4]);
	//graphics.DrawRectangle(&penblue, m_rtStyle[5]);
	//graphics.DrawRectangle(&penblue, m_rtStyle[6]);
	//graphics.DrawRectangle(&penblue, m_rtStyleSub[0]);
	//graphics.DrawRectangle(&penblue, m_rtStyleSub[1]);
	//graphics.DrawRectangle(&penblue, m_rtStyleSub[2]);
	//graphics.DrawRectangle(&penblue, m_rtStyleSub[3]);
	//graphics.DrawRectangle(&penblue, m_rtStyleSub[4]);


	int i = 0;
	m_iterImage = m_vecBtnStyle.begin() + m_nBtnCurGroup * 7;
	for (; m_iterImage!=m_vecBtnStyle.end(); ++m_iterImage,++i)
	{
		if (i < 7)
		{
			graphics.DrawImage(m_iterImage->pBitmap, m_rtStyle[i]);
		}
	}
	if (-1 != m_nBtnStyleSel && m_nBtnStyleSel+m_nBtnCurGroup*7 < m_vecBtnStyle.size())
	{
		graphics.DrawRectangle(&penblue, m_rtStyle[m_nBtnStyleSel]);
	}

	// 显示5种样式
	if (-1 == m_nBtnStyleSel)
	{
		graphics.ReleaseHDC(dc.m_hDC);
		return;
	}

	int nBtnId = -1;
	try
	{
		nBtnId = m_vecBtnStyle.at(m_nBtnCurGroup * 7 + m_nBtnStyleSel).nBtnId;
	}
	catch (...)
	{
		graphics.ReleaseHDC(dc.m_hDC);
		return;
	}

	if (nBtnId != -1)
	{
		for (m_iterImage=m_vecBtnImage.begin(); m_iterImage!=m_vecBtnImage.end(); ++m_iterImage)
		{
			if (m_iterImage->nBtnId == nBtnId)
			{
				switch (m_iterImage->nBtnSubId)
				{
				case 0:
					m_pBitmapComm = m_iterImage->pBitmap;
					break;
				case 1:
					m_pBitmapDown = m_iterImage->pBitmap;
					break;
				case 2:
					m_pBitmapOver = m_iterImage->pBitmap;
					break;
				case 3:
					m_pBitmapDisable = m_iterImage->pBitmap;
					break;
				case 4:
					m_pBitmapChecked = m_iterImage->pBitmap;
					break;
				default:
					break;
				}
			}
		}

		CString strTextContent;
		GetDlgItem(IDC_EDIT_TEXT_CONTENT)->GetWindowTextW(strTextContent);
		int nTextLen = strTextContent.GetLength();

		CString strTextSize;
		GetDlgItem(IDC_EDIT_TEXT_SIZE)->GetWindowTextW(strTextSize);
		int nTextSize = _ttoi(strTextSize);

		Font font(L"微软雅黑", nTextSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel,NULL);

		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);

		SolidBrush brush(Color(255, GetRValue(m_nTextColor), GetGValue(m_nTextColor), GetBValue(m_nTextColor)));


		RectF rect[5];
		rect[0] = m_rtStyleSub[0];
		rect[1] = m_rtStyleSub[1];
		rect[2] = m_rtStyleSub[2];
		rect[3] = m_rtStyleSub[3];
		rect[4] = m_rtStyleSub[4];
		rect[0].Y += 12;
		rect[1].Y += 12;
		rect[2].Y += 12;
		rect[3].Y += 12;
		rect[4].Y += 12;

		if (m_pBitmapComm != NULL)
		{
			graphics.DrawImage(m_pBitmapComm, m_rtStyleSub[0]);
			graphics.DrawString(strTextContent, nTextLen, &font, rect[0], &format, &brush);
		}
		if (m_pBitmapDown != NULL)
		{
			graphics.DrawImage(m_pBitmapDown, m_rtStyleSub[1]);
			graphics.DrawString(strTextContent, nTextLen, &font, rect[1], &format, &brush);
		}
		if (m_pBitmapOver != NULL)
		{
			graphics.DrawImage(m_pBitmapOver, m_rtStyleSub[2]);
			graphics.DrawString(strTextContent, nTextLen, &font, rect[2], &format, &brush);
		}
		if (m_pBitmapDisable != NULL)
		{
			graphics.DrawImage(m_pBitmapDisable, m_rtStyleSub[3]);
			graphics.DrawString(strTextContent, nTextLen, &font, rect[3], &format, &brush);
		}
		if (m_pBitmapChecked != NULL)
		{
			graphics.DrawImage(m_pBitmapChecked, m_rtStyleSub[4]);
			graphics.DrawString(strTextContent, nTextLen, &font, rect[4], &format, &brush);
		}
	}

	graphics.ReleaseHDC(dc.m_hDC);
}


BOOL CMetaBtnSetDlg::OnInitDialog()
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
	strSettingValue.Format(L"%d",m_nSettingValue);
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
		const int nResult = m_vec[n].fResult;
		strTemp.Format(L"%d",nResult);
		m_list_relation.SetItemText(n,2,strTemp);
	}

	ASSERT(m_pCanvasDlg != NULL);

	//
	InitImageStyleRect();

	LoadButtonImage();
	InitSelectButton();
	EnableUpNextButton();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMetaBtnSetDlg::OnBnClickedOk()
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
	m_nSettingValue = _wtoi(strTemp.GetString());
	const int nCount = m_list_relation.GetItemCount();
	m_vec.clear();
	for (int k=0;k<nCount;++k)
	{
		_tagRelationExpress express;
		express.strPointName = m_list_relation.GetItemText(k,0);
		express.nRelationExpress = GetIntRelationByText(m_list_relation.GetItemText(k,1));
		express.fResult = _wtoi(m_list_relation.GetItemText(k,2));
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

void CMetaBtnSetDlg::OnBnClickedButtonMetaButtonBindSetPointSelect()
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

void CMetaBtnSetDlg::OnBnClickedButtonMetaButtonBindSetPointClear()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_META_BUTTON_BIND_SET_POINT)->SetWindowTextW(L"");
	m_strBindSetPointName = L"";
}


void CMetaBtnSetDlg::OnStnClickedStaticTextColor()
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


HBRUSH CMetaBtnSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CMetaBtnSetDlg::InitList()
{
	m_list_relation.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_list_relation.InsertColumn(0,L"绑定点",LVCFMT_LEFT, 80);
	m_list_relation.InsertColumn(1,L"逻辑关系",LVCFMT_LEFT, 80);
	m_list_relation.InsertColumn(2,L"匹配值",LVCFMT_LEFT, 60);
}

void CMetaBtnSetDlg::InitComboRelation()
{
	m_combo_relation.AddString(L"与");
	m_combo_relation.AddString(L"或");
}

CString CMetaBtnSetDlg::GetRelationTextByInt( const int nInt )
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

int CMetaBtnSetDlg::GetIntRelationByText( const CString strText )
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


void CMetaBtnSetDlg::OnBnClickedButtonRelationAdd()
{
	const int nCount = m_list_relation.GetItemCount();
	m_list_relation.InsertItem(nCount,L"绑定点");
	m_list_relation.SetItemText(nCount,1,L"等于");
	m_list_relation.SetItemText(nCount,2,L"0");
}


void CMetaBtnSetDlg::OnBnClickedButtonRelationDelete()
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


void CMetaBtnSetDlg::OnNMDblclkListRelation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CRelationItemSettingDlg dlg;
	dlg.m_strPointName = m_list_relation.GetItemText(pNMItemActivate->iItem,0);
	dlg.m_nRelation = GetIntRelationByText(m_list_relation.GetItemText(pNMItemActivate->iItem,1));
	dlg.m_fResult = _wtoi(m_list_relation.GetItemText(pNMItemActivate->iItem,2));
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


void CMetaBtnSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (point.x > m_rtStyle[0].X && point.x < m_rtStyle[0].X + m_rtStyle[0].Width
		&& point.y > m_rtStyle[0].Y && point.y < m_rtStyle[0].Y + m_rtStyle[0].Height)
	{
		m_nBtnStyleSel = 0;
	}
	else if (point.x > m_rtStyle[1].X && point.x < m_rtStyle[1].X + m_rtStyle[1].Width
		&& point.y > m_rtStyle[1].Y && point.y < m_rtStyle[1].Y + m_rtStyle[1].Height)
	{
		m_nBtnStyleSel = 1;
	}
	else if (point.x > m_rtStyle[2].X && point.x < m_rtStyle[2].X + m_rtStyle[2].Width
		&& point.y > m_rtStyle[2].Y && point.y < m_rtStyle[2].Y + m_rtStyle[2].Height)
	{
		m_nBtnStyleSel = 2;
	}
	else if (point.x > m_rtStyle[3].X && point.x < m_rtStyle[3].X + m_rtStyle[3].Width
		&& point.y > m_rtStyle[3].Y && point.y < m_rtStyle[3].Y + m_rtStyle[3].Height)
	{
		m_nBtnStyleSel = 3;
	}
	else if (point.x > m_rtStyle[4].X && point.x < m_rtStyle[4].X + m_rtStyle[4].Width
		&& point.y > m_rtStyle[4].Y && point.y < m_rtStyle[4].Y + m_rtStyle[4].Height)
	{
		m_nBtnStyleSel = 4;
	}
	else if (point.x > m_rtStyle[5].X && point.x < m_rtStyle[5].X + m_rtStyle[5].Width
		&& point.y > m_rtStyle[5].Y && point.y < m_rtStyle[5].Y + m_rtStyle[5].Height)
	{
		m_nBtnStyleSel = 5;
	}
	else if (point.x > m_rtStyle[6].X && point.x < m_rtStyle[6].X + m_rtStyle[6].Width
		&& point.y > m_rtStyle[6].Y && point.y < m_rtStyle[6].Y + m_rtStyle[6].Height)
	{
		m_nBtnStyleSel = 6;
	}
	else
	{
		//m_nBtnStyleSel = -1;
	}

	int nBtnId = -1;
	try
	{
		nBtnId = m_vecBtnStyle.at(m_nBtnStyleSel + m_nBtnCurGroup * 7).nBtnId;
	}
	catch(...)
	{
		return;
	}

	for (m_iterImage=m_vecBtnImage.begin(); m_iterImage!=m_vecBtnImage.end(); ++m_iterImage)
	{
		if (m_iterImage->nBtnId == nBtnId)
		{
			if (0 == m_iterImage->nBtnSubId)
			{
				m_nIDPicComm = m_iterImage->ID;
				m_picInfo[0] = *m_iterImage;
			}
			else if (1 == m_iterImage->nBtnSubId)
			{
				m_nIDPicDown = m_iterImage->ID;
				m_picInfo[1] = *m_iterImage;
			}
			else if (2 == m_iterImage->nBtnSubId)
			{
				m_nIDPicOver = m_iterImage->ID;
				m_picInfo[2] = *m_iterImage;
			}
			else if (3 == m_iterImage->nBtnSubId)
			{
				m_nIDPicDisable = m_iterImage->ID;
				m_picInfo[3] = *m_iterImage;
			}
			else if (4 == m_iterImage->nBtnSubId)
			{
				m_nICPicChecked = m_iterImage->ID;
				m_picInfo[4] = *m_iterImage;
			}
		}
	}

	Invalidate();

	CDialog::OnLButtonDown(nFlags, point);
}


void CMetaBtnSetDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonUp(nFlags, point);
}


void CMetaBtnSetDlg::OnMouseMove(UINT nFlags, CPoint point)
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


//////////////////////////////////////////////////////////////////////////


void CMetaBtnSetDlg::OnBnClickedBtnStyleAdd()
{
	// TODO: Add your control notification handler code here
	int nSelectType = 7;	// 按钮类型编号值为7
	CString strSelectType;
	strSelectType.Format(_T("%d"),nSelectType);

	char szType[MAX_PATH] = {0};
	size_t converted = 0;
	wcstombs_s(&converted, szType, sizeof(szType), strSelectType.GetString(), sizeof(szType));
	assert(converted > 0);


	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_ALLOWMULTISELECT, _T("PNG File (*.png)|*.png|"), NULL);
	if (IDOK == dlg.DoModal())
	{
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );

		const int nMaxBtnId = m_pProjectSqlite->GetMaxLibImageBtnId(false) + 1;
		int nBtnSubId = -1;
		POSITION pos = dlg.GetStartPosition();
		int nCount = 0;
		CString	strPreName;
		CString	strRec;
		while (NULL != pos)
		{
			strPreName = dlg.GetNextPathName(pos);
			int nLen = strPreName.GetLength();
			strPreName = strPreName.Right(nLen - strPreName.ReverseFind(_T('\\')) - 1);

			strPreName = strPreName.Left(strPreName.Find(_T('_')));
			if (0 == nCount)
			{
				strRec = strPreName;
			}
			else
			{
				if (strPreName != strRec)
				{
					MessageBox(_T("请按照格式导入图片！"), _T("错误"), MB_OK|MB_ICONERROR);
					return;
				}
			}

			++nCount;
		}
		if (5 != nCount)
		{
			MessageBox(_T("请每组导入5张图片！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		//
		pos = dlg.GetStartPosition();
		while (NULL != pos)
		{
			CString strFilePath = dlg.GetNextPathName(pos);
			CString strFolder = strFilePath.Left(strFilePath.ReverseFind(L'\\'));

			CString strFileNameWithEx =	strFilePath.Right(strFilePath.GetLength() - strFolder.GetLength() - 1);
			CString strFileName = strFileNameWithEx.Left(strFileNameWithEx.Find(L'.',0));
			FILE* pFile = NULL;
			const errno_t rs = _wfopen_s(&pFile, strFilePath.GetString(), L"rb");
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
			converted = 0;
			wcstombs_s(&converted, szFileName, sizeof(szFileName), strFileName.GetString(), sizeof(szFileName));
			assert(converted > 0);
			nBtnSubId = GetImageSubId(strFileName);
			assert(nBtnSubId != -1);
			if (-1 == nBtnSubId)
			{
				continue;
			}

			m_pProjectSqlite->InsertPictureData(nPicID, szFileName, szType,pBuf, file_size, false, nMaxBtnId, nBtnSubId);
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
						info.etype_equip = strSelectType.GetString();
						wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), strFileName.GetString(), sizeof(info.szName)/sizeof(wchar_t)-1);
						info.nSize = file_size;
						info.pBlock = new char[file_size];
						if (info.pBlock != NULL)
						{
							memset(info.pBlock, 0, file_size);
							memcpy_s(info.pBlock, file_size, pBuf, file_size);
						}
						info.nBtnId = nMaxBtnId;
						info.nBtnSubId = nBtnSubId;
						m_pProjectSqlite->InsertToLibImage(info);

						PictureView pic;
						pic.pBitmap = info.pBitmap;
						pic.strName = info.szName;
						pic.nID = info.ID;
						pic.etype_equip = strSelectType.GetString();

						GlobalUnlock(hMemBmp);
					}
					GlobalFree(hMemBmp);
					hMemBmp = NULL;
				}
			}
			SAFE_DELETE_ARRAY(pBuf);
		}
		setlocale( LC_ALL, old_locale );
		MessageBox(_T("添加图片成功！"), _T("成功"), MB_OK|MB_ICONASTERISK);
		LoadButtonImage();
		EnableUpNextButton();
		Invalidate();
	}
}


void CMetaBtnSetDlg::OnBnClickedBtnStyleDel()
{
	// TODO: Add your control notification handler code here
	if (-1 == m_nBtnStyleSel)
	{
		return;
	}

	int nDel = m_nBtnStyleSel + m_nBtnCurGroup * 7;
	PictureInfo	info;
	int nBtnId = 0;

	try
	{
		info = m_vecBtnStyle.at(nDel);
	}
	catch (...)
	{
		MessageBox(_T("删除样式异常！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}


	assert(m_pProjectSqlite != NULL);
	if (NULL == m_pProjectSqlite)
	{
		return;
	}

	nBtnId = info.nBtnId;
	vector<int>	vecDelId;
	vector<int>::const_iterator	iter;

	for (m_iterImage=m_pProjectSqlite->m_vPictureListImage.begin(); m_iterImage!=m_pProjectSqlite->m_vPictureListImage.end(); ++m_iterImage)
	{
		if (m_iterImage->nBtnId == nBtnId)
		{
			vecDelId.push_back(m_iterImage->ID);
		}
	}
	for (iter=vecDelId.begin(); iter!=vecDelId.end(); ++iter)
	{
		m_pProjectSqlite->DeleteOnePicture(*iter, false);
	}

	m_nBtnStyleSel = -1;
	m_nIDPicComm = -1;
	m_nIDPicDown = -1;
	m_nIDPicOver = -1;
	m_nIDPicDisable = -1;
	m_nICPicChecked = -1;

	m_pProjectSqlite->LoadPictureLibSqlite(false);
	LoadButtonImage();
	EnableUpNextButton();
	Invalidate();
}


void CMetaBtnSetDlg::OnBnClickedBtnStyleUp()
{
	// TODO: Add your control notification handler code here
	if (m_nBtnCurGroup > 0)
	{
		--m_nBtnCurGroup;
		m_nBtnStyleSel = -1;
		EnableUpNextButton();
		Invalidate();
	}
}


void CMetaBtnSetDlg::OnBnClickedBtnStyleNext()
{
	// TODO: Add your control notification handler code here
	++m_nBtnCurGroup;
	m_nBtnStyleSel = -1;
	EnableUpNextButton();
	Invalidate();
}

int CMetaBtnSetDlg::GetImageSubId(const CString strImageName)
{
	CString	strName(strImageName);
	int nPicSubId = 0;

	int nPos = strName.ReverseFind(_T('_'));
	strName = strName.Mid(nPos + 1);
	strName.MakeLower();

	if (_T("common") == strName)
	{
		nPicSubId = 0;
	}
	else if (_T("down") == strName)
	{
		nPicSubId = 1;
	}
	else if (_T("over") == strName)
	{
		nPicSubId = 2;
	}
	else if (_T("disable") == strName)
	{
		nPicSubId = 3;
	}
	else if (_T("checked") == strName)
	{
		nPicSubId = 4;
	}
	else
	{
		nPicSubId = -1;
	}

	return nPicSubId;
}

void CMetaBtnSetDlg::InitImageStyleRect(void)
{
	const int nX		= 50;
	const int nY		= 80;
	const int nWidth	= 90;
	const int nHeight	= 45;
	const int nInterval	= 20;


	m_rtStyle[0].X = nX;
	m_rtStyle[0].Y = nY;
	m_rtStyle[0].Width = nWidth;
	m_rtStyle[0].Height = nHeight;

	m_rtStyle[1].X = nX + nWidth + nInterval;
	m_rtStyle[1].Y = nY;
	m_rtStyle[1].Width = nWidth;
	m_rtStyle[1].Height = nHeight;

	m_rtStyle[2].X = nX + (nWidth + nInterval) * 2;
	m_rtStyle[2].Y = nY;					      
	m_rtStyle[2].Width = nWidth;			      
	m_rtStyle[2].Height = nHeight;			      
												      
	m_rtStyle[3].X = nX + (nWidth + nInterval) * 3;
	m_rtStyle[3].Y = nY;					      
	m_rtStyle[3].Width = nWidth;			      
	m_rtStyle[3].Height = nHeight;			      
												      
	m_rtStyle[4].X = nX + (nWidth + nInterval) * 4;
	m_rtStyle[4].Y = nY;					      
	m_rtStyle[4].Width = nWidth;			      
	m_rtStyle[4].Height = nHeight;			      
												      
	m_rtStyle[5].X = nX + (nWidth + nInterval) * 5;
	m_rtStyle[5].Y = nY;					      
	m_rtStyle[5].Width = nWidth;			      
	m_rtStyle[5].Height = nHeight;			      
												      
	m_rtStyle[6].X = nX + (nWidth + nInterval) * 6;
	m_rtStyle[6].Y = nY;
	m_rtStyle[6].Width = nWidth;
	m_rtStyle[6].Height = nHeight;


	//
	const int nXSub			= 100;
	const int nYSub			= 190;
	const int nIntervalSub	= 50;


	m_rtStyleSub[0].X = nXSub;
	m_rtStyleSub[0].Y = nYSub;
	m_rtStyleSub[0].Width = nWidth;
	m_rtStyleSub[0].Height = nHeight;

	m_rtStyleSub[1].X = nXSub + nWidth + nIntervalSub;
	m_rtStyleSub[1].Y = nYSub;
	m_rtStyleSub[1].Width = nWidth;
	m_rtStyleSub[1].Height = nHeight;

	m_rtStyleSub[2].X = nXSub + (nWidth + nIntervalSub) * 2;
	m_rtStyleSub[2].Y = nYSub;
	m_rtStyleSub[2].Width = nWidth;
	m_rtStyleSub[2].Height = nHeight;

	m_rtStyleSub[3].X = nXSub + (nWidth + nIntervalSub) * 3;
	m_rtStyleSub[3].Y = nYSub;
	m_rtStyleSub[3].Width = nWidth;
	m_rtStyleSub[3].Height = nHeight;

	m_rtStyleSub[4].X = nXSub + (nWidth + nIntervalSub) * 4;
	m_rtStyleSub[4].Y = nYSub;
	m_rtStyleSub[4].Width = nWidth;
	m_rtStyleSub[4].Height = nHeight;
}

void CMetaBtnSetDlg::LoadButtonImage(void)
{
	m_vecBtnImage.clear();
	m_vecBtnStyle.clear();

	//for (m_iterImage=m_pProjectSqlite->m_vPictureListImage.begin(); m_iterImage!=m_pProjectSqlite->m_vPictureListImage.end(); ++m_iterImage)
	//{
	//	if (L"7" == m_iterImage->etype_equip && -1 != m_iterImage->nBtnSubId)
	//	{
	//		m_vecBtnImage.push_back(*m_iterImage);

	//		if (0 == m_iterImage->nBtnSubId)
	//		{
	//			m_vecBtnStyle.push_back(*m_iterImage);
	//		}
	//	}
	//}



	//
	CString	strSysPath;
	GetSysPath(strSysPath);
	strSysPath += _T("\\Images\\");

	CFileFind finder;
	CString	strTitle[5];
	CString strPath[5];

	strTitle[0] = _T("btn_style1_common.png");
	strTitle[1] = _T("btn_style1_down.png");
	strTitle[2] = _T("btn_style1_over.png");
	strTitle[3] = _T("btn_style1_disable.png");
	strTitle[4] = _T("btn_style1_checked.png");
	strPath[0] = strSysPath + strTitle[0];
	strPath[1] = strSysPath + strTitle[1];
	strPath[2] = strSysPath + strTitle[2];
	strPath[3] = strSysPath + strTitle[3];
	strPath[4] = strSysPath + strTitle[4];

	vector<PictureInfo>::const_iterator	iter;
	CProjectSqlite* pSql = m_pMainFrm->m_pPrjSqlite;
	assert(pSql != NULL);
	if (NULL == pSql)
	{
		return;
	}

	CString	strName;
	for (int i=0; i<5 ;i++)
	{
		bool bExist = false;
		for (iter=pSql->m_vPictureList.begin(); iter!=pSql->m_vPictureList.end(); ++iter)
		{
			strName.Format(L"%s", iter->szName);
			if (strName == strTitle[i])
			{
				bExist = true;
				break;
			}
		}


		//
		int nBtnId = _ttoi(strTitle[i].Mid(9, 1));

		//
		assert(m_pMainFrm != NULL);
		if (NULL == m_pMainFrm)
		{
			return;
		}

		FILE* pFile = NULL;
		const errno_t rs = _wfopen_s(&pFile, strPath[i].GetString(), L"rb");
		assert(0 == rs);
		assert(pFile);
		fseek(pFile, 0, SEEK_END);
		const int file_size = ftell(pFile);
		assert(file_size > 0);
		char* pBuf = new char[file_size];
		memset(pBuf, 0, file_size);
		fseek(pFile, 0, SEEK_SET);
		fread(pBuf, sizeof(char), file_size, pFile);
		const int nPicID = m_pMainFrm->m_pPrjSqlite->GetMaxLibImageNum(false)+1+INSERT_META_FILE_ID_START;
		char szFileName[MAX_PATH] = {0};
		size_t converted = 0;
		wcstombs_s(&converted, szFileName, sizeof(szFileName), strTitle[i].GetString(), sizeof(szFileName));
		assert(converted > 0);

		if (!bExist)
		{
			char szType[MAX_PATH] = {0};
			m_pMainFrm->m_pPrjSqlite->InsertPictureData(nPicID, szFileName, szType, pBuf, file_size, true);
		}

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
					wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), strTitle[i].GetString(), sizeof(info.szName)/sizeof(wchar_t)-1);
					info.nSize = file_size;
					info.pBlock = new char[file_size];
					if (info.pBlock != NULL)
					{
						memset(info.pBlock, 0, file_size);
						memcpy_s(info.pBlock, file_size, pBuf, file_size);
					}
					//m_pMainFrm->m_pPrjSqlite->InsertToLibImage(info);
					info.nBtnId = nBtnId;
					info.nBtnSubId = i;
					m_vecBtnImage.push_back(info);
					if (1 == info.nBtnSubId)
					{
						m_vecBtnStyle.push_back(info);
					}

					GlobalUnlock(hMemBmp);
				}
				GlobalFree(hMemBmp);
				hMemBmp = NULL;
			}
		}
		SAFE_DELETE_ARRAY(pBuf);
	}
	finder.Close();
}

void CMetaBtnSetDlg::EnableUpNextButton(void)
{
	int nSize = m_vecBtnStyle.size();
	if (nSize <= 7)
	{
		m_btnStyleUp.EnableWindow(FALSE);
		m_btnStyleNext.EnableWindow(FALSE);
	}
	else if (nSize > 7 && 0 == m_nBtnCurGroup)
	{
		m_btnStyleUp.EnableWindow(FALSE);
		m_btnStyleNext.EnableWindow(TRUE);
	}
	else if (nSize > 14 && m_nBtnCurGroup > 0)
	{
		m_btnStyleUp.EnableWindow(TRUE);
		m_btnStyleNext.EnableWindow(TRUE);
	}
	else if (nSize > 7 && m_nBtnCurGroup > 0)
	{
		m_btnStyleUp.EnableWindow(TRUE);
		m_btnStyleNext.EnableWindow(FALSE);
	}
	else
	{
		// unknow
	}
}


void CMetaBtnSetDlg::OnEnChangeEditTextContent()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate();
}


void CMetaBtnSetDlg::OnEnChangeEditTextSize()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate();
}

void CMetaBtnSetDlg::InitSelectButton()
{
	if (-1 != m_nIDPicComm)
	{
		int i = 0;
		bool bFind = false;
		for (m_iterImage=m_vecBtnStyle.begin(); m_iterImage!=m_vecBtnStyle.end(); ++m_iterImage,++i)
		{
			if (m_iterImage->ID == m_nIDPicComm)
			{
				bFind = true;
				break;
			}
		}

		if (bFind)
		{
			m_nBtnStyleSel = i % 7;
			m_nBtnCurGroup = i / 7;
		}
	}
}
