#pragma once


// CDiagnoseGraphicPlusItemPropertySettingDlg �Ի���

class CDiagnoseGraphicPlusItemPropertySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDiagnoseGraphicPlusItemPropertySettingDlg)

public:
	CDiagnoseGraphicPlusItemPropertySettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiagnoseGraphicPlusItemPropertySettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DIAGNOSE_GRAPHIC_PLUS_ITEM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strItemName;
	CString m_strBindPointName;
	COLORREF m_nColor;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticDiagnoseGraphicPlusItemColor();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDiagnoseGraphicPlusItemBindPointName();
};
