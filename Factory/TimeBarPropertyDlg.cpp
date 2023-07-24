// TimeBarPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TimeBarPropertyDlg.h"
//#include "afxdialogex.h"
#include "../ComponentDraw/EqmDrawTimeBar.h"
#include "DataPoint/DataPointManager.h"
#include "DataComConfigDlg.h"
#include "TimeBarItemPropertyDlg.h"
#include "Tools/CustomTools/CustomTools.h"
// CTimeBarPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CTimeBarPropertyDlg, CDialog)

CTimeBarPropertyDlg::CTimeBarPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeBarPropertyDlg::IDD, pParent)
{

}

CTimeBarPropertyDlg::CTimeBarPropertyDlg( CEqmDrawTimeBar* pTimeBar, CDataPointManager* pMap, CWnd* pParent /*= NULL*/ ): CDialog(CTimeBarPropertyDlg::IDD, pParent),
m_pTimeBar(pTimeBar),
m_pMap(pMap)
{
	
}

CTimeBarPropertyDlg::~CTimeBarPropertyDlg()
{
}

void CTimeBarPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TIME_BAR_ITEM, m_list);
	DDX_Control(pDX, IDC_COMBO_TIME_BAR_LAYER, m_combo_layer);
	DDX_Control(pDX, IDC_COMBO_QUERY_TYPE, m_combo_query_type);
}


BEGIN_MESSAGE_MAP(CTimeBarPropertyDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_TIME_BAR_COLOR, &CTimeBarPropertyDlg::OnStnClickedStaticTimeBarColor)
	ON_BN_CLICKED(IDC_BUTTON1_TIME_BAR_SELECT_POINT, &CTimeBarPropertyDlg::OnBnClickedButton1TimeBarSelectPoint)
	ON_BN_CLICKED(IDC_BUTTON_TIME_BAR_ADD_ITEM, &CTimeBarPropertyDlg::OnBnClickedButtonTimeBarAddItem)
	ON_BN_CLICKED(IDC_BUTTON_TIME_BAR_DELETE_ITEM, &CTimeBarPropertyDlg::OnBnClickedButtonTimeBarDeleteItem)
	ON_BN_CLICKED(IDC_BUTTON_TIME_BAR_UP_ITEM, &CTimeBarPropertyDlg::OnBnClickedButtonTimeBarUpItem)
	ON_BN_CLICKED(IDC_BUTTON_TIME_BAR_DOWN_ITEM, &CTimeBarPropertyDlg::OnBnClickedButtonTimeBarDownItem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TIME_BAR_ITEM, &CTimeBarPropertyDlg::OnNMDblclkListTimeBarItem)
	ON_BN_CLICKED(IDOK, &CTimeBarPropertyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1_TIME_BAR_SELECT_POINT3, &CTimeBarPropertyDlg::OnBnClickedButton1TimeBarSelectPoint3)
END_MESSAGE_MAP()


// CTimeBarPropertyDlg 消息处理程序


BOOL CTimeBarPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

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
	m_combo_layer.SetCurSel((int)m_pTimeBar->GetLayer());

	m_combo_query_type.AddString(L"分钟");
	m_combo_query_type.AddString(L"小时");
	m_combo_query_type.AddString(L"天");
	m_combo_query_type.AddString(L"周");
	m_combo_query_type.AddString(L"月");
	m_combo_query_type.AddString(L"年");

	InitListControl();
	if (m_pTimeBar)
	{
		GetDlgItem(IDC_EDIT_TIME_BAR_BIND_POINT)->SetWindowTextW(m_pTimeBar->GetBindPointStart());
		m_combo_query_type.SetCurSel(m_pTimeBar->GetQueryType());
		GetDlgItem(IDC_EDIT_TIME_BAR_BIND_POINT_STATUS)->SetWindowTextW(m_pTimeBar->GetBindPointStatus());

		const vector<_tag_bar_item>& vec = m_pTimeBar->GetItemVec();
		const size_t size = vec.size();
		for (size_t i=0;i<size;++i)
		{
			CString strIndex;
			strIndex.Format(L"%d",vec[i].nIndex);
			m_list.InsertItem(i,strIndex);
			m_list.SetItemText(i,1,vec[i].strName);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CTimeBarPropertyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIME_BAR_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_pTimeBar->GetBarColor());
		if (hBrush)
		{
			return hBrush;
		}
	}
	return hbr;
}


void CTimeBarPropertyDlg::OnStnClickedStaticTimeBarColor()
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
	CHOOSECOLOR cc = {sizeof( CHOOSECOLOR ),m_hWnd,NULL,m_pTimeBar->GetBarColor(),cusColor,CC_FULLOPEN,NULL,NULL,NULL};
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
		m_pTimeBar->SetBarColor(cc.rgbResult);
		Invalidate();
	}
}

