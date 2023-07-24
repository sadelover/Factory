#pragma once


// CNewEquipmentPointsDlg dialog

class CNewEquipmentPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewEquipmentPointsDlg)

public:
	CNewEquipmentPointsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewEquipmentPointsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEW_EQUIPMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditNumChiller();
	afx_msg void OnBnClickedOk();
	int m_nChillerCount;//冷机台数
	int m_nEquipmentStartNum;//设备起始编号
	int m_nChTowerCount;//冷却塔台数
	int m_nCWPCount;//冷机泵台数
	int m_nPriChCount;//一次冷冻泵
	int m_nSecCWPCount;//二次
	int m_nPriHWPCount;//一次热泵
	int m_nSecHCWCount;//二次
	CString m_strNewRoomName;//机房名
	BOOL m_bAddControlPoints;//
	BOOL m_bChillerHasValve;//
	int m_nPriChWPCount;//
	int m_nFanBlowerCount;//每台风机台数
	CString m_PlantDescription;
};
