
// PointHisdataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PointHisdataDlg.h"
#include "afxdialogex.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"

#include "ModifyHisData.h"
#include "RemoveHisData.h"
// CPointHisdataDlg dialog
#include "InputTextDlg.h"
#include "InputConditionDlg.h"

IMPLEMENT_DYNAMIC(CPointHisdataDlg, CDialogEx)

CPointHisdataDlg::CPointHisdataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointHisdataDlg::IDD, pParent)
	, m_DateTo(COleDateTime::GetCurrentTime())
	, m_nTimePeriod(0)
{
	m_strServer = _T("");
	m_nServerPort = 5000;
	m_DateFrom = COleDateTime::GetCurrentTime();
	m_DateTo = m_DateFrom;
	m_TimeFrom = m_DateFrom;
	m_TimeTo = m_DateTo;

}

CPointHisdataDlg::~CPointHisdataDlg()
{
}

void CPointHisdataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CUSTOM_DATATABLE, m_Grid);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_DateFrom);
	DDX_DateTimeCtrl(pDX, IDC_TIME_START, m_TimeFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_DateTo);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_TimeTo);
	DDX_CBIndex(pDX, IDC_COMBO_STEP, m_nTimePeriod);
	DDX_Control(pDX, IDC_LIST_POINTNAME, m_lstPointName);
}


BEGIN_MESSAGE_MAP(CPointHisdataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CPointHisdataDlg::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CPointHisdataDlg::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CPointHisdataDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CPointHisdataDlg::OnBnClickedButtonRemove)
	ON_CBN_SELCHANGE(IDC_COMBO_STEP, &CPointHisdataDlg::OnCbnSelchangeComboStep)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_IN_RANGE, &CPointHisdataDlg::OnBnClickedButtonRemoveInRange)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_FROM_TEXT, &CPointHisdataDlg::OnBnClickedButtonImportFromText)
END_MESSAGE_MAP()


// CPointHisdataDlg message handlers


void CPointHisdataDlg::OnBnClickedButtonQuery()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	COleDateTimeSpan tspan = COleDateTimeSpan(0,0,1,0);
	COleDateTime tStart(m_DateFrom.GetYear(), m_DateFrom.GetMonth(),m_DateFrom.GetDay(),
		m_TimeFrom.GetHour(), m_TimeFrom.GetMinute(), 0);
	COleDateTime tEnd(m_DateTo.GetYear(), m_DateTo.GetMonth(),m_DateTo.GetDay(),
		m_TimeTo.GetHour(), m_TimeTo.GetMinute(), 0);

	m_tStart = tStart;
	m_tEnd = tEnd;


	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strServer, m_nServerPort, _T("get_history_data_padded"));

	wstring wstrTimeStart,wstrTimeEnd ;
	Project::Tools::OleTime_To_String(m_tStart, wstrTimeStart );
	Project::Tools::OleTime_To_String(m_tEnd, wstrTimeEnd );

	Json::Value myRoot;


	m_Grid.DeleteAllItems();
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(m_strPointNameList.size()+1);
	m_Grid.SetItemText(0,0, _T("Time"));

	m_Grid.SetDefCellMargin(2);
	m_Grid.SetFixedRowCount(1);

	for(int i=0;i<m_strPointNameList.size();i++)
	{
		myRoot["pointList"].append(Project::Tools::WideCharToUtf8(m_strPointNameList[i].GetString()));

		m_Grid.SetItemText(0,i+1, m_strPointNameList[i]);
	}
	
	myRoot["timeStart"] = Project::Tools::WideCharToUtf8(wstrTimeStart.c_str());
	myRoot["timeEnd"] = Project::Tools::WideCharToUtf8(wstrTimeEnd.c_str());

	string strPeriod = "m1";
	

	myRoot["timeFormat"] = "m1";
	if(m_nTimePeriod==1)
		myRoot["timeFormat"] = "m5";
	else if(m_nTimePeriod==2){
		myRoot["timeFormat"] = "h1";
	}else if(m_nTimePeriod==3){
		myRoot["timeFormat"] = "d1";
	}
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
				Json::Value dataObj = myReturnRoot["map"];

				Json::Value timeObj = myReturnRoot["time"];
				m_Grid.SetRowCount(1+ timeObj.size());
				if(dataObj.isObject())
				{


					for(int tt=0;tt<timeObj.size();tt++)
					{
						Json::Value strTime = timeObj[Json::UInt(tt)];
						wstring wstrTime ;
						Project::Tools::UTF8ToWideChar(strTime.asString(), wstrTime);
						m_Grid.SetItemText(tt+1, 0, wstrTime.c_str());
						for(int i=0;i<m_strPointNameList.size();i++)
						{
							string strPointName_utf8 = Project::Tools::WideCharToUtf8(m_strPointNameList[i].GetString());
							Json::Value myPointData = dataObj[strPointName_utf8];
							if(myPointData.isArray() && myPointData.size()>tt)
							{
								Json::Value pv = myPointData.get(Json::Value::UInt(tt), Json::Value(""));

								if(pv.isDouble())
								{
									CString wstrVV;
									wstrVV.Format(_T("%.3f"), pv.asDouble());
									m_Grid.SetItemText(tt+1, i+1, wstrVV);
								}
								else if(pv.isString())
								{
									string strVV =  pv.asString();
									wstring wstrVV;
									Project::Tools::UTF8ToWideChar(strVV, wstrVV);
									m_Grid.SetItemText(tt+1, i+1, wstrVV.c_str());
								}
								
							}

							m_Grid.SetItemState(tt+1,i+1, GVIS_READONLY);
						}
					}


					
					
				}
			}
		}
	}
	else
	{
		AfxMessageBox(_T("后台请求失败，请检查网络或后台"));
		return;
	}
	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();
}


