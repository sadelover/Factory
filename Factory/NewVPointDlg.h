#pragma once


// CNewVPointDlg �Ի���

class CNewVPointDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewVPointDlg)

public:
	CNewVPointDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewVPointDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADD_VPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPointID;
	int m_nPointType;
	CString m_strPointDef;
	afx_msg void OnBnClickedOk();
};
