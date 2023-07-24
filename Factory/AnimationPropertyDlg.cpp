// AnimationPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AnimationPropertyDlg.h"
#include "afxdialogex.h"
#include "../ComponentDraw/ProjectSqlite.h"
#include "AnimationView.h"
#include "MetaFileView.h"
#include "Tools/CustomTools/CustomTools.h"
#include "MainFrm.h"
#include "PictureTypeSelectDlg.h"
#include "FileDialogEx.h"
// CAnimationPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CAnimationPropertyDlg, CDialog)

CAnimationPropertyDlg::CAnimationPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationPropertyDlg::IDD, pParent)
	, m_nAnimationID(0)
	, m_strAniamtionName(_T(""))
	, m_strAnimationSeries(_T(""))
	,m_pAnimationView(NULL)
	,m_pMetaFileView(NULL)
{

}

CAnimationPropertyDlg::~CAnimationPropertyDlg()
{
}

void CAnimationPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_ID, m_nAnimationID);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_NAME, m_strAniamtionName);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_SERIES, m_strAnimationSeries);
}


BEGIN_MESSAGE_MAP(CAnimationPropertyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAnimationPropertyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ANIMATION, &CAnimationPropertyDlg::OnBnClickedButtonSelectAnimation)
END_MESSAGE_MAP()


// CAnimationPropertyDlg 消息处理程序


void CAnimationPropertyDlg::OnBnClickedOk()
{
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_ALL, "chs" );
	UpdateData(TRUE);
	char szTempName[MAX_PATH] = {0};
	char szTempAnimationList[1000] = {0};
	size_t converted = 0;
	errno_t err =  wcstombs_s(&converted, szTempName, sizeof(szTempName), m_strAniamtionName.GetString(), sizeof(szTempName));
	ASSERT(converted > 0);
	converted = 0;
	wcstombs_s(&converted, szTempAnimationList, sizeof(szTempAnimationList), m_strAnimationSeries.GetString(), sizeof(szTempAnimationList));
	ASSERT(converted > 0);
	m_pProjectSqlite->InsertRecordToAnimationList(m_nAnimationID, szTempName, szTempAnimationList, false);
	setlocale( LC_ALL, old_locale );
	m_pProjectSqlite->InsertAnimationVec(m_nAnimationID, m_strAniamtionName, m_strAnimationSeries);
	{
		AnimationView aniView;
		const size_t size_ani = m_pProjectSqlite->m_vAnimationListImage.size();
		aniView.nID = m_pProjectSqlite->m_vAnimationListImage[size_ani-1].ID;
		const size_t size_pic = m_pProjectSqlite->m_vAnimationListImage[size_ani-1].vecpBitmap.size();
		for (size_t j=0;j<size_pic;++j)
		{
			aniView.vec_pBitmap.push_back(m_pProjectSqlite->m_vAnimationListImage[size_ani-1].vecPicture[j].pBitmap);
		}
		aniView.strName = m_pProjectSqlite->m_vAnimationListImage[size_ani-1].szName.c_str();
		m_pAnimationView->Insert(aniView);

		const int nPage = (int)ceilf((double)m_pAnimationView->m_vecAnimationView.size()/(double)(ROW_PICTURE_NUM*COLUMN_PICTURE_NUM));
		m_pAnimationView->m_nCurPage = nPage-1;
		m_pAnimationView->Invalidate(TRUE);
		m_pAnimationView->UpdateWindow();
	}
	CDialog::OnOK();
}

void CAnimationPropertyDlg::AddAnimationSeries( const int nPicID )
{
	CString strTemp;
	strTemp.Format(L"%d",nPicID);
	if (m_strAnimationSeries.GetLength()>0)
	{
		m_strAnimationSeries = m_strAnimationSeries+L","+strTemp;
	}
	else
	{
		m_strAnimationSeries = strTemp;
	}
	UpdateData(FALSE);
}


void CAnimationPropertyDlg::OnBnClickedButtonSelectAnimation()
{
	CPictureTypeSelectDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		int nSelectType = dlg.m_nSelectType;
		CString strSelectType;
		strSelectType.Format(_T("%d"),nSelectType);

		char szType[MAX_PATH] = {0};
		size_t converted = 0;
		wcstombs_s(&converted, szType, sizeof(szType), strSelectType.GetString(), sizeof(szType));
		assert(converted > 0);

		CString strSys; 
		GetSysPath(strSys);
		strSys +=  _T("\\config");
		CString path;
		GetSysPath(path);
		path += L"\\factory.ini";
		CFileDialogEx dlg(TRUE, _T(""), _T(""), OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY, _T("PNG File (*.png)|*.png|JPG File (*.jpg)|*.jpg|All Files (*.*)|*.*||"), NULL);
		wchar_t strfilepath[1024] = {};
		GetPrivateProfileString(L"beopfactory", L"lastselectpath", L"", strfilepath, 1024, path);
		if (wcslen(strfilepath)>0)
		{
			dlg.m_ofn.lpstrInitialDir= strfilepath;
		}
		const int MIN_FILE_NUMBER = 100;
		dlg.m_ofn.lpstrFile = new wchar_t[_MAX_PATH * MIN_FILE_NUMBER];          
		memset(dlg.m_ofn.lpstrFile, 0, _MAX_PATH * MIN_FILE_NUMBER);
		dlg.m_ofn.nMaxFile = _MAX_PATH * MIN_FILE_NUMBER;
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
				int pos_line = strFilePath.ReverseFind(L'\\');
				int pos_dot = strFilePath.ReverseFind(L'.');
				CString strFileName = strFilePath.Mid(pos_line+1,pos_dot-pos_line-1);

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
				const int nPicID = m_pProjectSqlite->GetMaxLibImageAnimationNum(false)+1;
				UpdateData();
				CString strID;
				strID.Format(L"%d",nPicID);
				if (m_strAnimationSeries.GetLength() == 0)
				{
					m_strAnimationSeries = strID;
				}
				else
				{
					m_strAnimationSeries = m_strAnimationSeries + L"," + strID;
				}
				UpdateData(FALSE);
				char szFileName[MAX_PATH] = {0};
				size_t converted = 0;
				wcstombs_s(&converted, szFileName, sizeof(szFileName), strFileName.GetString(), sizeof(szFileName));
				assert(converted > 0);
				m_pProjectSqlite->InsertPictureAnimationData(nPicID, szFileName,szType, pBuf, file_size, false);
				fclose(pFile);
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
						info.nSize = file_size;
						info.pBitmap = pBitmap;
						info.etype_equip = strSelectType.GetString();
						wcsncpy_s(info.szName, sizeof(info.szName)/sizeof(wchar_t), strFileName.GetString(), sizeof(info.szName)/sizeof(wchar_t));
						info.pBlock = new char[file_size];
						if (info.pBlock != NULL)
						{
							memset(info.pBlock, 0, file_size);
							memcpy_s(info.pBlock, file_size, pBuf, file_size);
						}
						m_pProjectSqlite->InsertToLibImageAnimation(info);
						GlobalUnlock(hMemBmp);
					}
					GlobalFree(hMemBmp);
					hMemBmp = NULL;
				}
				SAFE_DELETE_ARRAY(pBuf);
			}
			setlocale( LC_ALL, old_locale );
		}
		delete[] dlg.m_ofn.lpstrFile;
	}	
}
