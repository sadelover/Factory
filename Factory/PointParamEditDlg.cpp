// PointParamEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PointParamEditDlg.h"
#include "afxdialogex.h"
#include "DataPoint/DataPointEntry.h"
#include "DataPointConfigDlg.h"
#include "DataComConfigDlg.h"
#include "../ComponentDraw/EqmDrawDefines.h"
#include "MainFrm.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"
#include "PointEditAPISelect.h"
//#include "resource.h"
// CPointParamEditDlg �Ի���

IMPLEMENT_DYNAMIC(CPointParamEditDlg, CDialog)

CPointParamEditDlg::CPointParamEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointParamEditDlg::IDD, pParent)
	, m_strPhysicalID(_T(""))
	, m_strRemark(_T(""))
	, m_strParam1(_T(""))
	, m_strParam2(_T(""))
	, m_strParam3(_T(""))
	, m_strSource(_T(""))
	, m_strUnit(_T("0"))
	, m_strProperty(_T("R"))
	, m_strParam4(_T(""))
	, m_strParam5(_T(""))
	, m_strParam6(_T(""))
	, m_strParam7(_T(""))
	, m_strParam8(_T(""))
	, m_strParam9(_T(""))
	, m_strParam10(_T(""))
	, m_strParam11(_T(""))
	, m_strParam12(_T("0"))
	, m_strParam13(_T("0"))
	, m_strParam14(_T(""))
	, m_strDateTimeType(_T(""))
	, m_store_cycle(E_STORE_NULL)
	, m_str_store_cycle(_T(""))
	, m_pPointMap(NULL)
	, m_bIsVisited(FALSE)
	, m_strParam15(_T(""))
	, m_strParam16(_T(""))
	, m_strParam17(_T(""))
	, m_strParam18(_T(""))
{
	m_eType = enum_Edit;
}

CPointParamEditDlg::~CPointParamEditDlg()
{
}

void CPointParamEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DLG_PHYSICALID, m_strPhysicalID);
	DDX_Text(pDX, IDC_EDIT_DLG_REMARK, m_strRemark);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM1, m_strParam1);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM2, m_strParam2);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM3, m_strParam3);
	DDX_CBString(pDX, IDC_COMBO_DLG_SOURCE, m_strSource);
	DDX_CBString(pDX, IDC_COMBO_DLG_UNIT, m_strUnit);
	DDX_CBString(pDX, IDC_COMBO_DLG_PROPERTY, m_strProperty);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM4, m_strParam4);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM5, m_strParam5);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM6, m_strParam6);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM7, m_strParam7);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM8, m_strParam8);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM9, m_strParam9);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM10, m_strParam10);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM12, m_strParam12);
	DDX_Text(pDX, IDC_EDIT_DLG_PARAM13, m_strParam13);
	DDX_Control(pDX, IDC_STATIC_PARAM1, m_StaticParam1);
	DDX_Control(pDX, IDC_STATIC_PARAM2, m_StaticParam2);
	DDX_Control(pDX, IDC_STATIC_PARAM3, m_StaticParam3);
	DDX_Control(pDX, IDC_STATIC_PARAM4, m_StaticParam4);
	DDX_Control(pDX, IDC_STATIC_PARAM5, m_StaticParam5);
	DDX_Control(pDX, IDC_STATIC_PARAM6, m_StaticParam6);
	DDX_Control(pDX, IDC_STATIC_PARAM7, m_StaticParam7);
	DDX_Control(pDX, IDC_STATIC_PARAM8, m_StaticParam8);
	DDX_Control(pDX, IDC_STATIC_PARAM9, m_StaticParam9);
	DDX_Control(pDX, IDC_STATIC_PARAM10, m_StaticParam10);
	DDX_Control(pDX, IDC_STATIC_PARAM12, m_StaticParam12);
	DDX_Control(pDX, IDC_STATIC_PARAM13, m_StaticParam13);
	DDX_Control(pDX, IDC_COMBO_DLG_SOURCE, m_comboPointType);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM1, m_EditParam1);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM2, m_EditParam2);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM3, m_EditParam3);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM4, m_EditParam4);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM5, m_EditParam5);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM6, m_EditParam6);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM7, m_EditParam7);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM8, m_EditParam8);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM9, m_EditParam9);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM10, m_EditParam10);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM11, m_EditParam11);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM12, m_EditParam12);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM13, m_EditParam13);
	DDX_Control(pDX, IDC_EDIT_DLG_PARAM14, m_EditParam14);

	DDX_Control(pDX, IDC_COMBO3, m_combo_datetime_type);
	DDX_CBString(pDX, IDC_COMBO3, m_strDateTimeType);
	DDX_Control(pDX, IDC_COMBO_STORE_CYCLE, m_combo_store_cycle);
	DDX_Control(pDX, IDC_BTN_BIND_SET_MAX, m_btnBindSetPointMax);
	DDX_Control(pDX, IDC_BTN_BIND_SET_MIN, m_btnBindSetPointMin);
	DDX_Check(pDX, IDC_CHK_VISIT, m_bIsVisited);
	DDX_Text(pDX, IDC_EDIT_CUST_NAME, m_strParam15);
	DDX_Control(pDX, IDC_COMBO_SYS, m_comboSystem);
	DDX_Control(pDX, IDC_COMBO_DEV, m_comboDevice);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_CBString(pDX, IDC_COMBO_SYS, m_strParam16);
	DDX_CBString(pDX, IDC_COMBO_DEV, m_strParam17);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_strParam18);
	DDX_Control(pDX, IDC_modbusTypeSelect, m_cModbusTypeSelect);
}


BEGIN_MESSAGE_MAP(CPointParamEditDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPointParamEditDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_DLG_SOURCE, &CPointParamEditDlg::OnCbnSelchangeComboDlgSource)
	ON_CBN_SELCHANGE(IDC_COMBO_STORE_CYCLE, &CPointParamEditDlg::OnCbnSelchangeComboStoreCycle)
	ON_BN_CLICKED(IDC_BTN_BIND_SET_MAX, &CPointParamEditDlg::OnBnClickedBtnBindSetMax)
	ON_BN_CLICKED(IDC_BTN_BIND_SET_MIN, &CPointParamEditDlg::OnBnClickedBtnBindSetMin)
	ON_BN_CLICKED(IDC_BUTTON2, &CPointParamEditDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT_DLG_PARAM1, &CPointParamEditDlg::OnEnChangeEditDlgParam1)
	ON_EN_CHANGE(IDC_EDIT_DLG_PARAM12, &CPointParamEditDlg::OnEnChangeEditDlgParam12)
	ON_BN_CLICKED(IDC_BUTTON_API, &CPointParamEditDlg::OnBnClickedButtonApi)
	ON_CBN_SELCHANGE(IDC_modbusTypeSelect, &CPointParamEditDlg::OnCbnSelchangemodbustypeselect)
	ON_CBN_SELCHANGE(IDC_modbusTypeSelect, &CPointParamEditDlg::OnCbnSelchangemodbustypeselect)
	ON_BN_CLICKED(IDC_BUTTON_HISTORICAL_DATA_CALCULATION, &CPointParamEditDlg::OnBnClickedButtonHistoricalDataCalculation)
