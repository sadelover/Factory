// ChartPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChartPropertyDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/EqmDrawBase.h"
#include "../ComponentDraw/EqmDrawLineGraphic.h"
#include "../ComponentDraw/EqmDrawBarGraphic.h"
#include "../ComponentDraw/EqmDrawPieGraphic.h"
#include "DataComConfigDlg.h"
#include "ChartItemPropertyDlg.h"
#include <cassert>
#include "MainFrm.h"
#include "Tools/CustomTools/CustomTools.h"

// CChartPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CChartPropertyDlg, CDialog)

CChartPropertyDlg::CChartPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartPropertyDlg::IDD, pParent)
	, m_strChartTitle(_T("请输入图表显示的标题"))
	, m_nUpdateInterval(2000)
	, m_strXName(_T(""))
	, m_strXUnit(_T(""))
	, m_strYName(_T(""))
	,m_pLineGraphic(NULL)
	,m_pBarGraphic(NULL)
	,m_pPieGraphic(NULL)
	,m_eBarType(eBarNormal)
	, m_strAssoPoint1(_T(""))
	, m_strAssoPoint2(_T(""))
	, m_strNameItem1(_T(""))
	, m_strNameItem2(_T(""))
	, m_strMaxValue(_T(""))
	,m_nQueryType(0)
	,m_nColorIndex(0)
	,m_nTextColor(RGB(0,0,0))
	, m_dYMax(0)
	, m_dYMin(0)
	, m_nCheckMax(0)
	, m_nCheckMin(0)
	, m_nLayer(0)
{

}

CChartPropertyDlg::CChartPropertyDlg( CEqmDrawBase* pBase, CWnd* pParent)
	: CDialog(CChartPropertyDlg::IDD, pParent)
	,m_pBase(pBase)
	, m_strChartTitle(_T("请输入图表显示的标题"))
	, m_nUpdateInterval(0)
	, m_strXName(_T(""))
	, m_strXUnit(_T(""))
	, m_strYName(_T(""))
	,m_pLineGraphic(NULL)
	,m_pBarGraphic(NULL)
	,m_pPieGraphic(NULL)
	,m_eBarType(eBarNormal)
	,m_nColorIndex(0)
{

}

CChartPropertyDlg::~CChartPropertyDlg()
{
}

void CChartPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHART_TITLE, m_strChartTitle);
	DDX_Text(pDX, IDC_EDIT_UPDATE_INTERVAL, m_nUpdateInterval);
	DDX_Text(pDX, IDC_EDIT_X_TITLE, m_strXName);
	DDX_CBString(pDX, IDC_COMBO_X_UNIT, m_strXUnit);
	DDX_Text(pDX, IDC_EDIT_Y_TITLE, m_strYName);
	DDX_Control(pDX, IDC_LIST_GRAPH_ITEM_LIST, m_listGraphItem);
	DDX_Control(pDX, IDC_COMBO_X_UNIT, m_comboXUnit);
	DDX_Control(pDX, IDC_COMBO_BAR_TYPE, m_combo_bar_type);
	DDX_Text(pDX, IDC_EDIT_ASSO_POINT1, m_strAssoPoint1);
	DDX_Text(pDX, IDC_EDIT_ASSO_POINT2, m_strAssoPoint2);
	DDX_Text(pDX, IDC_EDIT_ITEM1_NAME, m_strNameItem1);
	DDX_Text(pDX, IDC_EDIT_ITEM2_NAME, m_strNameItem2);
	DDX_Text(pDX, IDC_EDIT_CHART_MAX_VALUE, m_strMaxValue);
	DDX_Control(pDX, IDC_COMBO_QUERY_TYPE_BAR, m_combo_query_type);
	DDX_Text(pDX, IDC_EDIT_AXIS_Y_MAX, m_dYMax);
	DDX_Text(pDX, IDC_EDIT_AXIS_Y_MIN, m_dYMin);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
}


