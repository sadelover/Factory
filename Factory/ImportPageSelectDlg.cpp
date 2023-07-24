// ImportPageSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImportPageSelectDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "DataPoint/DataPointManager.h"
#include "DataPoint/AlarmPointManager.h"
#include "DataPoint/LogicPointManager.h"
#include "../ComponentDraw/ProjectSqlite.h"
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
#include "../ComponentDraw/EqmDrawPolygon.h"
#include "../ComponentDraw/EqmDrawShellExecute.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "../ComponentDraw/EqmDrawLiquidLevel.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../ComponentDraw/EqmDrawScrawl.h"
#include "../ComponentDraw/EqmDrawHistoryTrend.h"
#include "../ComponentDraw/EqmDrawEnergySaveRoi.h"
#include "../ComponentDraw/EqmDrawMapNavigate.h"
#include "../ComponentDraw/EqmDrawNavigateWindow.h"
#include "../ComponentDraw/EqmDrawLevelRuler.h"
#include "../ComponentDraw/EqmDrawDataAnalyse.h"
#include "../ComponentDraw/EqmDrawTemperDistri.h"
#include "../ComponentDraw/EqmDrawDataReport.h"
#include "../ComponentDraw/EqmDrawPlaneTempeDistr.h"
#include "../ComponentDraw/EqmDrawDottedLineFrm.h"
#include "../ComponentDraw/EqmDrawGeneralComponent.h"
#include "../ComponentDraw/EqmDrawRectangle.h"
#include "../ComponentDraw/EqmDrawStraightLine.h"
#include "MainFrm.h"
#include "BEOP-DesignerView.h"
#include "TreeManageDlg.h"
#include "ImportSettingDlg.h"
#include "Tools/Util/UtilString.h"
#include "Tools/CustomTools/CustomTools.h"
#include "ReplacePointName.h"
#include "Tools/DirectoryOperations.h"

#define WM_MYMSG_INIT_FINISH (WM_USER +0x200)


// CImportPageSelectDlg 对话框

IMPLEMENT_DYNAMIC(CImportPageSelectDlg, CSkinDlg)

CImportPageSelectDlg::CImportPageSelectDlg(bool bTemplate,CWnd* pParent /*=NULL*/)
	: CSkinDlg(CImportPageSelectDlg::IDD, pParent),m_project_sqlite(NULL)
	, m_bTemplate(bTemplate)
	, m_threadhandle_LoadProject(NULL)
	, m_bInitImport(false)
	, m_bConfigEsixt(false)
	, m_strFileName(_T(""))
	, m_nID(0)
	, m_bImporSelectResource(false)
	, m_bImporSelectResourceReady(false)
	, m_bResourceChanged(false)
	, m_bSelectAll(FALSE)
{

}

CImportPageSelectDlg::~CImportPageSelectDlg()
{
	if(m_threadhandle_LoadProject)
	{
		CloseHandle(m_threadhandle_LoadProject);
		m_threadhandle_LoadProject = NULL;
	}
}

void CImportPageSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGE_IMPORT, m_list_page);
	DDX_Control(pDX, IDC_EDIT_PATH_IMPORT, m_page_import_path);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SELECT_ALL, m_btnSelectAll);
}


BEGIN_MESSAGE_MAP(CImportPageSelectDlg, CSkinDlg)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_PAGE_SELECT, &CImportPageSelectDlg::OnBnClickedButtonImportPageSelect)
	ON_BN_CLICKED(IDOK, &CImportPageSelectDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MYMSG_INIT_FINISH, OnMyMsgInitFinish)
	ON_BN_CLICKED(IDC_BTN_SELECT_ALL, &CImportPageSelectDlg::OnBnClickedBtnSelectAll)
END_MESSAGE_MAP()


// CImportPageSelectDlg 消息处理程序


void CImportPageSelectDlg::OnBnClickedButtonImportPageSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath = _T("");
	if(m_bTemplate)
	{
		strPath = m_strSysPath + _T("\\template");
	}
	CFileDialog dlgOpenFile(TRUE,NULL,_T(""),OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"db Files (*.s3db;*.4db)|*.s3db;*.4db|");
	dlgOpenFile.m_ofn.lpstrInitialDir= strPath;
	if(dlgOpenFile.DoModal()==IDOK)
	{
		BeginWaitCursor();
		m_strPath = dlgOpenFile.GetPathName();
		m_strFileName = dlgOpenFile.GetFileName();
		m_page_import_path.SetWindowTextW(m_strPath);

		CLogicPointManager			logicPotManager;
		CAlarmPointManager			AlarmPotManager;
		m_project.ClearProject();
		m_vecNewPicID.clear();

		CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
		m_nMaxPicID = pFrame->m_pPrjSqlite->GetMaxLibImageNum()+1;
		m_nMaxAniID = pFrame->m_pPrjSqlite->GetMaxLibAnimationNum()+1;
		m_nPointMaxIndex = 0;
		vector<DataPointEntry> vecPoint = pFrame->m_pPrjSqlite->GetPointList();
		for(int k=0 ;k<vecPoint.size(); ++k)
		{
			if(vecPoint[k].GetPointIndex()>=m_nPointMaxIndex)
			{
				m_nPointMaxIndex = vecPoint[k].GetPointIndex();
			}
		}

		if (m_project_sqlite == NULL)
		{
			m_project_sqlite = new CProjectSqlite(m_project,m_pointmap,logicPotManager,AlarmPotManager);
		}
		if(m_strFileName.Right(4)==_T(".4db"))
		{
			m_project_sqlite->SetFileEncryptType(false);
		}
		else
		{
			m_project_sqlite->SetFileEncryptType(true);
		}


		m_project_sqlite->SetFilePath(m_strPath.GetString());
		m_project_sqlite->SetOwner(eFactory);
		m_project_sqlite->LoadProjectPages();
		m_project_sqlite->LoadProjectSqlite_opcid3();

		m_list_page.DeleteAllItems();
		vector<CEqmDrawPage*>& page_list = m_project.GetPageList();
		for (size_t i=0;i<page_list.size();++i)
		{
			m_list_page.InsertItem(i,L"");
			CString strTemp;
			strTemp.Format(L"%d",page_list[i]->GetID());
			m_list_page.SetItemText(i,1,strTemp);
			m_list_page.SetItemText(i,2,page_list[i]->GetPageName().c_str());
		}
		m_btnOk.EnableWindow(TRUE);
		m_bResourceChanged = false;
		EndWaitCursor();
	} 
}

