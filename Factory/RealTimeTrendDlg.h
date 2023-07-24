#pragma once


#include "DB_BasicIO/StructDefine/Define_Record_Point_List.h"
//#include "DB_BasicIO/TrendAnalyzeDataAccess.h"
#include "SelectPointDlg.h"
#include "ControlEx/ButtonST.h"

#include "ChartViewer.h"
#include "chartdir.h"
#include "bchartdir.h"
#include "ControlEx/ImageButton.h"

struct CReadRealData{
	CReadRealData::CReadRealData()
	{
		mIdx = 0;
		mPointIdx = 0;
		mEnable = false;
	}
	bool        mEnable;
	UINT        mIdx;
	UINT        mPointIdx;
	int         mColor;
	wstring     mStrPlcShortName;
	wstring     mStrBtnName;
	wstring		strDescription;
	wstring		strUnit;
	wstring		strSourceType;
	wstring		strParam1;
	vector<RecordPointValue>    m_vecRecord;
	vector<double>              m_vecData;
};

class COPCCtrl;
class CDataPointManager;
class CUPPCServerDataAccess;
class CPageDlg;

// CRealTimeTrendDlg dialog

class CRealTimeTrendDlg : public CDialog
{
	DECLARE_DYNAMIC(CRealTimeTrendDlg)

public:
	CRealTimeTrendDlg(CWnd* pParent = NULL);
	CRealTimeTrendDlg(CUPPCServerDataAccess*	pBeopDataAccess, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRealTimeTrendDlg();

	void             SetParentHandle(const HWND& hWnd);
	HWND             GetParentHandle() const;

	void			SaveRealPoint(UINT dlgIdx,UINT buttonIdx,UINT pointIdx,
								const wstring& wstrPlcShort,  const wstring& strDescription, 
								const wstring& strUnit, const wstring& strSourceType , const wstring& strParam1);

	void			DeleteRealPoint(UINT dlgIdx,UINT buttonIdx);
	void			SetDlgID(UINT dlgIdx);
	void	InitButtonData(UINT dlgIdx);
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedButtonParam1();
	afx_msg void OnBnClickedButtonParam2();
	afx_msg void OnBnClickedButtonParam3();
	afx_msg void OnBnClickedButtonParam4();
	afx_msg void OnBnClickedButtonParam5();
	void OnBtnParam(UINT idx);

	afx_msg void OnBnClickedButtonDelete1();
	afx_msg void OnBnClickedButtonDelete2();
	afx_msg void OnBnClickedButtonDelete3();
	afx_msg void OnBnClickedButtonDelete4();
	afx_msg void OnBnClickedButtonDelete5();

	afx_msg void OnBnClickedButtonDeleteAll();

	afx_msg void OnDtnDatetimechangeEnddateCurve(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonMoveline();
	afx_msg void OnBnClickedButtonMoveright();
	afx_msg void OnDtnCloseupStartdateCurve(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnUserstringStartdateCurve(NMHDR *pNMHDR, LRESULT *pResult);

// Dialog Data
	enum { IDD = IDD_DIALOG_REALTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void    SetButtonProp(UINT buttonIdx,UINT pointIdx,
		const wstring& wstrPlcShort,  const wstring& strDescription, const wstring& strUnit, const wstring& strSourceType , const wstring& strParam1);
	CImageButton*   CreateButtonSt(UINT id, const wstring& wstrName, const CRect& rect);
	void	InitGraph();
	void    InitButton();
	void	InitChartLineColor();
	void    InitSize();
	void    InitSaveData();

	bool	IsShowInFoDlg();
	void    DrawChart();
	void	DrawHiseryCurveChart();

	void	UpdateVectorData();

	void EnableControlsOper();
	void DisableControlsOper();

	bool	CompTwoTime();
	bool	CompTwoTimeEnd();
	void    ReSize(int nID);

	void    OnTimerUpdate();

	void GetPointInfo(vector<double>  &m_vecRecord,int &nNum, double &nAvg, double &nMax, double &nMin);

	void    ShiftData(double *data, int len, double newValue);

	void    OnBtnDelete(UINT idx);

	void	OnBtnInfo(UINT idx);

	virtual CWnd*   GetDlgItem(int nID) const;
public:
		void InitPointView();
		void  Exit();
protected:
	HWND		    m_hParentHandle;

public:

	vector<CReadRealData>       g_vecReadData;
	vector<double>          m_vecTime;

	COleDateTime m_startDate;
	COleDateTime m_endDate;
	COleDateTime m_startTime;
	COleDateTime m_endTime;
	COleDateTime oleTimeStart,  oleTimeEnd ;
	COleDateTime m_SaveStartDate,m_SaveEndDate,m_SaveStartTime,m_SaveEndTime;
	int pointIndex;

	int m__nTimeRange;
	bool FirstInitChart;

	CRect	m_RectChart;
	CChartViewer m_ChartViewer;
	int    m_nMax;
	vector<int>   m_vecLineColor;

	CButton m_btn_param1;
	CButton m_btn_param2;
	CButton m_btn_param3;
	CButton m_btn_param4;
	CButton m_btn_param5;
	vector<CButton*>   m_vecBtnParam;

	CButton m_delete_param1;
	CButton m_delete_param2;
	CButton m_delete_param3;
	CButton m_delete_Param4;
	CButton m_delete_param5;
	CButton m_info_param1;
	CButton m_info_param2;
	CButton m_info_param3;
	CButton m_info_param4;
	CButton m_info_param5;
	CButton m_btn_moveTime;

	CButton m_btn_move_right;

	CButton m_delete_all;

	float m_Multiple_width;
	float m_Mutiple_heith;
	bool m_change_flag;
	int  m_nWidth;
	int  m_nHeight;
	int m_nDlgWidth;
	int m_nDlgHeight;

	UINT m_uDlgIdx;

	//vector<CReadRealData>       m_vecReadData;
	//vector<double>          m_vecTime;

	CUPPCServerDataAccess*	m_pServerDataAccess;

	//CDataPointManager*     m_pointmap;
	CPageDlg *m_pBEOPGatewayDlg;

	std::vector<wstring>  m_iniWatchPointList;

	afx_msg void OnBnClickedButtonInfo1();
	afx_msg void OnBnClickedButtonInfo2();
	afx_msg void OnBnClickedButtonInfo3();
	afx_msg void OnBnClickedButtonInfo4();
	afx_msg void OnBnClickedButtonInfo5();

	bool m_bIsNeedUpdateLimits;
	double m_dYMax;
	double m_dYMin;
	afx_msg void OnBnClickedButtonManual();
	afx_msg void OnBnClickedButtonAuto();
};
