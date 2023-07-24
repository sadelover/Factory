#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include <hash_map>
// CSetModbus dialog
class CUPPCServerDataAccess;
class COPCConnection_AutoBrowse;

class CSetModbus : public CDialog
{
	DECLARE_DYNAMIC(CSetModbus)

public:
	CSetModbus(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetModbus();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETMODBUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSettingModbus();
	afx_msg void OnBnClickedCheckModbusMode();
	bool InitData();
	virtual BOOL OnInitDialog();
public:
	BOOL					m_bModbusOneByOne;
	int						m_nModbusInterval;
	int						m_nModbusMutilReadCount;
	int						m_nModbusIDInterval;
	int						m_modbustimeout;		//modbus��ʱʱ�䣨modbustimeout/5000ms��
	int						m_modbuspollinterval;   //modbus��ѯʱ�䣨modbuspollinterval/2s��
	int						m_co3pcmdinterval;		//co3p��ѯ�����co3pcmdinterval/50ms��
	int						m_co3ptimeoutl;			//co3p��ʱ�����co3ptimeoutl/5000ms��
	int						m_co3prollinterval;//co3p��ѯ�����co3prollinterval/2s��
	CUPPCServerDataAccess*	      m_pDataAccess;
	afx_msg void OnEnChangeEditModbusInterval();
};
