#pragma once


// CSetEqmPositionDlg �Ի���

class CSetEqmPositionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetEqmPositionDlg)

public:
	CSetEqmPositionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetEqmPositionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_POSITION_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_x;
	int m_y;
	afx_msg void OnBnClickedOk();
	int m_width;
	int m_height;
	afx_msg void OnBnClickedButtonCopyParams();
	afx_msg void OnBnClickedButtonSetParams();
};
