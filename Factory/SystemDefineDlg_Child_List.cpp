// SystemDefineDlg_Child_List.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SystemDefineDlg_Child_List.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include <regex>
#include "Tools/Tools_String.h"
#include<sstream>

// CSystemDefineDlg_Child_List �Ի���

IMPLEMENT_DYNAMIC(CSystemDefineDlg_Child_List, CDialogEx)

CSystemDefineDlg_Child_List::CSystemDefineDlg_Child_List(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemDefineDlg_Child_List::IDD, pParent),
	m_JsonValue_SysDefine()
	, m_cstrPriChWPGroupList(_T(""))
	, m_cstrRoomName(_T(""))
	, m_cstrChGroupList(_T(""))
	, m_cstrCWPGroupList(_T(""))
	, m_cstrCTGroupList(_T(""))
	, m_cstrChCapacityList(_T(""))
	, m_cstrGlycolPumpGroupList(_T(""))
	, m_cstrIsChillerConnectGlycolPumpOneToOne(_T(""))
	, m_cstrIsChillerConnectIceHXOneToOne(_T(""))
	, m_cstrBaseChillerList(_T(""))
	, m_cstrIceHXGroupList(_T(""))
	, m_cstrExceptChGroupList(_T(""))
	, m_cstrPreferChGroupList(_T(""))
	, m_cstrACArea(_T(""))
	,m_ComboBox_Index(-1)
	, m_cstrSecChWPGroupList(_T(""))
{

}

CSystemDefineDlg_Child_List::~CSystemDefineDlg_Child_List()
{
}

