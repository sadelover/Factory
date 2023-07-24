// PaneArbiter.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "PaneArbiter.h"
#include "afxdialogex.h"
#include "VECTOR"
#include "Tools/warning/MsgDefine.h"
#include "ControlEx/WarningDlg.h"
#include "LogicBase.h"
#include "../ComponentDraw/sqlite/SqliteAcess.h"
#include "sstream"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "SetConstValueDlg.h"
#include "Tools/Util/UtilString.h"
#include "../DB_BasicIO/WarningConfigItem.h"
#include "SetParameterDlg.h"
#include "BEOP-Designer.h"
#include "BEOPLogicManager.h"
#include "DLLObject.h"
#include "MainFrm.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "InputLogicNameDlg.h"
#include "LogicDllThread.h"
#include "LogicParameter.h"
#include "ModifyRelationDlg.h"
#include "AddDllThreadNameDlg.h"
#include "BEOP-DesignerView.h"
#include "TreeManageDlg.h"
#include "SystemTactics.h"
#include "TacticLogQueryDlg.h"


#include "../ServerDataAccess/ServerDataAccess.h"
#include <sstream>
#include "Tools/Util/UtilString.h"
#include "SelectPointDlg.h"
#include "ControlEx/WarningDlg.h"
#include "SetConstValueDlg.h"
#include "BEOPLogicManager.h"
#include "InputLogicPeriodDlg.h"
#include "ReplacePointName.h"


#include "atlstr.h"  
#include "LogicHistoryRunDlg.h"
#include "LogicParameterPoint.h"
#include "LogicParameterConst.h"
#include "JSON/json.h"

#define  ONE_MIMUTE 60000		//1���� = 60000����

using namespace std;
// CPaneArbiter �Ի���


IMPLEMENT_DYNAMIC(CPaneArbiter, CDialogEx)

CPaneArbiter::CPaneArbiter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPaneArbiter::IDD, pParent)
	, m_strLogicRelation(_T(""))
	, m_nThreadNum(0)
	, m_nThreadCurSelNum(-1)
	, m_pLogicManager(NULL)
	, m_bIsLstTitleInit(false)
{
	m_iListCount =0;

	m_bWatchPointUpdate = false;

	m_pDataAccess = NULL;
	//////////////////////////////////////////////////////////////////////////
}

CPaneArbiter::~CPaneArbiter()
{
	if(m_pLogicManager)
	{
		delete(m_pLogicManager);
		m_pLogicManager = NULL;
	}
}
void CPaneArbiter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ControlLogic, m_ControlLogicList);
	DDX_Text(pDX, IDC_EDIT_RELATIONSHIP, m_strLogicRelation);
	DDX_Text(pDX, IDC_STATIC_LogicName, m_strCurSelName);
}


BEGIN_MESSAGE_MAP(CPaneArbiter, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPaneArbiter::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPaneArbiter::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOADCAL, &CPaneArbiter::OnBnClickedButtonLoadcal)
	ON_BN_CLICKED(IDC_BUTTONGATEWAY_UNLOADCAL, &CPaneArbiter::OnBnClickedButtongatewayUnloadcal)

	ON_WM_TIMER()

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ControlLogic, &CPaneArbiter::OnNMDblclkListControllogic)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ControlLogic, &CPaneArbiter::OnNMClickListControllogic)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTCONFIG, &CPaneArbiter::OnBnClickedButtonImportconfig)
	ON_BN_CLICKED(IDC_BUTTON_Config, &CPaneArbiter::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_RELATION, &CPaneArbiter::OnBnClickedButtonModifyRelation)
	ON_BN_CLICKED(IDC_BTN_LOAD_SYS_CAL, &CPaneArbiter::OnBnClickedBtnLoadSysCal)
	ON_BN_CLICKED(IDC_BTN_CHANGE_NAME, &CPaneArbiter::OnBnClickedBtnChangeName)
	ON_BN_CLICKED(IDC_BTN_QUERY_LOG, &CPaneArbiter::OnBnClickedBtnQueryLog)
	ON_BN_CLICKED(IDC_BTN_HISTORYTEST, &CPaneArbiter::OnBnClickedBtnHistorytest)
	ON_BN_CLICKED(IDC_BTN_COPY_LOGIC, &CPaneArbiter::OnBnClickedBtnCopyLogic)
	ON_EN_CHANGE(IDC_EDIT_RELATIONSHIP, &CPaneArbiter::OnEnChangeEditRelationship)
	/*
	//ON_BN_CLICKED(IDC_BUTTON_ADD_FLOW, &CPaneArbiter::OnBnClickedButtonAddFlow)
	//ON_BN_CLICKED(IDC_BUTTON_DEL_FLOW, &CPaneArbiter::OnBnClickedButtonDelFlow)
	//ON_BN_CLICKED(IDC_BUTTON_RENAME_FLOW, &CPaneArbiter::OnBnClickedButtonRenameFlow)
	//ON_BN_CLICKED(IDC_BUTTON_Cal_RunServer, &CPaneArbiter::OnBnClickedButtonCalRunserver)
	ON_BN_CLICKED(IDC_BUTTON_Cal_PauseServer, &CPaneArbiter::OnBnClickedButtonCalPauseserver)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_LOGICDLL, &CPaneArbiter::OnBnClickedButtonRefreshLogicdll)
	*/
END_MESSAGE_MAP()

// CPaneArbiter ��Ϣ�������
void CPaneArbiter::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CDialogEx::OnOK();
}
void CPaneArbiter::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//	CDialogEx::OnCancel();
}

/*
void CPaneArbiter::OnBnClickedButtonUploadStatus()
{

	for (int i=0;i<m_pLogicManager->vecImportDLLList.size();++i)
	{
		bool bRun = m_pLogicManager->vecImportDLLList[i]->GetDllRunStatus();
		wstring strValue = m_pDataAccess->ReadOrCreateCoreDebugItemValue(m_pLogicManager->vecImportDLLList[i]->GetDllName().GetString());
		if(bRun && strValue==L"1")
			;
		else if(!bRun && strValue==L"0")
			;
		else
		{
			m_pDataAccess->WriteCoreDebugItemValue(m_pLogicManager->vecImportDLLList[i]->GetDllName().GetString(), bRun?L"1":L"0");
		}

		m_ControlLogicList.SetItemText(i, 6, bRun?L"On":L"Off");

		m_pLogicManager->vecImportDLLList[i]->SetDllRunStatus(false); //ֹͣ���ز�������
		m_ControlLogicList.SetItemText(i, 1, L"Off");

	}

}*/
/*
	�½��Զ������
*/
void CPaneArbiter::OnBnClickedButtonLoadcal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strthreadName(m_strCurSelName);
	if (_T("") == m_strCurSelName)
	{
		MessageBox(_T("����ѡ���̣߳�"), _T("����"), MB_ICONERROR);
		return;
	}
	strthreadName = strthreadName.Left(strthreadName.Find(':'));


	std::vector<CString> vecSelDllName;
	std::vector<CString> vecOriginDllName;
	std::vector<CString> vecSelDllPath;
	std::vector<CDLLObject*> vecReImportDLLList;
	m_pLogicManager->m_vecSelDllName_Rep.clear();
	m_pLogicManager->m_vecDllPath_Rep.clear();

	CString strFileName;
	CFileDialog dlgOpenFile(TRUE,NULL,L"*.dll",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"Dll Files (*.dll)|*.dll|");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		strFileName = dlgOpenFile.GetFileTitle();

		CString strTacticName;
		CInputLogicNameDlg dlgInputName(true);
		dlgInputName.m_strLogicName = strFileName;
		if(dlgInputName.DoModal()==IDOK)
		{
			strTacticName = dlgInputName.m_strLogicName;
			if(m_pLogicManager->FindDllNameIsExist(strTacticName))
			{
				AfxMessageBox(L"�����Ѵ��ڣ�����ʧ��!");
				return;
			}
		}
		else
		{
			return;
		}


		POSITION ps = dlgOpenFile.GetStartPosition();	
		while (ps !=NULL)
		{
			//��ȡȫ·��
			strFileName = dlgOpenFile.GetNextPathName(ps);
			vecSelDllName.push_back(strTacticName + _T(".dll"));	// ������+".dll"
			vecSelDllPath.push_back(strFileName);					// dllȫ·��+����
			vecOriginDllName.push_back(dlgOpenFile.GetFileTitle());	// dll��������".dll"��
		}
	}
