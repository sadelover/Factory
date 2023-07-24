#pragma once



#include "afxwin.h"
#include "afxcmn.h"
#include "../ComponentDraw/EqmDrawButton.h"

class CProjectSqlite;
class CCanvasDlg;
class CMainFrame;


// CMetaBtnSetDlg dialog

class CMetaBtnSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMetaBtnSetDlg)

public:
	CMetaBtnSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMetaBtnSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_META_BTN_SET };

	int								m_nIDPicComm;
	int								m_nIDPicDown;
	int								m_nIDPicOver;
	int								m_nIDPicDisable;
	int								m_nICPicChecked;
	int								m_nLayer;
	int								m_nLinkPageID;

	Image*							m_pBitmapComm;
	Image*							m_pBitmapDown;
	Image*							m_pBitmapOver;
	Image*							m_pBitmapDisable;
	Image*							m_pBitmapChecked;

	PictureInfo						m_picInfo[5];

	CString							m_strBindSetPointName;
	int								m_nSettingValue;
	CString							m_strTextContent;
	COLORREF						m_nTextColor;
	int								m_nTextSize;
	CString							m_strOperation;

	CProjectSqlite* m_pProjectSqlite;

private:
	CCanvasDlg*	m_pCanvasDlg;
	CMainFrame*	m_pMainFrm;
	RectF		m_rtStyle[7];
	RectF		m_rtStyleSub[5];
	vector<PictureInfo>	m_vecBtnImage;
	vector<PictureInfo>	m_vecBtnStyle;	// 第一行显示
	vector<PictureInfo>::iterator	m_iterImage;
	int			m_nBtnStyleSel;
	int			m_nBtnCurGroup;

private:
	void	InitImageStyleRect(void);
	void	LoadButtonImage(void);
	void	EnableUpNextButton(void);
	int		GetImageSubId(const CString strImageName);	// modify later
	void	InitSelectButton(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
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
	afx_msg void OnBnClickedBtnStyleAdd();
	afx_msg void OnBnClickedBtnStyleDel();
	afx_msg void OnBnClickedBtnStyleUp();
	afx_msg void OnBnClickedBtnStyleNext();
	CButton m_btnStyleUp;
	CButton m_btnStyleNext;
	afx_msg void OnEnChangeEditTextContent();
	afx_msg void OnEnChangeEditTextSize();
};