int CSystemDefineDlg_Child_List::SetData(CString & rCstr)
{
	ResetAll();
	if(rCstr.GetLength() == 0)
		return true;
	Json::Reader JsonReader;
	if(JsonReader.parse(Project::Tools::WideCharToUtf8(rCstr.GetString()), m_JsonValue_SysDefine))
	{
		if(m_JsonValue_SysDefine.isMember("ChillerPlantRoom") == true)
		{
			m_JsonValue_ChillerPlantRoom = m_JsonValue_SysDefine["ChillerPlantRoom"];
			if(m_JsonValue_ChillerPlantRoom.isArray() == true)
			{
				for(unsigned int itr = 0; itr < m_JsonValue_ChillerPlantRoom.size();itr++)
				{
					CString temp = Project::Tools::UTF8ToWideChar(m_JsonValue_ChillerPlantRoom[itr]["RoomName"].asString().c_str()).c_str();
					m_ComboBox_RoomName.InsertString(itr, temp);
				}
				Interface_TurnPage(0);
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return -1;
	}
	UpdateData(FALSE);
	return 0;
}

void CSystemDefineDlg_Child_List::GetData(CString & rCstr)
{
	if(m_JsonValue_ChillerPlantRoom.isNull() == false)
		m_JsonValue_SysDefine["ChillerPlantRoom"] = m_JsonValue_ChillerPlantRoom;
	string strSysDefine = m_JsonValue_SysDefine.toStyledString();	
	wstring wstrSysDefine = Project::Tools::UTF8ToWideChar(strSysDefine.c_str());
	rCstr = wstrSysDefine.c_str();
}

void CSystemDefineDlg_Child_List::Interface_SetParam(unsigned int nIndex)
{
	Json::Value &rItem = m_JsonValue_ChillerPlantRoom[nIndex];
	if(rItem["PriChWPGroupList"].isString())
		m_cstrPriChWPGroupList = Project::Tools::UTF8ToWideChar(rItem["PriChWPGroupList"].asString().c_str()).c_str();

	if(rItem["RoomName"].isString())
		m_cstrRoomName = Project::Tools::UTF8ToWideChar(rItem["RoomName"].asString().c_str()).c_str();

	if(rItem["ChGroupList"].isString())
		m_cstrChGroupList = Project::Tools::UTF8ToWideChar(rItem["ChGroupList"].asString().c_str()).c_str();

	if(rItem["CWPGroupList"].isString())
		m_cstrCWPGroupList = Project::Tools::UTF8ToWideChar(rItem["CWPGroupList"].asString().c_str()).c_str();

	if(rItem["CTGroupList"].isString())
		m_cstrCTGroupList = Project::Tools::UTF8ToWideChar(rItem["CTGroupList"].asString().c_str()).c_str();

	if(rItem["ChCapacityList"].isString())
		m_cstrChCapacityList = Project::Tools::UTF8ToWideChar(rItem["ChCapacityList"].asString().c_str()).c_str();

	if(rItem["GlycolPumpGroupList"].isString())
		m_cstrGlycolPumpGroupList = Project::Tools::UTF8ToWideChar(rItem["GlycolPumpGroupList"].asString().c_str()).c_str();


	if(rItem["IsChillerConnectGlycolPumpOneToOne"].isString())
		m_cstrIsChillerConnectGlycolPumpOneToOne = Project::Tools::UTF8ToWideChar(rItem["IsChillerConnectGlycolPumpOneToOne"].asString().c_str()).c_str();

	if(rItem["IsChillerConnectIceHXOneToOne"].isString())
		m_cstrIsChillerConnectIceHXOneToOne = Project::Tools::UTF8ToWideChar(rItem["IsChillerConnectIceHXOneToOne"].asString().c_str()).c_str();

	if(rItem["BaseChillerList"].isString())
		m_cstrBaseChillerList = Project::Tools::UTF8ToWideChar(rItem["BaseChillerList"].asString().c_str()).c_str();


	if(rItem["IceHXGroupList"].isString())
		m_cstrIceHXGroupList = Project::Tools::UTF8ToWideChar(rItem["IceHXGroupList"].asString().c_str()).c_str();

	if(rItem["ExceptChGroupList"].isString())
		m_cstrExceptChGroupList = Project::Tools::UTF8ToWideChar(rItem["ExceptChGroupList"].asString().c_str()).c_str();

	if(rItem["PreferChGroupList"].isString())
		m_cstrPreferChGroupList = Project::Tools::UTF8ToWideChar(rItem["PreferChGroupList"].asString().c_str()).c_str();

	if(rItem["ACArea"].isString())
		m_cstrACArea = Project::Tools::UTF8ToWideChar(rItem["ACArea"].asString().c_str()).c_str();

	if(rItem["SecChWPGroupList"].isString())
		m_cstrSecChWPGroupList = Project::Tools::UTF8ToWideChar(rItem["SecChWPGroupList"].asString().c_str()).c_str();
}

void CSystemDefineDlg_Child_List::Interface_TurnPage(int nIndex)
{
	m_ComboBox_Index = nIndex;
	m_ComboBox_RoomName.SetCurSel(nIndex);
	Interface_SetParam(nIndex);
}

void CSystemDefineDlg_Child_List::ResetAll(void)
{
	m_ComboBox_Index = -1;
	m_ComboBox_RoomName.Clear();
	m_ComboBox_RoomName.ResetContent();

	m_JsonValue_SysDefine.clear();
	m_JsonValue_ChillerPlantRoom.clear();

	m_cstrPriChWPGroupList = L"";
	// ��������
	m_cstrRoomName= L"";
	// �����
	m_cstrChGroupList= L"";
	// ��ȴ����
	m_cstrCWPGroupList= L"";
	// ��ȴ����
	m_cstrCTGroupList= L"";
	// ���������kW��
	m_cstrChCapacityList= L"";
	// �Ҷ�������
	m_cstrGlycolPumpGroupList= L"";
	// ������Ҷ������Ƿ�Ϊһһ��Ӧ��1-�ǣ�0-��
	m_cstrIsChillerConnectGlycolPumpOneToOne= L"";
	// ���������廻�Ƿ�Ϊһһ��Ӧ��1-�ǣ�0-��
	m_cstrIsChillerConnectIceHXOneToOne= L"";
	// ����������
	m_cstrBaseChillerList= L"";
	// ����廻��
	m_cstrIceHXGroupList= L"";
	// ������һ�����е�������
	m_cstrExceptChGroupList= L"";
	// �������е�������
	m_cstrPreferChGroupList= L"";
	// �յ������m2��
	m_cstrACArea= L"";
	// �����䶳����
	m_cstrSecChWPGroupList = L"";
	UpdateData(FALSE);
}

void CSystemDefineDlg_Child_List::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PriChWPGroupList, m_cstrPriChWPGroupList);
	DDX_Text(pDX, IDC_EDIT_RoomName,m_cstrRoomName);
	DDX_Text(pDX, IDC_EDIT_ChGroupList,m_cstrChGroupList);
	DDX_Text(pDX, IDC_EDIT_CWPGroupList,m_cstrCWPGroupList);
	DDX_Text(pDX, IDC_EDIT_CTGroupList,m_cstrCTGroupList);
	DDX_Text(pDX, IDC_EDIT_ChCapacityList,m_cstrChCapacityList);
	DDX_Text(pDX, IDC_EDIT_GlycolPumpGroupList,m_cstrGlycolPumpGroupList);
	DDX_Text(pDX, IDC_EDIT_IsChillerConnectGlycolPumpOneToOne,m_cstrIsChillerConnectGlycolPumpOneToOne);
	DDX_Text(pDX, IDC_EDIT_IsChillerConnectIceHXOneToOne,m_cstrIsChillerConnectIceHXOneToOne);
	DDX_Text(pDX, IDC_EDIT_BaseChillerList,m_cstrBaseChillerList);
	DDX_Text(pDX, IDC_EDIT_IceHXGroupList,m_cstrIceHXGroupList);
	DDX_Text(pDX, IDC_EDIT_ExceptChGroupList,m_cstrExceptChGroupList);
	DDX_Text(pDX, IDC_EDIT_PreferChGroupList,m_cstrPreferChGroupList);
	DDX_Text(pDX, IDC_EDIT_ACArea,m_cstrACArea);

	DDX_Control(pDX, IDC_COMBO_RommNameList, m_ComboBox_RoomName);
	DDX_Text(pDX, IDC_EDIT_SecChWPGroupList, m_cstrSecChWPGroupList);
}


