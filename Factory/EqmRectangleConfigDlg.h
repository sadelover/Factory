#pragma once

#include "ComponentDraw/EqmDrawRectangle.h"
#include "EqmRectangleConfigDlg_BlueWhit.h"
#include "EqmRectangleConfigDlg_Rect.h"
#include "afxwin.h"
//RectangleConfigDlg �Ի���

class CEqmRectangleConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEqmRectangleConfigDlg)

public:
	CEqmRectangleConfigDlg(CEqmDrawRectangle* pDrawRectangle,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEqmRectangleConfigDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECTANGLE_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
private:
	CEqmRectangleConfigDlg_BlueWhit m_childDlg_bw;
	CEqmRectangleConfigDlg_Rect		m_childDlg_rect;
	CComboBox m_combo_childDlg;
	int m_nLastSelIndex;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboChildDlg();
};
