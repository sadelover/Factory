
#include "stdafx.h"

#include "CEqmDrawPage.h"
#include <cassert>

#include "EqmDrawDevice.h"
#include "EqmDrawPipe.h"
#include "EqmDrawText.h"
#include "EqmDrawGraphic.h"
//严观微增加
#include "EqmDrawBend.h"
#include "EqmDrawDashBoard.h"
#include "EqmDrawLineGraphic.h"
#include "EqmDrawBarGraphic.h"
#include "EqmDrawPieGraphic.h"
#include "EqmDrawButton.h"
#include "EqmDrawTimeSelect.h"
#include "EqmDrawTimeBar.h"
#include "EqmDrawProgressBar.h"
#include "EqmDrawDiagnoseGraphic.h"
#include "EqmDrawPolygon.h"
#include "EqmDrawShellExecute.h"
#include "EqmDrawEmbededPage.h"
#include "EqmDrawLiquidLevel.h"
#include "EqmDrawButtonPlus.h"
#include "EqmDrawScrawl.h"
#include "EqmDrawAreaChange.h"
#include "EqmDrawHistoryTrend.h"
#include "EqmDrawEnergySaveRoi.h"
#include "EqmDrawMapNavigate.h"
#include "EqmDrawNavigateWindow.h"
#include "EqmDrawLevelRuler.h"
#include "EqmDrawDataAnalyse.h"
#include "EqmDrawTemperDistri.h"
#include "EqmDrawDataReport.h"
#include "EqmDrawPlaneTempeDistr.h"
#include "EqmDrawDottedLineFrm.h"
#include "EqmDrawGeneralComponent.h"
#include "EqmDrawRectangle.h"
#include "EqmDrawStraightLine.h"

list<CEqmDrawDevice*>& CEqmDrawPage::GetDrawDeviceList()
{
	return m_devicelist;
}

list<CEqmDrawText*>& CEqmDrawPage::GetPageTextList()
{
	return m_textlist;
}

list<CEqmDrawPipe*>& CEqmDrawPage::GetPagePipeList()
{
	return m_pipelist;
}


CEqmDrawPage::CEqmDrawPage( int pageid, const wstring& pagename, 
						   int projectid, int pageshoworder,
						   PAGETYPE mtype,int mwidth,int mheight,
						   int mxposition,int myposition,DWORD mbackgroundColor )
					:m_pageid(pageid),
					m_pagename(pagename),
					m_projectid(projectid),
					m_type(mtype),
					m_width(mwidth),
					m_height(mheight),
					m_xposition(mxposition),
					m_yposition(myposition),
					m_nStartColor(mbackgroundColor),
					m_nEndColor(mbackgroundColor),
					m_nBKImageID(0),
					m_bIsMainPage(false),
					m_nWarningCount(0),
					m_pBkImage(NULL),
					m_nPageOrder(pageshoworder)
{	
	//m_pOsgNode->setMatrix(osg::Matrix::scale(1,1,1)*osg::Matrix::translate(0,0,0));
	m_GroupID  = -1;
	m_bNeedSave = true;
	m_bShowTopInObserver = false;

	m_bDetailLoaded = false;
	m_pPickText = NULL;
	m_nPageMinRight = 0;
	
}

CEqmDrawPage::CEqmDrawPage()
:m_pageid(0),
m_projectid(0),
m_type(FULLSCREEN),
m_width(0),
m_height(0),
m_xposition(0),
m_yposition(0),
m_nStartColor(DEFAULTBACKGROUNDCOLOR),
m_nEndColor(0),
m_nBKImageID(0),
m_bIsMainPage(false),
m_nWarningCount(0),
m_pBkImage(NULL)
{
	m_bNeedSave = true;
	m_bShowTopInObserver = false;

	m_bDetailLoaded = false;
	m_pPickText = NULL;
	m_nPageMinRight = 0;
}

void CEqmDrawPage::AddBase(CEqmDrawBase* pBase)
{
	assert(pBase);
	if(!pBase) return;

	CEqmDrawDevice* pDevice = dynamic_cast<CEqmDrawDevice*>(pBase);
	if(pDevice)
	{
		AddDevice(pDevice);
		return;
	}

	CEqmDrawText* pText = dynamic_cast<CEqmDrawText*>(pBase);
	if(pText)
	{
		AddText(pText);
		return;
	}

	CEqmDrawPipe* pPipe = dynamic_cast<CEqmDrawPipe*>(pBase);
	if(pPipe)
	{
		AddPipe(pPipe);
		return;
	}

	CEqmDrawGraphic* pGraph= dynamic_cast<CEqmDrawGraphic*>(pBase);
	if(pGraph)
	{
		Add2DGraphic(pGraph);
		return;
	}

	CEqmDrawLineGraphic* pLineGraphic = dynamic_cast<CEqmDrawLineGraphic*>(pBase);
	if (pLineGraphic)
	{
		AddLineGraphic(pLineGraphic);
		return;
	}

	CEqmDrawBarGraphic* pBarGraphic = dynamic_cast<CEqmDrawBarGraphic*>(pBase);
	if (pBarGraphic)
	{
		AddBarGraphic(pBarGraphic);
		return;
	}

	CEqmDrawPieGraphic* pPieGraphic = dynamic_cast<CEqmDrawPieGraphic*>(pBase);
	if (pPieGraphic)
	{
		AddPieGraphic(pPieGraphic);
		return;
	}

	CEqmDrawDashBoard* pDashBoard = dynamic_cast<CEqmDrawDashBoard*>(pBase);
	if (pDashBoard)
	{
		AddDashboard(pDashBoard);
		return;
	}

	CEqmDrawBend* pBend = dynamic_cast<CEqmDrawBend*>(pBase);
	if (pBend)
	{
		AddBend(pBend);
		return;
	}

	CEqmDrawButton* pButton = dynamic_cast<CEqmDrawButton*>(pBase);
	if (pButton)
	{
		AddButton(pButton);
		return;
	}

	CEqmDrawTimeSelect* pTimeSelect = dynamic_cast<CEqmDrawTimeSelect*>(pBase);
	if (pTimeSelect)
	{
		AddTimeSelect(pTimeSelect);
		return;
	}

	CEqmDrawTimeBar* pTimeBar = dynamic_cast<CEqmDrawTimeBar*>(pBase);
	if (pTimeBar)
	{
		AddTimeBar(pTimeBar);
		return;
	}

	CEqmDrawProgressBar* pProgressBar = dynamic_cast<CEqmDrawProgressBar*>(pBase);
	if (pProgressBar)
	{
		AddProgressBar(pProgressBar);
		return;
	}

	CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = dynamic_cast<CEqmDrawDiagnoseGraphic*>(pBase);
	if (pDiagnoseGraphic)
	{
		AddDiagnoseGraphic(pDiagnoseGraphic);
		return;
	}

	CEqmDrawPolygon* pPolygon = dynamic_cast<CEqmDrawPolygon*>(pBase);
	if (pPolygon)
	{
		AddPolygon(pPolygon);
		return;
	}

	CEqmDrawAreaChange* pAreaChange = dynamic_cast<CEqmDrawAreaChange*>(pBase);
	if (pAreaChange)
	{
		AddAreaChange(pAreaChange);
		return;
	}

	CEqmDrawShellExecute* pShellExecute = dynamic_cast<CEqmDrawShellExecute*>(pBase);
	if (pShellExecute)
	{
		AddShellExecute(pShellExecute);
		return;
	}

	CEqmDrawEmbededPage* pEmbededPage = dynamic_cast<CEqmDrawEmbededPage*>(pBase);
	if (pEmbededPage)
	{
		AddEmbededPage(pEmbededPage);
		return;
	}

	CEqmDrawLiquidLevel* pLiquidLevel = dynamic_cast<CEqmDrawLiquidLevel*>(pBase);
	if (pLiquidLevel)
	{
		AddLiquidLevel(pLiquidLevel);
		return;
	}

	CEqmDrawButtonPlus* pChkBtnPlus = dynamic_cast<CEqmDrawButtonPlus*>(pBase);
	if (pChkBtnPlus)
	{
		AddCheckButtonPlus(pChkBtnPlus);
		return;
	}

	CEqmDrawScrawl* pScrawl = dynamic_cast<CEqmDrawScrawl*>(pBase);
	if (pScrawl != NULL)
	{
		AddScrawlBoard(pScrawl);
		return;
	}

	CEqmDrawHistoryTrend* pHistTrend = dynamic_cast<CEqmDrawHistoryTrend*>(pBase);
	if (pHistTrend != NULL)
	{
		AddHistoryTrend(pHistTrend);
		return;
	}

	CEqmDrawEnergySaveRoi* pEnergySaveRoi = dynamic_cast<CEqmDrawEnergySaveRoi*>(pBase);
	if (pEnergySaveRoi != NULL)
	{
		AddEnergySaveRoi(pEnergySaveRoi);
		return;
	}

	CEqmDrawMapNavigate* pMapNavigate = dynamic_cast<CEqmDrawMapNavigate*>(pBase);
	if (pMapNavigate != NULL)
	{
		AddMapNavigate(pMapNavigate);
		return;
	}

	CEqmDrawNavigateWindow* pNavigateWindow = dynamic_cast<CEqmDrawNavigateWindow*>(pBase);
	if (pNavigateWindow != NULL)
	{
		AddNavigateWindow(pNavigateWindow);
		return;
	}

	CEqmDrawLevelRuler* pLevelRuler = dynamic_cast<CEqmDrawLevelRuler*>(pBase);
	if (pLevelRuler != NULL)
	{
		AddLevelRuler(pLevelRuler);
		return;
	}

	CEqmDrawDataAnalyse* pDataAnalyse = dynamic_cast<CEqmDrawDataAnalyse*>(pBase);
	if (pDataAnalyse != NULL)
	{
		AddDataAnalyse(pDataAnalyse);
		return;
	}

	CEqmDrawTemperDistri* pTemperDistri = dynamic_cast<CEqmDrawTemperDistri*>(pBase);
	if (pTemperDistri != NULL)
	{
		AddTemperDistri(pTemperDistri);
		return;
	}

	CEqmDrawDataReport* pDataReport = dynamic_cast<CEqmDrawDataReport*>(pBase);
	if (pDataReport != NULL)
	{
		AddDataReport(pDataReport);
		return;
	}

	CEqmDrawPlaneTempeDistr* pPlaneTempeDistr = dynamic_cast<CEqmDrawPlaneTempeDistr*>(pBase);
	if (pPlaneTempeDistr != NULL)
	{
		AddPlaneTempeDistr(pPlaneTempeDistr);
		return;
	}

	CEqmDrawDottedLineFrm* pDottedLineFrm = dynamic_cast<CEqmDrawDottedLineFrm*>(pBase);
	if (pDottedLineFrm != NULL)
	{
		AddDottedLineFrm(pDottedLineFrm);
		return;
	}

	CEqmDrawGeneralComponent* pGenComFrm = dynamic_cast<CEqmDrawGeneralComponent*>(pBase);
	if (pGenComFrm != NULL)
	{
		AddGeneralComponent(pGenComFrm);
		return;
	}
	CEqmDrawRectangle* pRectangle = dynamic_cast<CEqmDrawRectangle*>(pBase);
	if (pRectangle != NULL)
	{
		AddRectangle(pRectangle);
		return;
	}
	CEqmDrawStraightLine* pStraight = dynamic_cast<CEqmDrawStraightLine*>(pBase);
	if (pStraight != NULL)
	{
		AddStraightLine(pStraight);
		return;
	}
}

