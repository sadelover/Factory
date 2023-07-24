// DashBoardPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DashBoardPropertyDlg.h"
#include "DataComConfigDlg.h"


// CDashBoardPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CDashBoardPropertyDlg, CDialog)

CDashBoardPropertyDlg::CDashBoardPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDashBoardPropertyDlg::IDD, pParent)
	, m_strBindPointName(_T(""))
	, m_pDashBoard(NULL)
	, m_pPointMap(NULL)
	, m_dMaxValue(100.0f)
	, m_dMinValue(0.0f)
	, m_nStyle(0)
	, m_bIsBindMax(FALSE)
	, m_bIsBindMin(FALSE)
	, m_strMaxValue(_T(""))
	, m_strMinValue(_T(""))
	, m_nLayer(4)
	, m_FontColor(0x00ffffff)
	, m_DecimalPlaces(1)
{

}

CDashBoardPropertyDlg::~CDashBoardPropertyDlg()
{
}

void CDashBoardPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DASHBOARD_BIND_POINT, m_strBindPointName);
	DDX_Control(pDX, IDC_BTN_MAX_SEL_POINT, m_btnMaxSelPoint);
	DDX_Control(pDX, IDC_BTN_MIN_SEL_POINT, m_btnMinSelPoint);
	DDX_Check(pDX, IDC_CHK_BIND_MAX, m_bIsBindMax);
	DDX_Check(pDX, IDC_CHK_BIND_MIN, m_bIsBindMin);
	DDX_Text(pDX, IDC_EDITMETER_PAN_MAX, m_strMaxValue);
	DDX_Text(pDX, IDC_EDITMETER_PAN_MIN, m_strMinValue);
	DDX_Control(pDX, IDC_EDITMETER_PAN_MAX, m_editMax);
	DDX_Control(pDX, IDC_EDITMETER_PAN_MIN, m_editMin);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Text(pDX, IDC_EDIT1, m_DecimalPlaces);
	DDX_Control(pDX, IDC_FontColor, m_cFontColor);
}


BEGIN_MESSAGE_MAP(CDashBoardPropertyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DASHBOARD_SELECT_POINT, &CDashBoardPropertyDlg::OnBnClickedButtonDashboardSelectPoint)
	ON_BN_CLICKED(IDOK, &CDashBoardPropertyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_MAX_SEL_POINT, &CDashBoardPropertyDlg::OnBnClickedBtnMaxSelPoint)
	ON_BN_CLICKED(IDC_BTN_MIN_SEL_POINT, &CDashBoardPropertyDlg::OnBnClickedBtnMinSelPoint)
	ON_BN_CLICKED(IDC_CHK_BIND_MAX, &CDashBoardPropertyDlg::OnBnClickedChkBindMax)
	ON_BN_CLICKED(IDC_CHK_BIND_MIN, &CDashBoardPropertyDlg::OnBnClickedChkBindMin)
END_MESSAGE_MAP()


// CDashBoardPropertyDlg 消息处理程序


