// VPointHistoricalDataCalculation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VPointHistoricalDataCalculation.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "MainFrm.h"

#include "InputTextDlg.h"

// CVPointHistoricalDataCalculation �Ի���

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


// CVPointHistoricalDataCalculation ��Ϣ�������

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
	
	//ʱ��ѡ������ж�
	if(dateTime_date_from > dateTime_date_to){
		AfxMessageBox(L"��ʼ���ڲ��ô��ڽ���ʱ��");
		return false;
	}
	else if(dateTime_date_from == dateTime_date_to ){
		if(dateTime_time_from > dateTime_time_to){
			AfxMessageBox(L"��ʼʱ�䲻�ô��ڽ���ʱ��");
			return false;
		}
		else if(dateTime_time_from == dateTime_time_to){
			AfxMessageBox(L"��ʼʱ�䲻�õ��ڽ���ʱ��");
			return false;
		}	
	}
	ShowMsg(L"�������.....");
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(FALSE);//���밴ť����
	//���ýӿڻ�ȡ��ֵ�б�
	CString cstrTimeFrom = dateTime_date_from.Format(L"%Y-%m-%d") + L" " +dateTime_time_from.Format(L"%H:%M:%S");
	CString cstrTimeTo = dateTime_date_to.Format(L"%Y-%m-%d") + L" " +dateTime_time_to.Format(L"%H:%M:%S");
	CString *pCString = (CString*)m_combo_period.GetItemDataPtr(m_combo_period.GetCurSel());

	Json::Value jsonSendPacket;
	jsonSendPacket["timeFrom"] = Project::Tools::WideCharToUtf8(cstrTimeFrom.GetString());
	jsonSendPacket["timeTo"] = Project::Tools::WideCharToUtf8(cstrTimeTo.GetString());
	jsonSendPacket["script"];
	jsonSendPacket["period"] = Project::Tools::WideCharToUtf8((*pCString).GetString());
	m_gridInput.SetRedraw(FALSE);//��ֹ�ػ滭��
	for(int i=0; i<m_pointList.size();i++)
	{
		CalculationAndDisplayOnePoint(jsonSendPacket ,i);
	}
	m_gridInput.SetRedraw(TRUE);//�����ػ滭��
	m_gridInput.ResetScrollBars();
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(TRUE);//���밴ť����
	ShowMsg(L"������������");
	return  true;
}

