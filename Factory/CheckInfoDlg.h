#pragma once




// CCheckInfoDlg �Ի���

class CCheckInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckInfoDlg)

public:
	CCheckInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHECK_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strShow;
};
