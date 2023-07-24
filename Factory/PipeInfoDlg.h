#pragma once

#include "../ControlEx/SkinStatic.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"
// CPipeInfoDlg dialog

class CDataPointManager;

class CPipeInfoDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CPipeInfoDlg)

public:
	CPipeInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPipeInfoDlg();

// Dialog Data
	enum { IDD = IDD_PIPEINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
    void        InitComboLayer();
	void        InitSelectID();
	void		InitComboLogic();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnStnClickedStaticInnercolor();
	afx_msg void OnStnClickedStaticSidecolor();
    afx_msg void OnCbnSelchangeComboPipewidth();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedButtonSelectid();
    afx_msg void OnBnClickedButtonOpposite();
	afx_msg void OnBnClickedButtonPipeClear();
	afx_msg void OnBnClickedButtonAddImage();
	afx_msg void OnBnClickedButtonTestScript();
	afx_msg void OnCbnSelchangeComboType();
public:
	void	SetPointmap(CDataPointManager* pointmap);
private:
	void		InitWaterFashionComboBox();
	// 水流形状控制
	CComboBox m_cWaterFashion;
public:
	DWORD                   m_nInnerColor;
	DWORD                   m_nSideColor;
	CSkinStatic             m_stcInnerColor;
	CSkinStatic             m_stcSideColor;
	CComboBox               m_combo_pipewidth;
    CComboBox               m_comboLayer;

    int	                    m_pipewidth;
    int                     m_layer;
    int						m_nDirection;
    vector<CString>         m_vecDataName;

    CString                 m_strID;
	CCommonButton           m_btnOK;
	CCommonButton           m_btnCancel;

	CDataPointManager*		m_refpointmap;
	CComboBox				m_combo_logic;
	int						m_nLogic;
	int						m_nPipeType;			

	int						m_nPipeBitMapID;
	int						m_nPipeDenSity;			//0:疏松;1 较疏松;2 一般;3 较密集;4 密集;
	int						m_nPipeSpeed;			//0 慢;1 中;2 快;
	
	int						m_WaterFashion;
	
	
};
