#pragma once


// CPointIgnoreDefDlg �Ի���

class CPointIgnoreDefDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPointIgnoreDefDlg)

public:
	CPointIgnoreDefDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPointIgnoreDefDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_POINT_IGNORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditPoint();
	CString m_point_ignore_list;
};
