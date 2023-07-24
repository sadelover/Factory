
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree window

class CViewTree : public CTreeCtrl
{
// Construction
public:
	CViewTree();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CViewTree();

private:
	HTREEITEM	CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	HTREEITEM	CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter);

protected:
	UINT		m_TimerTicks;      //处理滚动的定时器所经过的时间
	//UINT		m_nScrollTimerID;  //处理滚动的定时器
	CPoint		m_HoverPoint;      //鼠标位置
	UINT		m_nHoverTimerID;   //鼠标敏感定时器
	DWORD		m_dwDragStart;     //按下鼠标左键那一刻的时间
	bool		m_bDragging;       //标识是否正在拖动过程中
	CImageList*	m_pDragImage;      //拖动时显示的图象列表
	HTREEITEM	m_hItemDragS;      //被拖动的标签
	HTREEITEM	m_hItemDragD;      //接受拖动的标签


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
