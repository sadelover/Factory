#pragma once

#include "atltime.h"
#include "afxcmn.h"
#include "ReportCtrl.h"
#include "afxwin.h"
#include "../ComponentDraw/EqmDrawDefines.h"

// CChartPropertyDlg �Ի���
class CEqmDrawBase;
class CDataPointManager;
class CEqmDrawLineGraphic;
class CEqmDrawBarGraphic;
class CEqmDrawPieGraphic;

class CChartPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CChartPropertyDlg)

public:
	CChartPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CChartPropertyDlg(CEqmDrawBase* pBase, CWnd* pParent = NULL);
	virtual ~CChartPropertyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHART_PROPERTY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strChartTitle;						//ͼ����ʾ�ı���
	int m_nUpdateInterval;							//����ʱ����
	CString m_strXName;								//������ʾ������
	CString m_strXUnit;								//���ᵥλ
	CString m_strYName;								//������ʾ������
	CReportCtrl m_listGraphItem;					//ͼ������б�ؼ�
	CComboBox m_comboXUnit;							//������Ͽ�ؼ�
	E_BAR_TYPE m_eBarType;							//��״ͼ������
	CString m_strStartPoint;						//�󶨵���ʼʱ���
	CString m_strEndPoint;							//�󶨵Ľ���ʱ���
	afx_msg void OnBnClickedButtonAddItem();
	afx_msg void OnBnClickedButtonDeleteItem();
	void	SetPointMap(CDataPointManager* pPointMap);
	int m_nQueryType;
	COLORREF m_nTextColor;
private:
	void	ShowWindowTitle();
	void	ShowDlgItem();
	void	EnableLeftBottomControls(const BOOL bShow);
	void	InitListCtrl();
	void	InitComboCtrl();
	void	InitBarComboCtrl();
	void	EnableCompareBarControls(const BOOL bShow);
	void	EnableItemControls(const BOOL bShow);
	void	EnableMaxValueControl(const BOOL bShow);
	void	EnableTimePointSelectControls(const BOOL bShow);
	CDataPointManager* m_pPointMap;
	CEqmDrawBase* m_pBase;
	CEqmDrawLineGraphic* m_pLineGraphic;
	CEqmDrawBarGraphic* m_pBarGraphic;
	CEqmDrawPieGraphic* m_pPieGraphic;
	int	m_nColorIndex;
public:
	afx_msg void OnNMDblclkListGraphItemList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	int IsValidDate( COleDateTime startDate, COleDateTime endDate, COleDateTime startTime, COleDateTime endTime );
	CComboBox m_combo_bar_type;
	afx_msg void OnCbnSelchangeComboBarType();
	afx_msg void OnBnClickedButtonAssoPoint1();
	afx_msg void OnBnClickedButtonAssoPoint2();
	CString m_strAssoPoint1;
	CString m_strAssoPoint2;
	CString m_strNameItem1;
	CString m_strNameItem2;
	CString m_strMaxValue;
	afx_msg void OnBnClickedButtonStartPoint();
	afx_msg void OnBnClickedButtonStartPointClear();
	CComboBox m_combo_query_type;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticTextColor();
	double m_dYMax;
	double m_dYMin;
	int m_nCheckMax;
	int m_nCheckMin;
	CComboBox m_comboLayer;
	int m_nLayer;
};
