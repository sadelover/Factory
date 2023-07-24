// DataSampleManageDlg.cpp : implementation file
//
#include "stdafx.h"
#include "DataSampleManageDlg.h"
#include "afxdialogex.h"

#include "HttpOperation.h"
#include "json/json.h"
#include "Tools/CustomTools/CustomTools.h"
#include "CreateSample.h"
#include "MainFrm.h"
#include "oleExcel/OperateExcelFile.h"
#include "Tools/CustomTools/CustomTools.h"
	// CDataSampleManageDlg dialog

	IMPLEMENT_DYNAMIC(CDataSampleManageDlg, CDialogEx)

	CDataSampleManageDlg::CDataSampleManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataSampleManageDlg::IDD, pParent)
{

	m_TimeFrom = COleDateTime::GetCurrentTime()-COleDateTimeSpan(10,0,0,0);
	m_TimeTo =  COleDateTime::GetCurrentTime();
	m_DateFrom = COleDateTime::GetCurrentTime()-COleDateTimeSpan(10,0,0,0);
	m_DateTo = COleDateTime::GetCurrentTime();
}

CDataSampleManageDlg::~CDataSampleManageDlg()
{
}

void CDataSampleManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CUSTOM_DATATABLE, m_Grid);
	DDX_DateTimeCtrl(pDX, IDC_DATE_START, m_DateFrom);
	DDX_DateTimeCtrl(pDX, IDC_TIME_START, m_TimeFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATE_END, m_DateTo);
	DDX_DateTimeCtrl(pDX, IDC_TIME_END, m_TimeTo);

	DDX_Control(pDX, IDC_COMBO_SAMPLELIST, m_comboSampeList);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, m_CDownLoad);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboSampeList);
}


BEGIN_MESSAGE_MAP(CDataSampleManageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDataSampleManageDlg::OnBnClickedButtonQuery)
	ON_CBN_SELCHANGE(IDC_COMBO_SAMPLELIST, &CDataSampleManageDlg::OnCbnSelchangeComboSamplelist)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CDataSampleManageDlg::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_SAMPLE, &CDataSampleManageDlg::OnBnClickedButtonCreateSample)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_SAMPLE, &CDataSampleManageDlg::OnBnClickedButtonImportSample)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SAMPLE, &CDataSampleManageDlg::OnBnClickedButtonDeleteSample)
END_MESSAGE_MAP()


// CDataSampleManageDlg message handlers




