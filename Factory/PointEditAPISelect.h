#pragma once
#include "afxwin.h"


// CPointEditAPISelect �Ի���

class CPointEditAPISelect : public CDialog
{
	DECLARE_DYNAMIC(CPointEditAPISelect)

public:
	CPointEditAPISelect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPointEditAPISelect();

// �Ի�������
	enum { IDD = IDD_DIALOG_POINT_PARAM_EDIT_APISELECT };
// ����ӿ�
	CString m_strAPIFunction;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboApiListt();
	afx_msg void OnCbnEditupdateComboApiListt();
	afx_msg void OnBnClickedButtonReset();
	/**
	 *	@brief �������ļ��ж�ȡ��API��json�б����ݲ��ŵ�m_APIList��
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