END_MESSAGE_MAP()


// CPointParamEditDlg ��Ϣ�������

/*
	��ʼ������
*/
BOOL CPointParamEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgTitle();
	{
		CRect rectBtn;
		GetDlgItem(IDOK)->GetWindowRect(&rectBtn);
		rectBtn.top	+= 15;
		rectBtn.bottom	+= 15;
		ScreenToClient(&rectBtn);
		GetDlgItem(IDOK)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	}
	{
		CRect rectBtn;
		GetDlgItem(IDCANCEL)->GetWindowRect(&rectBtn);
		rectBtn.top	+= 15;
		rectBtn.bottom	+= 15;
		ScreenToClient(&rectBtn);
		GetDlgItem(IDCANCEL)->MoveWindow(rectBtn.left, rectBtn.top, rectBtn.Width(), rectBtn.Height());
	}
	CenterWindow();
	InitMyControls();
	UpdateData(FALSE);
	if(m_strSource==_T(""))
		m_strSource = _T("vpoint");
	SetSource(m_strSource);
	SetUnit(m_strUnit);
	SetProperty(m_strProperty);
	SetEditPhysicalID();
	GetEditRect();
	InitModbusTypeSelect();//��ʼ��modbus���͵Ĳ���
	OnCbnSelchangeComboDlgSource();//���ݲ�ͬ���ͳ�ʼ������
	InitDateTimeControl();
	InitStoreCycle();
	
	if (m_strSource == L"dateTime")
	{
		m_strDateTimeType = m_strParam1;
		EnableNonDateTime(SW_HIDE);
		EnableDateTimeType(SW_SHOW);
	}
	m_strPhysicalIdOrig = m_strPhysicalID;
	UpdateData(FALSE);
	return TRUE;
}

void CPointParamEditDlg::InitMyControls()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(PH370);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(SIMENS1200);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(SIMENS300);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(SIMENS1200TCP);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(SIMENS300TCP);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(AB500);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(HONEYWELLEBI);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"modbus");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(MODBUSDIRECTREADINSERVER);
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"wireless");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"vpoint");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"bacnet");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"protocol104");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"lonworks");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"DB-Access");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"DB-SQLServer");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"DB-Oracle");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"DB-MySQL");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"custom1");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"custom2");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"custom3");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"custom4");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"custom5");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"dateTime");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"DanfossFCProtocol");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"Insight");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"WinCC1");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"KingView1");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"ArchestrA3");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"KEPware4");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"ABBScada");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"OPC");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"ModbusEquipment");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"ThirdParty");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"PersagyController");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"bacnet-mstp");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"OPCUA");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"bacnet-py");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"obix");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"logix");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->AddString(L"MoxaTCPServer");

	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->SetCurSel(11);

	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"%");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"kPa");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"��");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"h");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"Hz");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"V");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"Kw");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"l/s");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"KWh");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->AddString(L"m3/h");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->AddString(L"");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->AddString(L"R");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->AddString(L"W");
	//((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->AddString(L"R/W");
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->SetCurSel(0);
}

