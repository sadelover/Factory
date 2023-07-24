#pragma once



typedef enum tagRoiTimeType
{
	RoiTimeStart	= 0,
	RoiTimeOccur,
	RoiTimeEnd
}RoiTimeType;


// CTimePickDlg 对话框

class CTimePickDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimePickDlg)

public:
	CTimePickDlg(const RoiTimeType timeType, const COleDateTime tmStart, const COleDateTime tmOccur, const COleDateTime tmEnd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimePickDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIME_PICKER };

private:
	RoiTimeType		m_roiTimeType;
	COleDateTime	m_tmStart;
	COleDateTime	m_tmOccur;
	COleDateTime	m_tmEnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_Date;
	CDateTimeCtrl m_Time;
	int		m_nDateTime;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
