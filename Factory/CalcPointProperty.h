#pragma once


#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"

// CCalcPointProperty �Ի���

class CCalcPointProperty : public CSkinDlg
{
	DECLARE_DYNAMIC(CCalcPointProperty)

public:
	CCalcPointProperty(const BOOL bIsAdd, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalcPointProperty();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALC_POINT_PROPERTY };

private:
	BOOL	m_bIsAdd;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strName;
	int m_nPeriod;
	virtual void OnOK();
	CComboBox m_comboPeriod;
};
