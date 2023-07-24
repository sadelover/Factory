// ImportTemplatePageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportTemplatePageDlg.h"
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
#include "../ComponentDraw/EqmDrawDiagnoseGraphicPlus.h"
#include "../ComponentDraw/EqmDrawShellExecute.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "../ComponentDraw/EqmDrawLiquidLevel.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../ComponentDraw/EqmDrawScrawl.h"
#include "../ComponentDraw/EqmDrawHistoryTrend.h"
#include "../ComponentDraw/EqmDrawEnergySaveRoi.h"
#include "../ComponentDraw/EqmDrawMapNavigate.h"
#include "../ComponentDraw/EqmDrawDataAnalyse.h"
#include "../ComponentDraw/EqmDrawTemperDistri.h"
#include "../ComponentDraw/EqmDrawDataReport.h"
#include "../ComponentDraw/EqmDrawPlaneTempeDistr.h"
#include "../ComponentDraw/EqmDrawDottedLineFrm.h"
#include "../ComponentDraw/EqmDrawGeneralComponent.h"
#include "../ComponentDraw/EqmDrawRectangle.h"
#include "../ComponentDraw/EqmDrawStraightLine.h"
#include "../ComponentDraw/EqmDrawPolygon.h"
#include "MainFrm.h"
#include "BEOP-DesignerView.h"
#include "TreeManageDlg.h"
#include "ImportSettingDlg.h"
#include "Tools/Util/UtilString.h"
#include "Tools/CustomTools/CustomTools.h"
#include "ImportTemplateSettingDlg.h"
#include "ImportTemplateBatchSettingDlg.h"
#include "ImportTemplateBatchSettingExDlg.h"
// CImportTemplatePageDlg dialog

IMPLEMENT_DYNAMIC(CImportTemplatePageDlg, CSkinDlg)

CImportTemplatePageDlg::CImportTemplatePageDlg(CWnd* pParent /*=NULL*/)
	: CSkinDlg(CImportTemplatePageDlg::IDD, pParent)
	, m_project_sqlite(NULL)
	, m_threadhandle_LoadProject(NULL)
	, m_bInitImport(false)
	, m_bConfigEsixt(false)
	, m_strFileName(_T(""))
	, m_strSelectPageName(_T(""))
	, m_nID(0)
	, m_nSelectPageID(-1)
	, m_nMaxPicID(0)
	, m_bImporSelectResource(false)
	, m_bImporSelectResourceReady(false)
	, m_bResourceChanged(false)
{

}

CImportTemplatePageDlg::~CImportTemplatePageDlg()
{
	if(m_threadhandle_LoadProject)
	{
		CloseHandle(m_threadhandle_LoadProject);
		m_threadhandle_LoadProject = NULL;
	}
}

void CImportTemplatePageDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGE_IMPORT, m_list_page);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_BATCH_IMPORT, m_btnBatchImport);
	DDX_Control(pDX, IDC_BTN_BATCH_IMPORT_EX, m_btnBatchImportEx);
	DDX_Control(pDX, IDC_COMBO_PROJECT, m_comProjectList);
}

BEGIN_MESSAGE_MAP(CImportTemplatePageDlg, CSkinDlg)
	ON_BN_CLICKED(IDOK, &CImportTemplatePageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_BATCH_IMPORT, &CImportTemplatePageDlg::OnBnClickedBatchImport)
	ON_BN_CLICKED(IDC_BTN_BATCH_IMPORT_EX, &CImportTemplatePageDlg::OnBnClickedBatchImportEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_POINT, &CImportTemplatePageDlg::OnBnClickedButtonReplacePoint)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJECT, &CImportTemplatePageDlg::OnCbnSelchangeComboProject)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PAGE_IMPORT, &CImportTemplatePageDlg::OnNMClickListPageImport)
	ON_BN_CLICKED(IDCANCEL, &CImportTemplatePageDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CImportTemplatePageDlg message handlers


void CImportTemplatePageDlg::OnBnClickedButtonImportPageSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlgOpenFile(TRUE,NULL,L"*.s3db",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"s3db Files (*.s3db)|*.s3db|");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		BeginWaitCursor();
		m_strPath = dlgOpenFile.GetPathName();
		m_strFileName = dlgOpenFile.GetFileName();
		m_page_import_path.SetWindowTextW(m_strPath);
		//CDataPointManager			plcpointmap;
		CLogicPointManager			logicPotManager;
		CAlarmPointManager			AlarmPotManager;
		if (m_project_sqlite == NULL)
		{
			m_project_sqlite = new CProjectSqlite(m_project,m_pointmap,logicPotManager,AlarmPotManager);
		}
		m_project_sqlite->SetFilePath(m_strPath.GetString());
		m_project_sqlite->SetOwner(eFactory);
		m_project_sqlite->LoadProjectPages();
		vector<CEqmDrawPage*>& page_list = m_project.GetPageList();
		for (size_t i=0;i<page_list.size();++i)
		{
			m_list_page.InsertItem(i,L"");
			CString strTemp;
			strTemp.Format(L"%d",page_list[i]->GetID());
			m_list_page.SetItemText(i,0,strTemp);
			m_list_page.SetItemText(i,1,page_list[i]->GetPageName().c_str());
		}
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		EndWaitCursor();
	} 
}