void CEqmDrawPage::AddDevice( CEqmDrawDevice* device )
{
	assert(device != NULL);
	m_devicelist.push_back(device);
}

void CEqmDrawPage::AddText( CEqmDrawText* text )
{
	assert(text != NULL);
	m_textlist.push_back(text);
}

void CEqmDrawPage::AddPipe( CEqmDrawPipe* pipe )
{
	assert(pipe != NULL);
	m_pipelist.push_back(pipe);
}

void CEqmDrawPage::DeleteBase(CEqmDrawBase* pBase)
{
    assert(pBase);
    if(!pBase) return;

    CEqmDrawDevice* pDevice = dynamic_cast<CEqmDrawDevice*>(pBase);
    if(pDevice)
	{
        DeleteDevice(pDevice);
        return;
    }

    CEqmDrawText* pText = dynamic_cast<CEqmDrawText*>(pBase);
    if(pText)
	{
        DeleteText(pText);
        return;
    }

    CEqmDrawPipe* pPipe = dynamic_cast<CEqmDrawPipe*>(pBase);
    if(pPipe)
	{
        DeletePipe(pPipe);
        return;
    }

    CEqmDrawGraphic* pGraph= dynamic_cast<CEqmDrawGraphic*>(pBase);
    if(pGraph)
	{
        Delete2DGraphic(pGraph);
        return;
    }

	CEqmDrawBend* pBend = dynamic_cast<CEqmDrawBend*>(pBase);
	if (pBend)
	{
		DeleteBend(pBend);
		return;
	}

	CEqmDrawDashBoard* pDashBoard = dynamic_cast<CEqmDrawDashBoard*>(pBase);
	if (pDashBoard)
	{
		DeleteDashboard(pDashBoard);
		return;
	}

	CEqmDrawLineGraphic* pLineGraphic = dynamic_cast<CEqmDrawLineGraphic*>(pBase);
	if (pLineGraphic)
	{
		DeleteLineGraphic(pLineGraphic);
		return;
	}

	CEqmDrawBarGraphic* pBarGraphic = dynamic_cast<CEqmDrawBarGraphic*>(pBase);
	if (pBarGraphic)
	{
		DeleteBarGraphic(pBarGraphic);
		return;
	}

	CEqmDrawPieGraphic* pPieGraphic = dynamic_cast<CEqmDrawPieGraphic*>(pBase);
	if (pPieGraphic)
	{
		DeletePieGraphic(pPieGraphic);
		return;
	}

	CEqmDrawButton* pButton = dynamic_cast<CEqmDrawButton*>(pBase);
	if (pButton)
	{
		DeleteButton(pButton);
		return;
	}

	CEqmDrawTimeSelect* pTimeSelect = dynamic_cast<CEqmDrawTimeSelect*>(pBase);
	if (pTimeSelect)
	{
		DeleteTimeSelect(pTimeSelect);
		return;
	}

	CEqmDrawTimeBar* pTimeBar = dynamic_cast<CEqmDrawTimeBar*>(pBase);
	if (pTimeBar)
	{
		DeleteTimeBar(pTimeBar);
		return;
	}

	CEqmDrawProgressBar* pProgressBar = dynamic_cast<CEqmDrawProgressBar*>(pBase);
	if (pProgressBar)
	{
		DeleteProgressBar(pProgressBar);
		return;
	}


	CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = dynamic_cast<CEqmDrawDiagnoseGraphic*>(pBase);
	if (pDiagnoseGraphic)
	{
		DeleteDiagnoseGraphic(pDiagnoseGraphic);
		return;
	}

	CEqmDrawPolygon* pPolygon = dynamic_cast<CEqmDrawPolygon*>(pBase);
	if (pPolygon)
	{
		DeletePolygon(pPolygon);
		return;
	}

	CEqmDrawAreaChange* pAreaChange = dynamic_cast<CEqmDrawAreaChange*>(pBase);
	if (pAreaChange)
	{
		DeleteAreaChange(pAreaChange);
		return;
	}

	CEqmDrawShellExecute* pShellExecute = dynamic_cast<CEqmDrawShellExecute*>(pBase);
	if (pShellExecute)
	{
		DeleteShellExecute(pShellExecute);
		return;
	}

	CEqmDrawEmbededPage* pEmbededPage = dynamic_cast<CEqmDrawEmbededPage*>(pBase);
	if (pEmbededPage)
	{
		DeleteEmbededPage(pEmbededPage);
		return;
	}

	CEqmDrawLiquidLevel* pLiquidLevel = dynamic_cast<CEqmDrawLiquidLevel*>(pBase);
	if (pLiquidLevel != NULL)
	{
		DeleteLiquidLevel(pLiquidLevel);
		return;
	}

	CEqmDrawButtonPlus* pChkBtnPlus = dynamic_cast<CEqmDrawButtonPlus*>(pBase);
	if (pChkBtnPlus != NULL)
	{
		DeleteCheckButtonPlus(pChkBtnPlus);
		return;
	}

	CEqmDrawScrawl* pScrawl = dynamic_cast<CEqmDrawScrawl*>(pBase);
	if (pScrawl != NULL)
	{
		DeleteScrawlBoard(pScrawl);
		return;
	}

	CEqmDrawHistoryTrend* pHistTrend = dynamic_cast<CEqmDrawHistoryTrend*>(pBase);
	if (pHistTrend != NULL)
	{
		DeleteHistoryTrend(pHistTrend);
		return;
	}

	CEqmDrawEnergySaveRoi* pEnergySaveRoi = dynamic_cast<CEqmDrawEnergySaveRoi*>(pBase);
	if (pEnergySaveRoi != NULL)
	{
		DeleteEnergySaveRoi(pEnergySaveRoi);
		return;
	}

	CEqmDrawMapNavigate* pMapNavigate = dynamic_cast<CEqmDrawMapNavigate*>(pBase);
	if (pMapNavigate != NULL)
	{
		DeleteMapNavigate(pMapNavigate);
		return;
	}

	CEqmDrawNavigateWindow* pNavigateWindow = dynamic_cast<CEqmDrawNavigateWindow*>(pBase);
	if (pNavigateWindow != NULL)
	{
		DeleteNavigateWindow(pNavigateWindow);
		return;
	}

	CEqmDrawLevelRuler* pLevelRuler = dynamic_cast<CEqmDrawLevelRuler*>(pBase);
	if (pLevelRuler != NULL)
	{
		DeleteLevelRuler(pLevelRuler);
		return;
	}

	CEqmDrawDataAnalyse* pDataAnalyse = dynamic_cast<CEqmDrawDataAnalyse*>(pBase);
	if (pDataAnalyse != NULL)
	{
		DeleteDataAnalyse(pDataAnalyse);
		return;
	}

	CEqmDrawTemperDistri* pTemperDistri = dynamic_cast<CEqmDrawTemperDistri*>(pBase);
	if (pTemperDistri != NULL)
	{
		DeleteTemperDistri(pTemperDistri);
		return;
	}

	CEqmDrawDataReport* pDataReport = dynamic_cast<CEqmDrawDataReport*>(pBase);
	if (pDataReport != NULL)
	{
		DeleteDataReport(pDataReport);
		return;
	}

	CEqmDrawPlaneTempeDistr* pPlaneTempeDistr = dynamic_cast<CEqmDrawPlaneTempeDistr*>(pBase);
	if (pPlaneTempeDistr != NULL)
	{
		DeletePlaneTempeDistr(pPlaneTempeDistr);
		return;
	}

	CEqmDrawDottedLineFrm* pDottedLineFrm = dynamic_cast<CEqmDrawDottedLineFrm*>(pBase);
	if (pDottedLineFrm != NULL)
	{
		DeleteDottedLineFrm(pDottedLineFrm);
		return;
	}

	CEqmDrawGeneralComponent* pGeneralComponent = dynamic_cast<CEqmDrawGeneralComponent*>(pBase);
	if (pGeneralComponent != NULL)
	{
		DeleteGeneralComponent(pGeneralComponent);
		return;
	}
	CEqmDrawRectangle* pRectangle = dynamic_cast<CEqmDrawRectangle*>(pBase);
	if (pRectangle != NULL)
	{
		DeleteRectangle(pRectangle);
		return;
	}
	CEqmDrawStraightLine* pStraightLine = dynamic_cast<CEqmDrawStraightLine*>(pBase);
	if (pStraightLine != NULL)
	{
		DeleteStraightLine(pStraightLine);
		return;
	}
}

