#pragma once




// CNoModelDlg 对话框

class CNoModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNoModelDlg)

public:
	CNoModelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNoModelDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NO_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strShow;
};