BOOL CImportTemplatePageDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD oldstyle = m_list_page.GetExtendedStyle();
	m_list_page.SetExtendedStyle(oldstyle|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	//m_list_page.InsertColumn(0,_T("选中模板"),LVCFMT_LEFT, 100);
	m_list_page.InsertColumn(0,_T("页面ID"),LVCFMT_LEFT, 50);
	m_list_page.InsertColumn(1,_T("页面名称"),LVCFMT_LEFT, 300);

	GetSysPath(m_strSysPath);
	FindAllS3dbFile(m_strSysPath+_T("\\template"));

	m_comProjectList.ResetContent();
	for(int i=0; i<m_vecTemplateFiles.size(); ++i)
	{
		m_comProjectList.AddString(m_vecTemplateFiles[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImportTemplatePageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// 配置部分
	if (NULL == m_project_sqlite)
	{
		MessageBox(_T("请先选择需导入的s3db文件！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		return;
	}
	/*int nCount = 0;
	for (int i=0;i<m_list_page.GetItemCount();++i)
	{
		if (m_list_page.GetCheck(i))
		{
			nCount++;
		}
	}
	if(nCount>1)
	{
		MessageBox(_T("一次只允许选中一个页面！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		return;
	}
	m_strSelectPageName = _T("");
	for (int i=0;i<m_list_page.GetItemCount();++i)
	{
		if (m_list_page.GetCheck(i))
		{
			m_strSelectPageName = m_list_page.GetItemText(i,2);
			m_nSelectPageID = _wtoi(m_list_page.GetItemText(i,1));
			break;
		}
	}*/


	if(m_strSelectPageName != _T(""))
	{
		ImportSelectPages_CurrentPageResource(m_nMaxPicID);
	}
}

void CImportTemplatePageDlg::OnBnClickedBatchImport()
{
	if (NULL == m_project_sqlite)
	{
		MessageBox(_T("请先选择需导入的s3db文件！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		return;
	}

	if(m_strSelectPageName != _T(""))
	{
		ImportSelectPages_CurrentPageResource_Batch(m_nMaxPicID);
	}
}

void CImportTemplatePageDlg::OnBnClickedBatchImportEx()
{
	if (NULL == m_project_sqlite)
	{
		MessageBox(_T("请先选择需导入的s3db文件！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		return;
	}

	if(m_strSelectPageName != _T(""))
	{
		ImportSelectPages_CurrentPageResource_Batch_Ex(m_nMaxPicID);
	}
}

void CImportTemplatePageDlg::OnDestroy()
{
	CSkinDlg::OnDestroy();
	if (m_project_sqlite)
	{
		delete m_project_sqlite;
		m_project_sqlite = NULL;
		CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
		int a = 0;
	}
	// TODO: 在此处添加消息处理程序代码
}

int CImportTemplatePageDlg::GetNewPicIDAndInsertIntoLib( int nMaxPicID, int nPicID )
{
	//
	for(int i=0; i<m_vecNewPicID.size(); ++i)
	{
		if(m_vecNewPicID[i].nOldPicID == nPicID)
			return m_vecNewPicID[i].nNewPicID;
	}

	//
	int nNewPicID = nMaxPicID + nPicID;	
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	if (m_project_sqlite != NULL)
	{
		PictureInfo* pic = m_project_sqlite->GetPictureInfoByID(nPicID);
		if(pic == NULL || pic->nSize <=0)		//不存在
			return nPicID;
		//判断现有s3db是否含有改图
		PictureInfo* pic_now = pFrame->m_pPrjSqlite->GetPictureInfoByID(nPicID);
		bool bImageIsExist = false;
		if(pic_now != NULL)		//存在该ID图片 判断下是否一致
		{
			if(pic_now->nSize == pic->nSize && strcmp(pic_now->pBlock,pic->pBlock)==0)
			{
				bImageIsExist = true;
			}
		}

		if(!bImageIsExist)	//遍历现有整个图库找寻该图
		{
			pic_now = pFrame->m_pPrjSqlite->CompareAndReturnPictureInfoByBlock(pic->pBlock,pic->nSize, pic->szName, pic->etype_p, pic->etype_equip);
		}

		if(pic_now)	//已有图
		{
			nNewPicID = pic_now->ID;
		}
		else			//无图插入pic
		{
			if(pic)
			{
				pFrame->m_pPrjSqlite->InsertPictureData(nNewPicID,(char*)(pic->szName),Project::Tools::WideCharToAnsi(pic->etype_equip.c_str()).c_str(),pic->pBlock,pic->nSize);
				PictureInfo info;
				info.pBlock = new char[pic->nSize];
				if (info.pBlock !=NULL)
				{
					memcpy_s(info.pBlock,pic->nSize,pic->pBlock,pic->nSize);
				}
				info.ID = nNewPicID;
				info.pBitmap  = pic->pBitmap;
				wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t),pic->szName,sizeof(info.szName)/sizeof(wchar_t)-1);
				//pFrame->m_pPrjSqlite->InsertToLibImage(info);
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

void CImportTemplatePageDlg::EnableControl( bool bResult )
{
	GetDlgItem(IDC_LIST_PAGE_IMPORT)->EnableWindow(bResult);
	GetDlgItem(IDOK)->EnableWindow(bResult);
	GetDlgItem(IDCANCEL)->EnableWindow(bResult);
}

wstring CImportTemplatePageDlg::CreatePointIfNotFind(CProjectSqlite* project_sqlite, wstring strPointName)
{	
	if(project_sqlite)
	{
		wstring strNew = strPointName;
		bool bReplace;
		if(m_strPreName.GetLength()>0 && m_strReplaceName.GetLength()>0)
		{
			strNew = Replace(strPointName,m_strPreName.GetString(),m_strReplaceName.GetString(),bReplace);
		}

		if(!project_sqlite->IsPointExist(strNew))			//不存在点
		{
			DataPointEntry entry;
			entry = m_pointmap.GetEntry(strPointName);	
			if(entry.GetSourceType().length() == 0)		//不存在的点则不保存
			{
				return strPointName;
			}

			entry.SetShortName(strNew);
			m_nPointMaxIndex++;
			entry.SetPointIndex(m_nPointMaxIndex);

			if(m_strPreName.GetLength()>0 && m_strReplaceName.GetLength()>0)
			{
				entry.SetDescription(Replace(entry.GetDescription(),m_strPreName.GetString(),m_strReplaceName.GetString(),bReplace));
			}

			project_sqlite->AddPointEntry(entry);

			CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
			pFrame->SetPointTableFlag(true);
		}
	}
	return strPointName;
}

wstring CImportTemplatePageDlg::CreatePointIfNotFindEx(CProjectSqlite* project_sqlite, wstring strPointName)
{
	if (project_sqlite != NULL)
	{
		wstring strNew = strPointName;
		bool bReplace;
		CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();

		// 
		if (m_strPreName1.GetLength()>0 && m_strReplaceName1.GetLength()>0)
		{
			strNew = _T("");
			strNew = Replace(strPointName, m_strPreName1.GetString() ,m_strReplaceName1.GetString(), bReplace);
			if (!project_sqlite->IsPointExist(strNew))
			{
				DataPointEntry entry;
				entry = m_pointmap.GetEntry(strPointName);	
				if(entry.GetSourceType().length() == 0)
				{
					return strPointName;
				}
				entry.SetShortName(strNew);
				m_nPointMaxIndex++;
				entry.SetPointIndex(m_nPointMaxIndex);
				if (m_strPreName1.GetLength()>0 && m_strReplaceName1.GetLength()>0)
				{
					entry.SetDescription(Replace(entry.GetDescription(), m_strPreName1.GetString(), m_strReplaceName1.GetString(), bReplace));
				}
				project_sqlite->AddPointEntry(entry);
				pFrame->SetPointTableFlag(true);
			}
		}

		//
		if (m_strPreName2.GetLength()>0 && m_strReplaceName2.GetLength()>0)
		{
			strNew = _T("");
			strNew = Replace(strPointName, m_strPreName2.GetString() ,m_strReplaceName2.GetString(), bReplace);
			if (!project_sqlite->IsPointExist(strNew))
			{
				DataPointEntry entry;
				entry = m_pointmap.GetEntry(strPointName);	
				if(entry.GetSourceType().length() == 0)
				{
					return strPointName;
				}
				entry.SetShortName(strNew);
				m_nPointMaxIndex++;
				entry.SetPointIndex(m_nPointMaxIndex);
				if (m_strPreName2.GetLength()>0 && m_strReplaceName2.GetLength()>0)
				{
					entry.SetDescription(Replace(entry.GetDescription(), m_strPreName2.GetString(), m_strReplaceName2.GetString(), bReplace));
				}
				project_sqlite->AddPointEntry(entry);
				pFrame->SetPointTableFlag(true);
			}
		}

		//
		if (m_strPreName3.GetLength()>0 && m_strReplaceName3.GetLength()>0)
		{
			strNew = _T("");
			strNew = Replace(strPointName, m_strPreName3.GetString() ,m_strReplaceName3.GetString(), bReplace);
			if (!project_sqlite->IsPointExist(strNew))
			{
				DataPointEntry entry;
				entry = m_pointmap.GetEntry(strPointName);	
				if(entry.GetSourceType().length() == 0)
				{
					return strPointName;
				}
				entry.SetShortName(strNew);
				m_nPointMaxIndex++;
				entry.SetPointIndex(m_nPointMaxIndex);
				if (m_strPreName3.GetLength()>0 && m_strReplaceName3.GetLength()>0)
				{
					entry.SetDescription(Replace(entry.GetDescription(), m_strPreName3.GetString(), m_strReplaceName3.GetString(), bReplace));
				}
				project_sqlite->AddPointEntry(entry);
				pFrame->SetPointTableFlag(true);
			}
		}
	}
	return strPointName;
}

void CImportTemplatePageDlg::ImportSelectPages_CurrentPageResource(const int nMaxPicID)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());
	ASSERT(pView);

	const int nCount = m_list_page.GetItemCount();
	int nPageNum = 0;
	CString strOut;
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("开始导入模板页面，请稍候..."));
	EnableControl(false);

	//
	const int nPageID = m_nSelectPageID;
	CEqmDrawPage* pPage = m_project.GetPageByID(nPageID);
	if(!m_bResourceChanged)
	{
		GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("正在初始化模板图片资源..."));
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
		m_vecImportPageID.clear();
		m_vecNewPicID.clear();
		m_vecNewAniID.clear();
		m_bResourceChanged = true;
	}

	strOut.Format(_T("正在加载模板页面:%s"),pPage->GetPageName().c_str());
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
	if(!pPage->m_bDetailLoaded)
		m_project_sqlite->LoadPageDetails(pPage,NULL,NULL);

	//遍历点位查找数字
	CString strNum = FindNumInPageBindPoint(pPage);

	CImportTemplateSettingDlg dlg;
	dlg.m_strNewPageName = pPage->GetPageName().c_str();
	dlg.m_strPrePointName = strNum;
	if(dlg.DoModal() == IDOK)
	{
		m_strReplaceName = dlg.m_strReplacePointName;
		m_strPreName = dlg.m_strPrePointName;

		strOut = _T("正在导入模板页图元...");
		GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
		int nID = pFrame->m_MaxPageID+IMPORT_PAGE_ELEMENT_ID_START; 
		int nPageMaxOrder = pFrame->m_project.GetMaxPageShowOrder() + 1;
		CEqmDrawPage* pInsertPage = new CEqmDrawPage(nID,dlg.m_strNewPageName.GetString(),pPage->GetProjectID(),nPageMaxOrder,pPage->GetPageType(),pPage->GetPageWidth(),pPage->GetPageHeight(),pPage->GetPagexPosition(),pPage->GetPageyPosition(),pPage->GetStartColor());
		
		// insert background image
		int nBkImgIdNew = GetNewPicIDAndInsertIntoLib(nMaxPicID, pPage->GetBKImageID());
		pInsertPage->SetBKImageID(nBkImgIdNew);
		Image* pImage = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nBkImgIdNew, true);
		pInsertPage->SetBkImage(pImage);

		{
			list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
			list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
			bool bIsPic = true;
			int nNewId = 0;
			while (iter != deviceList.end())
			{
				CEqmDrawDevice* pDevice = new CEqmDrawDevice((*iter)->GetEqmType(), Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetDataComID(), (*iter)->GetRotateAngle(), (*iter)->GetEqmName(), (*iter)->GetEquipmentSubType(), (*iter)->GetWidth(), "", (*iter)->GetHeight(), 0.0f, 0.0f, 0.0f,(*iter)->GetBindType());
				ASSERT(pDevice);
				pDevice->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));//important
				pDevice->SetLayer((*iter)->GetLayer());
				CString strPoint = (*iter)->GetPhysicalID().c_str();

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
				pDevice->m_eventInfo.strSettingPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->m_eventInfo.strSettingPointName.GetString()).c_str();
				// import page link
				wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->m_eventInfo.nPageID);
				if (wstrPageName != L"")
				{
					int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
					if (-1 != nPageId)
					{
						pDevice->m_eventInfo.nPageID = nPageId;
					}
				}

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
				vector<CString> vec = (*iter)->GetDataPointList();
				for(size_t i=0; i<vec.size(); i++)
				{
					vec[i] = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].GetString()).c_str();
				}
				pPipe->SetDataPointList(vec);

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
					(*iter)->GetHeight(), (*iter)->GetFontSize(), (*iter)->GetFontName(), CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()),(*iter)->GetShowString(), (*iter)->GetFontColor(),
					(*iter)->GetTextShowMode(), (*iter)->GetTransparence(),(*iter)->GetBorderType(),(*iter)->GetBorderColor(), (*iter)->GetDeviceCfg(), (*iter)->GetFaultDiagnosis(),(*iter)->GetBindScript());
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
				pText->SetDateTimePoint(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetDateTimePoint().GetString()).c_str());
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
						vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
						vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
						vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
					pButton->SetBindSetPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindSetPointName().GetString()).c_str());
					pButton->SetSettingValue((*iter)->GetSettingValue());
					pButton->SetRelation((*iter)->GetRelation());
					pButton->SetDownloadEnable((*iter)->GetDownloadEnable());
					pButton->SetDownloadName((*iter)->GetDownloadName());

					// import page link
					wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->GetLinkPageID());
					if (wstrPageName != L"")
					{
						int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
						if (-1 != nPageId)
						{
							pButton->SetLinkPageID(nPageId);
						}
					}

					vector<_tagRelationExpress> vec = (*iter)->GetRelationVec();
					for(int i=0; i<vec.size(); i++)
					{
						vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
					pTimeSelect->SetPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPointName().GetString()).c_str());
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
					pTimeBar->SetBindPointStart(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointStart().GetString()).c_str());
					pTimeBar->SetQueryType((*iter)->GetQueryType());
					pTimeBar->SetBindPointStatus(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointStatus().GetString()).c_str());
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
					pProgressBar->SetBindPoint(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPoint().GetString()).c_str());
					pProgressBar->SetBindPointBase(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointBase().GetString()).c_str());
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

					vector<_tagDiagnoseItem>& vec = (*iter)->GetItemVec();
					for (size_t i=0;i<vec.size();++i)
					{
						vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();

						vector<_tagDiagnoseItemInLine>& vecItem = vec[i].vecItemInLine;
						for(size_t j=0; j<vecItem.size(); j++)
						{
							vecItem[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vecItem[i].strPointName.GetString()).c_str();
						}
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

				//pLiquidLevel->SetBindPoint((*iter)->GetBindPoint());
				pLiquidLevel->SetBindPoint(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPoint().GetString()).c_str());
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
				pChkBtnPlus->SetBindPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointName().GetString()).c_str());
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
				vector<HistoryTrendInfo> vec = (*iter)->GetHistoryTrendVector();
				for(int i=0; i<vec.size(); ++i)
				{
					vec[i].strBindPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strBindPointName.GetString()).c_str();
				}
				pHistoryTrend->SetHistoryTrendVector(vec);

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

				pMapNavigate->SetScaleRuler((*iter)->GetScaleRuler());
				pMapNavigate->SetMapPageInfo((*iter)->GetMapPageInfo());
				pMapNavigate->SetBanOnDisplay((*iter)->GetBanOnDisplay());
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

		if(strNum.GetLength()>0 && dlg.m_strReplacePointName.GetLength()>0)
		{
			strOut.Format(_T("正在替换点位:(原有点名包含%s的点替换为%s)"),dlg.m_strPrePointName,dlg.m_strReplacePointName);
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
			CString strResult;
			ReplacePagePoint(pInsertPage,dlg.m_strPrePointName,dlg.m_strReplacePointName,strResult);
		}

		strOut.Format(_T("已导入完成模板页图元,挂载页面到列表..."),nPageNum,pInsertPage->GetPageName().c_str());
		GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);

	/*	HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(pInsertPage->GetPageName().c_str(), m_hItem, nID);
		pView->m_pTreeDlg->m_treectrl.Expand(m_hItem, TVE_EXPAND);
		pView->m_pTreeDlg->m_treectrl.SetFocus();
		pView->m_pTreeDlg->m_treectrl.SelectItem(hPage);*/

		HTREEITEM hPage = pFrame->m_wndFileView.InsertPage(pInsertPage->GetPageName().c_str(), m_hItem, nID);
		pFrame->m_wndFileView.m_wndFileView.Expand(m_hItem, TVE_EXPAND);
		pFrame->m_wndFileView.m_wndFileView.SetFocus();
		pFrame->m_wndFileView.m_wndFileView.SelectItem(hPage);

		//CString strItemText = pView->m_pTreeDlg->m_treectrl.GetItemText(m_hItem);
		CString strItemText = pFrame->m_wndFileView.m_wndFileView.GetItemText(m_hItem);
		const int nGroupID = pFrame->m_project.GetGroupIDByGroupName(strItemText.GetString());
		pInsertPage->SetGroupID(nGroupID);
		pFrame->m_DrawContext.pPage = pInsertPage;
		pFrame->m_DrawContext.pPage->SetGroupID(nGroupID);
		pInsertPage->m_bDetailLoaded = true;
		pInsertPage->SetPageShowTopInObserver(pPage->GetPageShowTopInObserver());
		pFrame->m_project.AddPage(pInsertPage);
		pFrame->m_MaxPageID++;
	}
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("从模板导入页面完成..."));
	EnableControl(true);
	pFrame->SetSaveFlag(true);
}

