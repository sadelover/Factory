// ChartItemPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChartItemPropertyDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "Tools/CustomTools/CustomTools.h"

// CChartItemPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CChartItemPropertyDlg, CDialog)

CChartItemPropertyDlg::CChartItemPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartItemPropertyDlg::IDD, pParent)
	, m_strItemName(_T(""))
	, m_strBindPointName(_T(""))
	,m_nColor(0)
	, m_nBindPointStoreType(0)
	,m_pPointMap(NULL)
{

}

CChartItemPropertyDlg::~CChartItemPropertyDlg()
{
}

void CChartItemPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ITEM_NAME, m_strItemName);
	DDX_Text(pDX, IDC_EDIT_ITEM_BIND_POINT_NAME, m_strBindPointName);
	//DDX_Text(pDX, IDC_COMBO_ITEM_BIND_POINT_TYPE, m_nBindPointStoreType);
	DDX_Control(pDX, IDC_COMBO_ITEM_BIND_POINT_TYPE, m_combo_point_type);
}

BEGIN_MESSAGE_MAP(CChartItemPropertyDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CChartItemPropertyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_BIND_POINT_SELECT, &CChartItemPropertyDlg::OnBnClickedButtonItemBindPointSelect)
	ON_STN_CLICKED(IDC_STATIC_ITEM_COLOR, &CChartItemPropertyDlg::OnStnClickedStaticItemColor)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_BIND_POINT_TYPE, &CChartItemPropertyDlg::OnCbnSelchangeComboItemBindPointType)
END_MESSAGE_MAP()


// CChartItemPropertyDlg 消息处理程序


BOOL CChartItemPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_combo_point_type.SetCurSel(m_nBindPointStoreType);
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CChartItemPropertyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ITEM_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_nColor);
		if (hBrush)
		{
			return hBrush;
		}
	}
	return hbr;
}

void CChartItemPropertyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_ITEM_NAME)->GetWindowTextW(m_strItemName);
	GetDlgItem(IDC_EDIT_ITEM_BIND_POINT_NAME)->GetWindowTextW(m_strBindPointName);
	CDialog::OnOK();
}

void CChartItemPropertyDlg::OnBnClickedButtonItemBindPointSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataComConfigDlg dlg(*m_pPointMap);
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_ITEM_BIND_POINT_NAME)->SetWindowTextW(dlg.m_strPointName);
		m_nBindPointStoreType = dlg.m_selectedStoreType;
	}
	UpdateData(TRUE);
}

void CChartItemPropertyDlg::SetPointMap( CDataPointManager* pPointMap )
{
	ASSERT(pPointMap);

	m_pPointMap = pPointMap;
}

void CChartItemPropertyDlg::OnStnClickedStaticItemColor()
{
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
	CHOOSECOLOR cc = {sizeof( CHOOSECOLOR ),m_hWnd,NULL,m_nColor,cusColor,CC_FULLOPEN,NULL,NULL,NULL};
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
		m_nColor = cc.rgbResult;
		Invalidate();
	}
}


void CChartItemPropertyDlg::OnCbnSelchangeComboItemBindPointType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nBindPointStoreType = m_combo_point_type.GetCurSel();
	UpdateData(FALSE);
}