void CPointParamEditDlg::SetSource( CString strSource )
{
	const int nCount = ((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->GetCount();
	for (int i=0;i<nCount;++i)
	{
		CString strContent;
		((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->GetLBText(i,strContent);
		if (strContent.CompareNoCase(strSource) == 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->SetCurSel(i);
			return;
		}
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_SOURCE))->SetCurSel(0);
}

void CPointParamEditDlg::SetUnit( CString strUnit )
{
	const int nCount = ((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->GetCount();
	for (int i=0;i<nCount;++i)
	{
		CString strContent;
		((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->GetLBText(i,strContent);
		if (strContent.CompareNoCase(strUnit) == 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->SetCurSel(i);
			return;
		}
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_UNIT))->SetCurSel(0);
}

void CPointParamEditDlg::SetProperty( CString strProperty )
{
	const int nCount = ((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->GetCount();
	for (int i=0;i<nCount;++i)
	{
		CString strContent;
		((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->GetLBText(i,strContent);
		if (strContent.CompareNoCase(strProperty) == 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->SetCurSel(i);
			return;
		}
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_DLG_PROPERTY))->SetCurSel(0);
}

bool CPointParamEditDlg::CheckPointFormatValid(DataPointEntry entry)
{
	if(entry.GetSourceType()==L"bacnet")
	{
		CString strTemp;
		strTemp = entry.GetParam(2).c_str();
		if(strTemp!=_T("AI") && strTemp!=_T("AO") &&strTemp!=_T("BI") && strTemp!=_T("BO") && strTemp!=_T("AV") && strTemp!=_T("BV"))
		{
			AfxMessageBox(_T("bacnet�����Ͳ���ȷ!"));
			return false;
		}

		strTemp = entry.GetParam(1).c_str();
		if(strTemp.FindOneOf(_T("abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ .,;"))>=0 || strTemp.GetLength()==0)
		{
			AfxMessageBox(_T("Bacnet Server ID����Ϊ������!"));
			return false;
		}
		strTemp = entry.GetParam(3).c_str();
		if(strTemp.FindOneOf(_T("abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ .,;"))>=0 || strTemp.GetLength()==0)
		{
			AfxMessageBox(_T("����Ե�ַ����Ϊ������!"));
			return false;
		}
	}

	return true;
}


void CPointParamEditDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strPhysicalID;
	//bool bOK = false;
	GetDlgItem(IDC_EDIT_DLG_PHYSICALID)->GetWindowTextW(strPhysicalID);
	if (strPhysicalID.GetLength() == 0)
	{
		AfxMessageBox(L"PhysicalID����Ϊ��!");
		return;
	}
	//strPhysicalID = strPhysicalID.Left(1);
	if (_T(" ") == strPhysicalID.Left(1))
	{
		AfxMessageBox(L"PhysicalID���ַ�����Ϊ��!");
		return;
	}
	else
	{
		wchar_t wcFirst = strPhysicalID[0];
		if ((wcFirst>=L'a' && wcFirst<=L'z') || (wcFirst>=L'A' && wcFirst<=L'Z'))
		{
			//bOK = true;
		}
		else
		{
			AfxMessageBox(L"����ID������Ӣ���ַ���ͷ!");
			return;
		}

		for(int i=0;i<strPhysicalID.GetLength();i++)
		{
			int tsrt = strPhysicalID.GetLength();
			wchar_t wcOne = strPhysicalID.GetAt(i);
			if ((wcOne>=L'a' && wcOne<=L'z') || (wcOne>=L'A' && wcOne<=L'Z')
				|| wcOne=='_' || (wcOne>=L'0' && wcOne<=L'9'))
			{

			}
			else
			{
				AfxMessageBox(L"�������к��зǷ��ַ�(ֻ֧��Ӣ����ĸ�����������֡��»���!");
				return;
			}
		}
	}

	UpdateData(TRUE);

	if(m_strSource.GetLength() <= 0)
	{
		AfxMessageBox(L"����Դ����Ϊ��!");
		return;
	}

	m_store_cycle = (POINT_STORE_CYCLE)m_combo_store_cycle.GetCurSel();
	m_combo_store_cycle.GetLBText(m_combo_store_cycle.GetCurSel(), m_str_store_cycle);
	if (E_STORE_HALF_HOUR == m_store_cycle)
	{
		MessageBox(_T("�洢�����Ѳ�֧�ְ�Сʱ��\n������ѡ��"), _T("����"), MB_OK|MB_ICONERROR);
		return;
	}

	if (m_strSource == L"dateTime")
	{
		m_strParam1 = m_strDateTimeType;
	}
	UpdateData(FALSE);
	UpdateData(TRUE);
	CDataPointConfigDlg* pWnd = (CDataPointConfigDlg*)GetParent();
	ASSERT(pWnd);
	if (m_eType == enum_Add)
	{
		DataPointEntry entry;
		map<int, DataPointEntry>& datalist = pWnd->m_plcpointmap.GetPointMap();

		entry.SetPointIndex(GetMaxKeyID()+1);
		entry.SetShortName(m_strPhysicalID.GetString());
		entry.SetShortNameOrigin(m_strPhysicalID.GetString());
		entry.SetSourceType(m_strSource.GetString());
		entry.SetDescription(m_strRemark.GetString());
		entry.SetUnit(m_strUnit.GetString());



		if(m_strProperty == L"R")
		{
			entry.SetProperty(PLC_READ);
		}
		else if(m_strProperty == L"W")
		{
			entry.SetProperty(PLC_WRITE);
		}
		else if(m_strProperty == L"R/W")
		{
			entry.SetProperty(PLC_PROP_MAX);
		}
		entry.SetParam(1, m_strParam1.GetString());
		entry.SetParam(2, m_strParam2.GetString());
		entry.SetParam(3, m_strParam3.GetString());
		entry.SetParam(4, m_strParam4.GetString());
		entry.SetParam(5, m_strParam5.GetString());
		entry.SetParam(6, m_strParam6.GetString());
		entry.SetParam(7, m_strParam7.GetString());
		entry.SetParam(8, m_strParam8.GetString());
		entry.SetParam(9, m_strParam9.GetString());
		entry.SetParam(10, m_strParam10.GetString());
		entry.SetParam(11, m_strParam11.GetString());
		entry.SetParam(12, m_strParam12.GetString());
		entry.SetParam(13, m_strParam13.GetString());
		m_strParam14 = m_bIsVisited ? L"1" : L"0";
		entry.SetParam(14, m_strParam14.GetString());
		m_combo_store_cycle.SetCurSel(2);
		entry.SetStoreCycle((POINT_STORE_CYCLE)m_combo_store_cycle.GetCurSel());
		entry.SetParam(15, m_strParam15.GetString());
		entry.SetParam(16, m_strParam16.GetString());
		entry.SetParam(17, m_strParam17.GetString());
		entry.SetParam(18, m_strParam18.GetString());
		//if(!CheckPointFormatValid(entry))
		//	return;



		//���ID�Ƿ��ظ�
		map<int, DataPointEntry>::const_iterator it = datalist.begin();
		for (;it != datalist.end(); it++)
		{
			const DataPointEntry& entryMap = it->second;
			if (entryMap.GetShortName() == entry.GetShortName())
			{
				AfxMessageBox(L"PhysicalID�ظ�!");
				return;
			}
		}
		pWnd->AddOneEntry(entry);
	}
	else if (m_eType == enum_Edit)
	{
		map<int, DataPointEntry>& mapping = pWnd->m_plcpointmap.GetPointMap();
		map<int, DataPointEntry>::iterator it = mapping.begin();
		for (; it != mapping.end(); it++)
		{
			const DataPointEntry& entryMap = it->second;
			wstring wstrName		= entryMap.GetShortName();
			wstring	wstrPhyId		= m_strPhysicalID.GetString();
			wstring	wstrPhyIdOrig	= m_strPhysicalIdOrig.GetString();
			if (wstrName == wstrPhyId && wstrName != wstrPhyIdOrig)
			{
				AfxMessageBox(L"PhysicalID�ظ�!");
				return;
			}
		}

		it = mapping.begin();
		while (it != mapping.end())
		{
			if (it->second.GetShortName() == m_strPhysicalIdOrig.GetString())
			{
				it->second.SetShortName(m_strPhysicalID.GetString());
				it->second.SetShortNameOrigin(m_strPhysicalIdOrig.GetString());
				it->second.SetSourceType(m_strSource.GetString());
				it->second.SetDescription(m_strRemark.GetString());
				it->second.SetUnit(m_strUnit.GetString());
				if(m_strProperty == L"R")
				{
					it->second.SetProperty(PLC_READ);
				}
				else if(m_strProperty == L"W")
				{
					it->second.SetProperty(PLC_WRITE);
				}
				else if(m_strProperty == L"R/W")
				{
					it->second.SetProperty(PLC_PROP_MAX);
				}
				it->second.SetParam(1, m_strParam1.GetString());
				it->second.SetParam(2, m_strParam2.GetString());

				//if (SIMENS1200TCP == m_strSource || SIMENS300TCP == m_strSource)
				//{
				//	if (!CheckIpAddr(m_strParam3.GetString()))
				//	{
				//		GetDlgItem(IDC_EDIT_DLG_PARAM3)->SetFocus();
				//		((CEdit*)GetDlgItem(IDC_EDIT_DLG_PARAM3))->SetSel(0, -1);
				//		MessageBox(_T("SIMENS1200/300TCP�����ַ��ʽ����ȷ!"), _T("����"), MB_OK|MB_ICONSTOP);
				//		return;
				//	}
				//}
				//else if (L"modbus" == m_strSource)
				//{
				//	if (!CheckIpAddr(m_strParam5.GetString()))
				//	{
				//		GetDlgItem(IDC_EDIT_DLG_PARAM5)->SetFocus();
				//		((CEdit*)GetDlgItem(IDC_EDIT_DLG_PARAM5))->SetSel(0, -1);
				//		MessageBox(_T("modbus�����ַ��ʽ����ȷ!"), _T("����"), MB_OK|MB_ICONSTOP);
				//		return;
				//	}
				//}

				it->second.SetParam(3, m_strParam3.GetString());
				it->second.SetParam(4, m_strParam4.GetString());
				it->second.SetParam(5, m_strParam5.GetString());
				it->second.SetParam(6, m_strParam6.GetString());
				it->second.SetParam(7, m_strParam7.GetString());
				it->second.SetParam(8, m_strParam8.GetString());
				it->second.SetParam(9, m_strParam9.GetString());
				it->second.SetParam(10, m_strParam10.GetString());
				it->second.SetParam(11, m_strParam11.GetString());
				it->second.SetParam(12, m_strParam12.GetString());
				it->second.SetParam(13, m_strParam13.GetString());
				m_strParam14 = m_bIsVisited ? L"1" : L"0";
				it->second.SetParam(14, m_strParam14.GetString());

				it->second.SetStoreCycle((POINT_STORE_CYCLE)m_combo_store_cycle.GetCurSel());
				it->second.SetParam(15, m_strParam15.GetString());
				it->second.SetParam(16, m_strParam16.GetString());
				it->second.SetParam(17, m_strParam17.GetString());
				it->second.SetParam(18, m_strParam18.GetString());
				break;
			}
			++it;
		}
	}
	else
	{
		ASSERT(0);
	}

	return CDialog::OnOK();
}

void CPointParamEditDlg::SetDlgType( DLG_TYPE eType )
{
	m_eType = eType;
}

void CPointParamEditDlg::SetDlgTitle()
{
	if (m_eType == enum_Add)
	{
		SetWindowText(L"���ӵ�");
	}
	else if (m_eType == enum_Edit)
	{
		SetWindowText(L"�༭��");
	}
	else
	{
		ASSERT(0);
	}
}

void CPointParamEditDlg::SetEditPhysicalID()
{
	if (m_eType == enum_Add)
	{
		GetDlgItem(IDC_EDIT_DLG_PHYSICALID)->EnableWindow(TRUE);
	}
	else if (m_eType == enum_Edit)
	{
		GetDlgItem(IDC_EDIT_DLG_PHYSICALID)->EnableWindow(TRUE);
	}
	else
	{
		ASSERT(0);
	}
}

/*
����Դ�������͵�ѡ��仯�¼�
*/
void CPointParamEditDlg::OnCbnSelchangeComboDlgSource()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);//����ѡ��ѡ������Ǵ򿪶���ȡ����ʱ�����pointˢ�´���
	int ncur = m_comboPointType.GetCurSel();
	if(ncur<0)
		return;

	CString strTemp;
	m_comboPointType.GetLBText(ncur, strTemp);
	SetDefaultParamText();
	if(strTemp==_T("bacnet"))
	{
		m_StaticParam1.SetWindowText(L"Bacnet Server ID(0-255)");
		m_StaticParam2.SetWindowText(L"������(AI/AO/BI/BO/AV/BV)");
		m_StaticParam3.SetWindowText(L"����Ե�ַ(0-1000)");
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");	

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if(strTemp==SIMENS1200 || strTemp==SIMENS300)
	{
		m_StaticParam1.SetWindowText(L"PLC���Ե�ַ");
		m_StaticParam2.SetWindowText(L"������(VT_BOOL/VT_I2/VT_R4...)");
		m_StaticParam3.SetWindowText(L"����ӳ��(0:��;1:��;2:δ֪)");
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");
		m_StaticParam5.SetWindowText(L"����Դ����");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if (strTemp==SIMENS1200TCP || strTemp==SIMENS300TCP)
	{
		m_StaticParam1.SetWindowText(L"PLC���Ե�ַ");
		m_StaticParam2.SetWindowText(L"������(VT_BOOL/VT_I2/VT_R4...)");
		m_StaticParam3.SetWindowText(L"PLC TCP/IP�����ַ");
		m_StaticParam4.SetWindowText(L"slack");
		m_StaticParam5.SetWindowText(L"slot");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(TRUE);

		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if(strTemp== PH370 || strTemp==AB500 || strTemp==HONEYWELLEBI)
	{
		m_StaticParam1.SetWindowText(L"OPC������");
		m_StaticParam2.SetWindowText(L"������(VT_BOOL/VT_I2/VT_R4...)");
		m_StaticParam3.SetWindowText(L"����ӳ��(0:��;1:��;2:δ֪)");
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");

		m_EditParam4.EnableWindow(TRUE);
		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if(strTemp==_T("modbus"))
	{
		m_StaticParam1.SetWindowText(L"MODBUS ��ַ��");
		m_StaticParam2.SetWindowText(L"���ַ��");
		m_StaticParam3.SetWindowText(L"���ܺ�(Ĭ��03)");
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");
		m_StaticParam5.SetWindowText(L"MOXA IP��ַ(��192.168.0.1)");
		m_StaticParam6.SetWindowText(L"����");
		m_StaticParam7.SetWindowText(L"���ݳ���");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(FALSE);
		m_EditParam7.EnableWindow(TRUE);

		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);
				
		m_EditParam6.EnableWindow(FALSE);
		m_EditParam7.EnableWindow(FALSE);

		m_EditParam6.ShowWindow(SW_HIDE);
		m_cModbusTypeSelect.SetCurSel(_ttoi(m_strParam6));
		OnCbnSelchangemodbustypeselect();
		m_cModbusTypeSelect.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if(strTemp==MODBUSDIRECTREADINSERVER)
	{
		m_StaticParam1.SetWindowText(L"MODBUS ��ַ��");
		m_StaticParam2.SetWindowText(L"���ַ��");
		m_StaticParam3.SetWindowText(L"���ܺ�(Ĭ��03)");
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");
		m_StaticParam5.SetWindowText(L"MOXA IP��ַ(��192.168.0.1)");
		m_StaticParam6.SetWindowText(L"����");
		m_StaticParam7.SetWindowText(L"���ݳ���");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(TRUE);
		m_EditParam7.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if(strTemp==L"wireless")
	{
		m_StaticParam1.SetWindowText(L"��ַ��");
		m_StaticParam2.SetWindowText(L"��Ч����");
		m_StaticParam3.SetWindowText(L"��Ч����");
		m_StaticParam4.SetWindowText(L"��Ч����");
		m_StaticParam5.SetWindowText(L"��Ч����");
		m_StaticParam6.SetWindowText(L"��Ч����");
		m_StaticParam7.SetWindowText(L"��Ч����");

		m_EditParam1.EnableWindow(FALSE);
		m_EditParam2.EnableWindow(FALSE);
		m_EditParam3.EnableWindow(FALSE);
		m_EditParam4.EnableWindow(FALSE);
		m_EditParam5.EnableWindow(FALSE);
		m_EditParam6.EnableWindow(FALSE);
		m_EditParam7.EnableWindow(FALSE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if(strTemp=="protocol104")
	{
		m_StaticParam1.SetWindowText(L"104������IP");
		m_StaticParam2.SetWindowText(L"104�������˿�");
		m_StaticParam3.SetWindowText(L"֡���ͺ�(ң��Ĭ�� 6)");
		m_StaticParam4.SetWindowText(L"���");
		m_StaticParam5.SetWindowText(L"��������ʱ����");
		m_StaticParam6.SetWindowText(L"��ֵ����");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if(strTemp=="vpoint")
	{
		m_StaticParam1.SetWindowText(L"�߼��﷨�ű������磺 1 if <%PointName%>==1 else 0");
		m_StaticParam2.SetWindowText(L"��������ִ������(��)");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
	
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_SHOW);
	}
	else if (strTemp=="dateTime")
	{
		EnableNonDateTime(SW_HIDE);
		EnableDateTimeType(SW_SHOW);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if (strTemp=="DanfossFCProtocol")
	{
		m_StaticParam1.SetWindowText(_T("IP��ַ"));
		m_StaticParam2.SetWindowText(_T("�˿�"));
		m_StaticParam3.SetWindowText(_T("PLC��ַ"));
		m_StaticParam4.SetWindowText(_T("����(����ֵ=�ɼ�ֵ/����)"));

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if (strTemp=="Insight" || strTemp=="WinCC1" || strTemp=="KingView1" || strTemp=="ArchestrA3" || strTemp=="KEPware4" || strTemp=="ABBScada")
	{
		m_StaticParam1.SetWindowText(_T("����"));
		m_StaticParam2.SetWindowText(_T("������(VT_BOOL/VT_I2/VT_R4...)"));
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");
		m_StaticParam5.SetWindowText(L"����Դ����");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if (strTemp=="OPC")
	{
		m_StaticParam1.SetWindowText(_T("����"));
		m_StaticParam2.SetWindowText(_T("������(VT_BOOL/VT_I2/VT_R4...)"));
		m_StaticParam3.SetWindowText(_T("OPC������"));
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");
		m_StaticParam5.SetWindowText(L"����Դ����");
		m_StaticParam6.SetWindowText(L"OPCServer IP");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(TRUE);

		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if (strTemp=="OPCUA")
	{
		m_StaticParam1.SetWindowText(_T("����"));
		m_StaticParam2.SetWindowText(_T("������(VT_BOOL/VT_I2/VT_R4...)"));
		m_StaticParam3.SetWindowText(_T("OPC������"));
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");
		m_StaticParam5.SetWindowText(L"����Դ����");
		m_StaticParam6.SetWindowText(L"OPCServer IP");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if (strTemp=="obix")
	{
		m_StaticParam1.SetWindowText(_T("����"));
		m_StaticParam2.SetWindowText(_T("������(VT_BOOL/VT_I2/VT_R4...)"));
		m_StaticParam3.SetWindowText(_T("obix�����ַ"));
		m_StaticParam4.SetWindowText(L"obix������֤�û���");
		m_StaticParam5.SetWindowText(L"obix������֤����");
		m_StaticParam6.SetWindowText(L"OPCServer IP");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(TRUE);

		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else if (strTemp=="MoxaTCPServer")
	{
		m_StaticParam1.SetWindowText(L"MODBUS ��ַ��");
		m_StaticParam2.SetWindowText(L"���ַ��");
		m_StaticParam3.SetWindowText(L"���ܺ�(Ĭ��03)");
		m_StaticParam4.SetWindowText(L"����(����ֵ=�ɼ�ֵ/����)");
		m_StaticParam5.SetWindowText(L"MOXA IP��ַ(��192.168.0.1)");
		m_StaticParam6.SetWindowText(L"����");
		m_StaticParam7.SetWindowText(L"���ݳ���");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(FALSE);
		m_EditParam7.EnableWindow(TRUE);

		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_EditParam6.EnableWindow(FALSE);
		m_EditParam7.EnableWindow(FALSE);

		m_EditParam6.ShowWindow(SW_HIDE);
		m_cModbusTypeSelect.SetCurSel(_ttoi(m_strParam6));
		OnCbnSelchangemodbustypeselect();
		m_cModbusTypeSelect.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_StaticParam1.SetWindowText(L"Ԥ������1");
		m_StaticParam2.SetWindowText(L"Ԥ������2");
		m_StaticParam3.SetWindowText(L"Ԥ������3");
		m_StaticParam4.SetWindowText(L"Ԥ������4");
		m_StaticParam5.SetWindowText(L"Ԥ������5");
		m_StaticParam6.SetWindowText(L"Ԥ������6");
		m_StaticParam7.SetWindowText(L"Ԥ������7");
		m_StaticParam8.SetWindowText(L"Ԥ������8");
		m_StaticParam9.SetWindowText(L"Ԥ������9");
		m_StaticParam10.SetWindowText(L"Ԥ������10");

		m_EditParam1.EnableWindow(TRUE);
		m_EditParam2.EnableWindow(TRUE);
		m_EditParam3.EnableWindow(TRUE);
		m_EditParam4.EnableWindow(TRUE);
		m_EditParam5.EnableWindow(TRUE);
		m_EditParam6.EnableWindow(TRUE);
		m_EditParam7.EnableWindow(TRUE);
		m_EditParam8.EnableWindow(TRUE);
		m_EditParam9.EnableWindow(TRUE);
		m_EditParam10.EnableWindow(TRUE);
		
		EnableNonDateTime(SW_SHOW);
		EnableDateTimeType(SW_HIDE);

		m_cModbusTypeSelect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_API)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_HISTORICAL_DATA_CALCULATION)->ShowWindow(SW_HIDE);
	}
}


void CPointParamEditDlg::SetDefaultParamText()
{
	m_StaticParam1.SetWindowText(L"Ԥ������1");
	m_StaticParam2.SetWindowText(L"Ԥ������2");
	m_StaticParam3.SetWindowText(L"Ԥ������3");
	m_StaticParam4.SetWindowText(L"Ԥ������4");
	m_StaticParam5.SetWindowText(L"Ԥ������5");
	m_StaticParam6.SetWindowText(L"Ԥ������6");
	m_StaticParam7.SetWindowText(L"Ԥ������7");
	m_StaticParam8.SetWindowText(L"Ԥ������8");
	m_StaticParam9.SetWindowText(L"Ԥ������9");
	m_StaticParam10.SetWindowText(L"Ԥ������10");
	m_StaticParam12.SetWindowText(L"���ֵ");
	m_StaticParam13.SetWindowText(L"��Сֵ");
	m_EditParam1.EnableWindow(FALSE);
	m_EditParam2.EnableWindow(FALSE);
	m_EditParam3.EnableWindow(FALSE);
	m_EditParam4.EnableWindow(FALSE);
	m_EditParam5.EnableWindow(FALSE);
	m_EditParam6.EnableWindow(FALSE);
	m_EditParam7.EnableWindow(FALSE);
	m_EditParam8.EnableWindow(FALSE);
	m_EditParam9.EnableWindow(FALSE);
	m_EditParam10.EnableWindow(FALSE);
	m_EditParam11.EnableWindow(FALSE);
	m_EditParam12.EnableWindow(TRUE);
	m_EditParam13.EnableWindow(TRUE);
	m_EditParam14.EnableWindow(FALSE);
}

int CPointParamEditDlg::GetMaxKeyID()
{
	int nMaxID = 0;
	CDataPointConfigDlg* pWnd = (CDataPointConfigDlg*)GetParent();
	ASSERT(pWnd);
	map<int, DataPointEntry>& datalist = pWnd->m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::iterator iter = datalist.begin();
	for (;iter!=datalist.end();++iter)
	{
		if (iter->first > nMaxID)
		{
			nMaxID = iter->first;
		}
	}
	return nMaxID;
}

void CPointParamEditDlg::InitDateTimeControl()
{
	m_combo_datetime_type.AddString(L"hour:minute");
	m_combo_datetime_type.AddString(L"hour:minute:second");
	m_combo_datetime_type.AddString(L"year:month");
	m_combo_datetime_type.AddString(L"year:month:day");
	m_combo_datetime_type.AddString(L"year:month:day hour");
	m_combo_datetime_type.AddString(L"year:month:day hour:minute:second");
	m_combo_datetime_type.SetCurSel(0);
}

void CPointParamEditDlg::EnableNonDateTime( const int nShow )
{
	GetDlgItem(IDC_STATIC_PARAM1)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM2)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM3)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM4)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM5)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM6)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM7)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM8)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM9)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM10)->ShowWindow(nShow);
	GetDlgItem(IDC_STATIC_PARAM11)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PARAM12)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PARAM13)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PARAM14)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_EDIT_DLG_PARAM1)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM2)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM3)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM4)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM5)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM6)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM7)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM8)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM9)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM10)->ShowWindow(nShow);
	GetDlgItem(IDC_EDIT_DLG_PARAM11)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_DLG_PARAM12)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_DLG_PARAM13)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_DLG_PARAM14)->ShowWindow(SW_HIDE);
	

}

