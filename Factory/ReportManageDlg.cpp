// ReportManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReportManageDlg.h"
#include "afxdialogex.h"
#include "ModifyReport.h"
#include "MainFrm.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"

#include "ReportInfo.h"
#include "SelectTimeDlg.h"

// CReportManageDlg dialog

IMPLEMENT_DYNAMIC(CReportManageDlg, CDialogEx)

CReportManageDlg::CReportManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReportManageDlg::IDD, pParent)
{

}

CReportManageDlg::~CReportManageDlg()
{
}

void CReportManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_ADD, m_btnAdd);
	DDX_Control(pDX, ID_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_REMOVE_REPORT, m_btnDelete);
	DDX_Control(pDX, IDC_LIST_REPORT, m_list_Report);

	
}


BEGIN_MESSAGE_MAP(CReportManageDlg, CDialogEx)
	
	ON_BN_CLICKED(ID_ADD, &CReportManageDlg::OnBnClickedAdd)
	ON_BN_CLICKED(ID_MODIFY, &CReportManageDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_REMOVE_REPORT, &CReportManageDlg::OnBnClickedDelete)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_REPORT, &CReportManageDlg::OnBnClickedButtonDownloadReport)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_REPORT_GEN, &CReportManageDlg::OnBnClickedButtonDownloadReportGen)
END_MESSAGE_MAP()


// CReportManageDlg message handlers


BOOL CReportManageDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect;  
	m_list_Report.GetClientRect(&rect); 

	 m_list_Report.SetExtendedStyle(m_list_Report.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  
	
	m_list_Report.InsertColumn(0,_T("�ļ�ID"),LVCFMT_CENTER, 50, 0);
	
	m_list_Report.InsertColumn(1,_T("�ļ�����"),LVCFMT_CENTER, 300, 1);
	m_list_Report.InsertColumn(2,_T("�ļ�����"),LVCFMT_CENTER, 200, 2);
	m_list_Report.InsertColumn(3,_T("�ļ�����"),LVCFMT_CENTER, 150, 3);
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	
	pMainFrame->m_pPrjSqlite->GetReportInfo(m_nidList,m_strReportNameList,m_strReportDescriptionList, m_nFileTypeList);
	
	//��ȡ������Ϣ
	for (int i=0;i<m_strReportNameList.size();i++)
	{
		CString str ;
		str.Format(_T("%d"),m_nidList[i]);
		m_list_Report.InsertItem(i,str);
		m_list_Report.SetItemText(i,1,m_strReportNameList[i].GetString());
		m_list_Report.SetItemText(i,2,m_strReportDescriptionList[i].GetString());
		if(m_nFileTypeList[i]==0)
		{
			m_list_Report.SetItemText(i,3, _T("���渽��"));
		}
		else if(m_nFileTypeList[i]==1)
		{
			m_list_Report.SetItemText(i,3, _T("ϵͳ�ļ�"));
		}
		else if(m_nFileTypeList[i]==2)
		{
			m_list_Report.SetItemText(i,3, _T("����"));
		}
	}

	return TRUE; 
} // return TRUE unless you set the focus to a control
	

void CReportManageDlg::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog dlgOpenFile(TRUE, _T(""), _T(""), 0, _T("All File (*.*)|*.*|"), NULL);
	if(dlgOpenFile.DoModal()==IDOK)
	{
		POSITION ps = dlgOpenFile.GetStartPosition();
		m_strFilePath = dlgOpenFile.GetNextPathName(ps);
		
		CString strReportName = dlgOpenFile.GetFileName();
		CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		int nid;
		nid=pMainFrame->m_pPrjSqlite->GetMaxIDInReportTable();
		CReportInfo dlg;
		dlg.m_strReportName=strReportName;
		if (dlg.DoModal()==IDOK)
		{
			
			for (int i=0;i<m_strReportNameList.size();i++)
			{
				if (dlg.m_strReportName==m_strReportNameList[i])
				{
					AfxMessageBox(_T("�ļ����ظ���"));
					return;
				}
			}
		}
		else 
		{
			return;
		}
		
			

		CTime time;
		time = CTime::GetCurrentTime();
		CString strReportTime = time.Format("%Y-%m-%d %H:%M:%S");
		//pMainFrame->m_pPrjSqlite����4db���ݿ�ָ��
		bool bSuccess = pMainFrame->m_pPrjSqlite->AddReportInfo(nid+1,strReportName,strReportTime, m_strFilePath,dlg.m_strReportDescription,dlg.m_nFileType,dlg.m_nTimeType,dlg.m_strTimeConfig);

		if(!bSuccess)
		{
			AfxMessageBox(_T("�ļ��ϴ�ʧ�ܣ�������ļ��Ƿ�ռ�û�򿪱༭�С�"));
			return;
		}

		m_strReportNameList.push_back(strReportName);
		m_nidList.push_back(nid+1);
		CString str ;
		str.Format(_T("%d"),(nid+1));
		int ncur = m_list_Report.InsertItem(m_strReportNameList.size(),str);
		m_list_Report.SetItemText(ncur,1,strReportName);
		m_list_Report.SetItemText(ncur,2,dlg.m_strReportDescription);
		
		if(dlg.m_nFileType==0)
		{
			m_list_Report.SetItemText(ncur,3, _T("���渽��"));
		}
		else if(dlg.m_nFileType==1)
		{
			m_list_Report.SetItemText(ncur,3, _T("ϵͳ�ļ�"));
		}
		else if(dlg.m_nFileType==2)
		{
			m_list_Report.SetItemText(ncur,3, _T("����"));
		}
		UpdateData(FALSE);
	}
}


