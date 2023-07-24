#pragma once



class CDataPointManager;
class CCanvasDlg;

// CTemperDistriItemSetDlg dialog

class CTemperDistriItemSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTemperDistriItemSetDlg)

public:
	CTemperDistriItemSetDlg(
		const int nPicId,
		const CString strPointList,
		const CString strBindPtName,
		CDataPointManager* pDataPointMgr,
		CCanvasDlg* pCanvas,
		CWnd* pParent = NULL);   // standard constructor
	virtual ~CTemperDistriItemSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEMPER_DISTRI_ITEM };

private:
	CDataPointManager*	m_pDataPointMgr;
	CCanvasDlg*			m_pCanvas;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnPic();
	afx_msg void OnBnClickedBtnBindptName();
	int m_nPicId;
	CString m_strPointList;
	CString m_strBindPtName;
};