BEGIN_MESSAGE_MAP(CChartPropertyDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ITEM, &CChartPropertyDlg::OnBnClickedButtonAddItem)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ITEM, &CChartPropertyDlg::OnBnClickedButtonDeleteItem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GRAPH_ITEM_LIST, &CChartPropertyDlg::OnNMDblclkListGraphItemList)
	ON_BN_CLICKED(IDOK, &CChartPropertyDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_BAR_TYPE, &CChartPropertyDlg::OnCbnSelchangeComboBarType)
	ON_BN_CLICKED(IDC_BUTTON_ASSO_POINT1, &CChartPropertyDlg::OnBnClickedButtonAssoPoint1)
	ON_BN_CLICKED(IDC_BUTTON_ASSO_POINT2, &CChartPropertyDlg::OnBnClickedButtonAssoPoint2)
	ON_BN_CLICKED(IDC_BUTTON_START_POINT, &CChartPropertyDlg::OnBnClickedButtonStartPoint)
	ON_BN_CLICKED(IDC_BUTTON_START_POINT_CLEAR, &CChartPropertyDlg::OnBnClickedButtonStartPointClear)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_TEXT_COLOR, &CChartPropertyDlg::OnStnClickedStaticTextColor)
END_MESSAGE_MAP()


// CChartPropertyDlg 消息处理程序


