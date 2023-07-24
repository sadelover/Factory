// TemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TemplateDlg.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "MainFrm.h"
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
#include "../ComponentDraw/EqmDrawDataAnalyse.h"
#include "../ComponentDraw/EqmDrawTemperDistri.h"
#include "../ComponentDraw/EqmDrawDataReport.h"
#include "../ComponentDraw/EqmDrawPlaneTempeDistr.h"
#include "../ComponentDraw/EqmDrawDottedLineFrm.h"
#include "../ComponentDraw/EqmDrawGeneralComponent.h"
#include "../ComponentDraw/EqmDrawRectangle.h"
#include "../ComponentDraw/EqmDrawStraightLine.h"

// CTemplateDlg dialog

IMPLEMENT_DYNAMIC(CTemplateDlg, CSkinDlg)

CTemplateDlg::CTemplateDlg(const DWORD_PTR dwSrcPageId, CEqmDrawPage* pDrawPage, CWnd* pParent /*=NULL*/)
	: CSkinDlg(CTemplateDlg::IDD, pParent)
	, m_strTemplateName(_T(""))
	, m_nRadioCount(0)
	, m_dwSrcPageId(dwSrcPageId)
	, m_pMainFrm(NULL)
	, m_pSqlite(NULL)
	, m_pDrawPage(pDrawPage)
{

}

CTemplateDlg::~CTemplateDlg()
{
	SAFE_DELETE(m_pSqlite);
}

void CTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_TEMPLATE, m_comboTemplate);
	DDX_Control(pDX, IDC_EDIT_TEMPLATE, m_editTemplateName);
	DDX_Text(pDX, IDC_EDIT_TEMPLATE, m_strTemplateName);
	DDX_Radio(pDX, IDC_RADIO_EXIST, m_nRadioCount);
}


BEGIN_MESSAGE_MAP(CTemplateDlg, CSkinDlg)
	ON_BN_CLICKED(IDC_RADIO_EXIST, &CTemplateDlg::OnBnClickedRadioExist)
	ON_BN_CLICKED(IDC_RADIO_ADD, &CTemplateDlg::OnBnClickedRadioAdd)
END_MESSAGE_MAP()


// CTemplateDlg message handlers


BOOL CTemplateDlg::OnInitDialog()
{
	CSkinDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();

	InitData();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTemplateDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if(!Project::Tools::IsStrCharacterOrNumberOrUnderline(m_strTemplateName))
	{
		AfxMessageBox(_T("模板名称只能是英文字母或数字及下划线！"));
		return;
	}


	if (0 == m_nRadioCount)
	{
		if (ImportToSelectDb())
		{
			MessageBox(_T("导入页面至已有模板成功！"), _T("成功"), MB_OK|MB_ICONASTERISK);
		}
	}
	else if (1 == m_nRadioCount)
	{
		if(m_strTemplateName.GetLength() <= 0)
		{
			MessageBox(_T("模板名字不能为空!"));
			return;
		}
		
		if(m_strTemplateName.Find(' ') >= 0)
		{
			MessageBox(_T("模板名字不能含有空格!"));
			return;
		}

		if (ImportToNewDb())
		{
			MessageBox(_T("导入页面至新建模板成功！"), _T("成功"), MB_OK|MB_ICONASTERISK);
		}
	}
	else
	{
		// error
		MessageBox(_T("界面数据错误！"), _T("错误"), MB_OK|MB_ICONERROR);
	}

	CSkinDlg::OnOK();
}

void CTemplateDlg::InitControls(void)
{
	CString			strFindFlag;
	CFileFind		finder;
	TemplateInf		stTemplInf;


	m_vecTemplInf.clear();

	GetSysPath(m_strSysPath);
	strFindFlag = m_strSysPath;
	strFindFlag += _T("\\template\\*.s3db");

	BOOL bWorking = finder.FindFile(strFindFlag);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
		{
			continue;
		}

		if (finder.IsDirectory())
		{
			continue;
		}

		stTemplInf.strName = finder.GetFileName();
		stTemplInf.strPathName = finder.GetFilePath();
		m_vecTemplInf.push_back(stTemplInf);
	}


	vector<TemplateInf>::const_iterator	iter;
	for (iter=m_vecTemplInf.begin(); iter!=m_vecTemplInf.end(); ++iter)
	{
		m_comboTemplate.AddString(iter->strName);
	}
	m_comboTemplate.SetCurSel(0);


	EnableControls(TRUE);
}

