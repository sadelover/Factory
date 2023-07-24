#pragma once


// CModbusComConfigDlg dialog

class CModbusComConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModbusComConfigDlg)

public:
	CModbusComConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModbusComConfigDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODBUS_COM_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strConfig;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
