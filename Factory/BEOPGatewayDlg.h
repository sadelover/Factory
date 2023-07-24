
// BeopEngineDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "PaneArbiter.h"
#include "PointDialog.h"
#include "PaneServerManager.h"
#include "../ControlEx/ButtonST.h"
#include "DlgLoad.h"


#define DLG_WIDTH 775
#define DLG_HEIGHT 520


class CRealTimeTrendDlg;
class CUPPCServerDataAccess;

// CBEOPGatewayDlg 对话框
class CBEOPGatewayDlg : public CDialog
{
// 构造
public:
	CBEOPGatewayDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DIALOG_DEBUGTOOL };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:

	bool InitData();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();



	afx_msg LRESULT LogMsgFunc(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
	
	//=========================================

public:
	vector<CString> m_vecDataEngineLogList;

	NOTIFYICONDATA  m_notifystruct;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void OnCancel();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static UINT WINAPI  ThreadFuncUpdateData(LPVOID lparam);

	void StartUpdateThread();

	CDlgLoad m_PaneLoad;
	//CPaneArbiter m_PaneArbiter;
	//CPointDialog *m_pPointDisplay;
	CPaneServerManager m_PaneServerManager;


	//afx_msg void OnBnClickedButtonArbiter();
	afx_msg void OnBnClickedButtonPoint();

	afx_msg void OnBnClickedButtonCurve();
	afx_msg void OnDestroy();
	//CButtonST m_btnLogic;
	CButtonST m_btnPoint;
	CButtonST m_btnServerManager;


	CButtonST m_btnRealtimeCurve;
	CButtonST m_btnRealtimeCurve2;
	CButtonST m_btnRealtimeCurve3;
	CButtonST m_btnRealtimeCurve4;


	// TODO:  在此添加额外的初始化
	
	CUPPCServerDataAccess * m_pDataAccess;
	wstring m_strServerIP;
	HANDLE  m_EventStopThread;
	HANDLE m_hupdatedatahandle;


	//调试登录后的点表
	vector<DataPointEntry> m_vecServerPointList;

	afx_msg void OnBnClickedButtonServerManager();
	afx_msg void OnBnClickedButtonCurve2();
	afx_msg void OnBnClickedButtonCurve3();
	afx_msg void OnBnClickedButtonCurve4();
};