BOOL CChartPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowWindowTitle();
	InitListCtrl();
	InitComboCtrl();
	InitBarComboCtrl();
	m_combo_query_type.AddString(L"分钟");
	m_combo_query_type.AddString(L"小时");
	m_combo_query_type.AddString(L"天");
	m_combo_query_type.AddString(L"周");
	m_combo_query_type.AddString(L"月");
	m_combo_query_type.AddString(L"年");
	ASSERT(m_pBase);
	if (m_pBase->GetEqmType() == EQMDRAW_LINE_CHART)
	{
		m_pLineGraphic = dynamic_cast<CEqmDrawLineGraphic*>(m_pBase);
		m_strChartTitle = m_pLineGraphic->GetTitleName();
		m_nUpdateInterval = m_pLineGraphic->GetUpdateInterval();
		m_strXName = m_pLineGraphic->GetXName();
		m_strXUnit = m_pLineGraphic->GetXUnit();
		m_strYName = m_pLineGraphic->GetYName();
		double dMax = m_pLineGraphic->GetMaxValue();
		m_strMaxValue.Format(L"%.2f",dMax);
		m_dYMax = m_pLineGraphic->GetMaxValue();
		m_dYMin = m_pLineGraphic->GetMinValue();
		m_nCheckMax = m_pLineGraphic->GetMaxCheck();
		m_nCheckMin = m_pLineGraphic->GetMinCheck();
		((CButton*)GetDlgItem(IDC_CHECK_AXIS_Y_MAX))->SetCheck(m_nCheckMax);
		((CButton*)GetDlgItem(IDC_CHECK_AXIS_Y_MIN))->SetCheck(m_nCheckMin);
		vector<_Graph_Item_Property>& vecItem = m_pLineGraphic->GetItemVector();
		for (size_t i=0;i<vecItem.size();++i)
		{
			m_listGraphItem.InsertItem(i,vecItem[i].strItemName);
			m_listGraphItem.SetItemBkColor(i,1,vecItem[i].color);
			m_listGraphItem.SetItemText(i,2,vecItem[i].strPointName);

			CString strType;
			strType.Format(_T("%d"),vecItem[i].nStoreType);
			m_listGraphItem.SetItemText(i,3,strType);
		}
		m_nTextColor = m_pLineGraphic->GetTextColor();
	}
	else if (m_pBase->GetEqmType() == EQMDRAW_BAR_CHART)
	{
		m_pBarGraphic = dynamic_cast<CEqmDrawBarGraphic*>(m_pBase);
		m_eBarType = m_pBarGraphic->GetType();
		double dMax = m_pBarGraphic->GetMaxValue();
		m_strMaxValue.Format(L"%.2f",dMax);
		m_strStartPoint = m_pBarGraphic->GetStartPoint();
		GetDlgItem(IDC_EDIT_START_POINT)->SetWindowTextW(m_strStartPoint);
		m_nQueryType = m_pBarGraphic->GetQueryType();
		m_combo_query_type.SetCurSel(m_nQueryType);
		if (m_eBarType == eBarNormal)
		{
			m_strChartTitle = m_pBarGraphic->GetTitleName();
			m_nUpdateInterval = m_pBarGraphic->GetUpdateInterval();
			m_strXName = m_pBarGraphic->GetXName();
			m_strXUnit = m_pBarGraphic->GetXUnit();
			m_strYName = m_pBarGraphic->GetYName();
			vector<_Graph_Item_Property>& vecItem = m_pBarGraphic->GetItemVector();
			for (size_t i=0;i<vecItem.size();++i)
			{
				m_listGraphItem.InsertItem(i,vecItem[i].strItemName);
				m_listGraphItem.SetItemBkColor(i,1,vecItem[i].color);
				m_listGraphItem.SetItemText(i,2,vecItem[i].strPointName);

				CString strType;
				strType.Format(_T("%d"),vecItem[i].nStoreType);
				m_listGraphItem.SetItemText(i,3,strType);
			}
		}
		else if (m_eBarType == eBarCompare)
		{
			m_strChartTitle = m_pBarGraphic->GetTitleName();
			m_nUpdateInterval = m_pBarGraphic->GetUpdateInterval();
			vector<_Graph_Item_Property>& vecItem = m_pBarGraphic->GetItemVector();
			assert(vecItem.size() == 2);
			m_strNameItem1 = vecItem[0].strItemName;
			m_strNameItem2 = vecItem[1].strItemName;
			m_strAssoPoint1 = vecItem[0].strPointName;
			m_strAssoPoint2 = vecItem[1].strPointName;
		}
		m_nTextColor = m_pBarGraphic->GetTextColor();
	}
	else if (m_pBase->GetEqmType() == EQMDRAW_PIE_CHART)
	{
		m_pPieGraphic = dynamic_cast<CEqmDrawPieGraphic*>(m_pBase);
		m_strChartTitle = m_pPieGraphic->GetTitleName();
		m_nUpdateInterval = m_pPieGraphic->GetUpdateInterval();
		vector<_Graph_Item_Property>& vecItem = m_pPieGraphic->GetItemVector();
		for (size_t i=0;i<vecItem.size();++i)
		{
			m_listGraphItem.InsertItem(i,vecItem[i].strItemName);
			m_listGraphItem.SetItemBkColor(i,1,vecItem[i].color);
			m_listGraphItem.SetItemText(i,2,vecItem[i].strPointName);

			CString strType;
			strType.Format(_T("%d"),vecItem[i].nStoreType);
			m_listGraphItem.SetItemText(i,3,strType);
		}
		m_nTextColor = m_pPieGraphic->GetTextColor();
	}
	ShowDlgItem();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChartPropertyDlg::OnBnClickedButtonAddItem()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	COLORREF color = pMainFrm->m_colorArray[m_nColorIndex];
	m_nColorIndex++;
	if (m_nColorIndex >= 8)
	{
		m_nColorIndex = 0;
	}
	const int nCount = m_listGraphItem.GetItemCount();
	m_listGraphItem.InsertItem(nCount,L"新项");
	m_listGraphItem.SetItemBkColor(nCount,1,color);
	m_listGraphItem.SetItemText(nCount,2,L"绑定点名");
	m_listGraphItem.SetItemText(nCount,3,L"存储类型");
}

void CChartPropertyDlg::OnBnClickedButtonDeleteItem()
{
	if (AfxMessageBox(L"确认要删除吗?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_listGraphItem.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_listGraphItem.GetNextSelectedItem(pos);
			vec.push_back(nItem);
		}
		for (int i=vec.size()-1;i>=0;--i)
		{
			m_listGraphItem.DeleteItem(vec[i]);
		}
	}
}