void CPointParamEditDlg::EnableDateTimeType( const int nShow )
{
	RECT rect;
	GetDlgItem(IDC_STATIC_PARAM1)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_DATETIME_TYPE)->MoveWindow(&rect);
	GetDlgItem(IDC_EDIT_DLG_PARAM1)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(IDC_COMBO_DATETIME_TYPE)->MoveWindow(&rect);
	GetDlgItem(IDC_STATIC_DATETIME_TYPE)->ShowWindow(nShow);
	GetDlgItem(IDC_COMBO_DATETIME_TYPE)->ShowWindow(nShow);
}

void CPointParamEditDlg::InitStoreCycle()
{
	m_combo_store_cycle.AddString(L"���洢");
	m_combo_store_cycle.AddString(L"5����");
	m_combo_store_cycle.AddString(L"1����");
	m_combo_store_cycle.AddString(L"5����");
	m_combo_store_cycle.AddString(L"��Сʱ");
	m_combo_store_cycle.AddString(L"1Сʱ");
	m_combo_store_cycle.AddString(L"1��");
	m_combo_store_cycle.AddString(L"1�ܣ�7�죩");
	m_combo_store_cycle.AddString(L"1����");
	m_combo_store_cycle.AddString(L"1��");

	int nCurSel = 2;
	if(m_str_store_cycle.GetLength()>0)
	{
		for(int i=0;i<m_combo_store_cycle.GetCount();i++)
		{
			CString strItem;
			m_combo_store_cycle.GetLBText(i, strItem);
			if(strItem==m_str_store_cycle)
			{
				nCurSel = i;
				break;
			}
		}
	}
	
	m_combo_store_cycle.SetCurSel(nCurSel);
	
}


