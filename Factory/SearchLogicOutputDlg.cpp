// Search.cpp : implementation file
//

#include "stdafx.h"
#include "SearchLogicOutputDlg.h"
#include "afxdialogex.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"

#include "MainFrm.h"
#include "DataComConfigDlg.h"
#include <regex>
#include "Tools/DirectoryOperations.h"

// CSearchLogicOutputDlg dialog

IMPLEMENT_DYNAMIC(CSearchLogicOutputDlg, CDialogEx)

CSearchLogicOutputDlg::CSearchLogicOutputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchLogicOutputDlg::IDD, pParent)

	, m_strResultLog(_T(""))
{

}

CSearchLogicOutputDlg::~CSearchLogicOutputDlg()
{
}

void CSearchLogicOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_THREADLIST, m_ThreadList);


	DDX_Text(pDX, IDC_POINTNAME, m_strPointName);
	//DDX_Text(pDX, IDC_EDIT_DISPLAY_SEARCH_RESULT, m_strResultLog);
	DDX_Control(pDX, IDC_LIST_LOG, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CSearchLogicOutputDlg, CDialogEx)
	
	ON_CBN_SELCHANGE(IDC_COMBO_THREADLIST, &CSearchLogicOutputDlg::OnCbnSelchangeComboThreadlist)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearchLogicOutputDlg::OnBnClickedButtonSearch)
	
	ON_EN_CHANGE(IDC_EDIT_DISPLAY_SEARCH_RESULT, &CSearchLogicOutputDlg::OnEnChangeEditReplaceFrom)
	ON_BN_CLICKED(IDOK, &CSearchLogicOutputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_POINT, &CSearchLogicOutputDlg::OnBnClickedButtonSelectPoint)
	ON_BN_CLICKED(IDC_BUTTON_GO_STRATEGY, &CSearchLogicOutputDlg::OnBnClickedButtonGoStrategy)
	ON_WM_CLOSE()
END_MESSAGE_MAP()




// CSearchLogicOutputDlg message handlers




BOOL CSearchLogicOutputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	
	for (int i=0;i<m_pLogicManager->m_vecDllThreadList.size();i++)
	{
		CString str( m_pLogicManager->m_vecDllThreadList[i]->GetName().c_str() );;

		m_ThreadList.AddString(str);
	}
	
	//�ı�һ����ʽ
	m_listCtrl.SetBkColor(RGB(235, 235, 235));
	m_listCtrl.SetTextColor(RGB(0, 0, 128));
	m_listCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	//������
	m_listCtrl.InsertColumn(0, L"��־", LVCFMT_LEFT);
	m_listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	return true;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CSearchLogicOutputDlg::OnCbnSelchangeComboThreadlist()
{
	// TODO: Add your control notification handler code here
}

