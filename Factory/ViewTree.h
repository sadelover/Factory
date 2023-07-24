
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
	UINT		m_TimerTicks;      //��������Ķ�ʱ����������ʱ��
	//UINT		m_nScrollTimerID;  //��������Ķ�ʱ��
	CPoint		m_HoverPoint;      //���λ��
	UINT		m_nHoverTimerID;   //������ж�ʱ��
	DWORD		m_dwDragStart;     //������������һ�̵�ʱ��
	bool		m_bDragging;       //��ʶ�Ƿ������϶�������
	CImageList*	m_pDragImage;      //�϶�ʱ��ʾ��ͼ���б�
	HTREEITEM	m_hItemDragS;      //���϶��ı�ǩ
	HTREEITEM	m_hItemDragD;      //�����϶��ı�ǩ


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