void CEqmDrawPage::DeleteDevice( CEqmDrawDevice* device )
{	
	assert(device != NULL);
	CEqmDrawDevice* d = device;
	m_devicelist.remove(device);
	delete d;
}

void CEqmDrawPage::DeletePipe( CEqmDrawPipe* text )
{
	assert(text != NULL);
	CEqmDrawPipe* p = text;
	m_pipelist.remove(p);
	delete p;
}

void CEqmDrawPage::DeleteText( CEqmDrawText* pipe )
{
	assert(pipe != NULL);
	CEqmDrawText* t = pipe;
	m_textlist.remove(pipe);
	delete t;
}

int CEqmDrawPage::GetID() const
{
	return m_pageid;
}

void CEqmDrawPage::SetID(int pageid)
{
	m_pageid = pageid;
	m_bNeedSave = true;
}

UINT CEqmDrawPage::GetObjectsMaxId() const
{
    UINT maxid = 1;
    UINT tempId = 0;

    for (list<CEqmDrawDevice*>::const_iterator it = m_devicelist.begin();
        it != m_devicelist.end(); it++)
    {
        tempId = (*it)->GetID();
        if (tempId > maxid)
            maxid = tempId;
    }
    for (list<CEqmDrawPipe*>::const_iterator it = m_pipelist.begin();
        it != m_pipelist.end(); it++)
    {
        tempId = (*it)->GetID();
        if (tempId > maxid)
            maxid = tempId;
    }

    for (list<CEqmDrawText*>::const_iterator it = m_textlist.begin();
        it != m_textlist.end(); it++)
    {
        tempId = (*it)->GetID();
        if (tempId > maxid)
            maxid = tempId;
    }

    for (list<CEqmDrawGraphic*>::const_iterator it = m_graphiclist.begin();
        it != m_graphiclist.end(); it++)
    {
        tempId = (*it)->GetID();
        if (tempId > maxid)
            maxid = tempId;
    }

	for (list<CEqmDrawBend*>::const_iterator it = m_bendlist.begin();
		it != m_bendlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}
	
	for (list<CEqmDrawDashBoard*>::const_iterator it = m_dashboardlist.begin();
		it != m_dashboardlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawLineGraphic*>::const_iterator it = m_linechartlist.begin();
		it != m_linechartlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}
	
	for (list<CEqmDrawBarGraphic*>::const_iterator it = m_barchartlist.begin();
		it != m_barchartlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}
	
	for (list<CEqmDrawPieGraphic*>::const_iterator it = m_piechartlist.begin();
		it != m_piechartlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}
	
	for (list<CEqmDrawButton*>::const_iterator it = m_buttonlist.begin();
		it != m_buttonlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}
	
	for (list<CEqmDrawTimeSelect*>::const_iterator it = m_timeselectlist.begin();
		it != m_timeselectlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawTimeBar*>::const_iterator it = m_timebarlist.begin();
		it != m_timebarlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawProgressBar*>::const_iterator it = m_progressbarlist.begin();
		it != m_progressbarlist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawDiagnoseGraphic*>::const_iterator it = m_diagnoselist.begin();
		it != m_diagnoselist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawPolygon*>::const_iterator it = m_polygonList.begin();
		it != m_polygonList.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawAreaChange*>::const_iterator it = m_areachangelist.begin();
		it != m_areachangelist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawShellExecute*>::const_iterator it = m_shellexecutelist.begin();
		it != m_shellexecutelist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawEmbededPage*>::const_iterator it = m_embededpagelist.begin();
		it != m_embededpagelist.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawRectangle*>::const_iterator it = m_rectangleList.begin();
		it != m_rectangleList.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}

	for (list<CEqmDrawStraightLine*>::const_iterator it = m_straightLineList.begin();
		it != m_straightLineList.end(); it++)
	{
		tempId = (*it)->GetID();
		if (tempId > maxid)
			maxid = tempId;
	}
    return maxid;
}
//////////////////////////////////////////////////////////////////////////

wstring CEqmDrawPage::GetPageName() const
{
	return m_pagename;
}

void CEqmDrawPage::SetPageName( const wstring& pagename )
{
	m_pagename = pagename;
	m_bNeedSave = true;
}

CEqmDrawPage::~CEqmDrawPage()
{
	ClearAll();
}

CEqmDrawDevice* CEqmDrawPage::GetDevice( int id )
{
	for (list<CEqmDrawDevice*>::iterator it = m_devicelist.begin();it != m_devicelist.end(); it++)
	{
		if ((*it)->GetID() == id)
			return  (*it);
	}
	return NULL;
}

CEqmDrawPipe* CEqmDrawPage::GetPipe( int id )
{
	for (list<CEqmDrawPipe*>::iterator it = m_pipelist.begin();it != m_pipelist.end(); it++)
	{
		if ((*it)->GetID() == id)
			return  (*it);
	}
	return NULL;
}

CEqmDrawText* CEqmDrawPage::GetText( int id )
{
	for (list<CEqmDrawText*>::iterator it = m_textlist.begin();it != m_textlist.end(); it++)
	{
		if ((*it)->GetID() == id)
			return  (*it);
	}	
	return NULL;
}

bool CEqmDrawPage::FindText(wstring strPointName, vector<CEqmDrawText *> & textList)
{
	bool bFound = false;
	for (list<CEqmDrawText*>::iterator it = m_textlist.begin();
		it != m_textlist.end(); it++)
	{
		if ((*it)->GetPhysicalID() == strPointName)
		{
			//textList 不存在才添加
			bool bExist = false;
			for(int i=0;i<textList.size(); i++)
			{
				if((*it) == textList[i])
				{
					bExist = true;
					break;
				}
			}

			if(!bExist)
				textList.push_back(*it);
			bFound = true;
		}
	}	
	return bFound;
}

int CEqmDrawPage::GetProjectID() const
{
	return m_projectid;
}

void CEqmDrawPage::SetProjectID( int projectid ) 
{
	m_projectid = projectid;
}