void CSearchLogicOutputDlg::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if(m_strPointName.GetLength() == 0)
		return;

	m_listCtrl.DeleteAllItems();
	CString strPointName = m_strPointName;

	CString strResult;
	CHttpOperation http(m_strServer, m_nServerPort, _T("logic/search"));
	Json::Value myRoot;
	int nIndex = m_ThreadList.GetCurSel();
	CString strThreadName;
	if (nIndex!=-1)
	{
		m_ThreadList.GetLBText( nIndex, strThreadName);
		
	}
	else
	{
		strThreadName=_T("");
	}
	
	string strPointName_utf8 = Project::Tools::WideCharToUtf8(strPointName.GetString());
	string strThreadName_utf8 = Project::Tools::WideCharToUtf8(strThreadName.GetString());
	myRoot["pointname"]=strPointName_utf8;
	myRoot["threadname"]=strThreadName_utf8;
	string szBuf = myRoot.toStyledString();
	bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	if(bRet)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_utf8, myReturnRoot))
		{
			return ;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value myData =  myReturnRoot["data"];
				wstring wstrLog ;
				Project::Tools::UTF8ToWideChar(myData.asString(), wstrLog);
				m_strResultLog = wstrLog.c_str();		
				vector<CString>loglist;
				Project::Tools::SplitStringByChar(m_strResultLog, L'\n', loglist);
				for(int i=0; i<loglist.size();i++)
				{
					m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), loglist[i]);
				}
				// wstring wstrLog = Project::Tools::AnsiToWideChar(myData);				
				UpdateData(FALSE);
			}
		}
	}
	else
	{
		AfxMessageBox(L"����ʧ�ܣ�dompysite����δ������");
		return;//�˳�ԭ����Ҫ���ϰ汾���������Ҳ������
	}

	//Version: 2.5.00��֧�ֶ��ļ��ڵ�log����
	CString cstrALLLogsPath;
	Project::Tools::GetCoreLogPath(cstrALLLogsPath);
	vector<CString> cstrLogList;
	/*
	DirectoryOperations::GetAllFilePathList(cstrALLLogsPath,cstrLogList);
	//D:\dom\Factory_Code\Factory\trunk\bin\core\log	[1,2,3,4]�ļ���

	//ɾ���б������з�oprecord
	regex reg("^.*oprecord-\\d\\d\\d\\d-\\d\\d-\\d\\d$");
	vector<CString>::iterator nCount = cstrLogList.begin();
	for(;nCount!=cstrLogList.end();)
	{
		string strPath;
		Project::Tools::WideCharToUtf8(nCount->GetString(), strPath);
		bool ret = regex_match(strPath, reg);
		if(ret == false)
		{
			nCount = cstrLogList.erase(nCount);
		}
		else
		{
			nCount++;
		}
	}
	*/
	//ɸѡ�������ļ���
	COleDateTime t(COleDateTime::GetCurrentTime());
	COleDateTimeSpan oldDay(1,0,0,0);

	cstrLogList.push_back(cstrALLLogsPath + L"\\" + t.Format(L"oprecord-%Y-%m-%d"));
	t = t - oldDay;
	cstrLogList.push_back(cstrALLLogsPath + L"\\" + t.Format(L"oprecord-%Y-%m-%d"));
	t = t - oldDay;
	cstrLogList.push_back(cstrALLLogsPath + L"\\" + t.Format(L"oprecord-%Y-%m-%d"));
	
	//��ÿ��oprecord�ļ����еĵ����ļ������������ݷ��õ�m_strResultLog�༭���С�
	vector<CString>::reverse_iterator iter_cstrLog = cstrLogList.rbegin();
	for(;iter_cstrLog!=cstrLogList.rend();iter_cstrLog++)//�������е�oprecord�ļ���
	{
		CStdioFile fileControl;

		if(fileControl.Open(*iter_cstrLog +L"\\" + m_strPointName + L".txt", 
			CFile::modeRead | CFile::typeText | CFile::shareDenyNone | CFile::shareCompat ) == TRUE)
		{
			CString cstrFileLine;
			while(fileControl.ReadString(cstrFileLine))
			{
				//m_strResultLog = m_strResultLog + cstrFileLine + L"\r\n";//�������û�л��з�
				m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), cstrFileLine);
			}
			fileControl.Close();
		}
	}//end for

	UpdateData(FALSE);
}



void CSearchLogicOutputDlg::OnEnChangeEditReplaceFrom()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CSearchLogicOutputDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	CDialogEx::OnOK();
}


void CSearchLogicOutputDlg::OnBnClickedButtonSelectPoint()
{
	// TODO: Add your control notification handler code here

	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	CBEOPDesignerView *pView = static_cast<CBEOPDesignerView*>(pFrame->GetActiveView());

	CDataPointManager& point_manager = pFrame->m_plcpointmap;
	map<int, DataPointEntry>& datalist = point_manager.GetPointMap();

	CDataComConfigDlg dlg(point_manager);

	if(dlg.DoModal()==IDOK)
	{
		m_strPointName = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

#include "MainFrm.h"

void CSearchLogicOutputDlg::OnBnClickedButtonGoStrategy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;

	int nItem = m_listCtrl.GetNextSelectedItem(pos);
	CString cstr = m_listCtrl.GetItemText(nItem,0);

	cstr = cstr.Mid(cstr.Find(L"[")+1);//Thread:0_thread1][logic:testIF.dll]	1.000
	cstr = cstr.Mid(cstr.Find(L':')+1);
	cstr = cstr.Left(cstr.Find(L']'));

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame == NULL)
		return;

	HTREEITEM selectH = pMainFrame->m_wndFileView.FinItem_m_wndFileView(pMainFrame->m_wndFileView.GetTactHTI(), cstr);
	if(selectH  == NULL)
	{
		AfxMessageBox(L"��ǰ����û���ҵ�");
		return;
	}
	pMainFrame->m_wndFileView.m_wndFileView.SelectItem(selectH);
	LRESULT temp;
	pMainFrame->m_wndFileView.OnNMDblclkTree(NULL, &temp);
}


void CSearchLogicOutputDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	delete this;
	/*CDialogEx::OnClose();*/
}
