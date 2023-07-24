// AnimationView.cpp : 实现文件
//

#include "stdafx.h"
#include "AnimationView.h"
#include <cassert>
#include "../ComponentDraw/ProjectSqlite.h"
#include "MetaFileSelectDlg.h"

// CAnimationView

IMPLEMENT_DYNCREATE(CAnimationView, CView)

CAnimationView::CAnimationView()
{
	m_pProjectSqlite = NULL;
	m_pParent = NULL;
	m_nSelectIndex = -1;
	m_nCurPage = 0;
	m_nShowMod = 0;
}

CAnimationView::~CAnimationView()
{
	::KillTimer(m_hWnd,REDRAW_ANIMATION_TIMER_ID);
	KillTimer(CALC_INDEX_TIMER_ID);
}


BEGIN_MESSAGE_MAP(CAnimationView, CView)
	ON_WM_TIMER()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CAnimationView 绘图

void CAnimationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMetaFileSelectDlg* pMetaDlg = dynamic_cast<CMetaFileSelectDlg*>(m_pParent);
	ASSERT(pMetaDlg != NULL);
	if (NULL == pMetaDlg)
	{
		return;
	}

	std::vector<AnimationInfo>::const_iterator	iterAni;
	std::vector<PictureInfo>::const_iterator	iterPic;
	m_nShowMod = pMetaDlg->GetShowMode();
	if (0 == m_nShowMod)
	{
		for (iterAni=m_pProjectSqlite->m_vAnimationListImage.begin(); iterAni!=m_pProjectSqlite->m_vAnimationListImage.end(); ++iterAni)
		{
			AnimationView aniView;
			aniView.nID = iterAni->ID;
			aniView.strName = iterAni->szName.c_str();
			for (iterPic=iterAni->vecPicture.begin(); iterPic!=iterAni->vecPicture.end(); ++iterPic)
			{
				aniView.vec_pBitmap.push_back(iterPic->pBitmap);
			}

			m_vecAnimationView.push_back(aniView);
		}
	}
	else if (1 == m_nShowMod)
	{
		for (iterAni=m_pProjectSqlite->m_vAnimationList.begin(); iterAni!=m_pProjectSqlite->m_vAnimationList.end(); ++iterAni)
		{
			AnimationView aniView;
			aniView.nID = iterAni->ID;
			aniView.strName = iterAni->szName.c_str();
			for (iterPic=iterAni->vecPicture.begin(); iterPic!=iterAni->vecPicture.end(); ++iterPic)
			{
				aniView.vec_pBitmap.push_back(iterPic->pBitmap);
			}

			m_vecAnimationView.push_back(aniView);
		}
	}

	SetTimer(CALC_INDEX_TIMER_ID, CALC_INDEX_INTERVAL, NULL);
}

void CAnimationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码

	assert(pDC);
	assert(pDC->m_hDC);

	RECT rc;
	GetClientRect(&rc);
	Bitmap bmp(int(rc.right),int(rc.bottom));
	Graphics bmpGraphics(&bmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeInvalid);
	{
		for (int i=1;i<=COLUMN_PICTURE_NUM;++i)
		{
			for (int j=0;j<ROW_PICTURE_NUM;++j)
			{
				size_t size = 0;
				if (0 == m_nShowMod)
				{
					size = m_pProjectSqlite->m_vAnimationListImage.size();
				} 
				else if (1 == m_nShowMod)
				{
					size = m_pProjectSqlite->m_vAnimationList.size();
				}

				const int nIndex = j+(i-1)*ROW_PICTURE_NUM + COLUMN_PICTURE_NUM*ROW_PICTURE_NUM*m_nCurPage;
				const int nMod = nIndex%ROW_PICTURE_NUM;

				if (nIndex < (int)size)
				{
					m_vecAnimationView[nIndex].rectPicture.left = PICTURE_SPACING*(nMod+1)+PICTURE_VIEW_WIDTH*nMod;
					m_vecAnimationView[nIndex].rectPicture.top = PICTURE_SPACING*i+PICTURE_VIEW_HEIGHT*(i-1);
					m_vecAnimationView[nIndex].rectPicture.right = m_vecAnimationView[j+(i-1)*ROW_PICTURE_NUM].rectPicture.left+PICTURE_VIEW_WIDTH;
					m_vecAnimationView[nIndex].rectPicture.bottom = m_vecAnimationView[j+(i-1)*ROW_PICTURE_NUM].rectPicture.top+PICTURE_VIEW_HEIGHT;

					m_vecAnimationView[nIndex].rectRectangle.left = m_vecAnimationView[nIndex].rectPicture.left-RECTANGLE_TO_PICTURE;
					m_vecAnimationView[nIndex].rectRectangle.top = m_vecAnimationView[nIndex].rectPicture.top-RECTANGLE_TO_PICTURE;
					m_vecAnimationView[nIndex].rectRectangle.right = m_vecAnimationView[nIndex].rectPicture.right+RECTANGLE_TO_PICTURE;
					m_vecAnimationView[nIndex].rectRectangle.bottom = m_vecAnimationView[nIndex].rectPicture.bottom+RECTANGLE_TO_PICTURE;
					if(!m_vecAnimationView[nIndex].vec_pBitmap.empty())
					{
						bmpGraphics.DrawImage(m_vecAnimationView[nIndex].vec_pBitmap[m_vecAnimationView[nIndex].nIndex] ,m_vecAnimationView[nIndex].rectPicture.left,m_vecAnimationView[nIndex].rectPicture.top,PICTURE_VIEW_WIDTH,PICTURE_VIEW_HEIGHT);
					}

					if (m_vecAnimationView[nIndex].bIsShowRectangle)
					{
						Gdiplus::Pen pen((ARGB)Color::Red,1.0);
						bmpGraphics.DrawRectangle(&pen,m_vecAnimationView[nIndex].rectRectangle.left,m_vecAnimationView[nIndex].rectRectangle.top,m_vecAnimationView[nIndex].rectRectangle.Width(),m_vecAnimationView[nIndex].rectRectangle.Height());
						CString strShow(L"动画ID：");
						CString strID;
						strID.Format(L"%d",m_vecAnimationView[nIndex].nID);
						strShow += strID;
						CMetaFileSelectDlg* pWnd = dynamic_cast<CMetaFileSelectDlg*>(m_pParent);
						assert(pWnd);
						pWnd->GetDlgItem(IDC_PICTURE_NAME)->SetWindowText(strShow);
					}
					else
					{
						Gdiplus::Pen pen((ARGB)Color::White,1.0);
						bmpGraphics.DrawRectangle(&pen,m_vecAnimationView[nIndex].rectRectangle.left,m_vecAnimationView[nIndex].rectRectangle.top,m_vecAnimationView[nIndex].rectRectangle.Width(),m_vecAnimationView[nIndex].rectRectangle.Height());
					}
				}
			}
		}

	}
	Graphics graphics(pDC->m_hDC);
	CachedBitmap cachedBmp(&bmp,&graphics);
	graphics.DrawCachedBitmap(&cachedBmp,0,0);
	graphics.ReleaseHDC(pDC->m_hDC);
}


// CAnimationView 诊断

#ifdef _DEBUG
void CAnimationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif
#endif //_DEBUG


// CAnimationView 消息处理程序

BOOL CAnimationView::ShowWindow( int nCmdShow )
{
	switch (nCmdShow)
	{
	case SW_HIDE:
		{
			::KillTimer(m_hWnd,REDRAW_ANIMATION_TIMER_ID);
		}
		break;
	case SW_SHOW:
	case SW_SHOWNA:
	case SW_SHOWNORMAL:
		{
			::SetTimer(m_hWnd,REDRAW_ANIMATION_TIMER_ID,REDRAW_ANIMATION_TIMER_INTERVAL,0);
		}
		break;
	default:
		break;
	}
	return CView::ShowWindow(nCmdShow);
}

void CAnimationView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == REDRAW_ANIMATION_TIMER_ID)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}
	else if (nIDEvent == CALC_INDEX_TIMER_ID)
	{
		const size_t size = m_vecAnimationView.size();
		for (size_t i=0;i<size;++i)
		{
			const size_t size_ani = m_vecAnimationView[i].vec_pBitmap.size();
			if ((int)size_ani-1 > m_vecAnimationView[i].nIndex)
			{
				m_vecAnimationView[i].nIndex++;
			}
			else
			{
				m_vecAnimationView[i].nIndex = 0;
			}
		}
	}
	CView::OnTimer(nIDEvent);
}


int CAnimationView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


LRESULT CAnimationView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CView::WindowProc(message, wParam, lParam);
}

void CAnimationView::SetParent( CWnd* pParent )
{
	assert(pParent);
	m_pParent = pParent;
}

