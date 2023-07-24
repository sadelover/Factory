// ImportSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImportSettingDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Tools/Util/UtilString.h"
#include "Tools/CustomTools/CustomTools.h"
// CImportSettingDlg dialog

IMPLEMENT_DYNAMIC(CImportSettingDlg, CDialog)

CImportSettingDlg::CImportSettingDlg(CProjectSqlite* project_sqlite, CWnd* pParent /*=NULL*/)
	: CDialog(CImportSettingDlg::IDD, pParent)
	, m_bConfigEsixt(false)
	, m_strPrjName(L"")
	, m_nID(0)
	, m_project_sqlite(project_sqlite)
	, m_bImporSelectResource(FALSE)
	, m_nRetId(0)
{

}

CImportSettingDlg::~CImportSettingDlg()
{
}

void CImportSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_nID);
	DDX_Check(pDX, IDC_CHECK_IMPORT_TYPE, m_bImporSelectResource);
}

BEGIN_MESSAGE_MAP(CImportSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_POINT, &CImportSettingDlg::OnBnClickedButtonReloadPoint)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD_IMAGE, &CImportSettingDlg::OnBnClickedButtonReloadImage)
	ON_BN_CLICKED(IDOK, &CImportSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImportSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CImportSettingDlg::OnBnClickedBtnImport)
END_MESSAGE_MAP()
// CImportSettingDlg message handlers


void CImportSettingDlg::OnBnClickedButtonReloadPoint()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_LOAD_POINT))->GetCheck();

	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);

	SetWindowText(_T("正在加载点表资源..."));
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_POINT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_IMAGE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	
	if(nCheck)		//重新加载 需删除原来的资源
	{
		int nStartID = m_nID*E_POINT_BASE_OFFSET;
		int nEndID = (m_nID+1)*E_POINT_BASE_OFFSET;
		pFrame->m_pPrjSqlite->DeleteBasePointByID(nStartID,nEndID);
	}
	else
	{
		m_setting.bLoadPoint = 1;
		pFrame->m_pPrjSqlite->UpdateImportSettingByID(m_nID,m_setting.bLoadPoint,m_setting.bLoadImage);
	}

	if(m_project_sqlite)
	{
		m_project_sqlite->LoadProjectSqlite_opcid3();
		vector<DataPointEntry>  pointlist = m_project_sqlite->GetPointList();

		for(int i=0; i<pointlist.size(); i++)
		{
			if(!pFrame->m_pPrjSqlite->IsPointExist(pointlist[i].GetShortName()))
			{
				int nNewID = pointlist[i].GetPointIndex() + m_nID*E_POINT_BASE_OFFSET;
				pointlist[i].SetPointIndex(nNewID);
				pFrame->m_pPrjSqlite->AddPointEntry(pointlist[i]);
			}
		}
		pFrame->m_pPrjSqlite->SaveProjectSqlite_opcid3();

		((CButton*)GetDlgItem(IDC_CHECK_LOAD_POINT))->SetCheck(1);
	}

	SetWindowText(_T("导入配置"));
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_POINT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_IMAGE))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	
}

void CImportSettingDlg::OnBnClickedButtonReloadImage()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_LOAD_IMAGE))->GetCheck();

	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);

	SetWindowText(_T("正在加载图片资源..."));
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_POINT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_IMAGE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);

	if(nCheck)		//重新加载 需删除原来的资源
	{
		int nStartID = m_nID*E_POINT_BASE_OFFSET;
		int nEndID = (m_nID+1)*E_POINT_BASE_OFFSET;
		pFrame->m_pPrjSqlite->DeleteBaseImageByID(nStartID,nEndID);
	}
	else
	{
		m_setting.bLoadImage = 1;
		pFrame->m_pPrjSqlite->UpdateImportSettingByID(m_nID,m_setting.bLoadPoint,m_setting.bLoadImage);
	}

	if(m_project_sqlite)
	{
		m_project_sqlite->LoadPictureLibSqlite();
		
		std::vector<PictureInfo> vecPictureList = m_project_sqlite->m_vPictureList;
		for(int i=0;i<vecPictureList.size(); i++)
		{
			PictureInfo* pic = &vecPictureList[i];			
			if(pic)
			{
				int nNewID = pic->ID + m_nID*E_IMAGE_BASE_OFFSET;
				pic->ID = nNewID;

				pFrame->m_pPrjSqlite->InsertPictureData(pic->ID,(char*)(pic->szName),Project::Tools::WideCharToAnsi(pic->etype_equip.c_str()).c_str(),pic->pBlock,pic->nSize);

				PictureInfo info;
				info.pBlock = new char[pic->nSize];
				if (info.pBlock !=NULL)
				{
					memcpy_s(info.pBlock,pic->nSize,pic->pBlock,pic->nSize);
				}
				info.ID = nNewID;
				info.pBitmap  = pic->pBitmap;
				wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t),pic->szName,sizeof(info.szName)/sizeof(wchar_t)-1);
				pFrame->m_pPrjSqlite->InsertToLibImage(info);
			}
		}
		((CButton*)GetDlgItem(IDC_CHECK_LOAD_IMAGE))->SetCheck(1);
	}
	SetWindowText(_T("导入配置"));
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_POINT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_IMAGE))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
}