void CImportTemplatePageDlg::ImportSelectPages_CurrentPageResource_Batch(const int nMaxPicID)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());
	ASSERT(pView);

	const int nCount = m_list_page.GetItemCount();
	int nPageNum = 0;
	CString strOut;
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("开始导入模板页面，请稍候..."));
	EnableControl(false);

	//
	const int nPageID = m_nSelectPageID;
	CEqmDrawPage* pPage = m_project.GetPageByID(nPageID);
	if(!m_bResourceChanged)
	{
		GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("正在初始化模板图片资源..."));
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
		m_vecImportPageID.clear();
		m_vecNewPicID.clear();
		m_vecNewAniID.clear();
		m_bResourceChanged = true;
	}

	strOut.Format(_T("正在加载模板页面:%s"),pPage->GetPageName().c_str());
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
	if(!pPage->m_bDetailLoaded)
		m_project_sqlite->LoadPageDetails(pPage,NULL,NULL);

	//遍历点位查找数字
	CString strNum = FindNumInPageBindPoint(pPage);

	CImportTemplateBatchSettingDlg dlg;
	dlg.m_strModelSrcName = pPage->GetPageName().c_str();
	if(dlg.DoModal() == IDOK)
	{
		//m_strReplaceName = dlg.m_strReplacePointName;	// deal with it
		m_strPreName = dlg.m_strPrePointName;
		CString strHolder(dlg.m_strBatchHolderNum);
		int nReplaceMod = 0;	// =0 present -; =1 present ,
		int nLoopStart = 0;
		int nLoopEnd = 0;
		vector<CString> vecHolder;
		if (strHolder.Find(_T("-")) >= 0)
		{
			int nPos = strHolder.Find(_T("-"));
			nLoopStart = _ttoi(strHolder.Left(nPos));
			nLoopEnd = _ttoi(strHolder.Mid(nPos + 1));
		}
		else if (strHolder.Find(_T(",")) >= 0)
		{
			strHolder.TrimRight(_T(","));
			strHolder += _T(",");
			while (TRUE)
			{
				int nPos = strHolder.Find(_T(","));
				if (-1 == nPos)
				{
					break;
				}
				vecHolder.push_back(strHolder.Left(nPos));
				strHolder = strHolder.Mid(nPos + 1);
			}
			nLoopEnd = vecHolder.size() - 1;
			nReplaceMod = 1;
		}
		else
		{
			MessageBox(_T("占位符格式错误！"));
			return;
		}


		for (int i = nLoopStart; i <= nLoopEnd; i++)
		{
			strOut = _T("正在导入模板页图元...");
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
			int nID = pFrame->m_MaxPageID+IMPORT_PAGE_ELEMENT_ID_START; 
			int nPageMaxOrder = pFrame->m_project.GetMaxPageShowOrder() + 1;
			CString strNewModelName;
			if (0 == nReplaceMod)
			{
				CString strCnt;
				strCnt.Format(_T("%d"), i);
				strNewModelName = dlg.m_strBatchNewPageName;
				strNewModelName.Replace(_T("*"), strCnt);

				m_strReplaceName = dlg.m_strReplacePointName;
				m_strReplaceName.Replace(_T("*"), strCnt);
			}
			else if (1 == nReplaceMod)
			{
				strNewModelName = dlg.m_strBatchNewPageName;
				strNewModelName.Replace(_T("*"), vecHolder.at(i));

				m_strReplaceName = dlg.m_strReplacePointName;
				m_strReplaceName.Replace(_T("*"), vecHolder.at(i));
			}
			else
			{
				strNewModelName = dlg.m_strBatchNewPageName;
				m_strReplaceName = dlg.m_strReplacePointName;
			}
			CEqmDrawPage* pInsertPage = new CEqmDrawPage(nID,strNewModelName.GetString(),pPage->GetProjectID(),nPageMaxOrder,pPage->GetPageType(),pPage->GetPageWidth(),pPage->GetPageHeight(),pPage->GetPagexPosition(),pPage->GetPageyPosition(),pPage->GetStartColor());
		
			// insert background image
			int nBkImgIdNew = GetNewPicIDAndInsertIntoLib(nMaxPicID, pPage->GetBKImageID());
			pInsertPage->SetBKImageID(nBkImgIdNew);
			Image* pImage = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nBkImgIdNew, true);
			pInsertPage->SetBkImage(pImage);

			{
				list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
				list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
				bool bIsPic = true;
				int nNewId = 0;
				while (iter != deviceList.end())
				{
					CEqmDrawDevice* pDevice = new CEqmDrawDevice((*iter)->GetEqmType(), Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetDataComID(), (*iter)->GetRotateAngle(), (*iter)->GetEqmName(), (*iter)->GetEquipmentSubType(), (*iter)->GetWidth(), "", (*iter)->GetHeight(), 0.0f, 0.0f, 0.0f,(*iter)->GetBindType());
					ASSERT(pDevice);
					pDevice->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));//important
					pDevice->SetLayer((*iter)->GetLayer());
					CString strPoint = (*iter)->GetPhysicalID().c_str();

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
					pDevice->m_eventInfo.strSettingPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->m_eventInfo.strSettingPointName.GetString()).c_str();
					// import page link
					wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->m_eventInfo.nPageID);
					if (wstrPageName != L"")
					{
						int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
						if (-1 != nPageId)
						{
							pDevice->m_eventInfo.nPageID = nPageId;
						}
					}

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
					vector<CString> vec = (*iter)->GetDataPointList();
					for(size_t i=0; i<vec.size(); i++)
					{
						vec[i] = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].GetString()).c_str();
					}
					pPipe->SetDataPointList(vec);

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
						(*iter)->GetHeight(), (*iter)->GetFontSize(), (*iter)->GetFontName(), CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()),(*iter)->GetShowString(), (*iter)->GetFontColor(),
						(*iter)->GetTextShowMode(), (*iter)->GetTransparence(),(*iter)->GetBorderType(),(*iter)->GetBorderColor(), (*iter)->GetDeviceCfg(), (*iter)->GetFaultDiagnosis(),(*iter)->GetBindScript());
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
					pText->SetDateTimePoint(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetDateTimePoint().GetString()).c_str());
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
							vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
							vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
							vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
						pButton->SetBindSetPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindSetPointName().GetString()).c_str());
						pButton->SetSettingValue((*iter)->GetSettingValue());
						pButton->SetRelation((*iter)->GetRelation());
						pButton->SetDownloadEnable((*iter)->GetDownloadEnable());
						pButton->SetDownloadName((*iter)->GetDownloadName());

						// import page link
						wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->GetLinkPageID());
						if (wstrPageName != L"")
						{
							int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
							if (-1 != nPageId)
							{
								pButton->SetLinkPageID(nPageId);
							}
						}

						vector<_tagRelationExpress> vec = (*iter)->GetRelationVec();
						for(int i=0; i<vec.size(); i++)
						{
							vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
						pTimeSelect->SetPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetPointName().GetString()).c_str());
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
						pTimeBar->SetBindPointStart(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointStart().GetString()).c_str());
						pTimeBar->SetQueryType((*iter)->GetQueryType());
						pTimeBar->SetBindPointStatus(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointStatus().GetString()).c_str());
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
						pProgressBar->SetBindPoint(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPoint().GetString()).c_str());
						pProgressBar->SetBindPointBase(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointBase().GetString()).c_str());
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

						vector<_tagDiagnoseItem>& vec = (*iter)->GetItemVec();
						for (size_t i=0;i<vec.size();++i)
						{
							vec[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();

							vector<_tagDiagnoseItemInLine>& vecItem = vec[i].vecItemInLine;
							for(size_t j=0; j<vecItem.size(); j++)
							{
								vecItem[i].strPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vecItem[i].strPointName.GetString()).c_str();
							}
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

					//pLiquidLevel->SetBindPoint((*iter)->GetBindPoint());
					pLiquidLevel->SetBindPoint(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPoint().GetString()).c_str());
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
					pChkBtnPlus->SetBindPointName(CreatePointIfNotFind(pFrame->m_pPrjSqlite,(*iter)->GetBindPointName().GetString()).c_str());
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
					vector<HistoryTrendInfo> vec = (*iter)->GetHistoryTrendVector();
					for(int i=0; i<vec.size(); ++i)
					{
						vec[i].strBindPointName = CreatePointIfNotFind(pFrame->m_pPrjSqlite,vec[i].strBindPointName.GetString()).c_str();
					}
					pHistoryTrend->SetHistoryTrendVector(vec);

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

					pMapNavigate->SetScaleRuler((*iter)->GetScaleRuler());
					pMapNavigate->SetMapPageInfo((*iter)->GetMapPageInfo());
					pMapNavigate->SetBanOnDisplay((*iter)->GetBanOnDisplay());

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

			if(strNum.GetLength()>0 && dlg.m_strReplacePointName.GetLength()>0)
			{
				strOut.Format(_T("正在替换点位:(原有点名包含%s的点替换为%s)"),m_strPreName,m_strReplaceName);
				GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
				CString strResult;
				ReplacePagePoint(pInsertPage,m_strPreName,m_strReplaceName,strResult);
			}

			strOut.Format(_T("已导入完成模板页图元,挂载页面到列表..."),nPageNum,pInsertPage->GetPageName().c_str());
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);

		/*	HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(pInsertPage->GetPageName().c_str(), m_hItem, nID);
			pView->m_pTreeDlg->m_treectrl.Expand(m_hItem, TVE_EXPAND);
			pView->m_pTreeDlg->m_treectrl.SetFocus();
			pView->m_pTreeDlg->m_treectrl.SelectItem(hPage);*/

			HTREEITEM hPage = pFrame->m_wndFileView.InsertPage(pInsertPage->GetPageName().c_str(), m_hItem, nID);
			pFrame->m_wndFileView.m_wndFileView.Expand(m_hItem, TVE_EXPAND);
			pFrame->m_wndFileView.m_wndFileView.SetFocus();
			pFrame->m_wndFileView.m_wndFileView.SelectItem(hPage);

			//CString strItemText = pView->m_pTreeDlg->m_treectrl.GetItemText(m_hItem);
			CString strItemText = pFrame->m_wndFileView.m_wndFileView.GetItemText(m_hItem);
			const int nGroupID = pFrame->m_project.GetGroupIDByGroupName(strItemText.GetString());
			pInsertPage->SetGroupID(nGroupID);
			pFrame->m_DrawContext.pPage = pInsertPage;
			pFrame->m_DrawContext.pPage->SetGroupID(nGroupID);
			pInsertPage->m_bDetailLoaded = true;
			pInsertPage->SetPageShowTopInObserver(pPage->GetPageShowTopInObserver());
			pFrame->m_project.AddPage(pInsertPage);
			pFrame->m_MaxPageID++;
		}
	}
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("从模板导入页面完成..."));
	EnableControl(true);
	pFrame->SetSaveFlag(true);
}

