#pragma once




// CCheckInfoDlg 对话框

class CCheckInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckInfoDlg)

public:
	CCheckInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHECK_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strShow;
};