BOOL CImportPageSelectDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD oldstyle = m_list_page.GetExtendedStyle();
	m_list_page.SetExtendedStyle(oldstyle|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
	m_list_page.InsertColumn(0,_T("是否导入"),LVCFMT_LEFT, 70);
	m_list_page.InsertColumn(1,_T("页面ID"),LVCFMT_LEFT, 120);
	m_list_page.InsertColumn(2,_T("页面名称"),LVCFMT_LEFT, 300);
	m_list_page.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

	GetSysPath(m_strSysPath);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImportPageSelectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	// 配置部分
	if (NULL == m_project_sqlite)
	{
		MessageBox(_T("请先选择需导入的s3db文件！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		return;
	}

	EnableControl(false);

	UINT dwThreadId = 0;
	m_threadhandle_LoadProject = (HANDLE)_beginthreadex(NULL,
		0,
		ThreadFunc_LoadProject, 
		(LPVOID)this,
		0,
		&dwThreadId);


	//ImportSelectPages_CurrentPageResource();

}

void CImportPageSelectDlg::OnDestroy()
{
	CSkinDlg::OnDestroy();
	if (m_project_sqlite)
	{
		delete m_project_sqlite;
		m_project_sqlite = NULL;
	}
	// TODO: 在此处添加消息处理程序代码
}

int CImportPageSelectDlg::GetNewPicIDAndInsertIntoLib( int nMaxPicID, int nPicID )
{
	//在本次导入中的图片进行比对
	for(int i=0; i<m_vecNewPicID.size(); ++i)
	{
		if(m_vecNewPicID[i].nOldPicID == nPicID)
			return m_vecNewPicID[i].nNewPicID;
	}

	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	//获取一个没有在使用的id
	int nNewPicID = pFrame->m_pPrjSqlite->GetUnusedPictureID();	
	
	if (m_project_sqlite != NULL)
	{	
		//从导入的页面所在的4db中获取此ID的全部信息。
		PictureInfo* pic = m_project_sqlite->GetPictureInfoByID(nPicID);
		if(pic == NULL || pic->nSize <=0)		//不存在
			return nPicID;

		//判断现在打开的4db是否含有新图
		PictureInfo* pic_now = pFrame->m_pPrjSqlite->CompareAndReturnPictureInfoByBlock(pic->pBlock, pic->nSize, pic->szName, pic->etype_p, pic->etype_equip);
		if(pic_now)	//已有图
		{
			nNewPicID = pic_now->ID;
		}
		else			//无图插入pic
		{
			if(pic)
			{
				//插入到数据库
				pFrame->m_pPrjSqlite->InsertPictureData(nNewPicID,(char*)(pic->szName),Project::Tools::WideCharToAnsi(pic->etype_equip.c_str()).c_str(),pic->pBlock,pic->nSize);
				//插入到缓存中m_vPictureList;
				pFrame->m_pPrjSqlite->AddIntoPictureList(nNewPicID,pic);
			}
		}

	}
	NewPicIDInfo newPic;
	newPic.nOldPicID = nPicID;
	newPic.nNewPicID = nNewPicID;
	m_vecNewPicID.push_back(newPic);
	return nNewPicID;
}

//void CImportPageSelectDlg::ImportSelectPage()
//{
//	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
//	ASSERT(pFrame);
//	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());
//	ASSERT(pView);
//
//	const int nMaxPicID = pFrame->m_pPrjSqlite->GetMaxLibImageNum()+1;
//	const int nCount = m_list_page.GetItemCount();
//	m_vecImportNewPicID.clear();
//	int nPageNum = 0;
//	CString strOut;
//	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("开始导入页面，请稍候..."));
//	EnableControl(false);
//
//	for (int i=0;i<nCount;++i)
//	{
//		if (m_list_page.GetCheck(i))
//		{
//			int nID = pFrame->m_MaxPageID+IMPORT_PAGE_ELEMENT_ID_START; 
//			HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(m_list_page.GetItemText(i,2), m_hItem, nID);
//			pView->m_pTreeDlg->m_treectrl.Expand(m_hItem, TVE_EXPAND);
//			pView->m_pTreeDlg->m_treectrl.SetFocus();
//			pView->m_pTreeDlg->m_treectrl.SelectItem(hPage);
//
//			const int nPageID = _wtoi(m_list_page.GetItemText(i,1));
//			CEqmDrawPage* pPage = m_project.GetPageByID(nPageID);
//
//			strOut.Format(_T("正在导入页面:%s"),pPage->GetPageName().c_str());
//			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
//
//			m_project_sqlite->LoadPageDetails(pPage,NULL,NULL);
//			CEqmDrawPage* pInsertPage = new CEqmDrawPage(nID,pPage->GetPageName(),pPage->GetProjectID(),0,pPage->GetPageType(),pPage->GetPageWidth(),pPage->GetPageHeight(),pPage->GetPagexPosition(),pPage->GetPageyPosition(),pPage->GetStartColor());
//			{
//				list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
//				list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
//				while (iter != deviceList.end())
//				{
//					CEqmDrawDevice* pDevice = new CEqmDrawDevice((*iter)->GetEqmType(), Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetDataComID(), (*iter)->GetRotateAngle(), (*iter)->GetEqmName(), (*iter)->GetEquipmentSubType(), (*iter)->GetWidth(), "", (*iter)->GetHeight(), 0.0f, 0.0f, 0.0f);
//					ASSERT(pDevice);
//					pDevice->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));//important
//					pDevice->SetLayer((*iter)->GetLayer());
//					pDevice->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));
//					pDevice->SetPhysicalID(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()));
//					pDevice->SetDeviceValue((*iter)->GetDeviceValue());
//
//					if ((*iter)->GetBindString().size()>0)
//					{
//						pDevice->SetBindString((*iter)->GetBindString());
//						pDevice->ParseParam10((*iter)->GetBindString().c_str());
//					}
//
//					pDevice->m_eventInfo = (*iter)->m_eventInfo;
//					pDevice->SetLinkPageID((*iter)->GetLinkPageID());
//					Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(pDevice->GetPicID());
//					if(pBitmap)
//						pDevice->SetBitmap(pBitmap);
//					pInsertPage->AddDevice(pDevice);
//					iter++;
//				}
//			}
//			{
//				list<CEqmDrawPipe*>& deviceList = pPage->GetPagePipeList();
//				list<CEqmDrawPipe*>::iterator iter = deviceList.begin();
//				while (iter != deviceList.end())
//				{
//					CEqmDrawPipe* pPipe = new CEqmDrawPipe( nID, (*iter)->GetID(), (*iter)->GetPipeColor(), (*iter)->GetPipeWidth(), (*iter)->GetPointStringList() ); //8.9
//					ASSERT(pPipe);
//					pPipe->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//					pPipe->SetLayer((*iter)->GetLayer());
//					pPipe->SetDirection((*iter)->GetDirection());
//					pPipe->SetDataPointList((*iter)->GetDataPointList());
//					pPipe->SetPointLIst((*iter)->GetPointList());
//					pPipe->SetDataComID((*iter)->GetDataComIDVector());
//					pPipe->SetPipeColor((*iter)->GetPipeColor());
//					pPipe->SetPipeWidth((*iter)->GetPipeWidth());
//					pPipe->SetPipeType((*iter)->GetPipeType());
//					pInsertPage->AddPipe(pPipe);
//					iter++;
//				}
//			}
//			{
//				list<CEqmDrawText*>& textList = pPage->GetPageTextList();
//				list<CEqmDrawText*>::iterator iter = textList.begin();
//				while (iter != textList.end())
//				{
//					CEqmDrawText* pText = new CEqmDrawText(Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetWidth(), 
//						(*iter)->GetHeight(), (*iter)->GetFontSize(), (*iter)->GetFontName(), CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()),(*iter)->GetShowString(), (*iter)->GetFontColor(), (*iter)->GetTextShowMode(),(*iter)->GetTransparence(),(*iter)->GetBorderType(),(*iter)->GetBorderColor());
//					ASSERT(pText);
//					pText->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//					pText->SetLayer((*iter)->GetLayer());
//					pText->SetAlign((*iter)->GetAlign());
//					pText->SetDateTime((*iter)->GetDateTime());
//					pText->SetPointNum((*iter)->GetPointNum());
//					pText->SetTextModeShowString((*iter)->GetTextModeShowString());
//					pText->SetTextBKType((*iter)->GetTextBKType());
//					pInsertPage->AddText(pText);
//					iter++;
//				}
//			}
//			{
//				list<CEqmDrawGraphic*>& graphicList = pPage->Get2DGraphicList();
//				list<CEqmDrawGraphic*>::iterator iter = graphicList.begin();
//				while (iter != graphicList.end())
//				{
//					CEqmDrawGraphic* pgrapic = NULL;
//					EQMDRAWTYPE type = (*iter)->GetEqmType();
//					switch(type)
//					{
//					case GraphicType_LINE:		//直线
//						pgrapic = new CEqmDrawGraphicLine(
//							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetFilledColor(), (*iter)->GetLineStyle(), 
//							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
//						break;
//					case GraphicType_RECTANGLE:	//矩形
//						pgrapic = new CEqmDrawGraphicRect(
//							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
//							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(),(*iter)->GetTransparent(), (*iter)->GetWidth(), (*iter)->GetHeight(),(*iter)->GetBorderIs3D());
//						break;
//					case GraphicType_ELLIPSE:	//椭圆
//						pgrapic = new CEqmDrawGraphicEllipse(
//							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
//							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
//						break;
//					case GraphicType_POLYGON:	//多边形
//						pgrapic = new CEqmDrawGraphicPolygon(
//							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
//							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
//						break;
//					case GraphicType_TRIANGLE:
//						break;
//					default:
//						ASSERT(false);
//						break;
//					}
//
//					ASSERT(pgrapic);
//					pgrapic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//					pgrapic->SetLayer((*iter)->GetLayer());
//					pInsertPage->Add2DGraphic(pgrapic);
//					iter++;
//				}
//			}
//			{
//				list<CEqmDrawBend*>& bendList = pPage->GetBendList();
//				list<CEqmDrawBend*>::iterator iter = bendList.begin();
//				while (iter != bendList.end())
//				{
//					CEqmDrawBend* pBend = new CEqmDrawBend();
//					if(pBend)
//					{
//						pBend->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pBend->SetEqmType((*iter)->GetEqmType());
//						pBend->SetPageID(nID);
//						pBend->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pBend->SetWidth((*iter)->GetWidth());
//						pBend->SetHeight((*iter)->GetHeight());
//						pBend->SetRotateAngle((*iter)->GetRotateAngle());
//						pBend->SetLayer((*iter)->GetLayer());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(33);//弯头的ID是33
//						if(pBitmap)
//						{
//							pBend->SetDefaultBitmap(pBitmap);
//						}
//						pInsertPage->AddBend(pBend);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawDashBoard*>& dashboardList = pPage->GetDashboardList();
//				list<CEqmDrawDashBoard*>::iterator iter = dashboardList.begin();
//				while (iter != dashboardList.end())
//				{
//					CEqmDrawDashBoard* pDashBoard = new CEqmDrawDashBoard();
//					if(pDashBoard)
//					{
//						pDashBoard->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pDashBoard->SetEqmType((*iter)->GetEqmType());
//						pDashBoard->SetPageID(nID);
//						pDashBoard->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pDashBoard->SetWidth((*iter)->GetWidth());
//						pDashBoard->SetHeight((*iter)->GetHeight());
//						pDashBoard->SetStyle((*iter)->GetStyle());
//						pDashBoard->SetMaxValue((*iter)->GetMaxValue());
//						pDashBoard->SetMinValue((*iter)->GetMinValue());
//						pDashBoard->SetBindPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointName().GetString()).c_str());
//						int nPicID = 0;
//						if ((*iter)->GetStyle() == 0)
//						{
//							nPicID = 52;
//						}
//						else if ((*iter)->GetStyle() == 1)
//						{
//							nPicID = 76;
//						}
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nPicID);
//						if(pBitmap)
//						{
//							pDashBoard->SetDefaultBitmap(pBitmap);
//						}
//						pInsertPage->AddDashboard(pDashBoard);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawLineGraphic*>& linechartList = pPage->GetLineGraphList();
//				list<CEqmDrawLineGraphic*>::iterator iter = linechartList.begin();
//				while (iter != linechartList.end())
//				{
//					CEqmDrawLineGraphic* pLineGraphic = new CEqmDrawLineGraphic();
//					if(pLineGraphic)
//					{
//						pLineGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pLineGraphic->SetEqmType((*iter)->GetEqmType());
//						pLineGraphic->SetPageID(nID);
//						pLineGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pLineGraphic->SetTitleName((*iter)->GetTitleName());
//						pLineGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
//						pLineGraphic->SetXName((*iter)->GetXName());
//						pLineGraphic->SetXUnit((*iter)->GetXUnit());
//						pLineGraphic->SetYName((*iter)->GetYName());
//						pLineGraphic->SetWidth((*iter)->GetWidth());
//						pLineGraphic->SetHeight((*iter)->GetHeight());
//						pLineGraphic->SetType((*iter)->GetType());
//						pLineGraphic->SetMaxValue((*iter)->GetMaxValue());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(34);
//						if(pBitmap)
//						{
//							pLineGraphic->SetDefaultBitmap(pBitmap);
//						}
//						pFrame->m_pPrjSqlite->GetDetailChartByIDs(nPageID, pLineGraphic->GetID(), pLineGraphic->GetItemVector());
//						pInsertPage->AddLineGraphic(pLineGraphic);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawBarGraphic*>& barchartList = pPage->GetBarGraphList();
//				list<CEqmDrawBarGraphic*>::iterator iter = barchartList.begin();
//				while (iter != barchartList.end())
//				{
//					CEqmDrawBarGraphic* pBarGraphic = new CEqmDrawBarGraphic();
//					if(pBarGraphic)
//					{
//						pBarGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pBarGraphic->SetEqmType((*iter)->GetEqmType());
//						pBarGraphic->SetPageID(nID);
//						pBarGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pBarGraphic->SetTitleName((*iter)->GetTitleName());
//						pBarGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
//						pBarGraphic->SetXName((*iter)->GetXName());
//						pBarGraphic->SetXUnit((*iter)->GetXUnit());
//						pBarGraphic->SetYName((*iter)->GetYName());
//						pBarGraphic->SetWidth((*iter)->GetWidth());
//						pBarGraphic->SetHeight((*iter)->GetHeight());
//						pBarGraphic->SetType((*iter)->GetType());
//						pBarGraphic->SetMaxValue((*iter)->GetMaxValue());
//						pBarGraphic->SetStartPoint((*iter)->GetStartPoint());
//						pBarGraphic->SetQueryType((*iter)->GetQueryType());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(35);
//						if(pBitmap)
//						{
//							pBarGraphic->SetDefaultBitmap(pBitmap);
//						}
//						pFrame->m_pPrjSqlite->GetDetailChartByIDs(nPageID, pBarGraphic->GetID(), pBarGraphic->GetItemVector());
//						pInsertPage->AddBarGraphic(pBarGraphic);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawPieGraphic*>& piechartList = pPage->GetPieGraphList();
//				list<CEqmDrawPieGraphic*>::iterator iter = piechartList.begin();
//				while (iter != piechartList.end())
//				{
//					CEqmDrawPieGraphic* pPieGraphic = new CEqmDrawPieGraphic();
//					if(pPieGraphic)
//					{
//						pPieGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pPieGraphic->SetEqmType((*iter)->GetEqmType());
//						pPieGraphic->SetPageID(nID);
//						pPieGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pPieGraphic->SetTitleName((*iter)->GetTitleName());
//						pPieGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
//						pPieGraphic->SetWidth((*iter)->GetWidth());
//						pPieGraphic->SetHeight((*iter)->GetHeight());
//						pPieGraphic->SetType((*iter)->GetType());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(36);
//						if(pBitmap)
//						{
//							pPieGraphic->SetDefaultBitmap(pBitmap);
//						}
//						pFrame->m_pPrjSqlite->GetDetailChartByIDs(nPageID, pPieGraphic->GetID(), pPieGraphic->GetItemVector());
//						pInsertPage->AddPieGraphic(pPieGraphic);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawButton*>& buttonList = pPage->GetButtonList();
//				list<CEqmDrawButton*>::iterator iter = buttonList.begin();
//				while (iter != buttonList.end())
//				{
//					CEqmDrawButton* pButton = new CEqmDrawButton();
//					if(pButton)
//					{
//						pButton->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pButton->SetEqmType((*iter)->GetEqmType());
//						pButton->SetPageID(nID);
//						pButton->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pButton->SetWidth((*iter)->GetWidth());
//						pButton->SetHeight((*iter)->GetHeight());
//						pButton->SetLayer((*iter)->GetLayer());
//						int nComm = 0;
//						int nDisable = 0;
//						int nDown = 0;
//						int nOver = 0;
//						int nChecked = 0;
//						(*iter)->Get5PicID(nComm,nDisable,nDown,nOver,nChecked);
//						nComm = GetNewPicIDAndInsertIntoLib(nMaxPicID,nComm);
//						nDisable = GetNewPicIDAndInsertIntoLib(nMaxPicID,nDisable);
//						nDown = GetNewPicIDAndInsertIntoLib(nMaxPicID,nDown);
//						nOver = GetNewPicIDAndInsertIntoLib(nMaxPicID,nOver);
//						nChecked = GetNewPicIDAndInsertIntoLib(nMaxPicID,nChecked);
//						pButton->Set5PicID(nComm,nDisable,nDown,nOver,nChecked);
//						pButton->SetLinkPageID((*iter)->GetLinkPageID());
//						pButton->SetOperation((*iter)->GetOperation());
//						pButton->SetTextContent((*iter)->GetTextContent());
//						pButton->SetTextColor((*iter)->GetTextColor());
//						pButton->SetTextSize((*iter)->GetTextSize());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nComm);
//						if(pBitmap)
//						{
//							pButton->SetDefaultBitmap(pBitmap);
//						}
//						pInsertPage->AddButton(pButton);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawTimeSelect*>& timeSelectList = pPage->GetTimeSelectList();
//				list<CEqmDrawTimeSelect*>::iterator iter = timeSelectList.begin();
//				while (iter != timeSelectList.end())
//				{
//					CEqmDrawTimeSelect* pTimeSelect = new CEqmDrawTimeSelect();
//					if(pTimeSelect)
//					{
//						pTimeSelect->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pTimeSelect->SetEqmType((*iter)->GetEqmType());
//						pTimeSelect->SetPageID(nID);
//						pTimeSelect->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pTimeSelect->SetWidth((*iter)->GetWidth());
//						pTimeSelect->SetHeight((*iter)->GetHeight());
//						pTimeSelect->SetLayer((*iter)->GetLayer());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(95);
//						if(pBitmap)
//						{
//							pTimeSelect->SetDefaultBitmap(pBitmap);
//						}
//						pTimeSelect->SetRWProperty((*iter)->GetRWProperty());
//						pTimeSelect->SetDateTime((*iter)->GetDateTime());
//						pInsertPage->AddTimeSelect(pTimeSelect);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawTimeBar*>& timeBarList = pPage->GetTimeBarList();
//				list<CEqmDrawTimeBar*>::iterator iter = timeBarList.begin();
//				while (iter != timeBarList.end())
//				{
//					CEqmDrawTimeBar* pTimeBar = new CEqmDrawTimeBar();
//					if(pTimeBar)
//					{
//						pTimeBar->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pTimeBar->SetEqmType((*iter)->GetEqmType());
//						pTimeBar->SetPageID(nID);
//						pTimeBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pTimeBar->SetWidth((*iter)->GetWidth());
//						pTimeBar->SetHeight((*iter)->GetHeight());
//						pTimeBar->SetLayer((*iter)->GetLayer());
//						pTimeBar->SetBindPointStart((*iter)->GetBindPointStart());
//						pTimeBar->SetQueryType((*iter)->GetQueryType());
//						pTimeBar->SetBindPointStatus((*iter)->GetBindPointStatus());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(100);
//						if(pBitmap)
//						{
//							pTimeBar->SetDefaultBitmap(pBitmap);
//						}
//						pTimeBar->SetBarColor((*iter)->GetBarColor());
//						const vector<_tag_bar_item>& vec = (*iter)->GetItemVec();
//						const size_t size = vec.size();
//						for (size_t i=0;i<size;++i)
//						{
//							pTimeBar->InsertItem(vec[i]);
//						}
//						pInsertPage->AddTimeBar(pTimeBar);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawProgressBar*>& ProgressBarList = pPage->GetProgressBarList();
//				list<CEqmDrawProgressBar*>::iterator iter = ProgressBarList.begin();
//				while (iter != ProgressBarList.end())
//				{
//					CEqmDrawProgressBar* pProgressBar = new CEqmDrawProgressBar();
//					if(pProgressBar)
//					{
//						pProgressBar->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pProgressBar->SetEqmType((*iter)->GetEqmType());
//						pProgressBar->SetPageID(nID);
//						pProgressBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pProgressBar->SetWidth((*iter)->GetWidth());
//						pProgressBar->SetHeight((*iter)->GetHeight());
//						pProgressBar->SetLayer((*iter)->GetLayer());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(101);
//						if(pBitmap)
//						{
//							pProgressBar->SetDefaultBitmap(pBitmap);
//						}
//						pProgressBar->SetFactorValue((*iter)->GetFactorValue());
//						pProgressBar->SetBarColor((*iter)->GetBarColor());
//						pInsertPage->AddProgressBar(pProgressBar);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = pPage->GetDiagnoseList();
//				list<CEqmDrawDiagnoseGraphic*>::iterator iter = DiagnoseGraphicList.begin();
//				while (iter != DiagnoseGraphicList.end())
//				{
//					CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = new CEqmDrawDiagnoseGraphic();
//					if(pDiagnoseGraphic)
//					{
//						pDiagnoseGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pDiagnoseGraphic->SetEqmType((*iter)->GetEqmType());
//						pDiagnoseGraphic->SetPageID((*iter)->GetPageID());
//						pDiagnoseGraphic->SetID((*iter)->GetID());
//						pDiagnoseGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
//						pDiagnoseGraphic->SetWidth((*iter)->GetWidth());
//						pDiagnoseGraphic->SetHeight((*iter)->GetHeight());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(132);
//						if(pBitmap)
//						{
//							pDiagnoseGraphic->SetDefaultBitmap(pBitmap);
//						}
//						pInsertPage->AddDiagnoseGraphic(pDiagnoseGraphic);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawDiagnoseGraphicPlus*>& DiagnoseGraphicListPlus = pPage->GetDiagnoseListPlus();
//				list<CEqmDrawDiagnoseGraphicPlus*>::iterator iter = DiagnoseGraphicListPlus.begin();
//				while (iter != DiagnoseGraphicListPlus.end())
//				{
//					CEqmDrawDiagnoseGraphicPlus* pDiagnoseGraphicPlus = new CEqmDrawDiagnoseGraphicPlus();
//					if(pDiagnoseGraphicPlus)
//					{
//						pDiagnoseGraphicPlus->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pDiagnoseGraphicPlus->SetEqmType((*iter)->GetEqmType());
//						pDiagnoseGraphicPlus->SetPageID((*iter)->GetPageID());
//						pDiagnoseGraphicPlus->SetID((*iter)->GetID());
//						pDiagnoseGraphicPlus->SetWidth((*iter)->GetWidth());
//						pDiagnoseGraphicPlus->SetHeight((*iter)->GetHeight());
//						pDiagnoseGraphicPlus->SetNormalPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetNormalPicID()));
//						pDiagnoseGraphicPlus->SetAnimationOrPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetAnimationOrPicID()));
//						pDiagnoseGraphicPlus->SetIsAnimation((*iter)->GetIsAnimation());
//						pDiagnoseGraphicPlus->SetLinkPageID((*iter)->GetLinkPageID());
//						pDiagnoseGraphicPlus->SetShowMode((*iter)->GetShowMode());
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetNormalPicID());
//						if(pBitmap)
//						{
//							pDiagnoseGraphicPlus->SetDefaultBitmap(pBitmap);
//						}
//						pDiagnoseGraphicPlus->SetTimeScope((*iter)->GetTimeScope());
//						pDiagnoseGraphicPlus->SetTimeInterval((*iter)->GetTimeInterval());
//						pDiagnoseGraphicPlus->SetExtendTimeSpan((*iter)->GetExtendTimeSpan());
//
//						vector<_tagBoolBindInfo>& vec = (*iter)->GetBoolBindInfoVec();
//						for (size_t i=0;i<vec.size();++i)
//						{
//							pDiagnoseGraphicPlus->InsertOneBoolBindInfo(vec[i]);
//						}
//						pInsertPage->AddDiagnoseGraphicPlus(pDiagnoseGraphicPlus);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawAreaChange*>& AreaChangeList = pPage->GetAreaChangeList();
//				list<CEqmDrawAreaChange*>::iterator iter = AreaChangeList.begin();
//				while (iter != AreaChangeList.end())
//				{
//					CEqmDrawAreaChange* pAreaChange = new CEqmDrawAreaChange();
//					if(pAreaChange)
//					{
//						pAreaChange->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pAreaChange->SetEqmType((*iter)->GetEqmType());
//						pAreaChange->SetPageID(nID);
//						pAreaChange->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pAreaChange->SetWidth((*iter)->GetWidth());
//						pAreaChange->SetHeight((*iter)->GetHeight());
//						pAreaChange->SetLayer((*iter)->GetLayer());
//						pAreaChange->SetBitmap((*iter)->GetBitmap());
//						pAreaChange->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));
//						//pAreaChange->SetPicID((*iter)->GetPicID());
//						vector<_tagAreaInfo> vecArea = (*iter)->GetAreaInfo();
//						for(int j=0; j<vecArea.size(); j++)
//						{
//							vecArea[j].BnmID = GetNewPicIDAndInsertIntoLib(nMaxPicID,vecArea[j].BnmID);
//						}
//
//						pAreaChange->SetAreaInfo(vecArea);
//					
//						pInsertPage->AddAreaChange(pAreaChange);
//						iter++;
//					}
//				}
//			}
//			{
//				list<CEqmDrawShellExecute*>& shellExecuteList = pPage->GetShellExecuteList();
//				list<CEqmDrawShellExecute*>::iterator iter = shellExecuteList.begin();
//				while (iter != shellExecuteList.end())
//				{
//					CEqmDrawShellExecute* pShellExecute = new CEqmDrawShellExecute();
//					if(pShellExecute)
//					{
//						pShellExecute->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//						pShellExecute->SetEqmType(EQMDRAW_SHELL_EXECUTE);
//						pShellExecute->SetPageID(nID);
//						pShellExecute->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//						pShellExecute->SetWidth((*iter)->GetWidth());
//						pShellExecute->SetHeight((*iter)->GetHeight());
//						pShellExecute->SetLayer((*iter)->GetLayer());
//
//						pShellExecute->SetServerIP((*iter)->GetServerIP());
//						pShellExecute->SetFileName((*iter)->GetFileName());
//						pShellExecute->SetDownloadDirectory((*iter)->GetDownloadDirectory());
//						pShellExecute->SetFileID((*iter)->GetFileID());
//						pShellExecute->SetBKPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetBKPicID()));
//						pShellExecute->SetAutoDownload((*iter)->GetAutoDownload());
//
//						Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetBKPicID());
//						if(pBitmap)
//						{
//							pShellExecute->SetDefaultBitmap(pBitmap);
//						}
//						pInsertPage->AddShellExecute(pShellExecute);
//					}
//					iter++;
//				}
//			}
//			{
//				list<CEqmDrawEmbededPage*>& embededpageList = pPage->GetEmbededPageList();
//				list<CEqmDrawEmbededPage*>::iterator iter = embededpageList.begin();
//				while (iter != embededpageList.end())
//				{
//					CEqmDrawEmbededPage* pEmbededPage = new CEqmDrawEmbededPage();
//					pEmbededPage->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
//					pEmbededPage->SetEqmType(EQMDRAW_EMBEDED_PAGE);
//					pEmbededPage->SetPageID(nID);
//					pEmbededPage->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
//					pEmbededPage->SetWidth((*iter)->GetWidth());
//					pEmbededPage->SetHeight((*iter)->GetHeight());
//					pEmbededPage->SetLayer((*iter)->GetLayer());
//					pEmbededPage->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));
//					pEmbededPage->SetChangeInterval((*iter)->GetChangeInterval());
//					Gdiplus::Bitmap* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetPicID());
//					if(pBitmap)
//					{
//						pEmbededPage->SetDefaultBitmap(pBitmap);
//					}
//					vector<PageIDName>&	vec = (*iter)->GetPageList();
//					for (size_t i=0;i<vec.size();++i)
//					{
//						pEmbededPage->InsertPageList(vec[i].nPageID, vec[i].strPageName);
//					}
//					pInsertPage->AddEmbededPage(pEmbededPage);
//					++iter;
//				}
//			}
//			pInsertPage->SetID(nID);
//			CString strItemText = pView->m_pTreeDlg->m_treectrl.GetItemText(m_hItem);
//			const int nGroupID = pFrame->m_project.GetGroupIDByGroupName(strItemText.GetString());
//			pInsertPage->SetGroupID(nGroupID);
//			pFrame->m_DrawContext.pPage = pInsertPage;
//			pFrame->m_DrawContext.pPage->SetGroupID(nGroupID);
//			pInsertPage->m_bDetailLoaded = true;
//			pFrame->m_project.AddPage(pInsertPage);
//			nPageNum ++;
//			strOut.Format(_T("已导入第%d个页面:%s"),nPageNum,pInsertPage->GetPageName().c_str());
//			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
//			pFrame->m_MaxPageID++;
//		}
//	}
//	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("导入完成"));
//	//::PostMessage(this->m_hWnd, WM_MYMSG_INIT_FINISH, 0,0);
//}
//
UINT WINAPI CImportPageSelectDlg::ThreadFunc_LoadProject( LPVOID lparam )
{
	CImportPageSelectDlg *pThis = (CImportPageSelectDlg *) lparam;
	if (pThis)
	{
		pThis->ImportSelectPages_CurrentPageResource();
		pThis->EnableControl(true);
	}
	return 1;
}