void CChartPropertyDlg::ShowDlgItem()
{
	if (m_pBase->GetEqmType() == EQMDRAW_LINE_CHART)
	{
		EnableLeftBottomControls(TRUE);
		EnableItemControls(TRUE);
		EnableCompareBarControls(FALSE);
		EnableMaxValueControl(TRUE);
		m_combo_bar_type.EnableWindow(FALSE);
		EnableTimePointSelectControls(FALSE);
	}
	else if (m_pBase->GetEqmType() == EQMDRAW_BAR_CHART)
	{
		if (m_eBarType == eBarCompare)
		{
			m_combo_bar_type.EnableWindow(TRUE);
			m_combo_bar_type.SetCurSel(1);
			EnableCompareBarControls(TRUE);
			EnableLeftBottomControls(FALSE);
			EnableItemControls(FALSE);
			EnableMaxValueControl(TRUE);
			EnableTimePointSelectControls(FALSE);
		}
		else if(m_eBarType == eBarNormal)
		{
			m_combo_bar_type.EnableWindow(TRUE);
			m_combo_bar_type.SetCurSel(0);
			EnableCompareBarControls(FALSE);
			EnableLeftBottomControls(TRUE);
			EnableItemControls(TRUE);
			EnableMaxValueControl(TRUE);
			EnableTimePointSelectControls(TRUE);
		}
	}
	else if(m_pBase->GetEqmType() == EQMDRAW_PIE_CHART)
	{
		EnableLeftBottomControls(FALSE);
		EnableItemControls(TRUE);
		EnableCompareBarControls(FALSE);
		m_combo_bar_type.EnableWindow(FALSE);
		EnableMaxValueControl(FALSE);
		EnableTimePointSelectControls(FALSE);
	}
	else
	{
		ASSERT(0);
	}
}
		
void CChartPropertyDlg::ShowWindowTitle()
{
	if (m_pBase->GetEqmType() == EQMDRAW_LINE_CHART)
	{
		SetWindowText(L"折线图属性配置窗口");
	}
	else if (m_pBase->GetEqmType() == EQMDRAW_BAR_CHART)
	{
		SetWindowText(L"柱状图属性配置窗口");
	}
	else if(m_pBase->GetEqmType() == EQMDRAW_PIE_CHART)
	{
		SetWindowText(L"饼图属性配置窗口");
	}
	else
	{
		ASSERT(0);
	}
}

void CChartPropertyDlg::EnableLeftBottomControls( const BOOL bShow )
{
	GetDlgItem(IDC_STATIC_X_TITLE)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_X_UNIT)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_Y_TITLE)->EnableWindow(bShow);
	GetDlgItem(IDC_EDIT_X_TITLE)->EnableWindow(bShow);
	GetDlgItem(IDC_COMBO_X_UNIT)->EnableWindow(bShow);
	GetDlgItem(IDC_EDIT_Y_TITLE)->EnableWindow(bShow);
	GetDlgItem(IDC_CHECK_AXIS_Y_MIN)->EnableWindow(bShow);
	GetDlgItem(IDC_CHECK_AXIS_Y_MAX)->EnableWindow(bShow);
	GetDlgItem(IDC_EDIT_AXIS_Y_MIN)->EnableWindow(bShow);
	GetDlgItem(IDC_EDIT_AXIS_Y_MAX)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_X_TITLE4)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_X_TITLE3)->EnableWindow(bShow);
}

void CChartPropertyDlg::InitListCtrl()
{
	m_listGraphItem.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_listGraphItem.InsertColumn( 0, L"名称", LVCFMT_LEFT,90);
	m_listGraphItem.InsertColumn( 1, L"颜色", LVCFMT_LEFT,80);
	m_listGraphItem.InsertColumn( 2, L"绑定点", LVCFMT_LEFT,90);
	m_listGraphItem.InsertColumn( 3, L"存储类型", LVCFMT_LEFT,40);
}

void CChartPropertyDlg::InitComboCtrl()
{
	m_comboXUnit.ResetContent();
	m_comboXUnit.AddString(L"无");
	m_comboXUnit.AddString(L"自动");
	m_comboXUnit.AddString(L"年");
	m_comboXUnit.AddString(L"月");
	m_comboXUnit.AddString(L"年-月");
	m_comboXUnit.AddString(L"月-日");
	m_comboXUnit.AddString(L"月-日 时");
	m_comboXUnit.AddString(L"时:分");
	m_comboXUnit.SetCurSel(0);

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
}