//////////////////////////////////////////////////////////////////////////
	//����Ҫ���滻��dll�б�Ϊʱ,��ɾ��
	if (vecSelDllName.empty() && m_pLogicManager->m_vecSelDllName_Rep.empty() )
	{
		return ;
	}

	Sleep(1000);
	
	if(m_pLogicManager->LoadAndSaveDll(vecSelDllPath,vecSelDllName,vecOriginDllName,strthreadName))
	{
		DisplayDllName(vecSelDllName);

		if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
		{
			for(int i= 0; i<vecSelDllPath.size(); ++i)
			{
				CString strDllPath = vecSelDllPath[i];
				CString strDllOldName = strDllPath.Right(strDllPath.GetLength() - strDllPath.ReverseFind('\\') - 1);
				CString strDllName = vecSelDllName[i];
				if (!m_pDataAccess->UpdateLibInsertIntoFilestorageCal(strDllPath, strDllOldName, strDllName))
				{
					MessageBox(_T("�����filestorageʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				}

				if (!m_pDataAccess->UpdateLibInsertIntoUnit02Cal(L"4",strthreadName.GetString(),strDllName.GetString(),strDllName.GetString(),L"",L"",L"",L"",L"",L"",L"",strDllOldName.GetString(), m_pDataAccess->GetServerAddress()!=L"127.0.0.1" &&m_pDataAccess->GetServerAddress()!=L"localhost"  ))
				{
					MessageBox(_T("�����unit02ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//��dll���뵽m_ControlLogicList��
	//���ԭ�����б����б�Dll,��嵽��λ�ã�����嵽���
	//DisplayDllName(vecSelDllName);
	RefreshDLLsOfThread();
	return;

}

/************************************************************************/
/*  ��һ��DLL��ʾ�ڽ�����                                               */
/************************************************************************/
bool CPaneArbiter::DisplayDllName(std::vector<CString> &vecSelDllName)
{
	int i=0,j=0;
	for (i=0;i<vecSelDllName.size();++i)
	{
		CString dllname = vecSelDllName[i];
		CString displayname = dllname.Left(dllname.GetLength()-4);

		bool IShave = false;
		int  IndexPos = -1;
		for(j =0;j<m_ControlLogicList.GetItemCount();++j)
		{
			CString csText = m_ControlLogicList.GetItemText(j,0);

			if (csText == displayname)
			{
				IndexPos = j;
				IShave = true;
				break;
			}
		}
		if (IShave)
		{
			m_iListCount = IndexPos;
		}
		else
			m_iListCount = m_ControlLogicList.InsertItem(m_ControlLogicList.GetItemCount(), _T(""));

		SYSTEMTIME sysTime;
		CString strTime;
		::GetLocalTime(&sysTime);
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d  "), sysTime.wYear, sysTime.wMonth,sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		CString  strData = _T("����") + displayname + _T("����ɹ�!");
		MessageBox(strData, _T("�ɹ�"), MB_OK|MB_ICONASTERISK);
		//vecCurImportDLLList���ε���dll�����Ϣ
		CDLLObject *curImportDLL = m_pLogicManager->GetDLLObjectByName(dllname);

		m_ControlLogicList.SetItemText(m_iListCount, 0, displayname);
		m_ControlLogicList.SetItemText(m_iListCount, 1, curImportDLL->GetDllOriginName());
		
		m_ControlLogicList.SetItemText(m_iListCount, 2, curImportDLL->GetDllImportTime());
		m_ControlLogicList.SetItemText(m_iListCount, 3,  curImportDLL->GetDllAuthor());
		//�汾��
		m_ControlLogicList.SetItemText(m_iListCount, 4, curImportDLL->GetDllVersion());
		//����
		m_ControlLogicList.SetItemText(m_iListCount, 5, curImportDLL->GetDllDescription());
	}

	return true;
}

/*
	ɾ������
*/
void CPaneArbiter::OnBnClickedButtongatewayUnloadcal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strthreadName(m_strCurSelName);
	if (_T("") == m_strCurSelName)
	{
		MessageBox(_T("����ѡ����ԣ�"), _T("����"), MB_ICONERROR);
		return;
	}
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	m_pLogicManager->m_vecSelDllName.clear();
	UINT iSeclectSum = m_ControlLogicList.GetSelectedCount();
	POSITION pt = m_ControlLogicList.GetFirstSelectedItemPosition();
	for (UINT i = 0; i < iSeclectSum; i++)
	{
		CString csText = _T("");
		int iTem = m_ControlLogicList.GetNextSelectedItem(pt);
		csText = m_ControlLogicList.GetItemText(iTem, 0);
		csText += _T(".dll");

		m_pLogicManager->m_vecSelDllName.push_back(csText);  
	}
	if (m_pLogicManager->m_vecSelDllName.empty())
	{
		return ;
	}
	if (IDYES != MessageBox(_T("ȷ��ɾ����"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION))
	{
		return;
	}
	//ж�ض�̬�⣬�ȹرո�dll����ص���
	std::vector<int>  veciDelete_Dll_Index;
	for (int k= 0; k<m_pLogicManager->m_vecSelDllName.size();++k)
	{
		int Index_dll = -1;
		for (int i=0;i<m_pLogicManager->vecImportDLLList.size();++i)
		{
			if (m_pLogicManager->vecImportDLLList[i]->GetDllName() == m_pLogicManager->m_vecSelDllName[k])
			{
				Index_dll = i;
				break;
			}
		}
		if (Index_dll<0)
		{
			continue;
		}
		
			int n = m_ControlLogicList.GetItemCount();
			for (int i=0;i<n;++i)
			{
				CString cur_item = m_ControlLogicList.GetItemText(i, 0);
				cur_item+= _T(".dll");
				if (m_pLogicManager->vecImportDLLList[Index_dll]->GetDllName() == cur_item)
				{
					m_ControlLogicList.DeleteItem(i);
					break;
				}
			}

			m_pLogicManager->ExitDllByName(m_pLogicManager->vecImportDLLList[Index_dll]->GetDllName());
		for (int i=0;i<m_pLogicManager->m_vecSelDllName_Rep.size();++i)
		{
			if (m_pLogicManager->m_vecSelDllName_Rep[i] == m_pLogicManager->m_vecSelDllName[k])
			{
				m_pLogicManager->m_vecSelDllName_Rep.erase(m_pLogicManager->m_vecSelDllName_Rep.begin()+i);
				m_pLogicManager->m_vecDllPath_Rep.erase(m_pLogicManager->m_vecDllPath_Rep.begin()+i);
				break;
			}
		}

		m_pLogicManager->DeleteDllfromDB(m_pLogicManager->vecImportDLLList[Index_dll]->GetDllName());

	}

	m_pLogicManager->DeleteFromImportList(m_pLogicManager->m_vecSelDllName,strthreadName);

	vector <wstring>strTempVecSelDllName;
	for(int i=0;i< m_pLogicManager->m_vecSelDllName.size();i++)
		strTempVecSelDllName.push_back(m_pLogicManager->m_vecSelDllName.at(i).GetString());
	m_pLogicManager->DeleteDllParametersOfThreadByNames(strTempVecSelDllName);//ɾ��list_paramterConfig���е������������
	//20180720 ��������ӵ��Ե�ʱ�� ��Ҫ�������ݿ� ��unit02��
	if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
	{
		for(int i= 0; i<m_pLogicManager->m_vecSelDllName.size(); ++i)
		{
			CString strDllName = m_pLogicManager->m_vecSelDllName[i];
			
			if (!m_pDataAccess->UpdateLibInsertIntoUnit02Cal(L"5",strthreadName.GetString(),strDllName.GetString(),L"",L"",L"",L"",L"",L"",L"",L"",L"",m_pDataAccess->GetServerAddress()!=L"127.0.0.1" &&m_pDataAccess->GetServerAddress()!=L"localhost" ))
			{
				MessageBox(_T("�����unit02ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
			}
		}
	}

	return ;
}



void CPaneArbiter::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//Timer 1: Refresh LOG

	CDialogEx::OnTimer(nIDEvent);
}


void  CPaneArbiter::Exit()
{
	KillTimer(1);
	if (m_pLogicManager != NULL)
	{
		m_pLogicManager->Exit();
		m_pLogicManager = NULL;
	}
}


BOOL CPaneArbiter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ControlLogicList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_tDateFrom=COleDateTime::GetCurrentTime();
	m_tDateTo=COleDateTime::GetCurrentTime();
	m_tTimeFrom=COleDateTime::GetCurrentTime();
	m_tTimeFrom.SetTime(m_tTimeFrom.GetHour(), m_tTimeFrom.GetMinute(), 0);
	m_tTimeTo=COleDateTime::GetCurrentTime();
	m_tTimeTo.SetTime(m_tTimeTo.GetHour(), m_tTimeTo.GetMinute(), 0);
	m_nSeletectedPeriod =0;
	
	return TRUE;  
}


void CPaneArbiter::InitData(vector<pair<CString,int>>& vecName)
{
	///////////////////////////////////////////////////////////////////////////////////
	//��ʼ����dll�б�
	if (!m_bIsLstTitleInit)
	{
		InitListCtrlHead();
		m_bIsLstTitleInit = true;
	}

	vecName.clear();

	std::vector<CLogicDllThread*>::const_iterator iter;
	wstring strDllThreadName;
	wstring strStatusOnline;
	for (iter=m_pLogicManager->m_vecDllThreadList.begin(); iter!=m_pLogicManager->m_vecDllThreadList.end(); ++iter)
	{
		strDllThreadName = (*iter)->GetName();
		if (strDllThreadName != L"")
		{
			int nFlag = -1;
			if (m_pDataAccess->ReadOrCreateCoreDebugItemValue(strDllThreadName)==L"1")
			{
				strStatusOnline = L"::(On)";
				nFlag = 1;
			}
			else
			{
				strStatusOnline = L"::(Off)";
				nFlag = 0;
			}

			int nPeriod = 0;
			CDLLObject* pDllObj = (*iter)->GetDllObject(0);
			//ASSERT(NULL != pDllObj);
			if (NULL != pDllObj)
			{
				nPeriod = pDllObj->GetTimeSpan();
			}
			CString strEachName;
			strEachName.Format(_T("%s%s(%d)"), strDllThreadName.c_str(), strStatusOnline.c_str(), nPeriod);
			vecName.push_back(make_pair(strEachName, nFlag));
		}
	}

	//run warning scan thread
	SetTimer(1,2000,NULL);
}

CLogicDllThread * CPaneArbiter::FindDLLThreadByName(wstring strDllThreadName)
{
	for(int i=0;i<m_pLogicManager->m_vecDllThreadList.size();i++)
	{
		if(m_pLogicManager->m_vecDllThreadList[i]->GetName()==strDllThreadName)
			return m_pLogicManager->m_vecDllThreadList[i];
	}

	return NULL;
}

void CPaneArbiter::RefreshDLLsOfThread()
{
	m_ControlLogicList.DeleteAllItems();
	if (m_nThreadNum <= 0)
	{
		return;
	}

	CString strthreadName(m_strCurSelName);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	CLogicDllThread *pDLLThread = FindDLLThreadByName(strthreadName.GetString());
	if(pDLLThread == NULL)
		return;

	m_pLogicManager->LoadDllThreadRelationship(pDLLThread);//����dll��������������m_vecCurDllInputParameterList

	CString strAllRelationships;
	//Json::Value myRoot("");
	Json::Value myRoot;
	for(int i=0;i<pDLLThread->GetDllCount();i++)
	{
		CDLLObject * curImportDLL = pDLLThread->GetDllObject(i);
		CString dllname = curImportDLL->GetDllName();
		CString dlloriginname = curImportDLL->GetDllOriginName();
		CString strTime = curImportDLL->GetDllImportTime();
		CString author  = curImportDLL->GetDllAuthor();
		CString version = curImportDLL->GetDllVersion();
		CString description = curImportDLL->GetDllDescription();

		//Json::Value oneDll("");
		Json::Value oneDll_Input;
		Json::Value oneDll_Output;

		string strdllname;
		Project::Tools::WideCharToUtf8(dllname.GetString(), strdllname);//�ַ�ת��
		//myRoot[strdllname] = Json::Value(oneDll);
		CString displayname = dllname.Left(dllname.GetLength()-4);
		m_ControlLogicList.InsertItem(i, _T(""));
		m_ControlLogicList.SetItemText(i, 0, displayname);
		m_ControlLogicList.SetItemText(i, 1, dlloriginname);
		m_ControlLogicList.SetItemText(i, 2, strTime);
		m_ControlLogicList.SetItemText(i, 3, author);
		m_ControlLogicList.SetItemText(i, 4, version);
		m_ControlLogicList.SetItemText(i, 5, description);

		int j=0;
		for(j=0;j<curImportDLL->m_vecCurDllInputParameterList.size();j++)
		{
			CString strTemp;
			CLogicParameter * para = curImportDLL->m_vecCurDllInputParameterList[j];
			//strTemp.Format(L"%s\r\n::input::%s=%s;\r\n",curImportDLL->GetDllName(), para->GetName().c_str(), para->GetOutputString().c_str());
			//strAllRelationships +=strTemp;

			string strParamName;
			CString cstrParamName;
			cstrParamName.Format(_T("%03d:%s"),j+1, para->GetName().c_str());
			//cstrParamName.Format(_T("%s"), para->GetName().c_str());
			Project::Tools::WideCharToUtf8(cstrParamName.GetString(), strParamName);
			string strValue;
			strValue = Project::Tools::WideCharToAnsi(para->GetOutputString().c_str());
			oneDll_Input[strParamName] = Json::Value(strValue);
	
		}
		
		for(j=0;j<curImportDLL->m_vecCurDllOutputParameterList.size();j++)
		{
			CString strTemp;
			CLogicParameter * para = curImportDLL->m_vecCurDllOutputParameterList[j];
			//strTemp.Format(L"%s\r\n::output::%s=%s;\r\n",curImportDLL->GetDllName(), para->GetName().c_str(), para->GetOutputString().c_str());
			//strAllRelationships +=strTemp;

			string strParamName;
			CString cstrParamName;
			cstrParamName.Format(_T("%03d:%s"),j+1, para->GetName().c_str());
			//cstrParamName.Format(_T("%s"), para->GetName().c_str());
			Project::Tools::WideCharToUtf8(cstrParamName.GetString(), strParamName);
			string strValue;
			strValue = Project::Tools::WideCharToAnsi(para->GetOutputString().c_str());
			oneDll_Output[strParamName] = Json::Value(strValue);

		}
		//Json::Value rootInput = Json::Value("");
		//rootInput["input"] = oneDll_Input);
		myRoot[strdllname]["input"] = oneDll_Input;
		if(!oneDll_Output.isNull())
		    myRoot[strdllname]["output"] = oneDll_Output;
	}
	
	wstring wstrAllValue = Project::Tools::AnsiToWideChar( myRoot.toStyledString().data());
	m_strLogicRelation = wstrAllValue.c_str();
	UpdateData(FALSE);

}

void CPaneArbiter::OnNMDblclkListControllogic(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	OnBnClickedButtonConfig();
	return;
}
void CPaneArbiter::OnNMClickListControllogic(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	UINT iSeclectSum = m_ControlLogicList.GetSelectedCount();
	POSITION pt = m_ControlLogicList.GetFirstSelectedItemPosition();
	CString csText = _T("");
	int iTem = -1;
	for (UINT i = 0; i < iSeclectSum; i++)
	{
		csText = _T("");
		iTem = m_ControlLogicList.GetNextSelectedItem(pt);
		csText = m_ControlLogicList.GetItemText(iTem,0);
		csText += _T(".dll");
	}
}
/*
	������������
	����˵�������ò���
	���뷽����˫�����ԡ���������
*/
void CPaneArbiter::OnBnClickedButtonConfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strthreadName(m_strCurSelName);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));
	
	UINT iSeclectSum = m_ControlLogicList.GetSelectedCount();
	POSITION pt = m_ControlLogicList.GetFirstSelectedItemPosition();
	CString csText = _T("");
	int iTem = -1;
	for (UINT i = 0; i < iSeclectSum; i++)
	{
		csText = _T("");
		iTem = m_ControlLogicList.GetNextSelectedItem(pt);
		csText = m_ControlLogicList.GetItemText(iTem,0);
		csText =csText+ _T(".dll");
	}
	if (m_pLogicManager->vecImportDLLList.empty())
	{
		MessageBox(_T("����ѡ�в���!"));
		return;
	}
	//ʵ����ֻ��һ��ѡ��
	if (iTem >-1 && csText != _T(""))
	{
		CDLLObject *pDLLObject= m_pLogicManager->GetDLLObjectByName(csText);
		if(pDLLObject==NULL)
			return;

		if(!m_pLogicManager->RunDLL(csText))
			return;

		//����"���ÿ��Ʋ��Բ��� "ǰ������Ԥ����
		wstring strLongInputPara = pDLLObject->GetLB()->GetAllInputParameter().GetString();
		wstring strLongOutputPara = pDLLObject->GetLB()->GetAllOutputParameter().GetString();
		m_pLogicManager->m_vecCurDllInputParameterList  = m_pLogicManager->SeparateParameter(strLongInputPara.c_str());
		m_pLogicManager->m_vecCurDllOutputParameterList = m_pLogicManager->SeparateParameter(strLongOutputPara.c_str());

		std::vector<vec_wstr> inputDB,outputDB;
		CString declareDB;
		m_pLogicManager->GetInputParameterfromDB(pDLLObject->GetDllName(),inputDB);
		m_pLogicManager->GetOutputParameterfromDB(pDLLObject->GetDllName(),outputDB);
		m_pLogicManager->GetDeclareParameterfromDB(pDLLObject->GetDllName(), pDLLObject->GetDllOriginName(), pDLLObject->GetDllThreadName(), declareDB);//��ȡ����˵��
		SetParameterDlg ParamDlg(m_pLogicManager, m_pDataAccess);
		ParamDlg.m_strCurSelDll = csText;
		ParamDlg.m_pointmanager = m_pointmanager;
		ParamDlg.m_strIntroduction = pDLLObject->GetDllDescription();
	
		ParamDlg.m_vecCurDllInputParameterList	= inputDB;
		ParamDlg.m_vecCurDllOutputParameterList = outputDB;
		ParamDlg.m_strDllDeclare = declareDB;
		//ParamDlg.m_strThreadName = strthreadName;
		CString strOldName = pDLLObject->GetDllThreadName();	
 		ParamDlg.m_strThreadName = strOldName;
		ParamDlg.m_strDllOriginName = pDLLObject->GetDllOriginName();
		if(ParamDlg.DoModal() == IDOK);//����Ļ���и���ֵ
		{
			//m_pLogicManager->SaveDeclareParameterToDB(pDLLObject->GetDllName(),ParamDlg.m_strDllDeclare);���ڵ����ڴ���
		}
		//�����߳����仯�ж��л����Ե��߳�
		//����ԭ��������������δ��д�����ݿ�
		if(strOldName != ParamDlg.m_strThreadName)
		{
			pDLLObject->SetDllThreadName(ParamDlg.m_strThreadName);

			CLogicDllThread *pDllOldThread = FindDLLThreadByName(strOldName.GetString());
			pDllOldThread->DeleteDll(pDLLObject);

			CLogicDllThread *pDllThread = FindDLLThreadByName(ParamDlg.m_strThreadName.GetString());
			if(pDllThread==NULL)
			{
				pDllThread = new CLogicDllThread(ParamDlg.m_strThreadName.GetString());
				pDllThread->AddDll(pDLLObject);
				m_pLogicManager->m_vecDllThreadList.push_back(pDllThread);

				wstring strStatusOnline = L"::(Off)";
				if(m_pDataAccess->ReadOrCreateCoreDebugItemValue(ParamDlg.m_strThreadName.GetString())==L"1")
					strStatusOnline = L"::(On)";

				m_lstLogicThread.AddString(ParamDlg.m_strThreadName+strStatusOnline.c_str());
				//д�����ݿ�
				m_pDataAccess->WriteCoreDebugItemValue(ParamDlg.m_strThreadName.GetString(), L"0");
			}	
			else
			{
				pDllThread->AddDll(pDLLObject);
			}
			if(m_lstLogicThread.GetCount()>0)
			{
				m_lstLogicThread.SetCurSel(m_lstLogicThread.GetCount()-1);
			}

			RefreshDLLsOfThread();
		}

		m_pLogicManager->UnloadDll(csText); 
	}
	
}

/*
	�������ô���һdb
*/
void CPaneArbiter::OnBnClickedButtonImportconfig()
{
	for (int i=0;i<m_pLogicManager->vecImportDLLList.size();++i)
	{
		if (m_pLogicManager->vecImportDLLList[i]->GetDllName() != _T(""))
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("����"), _T("�Ѽ����в���dll,����ж�����ã�"), FALSE);
			if(WarnDlg.DoModal())
				return;
		}
	}

	//ˢ��dll
	for(int i=0;i<m_pLogicManager->vecImportDLLList.size();i++)
	{
		
		int n = m_ControlLogicList.GetItemCount();
		for (int j=0;j<n;++j)
		{
			CString cur_item = m_ControlLogicList.GetItemText(j,0);
			cur_item+= _T(".dll");
			if (m_pLogicManager->vecImportDLLList[i]->GetDllName() == cur_item)
			{
				m_ControlLogicList.DeleteItem(j);
				break;
			}
		}


		SAFE_DELETE(m_pLogicManager->vecImportDLLList[i]);
	}
	m_pLogicManager->vecImportDLLList.clear();

	CFileDialog dlgOpenFile(TRUE,NULL,L"*.s3db",OFN_HIDEREADONLY|OFN_HIDEREADONLY,L"s3db Files (*.s3db)|*.s3db|");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		CString dbName = dlgOpenFile.GetPathName();
		m_pLogicManager->LoadNewConfigFromS3DB(dbName);
	}

	m_pLogicManager->LoadDLLfromDB();
	for (int i=0;i<m_pLogicManager->vecImportDLLList.size();++i)
	{
		CDLLObject *curImportDLL = m_pLogicManager->vecImportDLLList[i];
		m_pLogicManager->StoreDllToDB(curImportDLL,true);
	}

	//ˢ�½���
	for (int i =0 ;i<m_pLogicManager->vecImportDLLList.size();++i)
	{
		CDLLObject * curImportDLL = m_pLogicManager->vecImportDLLList[i];
		CString dllname = curImportDLL->GetDllName();
		CString dlloriginname = curImportDLL->GetDllOriginName();
		CString strTime = curImportDLL->GetDllImportTime();
		CString author  = curImportDLL->GetDllAuthor();
		CString version = curImportDLL->GetDllVersion();
		CString description = curImportDLL->GetDllDescription();
		CString strDllThreadName = curImportDLL->GetDllThreadName();
		CLogicDllThread *pDllThread = FindDLLThreadByName(strDllThreadName.GetString());
		if(pDllThread==NULL)
		{
			wstring strStatusOnline = L"::(Off)";
			if(m_pDataAccess->ReadOrCreateCoreDebugItemValue(strDllThreadName.GetString())==L"1")
				strStatusOnline = L"::(On)";

			pDllThread = new CLogicDllThread(strDllThreadName.GetString());
			m_pLogicManager->m_vecDllThreadList.push_back(pDllThread);
			pDllThread->AddDll(curImportDLL);

			wstring strName = pDllThread->GetName() + strStatusOnline;
			int nsel = m_lstLogicThread.AddString(strName.c_str());
		}
		else
			pDllThread->AddDll(curImportDLL);

	}
}


bool CPaneArbiter::GetWatchPointUpdate()
{
	return m_bWatchPointUpdate;
}

void CPaneArbiter::SetWatchPointUpdate( bool bSet )
{
	m_bWatchPointUpdate = bSet;
}


void CPaneArbiter::OnBnClickedButtonCalRunserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CArray<int,int> arraySelect;
	int nCount = m_lstLogicThread.GetSelCount();
	if(nCount<=0)
	{
		MessageBox(_T("����ѡ��һ�̣߳�"));
		return;
	}
	arraySelect.SetSize(1);
	m_lstLogicThread.GetSelItems(1, arraySelect.GetData());

	int ncur = -1;
	for(int i=0; i<nCount; ++i)
	{
		ncur = arraySelect[i];		//ѡ����

		CString strthreadName(m_strCurSelName);
		m_lstLogicThread.GetText(ncur,strthreadName);

		strthreadName = strthreadName.Left(strthreadName.Find(':'));
		m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), L"1");

		//
		m_lstLogicThread.DeleteString(ncur);
		strthreadName += _T("::(On)");
		m_lstLogicThread.InsertString(ncur,strthreadName);
		m_lstLogicThread.SetCurSel(ncur);
	}
}


