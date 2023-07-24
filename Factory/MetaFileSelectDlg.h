#pragma once

#include "MetaFileView.h"

// CMetaFileSelectDlg 对话框
#define META_FILE_VIEW 0x1000
#define ANIMATION_VIEW 0x1001

enum e_wndType
{
	e_picture = 0,
	e_animation
};

class CProjectSqlite;
class CMetaFileView;
class CAnimationView;
class CAnimationPropertyDlg;

class CMetaFileSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CMetaFileSelectDlg)

public:
	CMetaFileSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMetaFileSelectDlg();
	void		SetID(const int nID);
	void		SetPicNum(const int nNum);
	void		ShowPictureView(e_wndType eWndType, E_SubType eSubType);
	void		SetCurrentPicID(const int nID);
	
// 对话框数据
	enum { IDD = IDD_DIALOG_METAFILE_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void SetWndType(const e_wndType eType);
	const e_wndType GetWndType();
	void SetBtnOkShow(const bool bShow);
	void			SetVectorPicNum(const bool bIsPic);
	vector<int>		GetVectorPicNum(void) const;
	void			SetDivideMode(const bool bFlag);

	void			SetShowMode(const int nShowMode);
	int				GetShowMode(void) const;
	
	int				m_nID;
	int				m_nSelectIndex;
	int				m_nPicNum;
	CMenu			m_menu;
	bool			m_bBtnOkShow;
private:
	CMetaFileView*	m_pMetaView;
	CAnimationView* m_pAnimationView;
	CProjectSqlite* m_pProjectSqlite;
	CAnimationPropertyDlg* m_pAnimationPropertyDlg;
	e_wndType		m_eWndType;
	E_SubType		m_eSubType;
	vector<int>		m_vecPicNum;
	bool			m_bSelectDivide;
	int				m_nShowMode;	// 0-图源库；1-当前打开项目
public:
	afx_msg void OnDestroy();
	afx_msg void OnMenuMeta();
	afx_msg void OnMenuAnimation();
	virtual BOOL OnInitDialog();
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnImportMetafile();
	afx_msg void OnImportAnimation();
	afx_msg void OnBnClickedOk();
	afx_msg void OnMenuLibDelete();
	afx_msg void OnMenuLibEdit();
	/*
	@ brief 翻页按钮
	*/
	afx_msg void OnBnClickedButtonPrePage();
	afx_msg void OnBnClickedButtonNextPage();
	CString m_strPageInfo;

	afx_msg void OnBnClickedButtonJump();

	afx_msg void OnTypeAll();
	afx_msg void OnTypeCH();
	afx_msg void OnTypePump();
	afx_msg void OnTypeTower();
	afx_msg void OnTypeAHU();
	afx_msg void OnTypeValue();
	afx_msg void OnTypePipe();
	afx_msg void OnTypeButton();
	afx_msg void OnTypeChart();
	afx_msg void OnTypeLayout();
	afx_msg void OnTypeIcon();
	afx_msg void OnTypeOther();
	CString m_strImageName;
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateMenuMeta(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuAnimation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImportMetafile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImportAnimation(CCmdUI *pCmdUI);
	afx_msg void OnImageDlgExit();
	afx_msg void OnUpdateMenuLibDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuLibEdit(CCmdUI *pCmdUI);
	afx_msg void OnStnClickedPictureName();
};