BEGIN_MESSAGE_MAP(CSystemDefineDlg_Child_List, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_RommNameList, &CSystemDefineDlg_Child_List::OnCbnSelchangeComboRommnamelist)
	ON_BN_CLICKED(IDC_BUTTON_NewItem, &CSystemDefineDlg_Child_List::OnBnClickedButtonNewitem)
	ON_BN_CLICKED(IDC_BUTTON_DeleteItem, &CSystemDefineDlg_Child_List::OnBnClickedButtonDeleteitem)
	ON_EN_CHANGE(IDC_EDIT_PriChWPGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditPrichwpgrouplist)
	ON_EN_CHANGE(IDC_EDIT_RoomName, &CSystemDefineDlg_Child_List::OnEnChangeEditRoomname)
	ON_EN_CHANGE(IDC_EDIT_ChGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditChgrouplist)
	ON_EN_CHANGE(IDC_EDIT_CWPGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditCwpgrouplist)
	ON_EN_CHANGE(IDC_EDIT_CTGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditCtgrouplist)
	ON_EN_CHANGE(IDC_EDIT_ChCapacityList, &CSystemDefineDlg_Child_List::OnEnChangeEditChcapacitylist)
	ON_EN_CHANGE(IDC_EDIT_IsChillerConnectGlycolPumpOneToOne, &CSystemDefineDlg_Child_List::OnEnChangeEditIschillerconnectglycolpumponetoone)
	ON_EN_CHANGE(IDC_EDIT_IsChillerConnectIceHXOneToOne, &CSystemDefineDlg_Child_List::OnEnChangeEditIschillerconnecticehxonetoone)
	ON_EN_CHANGE(IDC_EDIT_GlycolPumpGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditGlycolpumpgrouplist)
	ON_EN_CHANGE(IDC_EDIT_BaseChillerList, &CSystemDefineDlg_Child_List::OnEnChangeEditBasechillerlist)
	ON_EN_CHANGE(IDC_EDIT_IceHXGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditIcehxgrouplist)
	ON_EN_CHANGE(IDC_EDIT_ExceptChGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditExceptchgrouplist)
	ON_EN_CHANGE(IDC_EDIT_PreferChGroupList, &CSystemDefineDlg_Child_List::OnEnChangeEditPreferchgrouplist)
	ON_EN_CHANGE(IDC_EDIT_ACArea, &CSystemDefineDlg_Child_List::OnEnChangeEditAcarea)
	ON_EN_CHANGE(IDC_EDIT_SecChWPGroupList, &CSystemDefineDlg_Child_List::OnEnSecChWPGroupList)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CSystemDefineDlg_Child_List ��Ϣ�������


void CSystemDefineDlg_Child_List::OnCbnSelchangeComboRommnamelist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_ComboBox_RoomName.GetCurSel() != m_ComboBox_Index)
		Interface_TurnPage(m_ComboBox_RoomName.GetCurSel());
	UpdateData(FALSE);
}


