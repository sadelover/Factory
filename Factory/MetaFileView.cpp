// MetaFileView.cpp : 实现文件
//

#include "stdafx.h"
#include "MetaFileView.h"
#include <cassert>
#include "../ComponentDraw/ProjectSqlite.h"
#include "MetaFileSelectDlg.h"
#include "AnimationPropertyDlg.h"
#include <algorithm>
// CMetaFileView

const int g_nMaxSize = 300;

IMPLEMENT_DYNCREATE(CMetaFileView, CView)

CMetaFileView::CMetaFileView()
{
	m_pProjectSqlite = NULL;
	m_nCurPage = 0;
	m_pParent = NULL;
	m_pAnimationPropertyDlg = NULL;
	m_nSelectIndex = -1;
	m_pImagePreviewDlg = NULL;
}

CMetaFileView::~CMetaFileView()
{
	
}


BEGIN_MESSAGE_MAP(CMetaFileView, CView)
	ON_WM_TIMER()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMetaFileView 绘图

void CMetaFileView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMetaFileSelectDlg* pMetaDlg = dynamic_cast<CMetaFileSelectDlg*>(m_pParent);
	ASSERT(pMetaDlg != NULL);
	if (NULL == pMetaDlg)
	{
		return;
	}

	std::vector<PictureInfo>::const_iterator	iter;
	if (0 == pMetaDlg->GetShowMode())
	{
		for (iter=m_pProjectSqlite->m_vPictureListImage.begin(); iter!=m_pProjectSqlite->m_vPictureListImage.end(); ++iter)
		{
			PictureView pic;
			pic.pBitmap = iter->pBitmap;
			pic.strName = iter->szName;
			pic.nID = iter->ID;
			pic.etype_equip = iter->etype_equip;
			m_vecPictureView.push_back(pic);
		}
	}
	else if (1 == pMetaDlg->GetShowMode())
	{
		for (iter=m_pProjectSqlite->m_vPictureList.begin(); iter!=m_pProjectSqlite->m_vPictureList.end(); ++iter)
		{
			PictureView pic;
			pic.pBitmap = iter->pBitmap;
			pic.strName = iter->szName;
			pic.nID = iter->ID;
			pic.etype_equip = iter->etype_equip;
			m_vecPictureView.push_back(pic);
		}
	}

	UpdatePictureViewByType(E_TYPE_ALL);
}

