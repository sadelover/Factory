#pragma once


// CInputRedundencyIPDlg �Ի���

class CInputRedundencyIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputRedundencyIPDlg)

public:
	CInputRedundencyIPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputRedundencyIPDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT_REDUNDENCYIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strIP;
};
