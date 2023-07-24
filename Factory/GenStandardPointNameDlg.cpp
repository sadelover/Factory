// GenStandardPointNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GenStandardPointNameDlg.h"
#include "afxdialogex.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"

#include "HttpOperation.h"
#include "MainFrm.h"


// CGenStandardPointNameDlg dialog

IMPLEMENT_DYNAMIC(CGenStandardPointNameDlg, CDialogEx)

CGenStandardPointNameDlg::CGenStandardPointNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGenStandardPointNameDlg::IDD, pParent)
	, m_nRows(100)
{

}

CGenStandardPointNameDlg::~CGenStandardPointNameDlg()
{
}

void CGenStandardPointNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CUSTOM_DATATABLE, m_Grid);
	DDX_Text(pDX, IDC_EDIT_ROWS, m_nRows);
}


BEGIN_MESSAGE_MAP(CGenStandardPointNameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GEN, &CGenStandardPointNameDlg::OnBnClickedButtonGen)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CGenStandardPointNameDlg::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CGenStandardPointNameDlg message handlers


void CGenStandardPointNameDlg::OnBnClickedButtonGen()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(pFrame->m_strCalcIp, pFrame->m_nCalcPort, _T("pointtool/transferPointName"));

	wstring wstrTimeStart,wstrTimeEnd ;
	
	Json::Value myRoot;
	Json::Value nameData;
	Json::Value desData;
	for(int i=1;i<m_Grid.GetRowCount();i++)
	{

		string strTemp_utf8;

		CString strTemp = m_Grid.GetItemText(i, 0);
		Project::Tools::WideCharToUtf8(strTemp.GetString(), strTemp_utf8);
		nameData.append(strTemp_utf8);

		strTemp = m_Grid.GetItemText(i, 1);
		Project::Tools::WideCharToUtf8(strTemp.GetString(), strTemp_utf8);
		desData.append(strTemp_utf8);

	}



	myRoot["descriptionList"] = desData;
	myRoot["nameList"] = nameData;

	
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
			if(myReturnRoot.isObject())
			{
				Json::Value dataObj = myReturnRoot["data"];

				if(dataObj.isArray())
				{
				

					for(int tt=0;tt<dataObj.size();tt++)
					{
						string strPointName = dataObj[Json::UInt(tt)].asString();
						CString strTemp;
						wstring wstrPointName;
						Project::Tools::UTF8ToWideChar(strPointName, wstrPointName);
						strTemp = wstrPointName.c_str();

						m_Grid.AutoSize(GVS_BOTH);
						m_Grid.Refresh();
						
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


void CGenStandardPointNameDlg::OnBnClickedButtonQuery()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_Grid.SetRowCount(m_nRows+1);
}


BOOL CGenStandardPointNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Grid.DeleteAllItems();
	m_Grid.SetColumnCount(3);

	m_Grid.SetRowCount(101);
	m_Grid.SetItemText(0,0, _T("     英文点串     "));
	m_Grid.SetItemText(0,1, _T("     点注释     "));
	m_Grid.SetItemText(0,2, _T("自动转译生成的标准点名"));
	//m_Grid.SetDefCellHeight(16);

	m_Grid.SetDefCellMargin(2);
	m_Grid.SetFixedRowCount(1);
	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
