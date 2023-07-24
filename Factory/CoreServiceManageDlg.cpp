// CoreServiceManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoreServiceManageDlg.h"
#include "afxdialogex.h"

#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"
#include "NewProcessMonitorDlg.h"
#include "../ControlEx/WarningDlg.h"
#include "gridctrl/GridCtrl.h"
// CCoreServiceManageDlg dialog

IMPLEMENT_DYNAMIC(CCoreServiceManageDlg, CDialogEx)

CCoreServiceManageDlg::CCoreServiceManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCoreServiceManageDlg::IDD, pParent)
	, m_bEnableModbusClient(FALSE)
	, m_bEnableKeysight(FALSE)
	, m_bEnableModbusServer(FALSE)
	, m_strServerIP(_T("localhost"))
{
	m_strServer = _T("127.0.0.1");
	m_nServerPort = 5000;

}

CCoreServiceManageDlg::~CCoreServiceManageDlg()
{
}

void CCoreServiceManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MODBUS_CLIENT, m_bEnableModbusClient);
	DDX_Check(pDX, IDC_CHECK_ENABLE_KEYSIGHT, m_bEnableKeysight);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MODBUS_SERVER, m_bEnableModbusServer);

	DDX_Control(pDX, IDC_CUSTOM_PROCESS, m_Grid);
}


BEGIN_MESSAGE_MAP(CCoreServiceManageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ITEM2, &CCoreServiceManageDlg::OnBnClickedButtonAddItem2)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ITEM, &CCoreServiceManageDlg::OnBnClickedButtonAddItem)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ITEM, &CCoreServiceManageDlg::OnBnClickedButtonDeleteItem)
	ON_BN_CLICKED(IDOK, &CCoreServiceManageDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCoreServiceManageDlg message handlers


void CCoreServiceManageDlg::OnBnClickedButtonAddItem2()
{
	// TODO: Add your control notification handler code here
}


BOOL CCoreServiceManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	RefreshTable();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCoreServiceManageDlg::OnBnClickedButtonAddItem()
{
	// TODO: Add your control notification handler code here
	CNewProcessMonitorDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		CString strProcessName = dlg.m_strProcessName;
		CString strDirName = dlg.m_strDirName;

		CString strResult;
		Json::Value myRoot;
		myRoot["processName"] = Project::Tools::WideCharToUtf8(strProcessName.GetString());
		myRoot["directory"] = Project::Tools::WideCharToUtf8(strDirName.GetString());

		string szBuf = myRoot.toStyledString();
		CHttpOperation http_log(m_strServer, m_nServerPort, _T("process/addProcessName"));
		bool bRet_log = http_log.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
		if(bRet_log)
		{
			RefreshTable();
		}
		else
		{
			AfxMessageBox(L"保存失败，请确认是否开启dompysite。");
		}

	}	
}

void CCoreServiceManageDlg::RefreshTable()
{
	m_Grid.DeleteAllItems();
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3);
	m_Grid.SetItemText(0,0, _T("编号"));
	m_Grid.SetItemText(0,1, _T("目录"));
	m_Grid.SetItemText(0,2, _T("进程名"));

	m_Grid.SetDefCellMargin(2);
	m_Grid.SetFixedRowCount(1);


	CString strResult;
	Json::Value myRoot;

	string szBuf = myRoot.toStyledString();
	CHttpOperation http_log(m_strServer, m_nServerPort, _T("process/getProcessList"));
	bool bRet_log = http_log.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	if ( bRet_log)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (jsReader.parse(strResult_utf8, myReturnRoot))
		{
			if(!myReturnRoot.isNull() && myReturnRoot.isObject())
			{
				Json::Value dataObj = myReturnRoot["data"];
				if(!dataObj.isNull() && dataObj.isArray())
				{
					m_Grid.SetRowCount(1+dataObj.size());
					for(UINT i=0;i<dataObj.size();i++)
					{
						Json::Value dataItem = dataObj[i];
						if(dataItem.isObject())
						{
							string strDirName = dataItem["directoryName"].asString();
							string strExeName = dataItem["processName"].asString();

							wstring wstrDirName;
							Project::Tools::UTF8ToWideChar(strDirName, wstrDirName);
							wstring wstrProcessName;
							Project::Tools::UTF8ToWideChar(strExeName, wstrProcessName);

							CString strTemp;
							strTemp.Format(_T("%d"), i+1);
							m_Grid.SetItemText(i+1,0, strTemp);
							m_Grid.SetItemText(i+1,1, wstrDirName.c_str());
							m_Grid.SetItemText(i+1,2, wstrProcessName.c_str());
							m_Grid.SetItemState(i+1,0, GVIS_READONLY);
							m_Grid.SetItemState(i+1,1, GVIS_READONLY);
							m_Grid.SetItemState(i+1,2, GVIS_READONLY);

						}
					}
				}
			}
			return;
		}
	}


	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();
}

void CCoreServiceManageDlg::OnBnClickedButtonDeleteItem()
{
	// TODO: Add your control notification handler code here
	CCellRange cr = m_Grid.GetSelectedCellRange();

	if(cr.GetMaxRow()<1)
	{
		AfxMessageBox(_T("请在左侧列表中选中有效的配置行"));
		return;
	}

	CWarningDlg WarnDlg;
	CString strTemp;

	wstring wstrIP = m_strServerIP.GetString();
	strTemp.Format(L"是否确认删除选中项?", wstrIP.c_str());
	WarnDlg.InitWarn(_T("提示"), strTemp, FALSE);
	if(WarnDlg.DoModal()!=IDOK)
	{
		return;
	}

	
	Json::Value myRoot;
	CString strResult;

	for(int i=cr.GetMinRow();i<=cr.GetMaxRow();i++)
	{
		CString strProcessName = m_Grid.GetItemText(i, 2);
		string strOneProcess = Project::Tools::WideCharToUtf8(strProcessName.GetString());
		myRoot["processList"].append(strOneProcess);
	}

	string szBuf = myRoot.toStyledString();
	CHttpOperation http_log(m_strServer, m_nServerPort, _T("process/delProcessName"));
	bool bRet_log = http_log.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	if(bRet_log)
	{
		RefreshTable();
	}
}


void CCoreServiceManageDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