void CPointParamEditDlg::OnCbnSelchangeComboStoreCycle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}

bool CPointParamEditDlg::CheckIpAddr(const CString strIp)
{
	CString strIpFlag(_T("abcdefghijklmnopqrstuvwxyzABVDEFGHIJKLMNOPQRSTUVWXYZ ,;"));
	int		nLen = strIp.GetLength();
	int		nDotCnt = 0;


	for (int i=0; i<nLen; ++i)
	{
		if (_T('.') == strIp[i])
		{
			++nDotCnt;
		}
	}

	if (strIp.FindOneOf(strIpFlag) >= 0 || 0 == nLen || nDotCnt != 3)
	{
		return false;
	}

	return true;
}

//void CPointParamEditDlg::SetBtnBindSetPointPos(const BindSetPointType& bindType)
//{
//	const int nBindBtnWidth = 39;
//
//	switch (bindType)
//	{
//	case bindPoint_normal:
//		{
//			m_btnBindSetPointMax.SetWindowPos(NULL, m_rtEdit5.right - nBindBtnWidth, m_rtEdit5.top, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMin.SetWindowPos(NULL, m_rtEdit6.right - nBindBtnWidth, m_rtEdit6.top, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMax.ShowWindow(SW_SHOW);
//			m_btnBindSetPointMin.ShowWindow(SW_SHOW);
//
//			m_EditParam5.SetWindowPos(NULL, m_rtEdit5.left, m_rtEdit5.top, m_rtEdit5.Width() - 40, m_rtEdit5.Height(), SWP_FRAMECHANGED);
//			m_EditParam6.SetWindowPos(NULL, m_rtEdit6.left, m_rtEdit6.top, m_rtEdit6.Width() - 40, m_rtEdit6.Height(), SWP_FRAMECHANGED);
//			
//			m_EditParam2.SetWindowPos(NULL, m_rtEdit2.left, m_rtEdit2.top, m_rtEdit2.Width(), m_rtEdit2.Height(), SWP_FRAMECHANGED);
//			m_EditParam3.SetWindowPos(NULL, m_rtEdit3.left, m_rtEdit3.top, m_rtEdit3.Width(), m_rtEdit3.Height(), SWP_FRAMECHANGED);
//			m_EditParam7.SetWindowPos(NULL, m_rtEdit7.left, m_rtEdit7.top, m_rtEdit7.Width(), m_rtEdit7.Height(), SWP_FRAMECHANGED);
//
//			m_pointType = bindPoint_normal;
//		}
//		break;
//	case bindPoint_modbus:
//		{
//			m_btnBindSetPointMax.SetWindowPos(NULL, m_rtEdit6.right - nBindBtnWidth, m_rtEdit6.top, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMin.SetWindowPos(NULL, m_rtEdit7.right - nBindBtnWidth, m_rtEdit7.top, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMax.ShowWindow(SW_SHOW);
//			m_btnBindSetPointMin.ShowWindow(SW_SHOW);
//
//			m_EditParam6.SetWindowPos(NULL, m_rtEdit6.left, m_rtEdit6.top, m_rtEdit6.Width() - 40, m_rtEdit6.Height(), SWP_FRAMECHANGED);
//			m_EditParam7.SetWindowPos(NULL, m_rtEdit7.left, m_rtEdit7.top, m_rtEdit7.Width() - 40, m_rtEdit7.Height(), SWP_FRAMECHANGED);
//
//			m_EditParam2.SetWindowPos(NULL, m_rtEdit2.left, m_rtEdit2.top, m_rtEdit2.Width(), m_rtEdit2.Height(), SWP_FRAMECHANGED);
//			m_EditParam3.SetWindowPos(NULL, m_rtEdit3.left, m_rtEdit3.top, m_rtEdit3.Width(), m_rtEdit3.Height(), SWP_FRAMECHANGED);
//			m_EditParam5.SetWindowPos(NULL, m_rtEdit5.left, m_rtEdit5.top, m_rtEdit5.Width(), m_rtEdit5.Height(), SWP_FRAMECHANGED);
//
//			m_pointType = bindPoint_modbus;
//		}
//		break;
//	case bindPoint_vpoint:
//		{
//			m_btnBindSetPointMax.SetWindowPos(NULL, m_rtEdit2.right - nBindBtnWidth, m_rtEdit2.top, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMin.SetWindowPos(NULL, m_rtEdit3.right - nBindBtnWidth, m_rtEdit3.top, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMax.ShowWindow(SW_SHOW);
//			m_btnBindSetPointMin.ShowWindow(SW_SHOW);
//
//			m_EditParam2.SetWindowPos(NULL, m_rtEdit2.left, m_rtEdit2.top, m_rtEdit2.Width() - 40, m_rtEdit2.Height(), SWP_FRAMECHANGED);
//			m_EditParam3.SetWindowPos(NULL, m_rtEdit3.left, m_rtEdit3.top, m_rtEdit3.Width() - 40, m_rtEdit3.Height(), SWP_FRAMECHANGED);
//
//			m_EditParam5.SetWindowPos(NULL, m_rtEdit5.left, m_rtEdit5.top, m_rtEdit5.Width(), m_rtEdit5.Height(), SWP_FRAMECHANGED);
//			m_EditParam6.SetWindowPos(NULL, m_rtEdit6.left, m_rtEdit6.top, m_rtEdit6.Width(), m_rtEdit6.Height(), SWP_FRAMECHANGED);
//			m_EditParam7.SetWindowPos(NULL, m_rtEdit7.left, m_rtEdit7.top, m_rtEdit7.Width(), m_rtEdit7.Height(), SWP_FRAMECHANGED);
//
//			m_pointType = bindPoint_vpoint;
//		}
//		break;
//	case bindPoint_noShow:
//		{
//			m_btnBindSetPointMax.SetWindowPos(NULL, m_rtStoreCycle.right - nBindBtnWidth, m_rtStoreCycle.bottom + 30, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMin.SetWindowPos(NULL, m_rtStoreCycle.right - nBindBtnWidth, m_rtStoreCycle.bottom + 60, 0, 0, SWP_NOSIZE);
//			m_btnBindSetPointMax.ShowWindow(SW_HIDE);
//			m_btnBindSetPointMin.ShowWindow(SW_HIDE);
//
//			m_EditParam2.SetWindowPos(NULL, m_rtEdit2.left, m_rtEdit2.top, m_rtEdit2.Width(), m_rtEdit2.Height(), SWP_FRAMECHANGED);
//			m_EditParam3.SetWindowPos(NULL, m_rtEdit3.left, m_rtEdit3.top, m_rtEdit3.Width(), m_rtEdit3.Height(), SWP_FRAMECHANGED);
//			m_EditParam5.SetWindowPos(NULL, m_rtEdit5.left, m_rtEdit5.top, m_rtEdit5.Width(), m_rtEdit5.Height(), SWP_FRAMECHANGED);
//			m_EditParam6.SetWindowPos(NULL, m_rtEdit6.left, m_rtEdit6.top, m_rtEdit6.Width(), m_rtEdit6.Height(), SWP_FRAMECHANGED);
//			m_EditParam7.SetWindowPos(NULL, m_rtEdit7.left, m_rtEdit7.top, m_rtEdit7.Width(), m_rtEdit7.Height(), SWP_FRAMECHANGED);
//
//			m_pointType = bindPoint_noShow;
//		}
//		break;
//	default:
//		break;
//	}
//}

