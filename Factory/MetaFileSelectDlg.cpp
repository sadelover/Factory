// MetaFileSelectDlg.cpp : 实现文件
//严观微增加窗口

#include "stdafx.h"
#include "MetaFileSelectDlg.h"
#include "AnimationView.h"
#include <cassert>
#include "../ComponentDraw/ProjectSqlite.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "MainFrm.h"
#include "tools/CustomTools/CustomTools.h"
#include "AnimationPropertyDlg.h"
#include "BEOP-DesignerView.h"
#include "PageDlg.h"
#include "PictureTypeSelectDlg.h"
#include "FileDialogEx.h"
#include "ImageReplaceDlg.h"
// CMetaFileSelectDlg 对话框

static const int VIEW_BOTTOM_TO_DLG = 50;

IMPLEMENT_DYNAMIC(CMetaFileSelectDlg, CDialog)

CMetaFileSelectDlg::CMetaFileSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMetaFileSelectDlg::IDD, pParent)
,m_pMetaView(NULL)
,m_pAnimationView(NULL)
,m_pProjectSqlite(NULL)
,m_pAnimationPropertyDlg(NULL)
,m_nID(-1)
,m_eWndType(e_picture)
,m_nPicNum(0)
, m_strPageInfo(_T(""))
,m_bBtnOkShow(true)
,m_eSubType(E_TYPE_ALL)
,m_nSelectIndex(-1)
, m_strImageName(_T(""))
,m_bSelectDivide(false)
,m_nShowMode(0)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pProjectSqlite = pFrame->m_pPrjSqlite;
}

CMetaFileSelectDlg::~CMetaFileSelectDlg()
{

}

void CMetaFileSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PAGE_COUNT, m_strPageInfo);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strImageName);
}

BEGIN_MESSAGE_MAP(CMetaFileSelectDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_MENU_META, &CMetaFileSelectDlg::OnMenuMeta)
	ON_COMMAND(ID_MENU_ANIMATION, &CMetaFileSelectDlg::OnMenuAnimation)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_IMPORT_METAFILE, &CMetaFileSelectDlg::OnImportMetafile)
	ON_COMMAND(ID_IMPORT_ANIMATION, &CMetaFileSelectDlg::OnImportAnimation)
	ON_COMMAND(ID_TYPE_ALL, &CMetaFileSelectDlg::OnTypeAll)
	ON_COMMAND(ID_TYPE_CH, &CMetaFileSelectDlg::OnTypeCH)
	ON_COMMAND(ID_TYPE_PUMP, &CMetaFileSelectDlg::OnTypePump)
	ON_COMMAND(ID_TYPE_TOWER, &CMetaFileSelectDlg::OnTypeTower)
	ON_COMMAND(ID_TYPE_AHU, &CMetaFileSelectDlg::OnTypeAHU)
	ON_COMMAND(ID_TYPE_VALUE, &CMetaFileSelectDlg::OnTypeValue)
	ON_COMMAND(ID_TYPE_PIPE, &CMetaFileSelectDlg::OnTypePipe)
	ON_COMMAND(ID_TYPE_BUTTON, &CMetaFileSelectDlg::OnTypeButton)
	ON_COMMAND(ID_TYPE_CHART, &CMetaFileSelectDlg::OnTypeChart)
	ON_COMMAND(ID_TYPE_LAYOUT, &CMetaFileSelectDlg::OnTypeLayout)
	ON_COMMAND(ID_TYPE_ICON, &CMetaFileSelectDlg::OnTypeIcon)
	ON_COMMAND(ID_TYPE_OTHER, &CMetaFileSelectDlg::OnTypeOther)
	ON_BN_CLICKED(IDOK, &CMetaFileSelectDlg::OnBnClickedOk)
	ON_COMMAND(ID_MENU_LIB_DELETE, &CMetaFileSelectDlg::OnMenuLibDelete)
	ON_COMMAND(ID_MENU_LIB_EDIT, &CMetaFileSelectDlg::OnMenuLibEdit)
	ON_BN_CLICKED(IDC_BUTTON_PRE_PAGE, &CMetaFileSelectDlg::OnBnClickedButtonPrePage)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_PAGE, &CMetaFileSelectDlg::OnBnClickedButtonNextPage)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, &CMetaFileSelectDlg::OnBnClickedButtonJump)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CMetaFileSelectDlg::OnBnClickedButtonSearch)
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_MENU_META, &CMetaFileSelectDlg::OnUpdateMenuMeta)
	ON_UPDATE_COMMAND_UI(ID_MENU_ANIMATION, &CMetaFileSelectDlg::OnUpdateMenuAnimation)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_METAFILE, &CMetaFileSelectDlg::OnUpdateImportMetafile)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_ANIMATION, &CMetaFileSelectDlg::OnUpdateImportAnimation)
	ON_COMMAND(ID_IMAGE_DLG_EXIT, &CMetaFileSelectDlg::OnImageDlgExit)
	ON_UPDATE_COMMAND_UI(ID_MENU_LIB_DELETE, &CMetaFileSelectDlg::OnUpdateMenuLibDelete)
	ON_UPDATE_COMMAND_UI(ID_MENU_LIB_EDIT, &CMetaFileSelectDlg::OnUpdateMenuLibEdit)
	ON_STN_CLICKED(IDC_PICTURE_NAME, &CMetaFileSelectDlg::OnStnClickedPictureName)