void CSystemDefineDlg_Child_List::OnBnClickedButtonNewitem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//�½�һ����ΪRoomName+���֣���������б�����û������������ɸѡ�������ҳ���������
	int nRoomNameNumber = 1;
	vector<int> nList;
	std::wregex reg(L"RoomName\\d*");
	for(int itr = 0; itr < m_ComboBox_RoomName.GetCount(); itr++)
	{
		CString cstrTemp;
		m_ComboBox_RoomName.GetLBText(itr,cstrTemp);
		if(regex_match(cstrTemp.GetString(),reg))
		{
			CString seven = cstrTemp.Mid(8,cstrTemp.GetLength()-1);
			nList.push_back(stoi(seven.GetString()));
		}
	}
	if(nList.size() != 0)
	{
		for(int i=0;i<nList.size();i++)
		{
			if(nRoomNameNumber<nList[i])
				nRoomNameNumber = nList[i];
		}
		nRoomNameNumber+=1;
	}

	

	stringstream ss;
	ss<<"RoomName"<<nRoomNameNumber;
	string strRoomName = ss.str();

	//�½�һ������
	Json::Value jvItem;
	jvItem["PriChWPGroupList"] = "";
	jvItem["RoomName"] = strRoomName;
	jvItem["ChGroupList"] = "";
	jvItem["CWPGroupList"] = "";
	jvItem["CTGroupList"] = "";
	jvItem["ChCapacityList"] = "";
	jvItem["GlycolPumpGroupList"] = "";
	jvItem["IsChillerConnectGlycolPumpOneToOne"] = "";
	jvItem["IsChillerConnectIceHXOneToOne"] = "";
	jvItem["BaseChillerList"] = "";
	jvItem["IceHXGroupList"] = "";
	jvItem["ExceptChGroupList"] = "";
	jvItem["PreferChGroupList"] = "";
	jvItem["ACArea"] = "";
	jvItem["SecChWPGroupList"] = "";
	m_JsonValue_ChillerPlantRoom.append(jvItem);

	//�����б������һ��������ҳ�����µġ�
	int nIndex = m_ComboBox_RoomName.GetCount();
	CString cstrTemp = Project::Tools::UTF8ToWideChar(strRoomName.c_str()).c_str();
	m_ComboBox_RoomName.InsertString(nIndex, cstrTemp);
	Interface_TurnPage(nIndex);
	UpdateData(FALSE);
}


void CSystemDefineDlg_Child_List::OnBnClickedButtonDeleteitem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_ComboBox_RoomName.GetCount() == 0)
		return;
	int nIndex = m_ComboBox_RoomName.GetCurSel();
	m_JsonValue_ChillerPlantRoom.isremove(nIndex);
	string a = m_JsonValue_ChillerPlantRoom.toStyledString();
	wstring as = Project::Tools::UTF8ToWideChar(a.c_str());
	m_ComboBox_RoomName.Clear();//�����Ͽ�ѡ��
	m_ComboBox_RoomName.DeleteString(nIndex);
	if(m_ComboBox_RoomName.GetCount() != 0)
	{
		if(nIndex!=0)
			Interface_TurnPage(nIndex-1);
		else
			Interface_TurnPage(nIndex);
	}
	else
	{
		m_cstrPriChWPGroupList = L"";
		// ��������
		m_cstrRoomName= L"";
		// �����
		m_cstrChGroupList= L"";
		// ��ȴ����
		m_cstrCWPGroupList= L"";
		// ��ȴ����
		m_cstrCTGroupList= L"";
		// ���������kW��
		m_cstrChCapacityList= L"";
		// �Ҷ�������
		m_cstrGlycolPumpGroupList= L"";
		// ������Ҷ������Ƿ�Ϊһһ��Ӧ��1-�ǣ�0-��
		m_cstrIsChillerConnectGlycolPumpOneToOne= L"";
		// ���������廻�Ƿ�Ϊһһ��Ӧ��1-�ǣ�0-��
		m_cstrIsChillerConnectIceHXOneToOne= L"";
		// ����������
		m_cstrBaseChillerList= L"";
		// ����廻��
		m_cstrIceHXGroupList= L"";
		// ������һ�����е�������
		m_cstrExceptChGroupList= L"";
		// �������е�������
		m_cstrPreferChGroupList= L"";
		// �յ������m2��
		m_cstrACArea= L"";
		// �����䶳����
		m_cstrSecChWPGroupList = L"";
	}
	UpdateData(FALSE);
}

