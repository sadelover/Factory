// SelectPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectPointDlg.h"
#include "afxdialogex.h"
#include "PageDlg.h"

// CSelectPointDlg dialog

IMPLEMENT_DYNAMIC(CSelectPointDlg, CDialog)

CSelectPointDlg::CSelectPointDlg(CPageDlg* pGatewayWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CSelectPointDlg::IDD, pParent)
	, m_pGatewayDlg(pGatewayWnd)
	, m_nSelectIndex(-1)
	, m_strPointID(_T(""))
{

}

CSelectPointDlg::~CSelectPointDlg()
{
}

void CSelectPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_TREE, m_pPointTree);
	DDX_Text(pDX, IDC_EDIT_POINTID, m_strPointID);
}


BEGIN_MESSAGE_MAP(CSelectPointDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectPointDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSelectPointDlg::OnBnClickedButtonSearch)

END_MESSAGE_MAP()

BOOL CSelectPointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitTreeView();
	return TRUE;
}

void CSelectPointDlg::InitTreeView()
{
	m_pPointTree.DeleteAllItems();

	UINT nMask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;

	m_root=m_pPointTree.InsertItem(nMask,_T("点位总览"),0,0,0,0,-1,TVI_ROOT,NULL);
	AddPointIDtoTree(m_root,0);

	m_pPointTree.Expand(m_root,TVE_EXPAND);
}

void CSelectPointDlg::AddPointIDtoTree( HTREEITEM m_node, int pid )
{
	UINT nMask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;

	vector<DataPointEntry>  & m_pointlist = m_pGatewayDlg->m_vecServerPointList;
	const size_t size = m_pointlist.size();
	for(size_t i=0;i<size;++i)
	{
		wstring wstrName = L"[" + m_pointlist[i].GetShortName() + L"]   " + m_pointlist[i].GetDescription();
		CString strRemarkName = wstrName.c_str();
		int nIndex = m_pointlist[i].GetPointIndex();
		bool bInSelection = false;

		HTREEITEM m_select=m_pPointTree.InsertItem(nMask,strRemarkName,3,3,0,0,nIndex,m_node,NULL);
	}
	m_pPointTree.SelectItem(m_node);
}

// CSelectPointDlg message handlers


void CSelectPointDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_nSelectIndex = m_pPointTree.GetItemData(m_pPointTree.GetSelectedItem());
	if(m_nSelectIndex<0)
	{
		MessageBox(_T("请选择相应子节点！"));
		return;
	}
	CDialog::OnOK();
}


bool CSelectPointDlg::FindString(const wstring& strSrc0, const wstring& strToFind0)
{
	bool ifFind = false;
	CString strSrc1 = strSrc0.c_str();
	strSrc1.MakeLower();
	CString strToFind1 = strToFind0.c_str();
	strToFind1.MakeLower();

	int findPos = (int)strSrc1.Find(strToFind1);
	if(findPos >=0 && findPos < (int)strSrc1.GetLength() )
		ifFind = true;

	return ifFind;
}


void CSelectPointDlg::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	HTREEITEM itemRoot = m_pPointTree.GetRootItem();
	HTREEITEM itemChild = m_pPointTree.GetChildItem(itemRoot);
	while(itemChild)
	{
		CString strName = m_pPointTree.GetItemText(itemChild);
		strName.MakeLower();

		m_strPointID.MakeLower();
		if(strName.Find(m_strPointID)>=0)
		{
			m_pPointTree.SelectItem(itemChild);
			m_pPointTree.EnsureVisible(itemChild);
			return;
		}

		itemChild = m_pPointTree.GetNextSiblingItem(itemChild);
	}
}