END_MESSAGE_MAP()


// CMetaFileSelectDlg 消息处理程序


int CMetaFileSelectDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rect;
	GetClientRect(&rect);
	LPCWSTR lpszWndClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	if (m_pMetaView == NULL)
	{
		m_pMetaView = new CMetaFileView();
		assert(m_pMetaView);
		m_pMetaView->Create(lpszWndClassName,L"",WS_CHILD|WS_VISIBLE,CRect(0,0,rect.right,rect.bottom-VIEW_BOTTOM_TO_DLG),this,META_FILE_VIEW);
		m_pMetaView->SetParent(this);
	}
	if (m_pAnimationView == NULL)
	{
		m_pAnimationView = new CAnimationView();
		assert(m_pAnimationView);
		m_pAnimationView->Create(lpszWndClassName,L"",WS_CHILD|WS_VISIBLE,CRect(0,0,rect.right,rect.bottom-VIEW_BOTTOM_TO_DLG),this,ANIMATION_VIEW);
		m_pAnimationView->SetParent(this);
	}
	if (m_pAnimationPropertyDlg == NULL)
	{
		m_pAnimationPropertyDlg = new CAnimationPropertyDlg();
		assert(m_pAnimationPropertyDlg);
		m_pAnimationPropertyDlg->Create(IDD_DIALOG_ADD_ANIMATION, this);
		m_pAnimationPropertyDlg->ShowWindow(SW_HIDE);
		m_pAnimationPropertyDlg->m_pProjectSqlite = m_pProjectSqlite;
		m_pAnimationPropertyDlg->m_pAnimationView = m_pAnimationView;
		m_pAnimationPropertyDlg->m_pMetaFileView = m_pMetaView;
	}
	m_pMetaView->m_pAnimationPropertyDlg = m_pAnimationPropertyDlg;
	m_pMetaView->SetProjectSqlPointer(m_pProjectSqlite);
	m_pAnimationView->SetProjectSqlPointer(m_pProjectSqlite);
	m_menu.LoadMenu(IDR_MENU_DELETE);
	return 0;
}

void CMetaFileSelectDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pMetaView)
	{
		delete m_pMetaView;
		m_pMetaView = NULL;
	}
	if (m_pAnimationView)
	{
		delete m_pAnimationView;
		m_pAnimationView = NULL;
	}
	if (m_pAnimationPropertyDlg)
	{
		m_pAnimationPropertyDlg->DestroyWindow();
		delete m_pAnimationPropertyDlg;
		m_pAnimationPropertyDlg = NULL;
	}
}

void CMetaFileSelectDlg::OnMenuMeta()
{
	// TODO: 在此添加命令处理程序代码
	m_eWndType = e_picture;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
	if (m_pMetaView->IsWindowVisible())
	{
		return;
	}
	if (m_pAnimationView->IsWindowVisible())
	{
		m_pAnimationView->ShowWindow(SW_HIDE);
		GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);
		m_pMetaView->ShowWindow(SW_SHOW);
		const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
		UpdateData(0);
	}
}

void CMetaFileSelectDlg::OnMenuAnimation()
{
	// TODO: 在此添加命令处理程序代码
	m_eWndType = e_animation;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_CHECKED);
	if(m_pAnimationView->IsWindowVisible())
	{
		return;
	}
	if (m_pMetaView->IsWindowVisible())
	{
		m_pMetaView->ShowWindow(SW_HIDE);
		GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_UNCHECKED);
		m_pAnimationView->ShowWindow(SW_SHOW);
		const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		m_strPageInfo.Format(L"%d/%d",m_pAnimationView->m_nCurPage+1,nPage);
		UpdateData(0);
	}
}

