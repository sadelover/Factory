#pragma once


class CEqmDrawPage;

// CPageReplaceLinkName dialog

class CPageReplaceLinkName : public CDialogEx
{
	DECLARE_DYNAMIC(CPageReplaceLinkName)

public:
	CPageReplaceLinkName(CEqmDrawPage *pPage, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPageReplaceLinkName();

// Dialog Data
	enum { IDD = IDD_DIALOG_PAGE_REPLACE_LINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	std::wstring Replace(const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced);
	CEqmDrawPage* m_pPage;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strPageName;
	CString m_strReplaceFrom;
	CString m_strReplaceTo;
};
