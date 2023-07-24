#pragma once

#include "afxwin.h"
#include "afxcmn.h"


class CDataPointManager;
enum eButtonStyle;


// CCheckButtonPlusSettingDlg dialog

class CCheckButtonPlusSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckButtonPlusSettingDlg)

public:
	CCheckButtonPlusSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckButtonPlusSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHKBTN_PLUS_SETTING };

public:
	void	SetPointMap(CDataPointManager* pointmap);
	void	SetPicId(const int nPicId);

	void	SetLayerId(const int nLayerId);
	void	SetButtonStyle(const eButtonStyle eBtnStyle);
	void	SetTextColor(const COLORREF colorTxt);
	void	SetTextSize(const int nSize);
	void	SetSelectValue(const CString strSelVal);
	void	SetUnSelectValue(const CString strUnSelVal);
	void	SetBindPointName(const CString strBindPointName);
	void	SetTextContent(const CString strTxtContent);
	void	SetGroupId(const int nGroupId);
	void	SetExpression(const CString strExpression);
	void	SetOptionsDec(const CString cstrOptionsDec);

	int				GetLayerId(void);
	eButtonStyle	GetButtonStyle(void);
	COLORREF		GetTextColor(void);
	int				GetTextSize(void);
	CString			GetSelectValue(void);
	CString			GetUnSelectValue(void);
	CString			GetBindPointName(void);
	CString			GetTextContent(void);
	int				GetGroupId(void);
	CString			GetExpression(void);

	CString			GetImagePathSelectComm(void);
	CString			GetImagePathSelectOver(void);
	CString			GetImagePathUnSelectComm(void);
	CString			GetImagePathUnSelectOver(void);
	CString			GetOptionsDec(void);

private:
	void	InitControl(void);
	void	InitImagePath(void);
	CDataPointManager*	m_refpointmap;

	int				m_nLayer;
	eButtonStyle	m_eBtnStyle;
	COLORREF		m_colorTxt;
	CString			m_strSelVal;
	CString			m_strUnSelVal;
	CString			m_strBindPointName;
	CString			m_strTxtContent;
	int				m_nGroupId;
	int				m_nTxtSize;
	CString			m_nStrExpression;

	CString			m_strSysPath;
	CString			m_strRadioSelComPath;
	CString			m_strRadioSelOverPath;
	CString			m_strRadioUnSelComPath;
	CString			m_strRadioUnSelOverPath;
	CString			m_strCheckSelComPath;
	CString			m_strCheckSelOverPath;
	CString			m_strCheckUnSelComPath;
	CString			m_strCheckUnSelOverPath;
	CString			m_cstrOptionsDec;			// —°œÓ√Ë ˆ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_comboStyle;
	CComboBox m_comboLayer;
	//int m_nChkBtnStyle;
	//int m_nLayer;
	afx_msg void OnStnClickedTxtColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnBindPoint();
	afx_msg void OnCbnSelchangeComboStyle();
	
	
};
