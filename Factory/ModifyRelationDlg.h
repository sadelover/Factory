#pragma once


// CModifyRelationDlg �Ի���

class CBEOPLogicManager;
class CLogicDllThread;

class CModifyRelationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyRelationDlg)

public:
	CModifyRelationDlg(CBEOPLogicManager *pLogicManager, CLogicDllThread *pDllThread, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyRelationDlg();


// �Ի�������
	enum { IDD = IDD_DIALOG_MODIFY_RELATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedOk();
	bool Check();
	void Clear();
	bool ExplainRow(CString strRow);
	bool IsDllInThread(CLogicDllThread *pDllThread,wstring wsDllName);

	bool m_bSuccess;
	CString m_strRelation;
	CString m_strCompleteRelation;			//�����Ĺ�ϵ
	vector<wstring> m_strVNameList;
	vector<wstring> m_strPNameList;
	vector<wstring> m_strPTypeList;
	vector<wstring> m_strVExplainList;
	vector<wstring> m_strDllNameList;
	vector<wstring> m_strInOutList;
	vector<wstring> m_strOutputEnableList;
	vector<wstring> m_strDescriptionList;
	
	CBEOPLogicManager *m_pLogicManager;
	CLogicDllThread	  *m_pDllThread;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
};