void CChartPropertyDlg::SetPointMap( CDataPointManager* pPointMap )
{
	ASSERT(pPointMap);

	m_pPointMap = pPointMap;
}

void CChartPropertyDlg::OnNMDblclkListGraphItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CChartItemPropertyDlg dlg;
	dlg.SetPointMap(m_pPointMap);
	dlg.m_strItemName = m_listGraphItem.GetItemText(pNMItemActivate->iItem, 0);
	dlg.m_nColor = m_listGraphItem.GetItemBkColor(pNMItemActivate->iItem, 1);
	dlg.m_strBindPointName = m_listGraphItem.GetItemText(pNMItemActivate->iItem, 2);
	CString strType	= m_listGraphItem.GetItemText(pNMItemActivate->iItem, 3);
	dlg.m_nBindPointStoreType = _wtoi(strType);
	if (dlg.DoModal() == IDOK)
	{
		m_listGraphItem.SetItemText(pNMItemActivate->iItem, 0, dlg.m_strItemName);
		m_listGraphItem.SetItemBkColor(pNMItemActivate->iItem, 1, dlg.m_nColor);
		m_listGraphItem.SetItemText(pNMItemActivate->iItem, 2, dlg.m_strBindPointName);
		strType.Format(_T("%d"),dlg.m_nBindPointStoreType);
		m_listGraphItem.SetItemText(pNMItemActivate->iItem,3, strType);
	}
	*pResult = 0;
}


void CChartPropertyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_combo_bar_type.GetCurSel() == 0)
	{
		m_eBarType = eBarNormal;
	}
	else if (m_combo_bar_type.GetCurSel() == 1)
	{
		m_eBarType = eBarCompare;
	}
	GetDlgItem(IDC_EDIT_CHART_TITLE)->GetWindowText(m_strChartTitle);

	CMainFrame* pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrm != NULL);
	if (pMainFrm != NULL)
	{
		int nLen = m_strChartTitle.GetLength();
		if (nLen > pMainFrm->m_nItemNameLen)
		{
			CString strErr;
			strErr.Format(_T("名称长度不能大于%d！"), pMainFrm->m_nItemNameLen);
			MessageBox(strErr, _T("错误"), MB_OK|MB_ICONERROR);
			CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_CHART_TITLE));
			if (pEdit != NULL)
			{
				pEdit->SetSel(0, -1);
				pEdit->SetFocus();
			}
			return;
		}
	}

	CString strTemp;
	GetDlgItem(IDC_EDIT_UPDATE_INTERVAL)->GetWindowText(strTemp);
	m_nUpdateInterval = _wtoi(strTemp.GetString());
	GetDlgItem(IDC_EDIT_X_TITLE)->GetWindowText(m_strXName);
	GetDlgItem(IDC_STATIC_X_UNIT)->GetWindowTextW(m_strXUnit);
	GetDlgItem(IDC_EDIT_Y_TITLE)->GetWindowText(m_strYName);
	//if (m_nUpdateInterval == 0)
	//{
	//	AfxMessageBox(L"刷新间隔不能为0!");
	//	return;
	//}
	if (m_pBase->GetEqmType() == EQMDRAW_LINE_CHART)
	{
		//if (m_strMaxValue.GetLength() == 0)
		//{
		//	AfxMessageBox(L"量程不能为空!");
		//	return;
		//}
		m_pLineGraphic->SetType(m_eBarType);
		const int nCount = m_listGraphItem.GetItemCount();
		if (nCount > 0)
		{
			m_pLineGraphic->ClearGraphItem();
		}
		for (int i=0;i<nCount;++i)
		{
			_Graph_Item_Property item;
			item.strItemName = m_listGraphItem.GetItemText(i,0);
			item.color = m_listGraphItem.GetItemBkColor(i,1);
			item.strPointName = m_listGraphItem.GetItemText(i,2);
			item.nStoreType = _wtoi(m_listGraphItem.GetItemText(i,3));
			m_pLineGraphic->InsertGraphItem(item);
		}
		m_pLineGraphic->SetMaxValue(_wtof(m_strMaxValue.GetString()));

		m_nCheckMax = ((CButton*)GetDlgItem(IDC_CHECK_AXIS_Y_MAX))->GetCheck();
		m_nCheckMin = ((CButton*)GetDlgItem(IDC_CHECK_AXIS_Y_MIN))->GetCheck();
		m_pLineGraphic->SetMaxValue(m_dYMax);
		m_pLineGraphic->SetMinValue(m_dYMin);
		m_pLineGraphic->SetMaxCheck(m_nCheckMax);
		m_pLineGraphic->SetMinCheck(m_nCheckMin);

	}
	else if (m_pBase->GetEqmType() == EQMDRAW_BAR_CHART)
	{
		//if (m_strMaxValue.GetLength() == 0)
		//{
		//	AfxMessageBox(L"量程不能为空!");
		//	return;
		//}
		m_pBarGraphic->SetType(m_eBarType);
		GetDlgItem(IDC_EDIT_START_POINT)->GetWindowText(m_strStartPoint);
		m_nQueryType = m_combo_query_type.GetCurSel();
		m_pBarGraphic->SetStartPoint(m_strStartPoint);
		if (m_eBarType == eBarNormal)
		{
			const int nCount = m_listGraphItem.GetItemCount();
			if (nCount > 0)
			{
				m_pBarGraphic->ClearGraphItem();
			}
			for (int i=0;i<nCount;++i)
			{
				_Graph_Item_Property item;
				item.strItemName = m_listGraphItem.GetItemText(i,0);
				item.color = m_listGraphItem.GetItemBkColor(i,1);
				item.strPointName = m_listGraphItem.GetItemText(i,2);
				item.nStoreType = _wtoi(m_listGraphItem.GetItemText(i,3));
				m_pBarGraphic->InsertGraphItem(item);
			}
		}
		else if (m_eBarType == eBarCompare)
		{
			_Graph_Item_Property item1,item2;
			GetDlgItemText(IDC_EDIT_ITEM1_NAME, item1.strItemName);
			item1.color = 0;
			GetDlgItemText(IDC_EDIT_ASSO_POINT1, item1.strPointName);
			if (item1.strItemName.GetLength() == 0)
			{
				AfxMessageBox(L"项1名称不允许为空!");
				return;
			}
			if (item1.strPointName.GetLength() == 0)
			{
				AfxMessageBox(L"项1绑定点不允许为空!");
				return;
			}
			GetDlgItemText(IDC_EDIT_ITEM2_NAME, item2.strItemName);
			item2.color = 0;
			GetDlgItemText(IDC_EDIT_ASSO_POINT2, item2.strPointName);
			if (item2.strItemName.GetLength() == 0)
			{
				AfxMessageBox(L"项2名称不允许为空!");
				return;
			}
			if (item2.strPointName.GetLength() == 0)
			{
				AfxMessageBox(L"项2绑定点不允许为空!");
				return;
			}
			m_pBarGraphic->ClearGraphItem();
			m_pBarGraphic->InsertGraphItem(item1);
			m_pBarGraphic->InsertGraphItem(item2);
		}
		m_pBarGraphic->SetMaxValue(_wtof(m_strMaxValue.GetString()));
	}
	else if(m_pBase->GetEqmType() == EQMDRAW_PIE_CHART)
	{
		m_pPieGraphic->SetType(m_eBarType);
		const int nCount = m_listGraphItem.GetItemCount();
		if (nCount > 0)
		{
			m_pPieGraphic->ClearGraphItem();
		}
		for (int i=0;i<nCount;++i)
		{
			_Graph_Item_Property item;
			item.strItemName = m_listGraphItem.GetItemText(i,0);
			item.color = m_listGraphItem.GetItemBkColor(i,1);
			item.strPointName = m_listGraphItem.GetItemText(i,2);
			item.nStoreType = _wtoi(m_listGraphItem.GetItemText(i,3));
			m_pPieGraphic->InsertGraphItem(item);
		}
	}
	else
	{
		ASSERT(0);
	}
	CDialog::OnOK();
}

