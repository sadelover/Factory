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
*	\param pWnd ��ͼ���������ָ�룬����ΪNULL
*	\param pRect ���ڻ�ͼ������ο�ָ�룬Ĭ��ֵΪNULL��ʾ��ͼ��Ϊ�������ڿͻ���
*/
void ComponentDraw::init(CWnd* pWnd, CRect* pRect)
{
	assert(IsBadCodePtr(FARPROC(pWnd)) == 0 && _T("window object invalid��init failed��"));

	if ( NULL != m_pWndDC && IsWindow(m_pWnd->m_hWnd))
	{
		m_pWnd->ReleaseDC(m_pWndDC);
		m_pWndDC = NULL;	

	}

	m_pWnd = pWnd;
	m_pWndDC = m_pWnd->GetDC();//����dc
}

/** 
*	\param pFrame ��ÿһ֡ͼ������Ҫ����Ϣ�ṹ����ָ�룬Ĭ��ΪNULL��ʾû��֡��Ϣ
*	\param bDoubleBuffer �Ƿ����˫���壬Ĭ��ֵΪtrue
*/
void ComponentDraw::draw(DrawContext* pFrame)
{
	if (pFrame == NULL)
	{
		return;
	}
	m_pDrawContext = pFrame;
	//���¾�̬��ͼ�ڴ�
	
	// ��ͼ����ı�ʱ���»��ƾ�̬����
	if(m_bRectChanged || m_bStartup|| m_pDrawContext->type == FLASHSCREEN || m_pDrawContext->m_bIsChangeBackgroundColor)
	{
		updateStaticContex();
		setDrawObject(m_pStaticDC);
		prepareDraw();
		
		//���ƾ�̬����
		drawStaticScene();
		
		//����ǳ�ʼ�������ʼ����̬��ͼ���ݡ�
		//ֻ��ʼ��һ�Ρ�
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

	// ����̬�����������ڴ�dc��
	m_pMemDC->BitBlt(0, 0, screenwidth, screenheight, m_pStaticDC, 0, 0, SRCCOPY);

	// ÿ��ˢ��ʱ���ƶ�̬����
	//��̬�����Ȼ������ڴ�DC�ϣ��ٿ���������DC�ϡ�
	setDrawObject(m_pMemDC);
	prepareDraw();

	drawDynamicScene();
	m_pWndDC->StretchBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),m_pMemDC, 0, 0, screenwidth, screenheight, SRCCOPY);
}

void ComponentDraw::prepareDraw()
{

}

/** 
*	\param pDC ��ͼdc
*	
*	��̬������ָ����ʱ���˶���ͼ�β��֣�
*	�ⲿ��ͼ�β���Ҫ��ÿ��ˢ��ʱ�������ػ�.
*/
void ComponentDraw::drawStaticScene()
{

}

/** 
*	\param pDC ��ͼdc
*
*	��̬������ָ������ʱ��ı��ͼ�β��֣�
*	�ⲿ��ͼ����ÿ��ˢ�µ�ʱ����Ҫ�����ػ�.
*/
void ComponentDraw::drawDynamicScene()
{

}

/** 
*	\param rect ��ͼ����С���ο�
*	
*	�������û�ͼ�����С�󣬻�������̬�����ػ档
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
*	\return ��ͼ�������˸ı䷵��true����֮����false
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
	m_pGC = Graphics::FromHDC(m_pDC->GetSafeHdc());// �ӵ�ǰ��ͼdc��ȡ��Ӧ��gc
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
			assert( m_pStaticDC != NULL && _T("������̬��ͼdcʧ�ܣ�"));
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
			assert( m_pMemDC != NULL && _T("�����ڴ�dcʧ�ܣ�"));
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
