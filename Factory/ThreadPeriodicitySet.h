#pragma once




// CThreadPeriodicitySet dialog

class CThreadPeriodicitySet : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadPeriodicitySet)

public:
	CThreadPeriodicitySet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThreadPeriodicitySet();

// Dialog Data
	enum { IDD = IDD_DIALOG_THREAD_PERIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nThreadPeriodicity;
};