void CReportManageDlg::OnBnClickedModify()
{
	// TODO: Add your control notification handler code here
	if (AfxMessageBox(L"ȷ��Ҫ�޸��ļ�������?",MB_OKCANCEL) == IDOK)
	{
		POSITION pos = m_list_Report.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			CModifyReport dlg;
			if (dlg.DoModal()==IDOK)
			{
				int nItem = m_list_Report.GetNextSelectedItem(pos);
				CString str =m_list_Report.GetItemText(nItem,0);
				int nid=_ttoi(str);
				CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
				if (dlg.m_strNewReportName==_T(""))
				{
					AfxMessageBox(_T("�ļ�������Ϊ�գ�"));
					break;
				}
				pMainFrame->m_pPrjSqlite->ModifyReportInfo(nid,dlg.m_strNewReportName);
				m_list_Report.SetItemText(nItem,1,dlg.m_strNewReportName);
			}
			else
				return;
			
			

		}

	}
	
}


void CReportManageDlg::OnBnClickedDelete()
{
	// TODO: Add your control notification handler code here
	if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
	{
		POSITION pos = m_list_Report.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			AfxMessageBox(_T("No items were selected!\n"));
			return;
		}
		while(pos)
		{
			int nItem = m_list_Report.GetNextSelectedItem(pos);
			CString str =m_list_Report.GetItemText(nItem,0);
			CString strName = m_list_Report.GetItemText(nItem,1);
			int nid=_ttoi(str);
			CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
			pMainFrame->m_pPrjSqlite->DeleteReportInfo(nid);
			m_list_Report.DeleteItem(nItem);

			for(int i=0;i<m_strReportNameList.size();i++)
			{
				if(m_strReportNameList[i]==strName )
				{
					m_strReportNameList.erase(m_strReportNameList.begin()+i);
					i--;
				}
			}
		
		}
		
	}
	

}

void CReportManageDlg::OnClose()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnDestroy();
}


void CReportManageDlg::OnBnClickedButtonDownloadReport()
{
	// TODO: Add your control notification handler code here

	POSITION pos = m_list_Report.GetFirstSelectedItemPosition();
	int nItem = m_list_Report.GetNextSelectedItem(pos);
	CString strTemplateName =m_list_Report.GetItemText(nItem,1);

	int nDot = strTemplateName.ReverseFind('.');
	CString strFileType = strTemplateName.Mid(nDot+1);
	CString strFileTypeSelection = _T("*.") + strFileType;

	CString strResult;
	CHttpOperation http(m_strServer, m_nServerPort, _T("downloadReport"));

	CFileDialog dlg(FALSE, _T(""), strTemplateName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, strFileTypeSelection.GetString());
	//dlg.m_ofn.lpstrTitle = _T("�����ļ�");
	
	
	CString strNewFilePath;
	if ( IDOK == dlg.DoModal() )
	{	
		 strNewFilePath=  dlg.GetPathName()	;
	}
	else
	{

			return ;
	}
	Json::Value myRoot;
	string strTemplateName_utf8 = Project::Tools::WideCharToAnsi(strTemplateName.GetString());
	string strNewFilePath_utf8 = Project::Tools::WideCharToAnsi(strNewFilePath.GetString());
	
	myRoot["templateName"]=strTemplateName_utf8;
	myRoot["strReportFilePath"]=strNewFilePath_utf8;
	string szBuf = myRoot.toStyledString();
	bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	if (bRet)
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
				int bsucess =myData.isNull();
				if (bsucess)
				{
					AfxMessageBox(_T("���سɹ���"));
				}
				else
				{
					AfxMessageBox(_T("����ʧ�ܣ�"));
				}

				// wstring wstrLog = Project::Tools::AnsiToWideChar(myData);

				UpdateData(FALSE);


			}

		}
	}
	else
	{
		AfxMessageBox(_T("����ʧ�ܣ�����dompysite�Ƿ����л���4db�ļ��Ƿ���Ч"));
	}

	
	
	
}




