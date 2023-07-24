#pragma once

#include "afxwin.h"

// CDashBoardPropertyDlg 对话框

class CEqmDrawDashBoard;
class CDataPointManager;

class CDashBoardPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CDashBoardPropertyDlg)

public:
	CDashBoardPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDashBoardPropertyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DASHBOARD_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBindPointName;
	afx_msg void OnBnClickedButtonDashboardSelectPoint();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void	SetDashBoardPointer(CEqmDrawDashBoard* pDashBoard);
	void	SetPointMap(CDataPointManager* pPointMap);

	double  m_dMaxValue;
	double  m_dMinValue;

	int     m_nStyle;

private:
	void	SetMaxMinControlsStatus(const bool bIsMax);
	bool	CheckBindPointValue(const CString strSrc);
	void	InitControl(void);
	CEqmDrawDashBoard*							m_pDashBoard;
	CDataPointManager*							m_pPointMap;
	CMFCColorButton								m_cFontColor;
public:
	afx_msg void OnBnClickedBtnMaxSelPoint();
	afx_msg void OnBnClickedBtnMinSelPoint();
	CButton m_btnMaxSelPoint;
	CButton m_btnMinSelPoint;
	afx_msg void OnBnClickedChkBindMax();
	afx_msg void OnBnClickedChkBindMin();
	BOOL m_bIsBindMax;
	BOOL m_bIsBindMin;
	CString m_strMaxValue;
	CString m_strMinValue;
	CEdit m_editMax;
	CEdit m_editMin;
	CComboBox m_comboLayer;
	int m_nLayer;
	// 仪表盘字体颜色
	COLORREF m_FontColor;
	// 小数位数
	int m_DecimalPlaces;
	// 字体颜色
	
	afx_msg void OnBnClickedFontcolor();
};
