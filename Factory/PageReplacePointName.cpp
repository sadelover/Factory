// PageReplacePointName.cpp : implementation file
//

#include "stdafx.h"
#include "PageReplacePointName.h"
#include "afxdialogex.h"

#include "../ComponentDraw/CEqmDrawPage.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "../ComponentDraw/EqmDrawPipe.h"
#include "../ComponentDraw/EqmDrawText.h"
#include "../ComponentDraw/EqmDrawGraphic.h"
#include "../ComponentDraw/EqmDrawGraphicLine.h"
#include "../ComponentDraw/EqmDrawGraphicEllipse.h"
#include "../ComponentDraw/EqmDrawGraphicRect.h"
#include "../ComponentDraw/EqmDrawGraphicPolygon.h"
#include "../ComponentDraw/EqmDrawBend.h"
#include "../ComponentDraw/EqmDrawDashBoard.h"
#include "../ComponentDraw/EqmDrawLineGraphic.h"
#include "../ComponentDraw/EqmDrawBarGraphic.h"
#include "../ComponentDraw/EqmDrawPieGraphic.h"
#include "../ComponentDraw/EqmDrawButton.h"
#include "../ComponentDraw/EqmDrawTimeSelect.h"
#include "../ComponentDraw/EqmDrawTimeBar.h"
#include "../ComponentDraw/EqmDrawProgressBar.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphic.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphicPlus.h"
#include "../ComponentDraw/EqmDrawShellExecute.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "../ComponentDraw/EqmDrawLiquidLevel.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../ComponentDraw/EqmDrawScrawl.h"
#include "../ComponentDraw/EqmDrawHistoryTrend.h"
#include "../ComponentDraw/EqmDrawEnergySaveRoi.h"
#include "../ComponentDraw/EqmDrawMapNavigate.h"
// CPageReplacePointName dialog

IMPLEMENT_DYNAMIC(CPageReplacePointName, CDialog)

CPageReplacePointName::CPageReplacePointName(CEqmDrawPage *pPage,CWnd* pParent /*=NULL*/)
	: CDialog(CPageReplacePointName::IDD, pParent)
	, m_strSourcePointName(_T(""))
	, m_strReplacePointName(_T(""))
	, m_pPage(pPage)
{

}

CPageReplacePointName::~CPageReplacePointName()
{
}

void CPageReplacePointName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REPLACE_FROM, m_strSourcePointName);
	DDX_Text(pDX, IDC_EDIT_REPLACE_TO, m_strReplacePointName);
}


BEGIN_MESSAGE_MAP(CPageReplacePointName, CDialog)
	ON_BN_CLICKED(IDOK, &CPageReplacePointName::OnBnClickedOk)
END_MESSAGE_MAP()


// CPageReplacePointName message handlers


