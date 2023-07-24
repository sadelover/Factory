// EqmRectangleConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EqmRectangleConfigDlg.h"
#include "afxdialogex.h"
#include "CanvasDlg.h"

// CEqmRectangleConfigDlg 对话框

IMPLEMENT_DYNAMIC(CEqmRectangleConfigDlg, CDialogEx)

CEqmRectangleConfigDlg::CEqmRectangleConfigDlg(CEqmDrawRectangle* pDrawRectangle, CWnd* pParent /*=NULL*/)
	: CDialogEx(CEqmRectangleConfigDlg::IDD, pParent)
	, m_childDlg_bw((CCanvasDlg*)pParent,  pDrawRectangle)
	, m_childDlg_rect(pDrawRectangle)
	, m_nLastSelIndex(0)
{
	if(NULL == pDrawRectangle)
		return;
	m_nLastSelIndex = pDrawRectangle->GetRectangleStyle();
}

CEqmRectangleConfigDlg::~CEqmRectangleConfigDlg()
{
}

void CEqmRectangleConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHILD_DLG, m_combo_childDlg);
}


BEGIN_MESSAGE_MAP(CEqmRectangleConfigDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CHILD_DLG, &CEqmRectangleConfigDlg::OnCbnSelchangeComboChildDlg)
END_MESSAGE_MAP()


// CEqmRectangleConfigDlg 消息处理程序


BOOL CEqmRectangleConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect;
	GetDlgItem(IDC_STATIC_LAYOUT)->GetWindowRect(rect);
	ScreenToClient(&rect);

	m_childDlg_bw.Create(IDD_DIALOG_RECTANGLE_CONFIG_BLUE_WHITE, this);
	m_childDlg_bw.MoveWindow(rect);
	m_childDlg_bw.ShowWindow(SW_HIDE);

	m_childDlg_rect.Create(IDD_DIALOG_RECTANGLE_CONFIG_RECT, this);
	m_childDlg_rect.MoveWindow(rect);
	m_childDlg_rect.ShowWindow(SW_HIDE);

	m_combo_childDlg.InsertString(0, L"矩形");
	m_combo_childDlg.SetItemDataPtr(0, (void*)(&m_childDlg_rect));
	m_combo_childDlg.InsertString(1, L"蓝白文本框");
	m_combo_childDlg.SetItemDataPtr(1, (void*)(&m_childDlg_bw));

	m_combo_childDlg.SetCurSel(m_nLastSelIndex);
	((CWnd*)m_combo_childDlg.GetItemDataPtr(m_nLastSelIndex))->ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEqmRectangleConfigDlg::OnCbnSelchangeComboChildDlg()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelIndex = m_combo_childDlg.GetCurSel();
	if(m_nLastSelIndex == nSelIndex)
		return;
	m_nLastSelIndex = nSelIndex;
	int nCount = m_combo_childDlg.GetCount();
	for(int i=0;i<nCount;i++)
	{
		CWnd* pItemDlg = (CWnd*)m_combo_childDlg.GetItemDataPtr(i);
		if(NULL == pItemDlg)
			continue;
		pItemDlg->ShowWindow(SW_HIDE);
	}

	CWnd* pChildDlg = (CWnd*)m_combo_childDlg.GetItemDataPtr(nSelIndex);
	pChildDlg->ShowWindow(SW_SHOW);
}
