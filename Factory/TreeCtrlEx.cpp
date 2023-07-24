// TreeCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "TreeCtrlEx.h"
//#include "MainFrm.h"


#define   DRAG_DELAY   60


// CTreeCtrlEx

IMPLEMENT_DYNAMIC(CTreeCtrlEx, CTreeCtrl)

CTreeCtrlEx::CTreeCtrlEx()
	: m_TimerTicks(0)
	//, m_nScrollTimerID(0)
	, m_HoverPoint(0, 0)
	, m_nHoverTimerID(0)
	, m_dwDragStart(0)
	, m_bDragging(false)
	, m_pDragImage(NULL)
	, m_hItemDragS(NULL)
	, m_hItemDragD(NULL)
{
}

CTreeCtrlEx::~CTreeCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CTreeCtrlEx::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CTreeCtrlEx message handlers




void CTreeCtrlEx::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	//如果是无意拖动，则放弃操作
	if ((GetTickCount() - m_dwDragStart) < DRAG_DELAY)
	{
		return;
	}

	m_hItemDragS = pNMTreeView->itemNew.hItem;
	m_hItemDragD = NULL;
	

	//得到用于拖动时显示的图象列表
	m_pDragImage = CreateDragImage(m_hItemDragS);
	if (NULL == m_pDragImage)
	{
		return;
	}

	m_bDragging = true;
	m_pDragImage->BeginDrag(0, CPoint(8,8));
	CPoint  pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pDragImage->DragEnter(this, pt);  //"this"将拖动操作限制在该窗口
	SetCapture();

	//m_nScrollTimerID = SetTimer(2, 40, NULL);
}


void CTreeCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM  hItem	= NULL;
	UINT       flags	= 0;

	//检测鼠标敏感定时器是否存在,如果存在则删除,删除后再定时
	if (m_nHoverTimerID != 0)
	{
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;
	}
	m_nHoverTimerID = SetTimer(1, 800, NULL);  //定时为 0.8 秒则自动展开
	m_HoverPoint = point;

	if (m_bDragging)
	{
		CPoint  pt = point;
		CImageList::DragMove(pt);

		//鼠标经过时高亮显示
		CImageList::DragShowNolock(false);  //避免鼠标经过时留下难看的痕迹
		if ((hItem = HitTest(point,&flags)) != NULL)
		{
			SelectDropTarget(hItem);
			m_hItemDragD = hItem;
		}
		CImageList::DragShowNolock(true);

		//当条目被拖曳到左边缘时，将条目放在根下
		CRect  rect;
		GetClientRect(&rect);
		if (point.x < rect.left + 20)
		{
			m_hItemDragD = NULL;
		}
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}


void CTreeCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		m_bDragging = false;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImage;

		SelectDropTarget(NULL);

		if ((NULL == m_hItemDragS) || (NULL == m_hItemDragD))
		{
			return;
		}
		if (m_hItemDragS == m_hItemDragD)
		{
			//KillTimer(m_nScrollTimerID);
			return;
		}

		const HTREEITEM hDstParent	= GetParentItem(m_hItemDragD);
		const HTREEITEM hSrcParent	= GetParentItem(m_hItemDragS);
		HTREEITEM	hNew			= NULL;
		DWORD_PTR	dwDstItemVal	= GetItemData(m_hItemDragD);
		DWORD_PTR	dwSrcItemVal	= GetItemData(m_hItemDragS);

		if (dwDstItemVal >= 0 && dwSrcItemVal >= 0)
		{	// 判断是在页面根节点下
			if (NULL != GetParentItem(hSrcParent))
			{	// 判断是移动页节点
				if (hDstParent == hSrcParent)
				{	// 同一层次节点间拖动，调整被拖动节点的位置
					hNew = CopyItem(m_hItemDragS, hDstParent, m_hItemDragD);
				}
				else if (m_hItemDragD == hSrcParent)
				{	// 将节点拖向其父节点，将其排在该组第1个
					hNew = CopyItem(m_hItemDragS, m_hItemDragD, TVI_FIRST);
				}
				else if ((GetParentItem(hDstParent) == GetParentItem(hSrcParent)) && (hDstParent != hSrcParent))
				{	// 拖到其它的组节点下的页面上
					hNew = CopyItem(m_hItemDragS, hDstParent, m_hItemDragD);
				}
				else if ((hDstParent == GetParentItem(hSrcParent)) && (m_hItemDragD != hSrcParent))
				{	// 拖到其它组节点上，将其排在该组第1个
					hNew = CopyItem(m_hItemDragS, m_hItemDragD, TVI_FIRST);
				}
				else
				{	// 其它情况
					return;
				}
			}
			else if (NULL == GetParentItem(hSrcParent))
			{	// 判断是移动组节点
				if (hDstParent == hSrcParent)
				{	// 同一层次组间拖动，调整被拖动节点的位置
					hNew = CopyBranch(m_hItemDragS, hDstParent, m_hItemDragD);
				}
				else if (m_hItemDragD == hSrcParent)
				{	// 组移动到页面根节点，将其排在第1组
					hNew = CopyBranch(m_hItemDragS, m_hItemDragD, TVI_FIRST);
				}
				else
				{
					return;
				}
				//SetItemState(hNew, TVIS_EXPANDED, TVIS_EXPANDED);
				Expand(hNew, TVE_EXPAND);
			}

			DeleteItem(m_hItemDragS);
			//SelectItem(hNew);
			//KillTimer(m_nScrollTimerID);
		}
	}

	CTreeCtrl::OnLButtonUp(nFlags, point);
}


void CTreeCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_dwDragStart = GetTickCount();

	CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CTreeCtrlEx::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//鼠标敏感节点
	if (nIDEvent == m_nHoverTimerID)
	{
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;
		HTREEITEM  trItem = 0;
		UINT  uFlag = 0;
		trItem = HitTest(m_HoverPoint, &uFlag);
		if(trItem && m_bDragging)
		{
			SelectItem(trItem);
			Expand(trItem, TVE_EXPAND);
		}
	}
	//处理拖曳过程中的滚动问题
	//if (nIDEvent == m_nScrollTimerID)
	//{
		//m_TimerTicks++;
		//CPoint  pt;
		//GetCursorPos(&pt);
		//CRect  rect;
		//GetClientRect(&rect);
		//ClientToScreen(&rect);

		//HTREEITEM  hItem = GetFirstVisibleItem();

		//if (pt.y < rect.top + 10)
		//{
		//	//向上滚动
		//	int  slowscroll = 6 - (rect.top + 10 - pt.y )/20;
		//	if (0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)))
		//	{
		//		CImageList::DragShowNolock(false);
		//		SendMessage(WM_VSCROLL, SB_LINEUP);
		//		SelectDropTarget(hItem);
		//		m_hItemDragD = hItem;
		//		CImageList::DragShowNolock(true);
		//	}
		//}
		//else if (pt.y > rect.bottom - 10)
		//{
		//	//向下滚动
		//	int  slowscroll = 6 - (pt.y - rect.bottom + 10) / 20;
		//	if (0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)))
		//	{
		//		CImageList::DragShowNolock(false);
		//		SendMessage(WM_VSCROLL, SB_LINEDOWN);
		//		int  nCount = GetVisibleCount();
		//		for (int i=0; i<nCount-1; i++)
		//		{
		//			hItem = GetNextVisibleItem(hItem);
		//		}
		//		if (hItem != NULL)
		//		{
		//			SelectDropTarget(hItem);
		//		}
		//		m_hItemDragD = hItem;
		//		CImageList::DragShowNolock(true);
		//	}
		//}
	//}
	else
	{
		CTreeCtrl::OnTimer(nIDEvent);
	}
}

HTREEITEM CTreeCtrlEx::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT	tvstruct;
	HTREEITEM		hNewItem	= NULL;
	CString			sText;

	//得到源条目的信息
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem(&tvstruct.item);
	sText = GetItemText(hItem);
	tvstruct.item.cchTextMax	= sText.GetLength();
	tvstruct.item.pszText		= sText.LockBuffer();

	//将条目插入到合适的位置
	tvstruct.hParent		= htiNewParent;
	tvstruct.hInsertAfter	= htiAfter;
	tvstruct.item.mask		= TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;

	hNewItem = InsertItem(&tvstruct);
	sText.ReleaseBuffer();

	//限制拷贝条目数据和条目状态
	SetItemData(hNewItem, GetItemData(hItem));
	SetItemState(hNewItem, GetItemState(hItem, TVIS_STATEIMAGEMASK), TVIS_STATEIMAGEMASK);

	return hNewItem;
}

HTREEITEM CTreeCtrlEx::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	HTREEITEM	hChild		= NULL;
	HTREEITEM	hNewItem	= CopyItem(htiBranch, htiNewParent, htiAfter);

	hChild = GetChildItem(htiBranch);
	while (hChild != NULL)
	{
		CopyBranch(hChild, hNewItem, hNewItem);
		hChild = GetNextSiblingItem(hChild);
	}

	return hNewItem;
}
