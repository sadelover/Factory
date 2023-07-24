#pragma once
#include "afxwin.h"
#include "BEOP-DesignerView.h"
#include "BEOPLogicManager.h"
#include "LogicDllThread.h"
#include "LogicParameter.h"
#include "afxcmn.h"
#include "MainFrm.h"

// CSearchStrategyDlg �Ի���

class CSearchStrategyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchStrategyDlg)

public:
	CSearchStrategyDlg(CBEOPLogicManager * pLogicManager, vector<CLogicDllThread *> * pLogicList, CFileView* pFileView, HTREEITEM pTreeRoot, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSearchStrategyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEARCH_STRATEGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnNMDblclkListStrategy(NMHDR *pNMHDR, LRESULT *pResult);
	
	void SearchStartegy();
private:
	HTREEITEM FindHTREEITEM(HTREEITEM  item, CString strtext);
	static UINT Thread_ButtonSearch(LPVOID pParam);
	// ���������������ɴ��ո�
	CString m_strStrategyNameList;
	// �����б�
	vector<CLogicDllThread *> * m_pLogicList;
	//	���Թ���
	CBEOPLogicManager *  m_pLogicManager;
	//  FileView ���ĸ�����
	CFileView* m_pFileView;
	// FileView ��״�ṹ�ĸ�ָ��
	HTREEITEM m_pTreeRoot;
	//�б�ؼ�������ʾ�������Ŀؼ�
	CListCtrl m_list_strategy;
	//���԰�
	struct  StrategyStruct
	{
		CString strThreadName;
		CString strStrategyName;
		StrategyStruct(CString threadName, CString strategyName):strThreadName(threadName),strStrategyName(strategyName){}
	};
	vector<StrategyStruct> m_foundStrategyList;

	CProgressCtrl m_ProgressCtrl;
};