void CPointParamEditDlg::GetEditRect()
{
	m_EditParam2.GetWindowRect(m_rtEdit2);
	m_EditParam3.GetWindowRect(m_rtEdit3);
	m_EditParam5.GetWindowRect(m_rtEdit5);
	m_EditParam6.GetWindowRect(m_rtEdit6);
	m_EditParam7.GetWindowRect(m_rtEdit7);
	m_combo_store_cycle.GetWindowRect(m_rtStoreCycle);

	ScreenToClient(m_rtEdit2);
	ScreenToClient(m_rtEdit3);
	ScreenToClient(m_rtEdit5);
	ScreenToClient(m_rtEdit6);
	ScreenToClient(m_rtEdit7);
	ScreenToClient(m_rtStoreCycle);
}

void CPointParamEditDlg::SetPointMap(CDataPointManager* pPointMap)
{
	ASSERT(pPointMap != NULL);
	if (pPointMap != NULL)
	{
		m_pPointMap = pPointMap;
	}
}

void CPointParamEditDlg::OnBnClickedBtnBindSetMax()
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pPointMap != NULL);
	if (NULL == m_pPointMap)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pPointMap);
	if (IDOK == dlg.DoModal())
	{
		m_strParam12 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPointParamEditDlg::OnBnClickedBtnBindSetMin()
{
	// TODO: Add your control notification handler code here
	ASSERT(m_pPointMap != NULL);
	if (NULL == m_pPointMap)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pPointMap);
	if (IDOK == dlg.DoModal())
	{
		m_strParam13 = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CPointParamEditDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ã�/tool/evalStringExpression
	//	str = m_strParam12''
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(pFrame->m_strCalcIp, pFrame->m_nCalcPort, _T("/tool/evalStringExpression"));
	wstring wstrTimeStart,wstrTimeEnd ;

	Json::Value myRoot;


	myRoot["str"] = Project::Tools::WideCharToUtf8(m_strParam1.GetString());

	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));//
	if(bRet)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_utf8, myReturnRoot))
		{
			AfxMessageBox(_T("��׼�����Զ�����ʧ�ܣ�ԭ����ע����Ϣ�����"));
			return;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value dataObj = myReturnRoot["err"];
				if(dataObj.isInt())
				{
					int nErr = dataObj.asInt();
					if(nErr==1)
					{
						CString strTemp;
						strTemp.Format(_T("��ʽ��ʽ���󣬼���ʧ��"));
						AfxMessageBox(strTemp);
					}
					else
					{
						Json::Value dataObj = myReturnRoot["data"];
						if(dataObj.isDouble())
						{
							CString strTemp;
							strTemp.Format(_T("���Ϊ:%.4f"), dataObj.asDouble());
							AfxMessageBox(strTemp);
						}
						else if (dataObj.isInt())
						{
							CString strTemp;
							strTemp.Format(_T("���Ϊ:%d"), dataObj.asInt());
							AfxMessageBox(strTemp);
						}else if (dataObj.isString())
						{
							CString strTemp;
							wstring wstr = Project::Tools::AnsiToWideChar(dataObj.asString().data());
							strTemp.Format(_T("���Ϊ:%s"), wstr.c_str());
							AfxMessageBox(strTemp);
						}
					}
					
				}
				else{
					CString strTemp;
					strTemp.Format(_T("��̨���󣬼���ʧ��"));
					AfxMessageBox(strTemp);
				}
				
			}
		}
	}
	else
	{
		AfxMessageBox(_T("��ʽ����ʧ��"));
		return;
	}

}


