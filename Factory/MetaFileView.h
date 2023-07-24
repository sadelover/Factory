#pragma once
#include <Gdiplus.h>

#include "ImagePreviewDlg.h"

// CMetaFileView 视图
#define SCROLL_STEP 20
#define PICTURE_SPACING 3
#define ROW_PICTURE_NUM 15
#define COLUMN_PICTURE_NUM 7
#define PICTURE_VIEW_WIDTH 64
#define PICTURE_VIEW_HEIGHT 64
#define RECTANGLE_TO_PICTURE 1

class CAnimationPropertyDlg;

enum E_SubType
{
	E_TYPE_ALL = 0,
	E_TYPE_CH,
	E_TYPE_PUMP,
	E_TYPE_TOWER,
	E_TYPE_AHU,
	E_TYPE_VALUE,
	E_TYPE_PIPE,
	E_TYPE_BUTTON,
	E_TYPE_CHART,
	E_TYPE_LAYOUT,
	E_TYPE_ICON,
	E_TYPE_OTHET,
};

struct PictureView 
{
	PictureView()
	{
		pBitmap = NULL;
		bIsShowRectangle = false;
		nID = -1;
	};
	CRect rectPicture;
	CRect rectRectangle;
	Image* pBitmap;
	bool bIsShowRectangle;
	CString strName;
	int		nID;
	wstring etype_equip;
};

class CProjectSqlite;

class CMetaFileView : public CView
{
	DECLARE_DYNCREATE(CMetaFileView)

public:
	CMetaFileView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMetaFileView();
	void SetProjectSqlPointer(CProjectSqlite* pProjectSqlite);
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
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	void SetParent(CWnd* pParent);
	void UpdatePictureViewByType(E_SubType eSubType);
	void UpdatePictureViewByName(CString strPicName);
	void SetCurrentPicID(const int nID);
	void ClearSelectPicByPageID(const int nPageID);
	void		HidePreviewDlg();
	CProjectSqlite* m_pProjectSqlite;
	CAnimationPropertyDlg* m_pAnimationPropertyDlg;
	vector<PictureView> m_vecPictureView;
	vector<PictureView> m_vecPictureViewByType;
	int m_nCurPage;
	CWnd* m_pParent;
	int m_nSelectIndex;
	CImagePreviewDlg*	m_pImagePreviewDlg;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void	DeleteAndInsert(const int nID, PictureView item);
};


