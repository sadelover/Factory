#pragma once


// CInputRedundencyIPDlg 对话框

class CInputRedundencyIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputRedundencyIPDlg)

public:
	CInputRedundencyIPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputRedundencyIPDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT_REDUNDENCYIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strIP;
};