void CReportManageDlg::OnBnClickedButtonDownloadReportGen()
{
	CSelectTimeDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	COleDateTime tGen = dlg.m_tSelect;
	CString strGenTime = tGen.Format(_T("%Y-%m-%d %H:%M:%S"));

	// TODO: Add your control notification handler code here
	POSITION pos = m_list_Report.GetFirstSelectedItemPosition();
	int nItem = m_list_Report.GetNextSelectedItem(pos);
	CString strTemplateName =m_list_Report.GetItemText(nItem,1);

	int nDot = strTemplateName.ReverseFind('.');
	CString strFileType = strTemplateName.Mid(nDot+1);

	CString strURL = _T("");
	if(strFileType==L"xlsx")
	{
		strURL = _T("report/genExcelReportByTemplate");
	}
	else if(strFileType==_T("docx"))
	{
		strURL = _T("report/genWordReportByTemplate");
	}
	else
	{
		AfxMessageBox(_T("���ļ����Ǳ����ʽ���������ɱ���"));
		return;

	}

	CString strFileTypeSelection = _T("*.")+ strFileType;


	CString strResult;
	CHttpOperation http(m_strServer, m_nServerPort,strURL );

	Json::Value myRoot;
	string strTemplateName_utf8 = Project::Tools::WideCharToAnsi(strTemplateName.GetString());

	myRoot["templateName"]=strTemplateName_utf8;
	myRoot["record"] = 1;
	myRoot["actTime"] = Project::Tools::WideCharToUtf8(strGenTime.GetString());
	string szBuf = myRoot.toStyledString();
	bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	if (bRet)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_utf8, myReturnRoot))
		{
			AfxMessageBox(_T("��������ʧ�ܣ���̨����������Ϣ��"));
			return ;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value myErrData =  myReturnRoot["err"];
				wstring wstrLog ;
				bool bsucess =(myErrData.isInt() && myErrData.asInt()==0);
				if (bsucess)
				{
					Json::Value myData =  myReturnRoot["data"];
					if(myData.isNull())
					{
						AfxMessageBox(_T("��������ʧ�ܣ�"));
						return;
					}

					string strGenFileName = myData.asString();
					wstring wstrGenFileName;
					CString cstrGenFileName;
					Project::Tools::UTF8ToWideChar(strGenFileName, wstrGenFileName);
					cstrGenFileName = wstrGenFileName.c_str();
					CFileDialog dlg(FALSE, _T(""), cstrGenFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, strFileTypeSelection.GetString());
					//dlg.m_ofn.lpstrTitle = _T("�����ļ�");


					CString strNewFilePath;
					if ( IDOK == dlg.DoModal() )
					{	

						CString strNewFilePath=  dlg.GetPathName()	;
						
						CHttpOperation httpFile(m_strServer, m_nServerPort,_T("static/files/")+cstrGenFileName);
						bool bRet = httpFile.HttpGetFile(strNewFilePath);
						if (bRet)
						{
							//string str_utf8;
							//Project::Tools::WideCharToUtf8(strNewFilePath.GetString(), str_utf8);
							try{
								//LPCSTR lpstr = (LPCSTR)str_utf8.data();
								::ShellExecute(NULL,L"open",strNewFilePath,NULL,NULL,SW_SHOWNORMAL);
							}
							catch(CException *e)
							{
								int xx=0;
							}
							
						}
						else
						{
							AfxMessageBox(_T("�������ɳɹ�����������ʧ�ܣ�"));
						}
					}
					else
					{

						return ;
					}
				}
				else
				{
					AfxMessageBox(_T("��������ʧ�ܣ�"));
				}

				// wstring wstrLog = Project::Tools::AnsiToWideChar(myData);

				UpdateData(FALSE);


			}

		}
	}
	else
	{
		AfxMessageBox(_T("�������ɺ�ʱ�ϳ��������ĵȴ���"));
	}


}