void CImportTemplatePageDlg::ImportSelectPages_CurrentPageResource_Batch_Ex(const int nMaxPicID)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());
	ASSERT(pView);

	const int nCount = m_list_page.GetItemCount();
	int nPageNum = 0;
	CString strOut;
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("开始导入模板页面，请稍候..."));
	EnableControl(false);

	//
	const int nPageID = m_nSelectPageID;
	CEqmDrawPage* pPage = m_project.GetPageByID(nPageID);
	if(!m_bResourceChanged)
	{
		GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("正在初始化模板图片资源..."));
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
		m_vecImportPageID.clear();
		m_vecNewPicID.clear();
		m_vecNewAniID.clear();
		m_bResourceChanged = true;
	}

	strOut.Format(_T("正在加载模板页面:%s"),pPage->GetPageName().c_str());
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
	if(!pPage->m_bDetailLoaded)
		m_project_sqlite->LoadPageDetails(pPage,NULL,NULL);

	//遍历点位查找数字
	CString strNum = FindNumInPageBindPoint(pPage);

	CImportTemplateBatchSettingExDlg dlg;
	dlg.m_strModelSrcName = pPage->GetPageName().c_str();
	if(dlg.DoModal() == IDOK)
	{
		const CString strFlag1(_T("*"));
		const CString strFlag2(_T("@"));
		const CString strFlag3(_T("$"));

		HoldInfo holdInfo1 = ParseHolderInfo(dlg.m_strBatchHolderNum1);
		HoldInfo holdInfo2 = ParseHolderInfo(dlg.m_strBatchHolderNum2);
		HoldInfo holdInfo3 = ParseHolderInfo(dlg.m_strBatchHolderNum3);

		m_strPreName1 = dlg.m_strPointNameFrom1;
		m_strPreName2 = dlg.m_strPointNameFrom2;
		m_strPreName3 = dlg.m_strPointNameFrom3;

		vector<HoldContain>	vecContain;
		vector<HoldContain>::const_iterator	iterContain;

		if (holdInfo1.nLength > 0)
		{
			if (holdInfo2.nLength > 0)
			{
				if (holdInfo3.nLength > 0)
				{	// 1+2+3
					for (int i=0; i<holdInfo1.nLength; i++)
					{
						for (int j=0; j<holdInfo2.nLength; j++)
						{
							for (int k=0; k<holdInfo3.nLength; k++)
							{
								CString strPageName(dlg.m_strBatchNewPageName);
								CString	strRepName1(dlg.m_strPointNameTo1);
								CString	strRepName2(dlg.m_strPointNameTo2);
								CString	strRepName3(dlg.m_strPointNameTo3);

								CString strH1 = GetHolderInfoText(holdInfo1, i);
								strPageName.Replace(strFlag1, strH1);
								strRepName1.Replace(strFlag1, strH1);

								CString strH2 = GetHolderInfoText(holdInfo2, j);
								strPageName.Replace(strFlag2, strH2);
								strRepName2.Replace(strFlag2, strH2);

								CString strH3 = GetHolderInfoText(holdInfo3, k);
								strPageName.Replace(strFlag3, strH3);
								strRepName3.Replace(strFlag3, strH3);

								HoldContain contain;
								contain.strPageName = strPageName;
								contain.strReplaceName1 = strRepName1;
								contain.strReplaceName2 = strRepName2;
								contain.strReplaceName3 = strRepName3;
								vecContain.push_back(contain);
							}
						}
					}
				}
				else
				{	// 1+2
					for (int i=0; i<holdInfo1.nLength; i++)
					{
						for (int j=0; j<holdInfo2.nLength; j++)
						{
							CString strPageName(dlg.m_strBatchNewPageName);
							CString	strRepName1(dlg.m_strPointNameTo1);
							CString	strRepName2(dlg.m_strPointNameTo2);

							CString strH1 = GetHolderInfoText(holdInfo1, i);
							strPageName.Replace(strFlag1, strH1);
							strRepName1.Replace(strFlag1, strH1);

							CString strH2 = GetHolderInfoText(holdInfo2, j);
							strPageName.Replace(strFlag2, strH2);
							strRepName2.Replace(strFlag2, strH2);

							HoldContain contain;
							contain.strPageName = strPageName;
							contain.strReplaceName1 = strRepName1;
							contain.strReplaceName2 = strRepName2;
							vecContain.push_back(contain);
						}
					}
				}
			}
			else
			{	// 1
				for (int i=0; i<holdInfo1.nLength; i++)
				{
					CString strPageName(dlg.m_strBatchNewPageName);
					CString	strRepName1(dlg.m_strPointNameTo1);

					CString strH1 = GetHolderInfoText(holdInfo1, i);
					strPageName.Replace(strFlag1, strH1);
					strRepName1.Replace(strFlag1, strH1);

					HoldContain contain;
					contain.strPageName = strPageName;
					contain.strReplaceName1 = strRepName1;
					vecContain.push_back(contain);
				}
			}
		}

		for (iterContain=vecContain.begin(); iterContain!=vecContain.end(); iterContain++)
		{
			CString strNewModelName(iterContain->strPageName);
			m_strReplaceName1 = iterContain->strReplaceName1;
			m_strReplaceName2 = iterContain->strReplaceName2;
			m_strReplaceName3 = iterContain->strReplaceName3;

			// do import
			strOut = _T("正在导入模板页图元...");
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);

			int nID = pFrame->m_MaxPageID+IMPORT_PAGE_ELEMENT_ID_START; 
			int nPageMaxOrder = pFrame->m_project.GetMaxPageShowOrder() + 1;
			CEqmDrawPage* pInsertPage = new CEqmDrawPage(nID,strNewModelName.GetString(),pPage->GetProjectID(),nPageMaxOrder,pPage->GetPageType(),pPage->GetPageWidth(),pPage->GetPageHeight(),pPage->GetPagexPosition(),pPage->GetPageyPosition(),pPage->GetStartColor());

			// insert background image
			int nBkImgIdNew = GetNewPicIDAndInsertIntoLib(nMaxPicID, pPage->GetBKImageID());
			pInsertPage->SetBKImageID(nBkImgIdNew);
			Image* pImage = pFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nBkImgIdNew, true);
			pInsertPage->SetBkImage(pImage);

			{
				list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
				list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
				bool bIsPic = true;
				int nNewId = 0;
				while (iter != deviceList.end())
				{
					CEqmDrawDevice* pDevice = new CEqmDrawDevice((*iter)->GetEqmType(), Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetDataComID(), (*iter)->GetRotateAngle(), (*iter)->GetEqmName(), (*iter)->GetEquipmentSubType(), (*iter)->GetWidth(), "", (*iter)->GetHeight(), 0.0f, 0.0f, 0.0f,(*iter)->GetBindType());
					ASSERT(pDevice);
					pDevice->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++pFrame->m_MaxEqmtID));//important
					pDevice->SetLayer((*iter)->GetLayer());
					CString strPoint = (*iter)->GetPhysicalID().c_str();

					nNewId = GetNewPicIDAndInsertIntoLib(nMaxPicID,(*iter)->GetPicID());
					pDevice->SetPicID(nNewId);
					pDevice->SetPhysicalID(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()));
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
					pDevice->m_eventInfo.strSettingPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->m_eventInfo.strSettingPointName.GetString()).c_str();
					// import page link
					wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->m_eventInfo.nPageID);
					if (wstrPageName != L"")
					{
						int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
						if (-1 != nPageId)
						{
							pDevice->m_eventInfo.nPageID = nPageId;
						}
					}

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
					vector<CString> vec = (*iter)->GetDataPointList();
					for(size_t i=0; i<vec.size(); i++)
					{
						vec[i] = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vec[i].GetString()).c_str();
					}
					pPipe->SetDataPointList(vec);

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
						(*iter)->GetHeight(), (*iter)->GetFontSize(), (*iter)->GetFontName(), CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetPhysicalID()),(*iter)->GetShowString(), (*iter)->GetFontColor(),
						(*iter)->GetTextShowMode(), (*iter)->GetTransparence(),(*iter)->GetBorderType(),(*iter)->GetBorderColor(), (*iter)->GetDeviceCfg(), (*iter)->GetFaultDiagnosis(),(*iter)->GetBindScript());
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
					pText->SetDateTimePoint(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetDateTimePoint().GetString()).c_str());
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
						pDashBoard->SetBindPointName(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindPointName().GetString()).c_str());
						pDashBoard->SetMaxString((*iter)->GetMaxString());
						pDashBoard->SetMinString((*iter)->GetMinString());
						pDashBoard->SetMaxBindFlag((*iter)->GetMaxBindFlag());
						pDashBoard->SetMinBindFlag((*iter)->GetMinBindFlag());
						pDashBoard->SetFontColor((*iter)->GetFontColor());
						pDashBoard->SetDecimalPlaces((*iter)->GetDecimalPlaces());
						CString	strImgPath;
						if ((*iter)->GetStyle() == 0)
						{
							strImgPath = m_strSysPath + L"\\Images\\dial1.png";
						}
						else if ((*iter)->GetStyle() == 1)
						{
							strImgPath = m_strSysPath + L"\\Images\\dial2.png";
						}
						else if ((*iter)->GetStyle() == 2)
						{
							strImgPath = m_strSysPath + L"\\Images\\dial3.png";
						}
						else
						{
							strImgPath = m_strSysPath + L"\\Images\\dial1.png";
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
							vec[i].strPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
							vec[i].strPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
							vec[i].strPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
						pButton->SetBindSetPointName(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindSetPointName().GetString()).c_str());
						pButton->SetSettingValue((*iter)->GetSettingValue());
						pButton->SetRelation((*iter)->GetRelation());
						pButton->SetDownloadEnable((*iter)->GetDownloadEnable());
						pButton->SetDownloadName((*iter)->GetDownloadName());

						// import page link
						wstring wstrPageName = m_project.GetPageNameByPageID((*iter)->GetLinkPageID());
						if (wstrPageName != L"")
						{
							int nPageId = pFrame->m_project.GetPageIDByPageName(wstrPageName);
							if (-1 != nPageId)
							{
								pButton->SetLinkPageID(nPageId);
							}
						}

						vector<_tagRelationExpress> vec = (*iter)->GetRelationVec();
						for(int i=0; i<vec.size(); i++)
						{
							vec[i].strPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();
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
						pTimeSelect->SetPointName(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetPointName().GetString()).c_str());
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
						pTimeBar->SetBindPointStart(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindPointStart().GetString()).c_str());
						pTimeBar->SetQueryType((*iter)->GetQueryType());
						pTimeBar->SetBindPointStatus(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindPointStatus().GetString()).c_str());
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
						pProgressBar->SetBindPoint(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindPoint().GetString()).c_str());
						pProgressBar->SetBindPointBase(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindPointBase().GetString()).c_str());
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

						vector<_tagDiagnoseItem>& vec = (*iter)->GetItemVec();
						for (size_t i=0;i<vec.size();++i)
						{
							vec[i].strPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vec[i].strPointName.GetString()).c_str();

							vector<_tagDiagnoseItemInLine>& vecItem = vec[i].vecItemInLine;
							for(size_t j=0; j<vecItem.size(); j++)
							{
								vecItem[i].strPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vecItem[i].strPointName.GetString()).c_str();
							}
							pDiagnoseGraphic->InsertItem(vec[i]);
						}

						pInsertPage->AddDiagnoseGraphic(pDiagnoseGraphic);
						iter++;
					}
				}
			}
			{
				list<CEqmDrawPolygon*>& DiagnoseGraphicListPlus = pPage->GetPolygon();
				list<CEqmDrawPolygon*>::iterator iter = DiagnoseGraphicListPlus.begin();
				while (iter != DiagnoseGraphicListPlus.end())
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

					//pLiquidLevel->SetBindPoint((*iter)->GetBindPoint());
					pLiquidLevel->SetBindPoint(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindPoint().GetString()).c_str());
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
					pChkBtnPlus->SetBindPointName(CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,(*iter)->GetBindPointName().GetString()).c_str());
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
					vector<HistoryTrendInfo> vec = (*iter)->GetHistoryTrendVector();
					for(int i=0; i<vec.size(); ++i)
					{
						vec[i].strBindPointName = CreatePointIfNotFindEx(pFrame->m_pPrjSqlite,vec[i].strBindPointName.GetString()).c_str();
					}
					pHistoryTrend->SetHistoryTrendVector(vec);

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

					pMapNavigate->SetScaleRuler((*iter)->GetScaleRuler());
					pMapNavigate->SetMapPageInfo((*iter)->GetMapPageInfo());
					pMapNavigate->SetBanOnDisplay((*iter)->GetBanOnDisplay());

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

			CString strResult;
			if (strNum.GetLength()>0 && m_strReplaceName1.GetLength()>0)
			{
				strOut.Format(_T("正在替换点位:(原有点名包含%s的点替换为%s)"), m_strPreName1, m_strReplaceName1);
				GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
				ReplacePagePoint(pInsertPage, m_strPreName1, m_strReplaceName1, strResult);
			}
			if (strNum.GetLength()>0 && m_strReplaceName2.GetLength()>0)
			{
				strOut.Format(_T("正在替换点位:(原有点名包含%s的点替换为%s)"), m_strPreName2, m_strReplaceName2);
				GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
				ReplacePagePoint(pInsertPage, m_strPreName2, m_strReplaceName2, strResult);
			}
			if (strNum.GetLength()>0 && m_strReplaceName3.GetLength()>0)
			{
				strOut.Format(_T("正在替换点位:(原有点名包含%s的点替换为%s)"), m_strPreName3, m_strReplaceName3);
				GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);
				ReplacePagePoint(pInsertPage, m_strPreName3, m_strReplaceName3, strResult);
			}

			strOut.Format(_T("已导入完成模板页图元,挂载页面到列表..."),nPageNum,pInsertPage->GetPageName().c_str());
			GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(strOut);

		/*	HTREEITEM hPage = pView->m_pTreeDlg->InsertPage(pInsertPage->GetPageName().c_str(), m_hItem, nID);
			pView->m_pTreeDlg->m_treectrl.Expand(m_hItem, TVE_EXPAND);
			pView->m_pTreeDlg->m_treectrl.SetFocus();
			pView->m_pTreeDlg->m_treectrl.SelectItem(hPage);*/

			HTREEITEM hPage = pFrame->m_wndFileView.InsertPage(pInsertPage->GetPageName().c_str(), m_hItem, nID);
			pFrame->m_wndFileView.m_wndFileView.Expand(m_hItem, TVE_EXPAND);
			pFrame->m_wndFileView.m_wndFileView.SetFocus();
			pFrame->m_wndFileView.m_wndFileView.SelectItem(hPage);

			//CString strItemText = pView->m_pTreeDlg->m_treectrl.GetItemText(m_hItem);
			CString strItemText = pFrame->m_wndFileView.m_wndFileView.GetItemText(m_hItem);
			const int nGroupID = pFrame->m_project.GetGroupIDByGroupName(strItemText.GetString());
			pInsertPage->SetGroupID(nGroupID);
			pFrame->m_DrawContext.pPage = pInsertPage;
			pFrame->m_DrawContext.pPage->SetGroupID(nGroupID);
			pInsertPage->m_bDetailLoaded = true;
			pInsertPage->SetPageShowTopInObserver(pPage->GetPageShowTopInObserver());
			pFrame->m_project.AddPage(pInsertPage);
			pFrame->m_MaxPageID++;
		}
	}
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(_T("从模板导入页面完成..."));
	EnableControl(true);
	pFrame->SetSaveFlag(true);
}