void CMetaFileView::OnDraw(CDC* pDC)
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
	const int nOffset = COLUMN_PICTURE_NUM * ROW_PICTURE_NUM * m_nCurPage;
	{
		for (int i=1;i<=COLUMN_PICTURE_NUM;++i)
		{
			for (int j=0;j<ROW_PICTURE_NUM;++j)
			{
				const size_t size = m_vecPictureViewByType.size();
				
				const int nIndex = j+(i-1)*ROW_PICTURE_NUM + COLUMN_PICTURE_NUM*ROW_PICTURE_NUM*m_nCurPage;
				const int nMod = nIndex%ROW_PICTURE_NUM;

				if (nIndex < (int)size && nIndex >= 0)
				{
					m_vecPictureViewByType[nIndex].rectPicture.left = PICTURE_SPACING*(nMod+1)+PICTURE_VIEW_WIDTH*nMod;
					m_vecPictureViewByType[nIndex].rectPicture.top = PICTURE_SPACING*i+PICTURE_VIEW_HEIGHT*(i-1);
					m_vecPictureViewByType[nIndex].rectPicture.right = m_vecPictureViewByType[j+(i-1)*ROW_PICTURE_NUM + nOffset].rectPicture.left+PICTURE_VIEW_WIDTH;
					m_vecPictureViewByType[nIndex].rectPicture.bottom = m_vecPictureViewByType[j+(i-1)*ROW_PICTURE_NUM + nOffset].rectPicture.top+PICTURE_VIEW_HEIGHT;

					m_vecPictureViewByType[nIndex].rectRectangle.left = m_vecPictureViewByType[nIndex].rectPicture.left-RECTANGLE_TO_PICTURE;
					m_vecPictureViewByType[nIndex].rectRectangle.top = m_vecPictureViewByType[nIndex].rectPicture.top-RECTANGLE_TO_PICTURE;
					m_vecPictureViewByType[nIndex].rectRectangle.right = m_vecPictureViewByType[nIndex].rectPicture.right+RECTANGLE_TO_PICTURE;
					m_vecPictureViewByType[nIndex].rectRectangle.bottom = m_vecPictureViewByType[nIndex].rectPicture.bottom+RECTANGLE_TO_PICTURE;

					int nWidth = 0;
					int nHeight = 0;
					Image* pBitmap = m_vecPictureViewByType[nIndex].pBitmap;
					if (NULL == pBitmap)
					{
						continue;
					}
					if(PICTURE_VIEW_WIDTH >= pBitmap->GetWidth())
					{
						nWidth = pBitmap->GetWidth();
					}
					else
					{
						nWidth = PICTURE_VIEW_WIDTH;
					}
					if(PICTURE_VIEW_HEIGHT >= pBitmap->GetHeight())
					{
						nHeight = pBitmap->GetHeight();
					}
					else
					{
						nHeight = PICTURE_VIEW_HEIGHT;
					}
					if (pBitmap != NULL)
					{
						bmpGraphics.DrawImage(pBitmap,m_vecPictureViewByType[nIndex].rectPicture.left,m_vecPictureViewByType[nIndex].rectPicture.top,nWidth,nHeight);
					}
					
					if (m_vecPictureViewByType[nIndex].bIsShowRectangle)
					{
						Gdiplus::Pen pen((ARGB)Color::Red,1.0);
						bmpGraphics.DrawRectangle(&pen,m_vecPictureViewByType[nIndex].rectRectangle.left,m_vecPictureViewByType[nIndex].rectRectangle.top,m_vecPictureViewByType[nIndex].rectRectangle.Width(),m_vecPictureViewByType[nIndex].rectRectangle.Height());
						CString strShow(L"图片ID：");
						CString strID;
						strID.Format(L"%d",m_vecPictureViewByType[nIndex].nID);
						strShow += strID;

						CString strLast;
						strLast.Format(L"%s   图片名称：%s",strShow,m_vecPictureViewByType[nIndex].strName);
						CMetaFileSelectDlg* pWnd = dynamic_cast<CMetaFileSelectDlg*>(m_pParent);
						assert(pWnd);
						pWnd->GetDlgItem(IDC_PICTURE_NAME)->SetWindowText(strLast);
					}
					else
					{
						Gdiplus::Pen pen((ARGB)Color::White,1.0);
						bmpGraphics.DrawRectangle(&pen,m_vecPictureViewByType[nIndex].rectRectangle.left,m_vecPictureViewByType[nIndex].rectRectangle.top,m_vecPictureViewByType[nIndex].rectRectangle.Width(),m_vecPictureViewByType[nIndex].rectRectangle.Height());
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


// CMetaFileView 诊断

#ifdef _DEBUG
void CMetaFileView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMetaFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif
#endif //_DEBUG


// CMetaFileView 消息处理程序


int CMetaFileView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CMetaFileView::SetProjectSqlPointer( CProjectSqlite* pProjectSqlite )
{
	assert(pProjectSqlite);

	m_pProjectSqlite = pProjectSqlite;
}

void CMetaFileView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (m_nSelectIndex == -1)
	//{
	//	bool bInPicture = false;
	//	CPoint pt;
	//	pt.x = point.x;
	//	pt.y = point.y;
	//	for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureView.size());++i)
	//	{
	//		if (PtInRect(m_vecPictureView[i].rectPicture,pt))
	//		{
	//			m_vecPictureView[i].bIsShowRectangle = true;
	//			bInPicture = true;
	//		}
	//		else
	//		{
	//			m_vecPictureView[i].bIsShowRectangle = false;
	//		}
	//	}
	//}
	//else
	//{
	//	for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureView.size());++i)
	//	{
	//		if (i != m_nSelectIndex)
	//		{
	//			m_vecPictureView[i].bIsShowRectangle = false;
	//		}
	//		else
	//		{
	//			m_vecPictureView[i].bIsShowRectangle = true;
	//		}
	//	}
	//}
	CView::OnMouseMove(nFlags, point);
}

void CMetaFileView::SetParent( CWnd* pParent )
{
	assert(pParent);
	m_pParent = pParent;
}


void CMetaFileView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMetaFileSelectDlg* pWnd = (CMetaFileSelectDlg*)m_pParent;
	CPoint pt;
	pt.x = point.x;
	pt.y = point.y;
	if(pWnd->m_bBtnOkShow)
	{
		for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureViewByType.size());++i)
		{
			if (PtInRect(m_vecPictureViewByType[i].rectPicture,pt))
			{
				pWnd->SetID(m_vecPictureViewByType[i].nID);
				pWnd->SetPicNum(1);
				pWnd->SetVectorPicNum(true);
				pWnd->EndDialog(1);
			}
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}


void CMetaFileView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);
	CMetaFileSelectDlg* pWnd = (CMetaFileSelectDlg*)m_pParent;
	CPoint pt;
	pt.x = point.x;
	pt.y = point.y;

	Image* pBitmap = NULL;

	if (GetKeyState(VK_CONTROL) >= 0)
	{	// control is not clicked down
		for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureViewByType.size());++i)
		{
			if (PtInRect(m_vecPictureViewByType[i].rectPicture, pt))
			{
				if (m_nSelectIndex == -1)
				{
					m_nSelectIndex = i;
					{
						for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureViewByType.size());++i)
						{
							if (i != m_nSelectIndex)
							{
								m_vecPictureViewByType[i].bIsShowRectangle = false;
							}
							else
							{
								m_vecPictureViewByType[i].bIsShowRectangle = true;
								pBitmap = m_vecPictureViewByType[i].pBitmap;
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
							for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureViewByType.size());++i)
							{
								if (i != m_nSelectIndex)
								{
									m_vecPictureViewByType[i].bIsShowRectangle = false;
								}
								else
								{
									m_vecPictureViewByType[i].bIsShowRectangle = true;
									pBitmap = m_vecPictureViewByType[i].pBitmap;
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
		for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureViewByType.size());++i)
		{
			if (PtInRect(m_vecPictureViewByType[i].rectPicture, pt))
			{
				if (m_vecPictureViewByType[i].bIsShowRectangle)
				{
					m_vecPictureViewByType[i].bIsShowRectangle = false;
				}
				else
				{
					m_vecPictureViewByType[i].bIsShowRectangle = true;
					pBitmap = m_vecPictureViewByType[i].pBitmap;
					m_nSelectIndex = i;
				}
			}
		}
	}
	InvalidateRect(rect,FALSE);

	if(pBitmap)
	{
		if(m_pImagePreviewDlg == NULL)
		{
			m_pImagePreviewDlg = new CImagePreviewDlg();
			m_pImagePreviewDlg->Create(IDD_DIALOG_IMAGE_PREVIEW, this);
		}

		int nWidth = 0;
		int nHeight = 0;
		nWidth = (pBitmap->GetWidth()>g_nMaxSize)?g_nMaxSize:pBitmap->GetWidth();
		nHeight = (pBitmap->GetHeight()>g_nMaxSize)?g_nMaxSize:pBitmap->GetHeight();

		pt.x = pt.x + 10;
		CRect rect;
		rect.left = pt.x;
		rect.top = pt.y;
		rect.right = pt.x+nWidth;
		rect.bottom = pt.y+nHeight;
		ClientToScreen(rect);
		m_pImagePreviewDlg->MoveWindow(rect.left,rect.top,nWidth+20,nHeight+40);
		m_pImagePreviewDlg->ShowPreviewImage(pBitmap,pt);
		m_pImagePreviewDlg->ShowWindow(TRUE);
		m_pImagePreviewDlg->Invalidate();
	}

	CView::OnLButtonDown(nFlags, point);
}

void CMetaFileView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMetaFileSelectDlg* pWnd = (CMetaFileSelectDlg*)m_pParent;
	CPoint pt;
	pt.x = point.x;
	pt.y = point.y;

	if(m_pImagePreviewDlg != NULL)
	{
		m_pImagePreviewDlg->ShowWindow(FALSE);
	}
	for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureViewByType.size());++i)
	{
		if (PtInRect(m_vecPictureViewByType[i].rectPicture,pt))
		{
			m_nSelectIndex = i;
			CPoint oPoint;
			GetCursorPos(&oPoint);
			CMenu* pSubMenu = pWnd->m_menu.GetSubMenu(0);
			pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, pWnd);
		}
	}
	CView::OnRButtonDown(nFlags, point);
}