void CPaneArbiter::OnBnClickedButtonCalPauseserver()
{
	CArray<int,int> arraySelect;
	int nCount = m_lstLogicThread.GetSelCount();
	if(nCount<=0)
	{
		MessageBox(_T("����ѡ��һ�̣߳�"));
		return;
	}
	arraySelect.SetSize(nCount);
	m_lstLogicThread.GetSelItems(nCount,arraySelect.GetData());

	int ncur = -1;
	for(int i=0; i<nCount; ++i)
	{
		ncur = arraySelect[i];		//ѡ����
		CString strthreadName;
		m_lstLogicThread.GetText(ncur,strthreadName);
		strthreadName = strthreadName.Left(strthreadName.Find(':'));
		m_pDataAccess->WriteCoreDebugItemValue(strthreadName.GetString(), L"0");

		//
		m_lstLogicThread.DeleteString(ncur);
		strthreadName += _T("::(Off)");
		m_lstLogicThread.InsertString(ncur,strthreadName);

		m_lstLogicThread.SetCurSel(ncur);
	}

}

bool CPaneArbiter::ReplaceDllByOriginName(CString strDllFilePath)
{
	std::vector<CString> vecSelDllName;
	std::vector<CString> vecSelDllOriginName;
	std::vector<CString> vecSelDllPath;
	std::vector<CString> vecSelThreadName;	
	std::vector<CString> allRepDll;
	std::vector<int>	 vecSelDllTimeSpan;					//ѡ��dll��ʱ����
	std::vector<CDLLObject*> vecReImportDLLList;
	m_pLogicManager->m_vecSelDllName_Rep.clear();
	m_pLogicManager->m_vecDllPath_Rep.clear();

	CString strFileName;
	int nTempDot = strDllFilePath.ReverseFind('\\');
	int nTempLen = strDllFilePath.GetLength();
	CString strDllName = strDllFilePath.Right(nTempLen-nTempDot-1);
	CString dlloriginname;
	if (strDllName.GetLength() > 4)
	{	//ȥ��".dll"
		dlloriginname = strDllName.Left(strDllName.GetLength() - 4);
	}

	bool bISInsert = false;
	int	 nTimeSpan = 20;
	for (int i=0;i<m_pLogicManager->vecImportDLLList.size();++i)
	{
		//if (strDllName == m_pLogicManager->vecImportDLLList[i]->GetDllName())
		if(dlloriginname == m_pLogicManager->vecImportDLLList[i]->GetDllOriginName())
		{
			bISInsert = true;
			CString dllname = m_pLogicManager->vecImportDLLList[i]->GetDllName();
			CString threadname = m_pLogicManager->vecImportDLLList[i]->GetDllThreadName();
			vecSelDllName.push_back(dllname);
			vecSelDllOriginName.push_back(dlloriginname);
			vecSelDllPath.push_back(strDllFilePath);
			allRepDll.push_back(dllname);
			vecSelThreadName.push_back(threadname);
			nTimeSpan = m_pLogicManager->vecImportDLLList[i]->GetTimeSpan();
			vecSelDllTimeSpan.push_back(nTimeSpan);
		}
	}

	if (!bISInsert)
	{
		AfxMessageBox(L"û���ҵ�DLL��ʶ������ͬ�Ĳ���");
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	//����Ҫ���滻��dll�б�Ϊʱ,��ɾ��
	if (vecSelDllName.empty() && m_pLogicManager->m_vecSelDllName_Rep.empty() )
	{
		return false;
	}
	//������Ҫ�滻��Dllm_pLogicManager->vecImportDLLList����Ϊ����
	/*m_pLogicManager->UnloadDll(vecSelDllName,strthreadName);
	m_pLogicManager->LoadAndSaveDll(vecSelDllPath,vecSelDllName, vecSelDllOriginName,strthreadName,nTimeSpan);*/

	m_pLogicManager->UnloadDll(vecSelDllName, vecSelThreadName);
	if(m_pLogicManager->LoadAndSaveDll(vecSelDllPath,vecSelDllName, vecSelDllOriginName,vecSelThreadName,vecSelDllTimeSpan))
		DisplayDllName(vecSelDllName);
	else
	{
		AfxMessageBox(_T("����ʧ��:"));
		return false;
	}
	RefreshDLLsOfThread();

	return true;
}


void CPaneArbiter::OnBnClickedButtonRefreshLogicdll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	vector<CString> strDllOriginNameList;
	CString strPathName;
	CFileDialog dlgOpenFile(TRUE,NULL,L"*.dll",OFN_HIDEREADONLY|OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT,L"Dll Files (*.dll)|*.dll|");
	if(dlgOpenFile.DoModal()==IDOK)
	{
		POSITION ps = dlgOpenFile.GetStartPosition();	
		while (ps !=NULL)
		{
			//��ȡȫ·��
			strPathName = dlgOpenFile.GetNextPathName(ps);
			strDllOriginNameList.push_back(strPathName);
		}

		vector<CString> strDllOriginNameListReplaced;

		for(int i=0;i<strDllOriginNameList.size();i++)
		{
			bool bReplaced = ReplaceDllByOriginName(strDllOriginNameList[i]);
			if(bReplaced)
				strDllOriginNameListReplaced.push_back(strDllOriginNameList[i]);
			else
				return;
		}

		// Mysql OperationF
		// write table filestorage
		vector<CString>::const_iterator	iter;
		CString strFileName;
		for (iter=strDllOriginNameListReplaced.begin(); iter!=strDllOriginNameListReplaced.end(); ++iter)
		{
			strFileName = (*iter).Right((*iter).GetLength() - (*iter).ReverseFind('\\') - 1);
			if(m_pDataAccess->GetServerAddress()!=L"127.0.0.1" &&m_pDataAccess->GetServerAddress()!=L"localhost")
			{
				if (!m_pDataAccess->UpdateLibInsertIntoFilestorageCal(*iter, strFileName, strFileName))
				{
					MessageBox(_T("�����filestorageʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				}
			}
			



			if (!m_pDataAccess->UpdateLibInsertIntoUnit02Cal(strFileName, m_pDataAccess->GetServerAddress()!=L"127.0.0.1" &&m_pDataAccess->GetServerAddress()!=L"localhost" ))
			{
				MessageBox(_T("�����unit02ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
			}

			m_pDataAccess->UpdateLibReloadIntoUnit02Cal(strFileName);
		}

		//replace into syslogic
		CString strWildcard;
		Project::Tools::GetSysPath(strWildcard);
		strWildcard += _T("\\syslogic\\");
		CString strFFName = strFileName.Right(strFileName.GetLength() - strFileName.ReverseFind('\\') - 1);
		strWildcard+= strFFName;

		if(::CopyFile(strPathName, strWildcard, FALSE)==0)
		{
			return;
		}
	}
}


void CPaneArbiter::OnBnClickedButtonAddFlow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddDllThreadNameDlg dlg(true);
	if(dlg.DoModal() == IDOK)
	{
		if(IsThreadNameExist(dlg.m_strDllName))
		{
			AfxMessageBox(L"�����߳��Ѵ��ڣ�����ʧ��!");
			return;
		}

		m_lstLogicThread.AddString(dlg.m_strDllName + _T("::(Off)"));

		CLogicDllThread *pDllThread = FindDLLThreadByName(dlg.m_strDllName.GetString());
		if(pDllThread==NULL)
		{
			pDllThread = new CLogicDllThread(dlg.m_strDllName.GetString());
			m_pLogicManager->m_vecDllThreadList.push_back(pDllThread);
		}

		//д�����ݿ�
		m_pDataAccess->WriteCoreDebugItemValue(dlg.m_strDllName.GetString(), L"0");

		if(m_lstLogicThread.GetCount()>0)
		{
			m_lstLogicThread.SetCurSel(m_lstLogicThread.GetCount()-1);
		}

		RefreshDLLsOfThread();
	}
}


void CPaneArbiter::OnBnClickedButtonDelFlow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if(m_lstLogicThread.GetCount()<=0)
	//	return;

	int ncur = m_lstLogicThread.GetCurSel();
	if(ncur<0)
	{
		MessageBox(_T("����ѡ��һ�̣߳�"));
		return;
	}

	CString strthreadName;
	m_lstLogicThread.GetText(ncur,strthreadName);
	m_lstLogicThread.DeleteString(ncur);
	//�޸�s3db
	strthreadName = strthreadName.Left(strthreadName.Find(':'));
	m_pLogicManager->DeleteThreadName(strthreadName);

	if(m_lstLogicThread.GetCount()>0)
	{
		m_lstLogicThread.SetCurSel(m_lstLogicThread.GetCount()-1);
	}

	RefreshDLLsOfThread();

}


void CPaneArbiter::OnBnClickedButtonRenameFlow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ncur = m_lstLogicThread.GetCurSel();
	if(ncur<0)
	{
		MessageBox(_T("����ѡ��һ�̣߳�"));
		return;
	}
	CString strthreadName,strThread;
	m_lstLogicThread.GetText(ncur,strthreadName);
	strThread = strthreadName;
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	CAddDllThreadNameDlg dlg(false);
	dlg.m_strDllName = strthreadName;
	if(dlg.DoModal() == IDOK)
	{
		if(strthreadName != dlg.m_strDllName && IsThreadNameExist(dlg.m_strDllName))
		{
			AfxMessageBox(L"�����߳��Ѵ��ڣ�����ʧ��!");
			return;
		}
		else if(strthreadName == dlg.m_strDllName)
		{
			return;
		}

		m_lstLogicThread.DeleteString(ncur);
		strThread.Replace(strthreadName,dlg.m_strDllName);
		m_lstLogicThread.InsertString(ncur,strThread);
		//�޸�s3db
		m_pLogicManager->UpdateThreadName(strthreadName,dlg.m_strDllName);

		CLogicDllThread *pDLLThread = FindDLLThreadByName(strthreadName.GetString());
		if(pDLLThread == NULL)
			return;
		pDLLThread->SetName(dlg.m_strDllName.GetString());
	}
}


void CPaneArbiter::OnBnClickedButtonModifyRelation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nThreadCurSelNum < 0)
	{
		MessageBox(_T("����ѡ��һ�̣߳�"));
		return;
	}

	//��ȡ�߳��࣬���û���߳������б���Ѱ��
	wstring wstrThreadName = m_strCurSelName.Left(m_strCurSelName.Find(L":")).GetString();
	CLogicDllThread *pDllThread = NULL;
	for(int i=0; i<m_pLogicManager->m_vecDllThreadList.size(); i++)
	{
		if(m_pLogicManager->m_vecDllThreadList[i]->GetName() == wstrThreadName)
			pDllThread = m_pLogicManager->m_vecDllThreadList[i];
	}
	/*CLogicDllThread *pDllThread = m_pLogicManager->m_vecDllThreadList[m_nThreadCurSelNum];*///�ɻ�ȡ�߳���ķ�����ƫ�Ʒ���
	CModifyRelationDlg dlg(m_pLogicManager,pDllThread);
	dlg.m_strRelation = m_strLogicRelation;
	if(dlg.DoModal()== IDOK)//�����޸Ľ���
	{
		if(dlg.m_bSuccess == true)
		{
			m_strLogicRelation = dlg.m_strRelation;
			UpdateData(FALSE);
		}
	}
}
bool CPaneArbiter::IsDllInThread(CLogicDllThread *pDllThread,wstring wsDllName)
{
	if (pDllThread == NULL)
	{
		return false;
	}

	int iCount = pDllThread->GetDllCount();
	for (int i= 0;i<iCount;++i)
	{
		if (pDllThread->GetDllObject(i)->GetDllName().GetString() == wsDllName)
		{
			return true;
		}
	}

	return false;
}
bool CPaneArbiter::IsVNameInDll(CLogicDllThread *pDllThread,wstring wsVName,wstring wsDllName,wstring wsInOut)
{
	if (pDllThread == NULL || wsVName == L"" || wsDllName == L"")
	{
		return false;
	}

	CDLLObject *pDLLObject = NULL;

	int iCount = pDllThread->GetDllCount();
	for (int i= 0;i<iCount;++i)
	{
		if (pDllThread->GetDllObject(i)->GetDllName().GetString() == wsDllName)
		{
			pDLLObject = pDllThread->GetDllObject(i);
			
			break;
		}
	}

	if (pDLLObject == NULL)
	{
		return false;
	}

	if (wsInOut == L"input")
	{
		for(int i=0;i<pDLLObject->m_vecCurDllInputParameterList.size();i++)
		{
			CLogicParameter * para = pDLLObject->m_vecCurDllInputParameterList[i];
			
			if (para->GetName() == wsVName)
			{
				return true;
			}
		}
	}else if (wsInOut == L"output")
	{
		for( int i =0;i<pDLLObject->m_vecCurDllOutputParameterList.size();i++)
		{
			CLogicParameter * para = pDLLObject->m_vecCurDllOutputParameterList[i];
			if (para->GetName() == wsVName)
			{
				return true;
			}
		}
	}

	return false;
}

