// ScrawlSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScrawlSettingDlg.h"
#include "afxdialogex.h"
#include "ScrawlItemSetDlg.h"
#include "MainFrm.h"


// CScrawlSettingDlg dialog

IMPLEMENT_DYNAMIC(CScrawlSettingDlg, CDialogEx)

CScrawlSettingDlg::CScrawlSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScrawlSettingDlg::IDD, pParent)
	, m_nLayer(4)
	, m_pMainFrame(NULL)
	, m_nColorIndexCurve(0)
	, m_nColorIndexColumn(0)
	, m_strShowWords(_T(""))
{

}

CScrawlSettingDlg::~CScrawlSettingDlg()
{
}

void CScrawlSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CURVE, m_listCurve);
	DDX_Control(pDX, IDC_LIST_COLUMN, m_listColumn);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_Text(pDX, IDC_EDIT_SHOW_WORDS, m_strShowWords);
}


BEGIN_MESSAGE_MAP(CScrawlSettingDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CURVE, &CScrawlSettingDlg::OnNMDblclkListCurve)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COLUMN, &CScrawlSettingDlg::OnNMDblclkListColumn)
	ON_BN_CLICKED(IDC_BTN_CURVE_ADD, &CScrawlSettingDlg::OnBnClickedBtnCurveAdd)
	ON_BN_CLICKED(IDC_BTN_CURVE_DEL, &CScrawlSettingDlg::OnBnClickedBtnCurveDel)
	ON_BN_CLICKED(IDC_BTN_COLUMN_ADD, &CScrawlSettingDlg::OnBnClickedBtnColumnAdd)
	ON_BN_CLICKED(IDC_BTN_COLUMN_DEL, &CScrawlSettingDlg::OnBnClickedBtnColumnDel)
END_MESSAGE_MAP()


// CScrawlSettingDlg message handlers


BOOL CScrawlSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitListControl();

	InitComboControl();

	InitListValue();

	m_pMainFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
	ASSERT(m_pMainFrame != NULL);
	if (NULL == m_pMainFrame)
	{
		MessageBox(_T("获取MainFrame指针失败！"), _T("错误"), MB_OK|MB_ICONERROR);
		return FALSE;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CScrawlSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_nLayer = m_comboLayer.GetCurSel();
	UpdateData(TRUE);

	CDialogEx::OnOK();
}


void CScrawlSettingDlg::OnNMDblclkListCurve(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OpenSettingDlg(pNMItemActivate->iItem, scrawlCurve);
}


void CScrawlSettingDlg::OnNMDblclkListColumn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	OpenSettingDlg(pNMItemActivate->iItem, scrawlColumn);
}


void CScrawlSettingDlg::OnBnClickedBtnCurveAdd()
{
	// TODO: Add your control notification handler code here
	CurveColumn	stCurveColumn;
	COLORREF color = m_pMainFrame->m_colorArray[m_nColorIndexCurve];
	++m_nColorIndexCurve;
	if (m_nColorIndexCurve >= 8)
	{
		m_nColorIndexCurve = 0;
	}
	stCurveColumn.color = color;
	stCurveColumn.scrawlType = scrawlCurve;
	m_vecCurve.push_back(stCurveColumn);

	InsertIntoListControl(stCurveColumn, scrawlCurve);
}


void CScrawlSettingDlg::OnBnClickedBtnCurveDel()
{
	// TODO: Add your control notification handler code here
	DeleteFromListControlAndMemory(scrawlCurve);
}


void CScrawlSettingDlg::OnBnClickedBtnColumnAdd()
{
	// TODO: Add your control notification handler code here
	CurveColumn	stCurveColumn;
	COLORREF color = m_pMainFrame->m_colorArray[m_nColorIndexColumn];
	++m_nColorIndexColumn;
	if (m_nColorIndexColumn >= 8)
	{
		m_nColorIndexColumn = 0;
	}
	stCurveColumn.color = color;
	stCurveColumn.scrawlType = scrawlColumn;
	m_vecColumn.push_back(stCurveColumn);

	InsertIntoListControl(stCurveColumn, scrawlColumn);
}


void CScrawlSettingDlg::OnBnClickedBtnColumnDel()
{
	// TODO: Add your control notification handler code here
	DeleteFromListControlAndMemory(scrawlColumn);
}

