#pragma once


// CChartItemPropertyDlg 对话框
class CDataPointManager;

class CChartItemPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CChartItemPropertyDlg)

public:
	CChartItemPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChartItemPropertyDlg();
	void	SetPointMap(CDataPointManager* pPointMap);

// 对话框数据
	enum { IDD = IDD_DIALOG_ITEM_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strItemName;
	CString m_strBindPointName;
	int		m_nBindPointStoreType;
	COLORREF m_nColor;
	CComboBox m_combo_point_type;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonItemBindPointSelect();
private:
	CDataPointManager* m_pPointMap;
public:
	afx_msg void OnStnClickedStaticItemColor();
	afx_msg void OnCbnSelchangeComboItemBindPointType();
};