void CTemplateDlg::InitData(void)
{
	m_pMainFrm = (CMainFrame*)AfxGetApp()->GetMainWnd();
	ASSERT(m_pMainFrm != NULL);

	CDataPointManager	plcPotMgr;
	CLogicPointManager	logicPotMgr;
	CAlarmPointManager	AlarmPotMgr;
	m_pSqlite = new CProjectSqlite(m_project, plcPotMgr, logicPotMgr, AlarmPotMgr, 0);
	ASSERT(m_pSqlite != NULL);
}

void CTemplateDlg::OnBnClickedRadioExist()
{
	// TODO: Add your control notification handler code here
	EnableControls(TRUE);
}


void CTemplateDlg::OnBnClickedRadioAdd()
{
	// TODO: Add your control notification handler code here
	EnableControls(FALSE);
}

void CTemplateDlg::EnableControls(const BOOL bEnableExist)
{
	m_comboTemplate.EnableWindow(bEnableExist);
	m_editTemplateName.EnableWindow(!bEnableExist);
}

bool CTemplateDlg::ImportToSelectDb(void)
{
	CString	strTemplateName;
	int nCurSel = m_comboTemplate.GetCurSel();
	m_comboTemplate.GetLBText(nCurSel, strTemplateName);

	CString	strDstTemplPathName;
	strDstTemplPathName.Format(_T("%s\\template\\%s"), m_strSysPath, strTemplateName);

	//
	if (NULL == m_pSqlite)
	{
		return false;
	}
	m_pSqlite->SetFilePath(strDstTemplPathName.GetString());
	m_pSqlite->SetOwner(eFactory);


	// 载入目标s3db中各表数据
	m_pSqlite->LoadGroupPageSqlite();
	m_pSqlite->LoadProjectSqlite_pages();

	const wstring strPageName(m_pDrawPage->GetPageName());
	vector<CEqmDrawPage*>	vecPage(m_pSqlite->GetDrawProject().GetPageList());
	vector<CEqmDrawPage*>::const_iterator	iter;
	bool	bPageNameRepeat = false;
	for (iter=vecPage.begin(); iter!=vecPage.end(); ++iter)
	{
		if ((*iter)->GetPageName() == strPageName)
		{
			bPageNameRepeat = true;
			break;
		}
	}
	if (bPageNameRepeat)
	{
		if (IDNO == MessageBox(_T("目标模板已存在相同页名，确定要导入？"), _T("提示"), MB_YESNO|MB_ICONWARNING))
		{
			return false;
		}
	}


	const int nID	= m_pSqlite->GetMaxPageID() + 1;	// dest page id
	int nMaxPicId	= m_pSqlite->GetMaxLibImageNum() + 1;
	int	nMaxAniId	= m_pSqlite->GetMaxLibAnimationNum() + 1;
	int	nMaxAniPicId= m_pSqlite->GetMaxLibImageAnimationNum() + 1;


	// 插入各表中该页的数据
	vector<GroupPage>	vecGroupPage(m_pSqlite->GetDrawProject().m_groupgage);
	if (vecGroupPage.empty())
	{
		return false;
	}
	int nGroupId = 0;
	try
	{
		nGroupId = vecGroupPage.at(0).GroupID;
	}
	catch (...)
	{
		return false;
	}

	ASSERT(m_pDrawPage != NULL);
	if (NULL == m_pDrawPage)
	{
		return false;
	}

	if (!ImportLogical(m_pDrawPage, nID, nGroupId, nMaxPicId, nMaxAniId, nMaxAniPicId, strDstTemplPathName))
	{
		return false;
	}


	// 保存目标s3db
	m_pSqlite->SaveProjectSqlite_pages();


	return true;
}

