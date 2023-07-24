#pragma once
#include "gridctrl/GridCtrl.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"
#include "ServerDataAccess/UPPCServerDataAccess.h"
#include "afxcmn.h"

class CUPPCServerDataAccess;
class CMainFrame;

// CChangePointNameForHistoricalDataDlg 对话框

class CChangePointNameForHistoricalDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangePointNameForHistoricalDataDlg)

public:
	CChangePointNameForHistoricalDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangePointNameForHistoricalDataDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHANGE_POINT_NAME_FOR_HISTORICAL_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void Set_m_pDataAccess(CUPPCServerDataAccess* pDataAccess);
	CUPPCServerDataAccess* Get_m_pDataAccess();
	void SetProgressPos(int nPos);
	void GetAllPointName(vector<wstring>& _wstrOldNameList, vector<wstring>& _wstrNewNameList);
	void SetButtonModifyState(BOOL bState);
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetGridRow();
	afx_msg void OnBnClickedButtonQueryPoint();
	afx_msg void OnBnClickedButtonModify();

	CUPPCServerDataAccess* m_pDataAccess;
	vector<CString> cstrTableNameList;
	vector<CString> cstrOldPointNameList;
	vector<CString> cstrNewPointNameList;
	CString m_cstrTempPointName;
	int m_nGridRow;

	CGridCtrl m_Grid_NameList;
	CProgressCtrl m_PgsCtlModify;

};
