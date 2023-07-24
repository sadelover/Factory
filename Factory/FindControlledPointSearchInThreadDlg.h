#pragma once
#include "afxwin.h"
#include "BEOP-DesignerView.h"
#include "BEOPLogicManager.h"
#include "LogicDllThread.h"
#include "LogicParameter.h"
#include "afxcmn.h"
#include "MainFrm.h"

// CControlledPointSearchDlg �Ի���
class CMainFrame;
class CLogicDllThread;
class CBEOPLogicManager;
class CDLLObject;
class CLogicParameter;

#define Progress_MAX 1000

class CFindControlledPointSearchInThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindControlledPointSearchInThreadDlg)

public:
	CFindControlledPointSearchInThreadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindControlledPointSearchInThreadDlg();
	void Set_m_logicList(vector<CLogicDllThread *>& logicList);
	void Set_m_pLogicManager(CBEOPLogicManager * pLogicManager);
	void GetCulThreadStrategyName(CString &cstrThreadName,CString &cstrStrategyName);
	void SetProgressPos(int nPos);
	void SearchPoint();
	void m_cstrFindPointThreadList_Add(CString cstrThreadName);
	void SetCFileView(CFileView* pFileView);
// �Ի�������
	enum { IDD = IDD_CONTROLLED_POINT_SERACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkListThread();
	//������ť����¼�
	afx_msg void OnBnClickedButtonSearch();
private:
	//��ʱ����
	//afx_msg void OnBnClickedOk();
	static UINT SearchPointThread(LPVOID pParam);
	//����ʹ�õĵ���
	CString m_cstrSearchPoint;
	// �б�ؼ�
	CListBox m_listBoxThread;
	// ����������
	CProgressCtrl m_ProgressSearch;

	vector<CLogicDllThread *> m_logicList;
	//
	CBEOPLogicManager *  m_pLogicManager;
	CFileView* m_pCFileView;
};
