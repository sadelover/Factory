#pragma once


#include "../ComponentDraw/EqmDrawTemperDistri.h"
#include "afxcmn.h"
#include "afxwin.h"


class CMainFrame;
class CCanvasDlg;

// CTemperDistriSettingDlg dialog

class CTemperDistriSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTemperDistriSettingDlg)

public:
	CTemperDistriSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTemperDistriSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEMPER_DISTRI };

public:
	vector<TemperDistriInfo>	GetTemperDistriInfo(void) const;
	void						SetTemperDistriInfo(const vector<TemperDistriInfo> vecTemperDistriInfo);

private:
	void	InitControls(void);
	void	InitData(void);

	vector<TemperDistriInfo>	m_vecTemperDistriInfo;
	CMainFrame*					m_pMainFrm;
	CCanvasDlg*					m_pCanvas;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	CListCtrl m_listShow;
	CComboBox m_comboLayer;
	int m_nLayer;
};
