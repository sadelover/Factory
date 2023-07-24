#include "StdAfx.h"
#include "DrawObject.h"
#include <assert.h>
#include "EqmDrawDefines.h"
#include "EqmDrawProject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////
//ComponentDraw*   ComponentDraw::m_pThis =NULL;
//////////////////////////////////////////////////////////////////////////
ComponentDraw::ComponentDraw()
{
	m_bStartup	   = true;
	m_bRectChanged = false;
	m_bChooseGrid  = false;
	m_pWnd = NULL;
	m_rect = 0;

	m_pMemDC = NULL;
	m_pWndDC = NULL;
	m_pStaticDC = NULL;

	m_pDC = NULL;
	m_pGC = NULL;
	m_backGroudColor = DEFAULTBACKGROUNDCOLOR;

	m_pDrawContext = NULL;
    //m_pThis = this;
}

ComponentDraw::~ComponentDraw()
{
	if(m_pWnd && IsWindow(m_pWnd->m_hWnd) && m_pWndDC)
	{
		m_pWnd->ReleaseDC(m_pWndDC);
		m_pWndDC = NULL;
		releaseMemDC();
		releaseGC();
	}
}
//////////////////////////////////////////////////////////////////////////

void ComponentDraw::releaseMemDC()
{
	if(m_pMemDC != NULL)
	{
		m_pMemDC->DeleteDC();
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if(m_pStaticDC != NULL)
	{
		m_pStaticDC->DeleteDC();
		delete m_pStaticDC;
		m_pStaticDC = NULL;
	}
}

void ComponentDraw::releaseGC()
{
	if(m_pGC != NULL)
	{
		delete m_pGC;
		m_pGC = NULL;
	}
}

/** 
*	\param pWnd 绘图窗口类对象指针，不能为NULL
*	\param pRect 窗口绘图区域矩形框指针，默认值为NULL表示绘图区为整个窗口客户区
*/
void ComponentDraw::init(CWnd* pWnd, CRect* pRect)
{
	assert(IsBadCodePtr(FARPROC(pWnd)) == 0 && _T("window object invalid，init failed！"));

	if ( NULL != m_pWndDC && IsWindow(m_pWnd->m_hWnd))
	{
		m_pWnd->ReleaseDC(m_pWndDC);
		m_pWndDC = NULL;	

	}

	m_pWnd = pWnd;
	m_pWndDC = m_pWnd->GetDC();//窗口dc
}

/** 
*	\param pFrame 画每一帧图像所需要的信息结构对象指针，默认为NULL表示没有帧信息
*	\param bDoubleBuffer 是否采用双缓冲，默认值为true
*/
void ComponentDraw::draw(DrawContext* pFrame)
{
	if (pFrame == NULL)
	{
		return;
	}
	m_pDrawContext = pFrame;
	//更新静态画图内存
	
	// 绘图区域改变时重新绘制静态场景
	if(m_bRectChanged || m_bStartup|| m_pDrawContext->type == FLASHSCREEN || m_pDrawContext->m_bIsChangeBackgroundColor)
	{
		updateStaticContex();
		setDrawObject(m_pStaticDC);
		prepareDraw();
		
		//绘制静态内容
		drawStaticScene();
		
		//如果是初始化，则初始化动态绘图内容。
		//只初始化一次。
		if (m_bStartup || m_bRectChanged )
		{
			DeleteDynamicMemDC();
			CreateDynamicMemDC();
		}
		

		m_bRectChanged = false;	
		m_bStartup = false;
	}
	DWORD screenwidth = 0;
	DWORD screenheight = 0;
	screenwidth = m_pDrawContext->GetScreenWidth();
	screenheight = m_pDrawContext->GetScreenHeight();

	// 将静态场景拷贝到内存dc上
	m_pMemDC->BitBlt(0, 0, screenwidth, screenheight, m_pStaticDC, 0, 0, SRCCOPY);

	// 每次刷新时绘制动态场景
	//动态场景先绘制在内存DC上，再拷贝到窗口DC上。
	setDrawObject(m_pMemDC);
	prepareDraw();

	drawDynamicScene();
	m_pWndDC->StretchBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),m_pMemDC, 0, 0, screenwidth, screenheight, SRCCOPY);
}

void ComponentDraw::prepareDraw()
{

}

/** 
*	\param pDC 画图dc
*	
*	静态场景是指不随时间运动的图形部分，
*	这部分图形不需要在每次刷新时都进行重绘.
*/
void ComponentDraw::drawStaticScene()
{

}

/** 
*	\param pDC 画图dc
*
*	动态场景是指会随着时间改变的图形部分，
*	这部分图形在每次刷新的时候都需要进行重绘.
*/
void ComponentDraw::drawDynamicScene()
{

}

/** 
*	\param rect 绘图区大小矩形框
*	
*	重新设置绘图区域大小后，会引发静态场景重绘。
*/
void ComponentDraw::setRect(CRect rect)
{
	if(m_rect == rect)
		m_bRectChanged = false;
	else
	{
		m_bRectChanged = true;
		m_rect = rect;
	}
}

/** 
*	\return 绘图区域发生了改变返回true，反之返回false
*/
bool ComponentDraw::isRectChanged()
{
	return m_bRectChanged;
}

void ComponentDraw::updateStaticContex()
{
	releaseGC();
	DeleteStaticMemDC();
	CreateStaticMemDC();
}

void ComponentDraw::setDrawObject(CDC* pDC)
{
	m_pDC = pDC;
	releaseGC();
	m_pGC = Graphics::FromHDC(m_pDC->GetSafeHdc());// 从当前画图dc获取对应的gc
}

CWnd* ComponentDraw::GetWnd()
{
	return m_pWnd;
}

void ComponentDraw::SetRectChanged( bool bRectChange )
{
	m_bRectChanged = bRectChange;
}

void ComponentDraw::SetChooseGrid( bool bChooseGrid )
{
	m_bChooseGrid = bChooseGrid;
}

bool ComponentDraw::GetChooseGrid()
{
	return m_bChooseGrid;
}

void ComponentDraw::CreateStaticMemDC()
{
	if (m_pDrawContext != NULL)
	{
		DWORD screenwidth = 0;
		DWORD screenheight = 0;
		screenwidth = m_pDrawContext->GetScreenWidth();
		screenheight = m_pDrawContext->GetScreenHeight();

		if(m_pStaticDC == NULL)
		{
			m_pStaticDC = new CMemDrawDC(m_pWndDC, &CRect(0,0, screenwidth, screenheight) );
			assert( m_pStaticDC != NULL && _T("创建静态画图dc失败！"));
		}
	}
}

void ComponentDraw::DeleteStaticMemDC()
{
	if(m_pStaticDC != NULL)
	{
		delete m_pStaticDC;
		m_pStaticDC = NULL;
	}
}

void ComponentDraw::CreateDynamicMemDC()
{
	if (m_pDrawContext != NULL)
	{
		DWORD screenwidth = 0;
		DWORD screenheight = 0;
		screenwidth = m_pDrawContext->GetScreenWidth();
		screenheight = m_pDrawContext->GetScreenHeight();

		if(m_pMemDC == NULL)
		{
			m_pMemDC = new CMemDrawDC(m_pWndDC, &CRect(0, 0, screenwidth, screenheight));
			assert( m_pMemDC != NULL && _T("创建内存dc失败！"));
		}
	}
}

void ComponentDraw::DeleteDynamicMemDC()
{
	if(m_pMemDC != NULL)
	{
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}
