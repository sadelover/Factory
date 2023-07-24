// ModifyRelationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ModifyRelationDlg.h"
#include "afxdialogex.h"
#include "LogicDllThread.h"
#include "BEOPLogicManager.h"
#include "JSON/json.h"
#include "./ControlEx/WarningDlg.h"
#include "BEOPLogicManager.h"
// CModifyRelationDlg 对话框

IMPLEMENT_DYNAMIC(CModifyRelationDlg, CDialogEx)

CModifyRelationDlg::CModifyRelationDlg(CBEOPLogicManager *pLogicManager, CLogicDllThread *pDllThread, CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyRelationDlg::IDD, pParent)
	, m_strRelation(_T(""))
	, m_strCompleteRelation(_T(""))
	, m_pLogicManager(pLogicManager)
	, m_pDllThread(pDllThread)
{

}

CModifyRelationDlg::~CModifyRelationDlg()
{
}

void CModifyRelationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RELATION, m_strRelation);
}


BEGIN_MESSAGE_MAP(CModifyRelationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CModifyRelationDlg::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDOK, &CModifyRelationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyRelationDlg 消息处理程序


void CModifyRelationDlg::OnBnClickedButtonCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	//m_strCompleteRelation = _T("");
	//m_strCompleteRelation += m_strRelation;
	//m_strCompleteRelation += L"\r\n"; //强制回车
	//Clear();
	//if(Check())
	//	AfxMessageBox(L"检查合法");
	//else
	//	AfxMessageBox((L"检查不合法"));
	UpdateData(TRUE);
	//m_strCompleteRelation=JSON.stringify(m_strCompleteRelation);
	try
	{
		/*string strAll;
		Project::Tools::WideCharToUtf8(m_strRelation.GetString(), strAll);
		Json::Value myRoot(strAll);*/

		if(Check())
		{
			CWarningDlg dlg;
			dlg.InitWarn(_T("提示"),_T("配置内容符合json标准。"));
			dlg.DoModal();
		}
		else
		{
			CWarningDlg dlg;
			dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
			dlg.DoModal();
		}

	}
	catch (CMemoryException* e)
	{
		CWarningDlg dlg;
		dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
		dlg.DoModal();
		return;
	}
	catch (CFileException* e)
	{
		CWarningDlg dlg;
		dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
		dlg.DoModal();
		return;
	}
	catch (CException* e)
	{
		CWarningDlg dlg;
		dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
		dlg.DoModal();
		return;
	}

}

void CModifyRelationDlg::Clear()
{
	for(int i=0;i<m_strPNameList.size();i++)
	{
		m_strPNameList.clear();
		m_strDllNameList.clear();
		m_strInOutList.clear();
		m_strPTypeList.clear();
		m_strVNameList.clear();
	}
}