void CPointHisdataDlg::OnBnClickedButtonDownload()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|")_T("All Files (*.*)|*.*||"), NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		if (strName.GetLength() == 0)
		{
			AfxMessageBox(_T("文件名称不能为空"));
			return;
		}
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
		setlocale( LC_ALL, "chs" );

		CStdioFile filehandle;
		CString strTemp;
		if(filehandle.Open(strFilePath, CFile::modeCreate|CFile::modeWrite))
		{
			int nRow = m_Grid.GetRowCount();
			int nColumn = m_Grid.GetColumnCount();
			for(int i=0;i<nRow;i++)
			{
				CString strOneRow;
				for(int j=0;j<nColumn;j++)
				{
					strTemp = m_Grid.GetItemText(i, j);
					strOneRow+=strTemp;
					if(j==(nColumn-1))
					{
						strOneRow+=_T("\n");
					}
					else
					{
						strOneRow+=_T(",");
					}
				}
				
				
				filehandle.WriteString(strOneRow);

			}

			
			
			filehandle.Close();
			setlocale( LC_ALL, old_locale );
			AfxMessageBox(_T("导出CSV文件成功"));
		}
		else
		{
			setlocale( LC_ALL, old_locale );
			AfxMessageBox(_T("导出CSV文件失败"));
		}	
	}
}


void CPointHisdataDlg::OnBnClickedButtonModify()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nId = -1;
	CString strPointTime;
	CString strPointName;
	string strTimeAt_Utf8;
	string strPointName_Utf8;
	string strPointValue_Utf8;
	
	
	CCellRange myRange = m_Grid.GetSelectedCellRange();
	m_nRowIndexMin = myRange.GetMinRow();
	m_nRowIndexMax=  myRange.GetMaxRow();
	m_nColumnMin = myRange.GetMinCol();
	m_nColumnMax = myRange.GetMaxCol();

	if(m_nRowIndexMin<0 || m_nRowIndexMax<0 || m_nColumnMax<0 || m_nColumnMin<0)
	{

		AfxMessageBox(_T("请选中需要修改的有效数据单元格"));
	}

	if(m_nRowIndexMax==0)
	{

		AfxMessageBox(_T("请选中需要修改的有效数据单元格"));
	}

	CModifyHisData dlg;
	dlg.m_strValue = _T("");
	if(dlg.DoModal()==IDOK)
	{
		//
		CString strNewValue=  dlg.m_strValue;
		Json::Value requestModifyList;
		for(int i=m_nRowIndexMin;i<=m_nRowIndexMax;i++)
		{
			Json::Value oneModifytItem;
			strPointTime=m_Grid.GetItemText(i,0);

			for(int j=m_nColumnMin;j<=m_nColumnMax;j++)
			{
				if(j==0)
					continue;
				strPointName=m_Grid.GetItemText(0,j);

				Project::Tools::WideCharToUtf8(strPointTime.GetString(), strTimeAt_Utf8);
				Project::Tools::WideCharToUtf8(strPointName.GetString(), strPointName_Utf8);
				Project::Tools::WideCharToUtf8(strNewValue.GetString(), strPointValue_Utf8);
				oneModifytItem["pointname"]=strPointName_Utf8;
				oneModifytItem["pointtime"]=strTimeAt_Utf8;
				oneModifytItem["pointvalue"]=strPointValue_Utf8;
				requestModifyList.append(oneModifytItem);
			}
			
			
		}
		

		if(!SaveHistoryData(requestModifyList,strNewValue))
			{
				AfxMessageBox(_T("保存失败"));
		    }
		else
		   {
			   CString strTemp;
			   strTemp.Format(_T("保存成功%d个历史数据"), requestModifyList.size());
			    AfxMessageBox(strTemp);
		    }
	}
}


