#pragma once

#include "../ControlEx/SkinStatic.h"
#include "../ComponentDraw/EqmDrawDefines.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"

// CTextInfoDlg dialog
class CMainFrame;
class CDataPointManager;

class CTextInfoDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CTextInfoDlg)
    DECLARE_MESSAGE_MAP()
public:
	CTextInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTextInfoDlg();

// Dialog Data
	enum { IDD = IDD_TEXTINFODLG };

public:
    TEXTSHOWMODE GetShowMode();

private:
	CMainFrame*	m_pMainFrm;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
    void        InitComboLayer();
    void        InitComboAlign();
	void		InitComboMode();
	void		InitComboBorderType();
	void		InitComboTextAttribute();

	afx_msg void OnBnClickedOk();
	afx_msg void OnStnClickedStaticChoosecolor();
	afx_msg void OnStnDblclickStaticChoosecolor();
    afx_msg void OnBnClickedButtonChoosedatacom();
    afx_msg void OnStnClickedStaticFont();
    afx_msg void OnCbnSelchangeComboBordertype();
    afx_msg void OnStnClickedChoosetextbordercolor();
	afx_msg void OnBnClickedButtonFontselect();
	afx_msg void OnCbnSelchangeComboTextmode();
	afx_msg void OnBnClickedButtonChoosedatatime();
	afx_msg void OnBnClickedButtonChoosedatapoint();
	afx_msg void OnBnClickedBtnTempFind();
	afx_msg void OnEnChangeEditTextScript();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEditDatetimeTime();
	afx_msg void OnEnChangeEditDatetimePoint();
	afx_msg void OnEnChangeEditTextModeString();
	afx_msg void OnCbnSelchangeComboAlign();
	afx_msg void OnCbnSelchangeComboBktype();
	afx_msg void OnBnClickedBtnTemperFind();

public:
	void SetPointMap(CDataPointManager* pointmap);
	int m_nColor;
	int m_nFontSize;
	COLORREF m_ColorFont;
	double m_dTransparent;
	int m_showmode;
	CString m_strText;

	CSkinStatic m_choostcolorstatic;
	
	CString     m_strPlcValue;

    CComboBox	m_comboshowmode;
    CComboBox   m_comboAlign;
    CComboBox   m_comboLayer;

	CCommonButton           m_btnOK;
	CCommonButton           m_btnCancel;
	CString m_fontname;
    int m_layer;
    int m_align; //0, left, 1 right

	BOOL m_bTextBorderIs3D;
	int  m_nBorderColor;
	int m_nID;
	CSkinStatic m_choosetextbordercolor;
	int         m_TextBorderType;
	CString     m_strTextModeString;
	CDataPointManager* m_refpointmap;

	CString m_strDateTime;
	CString m_strDateTimePoint;
	CString m_strBindScript;

	CComboBox m_comboBorderType;
	CComboBox m_comboTextAttri;

	int m_nTextAttri;
	int m_pointNum;

	/*
	*	m_nTextBKType;
	*	文字背景选择
	*	0-n
	*	无;自定义1;自定义2;自定义3;自定义4;自定义5;自定义6;自定义7;自定义8;自定义9;自定义10;
	*/
	int m_nTextBKType;
	int m_nTempTextBKType;//备份值
	CImage image;
	void TextBcakgroundUPdate(int nTextBG );

	// 显示类型的单位
	CString m_cstrUnit;
};
