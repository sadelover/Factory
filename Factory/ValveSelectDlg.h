#pragma once


// CValveSelectDlg �Ի���
class CProjectSqlite;

class CValveSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CValveSelectDlg)

public:
	CValveSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CValveSelectDlg();
	int			m_nID;
// �Ի�������
	enum { IDD = IDD_DIALOG_VALVE_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void	SetEqmName();
	CProjectSqlite* m_pProjectSqlite;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};