bool CVPointHistoricalDataCalculation::CalculationAndDisplayOnePoint(Json::Value& jsonSendPacket, int nIndex)
{
	ShowMsg(L"���ڷ�����������.....");
	string strScript;
	try
	{
		strScript =  Project::Tools::WideCharToUtf8(m_pointList.at(nIndex).cstrScript.GetString()) ;//��ǰ��ѯ���ĵ�Ľű�
	}
	catch (const std::out_of_range& oor)
	{
		return 0;
	}
	
	jsonSendPacket["script"] = strScript;

	//�ű����������
	CString cstrReturn;
	CHttpOperation http(m_CalcIp, m_nCalcPort, _T("/scriptCalculation/calHistory"));
	http.SetInternetTimeOut(300000, 300000); //���ǳ����������Ҫ���ó�ʱ�������web�ӿڷ��ش���������������ӳ�ʱ
	bool bRet = false;
	string temp_jsonpacket = jsonSendPacket.toStyledString();
	bRet = http.HttpPostGetRequestEx(0, jsonSendPacket.toStyledString().data(), cstrReturn,  _T("Content-Type: application/json"));//
	if(bRet == true)
	{
		ShowMsg(L"����ɹ������ڷ���.....");
	}

	if(bRet == false){
		AfxMessageBox(L"dompysiteδ����");
		ShowMsg(L"����ʧ��");
		return false;
	}

	//�������ص����ݰ�
	ShowMsg(L"�������������.....");
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
	//��json�е�ֵ�б���ֶ���֮ǰ�����ű�
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

	ShowMsg(L"������ɣ����ڲ�����.....");
	GridAddGroup(m_pointList.at(nIndex).cstrName, jsonPointValue.size());
	int nCol = m_gridInput.GetColumnCount();
	if(nCol != 0){
		nCol -=2;
	}
	for(unsigned int i = 0; i < jsonPointValue.size(); i++)
	{
		/*
			����json��0��ʼ���������Ҫ��1��ʼ
		*/
		CString cstrValue;
		CString cstrTime;

		if (jsonPointValue[i].isDouble())
		{
			double dValue = jsonPointValue[i].asDouble();//��Ҫdouble��
			cstrValue.Format(L"%.6f",dValue);
		}
		else if(jsonPointValue[i].isInt())
		{
			int nValue = jsonPointValue[i].asInt();//��Ҫdouble��
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
	ShowMsg(L"���ڴ������....");
	//�������
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

		//���ýӿ�
		ShowMsg(L"���ڲ������....");
		CHttpOperation http(m_CalcIp, m_nCalcPort, _T("/scriptCalculation/insertHistory"));
		http.SetInternetTimeOut(300000, 300000); //����ӳ�ʱ
		bool bRet = http.HttpPostGetRequestEx(0, jsonSendPacket.toStyledString().data(), cstrReturn,  _T("Content-Type: application/json"));
		if(false == bRet){
			CString cstrMsg = L"����dompysiteʧ�ܣ�dompysite����δ������";
			ShowMsg(cstrMsg);
			AfxMessageBox(cstrMsg);
			return;
		}
		string strReturn = Project::Tools::WideCharToUtf8(cstrReturn.GetString());
		Json::Reader jsonReader;
		Json::Value jsonValue;
		if(jsonReader.parse(strReturn,jsonValue))
		{	//��ӡ�ӿڷ��ص���Ϣ
			string strMsg = jsonValue["msg"].asString();
			CString cstrMsg = Project::Tools::UTF8ToWideChar(strMsg.c_str()).c_str();
			ShowMsg(cstrMsg);
			cstrInsertMsg = cstrInsertMsg + m_gridInput.GetItemText(0,nCol+1) + L":"+ cstrMsg + L"\r\n";
		}
		else
		{
			cstrInsertMsg = cstrInsertMsg + m_gridInput.GetItemText(0,nCol+1) + L"���ذ���ʽ����ȷ��\r\n";
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
	//��ʼ���б�
	int nCol = m_gridInput.GetColumnCount();
	m_gridInput.SetColumnCount(nCol+2);
	m_gridInput.SetFixedRowCount(1);
	m_gridInput.SetEditable(true);//������ݲ��ɱ༭����
	m_gridInput.SetItemText(0,nCol,L"ʱ��");
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	COleDateTime newTime = COleDateTime::GetCurrentTime();
	COleDateTime setTime(newTime.GetYear(), newTime.GetMonth(), newTime.GetDay(),
		(int)0, (int)0, (int)0);
	m_dateTime_from.SetTime(setTime);
	m_dateTime_to.SetTime(setTime);
	m_dateTime_time_from.SetTime(setTime);
	m_dateTime_time_to.SetTime(setTime);

	//��ʼ��ʱ��ָ�
	m_combo_period.InsertString(0,L"һ����");
	m_combo_period.InsertString(1,L"�����");
	m_combo_period.InsertString(2,L"һСʱ");
	m_combo_period.InsertString(3,L"һ��");
	
	m_combo_period.SetItemDataPtr(0, (void *)(&period_m1));
	m_combo_period.SetItemDataPtr(1, (void *)(&period_m5));
	m_combo_period.SetItemDataPtr(2, (void *)(&period_h1));
	m_combo_period.SetItemDataPtr(3, (void *)(&period_d1));
	m_combo_period.SetCurSel(0);
	 
	//ˢ�µ������ؼ�
	RefreshListCtrl();
	
	//���ð�ť
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(FALSE);//���밴ť����
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CVPointHistoricalDataCalculation::OnBnClickedButtonCalculation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GridClear();
	UpdateData(TRUE);
	AfxBeginThread(Thread_Calculation,(LPVOID)this);//
}


void CVPointHistoricalDataCalculation::OnBnClickedButtonInsert()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (AfxMessageBox(L"�������ݿ���̲����棬��ȷ�ϡ�",MB_OKCANCEL) == IDOK)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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