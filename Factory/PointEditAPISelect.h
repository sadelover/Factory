#pragma once
#include "afxwin.h"


// CPointEditAPISelect 对话框

class CPointEditAPISelect : public CDialog
{
	DECLARE_DYNAMIC(CPointEditAPISelect)

public:
	CPointEditAPISelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPointEditAPISelect();

// 对话框数据
	enum { IDD = IDD_DIALOG_POINT_PARAM_EDIT_APISELECT };
// 对外接口
	CString m_strAPIFunction;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboApiListt();
	afx_msg void OnCbnEditupdateComboApiListt();
	afx_msg void OnBnClickedButtonReset();
	/**
	 *	@brief 从配置文件中读取到API的json列表内容并放到m_APIList中
	*/
	bool readFileIntoAPIList(void);

	typedef struct 
	{
		CString API;
		CString brief;
		CString demo;
	}_APIContent;
	vector<_APIContent> m_APIList;

	CComboBox m_ItemAPIList;
	CString m_strAPIDes;

};
