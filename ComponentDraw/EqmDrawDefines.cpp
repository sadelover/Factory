
#include "stdafx.h"
#include "EqmDrawDefines.h"


#include "EqmDrawBase.h"
#include "../ComponentDraw/EqmDrawProject.h"
#include "EqmDrawDiagnoseGraphicPlus.h"

//////////////////////////////////////////////////////////////////////////
DrawContext::DrawContext(CEqmDrawProject* project)
:m_project(project)
,pPage(NULL)
,pNewEqmt(NULL)
,type(REDRAW)
,nShowLocation(0)
,bIsCreatePipe(false)
,m_nDrawMode(EQMDRAW_UNKNOWN) //1:chiller,2:Pump,3:coolingpump,4:coolingtower,5:ahu,6:valve,7:pipe,
,m_showType(SHOWTYPE_DRAW) //显示模式，绘图模式和运行模式
,m_lineStyle(Line_Solid)
,m_bDrawingLine(false)
, m_bDragging(false)   //是否拖拽
, m_bIsChangeSize(false)    //是否需要改变大小
, m_bIsDrawCell(false)     //是否画网格
, m_bStartChangeSize(false) //是否正在改变大小
,  m_ChangeDirection(0)  //记录改变size的方向
, m_bIsCreate2DGraph(false) //是否画2D图形
, m_ifSelectBox(false) 
, m_bIsChangeBackgroundColor(false)
, m_backGroudColor(DEFAULTBACKGROUNDCOLOR)
,m_fScreenScale(1.0)
,m_defaultpipewidth(DEFAULT_PIPE_WIDTH)
,pFormatEqmt(NULL)
,m_nCurViewPageIndex(-1)
,bIsCreatePolygon(false)
{
	vecSelectEqmt.clear();
}

DrawContext::~DrawContext()
{
}

void DrawContext::ClearSelect()
{
    for ( unsigned int nIndex = 0; nIndex < vecSelectEqmt.size(); nIndex++ )
    {
		if ( NULL != vecSelectEqmt[nIndex] )
		{
			vecSelectEqmt[nIndex]->SetSelected(false);
			vecSelectEqmt[nIndex]->SetMainAlign(false);
		}
       
    }

    vecSelectEqmt.clear();
}

void DrawContext::UpdateCopiedList( const vector<CEqmDrawBase*>& deletelist )
{
		for (unsigned int j = 0; j <  deletelist.size(); j++)
		{
			for (unsigned int i = 0; i < mVecSelectEqmtCopied.size(); i++)
			{
				if (mVecSelectEqmtCopied[i] == deletelist[j])
				{
					mVecSelectEqmtCopied.erase(mVecSelectEqmtCopied.begin()+i);
					break;
				}
			}
		}

}

void DrawContext::ResetContext()
{
	vecSelectEqmt.clear();
	mVecSelectEqmtCopied.clear();
}

void DrawContext::SetProject( CEqmDrawProject* project )
{
	m_project = project;
}

CEqmDrawProject* DrawContext::GetProject() 
{
	return m_project;
}

PipeColor DrawContext::GetDefaultPipeColor() const
{
	return m_pipeColor;
}

void DrawContext::SetPipeColor( const PipeColor& pipecolor )
{
	m_pipeColor = pipecolor;
}	

DWORD DrawContext::GetScreenWidth() const
{
	ASSERT(m_project != NULL);
	return m_project->GetScreenWidth();
}

DWORD DrawContext::GetScreenHeight() const
{
	ASSERT(m_project != NULL);
	return m_project->GetScreenHeight();
}

void DrawContext::SetScreenWidth( DWORD width )
{
	ASSERT(m_project != NULL);
	m_project->SetScreenWidth(width);
}

void DrawContext::SetScreenHeight( DWORD height )
{
	ASSERT(m_project != NULL);
	m_project->SetScreenHeight(height);
}

DWORD DrawContext::GetScaledWidth()
{
	return static_cast<DWORD>(m_fScreenScale*GetScreenWidth());
}

DWORD DrawContext::GetScaledheight()
{
	return static_cast<DWORD>(m_fScreenScale*GetScreenHeight());
}

int DrawContext::GetDefaultPipeWidth() const
{
	return m_defaultpipewidth;
}

void DrawContext::AddViewedPage(int nTabIndex)
{
	if(m_pViewedPageTabIndexList.size()>0 &&  m_nCurViewPageIndex>=0 && m_nCurViewPageIndex< m_pViewedPageTabIndexList.size()-1)
	{
		for(int i= 0; i<m_pViewedPageTabIndexList.size()-1-m_nCurViewPageIndex;i++)
		{
			m_pViewedPageTabIndexList.pop_back();
		}
	}
	if (m_pViewedPageTabIndexList.size() < 1)
	{
		m_pViewedPageTabIndexList.push_back(nTabIndex);
	}
	else
	{
		if (m_pViewedPageTabIndexList[m_pViewedPageTabIndexList.size()-1] != nTabIndex)
		{
			m_pViewedPageTabIndexList.push_back(nTabIndex);
		}
	}
	if(m_pViewedPageTabIndexList.size()>=100)
	{
		m_pViewedPageTabIndexList.erase(m_pViewedPageTabIndexList.begin());
	}
	m_nCurViewPageIndex = m_pViewedPageTabIndexList.size()-1;
}

void DrawContext::SetDefaultPipeWidth( int pipewidth )
{
	
	m_defaultpipewidth = pipewidth;
}

void DrawContext::SetLineWidth( const int width )
{
	m_iLineWidth = width;
}

const int DrawContext::GetLineWidth() const
{
	return m_iLineWidth;
}

void DrawContext::SetLineColor( const DWORD color )
{
	m_dwLineColor = color;
}

const DWORD DrawContext::GetLineColor() const
{
	return m_dwLineColor;
}