bool CTemplateDlg::ImportToNewDb(void)
{
	UpdateData(TRUE);

	wstring	wstrTemplName;
	int nDotPos = m_strTemplateName.ReverseFind(_T('.'));
	if (nDotPos != -1)
	{
		m_strTemplateName.Left(nDotPos);
	}
	wstrTemplName = m_strTemplateName;
	m_strTemplateName += _T(".s3db");

	CString	strDstTemplPathName, strTemplatePP;
	CString	strSysCfgPathName;
	strTemplatePP.Format(_T("%s\\template"), m_strSysPath);
	strDstTemplPathName.Format(_T("%s\\template\\%s"), m_strSysPath, m_strTemplateName);
	strSysCfgPathName.Format(_T("%s\\config\\beopsys.s3db"), m_strSysPath);


	// create new s3db file

	if(!m_pSqlite->FolderExist(strTemplatePP))
	{
		m_pSqlite->CreateFolder(strTemplatePP);
	}

	if (0 == ::CopyFile(strSysCfgPathName, strDstTemplPathName, FALSE))
	{
		MessageBox(_T("创建模板文件失败！"), _T("错误"), MB_OK|MB_ICONERROR);
		return false;
	}

	//
	if (NULL == m_pSqlite)
	{
		return false;
	}
	m_pSqlite->SetFilePath(strDstTemplPathName.GetString());
	m_pSqlite->SetOwner(eFactory);

	m_project.ClearPageList();
	m_project.SetProjectID(1);
	m_project.SetProjectName(wstrTemplName);
	m_project.SetScreenWidth(1920);
	m_project.SetScreenHeight(955);
	m_project.SetVersion(L"2.2");
	m_project.AddGroup(L"Group1");

	//
	const int nID	= 1;	// dest page id
	int nGroupId	= 1;
	int nMaxPicId	= 1;
	int	nMaxAniId	= 1;
	int	nMaxAniPicId= 1;


	ASSERT(m_pDrawPage != NULL);
	if (NULL == m_pDrawPage)
	{
		return false;
	}

	if (!ImportLogical(m_pDrawPage, nID, nGroupId, nMaxPicId, nMaxAniId, nMaxAniPicId, strDstTemplPathName))
	{
		return false;
	}


	// 保存目标s3db
	m_pSqlite->SaveGroupPageSqlite();
	m_pSqlite->SaveProjectSqlite_pages();


	return true;
}

bool CTemplateDlg::InsertIntoImageLib(const int nSrcPicId, const int nDstPicId)
{
	USES_CONVERSION;

	PictureInfo* pSrcInfo = m_pMainFrm->m_pPrjSqlite->GetPictureInfoByID(nSrcPicId);
	if (NULL == pSrcInfo)
	{
		return false;
	}

	m_pSqlite->InsertPictureData(nDstPicId, T2A(pSrcInfo->szName), Project::Tools::WideCharToAnsi(pSrcInfo->etype_equip.c_str()).c_str(), pSrcInfo->pBlock, pSrcInfo->nSize, true);
	return true;
}

bool CTemplateDlg::InsertIntoAnimationLib(const int nSrcAniId, const int nDstAniId, int& nMaxAniPicId, const wstring strDbPathName)
{
	const AnimationInfo* pSrcInfo = m_pMainFrm->m_pPrjSqlite->GetAnimationByID(nSrcAniId);
	if (NULL == pSrcInfo)
	{
		return false;
	}

	USES_CONVERSION;

	// insert into lib_animation
	vector<int> vecId = pSrcInfo->vecPicIDList;
	vector<int>::const_iterator	iterId;

	// 将vecId组合成字符串
	CString	strList;
	CString strTemp;
	int nNum = nMaxAniPicId;
	for (iterId=vecId.begin(); iterId!=vecId.end(); ++iterId)
	{
		strTemp.Format(_T("%d,"), nNum/**iterId*/);
		strList += strTemp;
		++nNum;
	}
	strList.TrimRight(_T(","));

	int nLen = strList.GetLength() + 1;
	char* pList = new char[nLen];
	memset(pList, 0, nLen);
	strcpy_s(pList, nLen, T2A(strList.GetBuffer()));
	strList.ReleaseBuffer();

	m_pSqlite->InsertRecordToAnimationList(nDstAniId, T2A((pSrcInfo->szName).c_str()), pList, true);
	SAFE_DELETE_ARRAY(pList);

	// 插入到内存结构m_vAnimationList
	m_pSqlite->AddIntoAnimationList(nDstAniId, pSrcInfo, true);


	// insert into lib_image_animation
	PictureInfo* pPicInf = NULL;
	vector<PictureInfo>::iterator	iterPic;

	for (iterId=vecId.begin(); iterId!=vecId.end(); ++iterId)
	{
		pPicInf = m_pMainFrm->m_pPrjSqlite->GetPictureInfoByIDOfAnimation(*iterId, true);
		if (NULL == pPicInf)
		{
			continue;
		}

		CSqliteAcess access(strDbPathName.c_str(), m_pSqlite->GetFileEncryptType());
		access.BeginTransaction();
		//access.DeleteOnePictureForAnimation(pPicInf->ID);
		access.CommitTransaction();

		m_pSqlite->InsertPictureAnimationData(nMaxAniPicId/*pPicInf->ID*/, T2A(pPicInf->szName), Project::Tools::WideCharToAnsi(pPicInf->etype_equip.c_str()).c_str(),pPicInf->pBlock, pPicInf->nSize, true);

		// 插入到内存结构m_vPictureforAnimationList
		m_pSqlite->AddIntoAnimationImage(nMaxAniPicId/*pPicInf->ID*/, pPicInf, true);
		++nMaxAniPicId;
	}

	return true;
}