BOOL CDataSampleManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strServer, m_nServerPort, _T("dataSample/getSampleList"));


	Json::Value myRoot;

	myRoot["sampleId"] =0;



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
			return FALSE;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value myData =  myReturnRoot["data"];
				if(myData.isArray())
				{
					for(int i=0;i<myData.size();i++)
					{
						Json::Value oneItem = myData[Json::UInt(i)];
						if(oneItem.isObject())
						{

							Json::Value itemName = oneItem["name"];
							Json::Value itemId = oneItem["id"];
							int nId  = itemId.asInt();
							string strName = itemName.asString();
							CString str;
							str=strName.c_str();   
							m_strSampleNameList.push_back(str);
							wstring wstrName = Project::Tools::AnsiToWideChar(strName.c_str());
							int ncur = m_comboSampeList.AddString(wstrName.c_str());
							m_comboSampeList.SetItemData(ncur, nId);
						}
					}
				}

			}
		}

	}

	if(m_comboSampeList.GetCount()>0)
		m_comboSampeList.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDataSampleManageDlg::OnBnClickedButtonQuery()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	int nSampleId = m_comboSampeList.GetItemData(m_comboSampeList.GetCurSel());

	CString strSampleName ;
	m_comboSampeList.GetLBText(nSampleId-1,strSampleName);


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
	CHttpOperation http(m_strServer, m_nServerPort, _T("dataSample/getSampleDataList"));

	wstring wstrTimeStart,wstrTimeEnd,wstrSampleName ;
	Project::Tools::OleTime_To_String(m_tStart, wstrTimeStart );
	Project::Tools::OleTime_To_String(m_tEnd, wstrTimeEnd );

	wstrSampleName=strSampleName.GetString();
	Json::Value myRoot;


	m_Grid.DeleteAllItems();
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(2);
	m_Grid.SetItemText(0,0, _T("开始时间"));
	m_Grid.SetItemText(0,1, _T("结束时间"));

	m_Grid.SetDefCellMargin(2);
	m_Grid.SetFixedRowCount(1);

	myRoot["dateFrom"] = Project::Tools::WideCharToUtf8(wstrTimeStart.substr(0,10).c_str());
	myRoot["dateTo"] = Project::Tools::WideCharToUtf8(wstrTimeEnd.substr(0,10).c_str());
	myRoot["sampleId"] = Project::Tools::WideCharToUtf8(wstrSampleName.c_str());


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
				
				Json::Value timeFrom;
				Json::Value timeTo;
				Json::Value myData =  myReturnRoot["data"];

				m_Grid.SetRowCount(1+myData.size());
				for(int i=0;i<myData.size();i++)
				{
					vector<Json::Value> inputItem(50);
					vector<Json::Value> outputItem(10);
					Json::Value oneItem = myData[Json::UInt(i)];

					if (oneItem.isObject())
					{
						inputItem[0] = oneItem["input01"];

						inputItem[1] = oneItem["input02"];
						inputItem[2] = oneItem["input03"];
						inputItem[3] = oneItem["input04"];
						inputItem[4] = oneItem["input05"];
						inputItem[5] = oneItem["input06"];
						inputItem[6] = oneItem["input07"];
						inputItem[7] = oneItem["input08"];
						inputItem[8] = oneItem["input09"];
						inputItem[9] = oneItem["input10"];

						inputItem[10] = oneItem["input11"];
						inputItem[11] = oneItem["input12"];
						inputItem[12] = oneItem["input13"];
						inputItem[13] = oneItem["input14"];
						inputItem[14] = oneItem["input15"];
						inputItem[15] = oneItem["input16"];
						inputItem[16] = oneItem["input17"];
						inputItem[17] = oneItem["input18"];
						inputItem[18] = oneItem["input19"];
						inputItem[19] = oneItem["input20"];

						inputItem[20] = oneItem["input21"];
						inputItem[21] = oneItem["input22"];
						inputItem[22] = oneItem["input23"];
						inputItem[23] = oneItem["input24"];
						inputItem[24] = oneItem["input25"];
						inputItem[25] = oneItem["input26"];
						inputItem[26] = oneItem["input27"];
						inputItem[27] = oneItem["input28"];
						inputItem[28] = oneItem["input29"];
						inputItem[29] = oneItem["input30"];

						inputItem[30] = oneItem["input31"];
						inputItem[31] = oneItem["input32"];
						inputItem[32] = oneItem["input33"];
						inputItem[33] = oneItem["input34"];
						inputItem[34] = oneItem["input35"];
						inputItem[35] = oneItem["input36"];
						inputItem[36] = oneItem["input37"];
						inputItem[37] = oneItem["input38"];
						inputItem[38] = oneItem["input39"];
						inputItem[39] = oneItem["input40"];

						inputItem[40] = oneItem["input41"];
						inputItem[41] = oneItem["input42"];
						inputItem[42] = oneItem["input43"];
						inputItem[43] = oneItem["input44"];
						inputItem[44] = oneItem["input45"];
						inputItem[45] = oneItem["input46"];
						inputItem[46] = oneItem["input47"];
						inputItem[47] = oneItem["input48"];
						inputItem[48] = oneItem["input49"];
						inputItem[49] = oneItem["input50"];

						outputItem[0] = oneItem["output01"];
						outputItem[1] = oneItem["output02"];
						outputItem[2] = oneItem["output03"];
						outputItem[3] = oneItem["output04"];
						outputItem[4] = oneItem["output05"];
						outputItem[5] = oneItem["output06"];
						outputItem[6] = oneItem["output07"];
						outputItem[7] = oneItem["output08"];
						outputItem[8] = oneItem["output09"];
						outputItem[9] = oneItem["output10"];
						timeFrom = oneItem["timeFrom"];
						timeTo = oneItem["timeTo"];
						vector <Json::Value>::iterator input_iter;
						
						for (input_iter=inputItem.begin(); input_iter!=inputItem.end();)
						{
							string str = input_iter->asString();
							int a = atoi(str.c_str());
							if (a==-1||str=="")
							{
								input_iter=inputItem.erase(input_iter);	
							}
							else
							{
								++input_iter;
							}
						}
						vector <Json::Value>::iterator output_iter;
						
						for (output_iter=outputItem.begin(); output_iter!=outputItem.end();)
						{
							string str = output_iter->asString();
							/*if (str=="")
							{
								output_iter=outputItem.erase(output_iter);
							}*/
							int a = atoi(str.c_str());
							if (a==-1||str=="")
							{
								output_iter=outputItem.erase(output_iter);	
						    }
							else
							{
								++output_iter;
							}
						}
						m_Grid.SetColumnCount(inputItem.size()+outputItem.size()+2);
						
						wstring wstritem ;
						Project::Tools::UTF8ToWideChar(timeFrom.asString(), wstritem);
						m_Grid.SetItemText(i+1, 0, wstritem.c_str());
						Project::Tools::UTF8ToWideChar(timeTo.asString(), wstritem);
						m_Grid.SetItemText(i+1, 1, wstritem.c_str());
		                for (int j=0;j<inputItem.size();j++)
		                {
							CString str;
							str.Format(_T("input%d"),j+1);
							m_Grid.SetItemText(0, j+2, str);
							wstring wstritem ;
							Project::Tools::UTF8ToWideChar(inputItem[j].asString(), wstritem);
							m_Grid.SetItemText(i+1, j+2, wstritem.c_str());
		                }
						for (int j=0;j<outputItem.size();j++)
						{
							wstring wstritem ;
							CString str;
							str.Format(_T("output%d"),j+1);
							m_Grid.SetItemText(0, j+2+inputItem.size(), str);
							Project::Tools::UTF8ToWideChar(outputItem[j].asString(), wstritem);
							m_Grid.SetItemText(i+1, j+2+inputItem.size(), wstritem.c_str());
						}

					}
					
						
				}

			}
		}
	}
	else
	{
		AfxMessageBox(_T("请求数据失败，可能是超时"));
    	return;
	}
	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();
}

