// PIFManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PIFManageDlg.h"
#include "afxdialogex.h"
#include "PIFEditDlg.h"

// CPIFManageDlg 对话框

IMPLEMENT_DYNAMIC(CPIFManageDlg, CSkinDlg)

CPIFManageDlg::CPIFManageDlg(CWnd* pParent, CLogicPointManager&  logicPointMap)
	: CSkinDlg(CPIFManageDlg::IDD, pParent)
	,m_LogicPtManager(logicPointMap)
{
	m_bSaved = true;
}

CPIFManageDlg::~CPIFManageDlg()
{
}

void CPIFManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PIF, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CPIFManageDlg, CSkinDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PIF, &CPIFManageDlg::OnNMDblclkListPif)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PIF, &CPIFManageDlg::OnBnClickedButtonAddPif)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PIF, &CPIFManageDlg::OnBnClickedButtonDeletePif)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LOGIC, &CPIFManageDlg::OnBnClickedButtonSaveLogic)
	ON_BN_CLICKED(IDOK, &CPIFManageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPIFManageDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPIFManageDlg 消息处理程序


BOOL CPIFManageDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//移动按钮
	{
		{
			CRect rectBtn;
			GetDlgItem(IDOK)->GetWindowRect(&rectBtn);
			rectBtn.top	+= 15;
			rectBtn.bottom	+= 15;
			ScreenToClient(&rectBtn);
			GetDlgItem(IDOK)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
		}
		{
			CRect rectBtn;
			GetDlgItem(IDCANCEL)->GetWindowRect(&rectBtn);
			rectBtn.top	+= 15;
			rectBtn.bottom	+= 15;
			ScreenToClient(&rectBtn);
			GetDlgItem(IDCANCEL)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
		}
	}
	InitList();
	vector< CLogicDataPointEntry>& logic_list = m_LogicPtManager.GetPointList();
	const size_t size = logic_list.size();
	for (size_t i=0;i<size;++i)
	{
		CString strIndex;
		strIndex.Format(L"%d",i+1);
		m_listCtrl.InsertItem(i,strIndex);
		m_listCtrl.SetItemText(i,1,logic_list[i].GetNameID().c_str());
		m_listCtrl.SetItemText(i,2,logic_list[i].GetLogicType().c_str());
		m_listCtrl.SetItemText(i,3,logic_list[i].GetDescription().c_str());
		m_listCtrl.SetItemText(i,4,logic_list[i].GetParam(1).c_str());
		m_listCtrl.SetItemText(i,5,logic_list[i].GetParam(2).c_str());
		m_listCtrl.SetItemText(i,6,logic_list[i].GetParam(3).c_str());
		m_listCtrl.SetItemText(i,7,logic_list[i].GetParam(4).c_str());
		m_listCtrl.SetItemText(i,8,logic_list[i].GetParam(5).c_str());
		m_listCtrl.SetItemText(i,9,logic_list[i].GetParam(6).c_str());
		m_listCtrl.SetItemText(i,10,logic_list[i].GetParam(7).c_str());
		m_listCtrl.SetItemText(i,11,logic_list[i].GetParam(8).c_str());
		m_listCtrl.SetItemText(i,12,logic_list[i].GetParam(9).c_str());
		m_listCtrl.SetItemText(i,13,logic_list[i].GetParam(10).c_str());
		m_listCtrl.SetItemText(i,14,logic_list[i].GetParam(11).c_str());
		m_listCtrl.SetItemText(i,15,logic_list[i].GetParam(12).c_str());
		m_listCtrl.SetItemText(i,16,logic_list[i].GetParam(13).c_str());
		m_listCtrl.SetItemText(i,17,logic_list[i].GetParam(14).c_str());
		m_listCtrl.SetItemText(i,18,logic_list[i].GetParam(15).c_str());
		m_listCtrl.SetItemText(i,19,logic_list[i].GetParam(16).c_str());
		m_listCtrl.SetItemText(i,20,logic_list[i].GetParam(17).c_str());
		m_listCtrl.SetItemText(i,21,logic_list[i].GetParam(18).c_str());
		m_listCtrl.SetItemText(i,22,logic_list[i].GetParam(19).c_str());
		m_listCtrl.SetItemText(i,23,logic_list[i].GetParam(20).c_str());
		m_listCtrl.SetItemText(i,24,logic_list[i].GetParam(21).c_str());
		m_listCtrl.SetItemText(i,25,logic_list[i].GetParam(22).c_str());
		m_listCtrl.SetItemText(i,26,logic_list[i].GetParam(23).c_str());
		m_listCtrl.SetItemText(i,27,logic_list[i].GetParam(24).c_str());
		m_listCtrl.SetItemText(i,28,logic_list[i].GetParam(25).c_str());
		m_listCtrl.SetItemText(i,29,logic_list[i].GetParam(26).c_str());
		m_listCtrl.SetItemText(i,30,logic_list[i].GetParam(27).c_str());
		m_listCtrl.SetItemText(i,31,logic_list[i].GetParam(28).c_str());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPIFManageDlg::InitList()
{
	m_listCtrl.SetBkColor(RGB(235, 235, 235));
	m_listCtrl.SetTextColor(RGB(0, 0, 128));

	m_listCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_listCtrl.InsertColumn( 0, _T("序号"), LVCFMT_LEFT, 70 );
	m_listCtrl.InsertColumn( 1, _T("名称"), LVCFMT_LEFT, 100 );
	m_listCtrl.InsertColumn( 2, _T("类型"), LVCFMT_LEFT, 100 );
	m_listCtrl.InsertColumn( 3, _T("释义"), LVCFMT_LEFT, 100 );
	m_listCtrl.InsertColumn( 4, _T("参数1"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 5, _T("参数2"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 6, _T("参数3"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 7, _T("参数4"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 8, _T("参数5"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 9, _T("参数6"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 10, _T("参数7"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 11, _T("参数8"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 12, _T("参数9"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 13, _T("参数10"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 14, _T("参数11"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 15, _T("参数12"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 16, _T("参数13"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 17, _T("参数14"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 18, _T("参数15"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 19, _T("参数16"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 20, _T("参数17"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 21, _T("参数18"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 22, _T("参数19"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 23, _T("参数20"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 24, _T("参数21"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 25, _T("参数22"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 26, _T("参数23"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 27, _T("参数24"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 28, _T("参数25"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 29, _T("参数26"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 30, _T("参数27"), LVCFMT_LEFT, 80 );
	m_listCtrl.InsertColumn( 31, _T("参数28"), LVCFMT_LEFT, 80 );
}


void CPIFManageDlg::OnNMDblclkListPif(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	const int nItem = pNMItemActivate->iItem;
	if (nItem < 0)
	{
		return;
	}
	CPIFEditDlg dlg(this);
	dlg.m_strName = m_listCtrl.GetItemText(nItem, 1);
	dlg.m_strType = m_listCtrl.GetItemText(nItem, 2);
	dlg.m_strRemark = m_listCtrl.GetItemText(nItem, 3);
	dlg.m_strParam1 = m_listCtrl.GetItemText(nItem, 4);
	dlg.m_strParam2 = m_listCtrl.GetItemText(nItem, 5);
	dlg.m_strParam3 = m_listCtrl.GetItemText(nItem, 6);
	dlg.m_strParam4 = m_listCtrl.GetItemText(nItem, 7);
	dlg.m_strParam5 = m_listCtrl.GetItemText(nItem, 8);
	dlg.m_strParam6 = m_listCtrl.GetItemText(nItem, 9);
	dlg.m_strParam7 = m_listCtrl.GetItemText(nItem, 10);
	dlg.m_strParam8 = m_listCtrl.GetItemText(nItem, 11);
	dlg.m_strParam9 = m_listCtrl.GetItemText(nItem, 12);
	dlg.m_strParam10 = m_listCtrl.GetItemText(nItem, 13);
	dlg.m_strParam11 = m_listCtrl.GetItemText(nItem, 14);
	dlg.m_strParam12 = m_listCtrl.GetItemText(nItem, 15);
	dlg.m_strParam13 = m_listCtrl.GetItemText(nItem, 16);
	dlg.m_strParam14 = m_listCtrl.GetItemText(nItem, 17);
	dlg.m_strParam15 = m_listCtrl.GetItemText(nItem, 18);
	dlg.m_strParam16 = m_listCtrl.GetItemText(nItem, 19);
	dlg.m_strParam17 = m_listCtrl.GetItemText(nItem, 20);
	dlg.m_strParam18 = m_listCtrl.GetItemText(nItem, 21);
	dlg.m_strParam19 = m_listCtrl.GetItemText(nItem, 22);
	dlg.m_strParam20 = m_listCtrl.GetItemText(nItem, 23);
	dlg.m_strParam21 = m_listCtrl.GetItemText(nItem, 24);
	dlg.m_strParam22 = m_listCtrl.GetItemText(nItem, 25);
	dlg.m_strParam23 = m_listCtrl.GetItemText(nItem, 26);
	dlg.m_strParam24 = m_listCtrl.GetItemText(nItem, 27);
	dlg.m_strParam25 = m_listCtrl.GetItemText(nItem, 28);
	dlg.m_strParam26 = m_listCtrl.GetItemText(nItem, 29);
	dlg.m_strParam27 = m_listCtrl.GetItemText(nItem, 30);
	dlg.m_strParam28 = m_listCtrl.GetItemText(nItem, 31);
	if (dlg.DoModal() == IDOK)
	{
		 m_listCtrl.SetItemText(nItem, 1 , dlg.m_strName);
		 m_listCtrl.SetItemText(nItem, 2 , dlg.m_strType);
		 m_listCtrl.SetItemText(nItem, 3 , dlg.m_strRemark);
		 m_listCtrl.SetItemText(nItem, 4 , dlg.m_strParam1);
		 m_listCtrl.SetItemText(nItem, 5 , dlg.m_strParam2);
		 m_listCtrl.SetItemText(nItem, 6 , dlg.m_strParam3);
		 m_listCtrl.SetItemText(nItem, 7 , dlg.m_strParam4);
		 m_listCtrl.SetItemText(nItem, 8 , dlg.m_strParam5);
		 m_listCtrl.SetItemText(nItem, 9 , dlg.m_strParam6);
		 m_listCtrl.SetItemText(nItem, 10 , dlg.m_strParam7);
		 m_listCtrl.SetItemText(nItem, 11 , dlg.m_strParam8);
		 m_listCtrl.SetItemText(nItem, 12 , dlg.m_strParam9);
		 m_listCtrl.SetItemText(nItem, 13 , dlg.m_strParam10);
		 m_listCtrl.SetItemText(nItem, 14 , dlg.m_strParam11);
		 m_listCtrl.SetItemText(nItem, 15 , dlg.m_strParam12);
		 m_listCtrl.SetItemText(nItem, 16 , dlg.m_strParam13);
		 m_listCtrl.SetItemText(nItem, 17 , dlg.m_strParam14);
		 m_listCtrl.SetItemText(nItem, 18 , dlg.m_strParam15);
		 m_listCtrl.SetItemText(nItem, 19 , dlg.m_strParam16);
		 m_listCtrl.SetItemText(nItem, 20 , dlg.m_strParam17);
		 m_listCtrl.SetItemText(nItem, 21 , dlg.m_strParam18);
		 m_listCtrl.SetItemText(nItem, 22 , dlg.m_strParam19);
		 m_listCtrl.SetItemText(nItem, 23 , dlg.m_strParam20);
		 m_listCtrl.SetItemText(nItem, 24 , dlg.m_strParam21);
		 m_listCtrl.SetItemText(nItem, 25 , dlg.m_strParam22);
		 m_listCtrl.SetItemText(nItem, 26 , dlg.m_strParam23);
		 m_listCtrl.SetItemText(nItem, 27 , dlg.m_strParam24);
		 m_listCtrl.SetItemText(nItem, 28 , dlg.m_strParam25);
		 m_listCtrl.SetItemText(nItem, 29 , dlg.m_strParam26);
		 m_listCtrl.SetItemText(nItem, 30 , dlg.m_strParam27);
		 m_listCtrl.SetItemText(nItem, 31 , dlg.m_strParam28);
	}
	m_bSaved = false;
	*pResult = 0;
}


void CPIFManageDlg::OnBnClickedButtonAddPif()
{
	// TODO: 在此添加控件通知处理程序代码
	const int nCount = m_listCtrl.GetItemCount();
	CString strIndex;
	strIndex.Format(L"%d",nCount+1);
	m_listCtrl.InsertItem(nCount, strIndex);
	m_listCtrl.SetItemText(nCount,1,L"请双击编辑");
	m_listCtrl.SetItemText(nCount,2,L"PIF");
	m_listCtrl.SetItemText(nCount,3,L"");
	m_listCtrl.SetItemText(nCount,4,L"");
	m_listCtrl.SetItemText(nCount,5,L"");
	m_listCtrl.SetItemText(nCount,6,L"");
	m_listCtrl.SetItemText(nCount,7,L"");
	m_listCtrl.SetItemText(nCount,8,L"");
	m_listCtrl.SetItemText(nCount,9,L"");
	m_listCtrl.SetItemText(nCount,10,L"");
	m_listCtrl.SetItemText(nCount,11,L"");
	m_listCtrl.SetItemText(nCount,12,L"");
	m_listCtrl.SetItemText(nCount,13,L"");
	m_listCtrl.SetItemText(nCount,14,L"");
	m_listCtrl.SetItemText(nCount,15,L"");
	m_listCtrl.SetItemText(nCount,16,L"");
	m_listCtrl.SetItemText(nCount,17,L"");
	m_listCtrl.SetItemText(nCount,18,L"");
	m_listCtrl.SetItemText(nCount,19,L"");
	m_listCtrl.SetItemText(nCount,20,L"");
	m_listCtrl.SetItemText(nCount,21,L"");
	m_listCtrl.SetItemText(nCount,22,L"");
	m_listCtrl.SetItemText(nCount,23,L"");
	m_listCtrl.SetItemText(nCount,24,L"");
	m_listCtrl.SetItemText(nCount,25,L"");
	m_listCtrl.SetItemText(nCount,26,L"");
	m_listCtrl.SetItemText(nCount,27,L"");
	m_listCtrl.SetItemText(nCount,28,L"");
	m_listCtrl.SetItemText(nCount,29,L"");
	m_listCtrl.SetItemText(nCount,30,L"");
	m_listCtrl.SetItemText(nCount,31,L"");
	m_bSaved = false;
}


void CPIFManageDlg::OnBnClickedButtonDeletePif()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox(L"确认要删除吗?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_listCtrl.GetNextSelectedItem(pos);
			vec.push_back(nItem);
		}
		for (int i=vec.size()-1;i>=0;--i)
		{
			m_listCtrl.DeleteItem(vec[i]);
		}
		ResetIndex();
		m_bSaved = false;
	}
}

void CPIFManageDlg::ResetIndex()
{
	int nCount = m_listCtrl.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		CString strIndex;
		strIndex.Format(L"%d",i+1);
		m_listCtrl.SetItemText(i,0,strIndex);
	}
}

void CPIFManageDlg::OnBnClickedButtonSaveLogic()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LogicPtManager.Clear();

	int nCount = m_listCtrl.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		CLogicDataPointEntry entry;
		entry.SetNameID(m_listCtrl.GetItemText(i, 1).GetString());
		entry.SetLogicType(m_listCtrl.GetItemText(i, 2).GetString());
		entry.SetDescription(m_listCtrl.GetItemText(i, 3).GetString());
		entry.SetParam(1,m_listCtrl.GetItemText(i, 4).GetString());
		entry.SetParam(2,m_listCtrl.GetItemText(i, 5).GetString());
		entry.SetParam(3,m_listCtrl.GetItemText(i, 6).GetString());
		entry.SetParam(4,m_listCtrl.GetItemText(i, 7).GetString());
		entry.SetParam(5,m_listCtrl.GetItemText(i, 8).GetString());
		entry.SetParam(6,m_listCtrl.GetItemText(i, 9).GetString());
		entry.SetParam(7,m_listCtrl.GetItemText(i, 10).GetString());
		entry.SetParam(8,m_listCtrl.GetItemText(i, 11).GetString());
		entry.SetParam(9,m_listCtrl.GetItemText(i, 12).GetString());
		entry.SetParam(10,m_listCtrl.GetItemText(i, 13).GetString());
		entry.SetParam(11,m_listCtrl.GetItemText(i, 14).GetString());
		entry.SetParam(12,m_listCtrl.GetItemText(i, 15).GetString());
		entry.SetParam(13,m_listCtrl.GetItemText(i, 16).GetString());
		entry.SetParam(14,m_listCtrl.GetItemText(i, 17).GetString());
		entry.SetParam(15,m_listCtrl.GetItemText(i, 18).GetString());
		entry.SetParam(16,m_listCtrl.GetItemText(i, 19).GetString());
		entry.SetParam(17,m_listCtrl.GetItemText(i, 20).GetString());
		entry.SetParam(18,m_listCtrl.GetItemText(i, 21).GetString());
		entry.SetParam(19,m_listCtrl.GetItemText(i, 22).GetString());
		entry.SetParam(20,m_listCtrl.GetItemText(i, 23).GetString());
		entry.SetParam(21,m_listCtrl.GetItemText(i, 24).GetString());
		entry.SetParam(22,m_listCtrl.GetItemText(i, 25).GetString());
		entry.SetParam(23,m_listCtrl.GetItemText(i, 26).GetString());
		entry.SetParam(24,m_listCtrl.GetItemText(i, 27).GetString());
		entry.SetParam(25,m_listCtrl.GetItemText(i, 28).GetString());
		entry.SetParam(26,m_listCtrl.GetItemText(i, 29).GetString());
		entry.SetParam(27,m_listCtrl.GetItemText(i, 30).GetString());
		entry.SetParam(28,m_listCtrl.GetItemText(i, 31).GetString());
		m_LogicPtManager.AddEntry(entry);
	}
	m_bSaved = true;
}

void CPIFManageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bSaved)
	{
		OnBnClickedButtonSaveLogic();
	}
	CSkinDlg::OnOK();
}

void CPIFManageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bSaved)
	{
		if (AfxMessageBox(L"确定要保存到逻辑点表吗?",MB_OKCANCEL) == IDOK)
		{
			OnBnClickedButtonSaveLogic();
		}
	}
	CSkinDlg::OnCancel();
}

void CPIFManageDlg::OnBnClickedClose()
{
	if (!m_bSaved)
	{
		if (AfxMessageBox(L"确定要保存到逻辑点表吗?",MB_OKCANCEL) == IDOK)
		{
			OnBnClickedButtonSaveLogic();
		}
	}
	CSkinDlg::OnCancel();
}
