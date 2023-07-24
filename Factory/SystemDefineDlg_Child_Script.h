#pragma once
#include "./json/json.h"

// CSystemDefineDlg_Child_Script �Ի���

class CSystemDefineDlg_Child_Script : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemDefineDlg_Child_Script)

public:
	CSystemDefineDlg_Child_Script(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemDefineDlg_Child_Script();
	/*
		@brief:��ʼ�����棬�������ں���
		@param:
			cstrSysDefine[in]:json��ʽ�ַ���
	*/
	void SetData(CString & cstrSysDefine);
	/*
		@brief:����json��ʽ�ַ���
		@param:
			cstrSysDefine[in]:json��ʽ�ַ���
		@retval:flase������json��ʽ
		@retval:true����json��ʽ
	*/
	bool GetData(CString & cstrSysDefine);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// �Ի�������
	enum { IDD = IDD_DIALOG_SYSTEM_DEFINE_Script };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//edit�ؼ��ı���
	CString m_strSystemDefine;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
