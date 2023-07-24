// GridCellRich.h: interface for the CGridCellRich class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELLRICH_H__51FDCB6F_A7D4_4213_BF6A_3987CB29214A__INCLUDED_)
#define AFX_GRIDCELLRICH_H__51FDCB6F_A7D4_4213_BF6A_3987CB29214A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGridCtrl;
#include "GridCell.h"

class CGridCellRich : public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellRich)
public:
	CGridCellRich();
	virtual ~CGridCellRich();
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd =TRUE);
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	void SetBold ();
	void SetItalic ();
	void SetUnderline ();
protected:
	CRichEditCtrl* m_pRichEditTemp;
};

#endif // !defined(AFX_GRIDCELLRICH_H__51FDCB6F_A7D4_4213_BF6A_3987CB29214A__INCLUDED_)
