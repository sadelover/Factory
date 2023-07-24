#pragma once


// CAddAreaDlg dialog

class CAddAreaDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddAreaDlg)

public:
	CAddAreaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddAreaDlg();
	virtual BOOL OnInitDialog();

	void	InitComboPage();

// Dialog Data
	enum { IDD = IDD_DIALOG_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPointList;
	int m_dXpos;
	int m_dYpos;

	int m_dWidth;
	int m_dHeight;
	CComboBox	m_combo_page;
	int	m_nLinkPageID;

	afx_msg void OnBnClickedButtonAddPoint();
	afx_msg void OnBnClickedButtonAddImage();
	afx_msg void OnBnClickedOk();
	int m_nBitMapID;
	afx_msg void OnBnClickedButtonLinkPage();
};
