// UPPC-Designer.h : main header file for the UPPC-Designer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
       // main symbols
// CBEOPDesignerApp:
// See UPPC-Designer.cpp for the implementation of this class
//
class CBEOPDesignerApp : public CWinAppEx
{
    DECLARE_MESSAGE_MAP()
public:
	CBEOPDesignerApp();
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	void AssociateFile();

public:
		CString sFilePathName;
			BOOL  m_bHiColorIcons;
	CRecentFileList* m_pRecFileList;
	afx_msg void OnFileMruFile1(UINT nID);

	wstring m_wstr_app_name;
private:
	BOOL CheckFileName(CString cstrPathName);
};
extern CBEOPDesignerApp theApp;