void CPageReplacePointName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_pPage && m_strSourcePointName.GetLength()>0)
	{
		int nReplaceCount = 0;
		//替换
		wstring strSourcePointName = m_strSourcePointName.GetString();
		wstring strReplacePointName = m_strReplacePointName.GetString();
		wstring strNewPointName;
		bool bReplace = false;
		{
			list<CEqmDrawDevice*>& deviceList = m_pPage->GetDrawDeviceList();
			list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
			while (iter != deviceList.end())
			{
				strNewPointName = Replace((*iter)->GetPhysicalID(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetPhysicalID(strNewPointName);
					nReplaceCount++;
				}

				strNewPointName = Replace((*iter)->GetEventInfo().strSettingPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->GetEventInfo().strSettingPointName = strNewPointName.c_str();
					nReplaceCount++;
				}
				iter++;
			}
		}
		{
			list<CEqmDrawPipe*>& deviceList = m_pPage->GetPagePipeList();
			list<CEqmDrawPipe*>::iterator iter = deviceList.begin();
			while (iter != deviceList.end())
			{
				vector<CString> vec = (*iter)->GetDataPointList();
				for(size_t i=0; i<vec.size(); i++)
				{
					strNewPointName = Replace(vec[i].GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i] = strNewPointName.c_str();
						nReplaceCount++;
					}
				}
				(*iter)->SetDataPointList(vec);
				iter++;
			}
		}
		{
			list<CEqmDrawText*>& textList = m_pPage->GetPageTextList();
			list<CEqmDrawText*>::iterator iter = textList.begin();
			while (iter != textList.end())
			{
				strNewPointName = Replace((*iter)->GetPhysicalID(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetPhysicalID(strNewPointName);
					nReplaceCount++;
				}

				strNewPointName = Replace((*iter)->GetDateTimePoint().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetDateTimePoint(strNewPointName.c_str());
					nReplaceCount++;
				}

				strNewPointName = Replace((*iter)->GetDeviceCfg().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetDeviceCfg(strNewPointName.c_str());
					nReplaceCount++;
				}

				strNewPointName = Replace((*iter)->GetTemperCfg().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetTemperCfg(strNewPointName.c_str());
					nReplaceCount++;
				}

				iter++;
			}
		}
		{
			list<CEqmDrawDashBoard*>& dashboardList = m_pPage->GetDashboardList();
			list<CEqmDrawDashBoard*>::iterator iter = dashboardList.begin();
			while (iter != dashboardList.end())
			{	
				strNewPointName = Replace((*iter)->GetBindPointName().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPointName(strNewPointName.c_str());
					nReplaceCount++;
				}
				iter++;				
			}
		}
		{
			list<CEqmDrawLineGraphic*>& linechartList = m_pPage->GetLineGraphList();
			list<CEqmDrawLineGraphic*>::iterator iter = linechartList.begin();
			while (iter != linechartList.end())
			{				
				vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
				for (size_t i=0;i<vec.size();++i)
				{
					strNewPointName = Replace(vec[i].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i].strPointName = strNewPointName.c_str();
						nReplaceCount++;
					}
				}
				iter++;			
			}
		}
		{
			list<CEqmDrawBarGraphic*>& barchartList = m_pPage->GetBarGraphList();
			list<CEqmDrawBarGraphic*>::iterator iter = barchartList.begin();
			while (iter != barchartList.end())
			{
				vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
				for (size_t i=0;i<vec.size();++i)
				{
					strNewPointName = Replace(vec[i].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i].strPointName = strNewPointName.c_str();
						nReplaceCount++;
					}
				}
				iter++;		
			}
		}
		{
			list<CEqmDrawPieGraphic*>& piechartList = m_pPage->GetPieGraphList();
			list<CEqmDrawPieGraphic*>::iterator iter = piechartList.begin();
			while (iter != piechartList.end())
			{
				vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
				for (size_t i=0;i<vec.size();++i)
				{
					strNewPointName = Replace(vec[i].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i].strPointName = strNewPointName.c_str();
						nReplaceCount++;
					}
				}
				iter++;		
			}
		}
		{
			list<CEqmDrawButton*>& buttonList = m_pPage->GetButtonList();
			list<CEqmDrawButton*>::iterator iter = buttonList.begin();
			while (iter != buttonList.end())
			{
				strNewPointName = Replace((*iter)->GetBindSetPointName().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindSetPointName(strNewPointName.c_str());
					nReplaceCount++;
				}

				vector<_tagRelationExpress>& vec = (*iter)->GetRelationVec();
				for(size_t i=0; i<vec.size(); i++)
				{
					strNewPointName = Replace(vec[i].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i].strPointName = strNewPointName.c_str();
						(*iter)->SetNeedSave(true);
						nReplaceCount++;
					}
				}
				iter++;			
			}
		}
		{
			list<CEqmDrawTimeBar*>& timeBarList = m_pPage->GetTimeBarList();
			list<CEqmDrawTimeBar*>::iterator iter = timeBarList.begin();
			while (iter != timeBarList.end())
			{
				strNewPointName = Replace((*iter)->GetBindPointStart().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPointStart(strNewPointName.c_str());
					nReplaceCount++;
				}

				strNewPointName = Replace((*iter)->GetBindPointStatus().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPointStatus(strNewPointName.c_str());
					nReplaceCount++;
				}				
				iter++;			
			}
		}
		{
			list<CEqmDrawTimeSelect*>& timeSelectList = m_pPage->GetTimeSelectList();
			list<CEqmDrawTimeSelect*>::iterator iter = timeSelectList.begin();
			while (iter != timeSelectList.end())
			{
				strNewPointName = Replace((*iter)->GetPointName().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetPointName(strNewPointName.c_str());
					nReplaceCount++;
				}					
				iter++;			
			}
		}
		{
			list<CEqmDrawProgressBar*>& ProgressBarList = m_pPage->GetProgressBarList();
			list<CEqmDrawProgressBar*>::iterator iter = ProgressBarList.begin();
			while (iter != ProgressBarList.end())
			{
				strNewPointName = Replace((*iter)->GetBindPoint().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPoint(strNewPointName.c_str());
					nReplaceCount++;
				}	

				strNewPointName = Replace((*iter)->GetBindPointBase().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPointBase(strNewPointName.c_str());
					nReplaceCount++;
				}	
				iter++;			
			}
		}
		{
			list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = m_pPage->GetDiagnoseList();
			list<CEqmDrawDiagnoseGraphic*>::iterator iter = DiagnoseGraphicList.begin();
			while (iter != DiagnoseGraphicList.end())
			{			
				vector<_tagDiagnoseItem>& vec = (*iter)->GetItemVec();
				for (size_t i=0;i<vec.size();++i)
				{
					strNewPointName = Replace(vec[i].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i].strPointName = strNewPointName.c_str();
						nReplaceCount++;
					}

					vector<_tagDiagnoseItemInLine>& vecItem = vec[i].vecItemInLine;
					for(size_t j=0; j<vecItem.size(); j++)
					{
						strNewPointName = Replace(vecItem[j].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
						if(bReplace)
						{
							vecItem[j].strPointName = strNewPointName.c_str();
							nReplaceCount++;
						}
					}
				}
				iter++;			
			}
		}
		{//多边形
			list<CEqmDrawPolygon*>& DiagnoseGraphicListPlus = m_pPage->GetPolygon();
			list<CEqmDrawPolygon*>::iterator iter = DiagnoseGraphicListPlus.begin();
			while (iter != DiagnoseGraphicListPlus.end())
			{			
				/*				vector<_tagBoolBindInfo>& vec = (*iter)->GetBoolBindInfoVec();
				for (size_t i=0;i<vec.size();++i)
				{
				strNewPointName = Replace(vec[i].strBoolPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
				vec[i].strBoolPointName = strNewPointName.c_str();
				nReplaceCount++;
				}

				strNewPointName = Replace(vec[i].strResultPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
				vec[i].strResultPointName = strNewPointName.c_str();
				nReplaceCount++;
				}

				strNewPointName = Replace(vec[i].strResultPointName2.GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
				vec[i].strResultPointName2 = strNewPointName.c_str();
				nReplaceCount++;
				}

				strNewPointName = Replace(vec[i].strResultPointName3.GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
				vec[i].strResultPointName3 = strNewPointName.c_str();
				nReplaceCount++;
				}

				strNewPointName = Replace(vec[i].strTreatmentStatePointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
				vec[i].strTreatmentStatePointName = strNewPointName.c_str();
				nReplaceCount++;
				}

				vector<_tagDiagnoseItemPlus>& vecItem = vec[i].vecDiagnoseItemPlus;
				for(size_t j=0; j<vecItem.size(); j++)
				{

				strNewPointName = Replace(vecItem[j].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
				vecItem[j].strPointName = strNewPointName.c_str();
				nReplaceCount++;
				}
				}
				}
				iter++;	*/		
			}
		}
		{//液位
			list<CEqmDrawLiquidLevel*>& liquidLevelList = m_pPage->GetLiquidLevelList();
			list<CEqmDrawLiquidLevel*>::iterator iter = liquidLevelList.begin();
			while (iter != liquidLevelList.end())
			{
				strNewPointName = Replace((*iter)->GetBindPoint().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPoint(strNewPointName.c_str());
					nReplaceCount++;
				}	
				iter++;			
			}
		}
		{//选择按钮
			list<CEqmDrawButtonPlus*>& chkBtnPlusList = m_pPage->GetCheckButtonPlusList();
			list<CEqmDrawButtonPlus*>::iterator iter = chkBtnPlusList.begin();
			while (iter != chkBtnPlusList.end())
			{
				strNewPointName = Replace((*iter)->GetBindPointName().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPointName(strNewPointName.c_str());
					nReplaceCount++;
				}	
				iter++;			
			}
		}
		{//便签
			list<CEqmDrawScrawl*>& scrawlList = m_pPage->GetScrawlList();
			list<CEqmDrawScrawl*>::iterator iter = scrawlList.begin();
			while (iter != scrawlList.end())
			{
				vector<CurveColumn>	vec = (*iter)->GetCurveColumn();
				for(int i=0; i<vec.size(); ++i)
				{					
					strNewPointName = Replace(vec[i].strPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i].strPointName = strNewPointName.c_str();
						nReplaceCount++;
					}
				}
				(*iter)->SetCurveColumn(vec);
				iter++;			
			}
		}
		{//曲线图元
			list<CEqmDrawHistoryTrend*>& historyTrendList = m_pPage->GetHistoryTrendList();
			list<CEqmDrawHistoryTrend*>::iterator iter = historyTrendList.begin();
			while (iter != historyTrendList.end())
			{
				vector<HistoryTrendInfo> vec = (*iter)->GetHistoryTrendVector();
				for(int i=0; i<vec.size(); ++i)
				{
					strNewPointName = Replace(vec[i].strBindPointName.GetString(),strSourcePointName,strReplacePointName,bReplace);
					if(bReplace)
					{
						vec[i].strBindPointName = strNewPointName.c_str();
						nReplaceCount++;
					}
				}
				(*iter)->SetHistoryTrendVector(vec);
				iter++;			
			}
		}
		{//ROI
			list<CEqmDrawEnergySaveRoi*>& energySaveRoiList = m_pPage->GetEnergySaveRoiList();
			list<CEqmDrawEnergySaveRoi*>::iterator iter = energySaveRoiList.begin();
			while (iter != energySaveRoiList.end())
			{
				strNewPointName = Replace((*iter)->GetBindPointName().GetString(),strSourcePointName,strReplacePointName,bReplace);
				if(bReplace)
				{
					(*iter)->SetBindPointName(strNewPointName.c_str());
					nReplaceCount++;
				}	
				iter++;			
			}
		}

		CString strOut;
		strOut.Format(_T("共替换%d处点名!"),nReplaceCount);
		MessageBox(strOut);
	}
	CDialog::OnOK();
}

BOOL CPageReplacePointName::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_pPage)
	{
		CString strPageName = m_pPage->GetPageName().c_str();
		((CEdit*)GetDlgItem(IDC_EDIT_PAGENAME))->SetWindowText(strPageName);
	}

	UpdateData(FALSE);
	return TRUE;
}

std::wstring CPageReplacePointName::Replace( const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced )
{
	size_t pos = 0;
	wstring tempStr = orignStr;
	wstring::size_type newStrLen = newStr.length();
	wstring::size_type oldStrLen = oldStr.length();
	bReplaced = false;
	while(true)
	{
		pos = tempStr.find(oldStr, pos);
		if (pos == wstring::npos) break;

		tempStr.replace(pos, oldStrLen, newStr);        
		pos += newStrLen;
		bReplaced = true;
	}

	return tempStr; 
}
