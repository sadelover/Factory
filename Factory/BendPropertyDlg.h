#pragma once
#include "afxwin.h"


// CBendPropertyDlg 对话框

class CBendPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CBendPropertyDlg)

public:
	CBendPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBendPropertyDlg();
	void	InitComboRotate();
	void	SetCurSel();
// 对话框数据
	enum { IDD = IDD_DIALOG_BEND_PROPERTY };

private:
	void	InitControl(void);
	void	InitComboSize(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nRotateAngle;
	CComboBox m_combo_rotate_angle;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_comboLayer;
	int m_nLayer;
	CComboBox m_comboBendSize;
	int m_nBendSize;
	afx_msg void OnCbnSelchangeComboLayer();
};
