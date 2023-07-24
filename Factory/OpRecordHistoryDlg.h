#pragma once
#include "gridctrl/GridCtrl.h"

// COpRecordHistoryDlg dialog

class COpRecordHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpRecordHistoryDlg)

public:
	COpRecordHistoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COpRecordHistoryDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPRECORD_HISTROY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonQuery();




	CGridCtrl m_Grid;
	COleDateTime m_DateFrom;
	COleDateTime m_DateTo;



	CString m_strServer;
	int m_nServerPort;





	COleDateTime m_tStart;
	COleDateTime m_tEnd;

};