BOOL CMetaFileSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(1)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
	if (m_eWndType == e_picture)
	{
		m_pMetaView->ShowWindow(SW_SHOW);
		m_pAnimationView->ShowWindow(SW_HIDE);
		const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		m_pMetaView->m_nCurPage = nPage - 1;
		m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
	}
	if (m_eWndType == e_animation)
	{
		m_pMetaView->ShowWindow(SW_HIDE);
		m_pAnimationView->ShowWindow(SW_SHOW);
		const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		m_strPageInfo.Format(L"%d/%d",m_pAnimationView->m_nCurPage+1,nPage);
	}
	if (m_bSelectDivide)
	{
		if (e_picture == m_eWndType)
		{
			GetMenu()->GetSubMenu(0)->CheckMenuItem(0, MF_BYPOSITION|MF_CHECKED);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION|MF_ENABLED);
			GetMenu()->GetSubMenu(0)->CheckMenuItem(1, MF_BYPOSITION|MF_UNCHECKED);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION|MF_GRAYED);
		}
		else if (e_animation == m_eWndType)
		{
			GetMenu()->GetSubMenu(0)->CheckMenuItem(0, MF_BYPOSITION|MF_UNCHECKED);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(0, MF_BYPOSITION|MF_GRAYED);
			GetMenu()->GetSubMenu(0)->CheckMenuItem(1, MF_BYPOSITION|MF_CHECKED);
			GetMenu()->GetSubMenu(0)->EnableMenuItem(1, MF_BYPOSITION|MF_ENABLED);
		}
		else
		{	// error
		}
	}

	if (m_bBtnOkShow)
	{
		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	}

	if(m_nSelectIndex != -1)
	{
		if(m_pMetaView)
		{
			m_pMetaView->SetCurrentPicID(m_nSelectIndex);
			const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
			m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
			m_pMetaView->Invalidate();
		}
	}

	UpdateData(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMetaFileSelectDlg::SetID(const int nID)
{
	m_nID = nID;
}

void CMetaFileSelectDlg::SetWndType( const e_wndType eType )
{
	m_eWndType = eType;
}

const e_wndType CMetaFileSelectDlg::GetWndType()
{
	return m_eWndType;
}

void CMetaFileSelectDlg::SetPicNum( const int nNum )
{
	m_nPicNum = nNum;
}

void CMetaFileSelectDlg::OnImportMetafile()
{
	CPictureTypeSelectDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		int nSelectType = dlg.m_nSelectType+1;
		CString strSelectType;
		strSelectType.Format(_T("%d"),nSelectType);

		char szType[MAX_PATH] = {0};
		size_t converted = 0;
		wcstombs_s(&converted, szType, sizeof(szType), strSelectType.GetString(), sizeof(szType));
		assert(converted > 0);

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		ASSERT(pFrame);
		CString strSys; 
		GetSysPath(strSys);
		strSys +=  _T("\\config");
		CString path;
		GetSysPath(path);
		path += L"\\factory.ini";
		CFileDialogEx dlg(TRUE, _T(""), _T(""), OFN_ALLOWMULTISELECT, _T("PNG File (*.png)|*.png|JPG File (*.jpg)|*.jpg|All Files (*.*)|*.*||"), NULL);
		dlg.SetOpenType(1);
		wchar_t strfilepath[1024] = {};
		GetPrivateProfileString(L"beopobserver", L"lastselectpath", L"", strfilepath, 1024, path);
		if (wcslen(strfilepath)>0)
		{
			dlg.m_ofn.lpstrInitialDir= strfilepath;
		}
		if (dlg.DoModal() == IDOK)
		{
			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
			setlocale( LC_ALL, "chs" );

			POSITION pos = dlg.GetStartPosition();
			while (NULL != pos)
			{
				CString strFilePath = dlg.GetNextPathName(pos);
				CString strFolder = strFilePath.Left(strFilePath.ReverseFind(L'\\'));

				WritePrivateProfileString (_T("beopobserver"),_T("lastselectpath"),strFolder,path);

				CString strFileNameWithEx =	strFilePath.Right(strFilePath.GetLength() - strFolder.GetLength() - 1);
				CString strFileName = strFileNameWithEx.Left(strFileNameWithEx.Find(L'.',0));
				FILE* pFile = NULL;
				const errno_t rs = _wfopen_s(&pFile, strFilePath.GetString(), L"rb");
				assert(0 == rs);
				assert(pFile);
				fseek(pFile, 0, SEEK_END);
				const int file_size = ftell(pFile);
				assert(file_size > 0);
				char* pBuf = new char[file_size];
				memset(pBuf, 0, file_size);
				fseek(pFile, 0, SEEK_SET);
				fread(pBuf, sizeof(char), file_size, pFile);
				const int nPicID = pFrame->m_pPrjSqlite->GetMaxLibImageNum(false)+1+INSERT_META_FILE_ID_START;
				char szFileName[MAX_PATH] = {0};
				converted = 0;
				wcstombs_s(&converted, szFileName, sizeof(szFileName), strFileName.GetString(), sizeof(szFileName));
				assert(converted > 0);

				CString strEx = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind(_T('.')) - 1);
				char szEx[MAX_PATH] = {0};
				USES_CONVERSION;
				sprintf_s(szEx, MAX_PATH, "%s", T2A(strEx.GetBuffer()));
				pFrame->m_pPrjSqlite->InsertPictureData(nPicID, szFileName, szType, pBuf, file_size, false, 0, 0, szEx);
				strEx.ReleaseBuffer();
				fclose(pFile);
				{
					CMemFile   memfile;  
					memfile.Attach((BYTE*)(pBuf),file_size,1024);  
					memfile.SetLength(file_size); 
					HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, file_size);
					if (hMemBmp)
					{
						IStream* pStmBmp = NULL;
						if(CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
						{
							BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
							memfile.SeekToBegin();
							memfile.Read(pbyBmp, file_size);
							Image* pBitmap = Image::FromStream(pStmBmp);
							PictureInfo info;
							info.ID = nPicID;
							info.pBitmap = pBitmap;
							info.etype_equip = strSelectType.GetString();
							wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), strFileNameWithEx.GetString(), sizeof(info.szName)/sizeof(wchar_t)-1);
							info.nSize = file_size;
							info.pBlock = new char[file_size];
							if (info.pBlock != NULL)
							{
								memset(info.pBlock, 0, file_size);
								memcpy_s(info.pBlock, file_size, pBuf, file_size);
							}
							pFrame->m_pPrjSqlite->InsertToLibImage(info);

							PictureView pic;
							pic.pBitmap = info.pBitmap;
							pic.strName = info.szName;
							pic.nID = info.ID;
							pic.etype_equip = strSelectType.GetString();
							m_pMetaView->m_vecPictureView.push_back(pic);
							const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
							m_pMetaView->m_nCurPage = nPage-1;
							m_pMetaView->Invalidate(TRUE);
							m_pMetaView->UpdateWindow();
							GlobalUnlock(hMemBmp);
							
						}
						//GlobalFree(hMemBmp);	// Marked for jpg show
						hMemBmp = NULL;
					}
				}
				SAFE_DELETE_ARRAY(pBuf);
			}
			setlocale( LC_ALL, old_locale );
			ShowPictureView(m_eWndType,m_eSubType);
		}
	}	
}