void CDataSampleManageDlg::OnCbnSelchangeComboSamplelist()
{

	// TODO: Add your control notification handler code here




}

void CDataSampleManageDlg::OnBnClickedButtonDownload()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)")_T("All Files (*.*)|*.*||"), NULL);

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

void CDataSampleManageDlg::OnBnClickedButtonCreateSample()
{
	// TODO: Add your control notification handler code here
	//根据input和output数量生成样本
	CCreateSample dlg;
	if (dlg.DoModal()==IDOK)
	{
		/*dlg.m_nInputNum
	    dlg.m_nOutputNum*/
		

		int nId = -1;
		CString strSrc;
		CString strResult;
		CHttpOperation http(m_strServer, m_nServerPort, _T("dataSample/getOrCreateSample"));
		wstring wstr = dlg.m_strSampleName.GetString();
		
		
		for (int i=0;i<m_strSampleNameList.size();i++)
		{
			if (dlg.m_strSampleName==m_strSampleNameList[i])
			{
				AfxMessageBox(_T("该样本名已存在！"));
				return;
			}
			else
			{
				continue;
			}
		}

		Json::Value myRoot;

		myRoot["sampleName"] = Project::Tools::WideCharToUtf8(wstr.c_str());



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
					Json::Value myInfo =  myReturnRoot["err"];
					if(myInfo.asInt()==0)
					{
						AfxMessageBox(_T("新建样本成功!"));
					}
					else
					{
						AfxMessageBox(_T("新建样本失败!"));
					}
					int ncur = m_comboSampeList.AddString(dlg.m_strSampleName);
					m_comboSampeList.SetItemData(ncur, myData.asInt());
				}
			}
	    }





	}
	else
	{
		return ;
	}
	




}

