// EditListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "EditListCtrl.h"

#include <cassert>

// CEditListCtrl

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)
CEditListCtrl::CEditListCtrl()
{
}

CEditListCtrl::~CEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_WM_NCLBUTTONDBLCLK()
	ON_MESSAGE(WM_USER_EDIT_END,OnEditEnd)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CEditListCtrl::ShowEdit(bool bShow, int m_nItem, int m_nSubItem, CRect rcCtrl)
{
	if (m_Edit.m_hWnd == NULL)
	{
		m_Edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,IDC_EDIT);
		m_Edit.ShowWindow(SW_HIDE);

		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		m_Edit.SetFont(&tpFont);
		tpFont.DeleteObject();
	}
	if (bShow == TRUE)
	{
		CString strItem = CListCtrl::GetItemText(m_nItem,m_nSubItem);
		m_Edit.MoveWindow(&rcCtrl);
		m_Edit.ShowWindow(SW_SHOW);
		m_Edit.SetWindowText(strItem);
		m_Edit.SetFocus();
		m_Edit.SetSel(-1); 
	}
	else
		m_Edit.ShowWindow(SW_HIDE);
}

// 编辑框失去焦点时促发事件
//
LRESULT CEditListCtrl::OnEditEnd(WPARAM wParam,LPARAM lParam)
{
	CString strText;
	m_Edit.GetWindowText(strText);
	CListCtrl::SetItemText(m_nItem,m_nSubItem,strText);
	m_Edit.ShowWindow(SW_HIDE);

	return 0;
}

void CEditListCtrl::SetViewType( const _tagViewType type )
{
	m_eViewType = type;
}

void CEditListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcCtrl;
	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	m_nItem = CListCtrl::SubItemHitTest(&lvhti);
	if (m_nItem == -1) return;
	m_nSubItem = lvhti.iSubItem;
	CListCtrl::GetSubItemRect(m_nItem,m_nSubItem,LVIR_LABEL,rcCtrl);
	//严观微，暂时不用显示编辑控件
	//ShowEdit(TRUE,m_nItem,m_nSubItem,rcCtrl);
	CListCtrl::OnLButtonDblClk(nFlags, point);
}


