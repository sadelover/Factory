#pragma once


// CBindPropertySettingDlg �Ի���

class CBindPropertySettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CBindPropertySettingDlg)

public:
	CBindPropertySettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBindPropertySettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DIAGNOSE_ITEM_BIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonResultSelect();
	afx_msg void OnBnClickedButtonTreatmentSelect();
	afx_msg void OnBnClickedOk();
	CString m_strBoolPointName;
	CString m_strResultPointName;
	CString m_strResultPointName2;
	CString m_strResultPointName3;
	CString m_strTreatmentPointName;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonResultSelect2();
	afx_msg void OnBnClickedButtonResultSelect3();
	afx_msg void OnBnClickedButtonBoolPointSelect();
};
