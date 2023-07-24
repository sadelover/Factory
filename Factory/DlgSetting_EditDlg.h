#pragma once

#include "../ControlEx/CommonButton.h"
#include "gridctrl/GridCtrl.h"

// CDlgSetting_EditDlg �Ի���

class CDlgSetting_EditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting_EditDlg)

public:
	CDlgSetting_EditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetting_EditDlg();

// �Ի�������
	enum { IDD = IDD_CONN_SETTING_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonModify();
private:
	vector<wstring> m_wstrNameList;
	vector<wstring> m_wstrValueList;
	CGridCtrl m_Grid;
};
