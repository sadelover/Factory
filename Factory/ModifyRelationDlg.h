#pragma once


// CModifyRelationDlg 对话框

class CBEOPLogicManager;
class CLogicDllThread;

class CModifyRelationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyRelationDlg)

public:
	CModifyRelationDlg(CBEOPLogicManager *pLogicManager, CLogicDllThread *pDllThread, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyRelationDlg();


// 对话框数据
	enum { IDD = IDD_DIALOG_MODIFY_RELATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	CString m_strCompleteRelation;			//完整的关系
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
