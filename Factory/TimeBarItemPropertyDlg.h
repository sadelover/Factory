#pragma once


// CTimeBarItemPropertyDlg �Ի���

class CTimeBarItemPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeBarItemPropertyDlg)

public:
	CTimeBarItemPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTimeBarItemPropertyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TIME_BAR_ITEM_PROPERTY_DLG };

	CString m_strIndex;
	CString m_strName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
