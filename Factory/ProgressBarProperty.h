#pragma once

//#include "afxwin.h"

// CProgressBarProperty �Ի���

class CEqmDrawProgressBar;
class CDataPointManager;

class CProgressBarProperty : public CDialog
{
	DECLARE_DYNAMIC(CProgressBarProperty)

public:
	CProgressBarProperty(CWnd* pParent = NULL);   // ��׼���캯��
	CProgressBarProperty(CEqmDrawProgressBar* pProgressBar, CDataPointManager* pMap, CWnd* pParent = NULL);   
	virtual ~CProgressBarProperty();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROGRESS_BAR_PROPERTY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
private:
	CEqmDrawProgressBar* m_pProgressBar;
	CDataPointManager* m_pMap;
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticProgressBarColor();
	afx_msg void OnBnClickedButtonProgressBarSelectPoint();
	CComboBox m_combo_layer;
	afx_msg void OnBnClickedButtonProgressBarSelectPointBase();
};