void CPointHisdataDlg::OnBnClickedButtonRemove()
{
	// TODO: Add your control notification handler code here
	//if (m_nTimePeriod!=0)
	//{
	//	AfxMessageBox(_T("查询间隔为1分钟时允许删除"));
	//	return;
	//}
	UpdateData(TRUE);
	
	CString strPointTime;
	CString strPointName;
	string strTimeAt_Utf8;
	string strPointName_Utf8;
	bool beDelete=false;	
	
	CCellRange myRange = m_Grid.GetSelectedCellRange();
	m_nRowIndexMin = myRange.GetMinRow();
	m_nRowIndexMax=  myRange.GetMaxRow();
	m_nColumnMin = myRange.GetMinCol();
	m_nColumnMax = myRange.GetMaxCol();

	if(m_nRowIndexMin<0 || m_nRowIndexMax<0 || m_nColumnMax<0 || m_nColumnMin<0)
	{

		AfxMessageBox(_T("请选中需要删除的有效数据单元格"));
	}

	if(m_nRowIndexMax==0)
	{

		AfxMessageBox(_T("请选中需要删除的有效数据单元格"));
	}
	
	CRemoveHisData dlg;
	if(dlg.DoModal()==IDOK)
	{
		
		Json::Value requestRemoveList;
		for(int i=m_nRowIndexMin;i<=m_nRowIndexMax;i++)
		{
			Json::Value oneRemoveItem;
			
			strPointTime=m_Grid.GetItemText(i,0);

			for(int j=m_nColumnMin;j<=m_nColumnMax;j++)
			{
				if(j==0)
					continue;

				strPointName=m_Grid.GetItemText(0,j);
				Project::Tools::WideCharToUtf8(strPointTime.GetString(), strTimeAt_Utf8);
				Project::Tools::WideCharToUtf8(strPointName.GetString(), strPointName_Utf8);

				oneRemoveItem["pointname"]=strPointName_Utf8;
				oneRemoveItem["pointtime"]=strTimeAt_Utf8;

				requestRemoveList.append(oneRemoveItem);
			}

			
		}


			Json::Value myRoot;
			CHttpOperation http(m_strServer, m_nServerPort, _T("hisdata/remove_history_data"));
			myRoot["removeList"] = requestRemoveList;
			string szBuf = myRoot.toStyledString();
			bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), m_strResult,  _T("Content-Type: application/json"));
			if(bRet)
			{
				Json::Reader jsReader;
				string strResult_utf8;
				Project::Tools::WideCharToUtf8(m_strResult.GetString(), strResult_utf8);
				Json::Value myReturnRoot;
				if (!jsReader.parse(strResult_utf8, myReturnRoot))
				{
					return ;
				}
				else
				{
					if(myReturnRoot.isObject())
					{
						Json::Value myData =  myReturnRoot["err"];
						int nbeErr = myData.asInt();
						if (nbeErr==0)
						{
							for (int i=m_nRowIndexMin;i<=m_nRowIndexMax;i++)
							{
								 beDelete=m_Grid.DeleteRow(m_nRowIndexMin);
								m_Grid.Refresh();
							}
							if (beDelete)
							{
								AfxMessageBox(_T("删除成功"));
							}
							
						}
						else
						{
							AfxMessageBox(_T("删除失败"));
						}	
					}
			     }
		     }
	  
	}
	
}

