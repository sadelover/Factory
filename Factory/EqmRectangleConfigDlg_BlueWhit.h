#pragma once

#include "ComponentDraw/EqmDrawRectangle.h"
#include "afxcolorbutton.h"
#include "afxcmn.h"
#include "afxwin.h"

class CCanvasDlg;

// CEqmRectangleConfig_BlueWhitDlg 对话框

class CEqmRectangleConfigDlg_BlueWhit : public CDialogEx
{
	DECLARE_DYNAMIC(CEqmRectangleConfigDlg_BlueWhit)

public:
	CEqmRectangleConfigDlg_BlueWhit(CCanvasDlg* pCanvalDlg,  CEqmDrawRectangle* pRectangle, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEqmRectangleConfigDlg_BlueWhit();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECTANGLE_CONFIG_BLUE_WHITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAddPoint();
	afx_msg void OnBnClickedButtonDeletePoint();
	afx_msg void OnBnClickedCheckRound();
	afx_msg void OnNMDblclkListPoint(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CEqmDrawRectangle* m_pRectangle;	
	CMFCColorButton m_colorBtn_title;
	int m_nTitileSize;
	CMFCColorButton m_colorBtn_Body;
	int m_nBodySize;
	CMFCColorButton m_colorBtn_paraphrase;
	int m_nParaphraseSize;
	CListCtrl m_listCtrl_point;

	BOOL m_bRound;
	int m_nRoundHeight;
	int m_nRoundWidth;
	CCanvasDlg* m_pCanvalDlg;
	CString m_csTitil;
};