void CDashBoardPropertyDlg::OnBnClickedButtonDashboardSelectPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataComConfigDlg dlg(*m_pPointMap);
	if (dlg.DoModal() == IDOK)
	{
		m_strBindPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CDashBoardPropertyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_dMaxValue = _wtof(m_strMaxValue.GetString());
	m_dMinValue = _wtof(m_strMinValue.GetString());

	if (!m_bIsBindMax)
	{
		if (!CheckBindPointValue(m_strMaxValue))
		{
			MessageBox(_T("最大值输入错误！"), _T("错误"), MB_OK|MB_ICONERROR);
			m_editMax.SetSel(0, -1);
			m_editMax.SetFocus();
			return;
		}
	}

	if (!m_bIsBindMin)
	{
		if (!CheckBindPointValue(m_strMinValue))
		{
			MessageBox(_T("最小值输入错误！"), _T("错误"), MB_OK|MB_ICONERROR);
			m_editMin.SetSel(0, -1);
			m_editMin.SetFocus();
			return;
		}
	}

	if (!m_bIsBindMax && !m_bIsBindMin)
	{	// compare max and min
		if (m_dMaxValue < m_dMinValue)
		{
			MessageBox(_T("最大值不能小于最小值！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}
	}

	if((m_DecimalPlaces < 0) || (m_DecimalPlaces > 10))
	{
		MessageBox(_T("小数点位数不得超过10个"));
		return ;
	}
	//获取颜色
	m_FontColor = m_cFontColor.GetColor();
	CDialog::OnOK();
}


BOOL CDashBoardPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitControl();

	/*if (m_nStyle == 0)
	{
		m_dMaxValue = 1.5;
		m_dMinValue = 0.4;

		GetDlgItem(IDC_EDITMETER_PAN_MAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITMETER_PAN_MIN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDITMETER_PAN_MAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDITMETER_PAN_MIN)->EnableWindow(TRUE);
	}*/
	CString strMax;
	strMax.Format(L"%.1f", m_dMaxValue);
	CString strMin;
	strMin.Format(L"%.1f", m_dMinValue);

	if (!m_bIsBindMax)
	{
		m_strMaxValue = strMax;
	}
	if (!m_bIsBindMin)
	{
		m_strMinValue = strMin;
	}
	UpdateData(FALSE);

	SetMaxMinControlsStatus(true);
	SetMaxMinControlsStatus(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDashBoardPropertyDlg::SetDashBoardPointer( CEqmDrawDashBoard* pDashBoard )
{
	ASSERT(pDashBoard);

	m_pDashBoard = pDashBoard;
}

void CDashBoardPropertyDlg::SetPointMap( CDataPointManager* pPointMap )
{
	ASSERT(pPointMap);

	m_pPointMap = pPointMap;
}


void CDashBoardPropertyDlg::OnBnClickedBtnMaxSelPoint()
{
	// TODO: Add your control notification handler code here
	CDataComConfigDlg dlg(*m_pPointMap);
	if (IDOK == dlg.DoModal())
	{
		m_strMaxValue = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CDashBoardPropertyDlg::OnBnClickedBtnMinSelPoint()
{
	// TODO: Add your control notification handler code here
	CDataComConfigDlg dlg(*m_pPointMap);
	if (IDOK == dlg.DoModal())
	{
		m_strMinValue = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CDashBoardPropertyDlg::OnBnClickedChkBindMax()
{
	// TODO: Add your control notification handler code here
	SetMaxMinControlsStatus(true);
	m_strMaxValue = _T("");
	UpdateData(FALSE);
}


void CDashBoardPropertyDlg::OnBnClickedChkBindMin()
{
	// TODO: Add your control notification handler code here
	SetMaxMinControlsStatus(false);
	m_strMinValue = _T("");
	UpdateData(FALSE);
}

void CDashBoardPropertyDlg::SetMaxMinControlsStatus(const bool bIsMax)
{
	UpdateData(TRUE);
	if (bIsMax)
	{
		if (m_bIsBindMax)
		{
			m_editMax.SetReadOnly(TRUE);
			m_btnMaxSelPoint.EnableWindow(TRUE);
		}
		else
		{
			m_editMax.SetReadOnly(FALSE);
			m_btnMaxSelPoint.EnableWindow(FALSE);
		}
	} 
	else
	{
		if (m_bIsBindMin)
		{
			m_editMin.SetReadOnly(TRUE);
			m_btnMinSelPoint.EnableWindow(TRUE);
		}
		else
		{
			m_editMin.SetReadOnly(FALSE);
			m_btnMinSelPoint.EnableWindow(FALSE);
		}
	}
}

bool CDashBoardPropertyDlg::CheckBindPointValue(const CString strSrc)
{
	// input must be numeric, if fail when input have alphabet or other content

	const CString strFlag(_T("0123456789."));
	CString	strChar;

	int nLen = strSrc.GetLength();
	for (int i=0; i<nLen; ++i)
	{
		strChar = strSrc.Mid(i, 1);
		if (-1 == strFlag.Find(strChar))
		{
			return false;
		}
	}

	return true;
}

void CDashBoardPropertyDlg::InitControl(void)
{
	// combobox
	m_comboLayer.AddString(_T("1层"));
	m_comboLayer.AddString(_T("2层"));
	m_comboLayer.AddString(_T("3层"));
	m_comboLayer.AddString(_T("4层"));
	m_comboLayer.AddString(_T("5层"));
	m_comboLayer.AddString(_T("6层"));
	m_comboLayer.AddString(_T("7层"));
	m_comboLayer.AddString(_T("8层"));
	m_comboLayer.AddString(_T("9层"));
	m_comboLayer.AddString(_T("10层"));

	m_comboLayer.SetCurSel(m_nLayer);
	
	m_cFontColor.SetColor(m_FontColor);
	m_cFontColor.EnableAutomaticButton (_T("默认"), afxGlobalData.clrBtnText);
	m_cFontColor.EnableOtherButton (_T("其他颜色"));
	m_cFontColor.SetColumnsNumber(5);
}