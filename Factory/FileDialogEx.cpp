// MyFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BEOP-designer.h"
#include "FileDialogEx.h"
#include "MainFrm.h"
#include "../ComponentDraw/ProjectSqlite.h"


// CMyFileDlg

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd)
		: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
		, m_pProjSqlite(NULL)
		, m_pMainFrm((CMainFrame*)pParentWnd)
		, m_nType(0)
		, m_bEncrypt(TRUE)
{

}

CFileDialogEx::~CFileDialogEx()
{
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
END_MESSAGE_MAP()



// CMyFileDlg message handlers




void CFileDialogEx::SetProjectSqlite(CProjectSqlite* pProjSqlite)
{
	ASSERT(pProjSqlite != NULL);
	m_pProjSqlite =	pProjSqlite;
}


BOOL CFileDialogEx::OnFileNameOK()
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL	bRet = FALSE;
	switch (m_nType)
	{
	case 0:
		bRet = CheckFileName();
		break;
	case 1:
		{
			bRet = CheckPngFile();
			if (!bRet)
			{
				MessageBox(_T("PNG格式不正确！"), _T("错误"), MB_OK|MB_ICONERROR);
			}
		}
		break;
	default:
		break;
	}

	if (!bRet)
	{
		return TRUE;
	}

	return CFileDialog::OnFileNameOK();
}

void CFileDialogEx::SetOpenType(const int nType)
{
	m_nType = nType;
}

BOOL CFileDialogEx::CheckFileName(void)
{
	CString	strPathName = GetPathName();
	if (!m_pMainFrm->CheckFileName(strPathName))
	{
		return FALSE;
	}

	if (m_pProjSqlite != NULL)
	{
		m_pProjSqlite->SetFilePath(strPathName.GetString());
		m_pProjSqlite->ParseFileType(strPathName);

		if (!m_pProjSqlite->LoadProjectConfig())
		{
			MessageBox(_T("打开项目文件格式错误！"), _T("错误"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFileDialogEx::CheckPngFile(void)
{
	CFile	file;
	CString strPathName;
	BOOL	bIsPng = FALSE;

	POSITION ps = GetStartPosition();
	while (ps != NULL)
	{
		strPathName = GetNextPathName(ps);

		if (!file.Open(strPathName, CFile::modeRead))
		{
			return FALSE;
		}

		int	nLen = static_cast<int>(file.GetLength());
		byte* pBuf = new byte[nLen + 1];
		memset(pBuf, 0, nLen + 1);

		file.Read(pBuf, nLen);
		if (*pBuf == 0x89
			&& *(pBuf+1) == 0x50
			&& *(pBuf+2) == 0x4e
			&& *(pBuf+3) == 0x47
			&& *(pBuf+4) == 0x0d
			&& *(pBuf+5) == 0x0a
			&& *(pBuf+6) == 0x1a
			&& *(pBuf+7) == 0x0a)
		{
			bIsPng = TRUE;
		}
		else
		{
			bIsPng = FALSE;
		}


		file.Close();
		if (pBuf != NULL)
		{
			delete[] pBuf;
			pBuf = NULL;
		}

		if (!bIsPng)
		{
			break;
		}
	}

	return bIsPng;
}

BOOL CFileDialogEx::GetFileType(void) const
{
	return m_bEncrypt;
}
