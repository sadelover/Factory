// VPointHistoricalDataCalculation.cpp : 实现文件
//

#include "stdafx.h"
#include "VPointHistoricalDataCalculation.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "MainFrm.h"

#include "InputTextDlg.h"

// CVPointHistoricalDataCalculation 对话框

IMPLEMENT_DYNAMIC(CVPointHistoricalDataCalculation, CDialogEx)

CVPointHistoricalDataCalculation::CVPointHistoricalDataCalculation(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVPointHistoricalDataCalculation::IDD, pParent)
{
}

CVPointHistoricalDataCalculation::CVPointHistoricalDataCalculation(vector<DataPointEntry> &vec, CDataPointManager* pointManager, CWnd* pParent /*= NULL*/)
	: CDialogEx(CVPointHistoricalDataCalculation::IDD, pParent)
{
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	m_CalcIp = pFrame->m_strCalcIp;
	m_nCalcPort = pFrame->m_nCalcPort;

	m_pointManager = pointManager;
	vector<DataPointEntry>::iterator iIter = vec.begin();
	for(;iIter!=vec.end();iIter++)
	{	PointStruct temp(iIter->GetShortName().c_str(), iIter->GetParam(1).c_str());
		m_pointList.push_back(temp);
	}
}

CVPointHistoricalDataCalculation::CVPointHistoricalDataCalculation(CString cstrPointName, CString cstrPointScript, CDataPointManager *pointManager, CWnd* pParent /*= NULL*/)
	: CDialogEx(CVPointHistoricalDataCalculation::IDD, pParent)
{
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	m_CalcIp = pFrame->m_strCalcIp;
	m_nCalcPort = pFrame->m_nCalcPort;

	m_pointManager = pointManager;
	m_pointList.clear();
	PointStruct temp(cstrPointName, cstrPointScript);
	m_pointList.push_back(temp);
}

CVPointHistoricalDataCalculation::~CVPointHistoricalDataCalculation()
{
}

void CVPointHistoricalDataCalculation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_gridInput);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FROM, m_dateTime_from);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TO, m_dateTime_to);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_combo_period);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FROM_TIME, m_dateTime_time_from);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TO_TIME, m_dateTime_time_to);
	DDX_Control(pDX, IDC_STATIC_MSG, m_static_Msg);
	DDX_Control(pDX, IDC_LIST_POINT, m_list_point);
}
#define WM_UPDATEMY_DATA    ((WM_USER) + 500)

BEGIN_MESSAGE_MAP(CVPointHistoricalDataCalculation, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATION, &CVPointHistoricalDataCalculation::OnBnClickedButtonCalculation)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CVPointHistoricalDataCalculation::OnBnClickedButtonInsert)
	ON_MESSAGE(WM_UPDATEMY_DATA, &CVPointHistoricalDataCalculation::OnUpdateMyData)
	ON_BN_CLICKED(IDC_BUTTON_POINT_LIST_CTL, &CVPointHistoricalDataCalculation::OnBnClickedButtonPointListCtl)
END_MESSAGE_MAP()


// CVPointHistoricalDataCalculation 消息处理程序

bool CVPointHistoricalDataCalculation::CalculationAndDisplayPoints(void)
{
	COleDateTime dateTime_date_from;
	COleDateTime dateTime_time_from;
	COleDateTime dateTime_date_to;
	COleDateTime dateTime_time_to;
	m_dateTime_from.GetTime(dateTime_date_from);
	m_dateTime_time_from.GetTime(dateTime_time_from);
	m_dateTime_to.GetTime(dateTime_date_to);
	m_dateTime_time_to.GetTime(dateTime_time_to);
	
	//时间选择错误判断
	if(dateTime_date_from > dateTime_date_to){
		AfxMessageBox(L"初始日期不得大于结束时间");
		return false;
	}
	else if(dateTime_date_from == dateTime_date_to ){
		if(dateTime_time_from > dateTime_time_to){
			AfxMessageBox(L"初始时间不得大于结束时间");
			return false;
		}
		else if(dateTime_time_from == dateTime_time_to){
			AfxMessageBox(L"初始时间不得等于结束时间");
			return false;
		}	
	}
	ShowMsg(L"打包参数.....");
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(FALSE);//插入按钮禁用
	//调用接口获取点值列表
	CString cstrTimeFrom = dateTime_date_from.Format(L"%Y-%m-%d") + L" " +dateTime_time_from.Format(L"%H:%M:%S");
	CString cstrTimeTo = dateTime_date_to.Format(L"%Y-%m-%d") + L" " +dateTime_time_to.Format(L"%H:%M:%S");
	CString *pCString = (CString*)m_combo_period.GetItemDataPtr(m_combo_period.GetCurSel());

	Json::Value jsonSendPacket;
	jsonSendPacket["timeFrom"] = Project::Tools::WideCharToUtf8(cstrTimeFrom.GetString());
	jsonSendPacket["timeTo"] = Project::Tools::WideCharToUtf8(cstrTimeTo.GetString());
	jsonSendPacket["script"];
	jsonSendPacket["period"] = Project::Tools::WideCharToUtf8((*pCString).GetString());
	m_gridInput.SetRedraw(FALSE);//禁止重绘画面
	for(int i=0; i<m_pointList.size();i++)
	{
		CalculationAndDisplayOnePoint(jsonSendPacket ,i);
	}
	m_gridInput.SetRedraw(TRUE);//允许重绘画面
	m_gridInput.ResetScrollBars();
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(TRUE);//插入按钮禁用
	ShowMsg(L"插入界面表格完成");
	return  true;
}