void CSystemDefineDlg_Child_List::OnEnChangeEditPrichwpgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["PriChWPGroupList"] = Project::Tools::WideCharToUtf8(m_cstrPriChWPGroupList.GetString());
	}
	else
	{
		m_cstrPriChWPGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditRoomname()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["RoomName"] = Project::Tools::WideCharToUtf8(m_cstrRoomName.GetString());
		//Ϊ����������Ͽ�ѡ�е��ַ�����û�к��ʵķ�����ֻ��ɾ��������ѡ��
		m_ComboBox_RoomName.DeleteString(nIndex);
		m_ComboBox_RoomName.InsertString(nIndex,m_cstrRoomName);
		m_ComboBox_RoomName.SetCurSel(nIndex);
	}
	else
	{
		
		m_cstrRoomName=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditChgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["ChGroupList"] = Project::Tools::WideCharToUtf8(m_cstrChGroupList.GetString());
	}
	else
	{
		
		m_cstrChGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditCwpgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["CWPGroupList"] = Project::Tools::WideCharToUtf8(m_cstrCWPGroupList.GetString());
	}
	else
	{
		m_cstrCWPGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditCtgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["CTGroupList"] = Project::Tools::WideCharToUtf8(m_cstrCTGroupList.GetString());
	}
	else
	{
		m_cstrCTGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditChcapacitylist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["ChCapacityList"] = Project::Tools::WideCharToUtf8(m_cstrChCapacityList.GetString());
	}
	else
	{
		m_cstrChCapacityList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditIschillerconnectglycolpumponetoone()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["IsChillerConnectGlycolPumpOneToOne"] = Project::Tools::WideCharToUtf8(m_cstrIsChillerConnectGlycolPumpOneToOne.GetString());
	}
	else
	{
		m_cstrIsChillerConnectGlycolPumpOneToOne=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditIschillerconnecticehxonetoone()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["IsChillerConnectIceHXOneToOne"] = Project::Tools::WideCharToUtf8(m_cstrIsChillerConnectIceHXOneToOne.GetString());
	}
	else
	{
		m_cstrIsChillerConnectIceHXOneToOne=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}

//?
void CSystemDefineDlg_Child_List::OnEnChangeEditGlycolpumpgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["GlycolPumpGroupList"] = Project::Tools::WideCharToUtf8(m_cstrGlycolPumpGroupList.GetString());
	}
	else
	{
		m_cstrGlycolPumpGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditBasechillerlist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["BaseChillerList"] = Project::Tools::WideCharToUtf8(m_cstrBaseChillerList.GetString());
	}
	else
	{
		m_cstrBaseChillerList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditIcehxgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["IceHXGroupList"] = Project::Tools::WideCharToUtf8(m_cstrIceHXGroupList.GetString());
	}
	else
	{
		m_cstrIceHXGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditExceptchgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["ExceptChGroupList"] = Project::Tools::WideCharToUtf8(m_cstrExceptChGroupList.GetString());
	}
	else
	{
		m_cstrExceptChGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditPreferchgrouplist()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["PreferChGroupList"] = Project::Tools::WideCharToUtf8(m_cstrPreferChGroupList.GetString());
	}
	else
	{
		m_cstrPreferChGroupList=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}


void CSystemDefineDlg_Child_List::OnEnChangeEditAcarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["ACArea"] = Project::Tools::WideCharToUtf8(m_cstrACArea.GetString());
	}
	else
	{
		m_cstrACArea=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}

void CSystemDefineDlg_Child_List::OnEnSecChWPGroupList()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if((m_ComboBox_RoomName.GetCount()>0) && (m_JsonValue_ChillerPlantRoom.size()>0))
	{
		UpdateData(TRUE);
		int nIndex = m_ComboBox_RoomName.GetCurSel();
		m_JsonValue_ChillerPlantRoom[nIndex]["SecChWPGroupList"] = Project::Tools::WideCharToUtf8(m_cstrSecChWPGroupList.GetString());
	}
	else
	{
		m_cstrACArea=L"";
		UpdateData(FALSE);
		AfxMessageBox(L"���½�һ��\"����\"��");
	}
}

BOOL CSystemDefineDlg_Child_List::PreTranslateMessage(MSG* pMsg)
{
	//�ж��Ƿ�Ϊ������Ϣ
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//�ж��Ƿ��¼���Enter��
		if(pMsg->wParam==VK_ESCAPE)
		{

			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CSystemDefineDlg_Child_List::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(nChar == VK_ESCAPE)
	{
		return;
	}
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CSystemDefineDlg_Child_List::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nChar == VK_ESCAPE)
	{
		return;
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