void CImportPageSelectDlg::EnableControl( bool bResult )
{
	GetDlgItem(IDC_EDIT_PATH_IMPORT)->EnableWindow(bResult);
	GetDlgItem(IDC_BUTTON_IMPORT_PAGE_SELECT)->EnableWindow(bResult);
	GetDlgItem(IDC_LIST_PAGE_IMPORT)->EnableWindow(bResult);
	m_btnSelectAll.EnableWindow(bResult);
	m_btnOk.EnableWindow(bResult);
	m_btnCancel.EnableWindow(bResult);
	SetCloseButtonState(bResult);
}

LRESULT CImportPageSelectDlg::OnMyMsgInitFinish( WPARAM wParam,LPARAM lParam )
{
	CSkinDlg::OnOK();
	return 0;
}

wstring CImportPageSelectDlg::CreatePointIfNotFind(CProjectSqlite* project_sqlite, wstring strPointName )
{	
	if(project_sqlite)
	{
		if(!project_sqlite->IsPointExist(strPointName))			//不存在点
		{
			DataPointEntry entry;
			entry = m_pointmap.GetEntry(strPointName);	
			if(entry.GetSourceType().length() == 0)		//不存在的点则不保存
			{
				return strPointName;
			}

			m_nPointMaxIndex++;
			entry.SetPointIndex(m_nPointMaxIndex);

			project_sqlite->AddPointEntry(entry);

			CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
			pFrame->SetPointTableFlag(true);
		}
	}
	return strPointName;
}

