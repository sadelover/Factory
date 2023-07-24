#pragma once
#include "afxdtctl.h"
#include "afxcmn.h"
#include "afxwin.h"


// CSearchStrategyInTimeAgeRunDlg �Ի���
#define  WM_UPDATEDATA  WM_USER + 5 
class CSearchStrategyInTimeAgeRunDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchStrategyInTimeAgeRunDlg)

public:
	CSearchStrategyInTimeAgeRunDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSearchStrategyInTimeAgeRunDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEARCH_STRATEGY_IN_TIME_AGE_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//��strLogFilePath�ļ��ж���
	void ReadLogFileContents(CString& strLogFilePath, CString& strLogFileContents);
	//��strLogFileContents�������ҵ���ӽ���С��time��ʱ�䲢���ظ�closestTime
	//strLogFileContents����log�ļ�����ʽ�ȶ�����
	void SearchClosestTime(CString& strLogFileContents, COleDateTime& selectTime, COleDateTime& closestTime);

	// ����ѡ����
	CDateTimeCtrl m_DTPicker_date;
	// ʱ��ѡ����
	CDateTimeCtrl m_DTPicker_time;
	// �༭��
	//CString m_edit_findStrategy;
	CString m_edit_log;
	// ������
	CProgressCtrl m_progressctrl1;
	
	//�������ȡ������ӽ�ѡ��ʱ�������
	struct LogAndFindTime 
	{
		CString strlogName;
		COleDateTime time;
		CString strLogPath;
	};
	CListBox m_list_strategy;
	vector<LogAndFindTime> m_findTimeList;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLbnDblclkListStrategy();
	//������ťִ�еú���
	void BnClickedButtonSearch(void);
	
};
UINT Thread_ButtonSearch(PVOID param);