void CAnimationView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (m_nSelectIndex == -1)
	//{
	//	bool bInPicture = false;
	//	CPoint pt;
	//	pt.x = point.x;
	//	pt.y = point.y;
	//	for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecAnimationView.size());++i)
	//	{
	//		if (PtInRect(m_vecAnimationView[i].rectPicture,pt))
	//		{
	//			m_vecAnimationView[i].bIsShowRectangle = true;
	//			bInPicture = true;
	//		}
	//		else
	//		{
	//			m_vecAnimationView[i].bIsShowRectangle = false;
	//		}
	//	}
	//	if (!bInPicture)
	//	{
	//		CMetaFileSelectDlg* pWnd = dynamic_cast<CMetaFileSelectDlg*>(m_pParent);
	//		assert(pWnd);
	//		pWnd->GetDlgItem(IDC_PICTURE_NAME)->SetWindowText(L"动画ID：");
	//	}
	//}
	//else
	//{
	//	const size_t size = m_vecAnimationView.size();
	//	for (size_t i=0;i<size;++i)
	//	{
	//		if (i != m_nSelectIndex)
	//		{
	//			m_vecAnimationView[i].bIsShowRectangle = false;
	//		}
	//		else
	//		{
	//			m_vecAnimationView[i].bIsShowRectangle = true;
	//		}
	//	}
	//}
	CView::OnMouseMove(nFlags, point);
}

void CAnimationView::SetProjectSqlPointer( CProjectSqlite* pProjectSqlite )
{
	m_pProjectSqlite = pProjectSqlite;
}


void CAnimationView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMetaFileSelectDlg* pWnd = (CMetaFileSelectDlg*)m_pParent;
	CPoint pt;
	pt.x = point.x;
	pt.y = point.y;
	for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecAnimationView.size());++i)
	{
		if (PtInRect(m_vecAnimationView[i].rectPicture,pt))
		{
			pWnd->SetID(m_vecAnimationView[i].nID);
			pWnd->SetPicNum(m_vecAnimationView[i].vec_pBitmap.size());
			pWnd->SetVectorPicNum(false);
			pWnd->EndDialog(1);
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CAnimationView::Insert( AnimationView viewInfo )
{
	m_vecAnimationView.push_back(viewInfo);
}

void CAnimationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMetaFileSelectDlg* pWnd = (CMetaFileSelectDlg*)m_pParent;
	CPoint pt;
	pt.x = point.x;
	pt.y = point.y;

	if (GetKeyState(VK_CONTROL) >= 0)
	{	// control is not clicked down
		for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecAnimationView.size());++i)
		{
			if (PtInRect(m_vecAnimationView[i].rectPicture,pt))
			{
				if (m_nSelectIndex == -1)
				{
					m_nSelectIndex = i;
					{
						const size_t size = m_vecAnimationView.size();
						for (size_t i=0;i<size;++i)
						{
							if (i != m_nSelectIndex)
							{
								m_vecAnimationView[i].bIsShowRectangle = false;
							}
							else
							{
								m_vecAnimationView[i].bIsShowRectangle = true;
							}
						}
					}
				}
				else
				{
					if (m_nSelectIndex == i)
					{
						m_nSelectIndex = -1;
					}
					else
					{
						m_nSelectIndex = i;
						{
							const size_t size = m_vecAnimationView.size();
							for (size_t i=0;i<size;++i)
							{
								if (i != m_nSelectIndex)
								{
									m_vecAnimationView[i].bIsShowRectangle = false;
								}
								else
								{
									m_vecAnimationView[i].bIsShowRectangle = true;
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{	// control is clicked down
		for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecAnimationView.size());++i)
		{
			if (PtInRect(m_vecAnimationView[i].rectPicture, pt))
			{
				if (m_vecAnimationView[i].bIsShowRectangle)
				{
					m_vecAnimationView[i].bIsShowRectangle = false;
				}
				else
				{
					m_vecAnimationView[i].bIsShowRectangle = true;
					m_nSelectIndex = i;
				}
			}
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CAnimationView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMetaFileSelectDlg* pWnd = (CMetaFileSelectDlg*)m_pParent;
	CPoint pt;
	pt.x = point.x;
	pt.y = point.y;
	for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecAnimationView.size());++i)
	{
		if (PtInRect(m_vecAnimationView[i].rectPicture,pt))
		{
			m_nSelectIndex = i;
			CPoint oPoint;
			GetCursorPos(&oPoint);
			CMenu* pSubMenu = pWnd->m_menu.GetSubMenu(0);
			pSubMenu->DeleteMenu(ID_MENU_LIB_EDIT,MF_BYCOMMAND);
			pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, pWnd);
		}
	}
	CView::OnRButtonDown(nFlags, point);
}

void CAnimationView::DeleteOne( const int nID )
{
	vector<AnimationView>::iterator iter = m_vecAnimationView.begin();
	while (iter != m_vecAnimationView.end())
	{
		if ((*iter).nID == nID)
		{
			iter = m_vecAnimationView.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
