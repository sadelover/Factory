// OpRecordHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpRecordHistoryDlg.h"
#include "afxdialogex.h"

#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"

// COpRecordHistoryDlg dialog

IMPLEMENT_DYNAMIC(COpRecordHistoryDlg, CDialogEx)

COpRecordHistoryDlg::COpRecordHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpRecordHistoryDlg::IDD, pParent)
{
	m_DateFrom = COleDateTime::GetCurrentTime();
	m_DateTo = m_DateFrom;
}

COpRecordHistoryDlg::~COpRecordHistoryDlg()
{
}

void COpRecordHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_DATATABLE, m_Grid);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_DateFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_DateTo);
}


BEGIN_MESSAGE_MAP(COpRecordHistoryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &COpRecordHistoryDlg::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// COpRecordHistoryDlg message handlers


void COpRecordHistoryDlg::OnBnClickedButtonQuery()
{
	// TODO: Add your control notification handler code here


	UpdateData(TRUE);

	COleDateTimeSpan tspan = COleDateTimeSpan(0,0,1,0);
	COleDateTime tStart(m_DateFrom.GetYear(), m_DateFrom.GetMonth(),m_DateFrom.GetDay(),
		0, 0, 0);
	COleDateTime tEnd(m_DateTo.GetYear(), m_DateTo.GetMonth(),m_DateTo.GetDay(),
		23, 59, 59);

	m_tStart = tStart;
	m_tEnd = tEnd;


	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strServer, m_nServerPort, _T("get_operation_log"));

	wstring wstrTimeStart,wstrTimeEnd ;
	Project::Tools::OleTime_To_String(m_tStart, wstrTimeStart );
	Project::Tools::OleTime_To_String(m_tEnd, wstrTimeEnd );

	Json::Value myRoot;


	m_Grid.DeleteAllItems();
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(3);
	m_Grid.SetItemText(0,0, _T("时间"));
	m_Grid.SetItemText(0,1, _T("操作用户或来源"));
	m_Grid.SetItemText(0,2, _T("操作内容"));

	m_Grid.SetDefCellMargin(2);
	m_Grid.SetFixedRowCount(1);


	myRoot["dateFrom"] = Project::Tools::WideCharToUtf8(wstrTimeStart.substr(0,10).c_str());
	myRoot["dateTo"] = Project::Tools::WideCharToUtf8(wstrTimeEnd.substr(0,10).c_str());


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
			return;
		}
		else
		{
			if(myReturnRoot.isArray())
			{
					m_Grid.SetRowCount(1+myReturnRoot.size());
					for(int tt=0;tt<myReturnRoot.size();tt++)
					{
						Json::Value recordArray = myReturnRoot[Json::UInt(tt)];

						for(int i=0;i<recordArray.size();i++)
						{
							

								string strVV =  recordArray[i].asString();
								wstring wstrVV;
								Project::Tools::UTF8ToWideChar(strVV, wstrVV);
								m_Grid.SetItemText(tt+1, i, wstrVV.c_str());
							
						}
					}




				
			}
		}
	}
	else
	{
		return;
	}



	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();
}