bool CVPointHistoricalDataCalculation::CalculationAndDisplayOnePoint(Json::Value& jsonSendPacket, int nIndex)
{
	ShowMsg(L"正在发送数据请求.....");
	string strScript;
	try
	{
		strScript =  Project::Tools::WideCharToUtf8(m_pointList.at(nIndex).cstrScript.GetString()) ;//当前轮询到的点的脚本
	}
	catch (const std::out_of_range& oor)
	{
		return 0;
	}
	
	jsonSendPacket["script"] = strScript;

	//脚本计算请求段
	CString cstrReturn;
	CHttpOperation http(m_CalcIp, m_nCalcPort, _T("/scriptCalculation/calHistory"));
	http.SetInternetTimeOut(300000, 300000); //凡是长请求，最好需要设置超时，否则会web接口返回错误，这里设置五分钟超时
	bool bRet = false;
	string temp_jsonpacket = jsonSendPacket.toStyledString();
	bRet = http.HttpPostGetRequestEx(0, jsonSendPacket.toStyledString().data(), cstrReturn,  _T("Content-Type: application/json"));//
	if(bRet == true)
	{
		ShowMsg(L"请求成功，正在分析.....");
	}

	if(bRet == false){
		AfxMessageBox(L"dompysite未启动");
		ShowMsg(L"请求失败");
		return false;
	}

	//分析返回的数据包
	ShowMsg(L"分析请求的数据.....");
	string strReturn(Project::Tools::WideCharToUtf8(cstrReturn.GetString()));
	Json::Reader jsonReader;
	Json::Value	jsonValue; 
	if(false == jsonReader.parse(strReturn,jsonValue)){
		return false;
	}
	if(0 != jsonValue["err"].asInt()){
		string strMsg(jsonValue["msg"].asString());
		CString cstrMsg = Project::Tools::UTF8ToWideChar(strMsg.c_str()).c_str();
		ShowMsg(cstrMsg);
		AfxMessageBox(cstrMsg);
		return false;
	}
	//在json中点值列表的字段是之前参数脚本
	Json::Value &jsonPointValue = jsonValue["data"]["map"][strScript.c_str()];
	Json::Value &jsonPointTime = jsonValue["data"]["time"];

	if(false == jsonPointValue.isArray()){
		return false;
	}
	if(false == jsonPointTime.isArray()){
		return false;
	}
	if(jsonPointValue.size() != jsonPointTime.size()){
		return false;
	}

	ShowMsg(L"分析完成，正在插入表格.....");
	GridAddGroup(m_pointList.at(nIndex).cstrName, jsonPointValue.size());
	int nCol = m_gridInput.GetColumnCount();
	if(nCol != 0){
		nCol -=2;
	}
	for(unsigned int i = 0; i < jsonPointValue.size(); i++)
	{
		/*
			遍历json从0开始，遍历表格要从1开始
		*/
		CString cstrValue;
		CString cstrTime;

		if (jsonPointValue[i].isDouble())
		{
			double dValue = jsonPointValue[i].asDouble();//需要double点
			cstrValue.Format(L"%.6f",dValue);
		}
		else if(jsonPointValue[i].isInt())
		{
			int nValue = jsonPointValue[i].asInt();//需要double点
			cstrValue.Format(L"%d",nValue);
		}
		string strTime = jsonPointTime[i].asString();
		cstrTime = Project::Tools::UTF8ToWideChar(strTime.c_str()).c_str();

		m_gridInput.SetItemText(i+1,nCol,cstrTime);
		m_gridInput.SetItemText(i+1,nCol+1,cstrValue);
	}
}

