#pragma once

#include "ComponentDraw/EqmDrawRectangle.h"
#include "afxcolorbutton.h"
#include "afxwin.h"
// CEqmRectangleConfig_RectDlg �Ի���

class CEqmRectangleConfigDlg_Rect : public CDialogEx
{
	DECLARE_DYNAMIC(CEqmRectangleConfigDlg_Rect)

public:
	CEqmRectangleConfigDlg_Rect(CEqmDrawRectangle* pRectangle, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEqmRectangleConfigDlg_Rect();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECTANGLE_CONFIG_RECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckFill();
	afx_msg void OnBnClickedCheckRound();

	CEqmDrawRectangle* m_pRectangle;
	CMFCColorButton m_colorBtn_border;
	int m_nBordeTransparency;
	BOOL m_bFill;
	CMFCColorButton m_colorBtn_fill;
	int m_nFillTransparency;
	BOOL m_bRound;
	int m_nRoundHeight;
	int m_nRoundWidth;
};