bool CTemplateDlg::ImportLogical(CEqmDrawPage* pPage, const int nID, const int nGroupId, int nMaxPicId, int nMaxAniId, int nMaxAniPicId, const CString strDbPathName)
{
	int nPageMaxOrder = m_pMainFrm->m_project.GetMaxPageShowOrder() + 1;
	CEqmDrawPage* pInsertPage = new CEqmDrawPage(nID,pPage->GetPageName(),pPage->GetProjectID(),nPageMaxOrder,pPage->GetPageType(),pPage->GetPageWidth(),pPage->GetPageHeight(),pPage->GetPagexPosition(),pPage->GetPageyPosition(),pPage->GetStartColor());
	ASSERT(pInsertPage != NULL);
	if (NULL == pInsertPage)
	{
		return false;
	}


	{
		list<CEqmDrawDevice*>& deviceList = pPage->GetDrawDeviceList();
		list<CEqmDrawDevice*>::iterator iter = deviceList.begin();
		while (iter != deviceList.end())
		{
			CEqmDrawDevice* pDevice = new CEqmDrawDevice((*iter)->GetEqmType(), Gdiplus::PointF((float)(*iter)->GetPos().X, (float)(*iter)->GetPos().Y), nID, (*iter)->GetID(), (*iter)->GetDataComID(), (*iter)->GetRotateAngle(), (*iter)->GetEqmName(), (*iter)->GetEquipmentSubType(), (*iter)->GetWidth(), "", (*iter)->GetHeight(), 0.0f, 0.0f, 0.0f,(*iter)->GetBindType());
			ASSERT(pDevice);
			pDevice->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));//important
			pDevice->SetLayer((*iter)->GetLayer());
			pDevice->SetPhysicalID((*iter)->GetPhysicalID());
			pDevice->SetDeviceValue((*iter)->GetDeviceValue());

			if ((*iter)->GetBindString().size()>0)
			{
				pDevice->SetBindString((*iter)->GetBindString());
				pDevice->ParseParam10((*iter)->GetBindString().c_str(), nMaxPicId);

				// import pic, animatino
				vector<BindInfo> vecBindInfo(pDevice->GetBindInfoVec());
				vector<BindInfo>::const_iterator	iter;
				int	nPicNum = 0;
				int nSrcPicNum = 0;
				int nNumFlag = nMaxPicId;
				for (iter=vecBindInfo.begin(); iter!=vecBindInfo.end(); ++iter)
				{
					nPicNum = iter->nPicID;
					nSrcPicNum = iter->nPicID - nNumFlag;

					if (InsertIntoImageLib(nSrcPicNum, nPicNum))
					{
						nMaxPicId = nPicNum + 1;
					}

					if (InsertIntoAnimationLib(nSrcPicNum, nPicNum, nMaxAniPicId, strDbPathName.GetString()))
					{
						nMaxAniId = nPicNum + 1;
					}
				}
			}

			pDevice->m_eventInfo = (*iter)->m_eventInfo;
			pDevice->SetLinkPageID((*iter)->GetLinkPageID() + nMaxPicId);
			pDevice->SetImageMetaType((*iter)->GetImageMetaType());
			Image* pBitmap = NULL;
			if (pDevice->GetImageMetaType())
			{
				pDevice->SetPicID(nMaxPicId);
				pBitmap = m_pMainFrm->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetPicID());
				if (pBitmap != NULL)
				{
					pDevice->SetBitmap(pBitmap);

					InsertIntoImageLib((*iter)->GetPicID(), nMaxPicId);
				}
				nMaxPicId++;
			}
			else
			{
				pDevice->SetPicID(nMaxAniId);
				pBitmap = m_pMainFrm->m_pPrjSqlite->GetBitmapByIdFromAnimationImageLib((*iter)->GetPicID());
				if (pBitmap != NULL)
				{
					pDevice->SetBitmap(pBitmap);

					InsertIntoAnimationLib((*iter)->GetPicID(), nMaxAniId, nMaxAniPicId, strDbPathName.GetString());
				}
				nMaxAniId++;
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
			pPipe->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				(*iter)->GetTextShowMode(),(*iter)->GetTransparence(),(*iter)->GetBorderType(),(*iter)->GetBorderColor(), (*iter)->GetDeviceCfg(),(*iter)->GetFaultDiagnosis(), (*iter)->GetBindScript());
			ASSERT(pText);
			pText->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
			pgrapic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pBend->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pDashBoard->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pLineGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pBarGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pPieGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pButton->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
				pButton->SetWidth((*iter)->GetWidth());
				pButton->SetHeight((*iter)->GetHeight());
				pButton->SetLayer((*iter)->GetLayer());
				int nComm = 0;
				int nDisable = 0;
				int nDown = 0;
				int nOver = 0;
				int nChecked = 0;
				(*iter)->Get5PicID(nComm,nDisable,nDown,nOver,nChecked);
				pButton->Set5PicID(nMaxPicId, nMaxPicId+1, nMaxPicId+2, nMaxPicId+3, nMaxPicId+4);
				pButton->SetLinkPageID((*iter)->GetLinkPageID()+ nMaxPicId);
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

				Image* pBitmap = m_pMainFrm->m_pPrjSqlite->GetBitmapByIDFromPictureLib(nComm);
				if (pBitmap != NULL)
				{
					pButton->SetDefaultBitmap(pBitmap);
					InsertIntoImageLib(nComm, nMaxPicId);
					InsertIntoImageLib(nDisable, nMaxPicId + 1);
					InsertIntoImageLib(nDown, nMaxPicId + 2);
					InsertIntoImageLib(nOver, nMaxPicId + 3);
					InsertIntoImageLib(nChecked, nMaxPicId + 4);
				}

				pInsertPage->AddButton(pButton);
				iter++;
				nMaxPicId += 5;
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
				pTimeSelect->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pTimeBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pProgressBar->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pDiagnoseGraphic->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
		list<CEqmDrawPolygon*>& DiagnoseGraphicListPlus = pPage->GetPolygon();
		list<CEqmDrawPolygon*>::iterator iter = DiagnoseGraphicListPlus.begin();
		while (iter != DiagnoseGraphicListPlus.end())
		{
			CEqmDrawPolygon* pDiagnoseGraphicPlus = new CEqmDrawPolygon();
			if(pDiagnoseGraphicPlus)
			{
				CEqmDrawPolygon* pPolygon = new CEqmDrawPolygon();
				if(pPolygon)
				{
					pPolygon->SetPointLIst((*iter)->GetPointList());
					pPolygon->SetEqmType((*iter)->GetEqmType());
					pPolygon->SetPageID(nID);
					pPolygon->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
				pAreaChange->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
				pAreaChange->SetWidth((*iter)->GetWidth());
				pAreaChange->SetHeight((*iter)->GetHeight());
				pAreaChange->SetLayer((*iter)->GetLayer());
				pAreaChange->SetPicID(nMaxPicId);
				Image* pBitmap = m_pMainFrm->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetPicID());
				if (pBitmap != NULL)
				{
					pAreaChange->SetBitmap(pBitmap);

					InsertIntoImageLib((*iter)->GetPicID(), nMaxPicId);
				}
				vector<_tagAreaInfo> vecArea = (*iter)->GetAreaInfo();
				for(int j=0; j<vecArea.size(); j++)
				{
					vecArea[j].BnmID = nMaxPicId + vecArea[j].BnmID;
					vecArea[j].nPageID = nMaxPicId + vecArea[j].nPageID;
				}

				pAreaChange->SetAreaInfo(vecArea);

				pInsertPage->AddAreaChange(pAreaChange);
				iter++;
				nMaxPicId++;
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
				pShellExecute->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
				pShellExecute->SetWidth((*iter)->GetWidth());
				pShellExecute->SetHeight((*iter)->GetHeight());
				pShellExecute->SetLayer((*iter)->GetLayer());

				pShellExecute->SetServerIP((*iter)->GetServerIP());
				pShellExecute->SetFileName((*iter)->GetFileName());
				pShellExecute->SetDownloadDirectory((*iter)->GetDownloadDirectory());
				pShellExecute->SetFileID((*iter)->GetFileID());
				pShellExecute->SetBKPicID(nMaxPicId);
				pShellExecute->SetAutoDownload((*iter)->GetAutoDownload());

				Image* pBitmap = m_pMainFrm->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetBKPicID());
				if (pBitmap != NULL)
				{
					pShellExecute->SetDefaultBitmap(pBitmap);
					InsertIntoImageLib((*iter)->GetBKPicID(), nMaxPicId);
				}
				pInsertPage->AddShellExecute(pShellExecute);
			}
			iter++;
			nMaxPicId++;
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
			pEmbededPage->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
			pEmbededPage->SetWidth((*iter)->GetWidth());
			pEmbededPage->SetHeight((*iter)->GetHeight());
			pEmbededPage->SetLayer((*iter)->GetLayer());
			pEmbededPage->SetPicID(nMaxPicId);
			pEmbededPage->SetChangeInterval((*iter)->GetChangeInterval());
			Image* pBitmap = m_pMainFrm->m_pPrjSqlite->GetBitmapByIDFromPictureLib((*iter)->GetPicID());
			if (pBitmap != NULL)
			{
				pEmbededPage->SetDefaultBitmap(pBitmap);
				InsertIntoImageLib((*iter)->GetPicID(), nMaxPicId);
			}
			vector<PageIDName>&	vec = (*iter)->GetPageList();
			for (size_t i=0;i<vec.size();++i)
			{
				pEmbededPage->InsertPageList(vec[i].nPageID, vec[i].strPageName);
			}
			pInsertPage->AddEmbededPage(pEmbededPage);
			++iter;
			++nMaxPicId;
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
			pLiquidLevel->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
			pChkBtnPlus->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
			pStraightLine->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
			pHistoryTrend->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
		list<CEqmDrawRectangle*>& energySaveRoiList = pPage->GetRectangleList();
		list<CEqmDrawRectangle*>::iterator iter = energySaveRoiList.begin();
		while (iter != energySaveRoiList.end())
		{
			CEqmDrawRectangle* pRectangle = new CEqmDrawRectangle();
			pRectangle->SetPos(Gdiplus::PointF((*iter)->GetPos().X, (*iter)->GetPos().Y));
			pRectangle->SetEqmType(EQMDRAW_RECTANGLE);
			pRectangle->SetPageID(nID);
			pRectangle->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
			pRectangle->SetWidth((*iter)->GetWidth());
			pRectangle->SetHeight((*iter)->GetHeight());
			pRectangle->SetLayer((*iter)->GetLayer());
			pRectangle->SetLineColor((*iter)->GetLineColor());
			pRectangle->SetLineWidth((*iter)->GetLineWidth());
			pRectangle->SetFillStyl((*iter)->GetFillStyl());
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
			pMapNavigate->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
			pNaviWind->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
			pLevelRuler->SetID(IMPORT_PAGE_ELEMENT_ID_START+(++m_pMainFrm->m_MaxEqmtID));
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
			pDataAnalyse->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++m_pMainFrm->m_MaxEqmtID));
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
			p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++m_pMainFrm->m_MaxEqmtID));
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
			p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++m_pMainFrm->m_MaxEqmtID));
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
			p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++m_pMainFrm->m_MaxEqmtID));
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
			p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++m_pMainFrm->m_MaxEqmtID));
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
			p->SetID(IMPORT_PAGE_ELEMENT_ID_START + (++m_pMainFrm->m_MaxEqmtID));
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
	pInsertPage->SetGroupID(nGroupId);
	pInsertPage->m_bDetailLoaded = true;
	pInsertPage->SetPageShowTopInObserver(pPage->GetPageShowTopInObserver());
	m_pSqlite->GetDrawProject().AddPage(pInsertPage);
	m_pMainFrm->m_MaxPageID++;


	return true;
}