void CScrawlSettingDlg::InitListControl(void)
{
	m_listCurve.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCurve.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 80);
	m_listCurve.InsertColumn(1, _T("颜色"), LVCFMT_LEFT, 80);
	m_listCurve.InsertColumn(2, _T("最大值"), LVCFMT_LEFT, 80);
	m_listCurve.InsertColumn(3, _T("最小值"), LVCFMT_LEFT, 80);
	m_listCurve.InsertColumn(4, _T("注释"), LVCFMT_LEFT, 120);

	m_listColumn.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listColumn.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 80);
	m_listColumn.InsertColumn(1, _T("颜色"), LVCFMT_LEFT, 80);
	m_listColumn.InsertColumn(2, _T("最大值"), LVCFMT_LEFT, 80);
	m_listColumn.InsertColumn(3, _T("最小值"), LVCFMT_LEFT, 80);
	m_listColumn.InsertColumn(4, _T("注释"), LVCFMT_LEFT, 120);
}

void CScrawlSettingDlg::InitComboControl(void)
{
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

void CScrawlSettingDlg::InitListValue(void)
{
	vector<CurveColumn>::const_iterator	iter;
	for (iter=m_vecCurve.begin(); iter!=m_vecCurve.end(); ++iter)
	{
		InsertIntoListControl(*iter, scrawlCurve);
	}
	for (iter=m_vecColumn.begin(); iter!=m_vecColumn.end(); ++iter)
	{
		InsertIntoListControl(*iter, scrawlColumn);
	}
}

void CScrawlSettingDlg::SetLayer(const int nLayer)
{
	m_nLayer = nLayer;
}

void CScrawlSettingDlg::SetShowWords(const CString strShowWords)
{
	m_strShowWords = strShowWords;
}

void CScrawlSettingDlg::SetCurveColumn(const vector<CurveColumn>& vecCurve, const vector<CurveColumn>& vecColumn)
{
	m_vecCurve = vecCurve;
	m_vecColumn = vecColumn;
}

int	CScrawlSettingDlg::GetPicId(void) const
{
	return g_nPicIndexScrawl;
}

int CScrawlSettingDlg::GetLayer(void) const
{
	return m_nLayer;
}

CString CScrawlSettingDlg::GetShowWords(void) const
{
	return m_strShowWords;
}

void CScrawlSettingDlg::GetCurveColumn(vector<CurveColumn>& vecCurve, vector<CurveColumn>& vecColumn) const
{
	vecCurve = m_vecCurve;
	vecColumn = m_vecColumn;
}

void CScrawlSettingDlg::InsertIntoListControl(const CurveColumn& stCurveColumn, const ScrawlListType& listType)
{
	CString	strVal;
	int nCount = 0;

	if (scrawlCurve == listType)
	{
		nCount = m_listCurve.GetItemCount();
		m_listCurve.InsertItem(nCount, stCurveColumn.strPointName);

		m_listCurve.SetItemBkColor(nCount, 1, stCurveColumn.color);

		strVal.Format(_T("%d"), stCurveColumn.nMax);
		m_listCurve.SetItemText(nCount, 2, strVal);

		strVal.Format(_T("%d"), stCurveColumn.nMin);
		m_listCurve.SetItemText(nCount, 3, strVal);

		m_listCurve.SetItemText(nCount, 4, stCurveColumn.strRemarks);
	}
	else if (scrawlColumn == listType)
	{
		nCount = m_listColumn.GetItemCount();
		m_listColumn.InsertItem(nCount, stCurveColumn.strPointName);

		m_listColumn.SetItemBkColor(nCount, 1, stCurveColumn.color);

		strVal.Format(_T("%d"), stCurveColumn.nMax);
		m_listColumn.SetItemText(nCount, 2, strVal);

		strVal.Format(_T("%d"), stCurveColumn.nMin);
		m_listColumn.SetItemText(nCount, 3, strVal);

		m_listColumn.SetItemText(nCount, 4, stCurveColumn.strRemarks);
	}
	else
	{
		// error occurs
	}
}

void CScrawlSettingDlg::DeleteFromListControlAndMemory(const ScrawlListType& listType)
{
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;
	vector<CurveColumn>::const_iterator	iter;
	vector<int>::const_iterator			iterDel;

	if (scrawlCurve == listType)
	{
		pos = m_listCurve.GetFirstSelectedItemPosition();
		if (NULL == pos)
		{
			MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		while (pos != NULL)
		{
			nItem = m_listCurve.GetNextSelectedItem(pos);
			vecDel.push_back(nItem);
		}
		for (int i=vecDel.size()-1; i>=0; --i)
		{
			m_listCurve.DeleteItem(vecDel[i]);

			nItem = 0;
			for (iter=m_vecCurve.begin(); iter!=m_vecCurve.end(); ++iter,++nItem)
			{
				if (nItem == vecDel[i])
				{
					m_vecCurve.erase(iter);
					break;
				}
			}
		}
	}
	else if (scrawlColumn == listType)
	{
		pos = m_listColumn.GetFirstSelectedItemPosition();
		if (NULL == pos)
		{
			MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		while (pos != NULL)
		{
			nItem = m_listColumn.GetNextSelectedItem(pos);
			vecDel.push_back(nItem);
		}
		for (int i=vecDel.size()-1; i>=0; --i)
		{
			m_listColumn.DeleteItem(vecDel[i]);

			nItem = 0;
			for (iter=m_vecColumn.begin(); iter!=m_vecColumn.end(); ++iter,++nItem)
			{
				if (nItem == vecDel[i])
				{
					m_vecColumn.erase(iter);
					break;
				}
			}
		}
	}
	else
	{
		// error occurs
	}
}

void CScrawlSettingDlg::OpenSettingDlg(const int nCurSel, const ScrawlListType& listType)
{
	CScrawlItemSetDlg dlg(&(m_pMainFrame->m_plcpointmap));
	CurveColumn stInf;

	if (scrawlCurve == listType)
	{
		try
		{
			stInf = m_vecCurve.at(nCurSel);
		}
		catch (...)
		{
			MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		dlg.SetPointName(stInf.strPointName);
		dlg.SetColor(stInf.color);
		dlg.SetMaxValue(stInf.nMax);
		dlg.SetMinValue(stInf.nMin);
		dlg.SetRemarks(stInf.strRemarks);
		if (IDOK == dlg.DoModal())
		{
			vector<CurveColumn>::iterator	iter;
			int i = 0;
			for (iter=m_vecCurve.begin(); iter!=m_vecCurve.end(); ++iter,++i)
			{
				if (nCurSel == i)
				{
					iter->strPointName	= dlg.GetPointName();
					iter->color			= dlg.GetColor();
					iter->nMax			= dlg.GetMaxValue();
					iter->nMin			= dlg.GetMinValue();
					iter->strRemarks	= dlg.GetRemarks();
					break;
				}
			}

			m_listCurve.SetItemText(nCurSel, 0, dlg.GetPointName());
			m_listCurve.SetItemBkColor(nCurSel, 1, dlg.GetColor());

			CString strVal;
			strVal.Format(_T("%d"), dlg.GetMaxValue());
			m_listCurve.SetItemText(nCurSel, 2, strVal);

			strVal.Format(_T("%d"), dlg.GetMinValue());
			m_listCurve.SetItemText(nCurSel, 3, strVal);
		
			m_listCurve.SetItemText(nCurSel, 4, dlg.GetRemarks());
		}
	}
	else if (scrawlColumn == listType)
	{
		try
		{
			stInf = m_vecColumn.at(nCurSel);
		}
		catch (...)
		{
			MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}

		dlg.SetPointName(stInf.strPointName);
		dlg.SetColor(stInf.color);
		dlg.SetMaxValue(stInf.nMax);
		dlg.SetMinValue(stInf.nMin);
		dlg.SetRemarks(stInf.strRemarks);
		if (IDOK == dlg.DoModal())
		{
			vector<CurveColumn>::iterator	iter;
			int i = 0;
			for (iter=m_vecColumn.begin(); iter!=m_vecColumn.end(); ++iter,++i)
			{
				if (nCurSel == i)
				{
					iter->strPointName	= dlg.GetPointName();
					iter->color			= dlg.GetColor();
					iter->nMax			= dlg.GetMaxValue();
					iter->nMin			= dlg.GetMinValue();
					iter->strRemarks	= dlg.GetRemarks();
					break;
				}
			}

			m_listColumn.SetItemText(nCurSel, 0, dlg.GetPointName());
			m_listColumn.SetItemBkColor(nCurSel, 1, dlg.GetColor());

			CString strVal;
			strVal.Format(_T("%d"), dlg.GetMaxValue());
			m_listColumn.SetItemText(nCurSel, 2, strVal);

			strVal.Format(_T("%d"), dlg.GetMinValue());
			m_listColumn.SetItemText(nCurSel, 3, strVal);

			m_listColumn.SetItemText(nCurSel, 4, dlg.GetRemarks());
		}
	}
	else
	{
		// error occurs
	}
}