bool CPointHisdataDlg::SaveHistoryData(Json::Value requestModifyList,CString strNewValue)
{
	    Json::Value myRoot;
	    CHttpOperation http(m_strServer, m_nServerPort, _T("hisdata/modify_history_data"));
		myRoot["modifyList"] = requestModifyList;
		string szBuf = myRoot.toStyledString();
		bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), m_strResult,  _T("Content-Type: application/json"));
		if(bRet)
		{
		
			Json::Reader jsReader;
			string strResult_utf8;
			Project::Tools::WideCharToUtf8(m_strResult.GetString(), strResult_utf8);
			Json::Value myReturnRoot;
			if (!jsReader.parse(strResult_utf8, myReturnRoot))
			{
				return true;
			}
			else
			{
				if(myReturnRoot.isObject())
				{
					Json::Value myData =  myReturnRoot["err"];
					int nbeErr = myData.asInt();
					if (nbeErr==0)
					{
						for (int i=m_nRowIndexMin;i<=m_nRowIndexMax;i++)
						{
							for(int j=m_nColumnMin;j<=m_nColumnMax;j++)
							{
								if(j==0)
									continue;

								m_Grid.SetItemText(i,j,strNewValue);
								m_Grid.Refresh();
							}
							
						}
						
						return true;
					}
					else
					{
						return false;
					}
					
				}
			}
		}
		
	
}
void CPointHisdataDlg::DisplayAllData()
{
	
	COleDateTimeSpan tspan = COleDateTimeSpan(0,0,1,0);
	COleDateTime tStart(m_DateFrom.GetYear(), m_DateFrom.GetMonth(),m_DateFrom.GetDay(),
		m_TimeFrom.GetHour(), m_TimeFrom.GetMinute(), 0);
	COleDateTime tEnd(m_DateTo.GetYear(), m_DateTo.GetMonth(),m_DateTo.GetDay(),
		m_TimeTo.GetHour(), m_TimeTo.GetMinute(), 0);

	m_tStart = tStart;
	m_tEnd = tEnd;


	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strServer, m_nServerPort, _T("get_history_data_padded"));

	wstring wstrTimeStart,wstrTimeEnd ;
	Project::Tools::OleTime_To_String(m_tStart, wstrTimeStart );
	Project::Tools::OleTime_To_String(m_tEnd, wstrTimeEnd );

	Json::Value myRoot;


	m_Grid.DeleteAllItems();
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(m_strPointNameList.size()+1);
	m_Grid.SetItemText(0,0, _T("Time"));

	m_Grid.SetDefCellMargin(2);
	m_Grid.SetFixedRowCount(1);

	for(int i=0;i<m_strPointNameList.size();i++)
	{
		myRoot["pointList"].append(Project::Tools::WideCharToUtf8(m_strPointNameList[i].GetString()));

		m_Grid.SetItemText(0,i+1, m_strPointNameList[i]);
	}

	myRoot["timeStart"] = Project::Tools::WideCharToUtf8(wstrTimeStart.c_str());
	myRoot["timeEnd"] = Project::Tools::WideCharToUtf8(wstrTimeEnd.c_str());

	string strPeriod = "m1";


	myRoot["timeFormat"] = "m1";
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
				Json::Value dataObj = myReturnRoot["map"];

				Json::Value timeObj = myReturnRoot["time"];
				m_Grid.SetRowCount(1+ timeObj.size());
				if(dataObj.isObject())
				{


					for(int tt=0;tt<timeObj.size();tt++)
					{
						Json::Value strTime = timeObj[Json::UInt(tt)];
						wstring wstrTime ;
						Project::Tools::UTF8ToWideChar(strTime.asString(), wstrTime);
						m_Grid.SetItemText(tt+1, 0, wstrTime.c_str());
						for(int i=0;i<m_strPointNameList.size();i++)
						{
							string strPointName_utf8 = Project::Tools::WideCharToUtf8(m_strPointNameList[i].GetString());
							Json::Value myPointData = dataObj[strPointName_utf8];
							if(myPointData.isArray() && myPointData.size()>tt)
							{
								Json::Value pv = myPointData.get(Json::Value::UInt(tt), Json::Value(""));

								if(pv.isDouble())
								{
									CString wstrVV;
									wstrVV.Format(_T("%.3f"), pv.asDouble());
									m_Grid.SetItemText(tt+1, i+1, wstrVV);
								}
								else if(pv.isString())
								{
									string strVV =  pv.asString();
									wstring wstrVV;
									Project::Tools::UTF8ToWideChar(strVV, wstrVV);
									m_Grid.SetItemText(tt+1, i+1, wstrVV.c_str());
								}

							}
						}
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


void CPointHisdataDlg::OnCbnSelchangeComboStep()
{
	// TODO: Add your control notification handler code here
}


BOOL CPointHisdataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lstPointName.ResetContent();
	for(int i=0;i<m_strPointNameList.size();i++)
	{
		m_lstPointName.AddString(m_strPointNameList[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CPointHisdataDlg::OnBnClickedButtonRemoveInRange()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	COleDateTimeSpan tspan = COleDateTimeSpan(0,0,1,0);
	COleDateTime tStart(m_DateFrom.GetYear(), m_DateFrom.GetMonth(),m_DateFrom.GetDay(),
		m_TimeFrom.GetHour(), m_TimeFrom.GetMinute(), 0);
	COleDateTime tEnd(m_DateTo.GetYear(), m_DateTo.GetMonth(),m_DateTo.GetDay(),
		m_TimeTo.GetHour(), m_TimeTo.GetMinute(), 0);


	CString strPointTime;
	CString strPointName;
	string strTimeAt_Utf8;
	string strPointName_Utf8;
	bool beDelete=false;	


	CInputConditionDlg dlg;
	CString strCondition;
	if(dlg.DoModal()==IDOK)
	{
		strCondition = dlg.m_strCondition;
	}




	if(AfxMessageBox(L"是否确认删除该时段内左侧点名清单的所有符合条件的历史数据?",MB_YESNO)!= IDYES)
	{
		return;
	}



	Json::Value requestRemoveList;
	for(int i=0;i<m_strPointNameList.size();i++)
	{
		Json::Value oneRemoveItem;

		Project::Tools::WideCharToUtf8(m_strPointNameList[i].GetString(), strPointName_Utf8);

		requestRemoveList.append(strPointName_Utf8);
	}

	CString strTimeFrom = tStart.Format(_T("%Y-%m-%d %H:%M:%S"));
	CString strTimeTo = tEnd.Format(_T("%Y-%m-%d %H:%M:%S"));
	string strTimeFrom_Utf8, strTimeTo_Utf8, strCondition_Utf8;
	Project::Tools::WideCharToUtf8(strTimeFrom.GetString(), strTimeFrom_Utf8);
	Project::Tools::WideCharToUtf8(strTimeTo.GetString(), strTimeTo_Utf8);
	Project::Tools::WideCharToUtf8(strCondition.GetString(), strCondition_Utf8);

		Json::Value myRoot;
		CHttpOperation http(m_strServer, m_nServerPort, _T("hisdata/remove_history_data_in_time_range"));
		myRoot["pointNameList"] = requestRemoveList;
		myRoot["timeFrom"] = strTimeFrom_Utf8;
		myRoot["timeTo"] = strTimeTo_Utf8;
		myRoot["where"] = strCondition_Utf8;
		
		string szBuf = myRoot.toStyledString();
		bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), m_strResult,  _T("Content-Type: application/json"));
		if(bRet)
		{
			Json::Reader jsReader;
			string strResult_utf8;
			Project::Tools::WideCharToUtf8(m_strResult.GetString(), strResult_utf8);
			Json::Value myReturnRoot;
			if (!jsReader.parse(strResult_utf8, myReturnRoot))
			{
				return ;
			}
			else
			{
				if(myReturnRoot.isObject())
				{
					Json::Value myData =  myReturnRoot["err"];
					int nbeErr = myData.asInt();
					if (nbeErr==0)
					{
						
						AfxMessageBox(_T("删除成功"));


					}
					else
					{
						AfxMessageBox(_T("删除失败"));
					}	
				}
			}
		}


}


void CPointHisdataDlg::OnBnClickedButtonImportFromText()
{
	// TODO: Add your control notification handler code here
	CInputTextDlg dlg;
	CString strCurAll;
	for(int i=0;i<m_lstPointName.GetCount();i++)
	{
		CString strTemp;
		m_lstPointName.GetText(i, strTemp);
		strCurAll+= strTemp;
		if(i<m_lstPointName.GetCount()-1)
			strCurAll+=_T(",");
	}
	dlg.m_strText = strCurAll;
	if(dlg.DoModal()==IDOK)
	{
		CString strAll = dlg.m_strText;
		std::vector<CString> strArray;
		Project::Tools::SplitStringByChar(strAll, ',', strArray);

		if(strArray.size()<=0)
		{
			Project::Tools::SplitStringByChar(strAll, ' ', strArray);
			if(strArray.size()<=0)
			{
				Project::Tools::SplitStringByChar(strAll, '\t', strArray);
				if(strArray.size()<=0)
				{
					AfxMessageBox(_T("未导入任何点名，必须以英文逗号、空格、TAB(只能一种)分隔点名"));
					return ;
				}
			}
		}

		m_lstPointName.ResetContent();
		m_strPointNameList.clear();
		for(int i=0;i<strArray.size();i++)
		{
			if(strArray[i].GetLength()<=0)
				continue;

			if(strArray[i]==_T(" "))
				continue;

			if(strArray[i].Find(_T("\t"))>=0)
				continue;

			m_strPointNameList.push_back(strArray[i]);
			m_lstPointName.AddString(strArray[i]);
		}
	}
}
