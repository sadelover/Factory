#pragma once


// CAnimationPropertyDlg �Ի���
class CProjectSqlite;
class CAnimationView;
class CMetaFileView;

class CAnimationPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnimationPropertyDlg)

public:
	CAnimationPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnimationPropertyDlg();
	void	AddAnimationSeries(const int nPicID);
// �Ի�������
	enum { IDD = IDD_DIALOG_ADD_ANIMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	int m_nAnimationID;
	CString m_strAniamtionName;
	CString m_strAnimationSeries;
	CProjectSqlite* m_pProjectSqlite;
	CAnimationView* m_pAnimationView;
	CMetaFileView* m_pMetaFileView;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSelectAnimation();
};
