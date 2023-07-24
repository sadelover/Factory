#pragma once

#include "../ControlEx/SkinStatic.h"
#include "../ControlEx/SkinDlg.h"
#include "../ControlEx/CommonButton.h"
#include "afxwin.h"
#include "AnimationView.h"
#include "MetaFileView.h"

class CCanvasDlg;
class CProjectSqlite;
// CStateChangeDlg 对话框

class CStateChangeDlg : public CSkinDlg
{
	DECLARE_DYNAMIC(CStateChangeDlg)

public:
	CStateChangeDlg(CCanvasDlg* pCanvalDlg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStateChangeDlg();
	
// 对话框数据
	enum { IDD = IDD_DIALOG_STATE_CHANGE };

private:
	CCanvasDlg*	m_pCanvasDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nValue;
	int m_nPicID;
	int m_nFrame;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPic();
	afx_msg void OnBnClickedButtonAni();
	static bool CompareFunction(const int pElementA,const int pElementB);
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void SetProjectSqlPointer(CProjectSqlite* pProjectSqlite);
	void ChangePictureByID();

	bool			m_bAnimationMode;
	CProjectSqlite* m_pProjectSqlite;
	AnimationView*	m_pAnimationView;
	PictureView*		m_pPictureView;
	int m_nInterval;
	CCommonButton m_btnOk;
	CCommonButton m_btnCancel;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
