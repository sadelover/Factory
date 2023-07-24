#pragma once
#include "./json/json.h"

// CSystemDefineDlg_Child_Script 对话框

class CSystemDefineDlg_Child_Script : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemDefineDlg_Child_Script)

public:
	CSystemDefineDlg_Child_Script(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemDefineDlg_Child_Script();
	/*
		@brief:初始化界面，界面的入口函数
		@param:
			cstrSysDefine[in]:json格式字符串
	*/
	void SetData(CString & cstrSysDefine);
	/*
		@brief:返回json格式字符串
		@param:
			cstrSysDefine[in]:json格式字符串
		@retval:flase不符合json格式
		@retval:true符合json格式
	*/
	bool GetData(CString & cstrSysDefine);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 对话框数据
	enum { IDD = IDD_DIALOG_SYSTEM_DEFINE_Script };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	//edit控件的变量
	CString m_strSystemDefine;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