void CMetaFileSelectDlg::OnImportAnimation()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	m_pAnimationPropertyDlg->m_nAnimationID = pFrame->m_pPrjSqlite->GetMaxLibAnimationNum(false) + 1;
	m_pAnimationPropertyDlg->m_strAniamtionName = L"新动画名称";
	m_pAnimationPropertyDlg->m_strAnimationSeries = L"";
	m_pAnimationPropertyDlg->UpdateData(FALSE);
	m_pAnimationPropertyDlg->ShowWindow(SW_SHOW);
}

void CMetaFileSelectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pMetaView->IsWindowVisible())
	{
		if (m_pMetaView->m_nSelectIndex == -1)
		{
			AfxMessageBox(L"请单击左键选中图元!");
			return;
		}
		SetID(m_pMetaView->m_vecPictureViewByType[m_pMetaView->m_nSelectIndex].nID);
		SetPicNum(1);
		SetVectorPicNum(true);
	}
	else if (m_pAnimationView->IsWindowVisible())
	{
		if (m_pAnimationView->m_nSelectIndex == -1)
		{
			AfxMessageBox(L"请单击左键选中动画!");
			return;
		}
		SetID(m_pAnimationView->m_vecAnimationView[m_pAnimationView->m_nSelectIndex].nID);
		SetPicNum(m_pAnimationView->m_vecAnimationView[m_pAnimationView->m_nSelectIndex].vec_pBitmap.size());
		SetVectorPicNum(false);
	}
	CDialog::OnOK();
}