void CPointParamEditDlg::OnEnChangeEditDlgParam1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPointParamEditDlg::OnEnChangeEditDlgParam12()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPointParamEditDlg::OnBnClickedButtonApi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CPointEditAPISelect dlg;
	if(dlg.DoModal())
	{
		if(dlg.m_strAPIFunction.GetLength()>1)
		{
			if (m_strParam1.GetLength()== 0)
				m_strParam1 = dlg.m_strAPIFunction;
			else
				m_strParam1 = m_strParam1 + L"\r\n" +dlg.m_strAPIFunction;
			UpdateData(FALSE);
		}
	}
}
void CPointParamEditDlg::InitModbusTypeSelect()
{
	m_cModbusTypeSelect.InsertString(0, _T("Signed"));		//�з���
	m_cModbusTypeSelect.InsertString(1 ,_T("Unsigned"));		//�޷���
	m_cModbusTypeSelect.InsertString(2, _T("Bit"));			//������
	m_cModbusTypeSelect.InsertString(3, _T("Long"));			//������
	m_cModbusTypeSelect.InsertString(4, _T("Long Inverse"));	//�����͸ߵ�λ����
	m_cModbusTypeSelect.InsertString(5, _T("Float"));			//������
	m_cModbusTypeSelect.InsertString(6, _T("FloatInverese"));	//�����͸ߵ�λ����
	m_cModbusTypeSelect.InsertString(7, _T("Double"));		//˫����
	m_cModbusTypeSelect.InsertString(8, _T("DoubleInverse"));	//˫���ȸߵ�λ����
	m_cModbusTypeSelect.InsertString(9, _T("String"));		//�ַ���
	m_cModbusTypeSelect.InsertString(10,_T("StringInverse"));	//�ַ����ߵ�λ����

}