void CDataSampleManageDlg::OnBnClickedButtonImportSample()
{
	// TODO: Add your control notification handler code here
	//导入Excel表格插入数据库
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CString strSys; 
	GetSysPath(strSys);
	strSys +=  _T("\\config");
	CFileDialog dlg(TRUE, _T(""), _T(""), 0, _T("Excel File (*.xls;*.xlsx)|*.xls;*.xlsx|")_T("CSV File (*.csv)|*.csv|"), NULL);
	dlg.m_ofn.lpstrInitialDir= strSys.GetString();
	if (dlg.DoModal() == IDOK)
	{

		CString strFilePath = dlg.GetPathName();
		CString strName = dlg.GetFileName();
		int nNameLen = strName.GetLength();
		CString strExt(strName.Right(nNameLen - strName.ReverseFind(_T('.'))-1));
		if (strExt.CompareNoCase(L"csv") == 0 )
		{
			vector < vector<CString> >vecSampleList;
			vector < vector<CString> >vecInputList;
			vector < vector<CString> >vecOutPutList;
			vector<int>vecSampleIdList;
			//bool bSucess = COperateExcelFile::Instance()->ReadSampleDataFromExcel(strFilePath,vecSampleList,vecInputList,vecOutPutList);
			//读CSV文件
			bool bSucess = ReadSampleDataFromCSV(strFilePath,vecSampleList,vecInputList,vecOutPutList);



			if (!bSucess)
			{
				return ;
			}
			//显示
			m_Grid.DeleteAllItems();
			m_Grid.SetRowCount(1);
			m_Grid.SetColumnCount(vecInputList[0].size()+vecOutPutList[0].size()+4);
			m_Grid.SetItemText(0,0, _T("样本id"));
			m_Grid.SetItemText(0,1, _T("样本名"));
			m_Grid.SetItemText(0,2, _T("开始时间"));
			m_Grid.SetItemText(0,3, _T("结束时间"));

			m_Grid.SetDefCellMargin(2);
			m_Grid.SetFixedRowCount(1);
			for (int j=0;j<vecInputList[0].size();j++)
			{
				CString strTemp;
				strTemp.Format(_T("input%d"),j+1);
				m_Grid.SetItemText(0, j+4, strTemp);

			}
			for (int j=0;j<vecOutPutList[0].size();j++)
			{
				CString str;
				str.Format(_T("output%d"),j+1);
				m_Grid.SetItemText(0, j+4+vecInputList[0].size(), str);

			}

			


					//检查是否存在该样本，若不存在，则创建新样本
			for (int i=0;i<vecSampleList.size();i++)
			{
				//for (int j=0;j<m_strSampleNameList.size();j++)
				//{
				//	while(vecSampleList[i][0]==m_strSampleNameList[j])
				//	{
				//		//AfxMessageBox(_T("该样本名已存在！已替换"));
				//		//删除该记录
				//		vecSampleList[i][0]=m_strSampleNameList[j]+_T("+");
				//		
				//	
				//	}
				//
				//	
				//}
				CString strResult_remove;
				CHttpOperation http_remove(m_strServer, m_nServerPort, _T("dataSample/getOrCreateSample"));
				Json::Value myRoot;
			
				myRoot["sampleName"]= Project::Tools::WideCharToUtf8(vecSampleList[i][0].GetString());
				string szBuf = myRoot.toStyledString();
				bool bRet = http_remove.HttpPostGetRequestEx(0, szBuf.data(), strResult_remove,  _T("Content-Type: application/json"));
				if (bRet)
				{
					Json::Reader jsReader;
					string strResult_remove_utf8;
					Project::Tools::WideCharToUtf8(strResult_remove.GetString(), strResult_remove_utf8);
					Json::Value myReturnRoot;
					if (!jsReader.parse(strResult_remove_utf8, myReturnRoot))
					{
						return ;
					}
					else
					{
						if(myReturnRoot.isObject())
						{
							Json::Value myData =  myReturnRoot["data"];
							int nid = myData.asInt();
							vecSampleIdList.push_back(nid);
							int ncur = m_comboSampeList.AddString(vecSampleList[i][0]);
							m_comboSampeList.SetItemData(ncur, myData.asInt());
						}
					}
				}
			}
			//清除原有历史数据
			for (int i=0;i<vecSampleIdList.size();i++)
			{
				CString strResult_remove;
				CHttpOperation http_remove(m_strServer, m_nServerPort, _T("dataSample/removeIO"));
				Json::Value myRoot;

				myRoot["sampleId"]= vecSampleIdList[i];
				string szBuf = myRoot.toStyledString();
				bool bRet = http_remove.HttpPostGetRequestEx(0, szBuf.data(), strResult_remove,  _T("Content-Type: application/json"));
				if (bRet)
				{
					Json::Reader jsReader;
					string strResult_remove_utf8;
					Project::Tools::WideCharToUtf8(strResult_remove.GetString(), strResult_remove_utf8);
					Json::Value myReturnRoot;
					if (!jsReader.parse(strResult_remove_utf8, myReturnRoot))
					{
						return ;
					}
					else
					{
						if(myReturnRoot.isObject())
						{
							Json::Value myData =  myReturnRoot["err"];
						    if (myData.asInt()==0)
						    {
								continue;
						    }
							else
							{
								return;
							}

						}
					}
				}
			}
			for (int i=0;i<vecSampleList.size();i++)
			{
				m_Grid.SetRowCount(i+2);
				//显示id
			    CString strTemp;
				strTemp.Format(_T("%d"),vecSampleIdList[i]);
				m_Grid.SetItemText(i+1,0,strTemp);
				//显示样本名
				m_Grid.SetItemText(i+1,1,vecSampleList[i][0]);
				//显示时间
				m_Grid.SetItemText(i+1,2,vecSampleList[i][1]);
				m_Grid.SetItemText(i+1,3,vecSampleList[i][2]);
				//显示输入
				for (int j=0;j<vecInputList[i].size();j++)
				{

					m_Grid.SetItemText(i+1,j+4,vecInputList[i][j]);
				}
				//显示输出
				for (int j=0;j<vecOutPutList[i].size();j++)
				{

					m_Grid.SetItemText(i+1,j+vecInputList[i].size()+4,vecOutPutList[i][j]);
				}
		
				//存入数据库
				//补全

				while(vecInputList[i].size()<50)
				{
					vecInputList[i].push_back(_T(""));
				}

				while(vecOutPutList[i].size()<10)
				{
					vecOutPutList[i].push_back(_T(""));
				}
			}
			
			
			CString strResult;
			CHttpOperation http(m_strServer, m_nServerPort, _T("dataSample/insertIOs"));
			Json::Value myRootList;

			for (int i=0;i<vecSampleList.size();i++)
			{
				
				wstring wstrTimeFrom = vecSampleList[i][1];
				wstring wstrTimeTo = vecSampleList[i][2];
				//input
				wstring wstrInput01 = vecInputList[i][0];
				wstring wstrInput02= vecInputList[i][1];
				wstring wstrInput03= vecInputList[i][2];
				wstring wstrInput04= vecInputList[i][3];
				wstring wstrInput05= vecInputList[i][4];
				wstring wstrInput06= vecInputList[i][5];
				wstring wstrInput07= vecInputList[i][6];
				wstring wstrInput08= vecInputList[i][7];
				wstring wstrInput09= vecInputList[i][8];
				wstring wstrInput10= vecInputList[i][9];
				wstring wstrInput11= vecInputList[i][10];
				wstring wstrInput12= vecInputList[i][11];
				wstring wstrInput13= vecInputList[i][12];
				wstring wstrInput14= vecInputList[i][13];
				wstring wstrInput15= vecInputList[i][14];
				wstring wstrInput16= vecInputList[i][15];
				wstring wstrInput17= vecInputList[i][16];
				wstring wstrInput18= vecInputList[i][17];
				wstring wstrInput19= vecInputList[i][18];
				wstring wstrInput20= vecInputList[i][19];
				wstring wstrInput21= vecInputList[i][20];
				wstring wstrInput22= vecInputList[i][21];
				wstring wstrInput23= vecInputList[i][22];
				wstring wstrInput24= vecInputList[i][23];
				wstring wstrInput25= vecInputList[i][24];
				wstring wstrInput26= vecInputList[i][25];
				wstring wstrInput27= vecInputList[i][26];
				wstring wstrInput28= vecInputList[i][27];
				wstring wstrInput29= vecInputList[i][28];
				wstring wstrInput30= vecInputList[i][29];
				wstring wstrInput31= vecInputList[i][30];
				wstring wstrInput32= vecInputList[i][31];
				wstring wstrInput33= vecInputList[i][32];
				wstring wstrInput34= vecInputList[i][33];
				wstring wstrInput35= vecInputList[i][34];
				wstring wstrInput36= vecInputList[i][35];
				wstring wstrInput37= vecInputList[i][36];
				wstring wstrInput38= vecInputList[i][37];
				wstring wstrInput39= vecInputList[i][38];
				wstring wstrInput40= vecInputList[i][39];
				wstring wstrInput41= vecInputList[i][40];
				wstring wstrInput42= vecInputList[i][41];
				wstring wstrInput43= vecInputList[i][42];
				wstring wstrInput44= vecInputList[i][43];
				wstring wstrInput45= vecInputList[i][44];
				wstring wstrInput46= vecInputList[i][45];
				wstring wstrInput47= vecInputList[i][46];
				wstring wstrInput48= vecInputList[i][47];
				wstring wstrInput49= vecInputList[i][48];
				wstring wstrInput50= vecInputList[i][49];
				//output
				wstring wstrOutput01=vecOutPutList[i][0];
				wstring wstrOutput02=vecOutPutList[i][1];
				wstring wstrOutput03=vecOutPutList[i][2];
				wstring wstrOutput04=vecOutPutList[i][3];
				wstring wstrOutput05=vecOutPutList[i][4];
				wstring wstrOutput06=vecOutPutList[i][5];
				wstring wstrOutput07=vecOutPutList[i][6];
				wstring wstrOutput08=vecOutPutList[i][7];
				wstring wstrOutput09=vecOutPutList[i][8];
				wstring wstrOutput10=vecOutPutList[i][9];


				Json::Value myRoot;
				
				myRoot.append(vecSampleIdList[i]);
				myRoot.append(Project::Tools::WideCharToUtf8(wstrTimeFrom.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrTimeTo.c_str()));
				//myRoot["sampleId"] = vecSampleIdList[i];
				//myRoot["timeFrom"] = Project::Tools::WideCharToUtf8(wstrTimeFrom.c_str()));
				//myRoot["timeTo"] = Project::Tools::WideCharToUtf8(wstrTimeTo.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrTimeTo.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput02.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput03.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput04.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput05.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput06.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput07.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput08.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput09.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput10.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput11.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput12.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput13.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput14.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput15.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput16.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput17.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput18.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput19.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput20.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput21.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput22.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput23.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput24.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput25.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput26.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput27.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput28.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput29.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput30.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput31.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput32.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput33.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput34.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput35.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput36.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput37.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput38.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput39.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput40.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput41.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput42.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput43.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput44.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput45.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput46.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput47.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput48.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput49.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrInput50.c_str()));

				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput01.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput02.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput03.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput04.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput05.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput06.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput07.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput08.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput09.c_str()));
				myRoot.append(Project::Tools::WideCharToUtf8(wstrOutput10.c_str()));
				myRootList["sampleList"].append(myRoot);
             }
				string szBuf = myRootList.toStyledString();
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
							Json::Value myData =  myReturnRoot["err"];
							if(myData.asInt()==0)
							{
								//continue;
							}
							else
							{
								return;
							}

						}
					}
					AfxMessageBox(L"读取样本文件成功!");
				}
				else
				{
					return;
				}

			
		}
	}
	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();
}
void CDataSampleManageDlg::OnBnClickedButtonDeleteSample()
{
	// TODO: Add your control notification handler code here
   //删除样本
	  CCellID nItem= m_Grid.GetFocusCell();
	  CString strResult;
	  CString strResult_delete;
	  //拿到id
	CHttpOperation http_id(m_strServer, m_nServerPort, _T("dataSample/getOrCreateSample"));
	Json::Value myRoot_id;
	int nitem = m_comboSampeList.GetItemData(m_comboSampeList.GetCurSel());
	CString strSampleName ;
	m_comboSampeList.GetLBText(nitem-1,strSampleName);
	wstring wstr = strSampleName.GetString();
	myRoot_id["sampleName"].append(Project::Tools::WideCharToUtf8(wstr.c_str()));
	string szBuf = myRoot_id.toStyledString();
	bool bRet = http_id.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));
	int nSampleId;
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
				nSampleId= myData.asInt();

			}
		}
	}
	  CString strSampleId ;
	  strSampleId.Format(_T("%d"),nSampleId);
	  CString strTimeFrom = m_Grid.GetItemText(nItem.row,0);
	  CString strTimeTo = m_Grid.GetItemText(nItem.row,1);
	  wstring wstrSampleId = strSampleId.GetString();
	  wstring wstrTimeFrom =strTimeFrom.GetString();
	  wstring wstrTimeTo = strTimeTo.GetString();
	 
	 CHttpOperation http_delete(m_strServer, m_nServerPort, _T("dataSample/removeIO"));
	Json::Value myRoot_delete;
	myRoot_delete["sampleId"] = Project::Tools::WideCharToUtf8(wstrSampleId.c_str());
	myRoot_delete["timeFrom"] = Project::Tools::WideCharToUtf8(wstrTimeFrom.c_str());
	myRoot_delete["timeTo"] = Project::Tools::WideCharToUtf8(wstrTimeTo.c_str());
	szBuf = myRoot_delete.toStyledString();
    bRet = http_delete.HttpPostGetRequestEx(0, szBuf.data(), strResult_delete,  _T("Content-Type: application/json"));
	if (bRet)
	{
		Json::Reader jsReader;
		string strResult_delete_utf8;
		Project::Tools::WideCharToUtf8(strResult_delete.GetString(), strResult_delete_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_delete_utf8, myReturnRoot))
		{
			return ;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value myData =  myReturnRoot["err"];
				if(myData.asInt()==0)
				{
					m_Grid.DeleteRow(nItem.row);
					AfxMessageBox(_T("删除样本成功!"));
				}
				else
				{
					AfxMessageBox(_T("删除样本失败!"));
				}

			}
		}


	}
	m_Grid.AutoSize(GVS_BOTH);
	m_Grid.Refresh();
}
bool CDataSampleManageDlg::ReadSampleDataFromCSV(CString strSamplePath,vector<vector<CString>>&vecSampleList,vector<vector<CString>>&vecInputList,vector<vector<CString>>&vecOutputList)
{
	char* old_locale=_strdup( setlocale(LC_CTYPE,NULL) ); 
	setlocale( LC_CTYPE,"chs");
	ASSERT(strSamplePath.GetLength()>0);

	
	CStdioFile file;
	if (!file.Open(strSamplePath,CFile::modeRead | CFile::shareDenyWrite))
	{
		TRACE(_T("Load (file): Error opening file %s\n"),strSamplePath);
		return false;
	}
	vector<CString>vecResultList;
	vector<CString>vecTitleList;
	CString strLine;
	BOOL bRet = file.ReadString(strLine);
	ASSERT(bRet);
	SplitStringByChar(strLine, L',', vecTitleList);
	for (int i=0;i<vecTitleList.size();i++)
	{
		int bfind = vecTitleList[i].Find(_T("input"));
		int bfind_2 = vecTitleList[i].Find(_T("output"));
		if (bfind!=-1||bfind_2!=-1)
		{
			continue;
		}
		else
		{
			if (i==vecTitleList.size()-1)
			{
				AfxMessageBox(_T("此文件非样本文件!"));
				return false;
			}
			
		}
	}
	int nCount =0;
	while(file.ReadString(strLine))
	{
		SplitStringByChar(strLine, L',', vecResultList);
		for (int i=0;i<3;i++)
		{
			vecSampleList.resize(nCount+1);
			vecSampleList[nCount].push_back(vecResultList[i]);
		}

		//初始化input数组
		for (int i=0;i<vecResultList.size();i++)
		{
			vecInputList.resize(nCount+1);
			int bfind = vecTitleList[i].Find(_T("input"));
			if (bfind!=-1)
			{
				vecInputList[nCount].push_back(vecResultList[i]);
			}
		}
		//初始化output数组
		for (int i=0;i<vecResultList.size();i++)
		{
			vecOutputList.resize(nCount+1);
			int bfind_2 = vecTitleList[i].Find(_T("output"));
			if (bfind_2!=-1)
			{
				vecOutputList[nCount].push_back(vecResultList[i]);
			}
		}
		nCount++;
		
	   
     }
	file.Close();
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
	AfxMessageBox(L"读取样本文件成功!");
	return true;
}