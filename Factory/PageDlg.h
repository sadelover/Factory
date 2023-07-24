#pragma once
#include "afxwin.h"
#include "CanvasDlg.h"
#include "PaneArbiter.h"
#include "CurveTabDlg.h"
#include "ComManager.h"
#include "CalcModulePanel.h"
#include "SetTable.h"
#include "PointDialog.h"
#include "RealTimeTrendDlg.h"

// CPageDlg dialog

class CPageDlg : public CDialog
{
	DECLARE_DYNAMIC(CPageDlg)

public:
	CPageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPageDlg();

// Dialog Data
	enum { IDD = IDD_PAGEDLG };

public:
	void	ResetArbiterPosition(void);
	CPaneArbiter	m_paneArbiter;
	CCalcModulePanel	m_paneCalc;

public:
	CUPPCServerDataAccess * m_pDataAccess;
	vector<DataPointEntry> m_vecServerPointList;		//���Ե�¼��ĵ��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CCanvasDlg m_dlgCanvas;
	CRect      m_rtCanvas;
	void OnZoomOut();
	void OnZoomIn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool m_bLButtonDown;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	POINT	m_PosLButtonDown;

//20180723////////////////////////////////////////////////////////////////////////
public:
	//��ֲԭCBEOPGatewayDlg����
	bool	InitData();
	void	StartUpdateThread();
	static UINT WINAPI  ThreadFuncUpdateData(LPVOID lparam);
	
	bool	RefreshServerPointList();

public:
	
	CSetTable			m_paneCommSet;		//ͨѶ����
	CComManager			m_paneCommDebug;	//ͨѶ����
	CPointDialog		*m_pPanePoint;		//ʵʱ���
	CCurveTabDlg		m_paneChart;		//ʵʱ�������

	CRealTimeTrendDlg	m_PaneRealtimeTrend1;

	HANDLE				m_hupdatedatahandle;
	HANDLE				m_EventStopThread;
	bool                m_bExitThread;
};
