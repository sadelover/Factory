#pragma once



// CInputLogicPeriodDlg �Ի���

class CInputLogicPeriodDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputLogicPeriodDlg)

public:
	CInputLogicPeriodDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputLogicPeriodDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT_LOGIC_PERIOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nLogicPeriod;
};
