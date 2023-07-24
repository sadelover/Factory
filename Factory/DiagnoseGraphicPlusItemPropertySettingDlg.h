#pragma once


// CDiagnoseGraphicPlusItemPropertySettingDlg 对话框

class CDiagnoseGraphicPlusItemPropertySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CDiagnoseGraphicPlusItemPropertySettingDlg)

public:
	CDiagnoseGraphicPlusItemPropertySettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiagnoseGraphicPlusItemPropertySettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DIAGNOSE_GRAPHIC_PLUS_ITEM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
