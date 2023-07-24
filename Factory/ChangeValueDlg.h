#pragma once
/*
* Copyright (c) 2020,�Ϻ��������ܿƼ����޹�˾
* All rights reserved.
* 
* ժ    Ҫ����ֵ���дֵ���񴰿���
*/

// CChangeValueDlg �Ի���
class CUPPCServerDataAccess;
class CChangeValueDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeValueDlg)

public:
	CChangeValueDlg(CUPPCServerDataAccess *pDataAccess, CString strName,  CString strValue, BOOL bSingle, int nTimeout, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangeValueDlg();
	void SetValue(CString strValue);
	void SetName(CString strName);
	CString GetValue();
	void	SetMultiName(const vector<CString>& vecPoint);
// �Ի�������
	enum { IDD = IDD_DIALOG_POINT_VALUE_CHANGE };

private:
	CString	GetMultiNameFromVector(const vector<CString>& vecPoint);
	BOOL			m_bSingle;
	vector<CString>	m_vecPoint;
	int				m_nTimeout;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CString m_strValue;
	CString m_strName;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CUPPCServerDataAccess *m_pDataAccess;
	int m_nCheckCount;
};