void CImportPageSelectDlg::ImportSelectPages()
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());
	ASSERT(pView);

	const int nMaxPicID = pFrame->m_pPrjSqlite->GetMaxLibImageNum()+1;
	const int nCount = m_list_page.GetItemCount();
	m_vecImportNewPicID.clear();
	int nPageNum = 0;
	CString strOut;
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("开始导入页面，请稍候..."));
	EnableControl(false);

	int nPageMaxOrder = pFrame->m_project.GetMaxPageShowOrder() + 1;
	for (int i=0;i<nCount;++i)
	{
		if (m_list_page.GetCheck(i))
		{
			const int nPageID = _wtoi(m_list_page.GetItemText(i,1));
			CEqmDrawPage* pPage = m_project.GetPageByID(nPageID);

			int nID = nPageID + m_nID*E_PAGE_BASE_OFFSET;


			/*HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(m_list_page.GetItemText(i,2), m_hItem, nID);
			pView->m_pTreeDlg->m_treectrl.Expand(m_hItem, TVE_EXPAND);
			pView->m_pTreeDlg->m_treectrl.SetFocus();
			pView->m_pTreeDlg->m_treectrl.SelectItem(hPage);*/


			HTREEITEM hPage = pFrame->m_wndFileView.InsertPage(m_list_page.GetItemText(i,2), m_hItem, nID);
			 pFrame->m_wndFileView.m_wndFileView.Expand(m_hItem, TVE_EXPAND);
			pFrame->m_wndFileView.m_wndFileView.SetFocus();
			pFrame->m_wndFileView.m_wndFileView.SelectItem(hPage);

			strOut.Format(_T("正在导入页面:%s"),pPage->GetPageName().c_str());
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);

			m_project_sqlite->LoadPageDetails(pPage,NULL,NULL);
			CEqmDrawPage* pInsertPage = new CEqmDrawPage(nID,pPage->GetPageName(),pPage->GetProjectID(),nPageMaxOrder,pPage->GetPageType(),pPage->GetPageWidth(),pPage->GetPageHeight(),pPage->GetPagexPosition(),pPage->GetPageyPosition(),pPage->GetStartColor());
			++nPageMaxOrder;
			{
				list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
				list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
				while (iter != deviceList.end())
				{
					CEqmDrawDevice* pDevice = new CEqmDrawDevice((*iter)->GetEqmType(), Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetDataComID(), (*iter)->GetRotateAngle(), (*iter)->GetEqmName(), (*iter)->GetEquipmentSubType(), (*iter)->GetWidth(), "", (*iter)->GetHeight(), 0.0f, 0.0f, 0.0f,(*iter)->GetBindType());
					ASSERT(pDevice);
					pDevice->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));//important
					pDevice->SetLayer((*iter)->GetLayer());
					pDevice->SetPicID(m_nID*E_IMAGE_BASE_OFFSET + (*iter)->GetPicID());
					pDevice->SetPhysicalID((*iter)->GetPhysicalID());
					pDevice->SetDeviceValue((*iter)->GetDeviceValue());

					if ((*iter)->GetBindString().size()>0)
					{
						pDevice->SetBindString((*iter)->GetBindString());
						pDevice->ParseParam10((*iter)->GetBindString().c_str(),m_nID*E_IMAGE_BASE_OFFSET);
					}

					pDevice->m_eventInfo = (*iter)->m_eventInfo;
					pDevice->SetLinkPageID((*iter)->GetLinkPageID()+ m_nID*E_PAGE_BASE_OFFSET);
					Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(pDevice->GetPicID());
					if(pBitmap)
						pDevice->SetBitmap(pBitmap);
					pInsertPage->AddDevice(pDevice);
					iter++;
				}
			}
			{
				list<CEqmDrawPipe*>& deviceList = pPage->GetPagePipeList();
				list<CEqmDrawPipe*>::iterator iter = deviceList.begin();
				while (iter != deviceList.end())
				{
					CEqmDrawPipe* pPipe = new CEqmDrawPipe( nID, (*iter)->GetID(), (*iter)->GetPipeColor(), (*iter)->GetPipeWidth(), (*iter)->GetPointStringList() ); //8.9
					ASSERT(pPipe);
					pPipe->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pPipe->SetLayer((*iter)->GetLayer());
					pPipe->SetDirection((*iter)->GetDirection());
					pPipe->SetDataPointList((*iter)->GetDataPointList());
					pPipe->SetPointLIst((*iter)->GetPointList());
					pPipe->SetDataComID((*iter)->GetDataComIDVector());
					pPipe->SetPipeColor((*iter)->GetPipeColor());
					pPipe->SetPipeWidth((*iter)->GetPipeWidth());
					pPipe->SetPipeType((*iter)->GetPipeType());
					pInsertPage->AddPipe(pPipe);
					iter++;
				}
			}
			{
				list<CEqmDrawText*>& textList = pPage->GetPageTextList();
				list<CEqmDrawText*>::iterator iter = textList.begin();
				while (iter != textList.end())
				{
					CEqmDrawText* pText = new CEqmDrawText(Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetWidth(), 
						(*iter)->GetHeight(), (*iter)->GetFontSize(), (*iter)->GetFontName(), (*iter)->GetPhysicalID(),(*iter)->GetShowString(), (*iter)->GetFontColor(),
						(*iter)->GetTextShowMode(),(*iter)->GetTransparence(),(*iter)->GetBorderType(),(*iter)->GetBorderColor(), (*iter)->GetDeviceCfg(), (*iter)->GetFaultDiagnosis(),(*iter)->GetBindScript());
					ASSERT(pText);
					pText->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pText->SetLayer((*iter)->GetLayer());
					pText->SetAlign((*iter)->GetAlign());
					pText->SetDateTime((*iter)->GetDateTime());
					int nPointNum = (*iter)->GetPointNum();
					if(nPointNum>4 || nPointNum<0)
						nPointNum = 1;
					pText->SetPointNum(nPointNum);
					pText->SetTextModeShowString((*iter)->GetTextModeShowString());
					pText->SetTextBKType((*iter)->GetTextBKType());
					pText->SetDateTimePoint((*iter)->GetDateTimePoint());
					pText->SetTextAttribute((*iter)->GetTextAttribute());
					pText->SetUnit((*iter)->GetUnit());
					pInsertPage->AddText(pText);
					iter++;
				}
			}
			{
				list<CEqmDrawGraphic*>& graphicList = pPage->Get2DGraphicList();
				list<CEqmDrawGraphic*>::iterator iter = graphicList.begin();
				while (iter != graphicList.end())
				{
					CEqmDrawGraphic* pgrapic = NULL;
					EQMDRAWTYPE type = (*iter)->GetEqmType();
					switch(type)
					{
					case GraphicType_LINE:		//直线
						pgrapic = new CEqmDrawGraphicLine(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetFilledColor(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
						break;
					case GraphicType_RECTANGLE:	//矩形
						pgrapic = new CEqmDrawGraphicRect(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(),(*iter)->GetTransparent(), (*iter)->GetWidth(), (*iter)->GetHeight(),(*iter)->GetBorderIs3D());
						break;
					case GraphicType_ELLIPSE:	//椭圆
						pgrapic = new CEqmDrawGraphicEllipse(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
						break;
					case GraphicType_POLYGON:	//多边形
						pgrapic = new CEqmDrawGraphicPolygon(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
						break;
					case GraphicType_TRIANGLE:
						break;
					default:
						ASSERT(false);
						break;
					}

					ASSERT(pgrapic);
					pgrapic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pgrapic->SetLayer((*iter)->GetLayer());
					pInsertPage->Add2DGraphic(pgrapic);
					iter++;
				}
			}
			{
				list<CEqmDrawBend*>& bendList = pPage->GetBendList();
				list<CEqmDrawBend*>::iterator iter = bendList.begin();
				while (iter != bendList.end())
				{
					CEqmDrawBend* pBend = new CEqmDrawBend();
					if(pBend)
					{
						pBend->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pBend->SetEqmType((*iter)->GetEqmType());
						pBend->SetPageID(nID);
						pBend->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pBend->SetWidth((*iter)->GetWidth());
						pBend->SetHeight((*iter)->GetHeight());
						pBend->SetRotateAngle((*iter)->GetRotateAngle());
						pBend->SetLayer((*iter)->GetLayer());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\bend.png");
						if(pBitmap)
						{
							pBend->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddBend(pBend);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawDashBoard*>& dashboardList = pPage->GetDashboardList();
				list<CEqmDrawDashBoard*>::iterator iter = dashboardList.begin();
				while (iter != dashboardList.end())
				{
					CEqmDrawDashBoard* pDashBoard = new CEqmDrawDashBoard();
					if(pDashBoard)
					{
						pDashBoard->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pDashBoard->SetEqmType((*iter)->GetEqmType());
						pDashBoard->SetPageID(nID);
						pDashBoard->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pDashBoard->SetWidth((*iter)->GetWidth());
						pDashBoard->SetHeight((*iter)->GetHeight());
						pDashBoard->SetStyle((*iter)->GetStyle());
						pDashBoard->SetMaxValue((*iter)->GetMaxValue());
						pDashBoard->SetMinValue((*iter)->GetMinValue());
						pDashBoard->SetBindPointName((*iter)->GetBindPointName());
						pDashBoard->SetMaxString((*iter)->GetMaxString());
						pDashBoard->SetMinString((*iter)->GetMinString());
						pDashBoard->SetMaxBindFlag((*iter)->GetMaxBindFlag());
						pDashBoard->SetMinBindFlag((*iter)->GetMinBindFlag());
						pDashBoard->SetFontColor((*iter)->GetFontColor());
						pDashBoard->SetDecimalPlaces((*iter)->GetDecimalPlaces());
						CString	strImgPath;
						if ((*iter)->GetStyle() == 0)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_0.png";
						}
						else if ((*iter)->GetStyle() == 1)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_1.png";
						}
						else if ((*iter)->GetStyle() == 2)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_2.png";
						}
						else if ((*iter)->GetStyle() == 3)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_3.png";
						}
						else
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_0.png";
						}
						Image* pBitmap = Image::FromFile(strImgPath);
						if(pBitmap)
						{
							pDashBoard->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddDashboard(pDashBoard);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawLineGraphic*>& linechartList = pPage->GetLineGraphList();
				list<CEqmDrawLineGraphic*>::iterator iter = linechartList.begin();
				while (iter != linechartList.end())
				{
					CEqmDrawLineGraphic* pLineGraphic = new CEqmDrawLineGraphic();
					if(pLineGraphic)
					{
						pLineGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pLineGraphic->SetEqmType((*iter)->GetEqmType());
						pLineGraphic->SetPageID(nID);
						pLineGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pLineGraphic->SetTitleName((*iter)->GetTitleName());
						pLineGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pLineGraphic->SetXName((*iter)->GetXName());
						pLineGraphic->SetXUnit((*iter)->GetXUnit());
						pLineGraphic->SetYName((*iter)->GetYName());
						pLineGraphic->SetWidth((*iter)->GetWidth());
						pLineGraphic->SetHeight((*iter)->GetHeight());
						pLineGraphic->SetType((*iter)->GetType());
						pLineGraphic->SetMaxValue((*iter)->GetMaxValue());
						pLineGraphic->SetTextColor((*iter)->GetTextColor());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartLine.png");
						if(pBitmap)
						{
							pLineGraphic->SetDefaultBitmap(pBitmap);
						}
						//m_project_sqlite->GetDetailChartByIDs(nPageID, (*iter)->GetID(), pLineGraphic->GetItemVector());
						vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
						for (size_t i=0;i<vec.size();++i)
						{
							pLineGraphic->InsertGraphItem(vec[i]);
						}

						pInsertPage->AddLineGraphic(pLineGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawBarGraphic*>& barchartList = pPage->GetBarGraphList();
				list<CEqmDrawBarGraphic*>::iterator iter = barchartList.begin();
				while (iter != barchartList.end())
				{
					CEqmDrawBarGraphic* pBarGraphic = new CEqmDrawBarGraphic();
					if(pBarGraphic)
					{
						pBarGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pBarGraphic->SetEqmType((*iter)->GetEqmType());
						pBarGraphic->SetPageID(nID);
						pBarGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pBarGraphic->SetTitleName((*iter)->GetTitleName());
						pBarGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pBarGraphic->SetXName((*iter)->GetXName());
						pBarGraphic->SetXUnit((*iter)->GetXUnit());
						pBarGraphic->SetYName((*iter)->GetYName());
						pBarGraphic->SetWidth((*iter)->GetWidth());
						pBarGraphic->SetHeight((*iter)->GetHeight());
						pBarGraphic->SetType((*iter)->GetType());
						pBarGraphic->SetMaxValue((*iter)->GetMaxValue());
						pBarGraphic->SetStartPoint((*iter)->GetStartPoint());
						pBarGraphic->SetQueryType((*iter)->GetQueryType());
						pBarGraphic->SetTextColor((*iter)->GetTextColor());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
						if(pBitmap)
						{
							pBarGraphic->SetDefaultBitmap(pBitmap);
						}
						vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
						for (size_t i=0;i<vec.size();++i)
						{
							pBarGraphic->InsertGraphItem(vec[i]);
						}
						pInsertPage->AddBarGraphic(pBarGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawPieGraphic*>& piechartList = pPage->GetPieGraphList();
				list<CEqmDrawPieGraphic*>::iterator iter = piechartList.begin();
				while (iter != piechartList.end())
				{
					CEqmDrawPieGraphic* pPieGraphic = new CEqmDrawPieGraphic();
					if(pPieGraphic)
					{
						pPieGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pPieGraphic->SetEqmType((*iter)->GetEqmType());
						pPieGraphic->SetPageID(nID);
						pPieGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pPieGraphic->SetTitleName((*iter)->GetTitleName());
						pPieGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pPieGraphic->SetWidth((*iter)->GetWidth());
						pPieGraphic->SetHeight((*iter)->GetHeight());
						pPieGraphic->SetType((*iter)->GetType());
						pPieGraphic->SetTextColor((*iter)->GetTextColor());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartPie.png");
						if(pBitmap)
						{
							pPieGraphic->SetDefaultBitmap(pBitmap);
						}
						vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
						for (size_t i=0;i<vec.size();++i)
						{
							pPieGraphic->InsertGraphItem(vec[i]);
						}
						pInsertPage->AddPieGraphic(pPieGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawButton*>& buttonList = pPage->GetButtonList();
				list<CEqmDrawButton*>::iterator iter = buttonList.begin();
				while (iter != buttonList.end())
				{
					CEqmDrawButton* pButton = new CEqmDrawButton();
					if(pButton)
					{
						pButton->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pButton->SetEqmType((*iter)->GetEqmType());
						pButton->SetPageID(nID);
						pButton->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pButton->SetWidth((*iter)->GetWidth());
						pButton->SetHeight((*iter)->GetHeight());
						pButton->SetLayer((*iter)->GetLayer());
						int nComm = 0;
						int nDisable = 0;
						int nDown = 0;
						int nOver = 0;
						int nChecked = 0;
						(*iter)->Get5PicID(nComm,nDisable,nDown,nOver,nChecked);
						nComm = m_nID*E_IMAGE_BASE_OFFSET + nComm;
						nDisable = m_nID*E_IMAGE_BASE_OFFSET +nDisable;
						nDown = m_nID*E_IMAGE_BASE_OFFSET + nDown;
						nOver = m_nID*E_IMAGE_BASE_OFFSET + nOver;
						nChecked = m_nID*E_IMAGE_BASE_OFFSET + nChecked;
						pButton->Set5PicID(nComm,nDisable,nDown,nOver,nChecked);
						pButton->SetLinkPageID((*iter)->GetLinkPageID()+ m_nID*E_PAGE_BASE_OFFSET);
						pButton->SetOperation((*iter)->GetOperation());
						pButton->SetTextContent((*iter)->GetTextContent());
						pButton->SetTextColor((*iter)->GetTextColor());
						pButton->SetTextSize((*iter)->GetTextSize());
						pButton->SetBindSetPointName((*iter)->GetBindSetPointName());
						pButton->SetSettingValue((*iter)->GetSettingValue());
						pButton->SetRelation((*iter)->GetRelation());
						vector<_tagRelationExpress> vec = (*iter)->GetRelationVec();
						for(int i=0; i<vec.size(); i++)
						{
							pButton->InsertExpress(vec[i]);
						}
						pButton->SetDownloadEnable((*iter)->GetDownloadEnable());
						pButton->SetDownloadName((*iter)->GetDownloadName());

						Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nComm);
						if(pBitmap)
						{
							pButton->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddButton(pButton);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawTimeSelect*>& timeSelectList = pPage->GetTimeSelectList();
				list<CEqmDrawTimeSelect*>::iterator iter = timeSelectList.begin();
				while (iter != timeSelectList.end())
				{
					CEqmDrawTimeSelect* pTimeSelect = new CEqmDrawTimeSelect();
					if(pTimeSelect)
					{
						pTimeSelect->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pTimeSelect->SetEqmType((*iter)->GetEqmType());
						pTimeSelect->SetPageID(nID);
						pTimeSelect->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pTimeSelect->SetWidth((*iter)->GetWidth());
						pTimeSelect->SetHeight((*iter)->GetHeight());
						pTimeSelect->SetLayer((*iter)->GetLayer());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\timeSelect.png");
						if(pBitmap)
						{
							pTimeSelect->SetDefaultBitmap(pBitmap);
						}
						pTimeSelect->SetRWProperty((*iter)->GetRWProperty());
						pTimeSelect->SetPointName((*iter)->GetPointName());
						pTimeSelect->SetTimeDataFormat((*iter)->GetTimeDataFormat());
						pInsertPage->AddTimeSelect(pTimeSelect);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawTimeBar*>& timeBarList = pPage->GetTimeBarList();
				list<CEqmDrawTimeBar*>::iterator iter = timeBarList.begin();
				while (iter != timeBarList.end())
				{
					CEqmDrawTimeBar* pTimeBar = new CEqmDrawTimeBar();
					if(pTimeBar)
					{
						pTimeBar->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pTimeBar->SetEqmType((*iter)->GetEqmType());
						pTimeBar->SetPageID(nID);
						pTimeBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pTimeBar->SetWidth((*iter)->GetWidth());
						pTimeBar->SetHeight((*iter)->GetHeight());
						pTimeBar->SetLayer((*iter)->GetLayer());
						pTimeBar->SetBindPointStart((*iter)->GetBindPointStart());
						pTimeBar->SetQueryType((*iter)->GetQueryType());
						pTimeBar->SetBindPointStatus((*iter)->GetBindPointStatus());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
						if(pBitmap)
						{
							pTimeBar->SetDefaultBitmap(pBitmap);
						}
						pTimeBar->SetBarColor((*iter)->GetBarColor());
						const vector<_tag_bar_item>& vec = (*iter)->GetItemVec();
						const size_t size = vec.size();
						for (size_t i=0;i<size;++i)
						{
							pTimeBar->InsertItem(vec[i]);
						}
						pInsertPage->AddTimeBar(pTimeBar);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawProgressBar*>& ProgressBarList = pPage->GetProgressBarList();
				list<CEqmDrawProgressBar*>::iterator iter = ProgressBarList.begin();
				while (iter != ProgressBarList.end())
				{
					CEqmDrawProgressBar* pProgressBar = new CEqmDrawProgressBar();
					if(pProgressBar)
					{
						pProgressBar->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pProgressBar->SetEqmType((*iter)->GetEqmType());
						pProgressBar->SetPageID(nID);
						pProgressBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pProgressBar->SetWidth((*iter)->GetWidth());
						pProgressBar->SetHeight((*iter)->GetHeight());
						pProgressBar->SetLayer((*iter)->GetLayer());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\progressBar.png");
						if(pBitmap)
						{
							pProgressBar->SetDefaultBitmap(pBitmap);
						}
						pProgressBar->SetFactorValue((*iter)->GetFactorValue());
						pProgressBar->SetBarColor((*iter)->GetBarColor());
						pProgressBar->SetBindPoint((*iter)->GetBindPoint());
						pProgressBar->SetBindPointBase((*iter)->GetBindPointBase());
						pInsertPage->AddProgressBar(pProgressBar);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = pPage->GetDiagnoseList();
				list<CEqmDrawDiagnoseGraphic*>::iterator iter = DiagnoseGraphicList.begin();
				while (iter != DiagnoseGraphicList.end())
				{
					CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = new CEqmDrawDiagnoseGraphic();
					if(pDiagnoseGraphic)
					{
						pDiagnoseGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pDiagnoseGraphic->SetEqmType((*iter)->GetEqmType());
						pDiagnoseGraphic->SetPageID(nID);
						pDiagnoseGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pDiagnoseGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pDiagnoseGraphic->SetWidth((*iter)->GetWidth());
						pDiagnoseGraphic->SetHeight((*iter)->GetHeight());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartDiagnose.png");
						if(pBitmap)
						{
							pDiagnoseGraphic->SetDefaultBitmap(pBitmap);
						}

						vector<_tagDiagnoseItem>& vec = (*iter)->GetItemVec();
						for (size_t i=0;i<vec.size();++i)
						{
							pDiagnoseGraphic->InsertItem(vec[i]);
						}
						pInsertPage->AddDiagnoseGraphic(pDiagnoseGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawPolygon*>& pPolygonList = pPage->GetPolygon();
				list<CEqmDrawPolygon*>::iterator iter = pPolygonList.begin();
				while (iter != pPolygonList.end())
				{
					CEqmDrawPolygon* pPolygon = new CEqmDrawPolygon();
					if(pPolygon)
					{
						pPolygon->SetPointLIst((*iter)->GetPointList());
						pPolygon->SetEqmType((*iter)->GetEqmType());
						pPolygon->SetPageID(nID);
						pPolygon->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pPolygon->SetLayer((*iter)->GetLayer());
						pPolygon->SetTemplateGroupId((*iter)->GetTemplateGroupId());
						pPolygon->SetTemplatePageId((*iter)->GetTemplatePageId());
						pPolygon->SetTemplatePageName((*iter)->GetTemplatePageName());
						pPolygon->SetTemplateName((*iter)->GetTemplateName());
						pPolygon->SetParam((*iter)->GetParam());
						pPolygon->SetLineColor_PptsWnd((*iter)->GetLineColor_PptsWnd());
						pPolygon->SetLineWidth_PptsWnd((*iter)->GetLineWidth_PptsWnd());
						pPolygon->SetNeedSave(true);

						pInsertPage->AddPolygon(pPolygon);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawAreaChange*>& AreaChangeList = pPage->GetAreaChangeList();
				list<CEqmDrawAreaChange*>::iterator iter = AreaChangeList.begin();
				while (iter != AreaChangeList.end())
				{
					CEqmDrawAreaChange* pAreaChange = new CEqmDrawAreaChange();
					if(pAreaChange)
					{
						pAreaChange->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pAreaChange->SetEqmType((*iter)->GetEqmType());
						pAreaChange->SetPageID(nID);
						pAreaChange->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pAreaChange->SetWidth((*iter)->GetWidth());
						pAreaChange->SetHeight((*iter)->GetHeight());
						pAreaChange->SetLayer((*iter)->GetLayer());
						pAreaChange->SetPicID(m_nID*E_IMAGE_BASE_OFFSET + (*iter)->GetPicID());
						Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(pAreaChange->GetPicID());
						if(pBitmap)
							pAreaChange->SetBitmap(pBitmap);
						//pAreaChange->SetPicID((*iter)->GetPicID());
						vector<_tagAreaInfo> vecArea = (*iter)->GetAreaInfo();
						for(int j=0; j<vecArea.size(); j++)
						{
							vecArea[j].BnmID = m_nID*E_IMAGE_BASE_OFFSET + vecArea[j].BnmID;
							vecArea[j].nPageID = m_nID*E_PAGE_BASE_OFFSET + vecArea[j].nPageID;
						}

						pAreaChange->SetAreaInfo(vecArea);

						pInsertPage->AddAreaChange(pAreaChange);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawShellExecute*>& shellExecuteList = pPage->GetShellExecuteList();
				list<CEqmDrawShellExecute*>::iterator iter = shellExecuteList.begin();
				while (iter != shellExecuteList.end())
				{
					CEqmDrawShellExecute* pShellExecute = new CEqmDrawShellExecute();
					if(pShellExecute)
					{
						pShellExecute->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pShellExecute->SetEqmType(EQMDRAW_SHELL_EXECUTE);
						pShellExecute->SetPageID(nID);
						pShellExecute->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pShellExecute->SetWidth((*iter)->GetWidth());
						pShellExecute->SetHeight((*iter)->GetHeight());
						pShellExecute->SetLayer((*iter)->GetLayer());

						pShellExecute->SetServerIP((*iter)->GetServerIP());
						pShellExecute->SetFileName((*iter)->GetFileName());
						pShellExecute->SetDownloadDirectory((*iter)->GetDownloadDirectory());
						pShellExecute->SetFileID((*iter)->GetFileID());
						pShellExecute->SetBKPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetBKPicID()));
						pShellExecute->SetAutoDownload((*iter)->GetAutoDownload());

						Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetBKPicID());
						if(pBitmap)
						{
							pShellExecute->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddShellExecute(pShellExecute);
					}
					iter++;
				}
			}
			{
				list<CEqmDrawEmbededPage*>& embededpageList = pPage->GetEmbededPageList();
				list<CEqmDrawEmbededPage*>::iterator iter = embededpageList.begin();
				while (iter != embededpageList.end())
				{
					CEqmDrawEmbededPage* pEmbededPage = new CEqmDrawEmbededPage();
					pEmbededPage->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pEmbededPage->SetEqmType(EQMDRAW_EMBEDED_PAGE);
					pEmbededPage->SetPageID(nID);
					pEmbededPage->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pEmbededPage->SetWidth((*iter)->GetWidth());
					pEmbededPage->SetHeight((*iter)->GetHeight());
					pEmbededPage->SetLayer((*iter)->GetLayer());
					pEmbededPage->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));
					pEmbededPage->SetChangeInterval((*iter)->GetChangeInterval());
					Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetPicID());
					if(pBitmap)
					{
						pEmbededPage->SetDefaultBitmap(pBitmap);
					}
					vector<PageIDName>&	vec = (*iter)->GetPageList();
					for (size_t i=0;i<vec.size();++i)
					{
						pEmbededPage->InsertPageList(vec[i].nPageID, vec[i].strPageName);
					}
					pInsertPage->AddEmbededPage(pEmbededPage);
					++iter;
				}
			}
			{
				list<CEqmDrawLiquidLevel*>& liquidLevelList = pPage->GetLiquidLevelList();
				list<CEqmDrawLiquidLevel*>::iterator iter = liquidLevelList.begin();
				while (iter != liquidLevelList.end())
				{
					CEqmDrawLiquidLevel* pLiquidLevel = new CEqmDrawLiquidLevel();
					pLiquidLevel->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pLiquidLevel->SetEqmType(EQMDRAW_LIQUID_LEVEL);
					pLiquidLevel->SetPageID(nID);
					pLiquidLevel->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pLiquidLevel->SetWidth((*iter)->GetWidth());
					pLiquidLevel->SetHeight((*iter)->GetHeight());
					pLiquidLevel->SetLayer((*iter)->GetLayer());
					//pLiquidLevel->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));

					pLiquidLevel->SetBindPoint((*iter)->GetBindPoint());
					pLiquidLevel->SetMaxValue((*iter)->GetMaxValue());
					pLiquidLevel->SetMinValue((*iter)->GetMinValue());
					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\liquidLevel.png");
					if (pBitmap != NULL)
					{
						pLiquidLevel->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddLiquidLevel(pLiquidLevel);
					++iter;
				}
			}
			{
				list<CEqmDrawButtonPlus*>& chkBtnPlusList = pPage->GetCheckButtonPlusList();
				list<CEqmDrawButtonPlus*>::iterator iter = chkBtnPlusList.begin();
				while (iter != chkBtnPlusList.end())
				{
					CEqmDrawButtonPlus* pChkBtnPlus = new CEqmDrawButtonPlus();
					pChkBtnPlus->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pChkBtnPlus->SetEqmType(EQMDRAW_CHKBTN_PLUS);
					pChkBtnPlus->SetPageID(nID);
					pChkBtnPlus->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pChkBtnPlus->SetWidth((*iter)->GetWidth());
					pChkBtnPlus->SetHeight((*iter)->GetHeight());
					pChkBtnPlus->SetLayer((*iter)->GetLayer());

					pChkBtnPlus->SetTextColor((*iter)->GetTextColor());
					pChkBtnPlus->SetTextSize((*iter)->GetTextSize());
					pChkBtnPlus->SetTextContent((*iter)->GetTextContent());
					pChkBtnPlus->SetButtonStyle((*iter)->GetButtonStyle());
					pChkBtnPlus->SetValueForUnCheck((*iter)->GetValueForUnCheck());
					pChkBtnPlus->SetValueForCheck((*iter)->GetValueForCheck());
					pChkBtnPlus->SetBindPointName((*iter)->GetBindPointName());
					pChkBtnPlus->SetGroupID((*iter)->GetGroupID());
					pChkBtnPlus->SetExpression((*iter)->GetExpression());
					pChkBtnPlus->SetPathUncheckCom((*iter)->GetPathUncheckCom());
					pChkBtnPlus->SetPathUncheckOver((*iter)->GetPathUncheckOver());
					pChkBtnPlus->SetPathCheckCom((*iter)->GetPathCheckCom());
					pChkBtnPlus->SetPathCheckOver((*iter)->GetPathCheckOver());
					pChkBtnPlus->SetOptionsDec((*iter)->GetOptionsDec());
					Image* pBitmap = Image::FromFile((*iter)->GetPathCheckCom());
					if (pBitmap != NULL)
					{
						pChkBtnPlus->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddCheckButtonPlus(pChkBtnPlus);
					++iter;
				}
			}
			{
				list<CEqmDrawStraightLine*>& straightLineList = pPage->GetStraightLineList();
				list<CEqmDrawStraightLine*>::iterator iter = straightLineList.begin();
				while (iter != straightLineList.end())
				{
					CEqmDrawStraightLine* pStraightLine = new CEqmDrawStraightLine();
					pStraightLine->SetEqmType(EQMDRAW_LINE);
					pStraightLine->SetPageID(nID);
					pStraightLine->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pStraightLine->SetLayer((*iter)->GetLayer());
					pStraightLine->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pStraightLine->SetTerminalPointX((*iter)->GetTerminalPointX());
					pStraightLine->SetTerminalPointY((*iter)->GetTerminalPointY());

					pStraightLine->SetLineWidth(pStraightLine->GetLineWidth());
					pStraightLine->SetLineStyle(pStraightLine->GetLineStyle());
					pStraightLine->SetLineColor(pStraightLine->GetLineColor());
					pStraightLine->SetTransparency(pStraightLine->GetTransparency());

					pStraightLine->SetStartShape(pStraightLine->GetStartShape());
					pStraightLine->SetStartShapeSize(pStraightLine->GetStartShapeSize());
					pStraightLine->SetEndShape(pStraightLine->GetEndShape());
					pStraightLine->SetEndShapeSize(pStraightLine->GetEndShapeSize());

					pInsertPage->AddStraightLine(pStraightLine);
					++iter;
				}
			}
			{
				list<CEqmDrawHistoryTrend*>& historyTrendList = pPage->GetHistoryTrendList();
				list<CEqmDrawHistoryTrend*>::iterator iter = historyTrendList.begin();
				while (iter != historyTrendList.end())
				{
					CEqmDrawHistoryTrend* pHistoryTrend = new CEqmDrawHistoryTrend();
					pHistoryTrend->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pHistoryTrend->SetEqmType(EQMDRAW_HISTORY_TREND);
					pHistoryTrend->SetPageID(nID);
					pHistoryTrend->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pHistoryTrend->SetWidth((*iter)->GetWidth());
					pHistoryTrend->SetHeight((*iter)->GetHeight());
					pHistoryTrend->SetLayer((*iter)->GetLayer());

					pHistoryTrend->SetHistoryTrendType((*iter)->GetHistoryTrendType());
					pHistoryTrend->SetHistoryTrendVector((*iter)->GetHistoryTrendVector());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\historyTrend.png");
					if (pBitmap != NULL)
					{
						pHistoryTrend->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddHistoryTrend(pHistoryTrend);
					++iter;
				}
			}
			{
				list<CEqmDrawRectangle*>& rectangleList = pPage->GetRectangleList();
				list<CEqmDrawRectangle*>::iterator iter = rectangleList.begin();
				while (iter != rectangleList.end())
				{
					CEqmDrawRectangle* pRectangle = new CEqmDrawRectangle();
					pRectangle->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pRectangle->SetEqmType(EQMDRAW_RECTANGLE);
					pRectangle->SetPageID(nID);
					pRectangle->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pRectangle->SetWidth((*iter)->GetWidth());
					pRectangle->SetHeight((*iter)->GetHeight());
					pRectangle->SetLayer((*iter)->GetLayer());
					pRectangle->SetLineColor((*iter)->GetLineColor());
					pRectangle->SetLineWidth((*iter)->GetLineWidth());
					pRectangle->SetFillStyl((*iter)->GetFillStyl());
					pRectangle->SetFillColor((*iter)->GetFillColor());
					pRectangle->SetToRoundRect((*iter)->GetToRoundRect());
					pRectangle->SetRoundHeight((*iter)->GetRoundWidth());
					pRectangle->SetRoundWidth((*iter)->GetRoundWidth());
					pInsertPage->AddRectangle(pRectangle);
					++iter;
				}
			}
			{
				list<CEqmDrawMapNavigate*>& mapNavigateList = pPage->GetMapNavigateList();
				list<CEqmDrawMapNavigate*>::iterator iter = mapNavigateList.begin();
				while (iter != mapNavigateList.end())
				{
					CEqmDrawMapNavigate* pMapNavigate = new CEqmDrawMapNavigate();
					pMapNavigate->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pMapNavigate->SetEqmType(EQMDRAW_MAP_NAVIGATE);
					pMapNavigate->SetPageID(nID);
					pMapNavigate->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pMapNavigate->SetWidth((*iter)->GetWidth());
					pMapNavigate->SetHeight((*iter)->GetHeight());
					pMapNavigate->SetLayer((*iter)->GetLayer());
					pMapNavigate->SetBanOnDisplay((*iter)->GetBanOnDisplay());

					pMapNavigate->SetScaleRuler((*iter)->GetScaleRuler());
					pMapNavigate->SetMapPageInfo((*iter)->GetMapPageInfo());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\MapNavigate.png");
					if (pBitmap != NULL)
					{
						pMapNavigate->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddMapNavigate(pMapNavigate);
					++iter;
				}
			}
			{
				list<CEqmDrawNavigateWindow*>& naviWindList = pPage->GetNavigateWindowList();
				list<CEqmDrawNavigateWindow*>::iterator iter = naviWindList.begin();
				while (iter != naviWindList.end())
				{
					CEqmDrawNavigateWindow* pNaviWind = new CEqmDrawNavigateWindow();
					pNaviWind->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pNaviWind->SetEqmType(EQMDRAW_NAVIGATE_WINDOW);
					pNaviWind->SetPageID(nID);
					pNaviWind->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pNaviWind->SetWidth((*iter)->GetWidth());
					pNaviWind->SetHeight((*iter)->GetHeight());
					pNaviWind->SetLayer((*iter)->GetLayer());

					pNaviWind->SetPageInfo((*iter)->GetPageInfo());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\NavigateWindow.png");
					if (pBitmap != NULL)
					{
						pNaviWind->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddNavigateWindow(pNaviWind);
					++iter;
				}
			}
			{
				list<CEqmDrawLevelRuler*>& levelRulerList = pPage->GetLevelRulerList();
				list<CEqmDrawLevelRuler*>::iterator iter = levelRulerList.begin();
				while (iter != levelRulerList.end())
				{
					CEqmDrawLevelRuler* pLevelRuler = new CEqmDrawLevelRuler();
					pLevelRuler->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pLevelRuler->SetEqmType(EQMDRAW_LEVEL_RULER);
					pLevelRuler->SetPageID(nID);
					pLevelRuler->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pLevelRuler->SetWidth((*iter)->GetWidth());
					pLevelRuler->SetHeight((*iter)->GetHeight());
					pLevelRuler->SetLayer((*iter)->GetLayer());

					pLevelRuler->SetName((*iter)->GetName());
					pLevelRuler->SetMax((*iter)->GetMax());
					pLevelRuler->SetMin((*iter)->GetMin());
					pLevelRuler->SetMainScaleInterval((*iter)->GetMainScaleInterval());
					pLevelRuler->SetMinorScaleInterval((*iter)->GetMinorScaleInterval());
					pLevelRuler->SetDecimalPlace((*iter)->GetDecimalPlace());
					pLevelRuler->SetLabelInfo((*iter)->GetLabelInfo());
					pLevelRuler->SetSectionInfo((*iter)->GetSectionInfo());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\LevelRuler.png");
					if (pBitmap != NULL)
					{
						pLevelRuler->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddLevelRuler(pLevelRuler);
					++iter;
				}
			}
			{
				list<CEqmDrawDataAnalyse*>& dataAnalyseList = pPage->GetDataAnalyseList();
				list<CEqmDrawDataAnalyse*>::iterator iter = dataAnalyseList.begin();
				while (iter != dataAnalyseList.end())
				{
					CEqmDrawDataAnalyse* pDataAnalyse = new CEqmDrawDataAnalyse();
					pDataAnalyse->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pDataAnalyse->SetEqmType(EQMDRAW_DATA_ANALYSE);
					pDataAnalyse->SetPageID(nID);
					pDataAnalyse->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					pDataAnalyse->SetWidth((*iter)->GetWidth());
					pDataAnalyse->SetHeight((*iter)->GetHeight());
					pDataAnalyse->SetLayer((*iter)->GetLayer());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataAnalyse.png");
					if (pBitmap != NULL)
					{
						pDataAnalyse->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddDataAnalyse(pDataAnalyse);
					++iter;
				}
			}
			{
				list<CEqmDrawTemperDistri*>& listContain = pPage->GetTemperDistriList();
				list<CEqmDrawTemperDistri*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawTemperDistri* p = new CEqmDrawTemperDistri();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_TEMPER_DISTRI);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetTemperDistri((*iter)->GetTemperDistri());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\TemperDistri.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddTemperDistri(p);
					++iter;
				}
			}
			{
				list<CEqmDrawDataReport*>& listContain = pPage->GetDataReportList();
				list<CEqmDrawDataReport*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawDataReport* p = new CEqmDrawDataReport();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_DATA_REPORT);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetPath((*iter)->GetPath());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataReport.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddDataReport(p);
					++iter;
				}
			}
			{
				list<CEqmDrawPlaneTempeDistr*>& listContain = pPage->GetPlaneTempeDistrList();
				list<CEqmDrawPlaneTempeDistr*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawPlaneTempeDistr* p = new CEqmDrawPlaneTempeDistr();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_PLANE_TEMPE_DISTR);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetPointName((*iter)->GetPointName());
					p->SetConfigType((*iter)->GetConfigType());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\PlaneTempeDistr.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddPlaneTempeDistr(p);
					++iter;
				}
			}
			{
				list<CEqmDrawDottedLineFrm*>& listContain = pPage->GetDottedLineFrmList();
				list<CEqmDrawDottedLineFrm*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawDottedLineFrm* p = new CEqmDrawDottedLineFrm();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_DOTLINE_FRAME);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());
					pInsertPage->AddDottedLineFrm(p);
					++iter;
				}
			}
			{
				list<CEqmDrawGeneralComponent*>& listContain = pPage->GetGeneralComponentList();
				list<CEqmDrawGeneralComponent*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawGeneralComponent* p = new CEqmDrawGeneralComponent();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_GENERAL_COMPONENT);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetConfigValue((*iter)->GetConfigValue());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\GeneralComponent.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddGeneralComponent(p);
					++iter;

				}
			}
			pInsertPage->SetID(nID);
			//CString strItemText = pView->m_pTreeDlg->m_treectrl.GetItemText(m_hItem);
			CString strItemText = pFrame->m_wndFileView.m_wndFileView.GetItemText(m_hItem);
			const int nGroupID = pFrame->m_project.GetGroupIDByGroupName(strItemText.GetString());
			pInsertPage->SetGroupID(nGroupID);
			pFrame->m_DrawContext.pPage = pInsertPage;
			pFrame->m_DrawContext.pPage->SetGroupID(nGroupID);
			pInsertPage->m_bDetailLoaded = true;
			pInsertPage->SetPageShowTopInObserver(pPage->GetPageShowTopInObserver());
			pFrame->m_project.AddPage(pInsertPage);
			nPageNum ++;
			strOut.Format(_T("已导入第%d个页面:%s"),nPageNum,pInsertPage->GetPageName().c_str());
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
			pFrame->m_MaxPageID++;
		}
	}
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("导入完成"));
	EnableControl(true);
}

void CImportPageSelectDlg::ImportSelectPages_CurrentPageResource()
{
	bool bSelectPage = false;
	for (int i=0;i<m_list_page.GetItemCount();++i)
	{
		if (m_list_page.GetCheck(i))
		{
			bSelectPage = true;
			break;
		}
	}

	if(!bSelectPage)
	{
		MessageBox(_T("请选择导入页面！"));
		return;
	}

	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());
	ASSERT(pView);

	const int nMaxPicID = m_nMaxPicID;
	const int nCount = m_list_page.GetItemCount();
	m_vecNewPicID.clear();
	int nPageNum = 0;
	CString strOut;
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("开始导入页面，请稍候..."));
	EnableControl(false);

	if(!m_bResourceChanged)
	{
		GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("正在初始化图片资源..."));
		m_project_sqlite->LoadPictureLibSqlite();
		m_project_sqlite->LoadAnimationSqlite();
		if (!(pFrame->m_bLoadIndependent))
		{
			pFrame->m_pPrjSqlite->LoadPictureLibSqlite();
			pFrame->m_pPrjSqlite->LoadAnimationSqlite();
			pFrame->m_pPrjSqlite->LoadPictureLibSqlite(false);
			pFrame->m_pPrjSqlite->LoadAnimationSqlite(false);
			pFrame->m_bLoadIndependent = true;
		}
		m_bResourceChanged = true;
		m_vecImportPageID.clear();
		m_vecNewPicID.clear();
		m_vecNewAniID.clear();
	}

	int nPageMaxOrder = pFrame->m_project.GetMaxPageShowOrder() + 1;
	int nIDBasement = nPageMaxOrder;
	int nElementid = pFrame->m_pPrjSqlite->Get_devices_max_id()+1;//获取最大“自定义图元ID”
	for (int i=0;i<nCount;++i)
	{
		if (m_list_page.GetCheck(i))
		{
			const int nPageID = _wtoi(m_list_page.GetItemText(i,1));
			CEqmDrawPage* pPage = m_project.GetPageByID(nPageID);//拿到页面类

			int nID =  nIDBasement+ IMPORT_PAGE_ELEMENT_ID_START + nPageID;//如果
			const int nPageValAdd = nID - nPageID;
			/*HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(m_list_page.GetItemText(i,2), m_hItem, nID);
			pView->m_pTreeDlg->m_treectrl.Expand(m_hItem, TVE_EXPAND);
			pView->m_pTreeDlg->m_treectrl.SetFocus();
			pView->m_pTreeDlg->m_treectrl.SelectItem(hPage);*/
			
			//页面树状列表插入新的
			HTREEITEM hPage = pFrame->m_wndFileView.InsertPage(m_list_page.GetItemText(i,2), m_hItem, nID);
			pFrame->m_wndFileView.m_wndFileView.Expand(m_hItem, TVE_EXPAND);
			pFrame->m_wndFileView.m_wndFileView.SetFocus();
			pFrame->m_wndFileView.m_wndFileView.SelectItem(hPage);


			strOut.Format(_T("正在导入页面:%s"),pPage->GetPageName().c_str());
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
			if(!pPage->m_bDetailLoaded)
				m_project_sqlite->LoadPageDetails(pPage,NULL,NULL);
			m_vecImportPageID.push_back(nID);//导入的页面暂存，暂不清楚用途
			CEqmDrawPage* pInsertPage = new CEqmDrawPage(nID,pPage->GetPageName(),pPage->GetProjectID(),nPageMaxOrder,pPage->GetPageType(),pPage->GetPageWidth(),pPage->GetPageHeight(),pPage->GetPagexPosition(),pPage->GetPageyPosition(),pPage->GetStartColor());

			// insert background image
			int nBkImgIdNew = GetNewPicIDAndInsertIntoLib(nMaxPicID, pPage->GetBKImageID());
			pInsertPage->SetBKImageID(nBkImgIdNew);
			Image* pImage = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nBkImgIdNew, true);
			pInsertPage->SetBkImage(pImage);

			++nPageMaxOrder;
			//插入自定义图元
			{
				list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
				list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
				bool bIsPic = true;
				int nNewId = 0;
				
				while (iter != deviceList.end())
				{
					CEqmDrawDevice* pDevice = new CEqmDrawDevice((*iter)->GetEqmType(), Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetDataComID(), (*iter)->GetRotateAngle(), (*iter)->GetEqmName(), (*iter)->GetEquipmentSubType(), (*iter)->GetWidth(), "", (*iter)->GetHeight(), 0.0f, 0.0f, 0.0f,(*iter)->GetBindType());
					ASSERT(pDevice);

					pDevice->SetID(nElementid++);
					pDevice->SetLayer((*iter)->GetLayer());
					nNewId = GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID());
					pDevice->SetPicID(nNewId);
					pDevice->SetPhysicalID(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()));
					pDevice->SetDeviceValue((*iter)->GetDeviceValue());
					bIsPic = (*iter)->GetImageMetaType();
					pDevice->SetImageMetaType(bIsPic);

					//插入动画显示图片
					if (!bIsPic)
					{
						const AnimationInfo* pAniInf = m_project_sqlite->GetAnimationByID((*iter)->GetPicID(), true);
						if (pAniInf != NULL)
						{
							int nUseId = 0;
							if (pFrame->m_pPrjSqlite->InsertAnimationIntoOpenS3db(pAniInf, nUseId))
							{
								if (nNewId != nUseId)
								{
									pDevice->SetPicID(nUseId);
								}
							}

							Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIdFromAnimationImageLib(nUseId, true);
							if (pBitmap != NULL)
							{
								pDevice->SetBitmap(pBitmap);
							}
						}
					}

					//插入事件图片
					vector<BindInfo> vecBind = (*iter)->GetBindInfoVec();
					for(int i=0; i<vecBind.size(); ++i)
					{
						if(vecBind[i].nFrame >1)  //动画
						{
							vecBind[i].nPicID = GetNewAnimationIDAndInsertIntoLib(m_nMaxAniID,vecBind[i].nPicID);
						}
						else
						{
							vecBind[i].nPicID = GetNewPicIDAndInsertIntoLib(nMaxPicID,vecBind[i].nPicID);
						}
					}

					//pDevice->m_vecBindInfo = vecBind;
					CString strParam10 = _T("");
					for (int i=0;i<vecBind.size();++i)
					{
						CString strOut;
						strOut.Format(_T("%d,%d,%d,%d"),vecBind[i].nValue,vecBind[i].nPicID,vecBind[i].nFrame,vecBind[i].nInterval);
						strParam10 += strOut;
						if (i != vecBind.size()-1)
						{
							strParam10 += L"|";
						}
					}	
					pDevice->ParseParam10(strParam10);

					pDevice->m_eventInfo = (*iter)->m_eventInfo;

					// import page link
					pDevice->m_eventInfo.nPageID += nPageValAdd;
					//wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->m_eventInfo.nPageID);
					//if (wstrPageName != L"")
					//{
					//	int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
					//	if (-1 != nPageId)
					//	{
					//		pDevice->m_eventInfo.nPageID = nPageId;
					//	}
					//}
					//

					pDevice->SetLinkPageID((*iter)->GetLinkPageID());
					if (bIsPic)
					{	// 图片图元
						Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(pDevice->GetPicID());
						if(pBitmap)
							pDevice->SetBitmap(pBitmap);
					}
					pInsertPage->AddDevice(pDevice);
					iter++;
				}
			}
			//插入管道
			{
				list<CEqmDrawPipe*>& deviceList = pPage->GetPagePipeList();
				list<CEqmDrawPipe*>::iterator iter = deviceList.begin();
				while (iter != deviceList.end())
				{
					CEqmDrawPipe* pPipe = new CEqmDrawPipe( nID, (*iter)->GetID(), (*iter)->GetPipeColor(), (*iter)->GetPipeWidth(), (*iter)->GetPointStringList() ); //8.9
					ASSERT(pPipe);
					pPipe->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pPipe->SetLayer((*iter)->GetLayer());
					pPipe->SetDirection((*iter)->GetDirection());
					pPipe->SetDataPointList((*iter)->GetDataPointList());
					pPipe->SetPointLIst((*iter)->GetPointList());
					pPipe->SetDataComID((*iter)->GetDataComIDVector());
					pPipe->SetPipeColor((*iter)->GetPipeColor());
					pPipe->SetPipeWidth((*iter)->GetPipeWidth());
					pPipe->SetPipeType((*iter)->GetPipeType());
					pPipe->SetPipeDenSity((*iter)->GetPipeDenSity());
					pPipe->SetPipeSpeed((*iter)->GetPipeSpeed());
					pPipe->SetWaterFashion((*iter)->GetWaterFashion());
					pInsertPage->AddPipe(pPipe);
					iter++;
				}
			}
			{
				list<CEqmDrawText*>& textList = pPage->GetPageTextList();
				list<CEqmDrawText*>::iterator iter = textList.begin();
				while (iter != textList.end())
				{
					CEqmDrawText* pText = new CEqmDrawText(Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetWidth(), 
						(*iter)->GetHeight(), (*iter)->GetFontSize(), (*iter)->GetFontName(), CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()),(*iter)->GetShowString(), (*iter)->GetFontColor(),
						(*iter)->GetTextShowMode(),(*iter)->GetTransparence(),(*iter)->GetBorderType(),(*iter)->GetBorderColor(), (*iter)->GetDeviceCfg(), (*iter)->GetFaultDiagnosis(),(*iter)->GetBindScript());
					ASSERT(pText);
					pText->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pText->SetLayer((*iter)->GetLayer());
					pText->SetAlign((*iter)->GetAlign());
					pText->SetDateTime((*iter)->GetDateTime());
					int nPointNum = (*iter)->GetPointNum();
					if(nPointNum>4 || nPointNum<0)
						nPointNum = 1;
					pText->SetPointNum(nPointNum);
					pText->SetTextModeShowString((*iter)->GetTextModeShowString());
					pText->SetTextBKType((*iter)->GetTextBKType());
					pText->SetTextAttribute((*iter)->GetTextAttribute());
					pText->SetUnit((*iter)->GetUnit());
					pInsertPage->AddText(pText);
					iter++;
				}
			}
			{
				list<CEqmDrawGraphic*>& graphicList = pPage->Get2DGraphicList();
				list<CEqmDrawGraphic*>::iterator iter = graphicList.begin();
				while (iter != graphicList.end())
				{
					CEqmDrawGraphic* pgrapic = NULL;
					EQMDRAWTYPE type = (*iter)->GetEqmType();
					switch(type)
					{
					case GraphicType_LINE:		//直线
						pgrapic = new CEqmDrawGraphicLine(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetFilledColor(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
						break;
					case GraphicType_RECTANGLE:	//矩形
						pgrapic = new CEqmDrawGraphicRect(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(),(*iter)->GetTransparent(), (*iter)->GetWidth(), (*iter)->GetHeight(),(*iter)->GetBorderIs3D());
						break;
					case GraphicType_ELLIPSE:	//椭圆
						pgrapic = new CEqmDrawGraphicEllipse(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
						break;
					case GraphicType_POLYGON:	//多边形
						pgrapic = new CEqmDrawGraphicPolygon(
							(EQMDRAWTYPE)type, Gdiplus::PointF(0,0), nID,  (*iter)->GetID(), (*iter)->GetLineColor(), (*iter)->GetFilledColor(), (*iter)->GetLineWidth(), (*iter)->GetLineStyle(), 
							(*iter)->GetPointStringList(), (*iter)->GetForwardPageID(), (*iter)->GetWidth(), (*iter)->GetHeight());
						break;
					case GraphicType_TRIANGLE:
						break;
					default:
						ASSERT(false);
						break;
					}

					ASSERT(pgrapic);
					pgrapic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pgrapic->SetLayer((*iter)->GetLayer());
					pInsertPage->Add2DGraphic(pgrapic);
					iter++;
				}
			}
			{
				list<CEqmDrawBend*>& bendList = pPage->GetBendList();
				list<CEqmDrawBend*>::iterator iter = bendList.begin();
				while (iter != bendList.end())
				{
					CEqmDrawBend* pBend = new CEqmDrawBend();
					if(pBend)
					{
						pBend->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pBend->SetEqmType((*iter)->GetEqmType());
						pBend->SetPageID(nID);
						pBend->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pBend->SetWidth((*iter)->GetWidth());
						pBend->SetHeight((*iter)->GetHeight());
						pBend->SetRotateAngle((*iter)->GetRotateAngle());
						pBend->SetLayer((*iter)->GetLayer());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\bend.png");
						if(pBitmap)
						{
							pBend->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddBend(pBend);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawDashBoard*>& dashboardList = pPage->GetDashboardList();
				list<CEqmDrawDashBoard*>::iterator iter = dashboardList.begin();
				while (iter != dashboardList.end())
				{
					CEqmDrawDashBoard* pDashBoard = new CEqmDrawDashBoard();
					if(pDashBoard)
					{
						pDashBoard->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pDashBoard->SetEqmType((*iter)->GetEqmType());
						pDashBoard->SetPageID(nID);
						pDashBoard->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pDashBoard->SetWidth((*iter)->GetWidth());
						pDashBoard->SetHeight((*iter)->GetHeight());
						pDashBoard->SetStyle((*iter)->GetStyle());
						pDashBoard->SetMaxValue((*iter)->GetMaxValue());
						pDashBoard->SetMinValue((*iter)->GetMinValue());
						pDashBoard->SetBindPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointName().GetString()).c_str());
						pDashBoard->SetMaxString((*iter)->GetMaxString());
						pDashBoard->SetMinString((*iter)->GetMinString());
						pDashBoard->SetMaxBindFlag((*iter)->GetMaxBindFlag());
						pDashBoard->SetMinBindFlag((*iter)->GetMinBindFlag());
						pDashBoard->SetFontColor((*iter)->GetFontColor());
						pDashBoard->SetDecimalPlaces((*iter)->GetDecimalPlaces());
						CString	strImgPath;
						if ((*iter)->GetStyle() == 0)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_0.png";
						}
						else if ((*iter)->GetStyle() == 1)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_1.png";
						}
						else if ((*iter)->GetStyle() == 2)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_2.png";
						}
						else if ((*iter)->GetStyle() == 3)
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_3.png";
						}
						else
						{
							strImgPath = m_strSysPath + L"\\Images\\meterpan_0.png";
						}
						Image* pBitmap = Image::FromFile(strImgPath);
						if(pBitmap)
						{
							pDashBoard->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddDashboard(pDashBoard);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawLineGraphic*>& linechartList = pPage->GetLineGraphList();
				list<CEqmDrawLineGraphic*>::iterator iter = linechartList.begin();
				while (iter != linechartList.end())
				{
					CEqmDrawLineGraphic* pLineGraphic = new CEqmDrawLineGraphic();
					if(pLineGraphic)
					{
						pLineGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pLineGraphic->SetEqmType((*iter)->GetEqmType());
						pLineGraphic->SetPageID(nID);
						pLineGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pLineGraphic->SetTitleName((*iter)->GetTitleName());
						pLineGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pLineGraphic->SetXName((*iter)->GetXName());
						pLineGraphic->SetXUnit((*iter)->GetXUnit());
						pLineGraphic->SetYName((*iter)->GetYName());
						pLineGraphic->SetWidth((*iter)->GetWidth());
						pLineGraphic->SetHeight((*iter)->GetHeight());
						pLineGraphic->SetType((*iter)->GetType());
						pLineGraphic->SetMaxValue((*iter)->GetMaxValue());
						pLineGraphic->SetTextColor((*iter)->GetTextColor());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartLine.png");
						if(pBitmap)
						{
							pLineGraphic->SetDefaultBitmap(pBitmap);
						}
						//m_project_sqlite->GetDetailChartByIDs(nPageID, (*iter)->GetID(), pLineGraphic->GetItemVector());
						vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
						for (size_t i=0;i<vec.size();++i)
						{
							pLineGraphic->InsertGraphItem(vec[i]);
						}
						pInsertPage->AddLineGraphic(pLineGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawBarGraphic*>& barchartList = pPage->GetBarGraphList();
				list<CEqmDrawBarGraphic*>::iterator iter = barchartList.begin();
				while (iter != barchartList.end())
				{
					CEqmDrawBarGraphic* pBarGraphic = new CEqmDrawBarGraphic();
					if(pBarGraphic)
					{
						pBarGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pBarGraphic->SetEqmType((*iter)->GetEqmType());
						pBarGraphic->SetPageID(nID);
						pBarGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pBarGraphic->SetTitleName((*iter)->GetTitleName());
						pBarGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pBarGraphic->SetXName((*iter)->GetXName());
						pBarGraphic->SetXUnit((*iter)->GetXUnit());
						pBarGraphic->SetYName((*iter)->GetYName());
						pBarGraphic->SetWidth((*iter)->GetWidth());
						pBarGraphic->SetHeight((*iter)->GetHeight());
						pBarGraphic->SetType((*iter)->GetType());
						pBarGraphic->SetMaxValue((*iter)->GetMaxValue());
						pBarGraphic->SetStartPoint((*iter)->GetStartPoint());
						pBarGraphic->SetQueryType((*iter)->GetQueryType());
						pBarGraphic->SetTextColor((*iter)->GetTextColor());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
						if(pBitmap)
						{
							pBarGraphic->SetDefaultBitmap(pBitmap);
						}
						vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
						for (size_t i=0;i<vec.size();++i)
						{
							pBarGraphic->InsertGraphItem(vec[i]);
						}
						pInsertPage->AddBarGraphic(pBarGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawPieGraphic*>& piechartList = pPage->GetPieGraphList();
				list<CEqmDrawPieGraphic*>::iterator iter = piechartList.begin();
				while (iter != piechartList.end())
				{
					CEqmDrawPieGraphic* pPieGraphic = new CEqmDrawPieGraphic();
					if(pPieGraphic)
					{
						pPieGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pPieGraphic->SetEqmType((*iter)->GetEqmType());
						pPieGraphic->SetPageID(nID);
						pPieGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pPieGraphic->SetTitleName((*iter)->GetTitleName());
						pPieGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pPieGraphic->SetWidth((*iter)->GetWidth());
						pPieGraphic->SetHeight((*iter)->GetHeight());
						pPieGraphic->SetType((*iter)->GetType());
						pPieGraphic->SetTextColor((*iter)->GetTextColor());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartPie.png");
						if(pBitmap)
						{
							pPieGraphic->SetDefaultBitmap(pBitmap);
						}
						vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
						for (size_t i=0;i<vec.size();++i)
						{
							pPieGraphic->InsertGraphItem(vec[i]);
						}
						pInsertPage->AddPieGraphic(pPieGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawButton*>& buttonList = pPage->GetButtonList();
				list<CEqmDrawButton*>::iterator iter = buttonList.begin();
				while (iter != buttonList.end())
				{
					CEqmDrawButton* pButton = new CEqmDrawButton();
					if(pButton)
					{
						pButton->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pButton->SetEqmType((*iter)->GetEqmType());
						pButton->SetPageID(nID);
						pButton->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pButton->SetWidth((*iter)->GetWidth());
						pButton->SetHeight((*iter)->GetHeight());
						pButton->SetLayer((*iter)->GetLayer());
						int nComm = 0;
						int nDisable = 0;
						int nDown = 0;
						int nOver = 0;
						int nChecked = 0;
						(*iter)->Get5PicID(nComm,nDisable,nDown,nOver,nChecked);
						nComm = GetNewPicIDAndInsertIntoLib(nMaxPicID,nComm);
						nDisable = GetNewPicIDAndInsertIntoLib(nMaxPicID,nDisable);
						nDown = GetNewPicIDAndInsertIntoLib(nMaxPicID,nDown);
						nOver = GetNewPicIDAndInsertIntoLib(nMaxPicID,nOver);
						nChecked = GetNewPicIDAndInsertIntoLib(nMaxPicID,nChecked);
						pButton->Set5PicID(nComm,nDisable,nDown,nOver,nChecked);
						pButton->SetOperation((*iter)->GetOperation());
						pButton->SetTextContent((*iter)->GetTextContent());
						pButton->SetTextColor((*iter)->GetTextColor());
						pButton->SetTextSize((*iter)->GetTextSize());
						pButton->SetBindSetPointName((*iter)->GetBindSetPointName());
						pButton->SetSettingValue((*iter)->GetSettingValue());
						pButton->SetRelation((*iter)->GetRelation());
						pButton->SetDownloadEnable((*iter)->GetDownloadEnable());
						pButton->SetDownloadName((*iter)->GetDownloadName());

						// import page link
						pButton->SetLinkPageID((*iter)->GetLinkPageID() + nPageValAdd);
						//wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->GetLinkPageID());
						//if (wstrPageName != L"")
						//{
						//	int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
						//	if (-1 != nPageId)
						//	{
						//		pButton->SetLinkPageID(nPageId);
						//	}
						//}

						vector<_tagRelationExpress> vec = (*iter)->GetRelationVec();
						for(int i=0; i<vec.size(); i++)
						{
							pButton->InsertExpress(vec[i]);
						}
						Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nComm);
						if(pBitmap)
						{
							pButton->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddButton(pButton);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawTimeSelect*>& timeSelectList = pPage->GetTimeSelectList();
				list<CEqmDrawTimeSelect*>::iterator iter = timeSelectList.begin();
				while (iter != timeSelectList.end())
				{
					CEqmDrawTimeSelect* pTimeSelect = new CEqmDrawTimeSelect();
					if(pTimeSelect)
					{
						pTimeSelect->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pTimeSelect->SetEqmType((*iter)->GetEqmType());
						pTimeSelect->SetPageID(nID);
						pTimeSelect->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pTimeSelect->SetWidth((*iter)->GetWidth());
						pTimeSelect->SetHeight((*iter)->GetHeight());
						pTimeSelect->SetLayer((*iter)->GetLayer());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\timeSelect.png");
						if(pBitmap)
						{
							pTimeSelect->SetDefaultBitmap(pBitmap);
						}
						pTimeSelect->SetRWProperty((*iter)->GetRWProperty());
						pTimeSelect->SetTimeDataFormat((*iter)->GetTimeDataFormat());
						pInsertPage->AddTimeSelect(pTimeSelect);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawTimeBar*>& timeBarList = pPage->GetTimeBarList();
				list<CEqmDrawTimeBar*>::iterator iter = timeBarList.begin();
				while (iter != timeBarList.end())
				{
					CEqmDrawTimeBar* pTimeBar = new CEqmDrawTimeBar();
					if(pTimeBar)
					{
						pTimeBar->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pTimeBar->SetEqmType((*iter)->GetEqmType());
						pTimeBar->SetPageID(nID);
						pTimeBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pTimeBar->SetWidth((*iter)->GetWidth());
						pTimeBar->SetHeight((*iter)->GetHeight());
						pTimeBar->SetLayer((*iter)->GetLayer());
						pTimeBar->SetBindPointStart((*iter)->GetBindPointStart());
						pTimeBar->SetQueryType((*iter)->GetQueryType());
						pTimeBar->SetBindPointStatus((*iter)->GetBindPointStatus());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartBar.png");
						if(pBitmap)
						{
							pTimeBar->SetDefaultBitmap(pBitmap);
						}
						pTimeBar->SetBarColor((*iter)->GetBarColor());
						const vector<_tag_bar_item>& vec = (*iter)->GetItemVec();
						const size_t size = vec.size();
						for (size_t i=0;i<size;++i)
						{
							pTimeBar->InsertItem(vec[i]);
						}
						pInsertPage->AddTimeBar(pTimeBar);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawProgressBar*>& ProgressBarList = pPage->GetProgressBarList();
				list<CEqmDrawProgressBar*>::iterator iter = ProgressBarList.begin();
				while (iter != ProgressBarList.end())
				{
					CEqmDrawProgressBar* pProgressBar = new CEqmDrawProgressBar();
					if(pProgressBar)
					{
						pProgressBar->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pProgressBar->SetEqmType((*iter)->GetEqmType());
						pProgressBar->SetPageID(nID);
						pProgressBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pProgressBar->SetWidth((*iter)->GetWidth());
						pProgressBar->SetHeight((*iter)->GetHeight());
						pProgressBar->SetLayer((*iter)->GetLayer());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\progressBar.png");
						if(pBitmap)
						{
							pProgressBar->SetDefaultBitmap(pBitmap);
						}
						pProgressBar->SetFactorValue((*iter)->GetFactorValue());
						pProgressBar->SetBarColor((*iter)->GetBarColor());
						pInsertPage->AddProgressBar(pProgressBar);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = pPage->GetDiagnoseList();
				list<CEqmDrawDiagnoseGraphic*>::iterator iter = DiagnoseGraphicList.begin();
				while (iter != DiagnoseGraphicList.end())
				{
					CEqmDrawDiagnoseGraphic* pDiagnoseGraphic = new CEqmDrawDiagnoseGraphic();
					if(pDiagnoseGraphic)
					{
						pDiagnoseGraphic->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pDiagnoseGraphic->SetEqmType((*iter)->GetEqmType());
						pDiagnoseGraphic->SetPageID(nID);
						pDiagnoseGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pDiagnoseGraphic->SetUpdateInterval((*iter)->GetUpdateInterval());
						pDiagnoseGraphic->SetWidth((*iter)->GetWidth());
						pDiagnoseGraphic->SetHeight((*iter)->GetHeight());
						Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\chartDiagnose.png");
						if(pBitmap)
						{
							pDiagnoseGraphic->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddDiagnoseGraphic(pDiagnoseGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawPolygon*>& pPolygonList = pPage->GetPolygon();
				list<CEqmDrawPolygon*>::iterator iter = pPolygonList.begin();
				while (iter != pPolygonList.end())
				{
					CEqmDrawPolygon* pPolygon = new CEqmDrawPolygon();
					if(pPolygon)
					{
						pPolygon->SetPointLIst((*iter)->GetPointList());
						pPolygon->SetEqmType((*iter)->GetEqmType());
						pPolygon->SetPageID(nID);
						pPolygon->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pPolygon->SetLayer((*iter)->GetLayer());
						pPolygon->SetTemplateGroupId((*iter)->GetTemplateGroupId());
						pPolygon->SetTemplatePageId((*iter)->GetTemplatePageId());
						pPolygon->SetTemplatePageName((*iter)->GetTemplatePageName());
						pPolygon->SetTemplateName((*iter)->GetTemplateName());
						pPolygon->SetParam((*iter)->GetParam());
						pPolygon->SetLineColor_PptsWnd((*iter)->GetLineColor_PptsWnd());
						pPolygon->SetLineWidth_PptsWnd((*iter)->GetLineWidth_PptsWnd());
						pPolygon->SetNeedSave(true);

						pInsertPage->AddPolygon(pPolygon);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawAreaChange*>& AreaChangeList = pPage->GetAreaChangeList();
				list<CEqmDrawAreaChange*>::iterator iter = AreaChangeList.begin();
				while (iter != AreaChangeList.end())
				{
					CEqmDrawAreaChange* pAreaChange = new CEqmDrawAreaChange();
					if(pAreaChange)
					{
						pAreaChange->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pAreaChange->SetEqmType((*iter)->GetEqmType());
						pAreaChange->SetPageID(nID);
						pAreaChange->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pAreaChange->SetWidth((*iter)->GetWidth());
						pAreaChange->SetHeight((*iter)->GetHeight());
						pAreaChange->SetLayer((*iter)->GetLayer());
						pAreaChange->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));
						Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(pAreaChange->GetPicID());
						if(pBitmap)
							pAreaChange->SetBitmap(pBitmap);

						vector<_tagAreaInfo> vecArea = (*iter)->GetAreaInfo();
						for(int j=0; j<vecArea.size(); j++)
						{
							vecArea[j].BnmID = GetNewPicIDAndInsertIntoLib(nMaxPicID,vecArea[j].BnmID);
							vecArea[j].nPageID = 0;
						}

						pAreaChange->SetAreaInfo(vecArea);

						pInsertPage->AddAreaChange(pAreaChange);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawShellExecute*>& shellExecuteList = pPage->GetShellExecuteList();
				list<CEqmDrawShellExecute*>::iterator iter = shellExecuteList.begin();
				while (iter != shellExecuteList.end())
				{
					CEqmDrawShellExecute* pShellExecute = new CEqmDrawShellExecute();
					if(pShellExecute)
					{
						pShellExecute->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
						pShellExecute->SetEqmType(EQMDRAW_SHELL_EXECUTE);
						pShellExecute->SetPageID(nID);
						pShellExecute->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
						pShellExecute->SetWidth((*iter)->GetWidth());
						pShellExecute->SetHeight((*iter)->GetHeight());
						pShellExecute->SetLayer((*iter)->GetLayer());

						pShellExecute->SetServerIP((*iter)->GetServerIP());
						pShellExecute->SetFileName((*iter)->GetFileName());
						pShellExecute->SetDownloadDirectory((*iter)->GetDownloadDirectory());
						pShellExecute->SetFileID((*iter)->GetFileID());
						pShellExecute->SetBKPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetBKPicID()));
						pShellExecute->SetAutoDownload((*iter)->GetAutoDownload());

						Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetBKPicID());
						if(pBitmap)
						{
							pShellExecute->SetDefaultBitmap(pBitmap);
						}
						pInsertPage->AddShellExecute(pShellExecute);
					}
					iter++;
				}
			}
			{
				list<CEqmDrawEmbededPage*>& embededpageList = pPage->GetEmbededPageList();
				list<CEqmDrawEmbededPage*>::iterator iter = embededpageList.begin();
				while (iter != embededpageList.end())
				{
					CEqmDrawEmbededPage* pEmbededPage = new CEqmDrawEmbededPage();
					pEmbededPage->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pEmbededPage->SetEqmType(EQMDRAW_EMBEDED_PAGE);
					pEmbededPage->SetPageID(nID);
					pEmbededPage->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pEmbededPage->SetWidth((*iter)->GetWidth());
					pEmbededPage->SetHeight((*iter)->GetHeight());
					pEmbededPage->SetLayer((*iter)->GetLayer());
					pEmbededPage->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));
					pEmbededPage->SetChangeInterval((*iter)->GetChangeInterval());
					Image* pBitmap = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetPicID());
					if(pBitmap)
					{
						pEmbededPage->SetDefaultBitmap(pBitmap);
					}
					vector<PageIDName>&	vec = (*iter)->GetPageList();
					for (size_t i=0;i<vec.size();++i)
					{
						pEmbededPage->InsertPageList(vec[i].nPageID, vec[i].strPageName);
					}
					pInsertPage->AddEmbededPage(pEmbededPage);
					++iter;
				}
			}
			{
				list<CEqmDrawLiquidLevel*>& liquidLevelList = pPage->GetLiquidLevelList();
				list<CEqmDrawLiquidLevel*>::iterator iter = liquidLevelList.begin();
				while (iter != liquidLevelList.end())
				{
					CEqmDrawLiquidLevel* pLiquidLevel = new CEqmDrawLiquidLevel();
					pLiquidLevel->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pLiquidLevel->SetEqmType(EQMDRAW_LIQUID_LEVEL);
					pLiquidLevel->SetPageID(nID);
					pLiquidLevel->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pLiquidLevel->SetWidth((*iter)->GetWidth());
					pLiquidLevel->SetHeight((*iter)->GetHeight());
					pLiquidLevel->SetLayer((*iter)->GetLayer());
					//pLiquidLevel->SetPicID(GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID()));

					pLiquidLevel->SetBindPoint((*iter)->GetBindPoint());
					pLiquidLevel->SetMaxValue((*iter)->GetMaxValue());
					pLiquidLevel->SetMinValue((*iter)->GetMinValue());
					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\liquidLevel.png");
					if (pBitmap != NULL)
					{
						pLiquidLevel->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddLiquidLevel(pLiquidLevel);
					++iter;
				}
			}
			{
				list<CEqmDrawButtonPlus*>& chkBtnPlusList = pPage->GetCheckButtonPlusList();
				list<CEqmDrawButtonPlus*>::iterator iter = chkBtnPlusList.begin();
				while (iter != chkBtnPlusList.end())
				{
					CEqmDrawButtonPlus* pChkBtnPlus = new CEqmDrawButtonPlus();
					pChkBtnPlus->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pChkBtnPlus->SetEqmType(EQMDRAW_CHKBTN_PLUS);
					pChkBtnPlus->SetPageID(nID);
					pChkBtnPlus->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pChkBtnPlus->SetWidth((*iter)->GetWidth());
					pChkBtnPlus->SetHeight((*iter)->GetHeight());
					pChkBtnPlus->SetLayer((*iter)->GetLayer());

					pChkBtnPlus->SetTextColor((*iter)->GetTextColor());
					pChkBtnPlus->SetTextSize((*iter)->GetTextSize());
					pChkBtnPlus->SetTextContent((*iter)->GetTextContent());
					pChkBtnPlus->SetButtonStyle((*iter)->GetButtonStyle());
					pChkBtnPlus->SetValueForUnCheck((*iter)->GetValueForUnCheck());
					pChkBtnPlus->SetValueForCheck((*iter)->GetValueForCheck());
					pChkBtnPlus->SetBindPointName((*iter)->GetBindPointName());
					pChkBtnPlus->SetGroupID((*iter)->GetGroupID());
					pChkBtnPlus->SetExpression((*iter)->GetExpression());
					pChkBtnPlus->SetPathUncheckCom((*iter)->GetPathUncheckCom());
					pChkBtnPlus->SetPathUncheckOver((*iter)->GetPathUncheckOver());
					pChkBtnPlus->SetPathCheckCom((*iter)->GetPathCheckCom());
					pChkBtnPlus->SetPathCheckOver((*iter)->GetPathCheckOver());
					pChkBtnPlus->SetOptionsDec((*iter)->GetOptionsDec());
					Image* pBitmap = Image::FromFile((*iter)->GetPathCheckCom());
					if (pBitmap != NULL)
					{
						pChkBtnPlus->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddCheckButtonPlus(pChkBtnPlus);
					++iter;
				}
			}
			{
				list<CEqmDrawStraightLine*>& straightLineList = pPage->GetStraightLineList();
				list<CEqmDrawStraightLine*>::iterator iter = straightLineList.begin();
				while (iter != straightLineList.end())
				{
					CEqmDrawStraightLine* pStraightLine = new CEqmDrawStraightLine();
					pStraightLine->SetEqmType(EQMDRAW_LINE);
					pStraightLine->SetPageID(nID);
					pStraightLine->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pStraightLine->SetLayer((*iter)->GetLayer());
					pStraightLine->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pStraightLine->SetTerminalPointX((*iter)->GetTerminalPointX());
					pStraightLine->SetTerminalPointY((*iter)->GetTerminalPointY());

					pStraightLine->SetLineWidth(pStraightLine->GetLineWidth());
					pStraightLine->SetLineStyle(pStraightLine->GetLineStyle());
					pStraightLine->SetLineColor(pStraightLine->GetLineColor());
					pStraightLine->SetTransparency(pStraightLine->GetTransparency());

					pStraightLine->SetStartShape(pStraightLine->GetStartShape());
					pStraightLine->SetStartShapeSize(pStraightLine->GetStartShapeSize());
					pStraightLine->SetEndShape(pStraightLine->GetEndShape());
					pStraightLine->SetEndShapeSize(pStraightLine->GetEndShapeSize());

					pInsertPage->AddStraightLine(pStraightLine);
					++iter;
				}
			}
			{
				list<CEqmDrawHistoryTrend*>& historyTrendList = pPage->GetHistoryTrendList();
				list<CEqmDrawHistoryTrend*>::iterator iter = historyTrendList.begin();
				while (iter != historyTrendList.end())
				{
					CEqmDrawHistoryTrend* pHistoryTrend = new CEqmDrawHistoryTrend();
					pHistoryTrend->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pHistoryTrend->SetEqmType(EQMDRAW_HISTORY_TREND);
					pHistoryTrend->SetPageID(nID);
					pHistoryTrend->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pHistoryTrend->SetWidth((*iter)->GetWidth());
					pHistoryTrend->SetHeight((*iter)->GetHeight());
					pHistoryTrend->SetLayer((*iter)->GetLayer());

					pHistoryTrend->SetHistoryTrendType((*iter)->GetHistoryTrendType());
					pHistoryTrend->SetHistoryTrendVector((*iter)->GetHistoryTrendVector());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\historyTrend.png");
					if (pBitmap != NULL)
					{
						pHistoryTrend->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddHistoryTrend(pHistoryTrend);
					++iter;
				}
			}
			{
				list<CEqmDrawRectangle*>& rectangleList = pPage->GetRectangleList();
				list<CEqmDrawRectangle*>::iterator iter = rectangleList.begin();
				while (iter != rectangleList.end())
				{
					CEqmDrawRectangle* pRectangle = new CEqmDrawRectangle();
					pRectangle->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pRectangle->SetEqmType(EQMDRAW_RECTANGLE);
					pRectangle->SetPageID(nID);
					pRectangle->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pRectangle->SetWidth((*iter)->GetWidth());
					pRectangle->SetHeight((*iter)->GetHeight());
					pRectangle->SetLayer((*iter)->GetLayer());
					pRectangle->SetLineColor((*iter)->GetLineColor());
					pRectangle->SetLineWidth((*iter)->GetLineWidth());
					pRectangle->SetFillStyl((*iter)->GetFillStyl());
					pRectangle->SetFillColor((*iter)->GetFillColor());
					pRectangle->SetToRoundRect((*iter)->GetToRoundRect());
					pRectangle->SetRoundHeight((*iter)->GetRoundWidth());
					pRectangle->SetRoundWidth((*iter)->GetRoundWidth());
					pInsertPage->AddRectangle(pRectangle);
					++iter;
				}
			}
			{
				list<CEqmDrawMapNavigate*>& mapNavigateList = pPage->GetMapNavigateList();
				list<CEqmDrawMapNavigate*>::iterator iter = mapNavigateList.begin();
				while (iter != mapNavigateList.end())
				{
					CEqmDrawMapNavigate* pMapNavigate = new CEqmDrawMapNavigate();
					pMapNavigate->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pMapNavigate->SetEqmType(EQMDRAW_MAP_NAVIGATE);
					pMapNavigate->SetPageID(nID);
					pMapNavigate->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pMapNavigate->SetWidth((*iter)->GetWidth());
					pMapNavigate->SetHeight((*iter)->GetHeight());
					pMapNavigate->SetLayer((*iter)->GetLayer());
					pMapNavigate->SetEquipTemplateGroupId((*iter)->GetEquipTemplateGroupId());
					pMapNavigate->SetEquipTemplatePageId((*iter)->GetEquipTemplatePageId());
					pMapNavigate->SetEquipTemplatePageName((*iter)->GetEquipTemplatePageName());
					pMapNavigate->SetParamListAllString((*iter)->GetParamListAllString());
					pMapNavigate->SetTemplateName((*iter)->GetTemplateName());
					pMapNavigate->SetScaleRuler((*iter)->GetScaleRuler());
					pMapNavigate->SetMapPageInfo((*iter)->GetMapPageInfo());
					pMapNavigate->SetBanOnDisplay((*iter)->GetBanOnDisplay());

					int nWidth, nHeight;
					CString cstrTemplateNo4db;
					CString strTemFile;

					DirectoryOperations::MakeDirectory(m_strSysPath+L"\\temp",cstrTemplateNo4db);
					cstrTemplateNo4db = (*iter)->GetTemplateName().Left((*iter)->GetTemplateName().Find(L".4db"));
					strTemFile.Format(L"%s\\temp\\%s\\%d.png", m_strSysPath, cstrTemplateNo4db, (*iter)->GetEquipTemplatePageId());
					

					pFrame->m_pPrjSqlite->ExtractTemplatePNGFile((*iter)->GetTemplateName(), (*iter)->GetEquipTemplatePageId(), strTemFile, nWidth, nHeight);
					Image* pBitmap = Image::FromFile(strTemFile);
					if (pBitmap != NULL)
					{
						pMapNavigate->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddMapNavigate(pMapNavigate);
					++iter;
				}
			}
			{
				list<CEqmDrawNavigateWindow*>& naviWindList = pPage->GetNavigateWindowList();
				list<CEqmDrawNavigateWindow*>::iterator iter = naviWindList.begin();
				while (iter != naviWindList.end())
				{
					CEqmDrawNavigateWindow* pNaviWind = new CEqmDrawNavigateWindow();
					pNaviWind->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pNaviWind->SetEqmType(EQMDRAW_NAVIGATE_WINDOW);
					pNaviWind->SetPageID(nID);
					pNaviWind->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pNaviWind->SetWidth((*iter)->GetWidth());
					pNaviWind->SetHeight((*iter)->GetHeight());
					pNaviWind->SetLayer((*iter)->GetLayer());

					pNaviWind->SetPageInfo((*iter)->GetPageInfo());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\NavigateWindow.png");
					if (pBitmap != NULL)
					{
						pNaviWind->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddNavigateWindow(pNaviWind);
					++iter;
				}
			}
			{
				list<CEqmDrawLevelRuler*>& levelRulerList = pPage->GetLevelRulerList();
				list<CEqmDrawLevelRuler*>::iterator iter = levelRulerList.begin();
				while (iter != levelRulerList.end())
				{
					CEqmDrawLevelRuler* pLevelRuler = new CEqmDrawLevelRuler();
					pLevelRuler->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pLevelRuler->SetEqmType(EQMDRAW_LEVEL_RULER);
					pLevelRuler->SetPageID(nID);
					pLevelRuler->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));
					pLevelRuler->SetWidth((*iter)->GetWidth());
					pLevelRuler->SetHeight((*iter)->GetHeight());
					pLevelRuler->SetLayer((*iter)->GetLayer());

					pLevelRuler->SetName((*iter)->GetName());
					pLevelRuler->SetMax((*iter)->GetMax());
					pLevelRuler->SetMin((*iter)->GetMin());
					pLevelRuler->SetMainScaleInterval((*iter)->GetMainScaleInterval());
					pLevelRuler->SetMinorScaleInterval((*iter)->GetMinorScaleInterval());
					pLevelRuler->SetDecimalPlace((*iter)->GetDecimalPlace());
					pLevelRuler->SetLabelInfo((*iter)->GetLabelInfo());
					pLevelRuler->SetSectionInfo((*iter)->GetSectionInfo());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\LevelRuler.png");
					if (pBitmap != NULL)
					{
						pLevelRuler->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddLevelRuler(pLevelRuler);
					++iter;
				}
			}
			{
				list<CEqmDrawDataAnalyse*>& dataAnalyseList = pPage->GetDataAnalyseList();
				list<CEqmDrawDataAnalyse*>::iterator iter = dataAnalyseList.begin();
				while (iter != dataAnalyseList.end())
				{
					CEqmDrawDataAnalyse* pDataAnalyse = new CEqmDrawDataAnalyse();
					pDataAnalyse->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					pDataAnalyse->SetEqmType(EQMDRAW_DATA_ANALYSE);
					pDataAnalyse->SetPageID(nID);
					pDataAnalyse->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					pDataAnalyse->SetWidth((*iter)->GetWidth());
					pDataAnalyse->SetHeight((*iter)->GetHeight());
					pDataAnalyse->SetLayer((*iter)->GetLayer());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataAnalyse.png");
					if (pBitmap != NULL)
					{
						pDataAnalyse->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddDataAnalyse(pDataAnalyse);
					++iter;
				}
			}
			{
				list<CEqmDrawTemperDistri*>& listContain = pPage->GetTemperDistriList();
				list<CEqmDrawTemperDistri*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawTemperDistri* p = new CEqmDrawTemperDistri();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_TEMPER_DISTRI);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetTemperDistri((*iter)->GetTemperDistri());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\TemperDistri.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddTemperDistri(p);
					++iter;
				}
			}
			{
				list<CEqmDrawDataReport*>& listContain = pPage->GetDataReportList();
				list<CEqmDrawDataReport*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawDataReport* p = new CEqmDrawDataReport();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_DATA_REPORT);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetPath((*iter)->GetPath());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\DataReport.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddDataReport(p);
					++iter;
				}
			}
			{
				list<CEqmDrawPlaneTempeDistr*>& listContain = pPage->GetPlaneTempeDistrList();
				list<CEqmDrawPlaneTempeDistr*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawPlaneTempeDistr* p = new CEqmDrawPlaneTempeDistr();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_PLANE_TEMPE_DISTR);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetPointName((*iter)->GetPointName());
					p->SetConfigType((*iter)->GetConfigType());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\PlaneTempeDistr.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddPlaneTempeDistr(p);
					++iter;
				}
			}
			{
				list<CEqmDrawDottedLineFrm*>& listContain = pPage->GetDottedLineFrmList();
				list<CEqmDrawDottedLineFrm*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawDottedLineFrm* p = new CEqmDrawDottedLineFrm();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_DOTLINE_FRAME);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());
					pInsertPage->AddDottedLineFrm(p);
					++iter;
				}
			}
			{
				list<CEqmDrawGeneralComponent*>& listContain = pPage->GetGeneralComponentList();
				list<CEqmDrawGeneralComponent*>::iterator iter = listContain.begin();
				while (iter != listContain.end())
				{
					CEqmDrawGeneralComponent* p = new CEqmDrawGeneralComponent();
					p->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
					p->SetEqmType(EQMDRAW_GENERAL_COMPONENT);
					p->SetPageID(nID);
					p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++pFrame->m_MaxEqmtID));
					p->SetWidth((*iter)->GetWidth());
					p->SetHeight((*iter)->GetHeight());
					p->SetLayer((*iter)->GetLayer());

					p->SetConfigValue((*iter)->GetConfigValue());

					Image* pBitmap = Image::FromFile(m_strSysPath + L"\\Images\\GeneralComponent.png");
					if (pBitmap != NULL)
					{
						p->SetDefaultBitmap(pBitmap);
					}
					pInsertPage->AddGeneralComponent(p);
					++iter;
				}
			}
			pInsertPage->SetID(nID);
			//CString strItemText = pView->m_pTreeDlg->m_treectrl.GetItemText(m_hItem);
			CString strItemText = pFrame->m_wndFileView.m_wndFileView.GetItemText(m_hItem);
			const int nGroupID = pFrame->m_project.GetGroupIDByGroupName(strItemText.GetString());
			pInsertPage->SetGroupID(nGroupID);
			pFrame->m_DrawContext.pPage = pInsertPage;
			pFrame->m_DrawContext.pPage->SetGroupID(nGroupID);
			pInsertPage->m_bDetailLoaded = true;
			pInsertPage->SetPageShowTopInObserver(pPage->GetPageShowTopInObserver());
			pFrame->m_project.AddPage(pInsertPage);
			nPageNum ++;
			strOut.Format(_T("已导入第%d个页面:%s"),nPageNum,pInsertPage->GetPageName().c_str());
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
			//pFrame->m_MaxPageID++;
		}
	}
	ClearEventInfoPageId();
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("导入完成"));
	EnableControl(true);
	pFrame->SetSaveFlag(true);
}

int CImportPageSelectDlg::GetNewAnimationIDAndInsertIntoLib( int nMaxAniID, int nAniID )
{
	for(int i=0; i<m_vecNewAniID.size(); ++i)
	{
		if(m_vecNewAniID[i].nOldPicID == nAniID)
			return m_vecNewAniID[i].nNewPicID;
	}

	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_ALL, "chs" );

	//
	int nNewAniID = nMaxAniID + nAniID;	
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	if (m_project_sqlite != NULL)
	{
		AnimationInfo* ani = m_project_sqlite->GetAnimationByID(nAniID);
		if(ani == NULL)		//不存在
			return nAniID;
		//判断现有s3db是否含有改动画
		AnimationInfo* ani_now = pFrame->m_pPrjSqlite->GetAnimationByID(nAniID);
		bool bAnimationIsExist = false;
		if(ani_now != NULL)		//存在该ID图片 判断下是否一致
		{
			if(ani_now->szName == ani->szName && ani_now->vecPicIDList.size() == ani->vecPicIDList.size())
			{
				bAnimationIsExist = true;
			}
		}

		if(!bAnimationIsExist)	//
		{
			ani_now = pFrame->m_pPrjSqlite->CompareAndReturnAnimationInfoByName(ani->szName,ani->vecPicture);
		}

		if(ani_now)	//已有图
		{
			nNewAniID = ani_now->ID;
		}
		else			//插入动画
		{
			if(ani)
			{
				int nNewPicID;
				CString strAnimationSeries = _T("");

				char szName[MAX_PATH] = {0};

				for(int i=0; i<ani->vecPicture.size(); ++i)
				{
					PictureInfo pic = ani->vecPicture[i];
					nNewPicID = pFrame->m_pPrjSqlite->GetMaxLibImageAnimationNum() + pic.ID;
					pic.ID = nNewPicID;

					size_t converted = 0;
					errno_t err =  wcstombs_s(&converted, szName, sizeof(szName), pic.szName, sizeof(szName));

					pFrame->m_pPrjSqlite->InsertPictureAnimationData(nNewPicID,szName, Project::Tools::WideCharToAnsi(pic.etype_equip.c_str()).c_str(), pic.pBlock, pic.nSize, true);
					//pFrame->m_pPrjSqlite->InsertToLibImageAnimation(pic);
					pFrame->m_pPrjSqlite->AddIntoAnimationImage(nNewPicID, &pic, true);
					CString strID;
					strID.Format(L"%d",pic.ID);
					if (strAnimationSeries.GetLength() == 0)
					{
						strAnimationSeries = strID;
					}
					else
					{
						strAnimationSeries = strAnimationSeries + L"," + strID;
					}
				}
				char szTempName[MAX_PATH] = {0};
				char szTempAnimationList[1000] = {0};
				size_t converted = 0;
				CString strAniamtionName = ani->szName.c_str();
				errno_t err =  wcstombs_s(&converted, szTempName, sizeof(szTempName), strAniamtionName.GetString(), sizeof(szTempName));
				ASSERT(converted > 0);
				converted = 0;
				wcstombs_s(&converted, szTempAnimationList, sizeof(szTempAnimationList), strAnimationSeries.GetString(), sizeof(szTempAnimationList));
				ASSERT(converted > 0);
				pFrame->m_pPrjSqlite->InsertRecordToAnimationList(nNewAniID, szTempName, szTempAnimationList, true);
				pFrame->m_pPrjSqlite->InsertAnimationList(nNewAniID, ani->szName.c_str(), strAnimationSeries);
			}
		}

	}
	NewPicIDInfo newPic;
	newPic.nOldPicID = nAniID;
	newPic.nNewPicID = nNewAniID;
	m_vecNewAniID.push_back(newPic);
	setlocale( LC_ALL, old_locale );

	return nNewAniID;
}


void CImportPageSelectDlg::OnBnClickedBtnSelectAll()
{
	// TODO: Add your control notification handler code here
	const int nCheckCount = m_list_page.GetItemCount();
	if (nCheckCount < 1)
	{
		return;
	}

	if (m_bSelectAll)
	{
		m_bSelectAll = FALSE;

		for (int i=0; i<nCheckCount; ++i)
		{
			m_list_page.SetCheck(i, FALSE);
		}

		m_btnSelectAll.SetWindowText(_T("全选"));
	}
	else
	{
		m_bSelectAll = TRUE;

		for (int i=0; i<nCheckCount; ++i)
		{
			m_list_page.SetCheck(i, TRUE);
		}

		m_btnSelectAll.SetWindowText(_T("不选"));
	}
}

void CImportPageSelectDlg::ClearEventInfoPageId(void)
{
	// clear invalidate EventInfo.nPageID;
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);
	if (NULL == pFrame)
	{
		return;
	}

	vector<CEqmDrawPage*> vecPage(pFrame->m_project.GetPageList());
	vector<CEqmDrawPage*>::const_iterator	iterPage;
	list<CEqmDrawDevice*>	listDevice;
	list<CEqmDrawDevice*>::const_iterator	iterDev;
	list<CEqmDrawButton*>	listBtn;
	list<CEqmDrawButton*>::const_iterator	iterBtn;

	vector<int>	vecPageId;
	vector<int>::const_iterator iterPageId;
	for (iterPage=vecPage.begin(); iterPage!=vecPage.end(); iterPage++)
	{
		vecPageId.push_back((*iterPage)->GetID());
	}

	for (iterPage=vecPage.begin(); iterPage!=vecPage.end(); iterPage++)
	{
		// image
		listDevice = (*iterPage)->GetDrawDeviceList();
		for (iterDev=listDevice.begin(); iterDev!=listDevice.end(); iterDev++)
		{
			EventInfo ev = (*iterDev)->GetEventInfo();
			bool bFind = false;
			for (iterPageId=vecPageId.begin(); iterPageId!=vecPageId.end(); iterPageId++)
			{
				if ((*iterPageId) == ev.nPageID)
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				(*iterDev)->m_eventInfo.nPageID = -1;
			}
		}

		// button
		listBtn = (*iterPage)->GetButtonList();
		for (iterBtn=listBtn.begin(); iterBtn!=listBtn.end(); iterBtn++)
		{
			int nLinkPageId = (*iterBtn)->GetLinkPageID();
			bool bFind = false;
			for (iterPageId=vecPageId.begin(); iterPageId!=vecPageId.end(); iterPageId++)
			{
				if ((*iterPageId) == nLinkPageId)
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				(*iterBtn)->SetLinkPageID(-1);
			}
		}
	}
}