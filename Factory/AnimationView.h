#pragma once
#include "MetaFileView.h"

#define REDRAW_ANIMATION_TIMER_ID 0x101
#define REDRAW_ANIMATION_TIMER_INTERVAL 1000
#define CALC_INDEX_TIMER_ID 0x102
#define CALC_INDEX_INTERVAL 50
// CAnimationView 视图

struct AnimationView
{
	AnimationView()
	{
		bIsShowRectangle = false;
		nIndex = 0;
	};
	CRect rectPicture;
	CRect rectRectangle;
	vector<Image*> vec_pBitmap;
	bool bIsShowRectangle;
	CString strName;
	int nIndex;
	int nID;
};

class CProjectSqlite;

class CAnimationView : public CView
{
	DECLARE_DYNCREATE(CAnimationView)

public:
	CAnimationView();           // 动态创建所使用的受保护的构造函数
	virtual ~CAnimationView();
	BOOL ShowWindow(int nCmdShow);
	void SetProjectSqlPointer(CProjectSqlite* pProjectSqlite);
	void Insert(AnimationView viewInfo);
	void DeleteOne(const int nID);
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void SetParent(CWnd* pParent);
	vector<AnimationView> m_vecAnimationView;
	int m_nSelectIndex;
	int m_nCurPage;
private:
	CProjectSqlite* m_pProjectSqlite;
	CWnd* m_pParent;
	int	m_nShowMod;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