void CImportTemplatePageDlg::OnBnClickedButtonReplacePoint()
{
	// TODO: Add your control notification handler code here
}

void CImportTemplatePageDlg::OnCbnSelchangeComboProject()
{
	// TODO: Add your control notification handler code here
	CString strFileName;
	m_comProjectList.GetLBText(m_comProjectList.GetCurSel(),strFileName);
	ShowTemplatePagesByName(strFileName);
}

void CImportTemplatePageDlg::FindAllS3dbFile( CString strFilePath )
{
	m_vecTemplateFiles.clear();
	CFileFind fileFinder;

	if (strFilePath.Right(1) == TEXT("\\"))
	{
		int nPos  = strFilePath.ReverseFind(TEXT('\\'));
		strFilePath = strFilePath.Left(nPos);
	}

	CString strPicFile = TEXT("");
	strPicFile.Format(TEXT("%s\\%s"),strFilePath,TEXT("*.*"));

	BOOL bWorking = fileFinder.FindFile(strPicFile);
	while (bWorking)
	{   
		bWorking = fileFinder.FindNextFile();
		if (fileFinder.IsDots())
		{
			continue;
		}

		CString strFilePath = fileFinder.GetFilePath();
		if (fileFinder.IsDirectory())
		{   
			//继续遍历目录
			FindAllS3dbFile(strFilePath);
		}
		else
		{   
			int nPos = strFilePath.ReverseFind(TEXT('.'));
			CString strExt = strFilePath.Right(strFilePath.GetLength() - nPos - 1);
			if (strExt.CompareNoCase(TEXT("s3db"))  == 0)
			{   
				CString strFileName;
				nPos = strFilePath.ReverseFind(TEXT('\\'));
				strFileName = strFilePath.Right(strFilePath.GetLength() - nPos - 1);
				m_vecTemplateFiles.push_back(strFileName);
			}
		}
	}
	fileFinder.Close();
}