void CVPointHistoricalDataCalculation::InsertDatabase(void)
{
	ShowMsg(L"正在打包参数....");
	//打包数据
	Json::Value jsonSendPacket;
	jsonSendPacket["valueList"];
	jsonSendPacket["timeList"];
	Json::Value & jsonKey_valueList = jsonSendPacket["valueList"];
	Json::Value & jsonKey_timeList	= jsonSendPacket["timeList"];
	CString cstrInsertMsg;
	for(int nCol=0; nCol<m_gridInput.GetColumnCount(); nCol+=2)
	{
		for(int i=1; i<m_gridInput.GetRowCount(); i++)
		{
			Json::Value jsonTemp1;
			Json::Value jsonTemp2;
			CString cstrTime = m_gridInput.GetItemText(i,nCol);
			CString cstrValue = m_gridInput.GetItemText(i,nCol+1);
			jsonKey_valueList.append(Project::Tools::WideCharToUtf8(cstrValue.GetString()));
			jsonKey_timeList.append(Project::Tools::WideCharToUtf8(cstrTime.GetString()));
		}
		jsonSendPacket["point"] = Project::Tools::WideCharToUtf8(m_gridInput.GetItemText(0,nCol+1).GetString());
		string strSendPacket = jsonSendPacket.toStyledString();
		CString cstrReturn;

		//调用接口
		ShowMsg(L"正在插入参数....");
		CHttpOperation http(m_CalcIp, m_nCalcPort, _T("/scriptCalculation/insertHistory"));
		http.SetInternetTimeOut(300000, 300000); //五分钟超时
		bool bRet = http.HttpPostGetRequestEx(0, jsonSendPacket.toStyledString().data(), cstrReturn,  _T("Content-Type: application/json"));
		if(false == bRet){
			CString cstrMsg = L"链接dompysite失败，dompysite可能未启动。";
			ShowMsg(cstrMsg);
			AfxMessageBox(cstrMsg);
			return;
		}
		string strReturn = Project::Tools::WideCharToUtf8(cstrReturn.GetString());
		Json::Reader jsonReader;
		Json::Value jsonValue;
		if(jsonReader.parse(strReturn,jsonValue))
		{	//打印接口返回的信息
			string strMsg = jsonValue["msg"].asString();
			CString cstrMsg = Project::Tools::UTF8ToWideChar(strMsg.c_str()).c_str();
			ShowMsg(cstrMsg);
			cstrInsertMsg = cstrInsertMsg + m_gridInput.GetItemText(0,nCol+1) + L":"+ cstrMsg + L"\r\n";
		}
		else
		{
			cstrInsertMsg = cstrInsertMsg + m_gridInput.GetItemText(0,nCol+1) + L"返回包格式不正确。\r\n";
		}
	}
	AfxMessageBox(cstrInsertMsg);
}

void CVPointHistoricalDataCalculation::GridSetRow(int nRow)
{
	nRow += 1;
	m_gridInput.SetRowCount(nRow);
}

void CVPointHistoricalDataCalculation::GridClear(void)
{
	m_gridInput.DeleteAllItems();
	m_gridInput.Refresh();
}

void CVPointHistoricalDataCalculation::GridAddGroup(CString cstrPointName, int nRow)
{
	//初始化列表
	int nCol = m_gridInput.GetColumnCount();
	m_gridInput.SetColumnCount(nCol+2);
	m_gridInput.SetFixedRowCount(1);
	m_gridInput.SetEditable(true);//表格内容不可编辑设置
	m_gridInput.SetItemText(0,nCol,L"时间");
	m_gridInput.SetItemText(0,nCol+1,cstrPointName);
	m_gridInput.SetColumnWidth(nCol,120);
	m_gridInput.SetColumnWidth(nCol+1,9*cstrPointName.GetLength());
	GridSetRow(nRow);
}