bool CModifyRelationDlg::Check()
{
	/*int nStart = 0;
	int nFindReturn = m_strCompleteRelation.Find(L"\r\n", 0);
	if(nFindReturn<0)
		return false;
	while(nFindReturn>=0)
	{
		CString strRow = m_strCompleteRelation.Mid(nStart, nFindReturn-nStart+1);
		if(strRow.GetLength()<=2)
			break;

		bool bExplain = ExplainRow(strRow);
		if(!bExplain)
		{
			return false;
		}
		nStart = nFindReturn+2;
		nFindReturn = m_strCompleteRelation.Find(L"\r\n", nStart);
	}*/
	string strAll;
	Project::Tools::WideCharToUtf8(m_strRelation.GetString(), strAll);
	Json::Value myRoot(strAll);//拿到策略关系 也就是全部的策略配置但不包括点描述
	Json::Reader jsReader;

	if (strAll.length()>0 && jsReader.parse(strAll, myRoot))
	{
		if(myRoot.isObject())
		{
			Json::Value::Members mem = myRoot.getMemberNames();
			for (auto iter = mem.begin(); iter != mem.end(); iter++)
			{
				//std::vector<vec_wstr> strParaList;
				//m_pLogicManager->GetInputParameterfromDB(m_pDllThread->GetName().c_str(),strParaList);//add
				//m_pLogicManager->GetOutputParameterfromDB(m_pDllThread->GetName().c_str(),strParaList);

				string strKey = (*iter);
				Json::Value jsonOneDll = myRoot[strKey];
				
				Json::Value jsonOneDll_Input = jsonOneDll["input"];
				Json::Value::Members memInput = jsonOneDll_Input.getMemberNames();
				for (auto iterInput = memInput.begin();iterInput != memInput.end(); iterInput++)
				{
					string strKeyInput = (*iterInput);

					string strInputVal =  jsonOneDll_Input[strKeyInput.c_str()].asString();

					int nSplit = strKeyInput.find(":");
					if(nSplit>=0)
					    strKeyInput = strKeyInput.substr(nSplit+1);
					//Json::Value jsonTemp =jsonOneDll_Input[strKeyInput];
					
					string s1("::");
					string strRow = strKey.c_str() + s1 + "input::" +strKeyInput+"="+  strInputVal.c_str()+";" ;

					wstring wstrTemp;
					Project::Tools::UTF8ToWideChar(strRow, wstrTemp);
					bool bExplain = ExplainRow(wstrTemp.c_str());
					if(!bExplain)
					{
						return false;
					}
				}

				Json::Value jsonOneDll_Output = jsonOneDll["output"];
				Json::Value::Members memOutput = jsonOneDll_Output.getMemberNames();
				for(auto interOutput = memOutput.begin();  interOutput != memOutput.end();  interOutput++)
				{
					string strKeyOutput = (*interOutput);

					string strOutputVal = jsonOneDll_Output[strKeyOutput.c_str()].asString();


					int nSplit = strKeyOutput.find(":");
					if(nSplit>=0)
						strKeyOutput = strKeyOutput.substr(nSplit+1);
					//Json::Value jsonTemp =jsonOneDll_Output[strKeyOnput];
					string s1("::");
					string strRow = strKey.c_str() + s1 + "output::" +strKeyOutput+"="+  strOutputVal.c_str()+";" ;
					wstring wstrTemp;
					Project::Tools::UTF8ToWideChar(strRow, wstrTemp);
					bool bExplain = ExplainRow(wstrTemp.c_str());
					if(!bExplain)
					{
						return false;
					}
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}
//配置插入数据是需要的参数
bool CModifyRelationDlg::ExplainRow(CString strRow)//AccumToPeriodStatics.dll::input::Total01=const:;
{
	int nFindEquation = strRow.Find(L"=");
	if(nFindEquation<0)
		return false;

	CString strLeft = strRow.Left(nFindEquation);//AccumToPeriodStatics.dll::input::Total01
	CString strRight = strRow.Mid(nFindEquation+1);//const:;

	/*寻找分号的方法更改为找到最后一个分号*/
	int nFindTail = strRight.ReverseFind(L';');
	if(nFindTail<0)
		return false;


	strRight = strRight.Left(nFindTail);

	//strLeft:  abc.dll::input::VName

	int nFindMao1 = strLeft.Find(L"::");
	if(nFindMao1<0)
		return false;

	CString strDllName = strLeft.Left(nFindMao1);
	int nFindMao2 = strLeft.Find(L"::", nFindMao1+2);
	if(nFindMao2<0 || (nFindMao2+2)>=strLeft.GetLength())
		return false;

	CString strInOut = strLeft.Mid(nFindMao1+2, nFindMao2-nFindMao1-2);
	strInOut.MakeLower();
	if(strInOut!=L"input" && strInOut!=L"output")
		return false;

	CString strVName = strLeft.Mid(nFindMao2+2);
	if(strVName.GetLength()<=0)
		return false;

	//strRight: const:0.3
	CString strType;
	CString strValue;
	int nFindMao3 = strRight.Find(L":");
	if(nFindMao3<0)
	{
		strType=_T("const");
		strValue = _T("");
	}
	else
	{

		strType = strRight.Left(nFindMao3);

		strValue = strRight.Mid(nFindMao3+1);
	}
	
	m_strDllNameList.push_back(strDllName.GetString());
	m_strInOutList.push_back(strInOut.GetString());
	m_strVNameList.push_back(strVName.GetString());
	
	
	//output
	m_strPTypeList.push_back(strType.GetString());
	m_strPNameList.push_back(strValue.GetString());


	return true;

}

void CModifyRelationDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//m_strCompleteRelation = _T("");
	//m_strCompleteRelation += m_strRelation;
	//m_strCompleteRelation += L"\r\n"; //强制回车
	//Clear();

	GetDlgItem(IDC_EDIT_RELATION)->EnableWindow(FALSE);
	m_bSuccess = false;
	if(!Check())
	{
		AfxMessageBox(L"配置脚本执行失败.");
		return;
	}

	if(m_pLogicManager)
	{

		//reload description

		bool bAllIn = true;
		//m_pLogicManager->DeleteDllParametersOfThreadByNames(m_strDllNameList);	//删除脚本配置中的所有dll的参数 

		SetWindowText(_T("正在执行配置脚本中..."));
		for(int i=0;i<m_strVNameList.size();i++)
		{
			//查找确认是否在thread中
			bool bIn = IsDllInThread(m_pDllThread,m_strDllNameList[i]);
			if(bIn)
				m_pLogicManager->UpdateOneParameter(m_strVNameList[i], m_strPNameList[i], m_strPTypeList[i], m_strDllNameList[i], m_strInOutList[i]);
			else
			{
				bAllIn = false;
			}
		}
		if(!bAllIn)
		{
			AfxMessageBox(L"部分dll名称不在该thread内，已被忽略.");
		}
		else
		{
			m_bSuccess = true;
			AfxMessageBox(L"配置脚本执行成功。");
		}
	}
	CDialogEx::OnOK();
}

bool CModifyRelationDlg::IsDllInThread( CLogicDllThread *pDllThread,wstring wsDllName )
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


BOOL CModifyRelationDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		switch (pMsg->wParam)
		{
		case 'a':
		case 'A':
			if (bCtrl)
			{
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RELATION);
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