void CEqmDrawPage::ClearAll()
{
	ClearList(m_pipelist);
	ClearList(m_devicelist);
	ClearList(m_textlist);
	ClearList(m_graphiclist);
	ClearList(m_bendlist);
	ClearList(m_dashboardlist);
	ClearList(m_linechartlist);
	ClearList(m_barchartlist);
	ClearList(m_piechartlist);
	ClearList(m_buttonlist);
	ClearList(m_timeselectlist);
	ClearList(m_timebarlist);
	ClearList(m_diagnoselist);
	ClearList(m_polygonList);
	ClearList(m_areachangelist);
	ClearList(m_shellexecutelist);
	ClearList(m_embededpagelist);
	ClearList(m_liquidLevellist);
	ClearList(m_chkBtnPluslist);
	ClearList(m_scrawllist);
	ClearList(m_historyTrendlist);
	ClearList(m_energySaveRoilist);
	ClearList(m_generalComponentlist);
	ClearList(m_rectangleList);
	ClearList(m_straightLineList);

	m_osg_compare_bar_chart_list.clear();
	m_osg_bar_chart_list.clear();
	m_osg_line_chart_list.clear();
	m_osg_meter_pan_list.clear();
	m_osg_button_list.clear();
	m_osg_time_bar_list.clear();
	m_osg_diagnose_chart_list.clear();
	m_osg_diagnose_chart_list_plus.clear();
	m_osg_mouse_image_changer_list.clear();
	m_osg_shellexecute_list.clear();
	m_osg_mapnavigate_list.clear();
	m_osg_embededpage_list.clear();
	m_osg_button_plus_list.clear();
	m_osg_scrawl_list.clear();
	m_osg_history_trend_list.clear();
	m_bDetailLoaded = false;
}

list<CEqmDrawGraphic*>& CEqmDrawPage::Get2DGraphicList()
{
	return m_graphiclist;
}

void CEqmDrawPage::Add2DGraphic( CEqmDrawGraphic* graphic )
{
	m_graphiclist.push_back(graphic);
}

void CEqmDrawPage::Delete2DGraphic( CEqmDrawGraphic* graphic )
{
	assert(graphic != NULL);
	m_graphiclist.remove(graphic);
	delete graphic;
}

CEqmDrawGraphic* CEqmDrawPage::Get2DGraphic( int id )
{
	for (list<CEqmDrawGraphic*>::iterator it = m_graphiclist.begin();it != m_graphiclist.end(); it++)
	{
		if ((*it)->GetID() == id)
		{
			return  (*it);
		}
	}
	return NULL;
}

PAGETYPE CEqmDrawPage::GetPageType() const
{
	return m_type;
}

void CEqmDrawPage::SetPageType( PAGETYPE mtype )
{
	m_type = mtype;
	m_bNeedSave = true;
}

int	  CEqmDrawPage::GetPageWidth() const
{
	return m_width;
}
void	 CEqmDrawPage::SetPageWidth(int mwidth)
{
	m_width = mwidth;
	m_bNeedSave = true;
}

int	  CEqmDrawPage::GetPageHeight() const
{
	return m_height;
}

void	 CEqmDrawPage::SetPageHeight(int mheight)
{
	m_height = mheight;
	m_bNeedSave = true;
}

int	 CEqmDrawPage::GetPagexPosition() const
{
	return m_xposition;
}
void	CEqmDrawPage::SetPagexPosition(int mxposition)
{
	m_xposition = mxposition;
	m_bNeedSave = true;
}


int	 CEqmDrawPage::GetPageyPosition() const
{
	return m_yposition;
}
void	CEqmDrawPage::SetPageyPosition(int myposition)
{
	m_yposition = myposition;
	m_bNeedSave = true;
}

void CEqmDrawPage::SetGroupID( const int nGroupID )
{
	m_GroupID = nGroupID;
	m_bNeedSave = true;
}

const int CEqmDrawPage::GetGroupID()
{
	return m_GroupID;
}

void CEqmDrawPage::SetStartColor( const int nStartColor )
{
	m_nStartColor = nStartColor;
	m_bNeedSave = true;
}

const int CEqmDrawPage::GetStartColor()
{
	return m_nStartColor;
}

void CEqmDrawPage::SetEndColor( const int nEndColor )
{
	m_nEndColor = nEndColor;
	m_bNeedSave = true;
}

const int CEqmDrawPage::GetEndColor()
{
	return m_nEndColor;
}

void CEqmDrawPage::SetBKImageID( const int nBKImageID )
{
	m_nBKImageID = nBKImageID;
	m_bNeedSave = true;
}

const int CEqmDrawPage::GetBKImageID()
{
	return m_nBKImageID;
}

list<CEqmDrawBend*>& CEqmDrawPage::GetBendList()
{
	return m_bendlist;
}

list<CEqmDrawDashBoard*>& CEqmDrawPage::GetDashboardList()
{
	return m_dashboardlist;
}

list<CEqmDrawLineGraphic*>& CEqmDrawPage::GetLineGraphList()
{
	return m_linechartlist;
}

list<CEqmDrawBarGraphic*>& CEqmDrawPage::GetBarGraphList()
{
	return m_barchartlist;
}

list<CEqmDrawPieGraphic*>& CEqmDrawPage::GetPieGraphList()
{
	return m_piechartlist;
}

void CEqmDrawPage::AddBend( CEqmDrawBend* pBend )
{
	if (pBend)
	{
		m_bendlist.push_back(pBend);
	}
}

void CEqmDrawPage::AddDashboard( CEqmDrawDashBoard* pDashboard )
{
	if (pDashboard)
	{
		m_dashboardlist.push_back(pDashboard);
	}
}

void CEqmDrawPage::AddLineGraphic( CEqmDrawLineGraphic* pLineGraphic )
{
	if (pLineGraphic)
	{
		m_linechartlist.push_back(pLineGraphic);
	}
}

void CEqmDrawPage::AddBarGraphic( CEqmDrawBarGraphic* pBarGraphic )
{
	if (pBarGraphic)
	{
		m_barchartlist.push_back(pBarGraphic);
	}
}

void CEqmDrawPage::AddPieGraphic( CEqmDrawPieGraphic* pPieGraphic )
{
	if (pPieGraphic)
	{
		m_piechartlist.push_back(pPieGraphic);
	}
}

void CEqmDrawPage::DeleteBend( CEqmDrawBend* pBend )
{
	if (pBend)
	{
		m_bendlist.remove(pBend);
		delete pBend;
	}
}

void CEqmDrawPage::DeleteDashboard( CEqmDrawDashBoard* pDashboard )
{
	if (pDashboard)
	{
		m_dashboardlist.remove(pDashboard);
		delete pDashboard;
	}
}

void CEqmDrawPage::DeleteLineGraphic( CEqmDrawLineGraphic* pLineGraphic )
{
	if (pLineGraphic)
	{
		m_linechartlist.remove(pLineGraphic);
		delete pLineGraphic;
	}
}

void CEqmDrawPage::DeleteBarGraphic( CEqmDrawBarGraphic* pBarGraphic )
{
	if (pBarGraphic)
	{
		m_barchartlist.remove(pBarGraphic);
		delete pBarGraphic;
	}
}

void CEqmDrawPage::DeletePieGraphic( CEqmDrawPieGraphic* pPieGraphic )
{
	if (pPieGraphic)
	{
		m_piechartlist.remove(pPieGraphic);
		delete pPieGraphic;
	}
}

