#pragma once




// CNoModelDlg �Ի���

class CNoModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNoModelDlg)

public:
	CNoModelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNoModelDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_NO_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strShow;
};
