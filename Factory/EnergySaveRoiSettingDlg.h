#pragma once


#include "afxwin.h"
#include "atlcomtime.h"
#include "../ComponentDraw/EqmDrawDefines.h"

class CDataPointManager;


// CEnergySaveRoiSettingDlg dialog

class CEnergySaveRoiSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnergySaveRoiSettingDlg)

public:
	CEnergySaveRoiSettingDlg(CDataPointManager* pDataPointMgr, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnergySaveRoiSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ENERGY_SAVE_ROI_SETTING };

public:
	void	SetLayer(const int nLayer);
	void	SetBindPointName(const CString strBindPointName);
	void	SetCost(const double dCost);
	void	SetStartTime(const COleDateTime time);
	void	SetEndTime(const COleDateTime time);
	void	SetOccurTime(const COleDateTime time);

	int		GetPicId(void);
	int		GetLayer(void);
	CString	GetBindPointName(void);
	double	GetCost(void);
	COleDateTime	GetStartTime(void);
	COleDateTime	GetEndTime(void);
	COleDateTime	GetOccurTime(void);

private:
	void	InitControls(void);
	void	InitData(void);

	CDataPointManager*	m_pDataPointMgr;
	COleDateTime m_dateTimeStart;
	COleDateTime m_dateTimeEnd;
	COleDateTime m_dateTimeOccur;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedBtnBindPoint();
	CComboBox m_comboLayer;
	CString m_strBindPointName;
	double m_dCost;
	COleDateTime m_dateStart;
	COleDateTime m_dateEnd;
	COleDateTime m_dateOccur;
	COleDateTime m_timeStart;
	COleDateTime m_timeEnd;
	COleDateTime m_timeOccur;
	int m_nLayer;
};
