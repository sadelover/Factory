#pragma once


#include "afxwin.h"
#include "../ComponentDraw/EqmDrawMapNavigate.h"
#include "afxcmn.h"

// CMapNavigateSettingDlg dialog

class CMapNavigateSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapNavigateSettingDlg)

public:
	CMapNavigateSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapNavigateSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MAP_NAVI };

public:
	void				SetLayer(const int nLayer);
	void				SetScaleRuler(const int nScaleRuler);
	void				SetMapPageInfo(const vector<CMapPageInfo>& vecMapPageInfo);

	int					GetPicId(void) const;
	int					GetLayer(void) const;
	int					GetScaleRuler(void) const;
	vector<CMapPageInfo> GetMapPageInfo(void) const;

private:
	void	InitControls(void);
	void	InitData(void);

	//void	AddOneItem(const CCoorInfo longitude1, const CCoorInfo latitdue1, const CCoorInfo longitude2, const CCoorInfo latitdue2, const int nPageId);
	void	DeleteItems(void);
	void	AddOneToList(const int nCount, const CMapPageInfo& mapPageInf);
	void	OpenSettingDlg(const CMapPageInfo& mapPageInfo, const int nSelCount);

	vector<CMapPageInfo>	m_vecMapPageInfo;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CComboBox m_comboLayer;
	int m_nLayer;
	afx_msg void OnNMDblclkListMapCfg(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listMapCfg;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	int m_nScale;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit1();
	int m_nEquNo;
};
