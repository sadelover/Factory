#pragma once
#include "afxwin.h"


// CBendPropertyDlg �Ի���

class CBendPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CBendPropertyDlg)

public:
	CBendPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBendPropertyDlg();
	void	InitComboRotate();
	void	SetCurSel();
// �Ի�������
	enum { IDD = IDD_DIALOG_BEND_PROPERTY };

private:
	void	InitControl(void);
	void	InitComboSize(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