void CMetaFileSelectDlg::OnMenuLibDelete()
{
	if (m_pAnimationView->IsWindowVisible())
	{
		vector<AnimationView>::const_iterator	iter;
		vector<int>	vecDel;
		vector<int>::const_iterator	itDel;
		for (iter=m_pAnimationView->m_vecAnimationView.begin(); iter!=m_pAnimationView->m_vecAnimationView.end(); ++iter)
		{
			if (iter->bIsShowRectangle)
			{
				vecDel.push_back(iter->nID);
			}
		}
		for (itDel=vecDel.begin(); itDel!=vecDel.end(); ++itDel)
		{
			m_pProjectSqlite->DeleteOneAnimation(*itDel);
			m_pAnimationView->DeleteOne(*itDel);
		}
		m_pAnimationView->Invalidate(TRUE);

		//if (m_pAnimationView->m_nSelectIndex != -1)
		//{
		//	m_pProjectSqlite->DeleteOneAnimation(m_pAnimationView->m_vecAnimationView[m_pAnimationView->m_nSelectIndex].nID);
		//	m_pAnimationView->DeleteOne(m_pAnimationView->m_vecAnimationView[m_pAnimationView->m_nSelectIndex].nID);
		//	m_pAnimationView->m_nSelectIndex = -1;
		//	m_pAnimationView->Invalidate(TRUE);
		//}
	}
	if (m_pMetaView->IsWindowVisible())
	{
		vector<PictureView>::const_iterator	iter;
		vector<int>	vecDel;
		vector<int>::const_iterator	itDel;
		for (iter=m_pMetaView->m_vecPictureViewByType.begin(); iter!=m_pMetaView->m_vecPictureViewByType.end(); ++iter)
		{
			if (iter->bIsShowRectangle)
			{
				vecDel.push_back(iter->nID);
			}
		}
		for (itDel=vecDel.begin(); itDel!=vecDel.end(); ++itDel)
		{
			m_pProjectSqlite->DeleteOnePicture(*itDel, false);
			m_pMetaView->DeleteOne(*itDel);
		}
		ShowPictureView(m_eWndType,m_eSubType);
		m_pMetaView->Invalidate(TRUE);

		//if (m_pMetaView->m_nSelectIndex != -1)
		//{
		//	m_pProjectSqlite->DeleteOnePicture(m_pMetaView->m_vecPictureView[m_pMetaView->m_nSelectIndex].nID, false);
		//	m_pMetaView->DeleteOne(m_pMetaView->m_vecPictureView[m_pMetaView->m_nSelectIndex].nID);
		//	m_pMetaView->m_nSelectIndex = -1;
		//	m_pMetaView->Invalidate(TRUE);
		//}
	}
}


