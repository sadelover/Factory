#pragma once


// CCreateSample dialog

class CCreateSample : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateSample)

public:
	CCreateSample(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateSample();

// Dialog Data
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nInputNum;
	int m_nOutputNum;
	CString m_strSampleName;
};
