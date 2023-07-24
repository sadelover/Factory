#pragma once

class CTreeManageDlg;
class CPageDlg;

class CBEOPDesignerView : public CView
{
protected: // create from serialization only
	CBEOPDesignerView();
	DECLARE_DYNCREATE(CBEOPDesignerView)
// Attributes
public:
	//CBEOPDesignerDoc* GetDocument() const;
// Operations
public:
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// Implementation
public:     
	virtual ~CBEOPDesignerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	CTreeManageDlg*		m_pTreeDlg;
	CPageDlg*			m_PageDlg;
	HBRUSH				m_hBrush;
	int					m_nPageCount;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnZoomout();
	afx_msg void OnZoomin();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateZoomout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZoomin(CCmdUI *pCmdUI);
	virtual void OnInitialUpdate();
};
