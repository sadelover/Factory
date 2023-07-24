#pragma once

#include "afxwin.h"

// CDashBoardPropertyDlg �Ի���

class CEqmDrawDashBoard;
class CDataPointManager;

class CDashBoardPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CDashBoardPropertyDlg)

public:
	CDashBoardPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDashBoardPropertyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DASHBOARD_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	// �Ǳ���������ɫ
	COLORREF m_FontColor;
	// С��λ��
	int m_DecimalPlaces;
	// ������ɫ
	
	afx_msg void OnBnClickedFontcolor();
};
