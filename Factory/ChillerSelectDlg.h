#pragma once


// CChillerSelectDlg �Ի���
class CProjectSqlite;

class CChillerSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CChillerSelectDlg)

public:
	CChillerSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChillerSelectDlg();
	int			m_nID;
// �Ի�������
	enum { IDD = IDD_DIALOG_CHILLER_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void	SetEqmPicID();
	CProjectSqlite* m_pProjectSqlite;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