void CMetaFileSelectDlg::OnMenuLibEdit()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CString path;
	GetSysPath(path);
	path += L"\\factory.ini";
	CFileDialogEx dlg(TRUE, _T(""), _T(""), 0, _T("PNG File (*.png)|*.png|JPG File (*.jpg)|*.jpg|All Files (*.*)|*.*||"), NULL);
	wchar_t strfilepath[1024] = {};
	GetPrivateProfileString(L"beopobserver", L"lastselectpath", L"", strfilepath, 1024, path);
	if (m_pMetaView->IsWindowVisible())
	{
		if (m_pMetaView->m_nSelectIndex != -1)
		{
			if (wcslen(strfilepath)>0)
			{
				dlg.m_ofn.lpstrInitialDir= strfilepath;
			}
			if (dlg.DoModal() == IDOK)
			{
				char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
				setlocale( LC_ALL, "chs" );
				CString strFilePath = dlg.GetPathName();
				CString strFolder = strFilePath.Left(strFilePath.ReverseFind(L'\\'));

				WritePrivateProfileString (_T("beopobserver"),_T("lastselectpath"),strFolder,path);

				CString strFileNameWithEx = dlg.GetFileName();
				CString strFileName = strFileNameWithEx.Left(strFileNameWithEx.Find(L'.',0));
				FILE* pFile = NULL;
				const errno_t rs = _wfopen_s(&pFile, strFilePath.GetString(), L"rb");
				assert(0 == rs);
				assert(pFile);
				fseek(pFile, 0, SEEK_END);
				const int file_size = ftell(pFile);
				assert(file_size > 0);
				char* pBuf = new char[file_size];
				memset(pBuf, 0, sizeof(pBuf));
				fseek(pFile, 0, SEEK_SET);
				fread(pBuf, sizeof(char), file_size, pFile);
				const int nPicID = m_pMetaView->m_vecPictureView[m_pMetaView->m_nSelectIndex].nID;
				wstring strSelectType = m_pMetaView->m_vecPictureView[m_pMetaView->m_nSelectIndex].etype_equip;
				char szFileName[MAX_PATH] = {0};
				size_t converted = 0;
				wcstombs_s(&converted, szFileName, sizeof(szFileName), strFileName.GetString(), sizeof(szFileName));
				assert(converted > 0);
				fclose(pFile);
				{
					CMemFile   memfile;  
					memfile.Attach((BYTE*)(pBuf),file_size,1024);  
					memfile.SetLength(file_size); 
					HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, file_size);
					if (hMemBmp)
					{
						IStream* pStmBmp = NULL;
						if(CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp) == S_OK)
						{
							BYTE* pbyBmp = (BYTE *)GlobalLock(hMemBmp);
							memfile.SeekToBegin();
							memfile.Read(pbyBmp, file_size);
							Image* pBitmap = Image::FromStream(pStmBmp);
							PictureInfo info;
							info.ID = nPicID;
							info.pBitmap = pBitmap;
							wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), strFileName.GetString(), sizeof(info.szName)/sizeof(wchar_t)-1);
							bool bIsSelDb = (1 == m_nShowMode) ? true : false;
							pFrame->m_pPrjSqlite->DeleteOnePictureAndInsert(m_pMetaView->m_vecPictureView[m_pMetaView->m_nSelectIndex].nID, info, bIsSelDb);
							PictureView pic;
							pic.pBitmap = info.pBitmap;
							pic.strName = info.szName;
							pic.nID = info.ID;

							char szType[MAX_PATH] = {0};
							converted = 0;
							wcstombs_s(&converted, szType, sizeof(szType), strSelectType.c_str(), sizeof(szType));
							assert(converted > 0);

							m_pMetaView->DeleteAndInsert(nPicID, pic);
							pFrame->m_pPrjSqlite->InsertPictureData(nPicID, szFileName, szType,pBuf, file_size, bIsSelDb);

							if (!pFrame->m_pPrjSqlite->GetIsNeedLoadAllPages())
							{
								pFrame->m_pPrjSqlite->LoadAllPagesDetail();
								pFrame->m_pPrjSqlite->SetIsNeedLoadAllPages(true);
							}
							vector<ImgReplaceInfo>	vecReplaceInfo;
							pFrame->m_pPrjSqlite->ReplacePic(nPicID, vecReplaceInfo);

							GlobalUnlock(hMemBmp);

							if (pFrame->m_pImgReplageDlg != NULL)
							{
								(pFrame->m_pImgReplageDlg)->SetReplaceInfo(vecReplaceInfo);
								(pFrame->m_pImgReplageDlg)->ShowWindow(TRUE);	// show replace info
							}
						}
						GlobalFree(hMemBmp);
						hMemBmp = NULL;
					}
				}
				setlocale( LC_ALL, old_locale );
				SAFE_DELETE_ARRAY(pBuf);
			}
			ShowPictureView(m_eWndType,m_eSubType);
			m_pMetaView->Invalidate(TRUE);
			CBEOPDesignerView* pView = (CBEOPDesignerView*)pFrame->GetActiveView();
			pView->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
		}
	}
}

void CMetaFileSelectDlg::OnBnClickedButtonPrePage()
{
	if (m_pMetaView->IsWindowVisible())
	{
		const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		if (m_pMetaView->m_nCurPage<nPage && m_pMetaView->m_nCurPage!=0)
		{
			m_pMetaView->ClearSelectPicByPageID(m_pMetaView->m_nCurPage);
			m_pMetaView->m_nCurPage--;
			if (m_pMetaView->m_nCurPage < 0)
			{
				m_pMetaView->m_nCurPage = 0;
			}
			const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
			m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
			m_pMetaView->Invalidate(1);
		}
		m_pMetaView->HidePreviewDlg();
	}
	if (m_pAnimationView->IsWindowVisible())
	{
		const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		if (m_pAnimationView->m_nCurPage<nPage)
		{
			m_pAnimationView->m_nCurPage--;
			if (m_pAnimationView->m_nCurPage < 0)
			{
				m_pAnimationView->m_nCurPage = 0;
			}
			const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
			m_strPageInfo.Format(L"%d/%d",m_pAnimationView->m_nCurPage+1,nPage);
			m_pAnimationView->Invalidate(1);
		}
	}
	UpdateData(0);
}