void CMetaFileView::DeleteOne( const int nID )
{
	vector<PictureView>::iterator iter = m_vecPictureView.begin();
	while (iter != m_vecPictureView.end())
	{
		if ((*iter).nID == nID)
		{
			iter = m_vecPictureView.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}


BOOL CMetaFileView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;  
	GetClientRect(rect);  
	DWORD backColor = RGB(255, 255, 255);
	pDC->FillSolidRect(rect,backColor);
	return TRUE;
}

void CMetaFileView::DeleteAndInsert( const int nID, PictureView item )
{
	vector<PictureView>::iterator iter = m_vecPictureView.begin();
	while (iter != m_vecPictureView.end())
	{
		if ((*iter).nID == nID)
		{
			iter = m_vecPictureView.erase(iter);
			m_vecPictureView.insert(iter,item);
			break;
		}
		else
		{
			++iter;
		}
	}
}

void CMetaFileView::UpdatePictureViewByType( E_SubType eSubType )
{
	if(eSubType == E_TYPE_ALL)
	{
		m_vecPictureViewByType = m_vecPictureView;
		return;
	}

	CString str;
	str.Format(_T("%d"),(int)eSubType);
	wstring strSubType = str.GetString();
	m_vecPictureViewByType.clear();
	for (int i=0; i<m_vecPictureView.size(); ++i)
	{
		if(m_vecPictureView[i].etype_equip == strSubType)
		{
			m_vecPictureViewByType.push_back(m_vecPictureView[i]);
		}
	}	
}

void CMetaFileView::SetCurrentPicID( const int nID )
{
	m_vecPictureViewByType = m_vecPictureView;
	CString str;
	str.Format(_T("%d"),nID);
	wstring strSubType = str.GetString();
	for (int i=0; i<m_vecPictureViewByType.size(); ++i)
	{
		if(m_vecPictureViewByType[i].nID == nID)
		{
			m_vecPictureViewByType[i].bIsShowRectangle = true;
			const int nPage = i/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM);
			m_nCurPage = nPage;
			m_nSelectIndex = i;
		}
	}	
}

void CMetaFileView::UpdatePictureViewByName( CString strPicName )
{
	if(strPicName.GetLength()==0)
		return;

	strPicName.MakeLower();

	m_vecPictureViewByType.clear();
	for (int i=0; i<m_vecPictureView.size(); ++i)
	{
		CString strCName = m_vecPictureView[i].strName;
		strCName.MakeLower();
		if(strCName.Find(strPicName)>=0)
		{
			m_vecPictureViewByType.push_back(m_vecPictureView[i]);
		}
	}	
}

void CMetaFileView::ClearSelectPicByPageID( const int nPageID )
{
	for (size_t i=ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage;i<min(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM+ROW_PICTURE_NUM*COLUMN_PICTURE_NUM*m_nCurPage,m_vecPictureViewByType.size());++i)
	{		
		if (m_vecPictureViewByType[i].bIsShowRectangle)
		{
			m_vecPictureViewByType[i].bIsShowRectangle = false;
		}
		m_nSelectIndex = -1;
	}
}

void CMetaFileView::HidePreviewDlg()
{
	if(m_pImagePreviewDlg != NULL)
	{
		m_pImagePreviewDlg->ShowWindow(FALSE);
	}
}