bool CPaneArbiter::IsThreadNameExist( CString strThreadName )
{
	bool bFind = false;
	for(int i=0; i<m_lstLogicThread.GetCount(); i++)
	{
		CString strthreadName__;
		m_lstLogicThread.GetText(i,strthreadName__);
		strthreadName__ = strthreadName__.Left(strthreadName__.Find(':'));

		if(strthreadName__ == strThreadName)
		{
			bFind = true;
			break;
		}
	}
	return bFind;
}

void CPaneArbiter::InitLogicMgrPointer(void)
{
 	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame != NULL);
	if (NULL == pFrame)
	{
		return;
	}

	if (NULL == m_pLogicManager)
	{
		ASSERT(pFrame->m_pPrjSqlite != NULL);
		if (NULL == pFrame->m_pPrjSqlite)
		{
			return;
		}
		m_pLogicManager = new CBEOPLogicManager(pFrame->m_strDBFileName, pFrame->m_pPrjSqlite->GetFileEncryptType(), this);
	}
}

void CPaneArbiter::SetThreadNum(const int nThreadAllNum, const int nThreadCurSelNum)
{
	m_nThreadNum = nThreadAllNum;
	m_nThreadCurSelNum = nThreadCurSelNum;
}

void CPaneArbiter::SetThreadCurSelName(const CString strCurSelName)
{
	m_strCurSelName = strCurSelName;
}

