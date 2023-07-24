#pragma once


#include "afxwin.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"

// CInputLogicNameDlg �Ի���

class CInputLogicNameDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CInputLogicNameDlg)

public:
	CInputLogicNameDlg(bool bIsCreate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputLogicNameDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT_LOGIC_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	bool	m_bIsCreate;	// true:�½�����, false:�޸Ĳ���
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	CString m_strLogicName;
};