void CMetaFileSelectDlg::OnBnClickedButtonNextPage()
{
	if (m_pMetaView->IsWindowVisible())
	{
		const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		if (m_pMetaView->m_nCurPage<nPage-1)
		{
			m_pMetaView->ClearSelectPicByPageID(m_pMetaView->m_nCurPage);
			m_pMetaView->m_nCurPage++;
			if (m_pMetaView->m_nCurPage >= nPage)
			{
				m_pMetaView->m_nCurPage = nPage-1;
			}
			const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
			m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
			m_pMetaView->Invalidate(1);
		}
		m_pMetaView->HidePreviewDlg();
	}
	if (m_pAnimationView->IsWindowVisible())
	{
		const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		if (m_pAnimationView->m_nCurPage<nPage)
		{
			m_pAnimationView->m_nCurPage++;
			if (m_pAnimationView->m_nCurPage >= nPage)
			{
				m_pAnimationView->m_nCurPage = nPage-1;
			}
			const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
			m_strPageInfo.Format(L"%d/%d",m_pAnimationView->m_nCurPage+1,nPage);
			m_pAnimationView->Invalidate(1);
		}
	}
	UpdateData(0);
}

void CMetaFileSelectDlg::OnBnClickedButtonJump()
{
	CString strText;
	GetDlgItem(IDC_EDIT_JUMP_PAGE)->GetWindowText(strText);
	if(strText.GetLength() > 0)
	{
		const int nIndex = _wtoi(strText.GetString());
		if (m_pMetaView->IsWindowVisible())
		{
			const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
			if(nIndex>=1 && nIndex<=nPage)
			{
				m_pMetaView->m_nCurPage = nIndex - 1;
				m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
				m_pMetaView->Invalidate(1);
			}
			else
			{
				AfxMessageBox(L"错误的页码!");
			}
			m_pMetaView->HidePreviewDlg();
		}
		if (m_pAnimationView->IsWindowVisible())
		{
			const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
			if(nIndex>=1 && nIndex<=nPage)
			{
				m_pAnimationView->m_nCurPage = nIndex - 1;
				m_strPageInfo.Format(L"%d/%d",m_pAnimationView->m_nCurPage+1,nPage);
				m_pAnimationView->Invalidate(1);
			}
			else
			{
				AfxMessageBox(L"错误的页码!");
			}
		}
	}
	UpdateData(0);
}

void CMetaFileSelectDlg::SetBtnOkShow(const bool bShow)
{
	m_bBtnOkShow = bShow;
}

void CMetaFileSelectDlg::SetVectorPicNum(const bool bIsPic)
{
	if (bIsPic)
	{
		vector<PictureView>::const_iterator	iter;

		m_vecPicNum.clear();
		for (iter=m_pMetaView->m_vecPictureViewByType.begin(); iter!=m_pMetaView->m_vecPictureViewByType.end(); ++iter)
		{
			if (iter->bIsShowRectangle)
			{
				m_vecPicNum.push_back(iter->nID);
			}
		}
	} 
	else
	{
		vector<AnimationView>::const_iterator	iter;
		
		m_vecPicNum.clear();
		for (iter=m_pAnimationView->m_vecAnimationView.begin(); iter!=m_pAnimationView->m_vecAnimationView.end(); ++iter)
		{
			if (iter->bIsShowRectangle)
			{
				m_vecPicNum.push_back(iter->nID);
			}
		}
	}
}

vector<int> CMetaFileSelectDlg::GetVectorPicNum(void) const
{
	return m_vecPicNum;
}

void CMetaFileSelectDlg::OnTypeAll()
{
	ShowPictureView(m_eWndType,E_TYPE_ALL);
}

void CMetaFileSelectDlg::OnTypeCH()
{
	ShowPictureView(m_eWndType,E_TYPE_CH);
}

void CMetaFileSelectDlg::OnTypePump()
{
	ShowPictureView(m_eWndType,E_TYPE_PUMP);
}

void CMetaFileSelectDlg::OnTypeTower()
{
	ShowPictureView(m_eWndType,E_TYPE_TOWER);
}

void CMetaFileSelectDlg::OnTypeAHU()
{
	ShowPictureView(m_eWndType,E_TYPE_AHU);
}