/*
	�½�ϵͳ����
*/
void CPaneArbiter::OnBnClickedBtnLoadSysCal()
{
	// TODO: Add your control notification handler code here
	CString strthreadName(m_strCurSelName);
	if (_T("") == m_strCurSelName)
	{
		MessageBox(_T("����ѡ���̣߳�"), _T("����"), MB_ICONERROR);
		return;
	}
	strthreadName = strthreadName.Left(strthreadName.Find(':'));


	std::vector<CString> vecSelDllName;
	std::vector<CString> vecOriginDllName;
	std::vector<CString> vecSelDllPath;
	std::vector<CDLLObject*> vecReImportDLLList;
	m_pLogicManager->m_vecSelDllName_Rep.clear();
	m_pLogicManager->m_vecDllPath_Rep.clear();

	SYS_TACTICS			stSysTactics;
	vector<SYS_TACTICS> vecSysTactics;
	vector<SYS_TACTICS>::const_iterator iterSysTact;
	
	CString strWildcard;
	Project::Tools::GetSysPath(strWildcard);
	strWildcard += _T("\\syslogic\\*.dll");
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots() || finder.IsDirectory())
		{
			continue;
		}

		stSysTactics.strFileTitle = finder.GetFileTitle();
		stSysTactics.strPathName = finder.GetFilePath();
		vecSysTactics.push_back(stSysTactics);
	}
	finder.Close();


	if (vecSysTactics.empty())
	{
		MessageBox(_T("��ȷ���ļ���syslogic���Ƿ����ϵͳ���ԣ�"), _T("����"), MB_OK|MB_ICONERROR);
		return;
	}
	CSystemTactics sysTactDlg(vecSysTactics);
	if (IDOK == sysTactDlg.DoModal())
	{
		CString strCurSelName = sysTactDlg.m_strCurSelName;
		CString strTacticsName = sysTactDlg.m_strSysTacticsName;

		if (m_pLogicManager->FindDllNameIsExist(strTacticsName))
		{
			AfxMessageBox(L"�����Ѵ��ڣ�����ʧ��!");
			return;
		}


		//��ȡȫ·��
		CString strFilePathName;
		for (iterSysTact=vecSysTactics.begin(); iterSysTact!=vecSysTactics.end(); ++iterSysTact)
		{
			if (iterSysTact->strFileTitle == strCurSelName)
			{
				strFilePathName = iterSysTact->strPathName;
				break;
			}
		}
		if (_T("") == strFilePathName || _T("") == strTacticsName)
		{
			return;
		}

		vecSelDllName.push_back(strTacticsName + _T(".dll"));	// ������+".dll"
		vecSelDllPath.push_back(strFilePathName);				// dllȫ·��+����
		vecOriginDllName.push_back(strCurSelName);				// dll��������".dll"��
	}


	//////////////////////////////////////////////////////////////////////////
	//����Ҫ���滻��dll�б�Ϊʱ,��ɾ��
	if (vecSelDllName.empty() && m_pLogicManager->m_vecSelDllName_Rep.empty() )
	{
		return ;
	}

	Sleep(1000);

	if (m_pLogicManager->LoadAndSaveDll(vecSelDllPath,vecSelDllName,vecOriginDllName,strthreadName))
	{
		DisplayDllName(vecSelDllName);

		if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
		{
			for(int i= 0; i<vecSelDllPath.size(); ++i)
			{
				CString strDllPath = vecSelDllPath[i];
				CString strDllOldName = strDllPath.Right(strDllPath.GetLength() - strDllPath.ReverseFind('\\') - 1);
				CString strDllName = vecSelDllName[i];
				if (!m_pDataAccess->UpdateLibInsertIntoFilestorageCal(strDllPath, strDllOldName, strDllName))
				{
					MessageBox(_T("�����filestorageʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				}

				if (!m_pDataAccess->UpdateLibInsertIntoUnit02Cal(L"4",strthreadName.GetString(),strDllName.GetString(),strDllName.GetString(),L"",L"",L"",L"",L"",L"",L"",strDllOldName.GetString(), m_pDataAccess->GetServerAddress()!=L"127.0.0.1" &&m_pDataAccess->GetServerAddress()!=L"localhost" ))
				{
					MessageBox(_T("�����unit02ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				}
			}
		}
	}
	RefreshDLLsOfThread();
}

void CPaneArbiter::GetInitThreadName(vector<CString>& vecName)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_pLogicManager->m_strDBFileName = pFrame->m_strDBFileName;
	m_pLogicManager->ReadDLLfromDB(pFrame->m_strDBFileName,m_pLogicManager->vecImportDLLList);

	vecName.clear();
	wstring strStatusOnline = L"::(disconnect)";
	for (int i =0; i<m_pLogicManager->vecImportDLLList.size(); ++i)
	{
		CDLLObject* curImportDLL = m_pLogicManager->vecImportDLLList[i];
		CString dllname = curImportDLL->GetDllName();
		CString dlloriginname = curImportDLL->GetDllOriginName();
		CString strTime = curImportDLL->GetDllImportTime();
		CString author  = curImportDLL->GetDllAuthor();
		CString version = curImportDLL->GetDllVersion();
		CString description = curImportDLL->GetDllDescription();
		CString strDllThreadName = curImportDLL->GetDllThreadName();
		CLogicDllThread *pDllThread = FindDLLThreadByName(strDllThreadName.GetString());
		int nPeriod = curImportDLL->GetTimeSpan();
		if (NULL == pDllThread)
		{
			pDllThread = new CLogicDllThread(strDllThreadName.GetString());
			m_pLogicManager->m_vecDllThreadList.push_back(pDllThread);
			pDllThread->AddDll(curImportDLL);

			CString strName;
			strName.Format(_T("%s%s(%d)"), (pDllThread->GetName()).c_str(), strStatusOnline.c_str(), nPeriod);
			vecName.push_back(strName);
		}
		else
		{
			pDllThread->AddDll(curImportDLL);
		}
	}
}

void CPaneArbiter::InitListTitle(void)
{
	if (!m_bIsLstTitleInit)
	{
		InitListCtrlHead();
		m_bIsLstTitleInit = true;
	}
}

/*
	����
*/
void CPaneArbiter::OnBnClickedBtnChangeName()
{
	// TODO: Add your control notification handler code here

	// get choose
	CString strDllName;
	CString	strTacticNameOld;
	CString	strTacticNameNew;
	POSITION pos = m_ControlLogicList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{	// no select
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_ControlLogicList.GetNextSelectedItem(pos);
			strTacticNameOld = m_ControlLogicList.GetItemText(nItem, 0);
			strDllName = m_ControlLogicList.GetItemText(nItem, 1);
			break;	// deal with first select
		}
	}


	// determine whether has same name
	CInputLogicNameDlg dlgInputName(false);
	dlgInputName.m_strLogicName = strTacticNameOld;
	if (IDOK == dlgInputName.DoModal())
	{
		strTacticNameNew = dlgInputName.m_strLogicName;
	}
	else
	{
		return;
	}


	if(m_pLogicManager->FindDllNameIsExist(strTacticNameNew))
	{
		AfxMessageBox(L"�����Ѵ��ڣ�����ʧ��!");
		return;
	}

	// close this dll call
	m_pLogicManager->ExitDllByName(strTacticNameOld + _T(".dll"));


	// change interface show
	int nCount = m_ControlLogicList.GetItemCount();
	for (int i=0; i<nCount; ++i)
	{
		if (strTacticNameOld == m_ControlLogicList.GetItemText(i, 0))
		{	// find it
			m_ControlLogicList.SetItemText(i, 0, strTacticNameNew);
			break;
		}
	}


	// list_dllstore
	m_pLogicManager->UpdateTacticName(strTacticNameOld, strTacticNameNew);


	// list_paramterConfig
	if (m_pLogicManager->UpdateTacticParamConfig(strTacticNameOld, strTacticNameNew))
	{
		// refresh text
		RefreshDLLsOfThread();
	}
}


BOOL CPaneArbiter::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		switch (pMsg->wParam)
		{
		case 'a':
		case 'A':
			if (bCtrl)
			{
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RELATIONSHIP);
				if (pEdit != NULL)
				{
					pEdit->SetFocus();
					pEdit->SetSel(0, -1);
				}
			}  
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CPaneArbiter::InitListCtrlHead(void)
{
	m_ControlLogicList.InsertColumn(0, _T("����"), LVCFMT_LEFT, 220);
	m_ControlLogicList.InsertColumn(1, _T("DLL��ʶ��"), LVCFMT_LEFT, 180);
	m_ControlLogicList.InsertColumn(2, _T("����ʱ��"), LVCFMT_LEFT, 130);
	m_ControlLogicList.InsertColumn(3, _T("����"), LVCFMT_LEFT, 100);
	m_ControlLogicList.InsertColumn(4, _T("�汾"), LVCFMT_LEFT, 100);
	m_ControlLogicList.InsertColumn(5, _T("����"), LVCFMT_LEFT, 100);
	m_ControlLogicList.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);
}

/*
	Log��ѯ
*/
void CPaneArbiter::OnBnClickedBtnQueryLog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_ControlLogicList.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{	// no select
		MessageBox(_T("����ѡ����ԣ�"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
		return;
	}

	CString strDllName(_T(""));
	CString strDllNameOrg(_T(""));
	while (pos != NULL)
	{
		int nItem = m_ControlLogicList.GetNextSelectedItem(pos);
		strDllName = m_ControlLogicList.GetItemText(nItem, 0);
		strDllNameOrg = m_ControlLogicList.GetItemText(nItem, 1);
		break;	// deal with first select
	}

	CTacticLogQueryDlg dlg;
	dlg.m_strDllName = strDllName;
	dlg.m_strDllNameOrg = strDllNameOrg;
	dlg.DoModal();
}


void CPaneArbiter::OnBnClickedBtnHistorytest()
{
	CString strthreadName(m_strCurSelName);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));

	UINT iSeclectSum = m_ControlLogicList.GetSelectedCount();
	POSITION pt = m_ControlLogicList.GetFirstSelectedItemPosition();
	CString csText = _T("");
	int iTem = -1;
	for (UINT i = 0; i < iSeclectSum; i++)
	{
		csText = _T("");
		iTem = m_ControlLogicList.GetNextSelectedItem(pt);
		csText = m_ControlLogicList.GetItemText(iTem,0);
		csText += _T(".dll");
	}
	if (m_pLogicManager->vecImportDLLList.empty())
	{
		MessageBox(_T("����ѡ�в���!"));
		return;
	}
	//ʵ����ֻ��һ��ѡ��
	if (iTem >-1 && csText != _T(""))
	{
		CDLLObject *pDLLObject= m_pLogicManager->GetDLLObjectByName(csText);
		if(pDLLObject==NULL)
			return;

		if(!m_pLogicManager->RunDLL(csText))
			return;

		CLogicBase *pLB = pDLLObject->GetLB();
		CString strBirthVersion = pLB->GetSystVersion();
		if(strBirthVersion.Left(1)==_T("V"))//V2.3
			strBirthVersion = strBirthVersion.Mid(1);
		double fVersion = _wtof(strBirthVersion.GetString());
		if(fVersion<3.0)
		{
			CWarningDlg warnDlg;
			warnDlg.InitWarn(_T("��ʾ"), _T("�ò��԰汾���ɣ���֧����ʷ����"));
			warnDlg.DoModal();
			return;
		}

		std::vector<vec_wstr> inputDB,outputDB;
		m_pLogicManager->GetInputParameterfromDB(pDLLObject->GetDllName(),inputDB);
		m_pLogicManager->GetOutputParameterfromDB(pDLLObject->GetDllName(),outputDB);
		pDLLObject->ClearParameters();
		for(int j=0;j< inputDB.size();j++)
		{//[5]("fChRatePower","350.000","const","���Ĭ�Ϲ���","")
			CLogicParameter *pp = NULL;
			vec_wstr strPara = inputDB[j];
			if(strPara[2]==L"const" || strPara[2]==L"strconst")
			{
				pp = new CLogicParameterConst(strPara[0],0, strPara[2], m_pDataAccess, strPara[1]);
			}

			else
			{
				pp = new CLogicParameterPoint(strPara[0],0, strPara[2], m_pDataAccess, strPara[1]);
			}
			pDLLObject->m_vecCurDllInputParameterList.push_back(pp);
		}
		for(int j=0;j< outputDB.size();j++)
		{
			CLogicParameter *pp = NULL;
			vec_wstr strPara = outputDB[j];
			if(strPara[2]==L"const" || strPara[2]==L"strconst")
			{
				pp = new CLogicParameterConst(strPara[0],1, strPara[2], m_pDataAccess, strPara[1]);
			}
			else
			{
				pp =  new CLogicParameterPoint(strPara[0],1, strPara[2], m_pDataAccess, strPara[1]);
			}
			pDLLObject->m_vecCurDllOutputParameterList.push_back(pp);
		}
		

		if(!pLB->SetActMode(0))
		{
			CWarningDlg warnDlg;
			warnDlg.InitWarn(_T("��ʾ"), _T("�ò��Բ�֧����ʷ����"));
			warnDlg.DoModal();
			return;
		}

		CLogicHistoryRunDlg dlg;
        dlg.m_tDateFrom=m_tDateFrom;
		dlg.m_tDateTo=m_tDateTo;
		dlg.m_tTimeFrom=m_tTimeFrom;
		dlg.m_tTimeTo=m_tTimeTo;
		dlg.m_pLogicManager = m_pLogicManager;
		dlg.m_strDllName = csText;
		dlg.DoModal();
		
			
			//������ʼʱ�����ںͽ���ʱ�������Լ��������
			m_tDateFrom=dlg.m_tDateFrom;
			m_tDateTo=dlg.m_tDateTo;
			m_tTimeFrom=dlg.m_tTimeFrom;
			m_tTimeTo=dlg.m_tTimeTo;
			m_nSeletectedPeriod = dlg.m_nSeletectedPeriod;
		
	}
}

/*
	��ѡ�и����½�
*/
void CPaneArbiter::OnBnClickedBtnCopyLogic()
{
	CString strthreadName(m_strCurSelName);
	strthreadName = strthreadName.Left(strthreadName.Find(':'));
	std::vector<CString> vecSelDllName;
	std::vector<CString> vecOriginDllName;
	std::vector<CString> vecSelDllPath;
	SYS_TACTICS			stSysTactics;
	vector<SYS_TACTICS> vecSysTactics;
	CString   strNewInstanceName;
	CString csText = _T("");
	m_pLogicManager->m_vecSelDllName.clear();
	UINT iSeclectSum = m_ControlLogicList.GetSelectedCount();
	POSITION pos = m_ControlLogicList.GetFirstSelectedItemPosition();
	if(iSeclectSum<=0)
		return;
	int iTem = m_ControlLogicList.GetNextSelectedItem(pos);//�ҵ�ѡ�в��Ե�����
	csText = m_ControlLogicList.GetItemText(iTem, 0);
	CString OriginDllName;
	OriginDllName = csText;
	CString csTextstyle;
	csTextstyle=csText+_T(".dll");
	vector<SYS_TACTICS>::const_iterator iterSysTact;
	if (_T("") == m_strCurSelName)
	{
		MessageBox(_T("����ѡ����ԣ�"), _T("����"), MB_ICONERROR);
		return;
	}
	CDLLObject *pDLLObject= m_pLogicManager->GetDLLObjectByName(csTextstyle);
	if(pDLLObject==NULL)
		return;
	CString DllOriginName;
	DllOriginName=pDLLObject->GetDllOriginName();
	if(!m_pLogicManager->RunDLL(csTextstyle))
		return;
	wstring strLongInputPara = pDLLObject->GetLB()->GetAllInputParameter().GetString();
	wstring strLongOutputPara = pDLLObject->GetLB()->GetAllOutputParameter().GetString();
	
	m_pLogicManager->m_vecCurDllInputParameterList  = m_pLogicManager->SeparateParameter(strLongInputPara.c_str());
	m_pLogicManager->m_vecCurDllOutputParameterList = m_pLogicManager->SeparateParameter(strLongOutputPara.c_str());

	std::vector<vec_wstr> inputDB,outputDB;
	m_pLogicManager->GetInputParameterfromDB(pDLLObject->GetDllName(),inputDB);
	m_pLogicManager->GetOutputParameterfromDB(pDLLObject->GetDllName(),outputDB);


	std::vector<vec_wstr> strNewLongInputPara = inputDB;
	std::vector<vec_wstr> strNewLongOnputPara  = outputDB;//�Ѿ����ڵĶ���

	//ParamDlg.m_strThreadName = strthreadName;

	//�ҵ�·��
	Project::Tools::GetSysPath(csText);
	CString theOldName;//·��
	theOldName = csText+_T("\\syslogic\\");
	csText = theOldName +pDLLObject->GetDllOriginName();
	csText+=+_T(".dll");

	BOOL bCopied = CopyFile(pDLLObject->GetDllPath().GetString(),csText.GetString(),true);

	CInputLogicNameDlg dlgInputName(true);
	dlgInputName.m_strLogicName = pDLLObject->GetDllName();
	dlgInputName.m_strLogicName = dlgInputName.m_strLogicName.Left(dlgInputName.m_strLogicName.Find(L".dll"));
	dlgInputName.m_strLogicName+="_copy01";
	if(dlgInputName.DoModal()==IDOK)//�½����Ե�����
	{
		CInputLogicNameDlg dlgInputNameg(true);
		strNewInstanceName = dlgInputName.m_strLogicName;//�ı������֮��������ָ�ֵstrTacticName
		if(strNewInstanceName == "")
		{
			AfxMessageBox(L"���Ʋ���Ϊ��");
			return;
		}
		//����CString Ѱ��"a-z,A-Z,0-9��_"����ķǷ��ַ�
		for(int I_NIN=0; I_NIN<strNewInstanceName.GetLength(); I_NIN++)
		{
			int unicode = strNewInstanceName.GetAt(I_NIN);
			if( unicode >= '0' && unicode <= '9' );
			else if ( unicode >= 'A' && unicode <= 'Z' );
			else if ( unicode >= 'a' && unicode <= 'z' );
			else if ( unicode == '_');
			else 
			{
				//�����Ƿ����ַ�����������
				CString strMessageBox;
				strMessageBox.Format(_T("���������������: \"%c\" "),strNewInstanceName.GetAt(I_NIN));
				AfxMessageBox(strMessageBox);
				return ;
			}
		}
		//vecNewDllName.push_back(strTacticName);
		if(m_pLogicManager->FindDllNameIsExist(strNewInstanceName))
		{
			AfxMessageBox(L"�����Ѵ��ڣ�����ʧ��!");
			return;
		}
	}
	else 
	{
		return;
	}
	
		
	//��ȡȫ·��

	strNewInstanceName+=_T(".dll");
	vecSelDllName.push_back(strNewInstanceName);	// ������+".dll"
	vecSelDllPath.push_back(csText);				// dllȫ·��+����
	vecOriginDllName.push_back(DllOriginName);				// dll��������".dll"��
	



	//����Ҫ���滻��dll�б�Ϊʱ,��ɾ��
	if (vecSelDllName.empty() && m_pLogicManager->m_vecSelDllName_Rep.empty() )
	{
		return ;
	}
	Sleep(1000);
	//����һ��.dll�ļ�
	//���ò��ԵĲ������ó�csText������ͬ�Ĳ������������ݿ� sqlite.
	//��CSTest�ļ���dll�ļ����������ݿ���
	if (m_pLogicManager->LoadAndSaveDll(vecSelDllPath,vecSelDllName,vecOriginDllName,strthreadName))//mathif��·����2222.dll,mathif,111threadname
	{
		
		CDLLObject *pnewDLLObject= m_pLogicManager->GetDLLObjectByName(strNewInstanceName);
	    bool bRunSuccess = m_pLogicManager->RunDLL(strNewInstanceName);

		if(!bRunSuccess)
			return;



		//���õĲ�������
		
		m_pLogicManager->m_vecCurDllInputParameterList	= strNewLongInputPara;
		m_pLogicManager->m_vecCurDllOutputParameterList = strNewLongOnputPara;

		//�������浽���ݿ���
		for(int i=0;i<m_pLogicManager->m_vecCurDllInputParameterList.size();i++)
		{
			CString strVariableName(m_pLogicManager->m_vecCurDllInputParameterList[i][0].c_str());
			CString strLinkDefine(m_pLogicManager->m_vecCurDllInputParameterList[i][1].c_str());
			CString strLinkType(m_pLogicManager->m_vecCurDllInputParameterList[i][2].c_str());
			SaveParameterToDBAndServer(strNewInstanceName,L"0", strVariableName,strLinkDefine,strLinkType,L"");
		}
		for(int i=0;i<m_pLogicManager->m_vecCurDllOutputParameterList.size();i++)
		{
			CString strVariableName(m_pLogicManager->m_vecCurDllOutputParameterList[i][0].c_str());
			CString strLinkDefine(m_pLogicManager->m_vecCurDllOutputParameterList[i][1].c_str());
			CString strLinkType(m_pLogicManager->m_vecCurDllOutputParameterList[i][2].c_str());
			SaveParameterToDBAndServer(strNewInstanceName,L"1", strVariableName,strLinkDefine,strLinkType,L"");
		}




		//������δ����ǵ��������ߵ���ʱ������ֱ��ͬ�����������ݿ�

		if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
		{
			for(int i= 0; i<vecSelDllPath.size(); ++i)
			{
				CString strDllPath = vecSelDllPath[i];
				CString strDllOldName = strDllPath.Right(strDllPath.GetLength() - strDllPath.ReverseFind('\\') - 1);
				CString strDllName = vecSelDllName[i];
				if (!m_pDataAccess->UpdateLibInsertIntoFilestorageCal(strDllPath, strDllOldName, strDllName))
				{
					MessageBox(_T("�����filestorageʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				}

				if (!m_pDataAccess->UpdateLibInsertIntoUnit02Cal(L"4",strthreadName.GetString(),strDllName.GetString(),strDllName.GetString(),L"",L"",L"",L"",L"",L"",L"",strDllOldName.GetString(), m_pDataAccess->GetServerAddress()!=L"127.0.0.1" &&m_pDataAccess->GetServerAddress()!=L"localhost" ))
				{
					MessageBox(_T("�����unit02ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				}
			}
		}
	
		
		DisplayDllName(vecSelDllName);
		m_pLogicManager->UnloadDll(strNewInstanceName); 
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//��dll���뵽m_ControlLogicList��
		//���ԭ�����б����б�Dll,��嵽��λ�ã�����嵽���
		//DisplayDllName(vecSelDllName)��
		RefreshDLLsOfThread();

	}
	
		
		return;
	}
		
		
	/** ������Եĵ�һ����������޸ģ�
	 strSetType:0 �������룬1: ���������2�����õ���ʱ�䣬ʱ�����variablename��
	����������������������붨�塢���붨������ */		
void  CPaneArbiter::SaveParameterToDBAndServer(CString strDllName,  CString strSetType, CString strVariableName, CString strLinkDefine, CString strLinkType, CString strCondition)
{
	if(strSetType==L"2")
	{//���õ�������
		double fPeriod = _wtof(strVariableName.GetString());
		if(fPeriod>0.f)
		{
			m_pLogicManager->SaveSpanTimetoDB(strDllName, fPeriod);
		}
		//pDLLObject->SetTimeSpan(ParamDlg.m_dNewSpanTime);
		//�������ڵ����ݿ�
	}
	else if(strSetType==L"0")
	{
		m_pLogicManager->SaveParametertoDB(0, strDllName, strVariableName, strLinkDefine, strLinkType, strCondition);
	}
	else if(strSetType==L"1")
	{
		m_pLogicManager->SaveParametertoDB(1, strDllName, strVariableName, strLinkDefine, strLinkType, strCondition);
	}

	//���µ��������ݿ�
	if (NULL == m_pDataAccess)
	{
		//MessageBox(_T("�����������ݿ⣡"), _T("����"), MB_OK|MB_ICONERROR);
		return;
	}
	else
	{
		m_pDataAccess->InsertLogicParameters(strDllName.GetString(), strDllName.GetString(), strSetType.GetString(), strVariableName.GetString(), strLinkDefine.GetString(), strLinkType.GetString(), strCondition.GetString());
	}

	//m_ControlLogicList.SetItemText(iTem,0, ParamDlg.m_strCurSelDll);
}



void CPaneArbiter::OnEnChangeEditRelationship()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
