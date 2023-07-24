#pragma once
#include "atlcomtime.h"
#include "afxwin.h"


// CCompletionData �Ի���

class CCompletionData : public CDialogEx
{
	DECLARE_DYNAMIC(CCompletionData)

public:
	CCompletionData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCompletionData();

// �Ի�������
	enum { IDD = IDD_DIALOG_COMPLETION_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CComboBox m_TimeDivisionCon;
	DECLARE_MESSAGE_MAP()
public:
	CString m_CompletionDataList;
	COleDateTime m_tFrom;
	COleDateTime m_tTo;
	int m_nPeriodType;
	afx_msg void OnEnChangeEditPoint();
	// ʱ��ָ�ѡ�����
	
	virtual BOOL OnInitDialog();
};