BOOL CImportSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	Init();
	UpdateData(FALSE);
	return TRUE;
}

void CImportSettingDlg::Init()
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	ASSERT(pFrame);

	string strPrjName = UtilString::ConvertWideCharToMultiByte(m_strPrjName);
	m_bConfigEsixt = pFrame->m_pPrjSqlite->FindImportSettingByPrjName(strPrjName,m_setting);
	if(m_bConfigEsixt)
	{
		CString str;
		str.Format(_T("%d"),m_setting.nID);
		m_nID = m_setting.nID;
		((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetWindowText(str);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_setting.strPrjName);
		((CButton*)GetDlgItem(IDC_CHECK_LOAD_POINT))->SetCheck(m_setting.bLoadPoint);
		((CButton*)GetDlgItem(IDC_CHECK_LOAD_IMAGE))->SetCheck(m_setting.bLoadImage);
		((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_POINT))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_IMAGE))->EnableWindow(FALSE);
	}
	((CEdit*)GetDlgItem(IDC_EDIT_ID))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_LOAD_POINT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_LOAD_IMAGE))->EnableWindow(FALSE);
}

void CImportSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(!m_bConfigEsixt)
	{
		CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
		ASSERT(pFrame);

		_Import_Setting setting;
		int nID = pFrame->m_pPrjSqlite->GetMaxImportSettingID();
		setting.nID = nID;
		setting.bLoadImage = false;
		setting.bLoadPoint = false;
		setting.nImageBaseOffset = nID*E_IMAGE_BASE_OFFSET;
		setting.nPointBaseOffset = nID*E_POINT_BASE_OFFSET;
		setting.strPrjName = m_strPrjName.c_str();

		pFrame->m_pPrjSqlite->InsertImportSetting(setting);

		CString str;
		str.Format(_T("%d"),setting.nID);
		((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetWindowText(str);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(setting.strPrjName);
		((CButton*)GetDlgItem(IDC_CHECK_LOAD_POINT))->SetCheck(setting.bLoadPoint);
		((CButton*)GetDlgItem(IDC_CHECK_LOAD_IMAGE))->SetCheck(setting.bLoadImage);
		((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_POINT))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_RELOAD_IMAGE))->EnableWindow(TRUE);

		pFrame->SetSaveFlag(true);
	}	
}


void CImportSettingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialog::OnCancel();
}


void CImportSettingDlg::OnBnClickedBtnImport()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!m_bImporSelectResource)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
		ASSERT(pFrame != NULL);
		if (NULL == pFrame)
		{
			return;
		}

		string strPrjName = UtilString::ConvertWideCharToMultiByte(m_strFileName.GetString());
		_Import_Setting importSet;
		m_bConfigEsixt = pFrame->m_pPrjSqlite->FindImportSettingByPrjName(strPrjName, importSet);
		if (m_bConfigEsixt)
		{
			if (!importSet.bLoadImage)
			{
				MessageBox(_T("请先加载图片资源！"), _T("提示"), MB_OK|MB_ICONASTERISK);
				return;
			}
			m_nRetId = importSet.nID;
			pFrame->SetSaveFlag(true);
		}
		else
		{
			MessageBox(_T("请先加载配置资源！"), _T("提示"), MB_OK|MB_ICONASTERISK);
			return;
		}
	}

	CDialog::OnOK();
}
