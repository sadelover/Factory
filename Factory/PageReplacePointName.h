#pragma once


// CPageReplacePointName dialog
class CEqmDrawPage;

class CPageReplacePointName : public CDialog
{
	DECLARE_DYNAMIC(CPageReplacePointName)

public:
	CPageReplacePointName(CEqmDrawPage *pPage,CWnd* pParent = NULL);   // standard constructor
	virtual ~CPageReplacePointName();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_PAGE_REPLACE_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSourcePointName;
	CString m_strReplacePointName;
	CEqmDrawPage* m_pPage;

	std::wstring Replace(const wstring& orignStr, const wstring& oldStr, const wstring& newStr, bool& bReplaced);
	afx_msg void OnBnClickedOk();
};