void CVPointHistoricalDataCalculation::RefreshListCtrl(void)
{
	m_list_point.ResetContent();
	vector<PointStruct>::iterator iIter = m_pointList.begin();
	for(;iIter!=m_pointList.end();iIter++)
	{
		m_list_point.AddString(iIter->cstrName);
	}
}

UINT CVPointHistoricalDataCalculation::Thread_Calculation(LPVOID pParam)
{
	CVPointHistoricalDataCalculation * pThis = (CVPointHistoricalDataCalculation*)pParam;
	pThis->CalculationAndDisplayPoints();
	pThis->PostMessage(WM_UPDATEMY_DATA);
	return 0;
}

UINT CVPointHistoricalDataCalculation::Thread_Insert(LPVOID pParam)
{
	CVPointHistoricalDataCalculation * pthis = (CVPointHistoricalDataCalculation*)pParam;
	pthis->InsertDatabase();
	return 0;
}

BOOL CVPointHistoricalDataCalculation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	COleDateTime newTime = COleDateTime::GetCurrentTime();
	COleDateTime setTime(newTime.GetYear(), newTime.GetMonth(), newTime.GetDay(),
		(int)0, (int)0, (int)0);
	m_dateTime_from.SetTime(setTime);
	m_dateTime_to.SetTime(setTime);
	m_dateTime_time_from.SetTime(setTime);
	m_dateTime_time_to.SetTime(setTime);

	//初始化时间分割
	m_combo_period.InsertString(0,L"一分钟");
	m_combo_period.InsertString(1,L"五分钟");
	m_combo_period.InsertString(2,L"一小时");
	m_combo_period.InsertString(3,L"一天");
	
	m_combo_period.SetItemDataPtr(0, (void *)(&period_m1));
	m_combo_period.SetItemDataPtr(1, (void *)(&period_m5));
	m_combo_period.SetItemDataPtr(2, (void *)(&period_h1));
	m_combo_period.SetItemDataPtr(3, (void *)(&period_d1));
	m_combo_period.SetCurSel(0);
	 
	//刷新点名表格控件
	RefreshListCtrl();
	
	//禁用按钮
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(FALSE);//插入按钮禁用
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CVPointHistoricalDataCalculation::OnBnClickedButtonCalculation()
{
	// TODO: 在此添加控件通知处理程序代码
	GridClear();
	UpdateData(TRUE);
	AfxBeginThread(Thread_Calculation,(LPVOID)this);//
}


void CVPointHistoricalDataCalculation::OnBnClickedButtonInsert()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (AfxMessageBox(L"插入数据库过程不可逆，请确认。",MB_OKCANCEL) == IDOK)
	{
		AfxBeginThread(Thread_Insert,(LPVOID)this);//
	}
}

LRESULT CVPointHistoricalDataCalculation::OnUpdateMyData(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	return 0;
}

void CVPointHistoricalDataCalculation::OnBnClickedButtonPointListCtl()
{
	// TODO: 在此添加控件通知处理程序代码
	CInputTextDlg dlg;
	dlg.m_strText.Empty();
	vector<PointStruct>::iterator iIter = m_pointList.begin();
	for(;iIter!=m_pointList.end()-1;iIter++)
	{
		dlg.m_strText += iIter->cstrName;
		dlg.m_strText += L",";
	}
	dlg.m_strText += iIter->cstrName;
	if(dlg.DoModal() == IDOK)
	{
		m_pointList.clear();
		vector<CString> pointname;
		Project::Tools::SplitStringByChar(dlg.m_strText, L',', pointname); 
		map<wstring, DataPointEntry> pointNameMap = m_pointManager->GetNameMap();
		for(vector<CString>::iterator iIter=pointname.begin(); iIter!=pointname.end();iIter++)
		{
			map<wstring, DataPointEntry>::iterator pointFound = pointNameMap.find(iIter->GetString());
			if(pointFound != pointNameMap.end())
			{
				PointStruct temp(pointFound->second.GetShortName().c_str(), pointFound->second.GetParam(1).c_str());
				m_pointList.push_back(temp);
			}
		}
		RefreshListCtrl();
	}
	UpdateData(FALSE);
}

void CVPointHistoricalDataCalculation::ShowMsg(CString& cstrMsg)
{
	m_static_Msg.SetWindowTextW(cstrMsg);
}

void CVPointHistoricalDataCalculation::ShowMsg(const wchar_t* cstrMsg)
{
	m_static_Msg.SetWindowTextW(cstrMsg);
}