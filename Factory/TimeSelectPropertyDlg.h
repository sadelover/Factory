#pragma once

//#include "afxdtctl.h"

// CTimeSelectPropertyDlg �Ի���

class CDataPointManager;

class CTimeSelectPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeSelectPropertyDlg)

public:
	CTimeSelectPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTimeSelectPropertyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TIME_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonTimePointSelect();
	afx_msg void OnBnClickedButtonTimeSelectRw();
	void SetPointMap( CDataPointManager* pPointMap );

	CDataPointManager*				m_pPointMap;
	CString							m_strBindPointName;
	int								m_rwProperty;//��дѡ��
	int								m_radioDateTimeSelect;//ʱ�����ݸ�ʽѡ��

};