/************************************************************************/
/* modbusģʽ����ѡ�����ݸ����¼�                                     */
/************************************************************************/
void CPointParamEditDlg::OnCbnSelchangemodbustypeselect()
{
	m_StaticParam7.SetWindowText(L"���ݳ���");
	m_EditParam7.EnableWindow(FALSE);
	//Signed����
	if (m_cModbusTypeSelect.GetCurSel() == 0)
	{
		m_strParam6 = _T("0");
		m_strParam7 = _T("1");
	}
	//Unsigned����
	else if (m_cModbusTypeSelect.GetCurSel() == 1)
	{
		m_strParam6 = _T("1");
		m_strParam7 = _T("1");
	}
	//Bit����
	else if (m_cModbusTypeSelect.GetCurSel() == 2)
	{
		m_strParam6 = _T("2");
		//m_strParam7 = _T("0-15");
		m_StaticParam7.SetWindowText(L"ȡ�ڼ�λ(0-15)");
		m_EditParam7.EnableWindow(TRUE);
	}
	//Long����
	else if (m_cModbusTypeSelect.GetCurSel() == 3)
	{
		m_strParam6 = _T("3");
		m_strParam7 = _T("2");
	}
	//Long Inverse �������͸ߵ�λ����
	else if (m_cModbusTypeSelect.GetCurSel() == 4)
	{
		m_strParam6 = _T("4");
		m_strParam7 = _T("2");
	}
	//Float ��������
	else if (m_cModbusTypeSelect.GetCurSel() == 5)
	{
		m_strParam6 = _T("5");
		m_strParam7 = _T("2");
	}
	//FloatInverese �������͸ߵ�λ����
	else if (m_cModbusTypeSelect.GetCurSel() == 6)
	{
		m_strParam6 = _T("6");
		m_strParam7 = _T("2");
	}
	//Double ��˫����
	else if (m_cModbusTypeSelect.GetCurSel() == 7)
	{
		m_strParam6 = _T("7");
		m_strParam7 = _T("4");
	}
	//DoubleInverse ��˫���ȸߵ�λ����
	else if (m_cModbusTypeSelect.GetCurSel() == 8)
	{
		m_strParam6 = _T("8");
		m_strParam7 = _T("4");
	}
	//String : �ַ���
	else if (m_cModbusTypeSelect.GetCurSel() == 9)
	{
		m_strParam6 = _T("9");
		m_strParam7 = _T("N");
	}
	//StringInverse : �ַ����ߵ�λ����
	else if (m_cModbusTypeSelect.GetCurSel() == 10)
	{
		m_strParam6 = _T("10");
		m_strParam7 = _T("N");
	}
	UpdateData(FALSE);
}

#include "VPointHistoricalDataCalculation.h"
void CPointParamEditDlg::OnBnClickedButtonHistoricalDataCalculation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_strSource != L"vpoint"){
		AfxMessageBox(L"ֻ��vpoint���͵�ſ��Լ��㡣");
		return;
	}
	else if(m_strParam1.GetLength() == 0){
		AfxMessageBox(L"�ű�����Ϊ�ա�");
		return;
	}
	else if (m_strPhysicalID.GetLength() == 0){
		AfxMessageBox(L"��������Ϊ�ա�");
		return;
	}
	
	CVPointHistoricalDataCalculation dlg(m_strPhysicalID, m_strParam1, m_pPointMap);
	dlg.DoModal();
}