int CChartPropertyDlg::IsValidDate( COleDateTime startDate, COleDateTime endDate, COleDateTime startTime, COleDateTime endTime )
{
	if (startDate == endDate)
	{
		if (startTime <= endTime)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if (startDate < endDate)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void CChartPropertyDlg::InitBarComboCtrl()
{
	m_combo_bar_type.AddString(L"普通柱状图");
	m_combo_bar_type.AddString(L"比较柱状图");
	m_combo_bar_type.SetCurSel(0);
}


void CChartPropertyDlg::OnCbnSelchangeComboBarType()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_combo_bar_type.GetCurSel() == 0)
	{
		EnableCompareBarControls(FALSE);
		EnableLeftBottomControls(TRUE);
		EnableItemControls(TRUE);
		EnableMaxValueControl(TRUE);
		EnableTimePointSelectControls(TRUE);
		m_eBarType = eBarNormal;
	}
	else if (m_combo_bar_type.GetCurSel() == 1)
	{
		EnableCompareBarControls(TRUE);
		EnableLeftBottomControls(FALSE);
		EnableItemControls(FALSE);
		EnableMaxValueControl(TRUE);
		EnableTimePointSelectControls(FALSE);
		m_eBarType = eBarCompare;
	}
}


void CChartPropertyDlg::OnBnClickedButtonAssoPoint1()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataComConfigDlg dlg(*m_pPointMap);
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_ASSO_POINT1)->SetWindowTextW(dlg.m_strPointName);
	}
}

void CChartPropertyDlg::OnBnClickedButtonAssoPoint2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataComConfigDlg dlg(*m_pPointMap);
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_EDIT_ASSO_POINT2)->SetWindowTextW(dlg.m_strPointName);
	}
}

void CChartPropertyDlg::EnableCompareBarControls( const BOOL bShow )
{
	GetDlgItem(IDC_EDIT_ASSO_POINT1)->EnableWindow(bShow);
	GetDlgItem(IDC_EDIT_ASSO_POINT2)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_ASSO_POINT1)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_ASSO_POINT2)->EnableWindow(bShow);
	GetDlgItem(IDC_EDIT_ITEM1_NAME)->EnableWindow(bShow);
	GetDlgItem(IDC_EDIT_ITEM2_NAME)->EnableWindow(bShow);
}

void CChartPropertyDlg::EnableItemControls( const BOOL bShow )
{
	GetDlgItem(IDC_LIST_ITEM_PROPERTY)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_ADD_ITEM)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_DELETE_ITEM)->EnableWindow(bShow);
}

void CChartPropertyDlg::EnableMaxValueControl( const BOOL bShow )
{
	GetDlgItem(IDC_EDIT_CHART_MAX_VALUE)->EnableWindow(bShow);
}


void CChartPropertyDlg::OnBnClickedButtonStartPoint()
{
	CDataComConfigDlg dlg(*m_pPointMap);
	if (dlg.DoModal() == IDOK)
	{
		m_strStartPoint = dlg.m_strPointName;
		GetDlgItem(IDC_EDIT_START_POINT)->SetWindowTextW(m_strStartPoint);
	}
}

void CChartPropertyDlg::OnBnClickedButtonStartPointClear()
{
	m_strStartPoint = L"";
	GetDlgItem(IDC_EDIT_START_POINT)->SetWindowTextW(L"");
}

void CChartPropertyDlg::EnableTimePointSelectControls( const BOOL bShow )
{
	GetDlgItem(IDC_EDIT_START_POINT)->EnableWindow(bShow);
	GetDlgItem(IDC_COMBO_QUERY_TYPE_BAR)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_START_POINT)->EnableWindow(bShow);
	GetDlgItem(IDC_BUTTON_START_POINT_CLEAR)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_START_POINT)->EnableWindow(bShow);
	GetDlgItem(IDC_STATIC_QUERY_TYPE)->EnableWindow(bShow);
}


HBRUSH CChartPropertyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CChartPropertyDlg::OnStnClickedStaticTextColor()
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
