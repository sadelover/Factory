#pragma once



// CInputLogicPeriodDlg 对话框

class CInputLogicPeriodDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputLogicPeriodDlg)

public:
	CInputLogicPeriodDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputLogicPeriodDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT_LOGIC_PERIOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nLogicPeriod;
};
