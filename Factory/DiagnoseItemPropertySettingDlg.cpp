// DiagnoseItemPropertySettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DiagnoseItemPropertySettingDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"
#include "SubItemPropertySettingDlg.h"


// CDiagnoseItemPropertySettingDlg 对话框

IMPLEMENT_DYNAMIC(CDiagnoseItemPropertySettingDlg, CDialog)

CDiagnoseItemPropertySettingDlg::CDiagnoseItemPropertySettingDlg( _tagDiagnoseItem* pItem, CWnd* pParent /*= NULL*/ )
	: CDialog(CDiagnoseItemPropertySettingDlg::IDD, pParent),m_pItem(pItem),m_nColor(pItem->color)
{
}

CDiagnoseItemPropertySettingDlg::~CDiagnoseItemPropertySettingDlg()
{
}

void CDiagnoseItemPropertySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIAGNOSE_SUBITEM, m_list_subitem);
}


BEGIN_MESSAGE_MAP(CDiagnoseItemPropertySettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_ITEM_POINT_SELECT, &CDiagnoseItemPropertySettingDlg::OnBnClickedButtonDiagnoseItemPointSelect)
	ON_STN_CLICKED(IDC_STATIC_DIAGNOSE_ITEM_COLOR, &CDiagnoseItemPropertySettingDlg::OnStnClickedStaticDiagnoseItemColor)
	ON_BN_CLICKED(IDOK, &CDiagnoseItemPropertySettingDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_SUBITEM_ADD, &CDiagnoseItemPropertySettingDlg::OnBnClickedButtonDiagnoseSubitemAdd)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_SUBITEM_DELETE, &CDiagnoseItemPropertySettingDlg::OnBnClickedButtonDiagnoseSubitemDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIAGNOSE_SUBITEM, &CDiagnoseItemPropertySettingDlg::OnNMDblclkListDiagnoseSubitem)
END_MESSAGE_MAP()


// CDiagnoseItemPropertySettingDlg 消息处理程序
void CDiagnoseItemPropertySettingDlg::OnBnClickedButtonDiagnoseItemPointSelect()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pMainFrm->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_pItem->strPointName = dlg.m_strPointName;
		GetDlgItem(IDC_EDIT2_DIAGNOSE_ITEM_POINT_BINDING)->SetWindowTextW(m_pItem->strPointName);
	}
}


void CDiagnoseItemPropertySettingDlg::OnStnClickedStaticDiagnoseItemColor()
{
	return;
	COLORREF cusColor[16];
	memset( cusColor, 0, sizeof( COLORREF) * 16 );
	CHOOSECOLOR cc = {sizeof( CHOOSECOLOR ),m_hWnd,NULL,m_nColor,cusColor,CC_FULLOPEN,NULL,NULL,NULL};
	if (ChooseColor(&cc))
	{
		m_nColor = cc.rgbResult;
		Invalidate();
	}
}


void CDiagnoseItemPropertySettingDlg::OnBnClickedOk()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_DIAGNOSE_ITEM_NAME)->GetWindowTextW(strTemp);
	if (_T("") == strTemp)
	{
		MessageBox(_T("请输入项名称！"), _T("错误"), MB_OK|MB_ICONERROR);
		GetDlgItem(IDC_EDIT_DIAGNOSE_ITEM_NAME)->SetFocus();
		return;
	}
	m_pItem->strItemName = strTemp;
	GetDlgItem(IDC_EDIT2_DIAGNOSE_ITEM_POINT_BINDING)->GetWindowTextW(strTemp);
	m_pItem->strPointName = strTemp;
	m_pItem->color = m_nColor;
	m_pItem->vecItemInLine.clear();
	const int nCount = m_list_subitem.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		_tagDiagnoseItemInLine item;
		item.nID = _wtoi(m_list_subitem.GetItemText(i,0));
		item.strPointName = m_list_subitem.GetItemText(i,1);
		item.strDescription = m_list_subitem.GetItemText(i,2);
		m_pItem->vecItemInLine.push_back(item);
	}
	CDialog::OnOK();
}


HBRUSH CDiagnoseItemPropertySettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_DIAGNOSE_ITEM_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_nColor);
		if (hBrush)
		{
			return hBrush;
		}
	}
	return hbr;
}


BOOL CDiagnoseItemPropertySettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT_DIAGNOSE_ITEM_NAME)->SetWindowTextW(m_pItem->strItemName);
	GetDlgItem(IDC_EDIT2_DIAGNOSE_ITEM_POINT_BINDING)->SetWindowTextW(m_pItem->strPointName);
	Init();
	for (size_t i=0;i<m_pItem->vecItemInLine.size();++i)
	{
		CString strTemp;
		strTemp.Format(L"%d",m_pItem->vecItemInLine[i].nID);
		m_list_subitem.InsertItem(i,strTemp);
		m_list_subitem.SetItemText(i,1,m_pItem->vecItemInLine[i].strPointName);
		m_list_subitem.SetItemText(i,2,m_pItem->vecItemInLine[i].strDescription);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDiagnoseItemPropertySettingDlg::Init()
{
	m_list_subitem.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_list_subitem.InsertColumn( 0, L"图片ID", LVCFMT_LEFT,50);
	m_list_subitem.InsertColumn( 1, L"点名", LVCFMT_LEFT,100);
	m_list_subitem.InsertColumn( 2, L"描述", LVCFMT_LEFT,150);
}


void CDiagnoseItemPropertySettingDlg::OnBnClickedButtonDiagnoseSubitemAdd()
{
	const int nCount = m_list_subitem.GetItemCount();
	m_list_subitem.InsertItem(nCount,L"-1");
	_tagDiagnoseItemInLine item;
	item.nID = -1;
	m_pItem->vecItemInLine.push_back(item);
}


void CDiagnoseItemPropertySettingDlg::OnBnClickedButtonDiagnoseSubitemDelete()
{
	if (AfxMessageBox(L"确认要删除吗?",MB_OKCANCEL) == IDOK)
	{
		vector<int> vec;
		POSITION pos = m_list_subitem.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_list_subitem.GetNextSelectedItem(pos);
			CString strItemName = m_list_subitem.GetItemText(nItem,1);
			vector<_tagDiagnoseItemInLine>::iterator iter = m_pItem->vecItemInLine.begin();
			while (iter != m_pItem->vecItemInLine.end())
			{
				if ((*iter).strPointName == strItemName)
				{
					iter = m_pItem->vecItemInLine.erase(iter);
					break;
				}
				else
				{
					++iter;
				}
			}
			vec.push_back(nItem);
		}
		for (int i=vec.size()-1;i>=0;--i)
		{
			m_list_subitem.DeleteItem(vec[i]);
		}
	}
}


void CDiagnoseItemPropertySettingDlg::OnNMDblclkListDiagnoseSubitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	const int nItem = pNMItemActivate->iItem;
	if(nItem != -1)
	{
		CSubItemPropertySettingDlg dlg;
		dlg.m_strPicID = m_list_subitem.GetItemText(nItem,0);
		dlg.m_strPointName = m_list_subitem.GetItemText(nItem,1);
		dlg.m_strDescription = m_list_subitem.GetItemText(nItem,2);
		if (dlg.DoModal() == IDOK)
		{
			m_list_subitem.SetItemText(nItem,0,dlg.m_strPicID);
			m_list_subitem.SetItemText(nItem,1,dlg.m_strPointName);
			m_list_subitem.SetItemText(nItem,2,dlg.m_strDescription);
		}
	}
	*pResult = 0;
}