void CMetaFileSelectDlg::OnTypeValue()
{
	ShowPictureView(m_eWndType,E_TYPE_VALUE);
}

void CMetaFileSelectDlg::OnTypePipe()
{
	ShowPictureView(m_eWndType,E_TYPE_PIPE);
}

void CMetaFileSelectDlg::OnTypeButton()
{
	ShowPictureView(m_eWndType,E_TYPE_BUTTON);
}

void CMetaFileSelectDlg::OnTypeChart()
{
	ShowPictureView(m_eWndType,E_TYPE_CHART);
}

void CMetaFileSelectDlg::OnTypeLayout()
{
	ShowPictureView(m_eWndType,E_TYPE_LAYOUT);
}

void CMetaFileSelectDlg::OnTypeIcon()
{
	ShowPictureView(m_eWndType,E_TYPE_ICON);
}

void CMetaFileSelectDlg::OnTypeOther()
{
	ShowPictureView(m_eWndType,E_TYPE_OTHET);
}

void CMetaFileSelectDlg::ShowPictureView( e_wndType eWndType, E_SubType eSubType )
{
	m_eSubType = eSubType;
	m_eWndType = e_picture;
	for(int i=0; i<=E_TYPE_OTHET; i++)
	{
		if(i == eSubType)
		{
			GetMenu()->GetSubMenu(1)->CheckMenuItem(eSubType,MF_BYPOSITION|MF_CHECKED);
		}
		else
		{
			GetMenu()->GetSubMenu(1)->CheckMenuItem(i,MF_BYPOSITION|MF_UNCHECKED);
		}
	}
	if (m_pMetaView->IsWindowVisible())
	{
		m_pAnimationView->ShowWindow(SW_HIDE);
		m_pMetaView->ShowWindow(SW_SHOW);
		m_pMetaView->UpdatePictureViewByType(eSubType);
		m_pMetaView->Invalidate();
		const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		m_pMetaView->m_nCurPage = nPage-1;
		m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
		UpdateData(0);
	}	
}

void CMetaFileSelectDlg::SetCurrentPicID( const int nID )
{
	m_nSelectIndex = nID;
}


void CMetaFileSelectDlg::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_strImageName.GetLength() == 0)
		return;

	if (m_pMetaView->IsWindowVisible())
	{
		m_pAnimationView->ShowWindow(SW_HIDE);
		m_pMetaView->ShowWindow(SW_SHOW);
		m_pMetaView->UpdatePictureViewByName(m_strImageName);
		m_pMetaView->Invalidate();
		const int nPage = (int)ceilf((double)m_pMetaView->m_vecPictureViewByType.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		m_pMetaView->m_nCurPage = nPage-1;
		m_strPageInfo.Format(L"%d/%d",m_pMetaView->m_nCurPage+1,nPage);
		UpdateData(0);
	}	
}

void CMetaFileSelectDlg::SetDivideMode(const bool bFlag)
{
	m_bSelectDivide = bFlag;
}

void CMetaFileSelectDlg::SetShowMode(const int nShowMode)
{
	m_nShowMode = nShowMode;
}

int CMetaFileSelectDlg::GetShowMode(void) const
{
	return m_nShowMode;
}


void CMetaFileSelectDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: Add your message handler code here
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}


void CMetaFileSelectDlg::OnUpdateMenuMeta(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}


void CMetaFileSelectDlg::OnUpdateMenuAnimation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = (0 == m_nShowMode) ? TRUE : FALSE;
	pCmdUI->Enable(bEnable);
}


void CMetaFileSelectDlg::OnUpdateImportMetafile(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = (0 == m_nShowMode) ? TRUE : FALSE;
	pCmdUI->Enable(bEnable);
}


void CMetaFileSelectDlg::OnUpdateImportAnimation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = (0 == m_nShowMode) ? TRUE : FALSE;
	pCmdUI->Enable(bEnable);
}


void CMetaFileSelectDlg::OnImageDlgExit()
{
	// TODO: Add your command handler code here
	CDialog::OnCancel();
}


void CMetaFileSelectDlg::OnUpdateMenuLibDelete(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	BOOL bEnable = (0 == m_nShowMode) ? TRUE : FALSE;
	pCmdUI->Enable(bEnable);
}


void CMetaFileSelectDlg::OnUpdateMenuLibEdit(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}


void CMetaFileSelectDlg::OnStnClickedPictureName()
{
	// TODO: Add your control notification handler code here
}