void CImportTemplatePageDlg::ShowTemplatePagesByName( CString strFileName )
{
	if(strFileName.GetLength() > 0)
	{
		m_strPath = m_strSysPath + _T("\\template\\");
		m_strPath += strFileName;

		CLogicPointManager			logicPotManager;
		CAlarmPointManager			AlarmPotManager;
		m_project.ClearProject();
		m_vecImportNewPicID.clear();

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
		m_project_sqlite->SetFilePath(m_strPath.GetString());
		m_project_sqlite->SetOwner(eFactory);
		m_project_sqlite->LoadProjectPages();
		m_project_sqlite->LoadProjectSqlite_opcid3();
		vector<CEqmDrawPage*>& page_list = m_project.GetPageList();
		m_list_page.DeleteAllItems();
		for (size_t i=0;i<page_list.size();++i)
		{
			m_list_page.InsertItem(i,L"");
			CString strTemp;
			strTemp.Format(L"%d",page_list[i]->GetID());
			m_list_page.SetItemText(i,0,strTemp);
			m_list_page.SetItemText(i,1,page_list[i]->GetPageName().c_str());
		}
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_bResourceChanged = false;
	}
}

std::wstring CImportTemplatePageDlg::Replace( const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced )
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

bool CImportTemplatePageDlg::ReplacePagePoint( CEqmDrawPage *pPage, CString strOld, CString strNew , CString& strResult)
{
	if(pPage && strOld.GetLength()>0)
	{
		int nReplaceCount = 0;
		//替换
		wstring strSourcePointName = strOld.GetString();
		wstring strReplacePointName = strNew.GetString();
		wstring strNewPointName;
		bool bReplace = false;
		{
			list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
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
			list<CEqmDrawPipe*>& deviceList = pPage->GetPagePipeList();
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
			list<CEqmDrawText*>& textList = pPage->GetPageTextList();
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
				iter++;
			}
		}
		{
			list<CEqmDrawDashBoard*>& dashboardList = pPage->GetDashboardList();
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
			list<CEqmDrawLineGraphic*>& linechartList = pPage->GetLineGraphList();
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
			list<CEqmDrawBarGraphic*>& barchartList = pPage->GetBarGraphList();
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
			list<CEqmDrawPieGraphic*>& piechartList = pPage->GetPieGraphList();
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
			list<CEqmDrawButton*>& buttonList = pPage->GetButtonList();
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
						nReplaceCount++;
					}
				}
				iter++;			
			}
		}
		{
			list<CEqmDrawTimeBar*>& timeBarList = pPage->GetTimeBarList();
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
			list<CEqmDrawTimeSelect*>& timeSelectList = pPage->GetTimeSelectList();
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
			list<CEqmDrawProgressBar*>& ProgressBarList = pPage->GetProgressBarList();
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
			list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = pPage->GetDiagnoseList();
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
		{
			list<CEqmDrawPolygon*>& polygonList = pPage->GetPolygon();
			list<CEqmDrawPolygon*>::iterator iter = polygonList.begin();
			while (iter != polygonList.end())
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
		{
			list<CEqmDrawLiquidLevel*>& liquidLevelList = pPage->GetLiquidLevelList();
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
		{
			list<CEqmDrawButtonPlus*>& chkBtnPlusList = pPage->GetCheckButtonPlusList();
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
			list<CEqmDrawScrawl*>& scrawlList = pPage->GetScrawlList();
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
		{
			list<CEqmDrawHistoryTrend*>& historyTrendList = pPage->GetHistoryTrendList();
			list<CEqmDrawHistoryTrend*>::iterator iter = historyTrendList.begin();
			while (iter != historyTrendList.end())
			{
				vector<HistoryTrendInfo> vec = (*iter)->GetHistoryTrendVector();
				for (size_t i=0;i<vec.size();++i)
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
		{
			list<CEqmDrawEnergySaveRoi*>& energySaveRoiList = pPage->GetEnergySaveRoiList();
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
		strOut.Format(_T("页面(%s)共替换%d处点名!\r\n"),pPage->GetPageName().c_str(),nReplaceCount);
		strResult += strOut;

		return true;
	}
	return false;
}

CString CImportTemplatePageDlg::FindNumInPageBindPoint( CEqmDrawPage *pPage )
{
	CString strNum;
	if(pPage)
	{
		{
			list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
			list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
			while (iter != deviceList.end())
			{
				if(FindNumInPoint((*iter)->GetPhysicalID(),strNum))
				{
					return strNum;
				}

				if(FindNumInPoint((*iter)->GetEventInfo().strSettingPointName.GetString(),strNum))
				{
					return strNum;
				}
				iter++;
			}
		}
		{
			list<CEqmDrawPipe*>& deviceList = pPage->GetPagePipeList();
			list<CEqmDrawPipe*>::iterator iter = deviceList.begin();
			while (iter != deviceList.end())
			{
				vector<CString> vec = (*iter)->GetDataPointList();
				for(size_t i=0; i<vec.size(); i++)
				{
					if(FindNumInPoint(vec[i].GetString(),strNum))
					{
						return strNum;
					}
				}
				iter++;
			}
		}
		{
			list<CEqmDrawText*>& textList = pPage->GetPageTextList();
			list<CEqmDrawText*>::iterator iter = textList.begin();
			while (iter != textList.end())
			{
				if(FindNumInPoint((*iter)->GetPhysicalID(),strNum))
				{
					return strNum;
				}

				if(FindNumInPoint((*iter)->GetDateTimePoint().GetString(),strNum))
				{
					return strNum;
				}
				iter++;
			}
		}
		{
			list<CEqmDrawDashBoard*>& dashboardList = pPage->GetDashboardList();
			list<CEqmDrawDashBoard*>::iterator iter = dashboardList.begin();
			while (iter != dashboardList.end())
			{	
				if(FindNumInPoint((*iter)->GetBindPointName().GetString(),strNum))
				{
					return strNum;
				}
				iter++;				
			}
		}
		{
			list<CEqmDrawLineGraphic*>& linechartList = pPage->GetLineGraphList();
			list<CEqmDrawLineGraphic*>::iterator iter = linechartList.begin();
			while (iter != linechartList.end())
			{				
				vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
				for (size_t i=0;i<vec.size();++i)
				{
					if(FindNumInPoint(vec[i].strPointName.GetString(),strNum))
					{
						return strNum;
					}
				}
				iter++;			
			}
		}
		{
			list<CEqmDrawBarGraphic*>& barchartList = pPage->GetBarGraphList();
			list<CEqmDrawBarGraphic*>::iterator iter = barchartList.begin();
			while (iter != barchartList.end())
			{
				vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
				for (size_t i=0;i<vec.size();++i)
				{
					if(FindNumInPoint(vec[i].strPointName.GetString(),strNum))
					{
						return strNum;
					}
				}
				iter++;		
			}
		}
		{
			list<CEqmDrawPieGraphic*>& piechartList = pPage->GetPieGraphList();
			list<CEqmDrawPieGraphic*>::iterator iter = piechartList.begin();
			while (iter != piechartList.end())
			{
				vector<_Graph_Item_Property>& vec = (*iter)->GetItemVector();
				for (size_t i=0;i<vec.size();++i)
				{
					if(FindNumInPoint(vec[i].strPointName.GetString(),strNum))
					{
						return strNum;
					}
				}
				iter++;		
			}
		}
		{
			list<CEqmDrawButton*>& buttonList = pPage->GetButtonList();
			list<CEqmDrawButton*>::iterator iter = buttonList.begin();
			while (iter != buttonList.end())
			{
				if(FindNumInPoint((*iter)->GetBindSetPointName().GetString(),strNum))
				{
					return strNum;
				}

				vector<_tagRelationExpress>& vec = (*iter)->GetRelationVec();
				for(size_t i=0; i<vec.size(); i++)
				{
					if(FindNumInPoint(vec[i].strPointName.GetString(),strNum))
					{
						return strNum;
					}
				}
				iter++;			
			}
		}
		{
			list<CEqmDrawTimeBar*>& timeBarList = pPage->GetTimeBarList();
			list<CEqmDrawTimeBar*>::iterator iter = timeBarList.begin();
			while (iter != timeBarList.end())
			{
				if(FindNumInPoint((*iter)->GetBindPointStart().GetString(),strNum))
				{
					return strNum;
				}

				if(FindNumInPoint((*iter)->GetBindPointStatus().GetString(),strNum))
				{
					return strNum;
				}		
				iter++;			
			}
		}
		{
			list<CEqmDrawTimeSelect*>& timeSelectList = pPage->GetTimeSelectList();
			list<CEqmDrawTimeSelect*>::iterator iter = timeSelectList.begin();
			while (iter != timeSelectList.end())
			{
				if(FindNumInPoint((*iter)->GetPointName().GetString(),strNum))
				{
					return strNum;
				}			
				iter++;			
			}
		}
		{
			list<CEqmDrawDiagnoseGraphic*>& DiagnoseGraphicList = pPage->GetDiagnoseList();
			list<CEqmDrawDiagnoseGraphic*>::iterator iter = DiagnoseGraphicList.begin();
			while (iter != DiagnoseGraphicList.end())
			{			
				vector<_tagDiagnoseItem>& vec = (*iter)->GetItemVec();
				for (size_t i=0;i<vec.size();++i)
				{
					if(FindNumInPoint(vec[i].strPointName.GetString(),strNum))
					{
						return strNum;
					}

					vector<_tagDiagnoseItemInLine>& vecItem = vec[i].vecItemInLine;
					for(size_t j=0; j<vecItem.size(); j++)
					{
						if(FindNumInPoint(vecItem[j].strPointName.GetString(),strNum))
						{
							return strNum;
						}
					}
				}
				iter++;			
			}
		}
		{
			list<CEqmDrawPolygon*>& DiagnoseGraphicListPlus = pPage->GetPolygon();
			list<CEqmDrawPolygon*>::iterator iter = DiagnoseGraphicListPlus.begin();
			while (iter != DiagnoseGraphicListPlus.end())
			{			
/*				vector<_tagBoolBindInfo>& vec = (*iter)->GetBoolBindInfoVec();
				for (size_t i=0;i<vec.size();++i)
				{
					if(FindNumInPoint(vec[i].strBoolPointName.GetString(),strNum))
					{
						return strNum;
					}

					if(FindNumInPoint(vec[i].strResultPointName.GetString(),strNum))
					{
						return strNum;
					}

					if(FindNumInPoint(vec[i].strResultPointName2.GetString(),strNum))
					{
						return strNum;
					}

					if(FindNumInPoint(vec[i].strResultPointName3.GetString(),strNum))
					{
						return strNum;
					}

					if(FindNumInPoint(vec[i].strTreatmentStatePointName.GetString(),strNum))
					{
						return strNum;
					}
					
					vector<_tagDiagnoseItemPlus>& vecItem = vec[i].vecDiagnoseItemPlus;
					for(size_t j=0; j<vecItem.size(); j++)
					{
						if(FindNumInPoint(vecItem[j].strPointName.GetString(),strNum))
						{
							return strNum;
						}
					}
				}
				iter++;	*/		
			}
		}
		{
			list<CEqmDrawLiquidLevel*>& liquidLevelList = pPage->GetLiquidLevelList();
			list<CEqmDrawLiquidLevel*>::iterator iter = liquidLevelList.begin();
			while (iter != liquidLevelList.end())
			{
				if(FindNumInPoint((*iter)->GetBindPoint().GetString(),strNum))
				{
					return strNum;
				}			
				iter++;	
			}
		}
		{
			list<CEqmDrawButtonPlus*>& chkBtnPlusList = pPage->GetCheckButtonPlusList();
			list<CEqmDrawButtonPlus*>::iterator iter = chkBtnPlusList.begin();
			while (iter != chkBtnPlusList.end())
			{
				if(FindNumInPoint((*iter)->GetBindPointName().GetString(),strNum))
				{
					return strNum;
				}				
				iter++;	
			}
		}
		{
			list<CEqmDrawHistoryTrend*>& historyTrendList = pPage->GetHistoryTrendList();
			list<CEqmDrawHistoryTrend*>::iterator iter = historyTrendList.begin();
			while (iter != historyTrendList.end())
			{
				vector<HistoryTrendInfo> vec = (*iter)->GetHistoryTrendVector();
				for (size_t i=0;i<vec.size();++i)
				{
					if(FindNumInPoint(vec[i].strBindPointName.GetString(),strNum))
					{
						return strNum;
					}
				}
				iter++;		
			}
		}
		{
			list<CEqmDrawEnergySaveRoi*>& energySaveRoiList = pPage->GetEnergySaveRoiList();
			list<CEqmDrawEnergySaveRoi*>::iterator iter = energySaveRoiList.begin();
			while (iter != energySaveRoiList.end())
			{
				if(FindNumInPoint((*iter)->GetBindPointName().GetString(),strNum))
				{
					return strNum;
				}				
				iter++;	
			}
		}
		return _T("");
	}
	return _T("");
}

bool CImportTemplatePageDlg::FindNumInPoint( wstring strPoint ,CString& strNum)
{
	CString strPointName = strPoint.c_str();
	strNum = _T("");
	TCHAR cTemp;
	bool bFindNum = false;
	bool bIsCurNum = false;
	for (int i = 0; i < strPointName.GetLength(); ++i)
	{
		cTemp = strPointName.GetAt(i);
		if (cTemp >=_T('0') && cTemp<=_T('9'))
		{
			strNum.AppendChar(cTemp);
			bFindNum = true;
			bIsCurNum = true;
		}
		else
		{
			bIsCurNum = false;
		}

		if (bFindNum && !bIsCurNum)
		{	// 发现数字后，再发现非数字
			if (strNum.GetLength() > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	if(strNum.GetLength()>0)
		return true;
	return false;
}


void CImportTemplatePageDlg::OnNMClickListPageImport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_strSelectPageName = m_list_page.GetItemText(pNMItemActivate->iItem,1);
	m_nSelectPageID = _wtoi(m_list_page.GetItemText(pNMItemActivate->iItem,0));
}

int CImportTemplatePageDlg::GetNewAnimationIDAndInsertIntoLib( int nMaxAniID, int nAniID )
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

HoldInfo CImportTemplatePageDlg::ParseHolderInfo(const CString& strH)
{
	HoldInfo holdInfo;
	CString strHolder(strH);

	if (strHolder.Find(_T("-")) >= 0)
	{
		int nPos = strHolder.Find(_T("-"));
		int nLoopStart = _ttoi(strHolder.Left(nPos));
		int nLoopEnd = _ttoi(strHolder.Mid(nPos + 1));
		for (int i=nLoopStart; i<=nLoopEnd; i++)
		{
			holdInfo.vecInt.push_back(i);
		}
		holdInfo.nType = 0;
		holdInfo.nLength = holdInfo.vecInt.size();
	}
	else if (strHolder.Find(_T(",")) >= 0)
	{
		strHolder.TrimRight(_T(","));
		strHolder += _T(",");
		while (TRUE)
		{
			int nPos = strHolder.Find(_T(","));
			if (-1 == nPos)
			{
				break;
			}
			holdInfo.vecStr.push_back(strHolder.Left(nPos));
			strHolder = strHolder.Mid(nPos + 1);
		}
		holdInfo.nType = 1;
		holdInfo.nLength = holdInfo.vecStr.size();
	}
	else
	{
		// input format error
	}

	return holdInfo;
}

CString CImportTemplatePageDlg::GetHolderInfoText(const HoldInfo& holdInfo, int nCount)
{
	CString strRet;
	if (0 == holdInfo.nType)
	{
		int n = holdInfo.vecInt.at(nCount);
		strRet.Format(_T("%d"), n);
	}
	else if (1 == holdInfo.nType)
	{
		strRet = holdInfo.vecStr.at(nCount);
	}
	else
	{
		// error
	}
	return strRet;
}


void CImportTemplatePageDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
}