void CTimeBarPropertyDlg::OnBnClickedButton1TimeBarSelectPoint()
{
	CDataComConfigDlg dlg(*m_pMap);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strPointName.GetLength() > 0)
		{
			GetDlgItem(IDC_EDIT_TIME_BAR_BIND_POINT)->SetWindowTextW(dlg.m_strPointName);
			m_pTimeBar->SetBindPointStart(dlg.m_strPointName);
		}
	}
}

void CTimeBarPropertyDlg::InitListControl()
{
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_list.InsertColumn(0,_T("序号"),LVCFMT_LEFT, 80);
	m_list.InsertColumn(1,_T("项名称"),LVCFMT_LEFT, 180);
}

void CTimeBarPropertyDlg::OnBnClickedButtonTimeBarAddItem()
{
	const int nCount = m_list.GetItemCount();
	CString strIndex;
	strIndex.Format(L"%d",nCount+1);
	m_list.InsertItem(nCount,strIndex);
	m_list.SetItemText(nCount,1,L"请双击该行编辑!");
}


void CTimeBarPropertyDlg::OnBnClickedButtonTimeBarDeleteItem()
{
	if (AfxMessageBox(L"确认要删除吗?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_list.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_list.GetNextSelectedItem(pos);
			vec.push_back(nItem);
		}
		for (int i=vec.size()-1;i>=0;--i)
		{
			m_list.DeleteItem(vec[i]);
		}
	}
}


void CTimeBarPropertyDlg::OnBnClickedButtonTimeBarUpItem()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("No items were selected!\n"));
		return;
	}
	if(pos)
	{
		int nItem = m_list.GetNextSelectedItem(pos);
		if (nItem > 0)
		{
			CString strIndex = m_list.GetItemText(nItem,0);
			CString strName = m_list.GetItemText(nItem,1);
			m_list.DeleteItem(nItem);
			m_list.InsertItem(nItem-1,strIndex);
			m_list.SetItemText(nItem-1,1,strName);
		}
	}
}


void CTimeBarPropertyDlg::OnBnClickedButtonTimeBarDownItem()
{
	const int nCount = m_list.GetItemCount();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(_T("No items were selected!\n"));
		return;
	}
	if(pos)
	{
		int nItem = m_list.GetNextSelectedItem(pos);
		if (nItem < nCount-1)
		{
			CString strIndex = m_list.GetItemText(nItem,0);
			CString strName = m_list.GetItemText(nItem,1);
			m_list.DeleteItem(nItem);
			m_list.InsertItem(nItem+1,strIndex);
			m_list.SetItemText(nItem+1,1,strName);
		}
	}
}


void CTimeBarPropertyDlg::OnNMDblclkListTimeBarItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	const int nItem = pNMItemActivate->iItem;
	CTimeBarItemPropertyDlg dlg;
	dlg.m_strIndex = m_list.GetItemText(nItem,0);
	dlg.m_strName = m_list.GetItemText(nItem,1);
	if(dlg.DoModal() == IDOK)
	{
		m_list.SetItemText(nItem,0,dlg.m_strIndex);
		m_list.SetItemText(nItem,1,dlg.m_strName);
	}
	*pResult = 0;
}


void CTimeBarPropertyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strBind;
	GetDlgItem(IDC_EDIT_TIME_BAR_BIND_POINT)->GetWindowTextW(strBind);
	m_pTimeBar->SetBindPointStart(strBind);
	GetDlgItem(IDC_EDIT_TIME_BAR_BIND_POINT_STATUS)->GetWindowTextW(strBind);
	m_pTimeBar->SetBindPointStatus(strBind);

	m_pTimeBar->SetLayer(m_combo_layer.GetCurSel());
	m_pTimeBar->SetQueryType(m_combo_query_type.GetCurSel());
	m_pTimeBar->DeleteAllItems();
	const int nCount = m_list.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		CString strIndex = m_list.GetItemText(i,0);
		CString strName = m_list.GetItemText(i,1);
		CString strBindPoint = m_list.GetItemText(i,2);
		_tag_bar_item item;
		item.nIndex = _wtoi(strIndex.GetString());
		item.strName = strName;
		m_pTimeBar->InsertItem(item);
	}
	CDialog::OnOK();
}

void CTimeBarPropertyDlg::OnBnClickedButton1TimeBarSelectPoint3()
{
	CDataComConfigDlg dlg(*m_pMap);
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strPointName.GetLength() > 0)
		{
			GetDlgItem(IDC_EDIT_TIME_BAR_BIND_POINT_STATUS)->SetWindowTextW(dlg.m_strPointName);
			m_pTimeBar->SetBindPointStatus(dlg.m_strPointName);
		}
	}
}
