#pragma once


// CValveSelectDlg �Ի���
class CProjectSqlite;

class CMeterPanSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CMeterPanSelectDlg)

public:
	CMeterPanSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMeterPanSelectDlg();
	int			m_nID;
	int			m_nDashBoardType;
// �Ի�������
	enum { IDD = IDD_DIALOG_METER_PAN_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void	SetEqmName();
	CProjectSqlite* m_pProjectSqlite;
	CString	m_strMeterPan1Path;							//�Ǳ���1ͼƬ·��
	CString	m_strMeterPan2Path;							//�Ǳ���2ͼƬ·��
	CString	m_strMeterPan3Path;							//�Ǳ���3ͼƬ·��
	CString	m_strMeterPan4Path;							//�Ǳ���4ͼƬ·��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};
