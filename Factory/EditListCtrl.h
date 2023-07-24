#pragma once

#include "ListCtrlEdit.h"

// CEditListCtrl

enum _tagViewType
{
	eLogView = 0,
	ePointView,
	eAlarmView,
	eDeviceView,
};

class CEditListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEditListCtrl)

public:
	CEditListCtrl();
	virtual ~CEditListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnEditEnd(WPARAM wParam,LPARAM lParam = FALSE);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void SetViewType(const _tagViewType type);
public:
	void ShowEdit(bool bShow, int nItem, int nSubItem, CRect rcCtrl);
	CListCtrlEdit m_Edit;
	int m_nItem;
	int m_nSubItem;
private:
	_tagViewType m_eViewType;
};