CEqmDrawBend* CEqmDrawPage::GetBend( const int nID )
{
	for (list<CEqmDrawBend*>::iterator it = m_bendlist.begin();it != m_bendlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

CEqmDrawDashBoard* CEqmDrawPage::GetDashBoard( const int nID )
{
	for (list<CEqmDrawDashBoard*>::iterator it = m_dashboardlist.begin();it != m_dashboardlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

CEqmDrawLineGraphic* CEqmDrawPage::GetLineGraphic( const int nID )
{
	for (list<CEqmDrawLineGraphic*>::iterator it = m_linechartlist.begin();it != m_linechartlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

CEqmDrawBarGraphic* CEqmDrawPage::GetBarGraphic( const int nID )
{
	for (list<CEqmDrawBarGraphic*>::iterator it = m_barchartlist.begin();it != m_barchartlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

CEqmDrawPieGraphic* CEqmDrawPage::GetPieGraphic( const int nID )
{
	for (list<CEqmDrawPieGraphic*>::iterator it = m_piechartlist.begin();it != m_piechartlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<COSGPlusCompareBar*>& CEqmDrawPage::GetOsgCompareBarChartList()
{
	return m_osg_compare_bar_chart_list;
}

void CEqmDrawPage::AddOsgCompareBarChart( COSGPlusCompareBar* pOsgBarChart )
{
	assert(pOsgBarChart);

	m_osg_compare_bar_chart_list.push_back(pOsgBarChart);
}

list<COSGPlusLineChart*>& CEqmDrawPage::GetOsgLineChartList()
{
	return m_osg_line_chart_list;
}

void CEqmDrawPage::AddOsgLineChart( COSGPlusLineChart* pOsgLineChart )
{
	assert(pOsgLineChart);

	m_osg_line_chart_list.push_back(pOsgLineChart);
}

void CEqmDrawPage::AddOsgMeterPan( COSGPlusMeterPan* pOsgMeterPan )
{
	m_osg_meter_pan_list.push_back(pOsgMeterPan);
}

list<COSGPlusMeterPan*>& CEqmDrawPage::GetOsgMeterPanList()
{
	return m_osg_meter_pan_list;
}

void CEqmDrawPage::AddButton( CEqmDrawButton* pButton )
{
	if (pButton)
	{
		m_buttonlist.push_back(pButton);
	}
}

list<CEqmDrawButton*>& CEqmDrawPage::GetButtonList()
{
	return m_buttonlist;
}

void CEqmDrawPage::DeleteButton( CEqmDrawButton* button )
{
	if (button)
	{
		m_buttonlist.remove(button);
		delete button;
	}
}

CEqmDrawButton* CEqmDrawPage::GetButton( const int nID )
{
	for (list<CEqmDrawButton*>::iterator it = m_buttonlist.begin();it != m_buttonlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

CEqmDrawPage* CEqmDrawPage::Copy()
{
	return new CEqmDrawPage(*this);
}

list<COSGPlusButton*>& CEqmDrawPage::GetOsgButtonList()
{
	return m_osg_button_list;
}

void CEqmDrawPage::AddOsgButton( COSGPlusButton* pButton )
{
	if (pButton)
	{
		m_osg_button_list.push_back(pButton);
	}
}

list<CEqmDrawTimeSelect*>& CEqmDrawPage::GetTimeSelectList()
{
	return m_timeselectlist;
}

void CEqmDrawPage::AddTimeSelect( CEqmDrawTimeSelect* pTimeSelect )
{
	if (pTimeSelect)
	{
		m_timeselectlist.push_back(pTimeSelect);
	}
}

void CEqmDrawPage::DeleteTimeSelect( CEqmDrawTimeSelect* timeselect )
{
	assert(timeselect != NULL);
	CEqmDrawTimeSelect* d = timeselect;
	m_timeselectlist.remove(timeselect);
	delete d;
}

CEqmDrawTimeSelect* CEqmDrawPage::GetTimeSelect( const int nID )
{
	for (list<CEqmDrawTimeSelect*>::iterator it = m_timeselectlist.begin();it != m_timeselectlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<CEqmDrawTimeBar*>& CEqmDrawPage::GetTimeBarList()
{
	return m_timebarlist;
}

void CEqmDrawPage::AddTimeBar( CEqmDrawTimeBar* pTimeBar )
{
	if (pTimeBar)
	{
		m_timebarlist.push_back(pTimeBar);
	}
}

void CEqmDrawPage::DeleteTimeBar( CEqmDrawTimeBar* timebar )
{
	assert(timebar != NULL);
	CEqmDrawTimeBar* d = timebar;
	m_timebarlist.remove(timebar);
	delete d;
}

CEqmDrawTimeBar* CEqmDrawPage::GetTimeBar( const int nID )
{
	for (list<CEqmDrawTimeBar*>::iterator it = m_timebarlist.begin();it != m_timebarlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<CEqmDrawProgressBar*>& CEqmDrawPage::GetProgressBarList()
{
	return m_progressbarlist;
}

void CEqmDrawPage::AddProgressBar( CEqmDrawProgressBar* pProgressBar )
{
	if (pProgressBar)
	{
		m_progressbarlist.push_back(pProgressBar);
	}
}

void CEqmDrawPage::DeleteProgressBar( CEqmDrawProgressBar* progressbar )
{
	assert(progressbar != NULL);
	CEqmDrawProgressBar* d = progressbar;
	m_progressbarlist.remove(progressbar);
	delete d;
}

CEqmDrawProgressBar* CEqmDrawPage::GetProgressBar( const int nID )
{
	for (list<CEqmDrawProgressBar*>::iterator it = m_progressbarlist.begin();it != m_progressbarlist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<COSGPlusBarChart*>& CEqmDrawPage::GetOsgBarChartList()
{
	return m_osg_bar_chart_list;
}

void CEqmDrawPage::AddOsgBarChart( COSGPlusBarChart* pOsgBarChart )
{
	assert(pOsgBarChart);

	m_osg_bar_chart_list.push_back(pOsgBarChart);
}

list<COSGPlusTimeBar*>& CEqmDrawPage::GetOsgTimeBarList()
{
	return m_osg_time_bar_list;
}

void CEqmDrawPage::AddOsgTimeBar( COSGPlusTimeBar* pTimeBar )
{
	m_osg_time_bar_list.push_back(pTimeBar);
}

void CEqmDrawPage::SetPageShowTopInObserver( const bool bShow )
{
	m_bShowTopInObserver = bShow;
	m_bNeedSave = true;
}

const bool CEqmDrawPage::GetPageShowTopInObserver()
{
	return m_bShowTopInObserver;
}

void CEqmDrawPage::SetBkImage(Image* pImage)
{
	if (pImage != NULL)
	{
		m_pBkImage = pImage;
		m_bNeedSave = true;
	}
}

Image* CEqmDrawPage::GetBkImage(void) const
{
	return m_pBkImage;
}

void CEqmDrawPage::SetWebAddress(const CString strWebAddr)
{
	m_strWedAddress = strWebAddr;
}

CString CEqmDrawPage::GetWebAddress(void) const
{
	return m_strWedAddress;
}

void CEqmDrawPage::SetPageShowOrder(const int nPageOrder)
{
	m_nPageOrder = nPageOrder;
	m_bNeedSave = true;
}

int CEqmDrawPage::GetPageShowOrder(void) const
{
	return m_nPageOrder;
}


void CEqmDrawPage::SetPageMinRight(const int nValue)
{
	m_nPageMinRight = nValue;
	m_bNeedSave = true;
}

int CEqmDrawPage::GetPageMinRight(void) const
{
	return m_nPageMinRight;
}


list<CEqmDrawDiagnoseGraphic*>& CEqmDrawPage::GetDiagnoseList()
{
	return m_diagnoselist;
}

void CEqmDrawPage::AddDiagnoseGraphic( CEqmDrawDiagnoseGraphic* pDiagnoseGraphic )
{
	if (pDiagnoseGraphic)
	{
		m_diagnoselist.push_back(pDiagnoseGraphic);
	}
}

void CEqmDrawPage::DeleteDiagnoseGraphic( CEqmDrawDiagnoseGraphic* pDiagnoseGraphic )
{
	assert(pDiagnoseGraphic != NULL);
	CEqmDrawDiagnoseGraphic* d = pDiagnoseGraphic;
	m_diagnoselist.remove(pDiagnoseGraphic);
	delete d;
}

CEqmDrawDiagnoseGraphic* CEqmDrawPage::GetDiagnoseGraphic( const int nID )
{
	for (list<CEqmDrawDiagnoseGraphic*>::iterator it = m_diagnoselist.begin();it != m_diagnoselist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<COSGPlusDiagnoseChart*>& CEqmDrawPage::GetOsgDiagnoseChartList()
{
	return m_osg_diagnose_chart_list;
}

void CEqmDrawPage::AddOsgDiagnoseChart( COSGPlusDiagnoseChart* pChart )
{
	if (pChart)
	{
		m_osg_diagnose_chart_list.push_back(pChart);
	}
}

list<CEqmDrawPolygon*>& CEqmDrawPage::GetPolygon()
{
	return m_polygonList;
}

list<COSGPlusDiagnoseChartPlus*>& CEqmDrawPage::GetOsgDiagnoseChartListPlus()
{
	return m_osg_diagnose_chart_list_plus;
}

void CEqmDrawPage::AddPolygon( CEqmDrawPolygon* pDiagnoseGraphic )
{
	if (pDiagnoseGraphic)
	{
		m_polygonList.push_back(pDiagnoseGraphic);
	}
}

void CEqmDrawPage::AddOsgDiagnoseChartPlus( COSGPlusDiagnoseChartPlus* pChart )
{
	if (pChart)
	{
		m_osg_diagnose_chart_list_plus.push_back(pChart);
	}
}

void CEqmDrawPage::DeletePolygon( CEqmDrawPolygon* pPolygon )
{
	assert(pPolygon != NULL);
	CEqmDrawPolygon* d = pPolygon;
	m_polygonList.remove(pPolygon);
	delete d;
}

CEqmDrawPolygon* CEqmDrawPage::GetPolygon( const int nID )
{
	for (list<CEqmDrawPolygon*>::iterator it = m_polygonList.begin();it != m_polygonList.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<CEqmDrawAreaChange*>& CEqmDrawPage::GetAreaChangeList()
{
	return m_areachangelist;
}

void CEqmDrawPage::AddAreaChange( CEqmDrawAreaChange* pAreaChange )
{
	if (pAreaChange)
	{
		m_areachangelist.push_back(pAreaChange);
	}
}

void CEqmDrawPage::DeleteAreaChange( CEqmDrawAreaChange* pAreaChange )
{
	assert(pAreaChange != NULL);
	CEqmDrawAreaChange* d = pAreaChange;
	m_areachangelist.remove(pAreaChange);
	delete d;
}

CEqmDrawAreaChange* CEqmDrawPage::GetAreaChange( const int nID )
{
	for (list<CEqmDrawAreaChange*>::iterator it = m_areachangelist.begin();it != m_areachangelist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

bool CEqmDrawPage::FindAreaChange( const int nPageID, const int nID )
{
	return true;
}

list<COSGPlusMouseImageChanger*>& CEqmDrawPage::GetMouseImageChangerList()
{
	return m_osg_mouse_image_changer_list;
}

void CEqmDrawPage::AddMouseImageChanger( COSGPlusMouseImageChanger* pMouseChage )
{
	m_osg_mouse_image_changer_list.push_back(pMouseChage);
}

bool CEqmDrawPage::FindOSGMouseImageChanger( const int nPageID, const int nID )
{
	return true;
}


bool CEqmDrawPage::FindDevice( wstring strPointName, vector<CEqmDrawDevice *> & deviceList )
{
	bool bFound = false;
	for (list<CEqmDrawDevice*>::iterator it = m_devicelist.begin();
		it != m_devicelist.end(); it++)
	{
		if ((*it)->GetPhysicalID() == strPointName)
		{
			//textList 不存在才添加
			bool bExist = false;
			for(int i=0;i<deviceList.size(); i++)
			{
				if((*it) == deviceList[i])
				{
					bExist = true;
					break;
				}
			}

			if(!bExist)
				deviceList.push_back(*it);
			bFound = true;
		}
	}	
	return bFound;
}

void CEqmDrawPage::AddShellExecute( CEqmDrawShellExecute* pShellExecute )
{
	if (pShellExecute)
	{
		m_shellexecutelist.push_back(pShellExecute);
	}
}

list<CEqmDrawShellExecute*>& CEqmDrawPage::GetShellExecuteList()
{
	return m_shellexecutelist;
}

void CEqmDrawPage::DeleteShellExecute( CEqmDrawShellExecute* pShellExecute )
{
	assert(pShellExecute != NULL);
	CEqmDrawShellExecute* d = pShellExecute;
	m_shellexecutelist.remove(pShellExecute);
	delete d;
}

CEqmDrawShellExecute* CEqmDrawPage::GetShellExecute( const int nID )
{
	for (list<CEqmDrawShellExecute*>::iterator it = m_shellexecutelist.begin();it != m_shellexecutelist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<COSGPlusShellExecute*>& CEqmDrawPage::GetOsgShellExecuteList()
{
	return m_osg_shellexecute_list;
}

void CEqmDrawPage::AddOsgShellExecute( COSGPlusShellExecute* pShellExecute )
{
	m_osg_shellexecute_list.push_back(pShellExecute);
}

list<CEqmDrawEmbededPage*>& CEqmDrawPage::GetEmbededPageList()
{
	return m_embededpagelist;
}

void CEqmDrawPage::AddEmbededPage( CEqmDrawEmbededPage* pEmbededPage )
{
	if (pEmbededPage)
	{
		m_embededpagelist.push_back(pEmbededPage);
	}
}

void CEqmDrawPage::DeleteEmbededPage( CEqmDrawEmbededPage* pEmbededPage )
{
	assert(pEmbededPage != NULL);
	CEqmDrawEmbededPage* d = pEmbededPage;
	m_embededpagelist.remove(pEmbededPage);
	delete d;
}

CEqmDrawEmbededPage* CEqmDrawPage::GetEmbededPage( const int nID )
{
	for (list<CEqmDrawEmbededPage*>::iterator it = m_embededpagelist.begin();it != m_embededpagelist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return  (*it);
		}
	}
	return NULL;
}

list<COSGPlusEmbededPage*>& CEqmDrawPage::GetOsgEmbededPageList()
{
	return m_osg_embededpage_list;
}

void CEqmDrawPage::AddOsgEmbededPage( COSGPlusEmbededPage* pEmbededPage )
{
	if (pEmbededPage)
	{
		m_osg_embededpage_list.push_back(pEmbededPage);
	}
}

list<CEqmDrawLiquidLevel*>& CEqmDrawPage::GetLiquidLevelList()
{
	return m_liquidLevellist;
}

void CEqmDrawPage::AddLiquidLevel(CEqmDrawLiquidLevel* pLiquidLevel)
{
	assert(pLiquidLevel != NULL);
	if (pLiquidLevel != NULL)
	{
		m_liquidLevellist.push_back(pLiquidLevel);
	}
}

void CEqmDrawPage::DeleteLiquidLevel(CEqmDrawLiquidLevel* pLiquidLevel)
{
	assert(pLiquidLevel != NULL);
	if (pLiquidLevel != NULL)
	{
		CEqmDrawLiquidLevel* d = pLiquidLevel;
		m_liquidLevellist.remove(pLiquidLevel);
		delete d;
	}
}

CEqmDrawLiquidLevel* CEqmDrawPage::GetLiquidLevel(const int nID)
{
	for (list<CEqmDrawLiquidLevel*>::iterator it=m_liquidLevellist.begin();it!=m_liquidLevellist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}
	return NULL;
}

list<CEqmDrawButtonPlus*>& CEqmDrawPage::GetCheckButtonPlusList()
{
	return m_chkBtnPluslist;
}

void CEqmDrawPage::AddCheckButtonPlus(CEqmDrawButtonPlus* pChkBtnPlus)
{
	assert(pChkBtnPlus != NULL);
	if (pChkBtnPlus != NULL)
	{
		m_chkBtnPluslist.push_back(pChkBtnPlus);
	}
}

void CEqmDrawPage::DeleteCheckButtonPlus(CEqmDrawButtonPlus* pChkBtnPlus)
{
	assert(pChkBtnPlus != NULL);
	if (pChkBtnPlus != NULL)
	{
		CEqmDrawButtonPlus* d = pChkBtnPlus;
		m_chkBtnPluslist.remove(pChkBtnPlus);
		delete d;
		d = NULL;
	}
}

CEqmDrawButtonPlus* CEqmDrawPage::GetCheckButtonPlus(const int nID)
{
	for (list<CEqmDrawButtonPlus*>::iterator it=m_chkBtnPluslist.begin(); it!=m_chkBtnPluslist.end(); it++)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}
	return NULL;
}

list<COSGPlusButtonPlus*>& CEqmDrawPage::GetOsgButtonPlusList()
{
	return m_osg_button_plus_list;
}

void CEqmDrawPage::AddOsgCheckButtonPlus( COSGPlusButtonPlus* pChkBtnPlus )
{
	if (pChkBtnPlus)
	{
		m_osg_button_plus_list.push_back(pChkBtnPlus);
	}
}


// notice board
list<CEqmDrawScrawl*>& CEqmDrawPage::GetScrawlList()
{
	return m_scrawllist;
}

void CEqmDrawPage::AddScrawlBoard(CEqmDrawScrawl* pScrawl)
{
	assert(pScrawl != NULL);
	if (pScrawl != NULL)
	{
		m_scrawllist.push_back(pScrawl);
	}
}

void CEqmDrawPage::DeleteScrawlBoard(CEqmDrawScrawl* pScrawl)
{
	assert(pScrawl != NULL);
	if (pScrawl != NULL)
	{
		CEqmDrawScrawl* d = pScrawl;
		m_scrawllist.remove(pScrawl);
		delete d;
		d = NULL;
	}
}

CEqmDrawScrawl* CEqmDrawPage::GetScrawl(const int nID)
{
	for (list<CEqmDrawScrawl*>::iterator it=m_scrawllist.begin(); it!=m_scrawllist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}
	return NULL;
}

list<COSGPlusScrawl*>& CEqmDrawPage::GetOsgScrawlList()
{
	return m_osg_scrawl_list;
}

void CEqmDrawPage::AddOsgScrawl(COSGPlusScrawl* pScrawl)
{
	if (pScrawl != NULL)
	{
		m_osg_scrawl_list.push_back(pScrawl);
	}
}

// HistoryTrend
list<CEqmDrawHistoryTrend*>& CEqmDrawPage::GetHistoryTrendList()
{
	return m_historyTrendlist;
}

void CEqmDrawPage::AddHistoryTrend(CEqmDrawHistoryTrend* pHistTrend)
{
	assert(pHistTrend != NULL);
	if (pHistTrend != NULL)
	{
		m_historyTrendlist.push_back(pHistTrend);
	}
}

void CEqmDrawPage::DeleteHistoryTrend(CEqmDrawHistoryTrend* pHistTrend)
{
	assert(pHistTrend != NULL);
	if (pHistTrend != NULL)
	{
		CEqmDrawHistoryTrend* d = pHistTrend;
		m_historyTrendlist.remove(pHistTrend);
		delete d;
		d = NULL;
	}
}

CEqmDrawHistoryTrend* CEqmDrawPage::GetHistoryTrend(const int nID)
{
	for (list<CEqmDrawHistoryTrend*>::iterator it=m_historyTrendlist.begin(); it!=m_historyTrendlist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

list<COSGPlusCurve*>& CEqmDrawPage::GetOsgHistoryTrendList()
{
	return m_osg_history_trend_list;
}

void CEqmDrawPage::AddOsgHistoryTrend(COSGPlusCurve* pHistoryTrend)
{
	if (pHistoryTrend != NULL)
	{
		m_osg_history_trend_list.push_back(pHistoryTrend);
	}
}

// EnergySaveRoi
list<CEqmDrawEnergySaveRoi*>& CEqmDrawPage::GetEnergySaveRoiList()
{
	return m_energySaveRoilist;
}

void CEqmDrawPage::AddEnergySaveRoi(CEqmDrawEnergySaveRoi* pEnergySaveRoi)
{
	assert(pEnergySaveRoi != NULL);
	if (pEnergySaveRoi != NULL)
	{
		m_energySaveRoilist.push_back(pEnergySaveRoi);
	}
}

void CEqmDrawPage::DeleteEnergySaveRoi(CEqmDrawEnergySaveRoi* pEnergySaveRoi)
{
	assert(pEnergySaveRoi != NULL);
	if (pEnergySaveRoi != NULL)
	{
		CEqmDrawEnergySaveRoi* d = pEnergySaveRoi;
		m_energySaveRoilist.remove(pEnergySaveRoi);
		delete d;
		d = NULL;
	}
}

CEqmDrawEnergySaveRoi* CEqmDrawPage::GetEnergySaveRoi(const int nID)
{
	for (list<CEqmDrawEnergySaveRoi*>::iterator it=m_energySaveRoilist.begin(); it!=m_energySaveRoilist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

list<COSGPlusROIChart*>& CEqmDrawPage::GetOSGROIChartList()
{
	return m_osg_roi_list;
}

void CEqmDrawPage::AddROIChart(COSGPlusROIChart* pChart)
{
	m_osg_roi_list.push_back(pChart);
}

// map navigate
list<CEqmDrawMapNavigate*>& CEqmDrawPage::GetMapNavigateList()
{
	return m_mapNavigatelist;
}

void CEqmDrawPage::AddMapNavigate(CEqmDrawMapNavigate* pMapNavigate)
{
	assert(pMapNavigate != NULL);
	if (pMapNavigate != NULL)
	{
		m_mapNavigatelist.push_back(pMapNavigate);
	}
}

void CEqmDrawPage::DeleteMapNavigate(CEqmDrawMapNavigate* pMapNavigate)
{
	assert(pMapNavigate != NULL);
	if (pMapNavigate != NULL)
	{
		CEqmDrawMapNavigate* d = pMapNavigate;
		m_mapNavigatelist.remove(pMapNavigate);
		delete d;
		d = NULL;
	}
}

CEqmDrawMapNavigate* CEqmDrawPage::GetMapNavigate(const int nID)
{
	for (list<CEqmDrawMapNavigate*>::iterator it=m_mapNavigatelist.begin(); it!=m_mapNavigatelist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

list<COSGPlusMapNavigate*>& CEqmDrawPage::GetOsgMapNavigateList()
{
	return m_osg_mapnavigate_list;
}

// navigate window
list<CEqmDrawNavigateWindow*>& CEqmDrawPage::GetNavigateWindowList()
{
	return m_navigateWindowlist;
}

void CEqmDrawPage::AddNavigateWindow(CEqmDrawNavigateWindow* pNavigateWindow)
{
	assert(pNavigateWindow != NULL);
	if (pNavigateWindow != NULL)
	{
		m_navigateWindowlist.push_back(pNavigateWindow);
	}
}

void CEqmDrawPage::DeleteNavigateWindow(CEqmDrawNavigateWindow* pNavigateWindow)
{
	assert(pNavigateWindow != NULL);
	if (pNavigateWindow != NULL)
	{
		CEqmDrawNavigateWindow* d = pNavigateWindow;
		m_navigateWindowlist.remove(pNavigateWindow);
		delete d;
		d = NULL;
	}
}

CEqmDrawNavigateWindow* CEqmDrawPage::GetNavigateWindow(const int nID)
{
	for (list<CEqmDrawNavigateWindow*>::iterator it=m_navigateWindowlist.begin(); it!=m_navigateWindowlist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

// level ruler
list<CEqmDrawLevelRuler*>& CEqmDrawPage::GetLevelRulerList()
{
	return m_levelRulerlist;
}

void CEqmDrawPage::AddLevelRuler(CEqmDrawLevelRuler* pLevelRuler)
{
	assert(pLevelRuler != NULL);
	if (pLevelRuler != NULL)
	{
		m_levelRulerlist.push_back(pLevelRuler);
	}
}

void CEqmDrawPage::DeleteLevelRuler(CEqmDrawLevelRuler* pLevelRuler)
{
	assert(pLevelRuler != NULL);
	if (pLevelRuler != NULL)
	{
		CEqmDrawLevelRuler* d = pLevelRuler;
		m_levelRulerlist.remove(pLevelRuler);
		delete d;
		d = NULL;
	}
}

CEqmDrawLevelRuler* CEqmDrawPage::GetLevelRuler(const int nID)
{
	for (list<CEqmDrawLevelRuler*>::iterator it=m_levelRulerlist.begin(); it!=m_levelRulerlist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

CEqmDrawBase* CEqmDrawPage::GetElementBase(const EQMDRAWTYPE type, int id )
{
	switch(type)
	{
	case EQMDRAW_CHILLER:
	case EQMDRAW_PUMP:
	case EQMDRAW_COOLINGTOWER:
	case EQMDRAW_VALVE:
	case EQMDRAW_METAFILE:
		{
			return GetDevice(id);
		}
		break;
	case EQMDRAW_TEXT:
		{
			return GetText(id);
		}
		break;
	case EQMDRAW_STRAIGHTPIPE:
	case EQMDRAW_FOLDPIPE:
		{
			return GetPipe(id);
		}
		break;
	case GraphicType_LINE:
	case GraphicType_RECTANGLE:
	case GraphicType_ELLIPSE:
		{
			return Get2DGraphic(id);
		}
		break;
	case EQMDRAW_METABUTTON:
		{
			return GetButton(id);
		}
		break;
	case EQMDRAW_LINE_CHART:
		{
			return GetLineGraphic(id);
		}
		break;
	case EQMDRAW_BAR_CHART:
		{
			return GetBarGraphic(id);
		}
		break;
	case EQMDRAW_PIE_CHART:
		{
			return GetPieGraphic(id);
		}
		break;
	case EQMDRAW_DIAGNOSE_CHART:
		{
			return GetDiagnoseGraphic(id);
		}
		break;
	case EQMDRAW_POLYGON:
		{
			return GetPolygon(id);
		}
		break;
	case EQMDRAW_DASHBOARD:
		{
			return GetDashBoard(id);
		}
		break;
	case EQMDRAW_BEND:
		{
			return GetBend(id);
		}
		break;
	case EQMDRAW_TIME_SELECT:
		{
			return GetTimeSelect(id);
		}
		break;
	case EQMDRAW_TIME_BAR:
		{
			return GetTimeBar(id);
		}
		break;
	case EQMDRAW_PROGRESS_BAR:
		{
			return GetProgressBar(id);
		}
		break;
	case EQMDRAW_AREA_CHANGE:
		{
			return GetAreaChange(id);
		}
		break;
	case EQMDRAW_SHELL_EXECUTE:
		{
			return GetShellExecute(id);
		}
		break;
	case EQMDRAW_EMBEDED_PAGE:
		{
			return GetEmbededPage(id);
		}
		break;
	case EQMDRAW_LIQUID_LEVEL:
		{
			return GetLiquidLevel(id);
		}
		break;
	case EQMDRAW_CHKBTN_PLUS:
		{
			return GetCheckButtonPlus(id);
		}
		break;
	case EQMDRAW_LINE:
		{
			return GetStraightLine(id);
		}
		break;
	case EQMDRAW_HISTORY_TREND:
		{
			return GetHistoryTrend(id);
		}
		break;
	/*case EQMDRAW_ENERGY_SAVE_ROI:*/
	case EQMDRAW_RECTANGLE:
		{
			return GetRectangle(id);
		}
		break;
	case EQMDRAW_MAP_NAVIGATE:
		{
			return GetMapNavigate(id);
		}
		break;
	case EQMDRAW_NAVIGATE_WINDOW:
		{
			return GetNavigateWindow(id);
		}
		break;
	case EQMDRAW_LEVEL_RULER:
		{
			return GetLevelRuler(id);
		}
		break;
	case EQMDRAW_DATA_ANALYSE:
		{
			return GetDataAnalyse(id);
		}
		break;
	case EQMDRAW_TEMPER_DISTRI:
		{
			return GetTemperDistri(id);
		}
		break;
	case EQMDRAW_DATA_REPORT:
		{
			return GetDataReport(id);
		}
		break;
	case EQMDRAW_PLANE_TEMPE_DISTR:
		{
			return GetPlaneTempeDistr(id);
		}
		break;
	case EQMDRAW_DOTLINE_FRAME:
		{
			return GetDottedLineFrm(id);
		}
		break;
	case EQMDRAW_GENERAL_COMPONENT:
		{
			return GetGeneralComponent(id);
		}
		break;
	default:
		break;
	}
	return NULL;
}

// data analyse
list<CEqmDrawDataAnalyse*>& CEqmDrawPage::GetDataAnalyseList()
{
	return m_dataAnalyselist;
}

void CEqmDrawPage::AddDataAnalyse(CEqmDrawDataAnalyse* pDataAnalyse)
{
	assert(pDataAnalyse != NULL);
	if (pDataAnalyse != NULL)
	{
		m_dataAnalyselist.push_back(pDataAnalyse);
	}
}

void CEqmDrawPage::DeleteDataAnalyse(CEqmDrawDataAnalyse* pDataAnalyse)
{
	assert(pDataAnalyse != NULL);
	if (pDataAnalyse != NULL)
	{
		CEqmDrawDataAnalyse* d = pDataAnalyse;
		m_dataAnalyselist.remove(pDataAnalyse);
		delete d;
		d = NULL;
	}
}

CEqmDrawDataAnalyse* CEqmDrawPage::GetDataAnalyse(const int nID)
{
	for (list<CEqmDrawDataAnalyse*>::iterator it=m_dataAnalyselist.begin(); it!=m_dataAnalyselist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

// Temperature Distribution
list<CEqmDrawTemperDistri*>& CEqmDrawPage::GetTemperDistriList()
{
	return m_temperDistrilist;
}

void CEqmDrawPage::AddTemperDistri(CEqmDrawTemperDistri* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		m_temperDistrilist.push_back(p);
	}
}

void CEqmDrawPage::DeleteTemperDistri(CEqmDrawTemperDistri* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		CEqmDrawTemperDistri* d = p;
		m_temperDistrilist.remove(p);
		delete d;
		d = NULL;
	}
}

CEqmDrawTemperDistri* CEqmDrawPage::GetTemperDistri(const int nID)
{
	for (list<CEqmDrawTemperDistri*>::iterator it=m_temperDistrilist.begin(); it!=m_temperDistrilist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

// Data report
list<CEqmDrawDataReport*>& CEqmDrawPage::GetDataReportList()
{
	return m_dataReportlist;
}

void CEqmDrawPage::AddDataReport(CEqmDrawDataReport* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		m_dataReportlist.push_back(p);
	}
}

void CEqmDrawPage::DeleteDataReport(CEqmDrawDataReport* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		CEqmDrawDataReport* d = p;
		m_dataReportlist.remove(p);
		delete d;
		d = NULL;
	}
}

CEqmDrawDataReport* CEqmDrawPage::GetDataReport(const int nID)
{
	for (list<CEqmDrawDataReport*>::iterator it=m_dataReportlist.begin(); it!=m_dataReportlist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

// Plane Temperature Distribution
list<CEqmDrawPlaneTempeDistr*>& CEqmDrawPage::GetPlaneTempeDistrList()
{
	return m_planeTempeDistrlist;
}

void CEqmDrawPage::AddPlaneTempeDistr(CEqmDrawPlaneTempeDistr* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		m_planeTempeDistrlist.push_back(p);
	}
}

void CEqmDrawPage::DeletePlaneTempeDistr(CEqmDrawPlaneTempeDistr* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		CEqmDrawPlaneTempeDistr* d = p;
		m_planeTempeDistrlist.remove(p);
		delete d;
		d = NULL;
	}
}

CEqmDrawPlaneTempeDistr* CEqmDrawPage::GetPlaneTempeDistr(const int nID)
{
	for (list<CEqmDrawPlaneTempeDistr*>::iterator it=m_planeTempeDistrlist.begin(); it!=m_planeTempeDistrlist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

// Dotted Line Frame
list<CEqmDrawDottedLineFrm*>& CEqmDrawPage::GetDottedLineFrmList()
{
	return m_dottedLineFrmlist;
}

void CEqmDrawPage::AddDottedLineFrm(CEqmDrawDottedLineFrm* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		m_dottedLineFrmlist.push_back(p);
	}
}

void CEqmDrawPage::DeleteDottedLineFrm(CEqmDrawDottedLineFrm* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		CEqmDrawDottedLineFrm* d = p;
		m_dottedLineFrmlist.remove(p);
		delete d;
		d = NULL;
	}
}

CEqmDrawDottedLineFrm* CEqmDrawPage::GetDottedLineFrm(const int nID)
{
	for (list<CEqmDrawDottedLineFrm*>::iterator it=m_dottedLineFrmlist.begin(); it!=m_dottedLineFrmlist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

// General Component
list<CEqmDrawGeneralComponent*>& CEqmDrawPage::GetGeneralComponentList()
{
	return m_generalComponentlist;
}

list<CEqmDrawRectangle*>& CEqmDrawPage::GetRectangleList()
{
	return m_rectangleList;
}

list<CEqmDrawStraightLine*>& CEqmDrawPage::GetStraightLineList()
{
	return m_straightLineList;
}

void CEqmDrawPage::AddGeneralComponent(CEqmDrawGeneralComponent* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		m_generalComponentlist.push_back(p);
	}
}

void CEqmDrawPage::AddRectangle(CEqmDrawRectangle* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		m_rectangleList.push_back(p);
	}
}

void CEqmDrawPage::AddStraightLine(CEqmDrawStraightLine *p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		m_straightLineList.push_back(p);
	}
}

void CEqmDrawPage::DeleteGeneralComponent(CEqmDrawGeneralComponent* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		CEqmDrawGeneralComponent* d = p;
		m_generalComponentlist.remove(p);
		delete d;
		d = NULL;
	}
}

void CEqmDrawPage::DeleteRectangle(CEqmDrawRectangle* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		CEqmDrawRectangle* d = p;
		m_rectangleList.remove(p);
		delete d;
		d = NULL;
	}
}

void CEqmDrawPage::DeleteStraightLine(CEqmDrawStraightLine* p)
{
	assert(p != NULL);
	if (p != NULL)
	{
		CEqmDrawStraightLine* d = p;
		m_straightLineList.remove(p);
		delete d;
		d = NULL;
	}
}

CEqmDrawGeneralComponent* CEqmDrawPage::GetGeneralComponent(const int nID)
{
	for (list<CEqmDrawGeneralComponent*>::iterator it=m_generalComponentlist.begin(); it!=m_generalComponentlist.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}
CEqmDrawRectangle* CEqmDrawPage::GetRectangle(const int nID)
{
	for (list<CEqmDrawRectangle*>::iterator it=m_rectangleList.begin(); it!=m_rectangleList.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

CEqmDrawStraightLine* CEqmDrawPage::GetStraightLine(const int nID)
{
	for (list<CEqmDrawStraightLine*>::iterator it=m_straightLineList.begin(); it!=m_straightLineList.end(); ++it)
	{
		if ((*it)->GetID() == nID)
		{
			return (*it);
		}
	}

	return NULL;
}

