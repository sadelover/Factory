#pragma once
#include "afxwin.h"


// CInputPipeWidthDlg dialog

class CInputPipeWidthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputPipeWidthDlg)

public:
	CInputPipeWidthDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputPipeWidthDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_INPUT_PIPE_WIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CComboBox m_cWaterFashion;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int m_nPipeWidth;
	int m_nSpeed;
	int m_nFlowDensityIndex;
	int m_nWaterFashion;
	
};
