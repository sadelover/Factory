#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "../ComponentDraw/EqmDrawButton.h"
// CMetaButtonSelectDlg 对话框
class CProjectSqlite;
class CCanvasDlg;

class CMetaButtonSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CMetaButtonSelectDlg)

public:
	CMetaButtonSelectDlg(BOOL bIsCreate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMetaButtonSelectDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_META_BUTTON_SELECT };

	int								m_nIDPicComm;
	int								m_nIDPicOver;
	int								m_nIDPicDown;
	int								m_nIDPicDisable;
	int								m_nLayer;
	int								m_nLinkPageID;
	int								m_nICPicChecked;

	Image*							m_pBitmapComm;
	Image*							m_pBitmapOver;
	Image*							m_pBitmapDown;
	Image*							m_pBitmapDisable;
	Image*							m_pBitmapChecked;

	CString							m_strBindSetPointName;
	//2019-9-11
	/*int							    m_nSettingValue;*/
	CString							    m_nSettingValue;
	CString							m_strTextContent;
	COLORREF						m_nTextColor;
	int								m_nTextSize;
	CString							m_strOperation;

	CProjectSqlite* m_pProjectSqlite;

private:
	void SetShowPicCombination(const int nType);
	void LoadPreButtonImage(void);
	void SetButtonGroupStruct(const CString arrName[5], const CString arrPath[5], vector<PictureInfo>& vecBtn);
	void InsertButtonGroupIntoOpenS3db(const vector<PictureInfo>& vecPicInf, vector<int>& vecId);

	CCanvasDlg*	m_pCanvasDlg;
	int	m_nBtnTop1;
	int	m_nBtnTop2;
	int	m_nBtnTop3;
	int	m_nBtnTop4;
	int	m_nBtnTop5;
	BOOL m_bIsCreate;
	vector<PictureInfo>	m_vecBtnRed;
	vector<PictureInfo>	m_vecBtnYellow;
	vector<PictureInfo>	m_vecBtnGreen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void							OnBnClickedButtonCommon();
	afx_msg void							OnBnClickedButtonDown();
	afx_msg void							OnBnClickedButtonOver();
	afx_msg void							OnBnClickedButtonDisable();
	afx_msg void							OnPaint();
	virtual BOOL							OnInitDialog();
	afx_msg void							OnBnClickedOk();
	afx_msg void							OnBnClickedButtonMetaButtonBindSetPointSelect();
	afx_msg void							OnBnClickedButtonMetaButtonBindSetPointClear();
	afx_msg void							OnStnClickedStaticTextColor();
	afx_msg HBRUSH							OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void									InitList();
	void									InitComboRelation();
	CString									GetRelationTextByInt(const int nInt);
	int										GetIntRelationByText(const CString strText);
	afx_msg void							OnBnClickedButtonRelationAdd();
	afx_msg void							OnBnClickedButtonRelationDelete();
	afx_msg void							OnNMDblclkListRelation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void							OnBnClickedButtonChecked();

	CComboBox								m_combo_layer;
	CListCtrl								m_list_relation;
	vector<_tagRelationExpress>				m_vec;
	CComboBox								m_combo_relation;
	int										m_nRelation;
	CComboBox								m_combo_link_page;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	int										m_nSelectedPicId;
	bool									m_bLBottonDown;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnDlEnableBrowse();
	afx_msg void OnBnClickedBtnDlNameBrowse();
	CString m_strDownloadEnable;
	CString m_strDownloadName;
	afx_msg void OnCbnSelchangeComboPreCombine();
	CComboBox m_comboPreCombine;
	afx_msg void OnEnChangeEditDlEnable();
	afx_msg void OnEnChangeEditDlName();
	afx_msg void OnCbnSelchangeComboLinkPage();
};
