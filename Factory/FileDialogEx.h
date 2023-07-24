#pragma once


class CProjectSqlite;
class CMainFrame;

// CFileDialogEx

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CFileDialogEx();

public:
	void	SetOpenType(const int nType);
	void	SetProjectSqlite(CProjectSqlite* pProjSqlite);
	BOOL	GetFileType(void) const;

private:
	BOOL	CheckFileName(void);
	BOOL	CheckPngFile(void);

	CProjectSqlite*	m_pProjSqlite;
	CMainFrame*		m_pMainFrm;
	int				m_nType;	// 0：项目文件名校验；1：PNG格式校验
	BOOL			m_bEncrypt;

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnFileNameOK();
};


