#pragma once

#include "../ControlEx/SkinDlg.h"

#include "string"
#include "afxwin.h"
#include <vector>

class CSetConstValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetConstValueDlg)

public:
	CSetConstValueDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetConstValueDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SetTime};

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();	
	bool StrIsNum(std::wstring StrCheck);
	bool UpdateAllPointList();
	void UpdateListPoint();

	void UpdateListPoint_Con();

	bool m_bCanModifyType;

	CString m_sSelStrName;
	CString m_dSelCurValue;
	CString m_strParamSetting;

	std::vector<CString> vecPointList;
	std::vector<int>     vecPointIndex;
	std::vector<CString> vecSearchPointList;
	std::vector<int>	 vecSearchpointIndex;

	//afx_msg void OnEnKillfocusEditSerch();
	afx_msg void OnEnChangeEditSerch();
	CString m_strSetValue;
	int  iInputOrOutput;
public:
	std::vector<CString> vecConSearchPointList;
	std::vector<int>	 vecConSearchpointIndex;
	int					 m_iConNewSelIndex;
	CString				 m_strConSetValue;

//	CComboBox	m_ConComboBoxList;
//	CString		m_sConSerch;
	//afx_msg void OnEnKillfocusEditConserch();
	CString     m_sValueType;
	CString     m_ConValue;
	CComboBox m_ComPointType;
	afx_msg void OnEnChangeEditstrategyname();

	afx_msg void OnBnClickedButtonChoosePoint();
	afx_msg void OnBnClickedButtonTestScript();
	void ReloadNameAndIndexForPoint();
};
