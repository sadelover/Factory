#include "StdAfx.h"
#include "BEOPLogicManager.h"
#include "../ControlEx/WarningDlg.h"
#include "sstream"
#include "VECTOR"
#include "Tools/warning/MsgDefine.h"
#include "ControlEx/WarningDlg.h"
#include "LogicBase.h"
#include "DB_BasicIO/sqlite/SqliteAcess.h"
#include "../Tools/Util/UtilString.h"
#include "LogicDllThread.h"
#include "LogicParameterConst.h"
#include "LogicParameterPoint.h"
#include <iostream>
CBEOPLogicManager::CBEOPLogicManager(wstring strDBFileName, const bool bEncry, CWnd *pWnd)
	:m_pWnd(pWnd)
	,m_strDBFileName(strDBFileName)
	,m_bEncry(bEncry)
{
	m_calcinternal = 10;
	m_vecSelDllName.clear();
	vecImportDLLList.clear();
	m_bExitThread = false;
	m_bThreadExitFinished = false;
	m_pDataAccess = NULL;
	m_iDeleteItem   = -1;
	//添加版本号
	m_sSystVerion = _T("V2.2");


}


CBEOPLogicManager::~CBEOPLogicManager(void)
{

	for(int i=0;i<vecImportDLLList.size();i++)
	{
		SAFE_DELETE(vecImportDLLList[i]);
	}
}



CString CBEOPLogicManager::GetSystVersion()
{
	return m_sSystVerion;
}


bool CBEOPLogicManager::Exit()
{
	//save running state into ini
	int i = 0;
	for (i=0;i<vecImportDLLList.size();++i)
	{
		if (vecImportDLLList[i]->GetLB() != NULL)
		{
			vecImportDLLList[i]->SetDllRunStatus(false);
		}
	}
	for (i=0;i<vecImportDLLList.size();++i)
	{
		if (vecImportDLLList[i]->GetLB() != NULL)
		{
			vecImportDLLList[i]->Exit();
		}
		ClearFile(vecImportDLLList[i]);
	}
	return true;
}

bool CBEOPLogicManager::UnloadDll(CString strDllName)
{
	CDLLObject *pObject = GetDLLObjectByName(strDllName);
	if(pObject)
	{
		pObject->Exit();
	}

	if (m_hDLL != NULL)
	{
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}

	return true;
}

bool CBEOPLogicManager::UnloadDll(std::vector<CString> DllList)
{
	if(DllList.empty())
		return false;

	int i = 0, j=0;

	for(i=0;i<DllList.size();i++)
	{
		for (j=0;j<vecImportDLLList.size();++j)
		{
			if (vecImportDLLList[j]->GetDllName()== DllList[i])
			{
				vecImportDLLList[j]->Exit();
				delete(vecImportDLLList[j]);
				vecImportDLLList.erase(vecImportDLLList.begin()+j);
				j--;
			}

		}

	}

	return true;
}

bool CBEOPLogicManager::UnloadDll(std::vector<CString> DllList,CString strThreadName)
{
	if(DllList.empty())
		return false;

	for(int m=0; m<m_vecDllThreadList.size(); m++)
	{
		if(m_vecDllThreadList[m]->GetName().c_str() == strThreadName)
		{
			for(int n=0;n<DllList.size();n++)
			{
				for(int l=0; l<m_vecDllThreadList[m]->GetDllCount(); l++)
				{
					if(DllList[n] == m_vecDllThreadList[m]->GetDllObject(l)->GetDllName())
					{
						m_vecDllThreadList[m]->EaraseDll(l);
						break;
					}
				}
			}
			break;
		}
	}

	int i = 0, j=0;
	for(i=0;i<DllList.size();i++)
	{
		for (j=0;j<vecImportDLLList.size();++j)
		{
			if (vecImportDLLList[j]->GetDllName()== DllList[i])
			{
				vecImportDLLList[j]->Exit();
				delete(vecImportDLLList[j]);
				vecImportDLLList.erase(vecImportDLLList.begin()+j);
				break;
			}
		}
	}

	return true;
}

bool CBEOPLogicManager::UnloadDll(std::vector<CString> DllList,std::vector<CString> ThreadList)
{
	if(DllList.empty())
		return false;

	for(int m=0; m<m_vecDllThreadList.size(); m++)
	{	
		for(int n=0;n<DllList.size();n++)
		{
			for(int l=0; l<m_vecDllThreadList[m]->GetDllCount(); l++)
			{
				if(DllList[n] == m_vecDllThreadList[m]->GetDllObject(l)->GetDllName())
				{
					m_vecDllThreadList[m]->EaraseDll(l);
				}
			}
		}		
	}

	int i = 0, j=0;
	for(i=0;i<DllList.size();i++)
	{
		for (j=0;j<vecImportDLLList.size();++j)
		{
			if (vecImportDLLList[j]->GetDllName()== DllList[i])
			{
				vecImportDLLList[j]->Exit();
				delete(vecImportDLLList[j]);
				vecImportDLLList.erase(vecImportDLLList.begin()+j);
				break;
			}
		}
	}

	return true;
}

/************************************************************************/
/* 
   加载并存储dll
*/
/************************************************************************/
bool CBEOPLogicManager::LoadAndSaveDll(std::vector<CString> PathList,std::vector<CString> DllList,  std::vector<CString> strDllOriginNameList)
{

	if (PathList.empty()||DllList.empty() || strDllOriginNameList.empty())
	{
		return false;
	}
	std::vector<CString> vecSelDllName = DllList;
	std::vector<CString> strFileName   = PathList;

	for (int i=0;i<vecSelDllName.size();++i)
	{
		CString dllname = vecSelDllName[i];
		CString strDllOriginName = strDllOriginNameList[i];
		//////////////////////////////////////////////////////////////////////////
		CString strDllPath = CopyDllFile(strFileName[i],dllname);
		//////////////////////////////////////////////////////////////////////////
		m_hDLL = NULL;
		m_hDLL = LoadLibrary(strDllPath);//加载动态链接库MyDll.dll文件；
		int nErr = GetLastError();
		if(m_hDLL==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("算法文件加载失败"), FALSE);
			WarnDlg.DoModal();
			return false;
		}
		m_pf_Dll = (pfnLogicBase)GetProcAddress(m_hDLL,"fnInitLogic");
		if(m_pf_Dll==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("未找到算法函数入口"), FALSE);
			WarnDlg.DoModal();
			FreeLibrary(m_hDLL);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//运行当前加载的dll函数
		CString Author = _T("");
		CLogicBase *pLBDll = m_pf_Dll();
		if(pLBDll != NULL)
		{
			CString dllver = pLBDll->GetSystVersion();
			CString gatewayver = GetSystVersion();
			double  ddllver = _ttof(dllver.Mid(1,dllver.GetLength()));
			double  dgatewayver = _ttof(gatewayver.Mid(1,gatewayver.GetLength()));

			if (ddllver < dgatewayver)
			{
				CWarningDlg WarnDlg;
				WarnDlg.InitWarn(_T("警告"), dllname+_T("文件版本不匹配！"), FALSE);
				WarnDlg.DoModal();
				//删除导入的 pLBDll
				//delete pLBDll;
				if (pLBDll != NULL)
				{
					delete pLBDll;
					pLBDll = NULL;
				}
				if (m_hDLL != NULL)
				{
					FreeLibrary(m_hDLL);
					m_hDLL = NULL;
				}
				continue;
			}
			else
			{

			}


		}else
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		
		CDLLObject * curImportDLL = new CDLLObject(m_pDataAccess);

		//存储dell相关信息
		//名字
		curImportDLL->SetDllName(dllname);
		curImportDLL->SetDllOriginName(strDllOriginName);
		curImportDLL->SetDllPath(strDllPath);
		//dll 句柄
		curImportDLL->SetHinstance(m_hDLL);
		//dll 函数接口
		curImportDLL->SetFun(m_pf_Dll);
		curImportDLL->SetDllRunStatus(false);
		curImportDLL->SetLB(pLBDll);
		int timespan = GetCurTimeSpanFromManager(dllname);
		curImportDLL->SetTimeSpan(timespan);
		curImportDLL->SetMsgWnd(m_pWnd);

		curImportDLL->SetS3DBPath(m_strDBFileName.c_str());

		curImportDLL->SetDllVersion(curImportDLL->GetLB()->GetDllLogicVersion());
		curImportDLL->SetDllDescription(curImportDLL->GetLB()->GetDllDescription());
		//Author = pLBDll->GetDllAuthor();
		curImportDLL->SetDllAuthor(curImportDLL->GetLB()->GetDllAuthor());
		//显示出入日志
		SYSTEMTIME sysTime;
		CString strTime;
		::GetLocalTime(&sysTime);
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth,sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		curImportDLL->SetDllImportTime(strTime);

		//存储dll信息到DB
		if(!StoreDllToDB(curImportDLL))
			return false;

		//存储本次导入dll
		UpdateDLLParameter(curImportDLL);

		vecImportDLLList.push_back(curImportDLL);
	}

	return true;
}

bool CBEOPLogicManager::LoadAndSaveDll( std::vector<CString> PathList,std::vector<CString> DllList, std::vector<CString> strDllOriginNameList,CString strThreadName )
{
	if (PathList.empty()||DllList.empty() || strDllOriginNameList.empty())
	{
		return false;
	}
	std::vector<CString> vecSelDllName = DllList;
	std::vector<CString> strFileName   = PathList;

	for (int i=0;i<vecSelDllName.size();++i)
	{
 		CString dllname = vecSelDllName[i];
		CString strDllOriginName = strDllOriginNameList[i];
		//////////////////////////////////////////////////////////////////////////
		CString strDllPath = CopyDllFile(strFileName[i],dllname);
		//////////////////////////////////////////////////////////////////////////
		m_hDLL = NULL;
		m_hDLL = LoadLibrary(strDllPath);//加载动态链接库MyDll.dll文件；
		int nErr = GetLastError();
		if(m_hDLL==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("算法文件加载失败"), FALSE);
			WarnDlg.DoModal();
			return false;
		}
		m_pf_Dll = (pfnLogicBase)GetProcAddress(m_hDLL,"fnInitLogic");
		if(m_pf_Dll==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("未找到算法函数入口"), FALSE);
			WarnDlg.DoModal();
			FreeLibrary(m_hDLL);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//运行当前加载的dll函数
		CString Author = _T("");
		CLogicBase *pLBDll = m_pf_Dll();
		if(pLBDll != NULL)
		{
			CString dllver = pLBDll->GetSystVersion();
			CString gatewayver = GetSystVersion();
			double  ddllver = _ttof(dllver.Mid(1,dllver.GetLength()));
			double  dgatewayver = _ttof(gatewayver.Mid(1,gatewayver.GetLength()));

			if (ddllver < dgatewayver)
			{
				CWarningDlg WarnDlg;
				WarnDlg.InitWarn(_T("警告"), dllname+_T("文件版本不匹配！"), FALSE);
				WarnDlg.DoModal();
				//删除导入的 pLBDll
				//delete pLBDll;
				if (pLBDll != NULL)
				{
					delete pLBDll;
					pLBDll = NULL;
				}
				if (m_hDLL != NULL)
				{
					FreeLibrary(m_hDLL);
					m_hDLL = NULL;
				}
				continue;
			}
			else
			{

			}


		}else
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////////

		CDLLObject * curImportDLL = new CDLLObject(m_pDataAccess);

		//存储dell相关信息
		//名字
		curImportDLL->SetDllName(dllname);
		curImportDLL->SetDllOriginName(strDllOriginName);
		curImportDLL->SetDllPath(strDllPath);
		//dll 句柄
		curImportDLL->SetHinstance(m_hDLL);
		//dll 函数接口
		curImportDLL->SetFun(m_pf_Dll);
		curImportDLL->SetDllRunStatus(false);
		curImportDLL->SetLB(pLBDll);
		int timespan = GetCurTimeSpanFromManager(dllname);
		curImportDLL->SetTimeSpan(timespan);
		curImportDLL->SetMsgWnd(m_pWnd);
		curImportDLL->SetDllThreadName(strThreadName);
		curImportDLL->SetS3DBPath(m_strDBFileName.c_str());
		
		curImportDLL->SetDllVersion(curImportDLL->GetLB()->GetDllLogicVersion());
		curImportDLL->SetDllDescription(curImportDLL->GetLB()->GetDllDescription());
		//Author = pLBDll->GetDllAuthor();
		curImportDLL->SetDllAuthor(curImportDLL->GetLB()->GetDllAuthor());
		//显示出入日志
		SYSTEMTIME sysTime;
		CString strTime;
		::GetLocalTime(&sysTime);
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth,sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		curImportDLL->SetDllImportTime(strTime);

		//存储dll信息到DB
		if(!StoreDllToDB(curImportDLL))
			return false;

		//存储本次导入dll
		UpdateDLLParameter(curImportDLL);

		vecImportDLLList.push_back(curImportDLL);

		for(int j=0; j<m_vecDllThreadList.size(); j++)
		{
			if(m_vecDllThreadList[j]->GetName().c_str() == strThreadName)
			{
				m_vecDllThreadList[j]->AddDll(curImportDLL);
				break;;
			}
		}

	}

	return true;
}

bool CBEOPLogicManager::LoadAndSaveDll( std::vector<CString> PathList,std::vector<CString> DllList, std::vector<CString> strDllOriginNameList,CString strThreadName,int nTimeSpan )
{
	if (PathList.empty()||DllList.empty() || strDllOriginNameList.empty())
	{
		return false;
	}
	std::vector<CString> vecSelDllName = DllList;
	std::vector<CString> strFileName   = PathList;

	for (int i=0;i<vecSelDllName.size();++i)
	{
		CString dllname = vecSelDllName[i];
		CString strDllOriginName = strDllOriginNameList[i];
		//////////////////////////////////////////////////////////////////////////
		CString strDllPath = CopyDllFile(strFileName[i],dllname);
		//////////////////////////////////////////////////////////////////////////
		m_hDLL = NULL;
		m_hDLL = LoadLibrary(strDllPath);//加载动态链接库MyDll.dll文件；
		int nErr = GetLastError();
		if(m_hDLL==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("算法文件加载失败"), FALSE);
			WarnDlg.DoModal();
			return false;
		}
		m_pf_Dll = (pfnLogicBase)GetProcAddress(m_hDLL,"fnInitLogic");
		if(m_pf_Dll==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("未找到算法函数入口"), FALSE);
			WarnDlg.DoModal();
			FreeLibrary(m_hDLL);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//运行当前加载的dll函数
		CString Author = _T("");
		CLogicBase *pLBDll = m_pf_Dll();
		if(pLBDll != NULL)
		{
			CString dllver = pLBDll->GetSystVersion();
			CString gatewayver = GetSystVersion();
			double  ddllver = _ttof(dllver.Mid(1,dllver.GetLength()));
			double  dgatewayver = _ttof(gatewayver.Mid(1,gatewayver.GetLength()));

			if (ddllver < dgatewayver)
			{
				CWarningDlg WarnDlg;
				WarnDlg.InitWarn(_T("警告"), dllname+_T("文件版本不匹配！"), FALSE);
				WarnDlg.DoModal();
				//删除导入的 pLBDll
				//delete pLBDll;
				if (pLBDll != NULL)
				{
					delete pLBDll;
					pLBDll = NULL;
				}
				if (m_hDLL != NULL)
				{
					FreeLibrary(m_hDLL);
					m_hDLL = NULL;
				}
				continue;
			}
			else
			{

			}


		}else
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////////

		CDLLObject * curImportDLL = new CDLLObject(m_pDataAccess);

		//存储dell相关信息
		//名字
		curImportDLL->SetDllName(dllname);
		curImportDLL->SetDllOriginName(strDllOriginName);
		curImportDLL->SetDllPath(strDllPath);
		//dll 句柄
		curImportDLL->SetHinstance(m_hDLL);
		//dll 函数接口
		curImportDLL->SetFun(m_pf_Dll);
		curImportDLL->SetDllRunStatus(false);
		curImportDLL->SetLB(pLBDll);
		curImportDLL->SetTimeSpan(nTimeSpan);
		curImportDLL->SetMsgWnd(m_pWnd);
		curImportDLL->SetDllThreadName(strThreadName);
		curImportDLL->SetS3DBPath(m_strDBFileName.c_str());

		curImportDLL->SetDllVersion(curImportDLL->GetLB()->GetDllLogicVersion());
		curImportDLL->SetDllDescription(curImportDLL->GetLB()->GetDllDescription());
		//Author = pLBDll->GetDllAuthor();
		curImportDLL->SetDllAuthor(curImportDLL->GetLB()->GetDllAuthor());
		//显示出入日志
		SYSTEMTIME sysTime;
		CString strTime;
		::GetLocalTime(&sysTime);
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth,sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		curImportDLL->SetDllImportTime(strTime);

		//存储dll信息到DB
		if(!StoreDllToDB(curImportDLL))
			return false;

		//存储本次导入dll
		UpdateDLLParameter(curImportDLL);

		vecImportDLLList.push_back(curImportDLL);

		for(int j=0; j<m_vecDllThreadList.size(); j++)
		{
			if(m_vecDllThreadList[j]->GetName().c_str() == strThreadName)
			{
				m_vecDllThreadList[j]->AddDll(curImportDLL);
				break;;
			}
		}

	}

	return true;
}

bool CBEOPLogicManager::LoadAndSaveDll( std::vector<CString> PathList,std::vector<CString> DllList, std::vector<CString> strDllOriginNameList, std::vector<CString> strThreadNameList,std::vector<int> vecTimeSpan )
{
	if (PathList.empty()||DllList.empty() || strDllOriginNameList.empty())
	{
		return false;
	}
	std::vector<CString> vecSelDllName = DllList;
	std::vector<CString> strFileName   = PathList;

	for (int i=0;i<vecSelDllName.size();++i)
	{
		CString dllname = vecSelDllName[i];
		CString strDllOriginName = strDllOriginNameList[i];
		CString strThreadName = strThreadNameList[i];
		int		nTimeSpan = vecTimeSpan[i];
		//////////////////////////////////////////////////////////////////////////
		CString strDllPath = CopyDllFile(strFileName[i],dllname);
		//////////////////////////////////////////////////////////////////////////
		m_hDLL = NULL;
		m_hDLL = LoadLibrary(strDllPath);//加载动态链接库MyDll.dll文件；
		int nErr = GetLastError();
		if(m_hDLL==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("算法文件加载失败"), FALSE);
			WarnDlg.DoModal();
			return false;
		}
		m_pf_Dll = (pfnLogicBase)GetProcAddress(m_hDLL,"fnInitLogic");
		if(m_pf_Dll==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("未找到算法函数入口"), FALSE);
			WarnDlg.DoModal();
			FreeLibrary(m_hDLL);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//运行当前加载的dll函数
		CString Author = _T("");
		CLogicBase *pLBDll = m_pf_Dll();
		if(pLBDll != NULL)
		{
			CString dllver = pLBDll->GetSystVersion();
			CString gatewayver = GetSystVersion();
			double  ddllver = _ttof(dllver.Mid(1,dllver.GetLength()));
			double  dgatewayver = _ttof(gatewayver.Mid(1,gatewayver.GetLength()));

			if (ddllver < dgatewayver)
			{
				CWarningDlg WarnDlg;
				WarnDlg.InitWarn(_T("警告"), dllname+_T("文件版本不匹配！"), FALSE);
				WarnDlg.DoModal();
				//删除导入的 pLBDll
				//delete pLBDll;
				if (pLBDll != NULL)
				{
					delete pLBDll;
					pLBDll = NULL;
				}
				if (m_hDLL != NULL)
				{
					FreeLibrary(m_hDLL);
					m_hDLL = NULL;
				}
				continue;
			}
			else
			{

			}


		}else
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////////

		CDLLObject * curImportDLL = new CDLLObject(m_pDataAccess);

		//存储dell相关信息
		//名字
		curImportDLL->SetDllName(dllname);
		curImportDLL->SetDllOriginName(strDllOriginName);
		curImportDLL->SetDllPath(strDllPath);
		//dll 句柄
		curImportDLL->SetHinstance(m_hDLL);
		//dll 函数接口
		curImportDLL->SetFun(m_pf_Dll);
		curImportDLL->SetDllRunStatus(false);
		curImportDLL->SetLB(pLBDll);
		curImportDLL->SetTimeSpan(nTimeSpan);
		curImportDLL->SetMsgWnd(m_pWnd);
		curImportDLL->SetDllThreadName(strThreadName);
		curImportDLL->SetS3DBPath(m_strDBFileName.c_str());

		curImportDLL->SetDllVersion(curImportDLL->GetLB()->GetDllLogicVersion());
		curImportDLL->SetDllDescription(curImportDLL->GetLB()->GetDllDescription());
		//Author = pLBDll->GetDllAuthor();
		curImportDLL->SetDllAuthor(curImportDLL->GetLB()->GetDllAuthor());
		//显示出入日志
		SYSTEMTIME sysTime;
		CString strTime;
		::GetLocalTime(&sysTime);
		strTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth,sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		curImportDLL->SetDllImportTime(strTime);

		//存储dll信息到DB
		if(!StoreDllToDB(curImportDLL))
			return false;

		//存储本次导入dll
		UpdateDLLParameter(curImportDLL);

		vecImportDLLList.push_back(curImportDLL);

		for(int j=0; j<m_vecDllThreadList.size(); j++)
		{
			if(m_vecDllThreadList[j]->GetName().c_str() == strThreadName)
			{
				m_vecDllThreadList[j]->AddDll(curImportDLL);
				break;;
			}
		}
	}

	return true;
}

int CBEOPLogicManager::GetCurTimeSpanFromManager(CString strDllName)
{
	if (vecImportDLLList.empty())
	{
		return 20;
	}
	CDLLObject *pDllOb = NULL;
	for (int k = 0;k < vecImportDLLList.size();++k)
	{
		pDllOb = vecImportDLLList[k];
		if(pDllOb->GetDllName()==strDllName)
		{
			return pDllOb->GetTimeSpan();
		}
	}

	return 20;
}


bool CBEOPLogicManager::LoadDLLfromDB()
{
	if (vecImportDLLList.empty())
	{
		return false;
	}
	CDLLObject *pDllOb = NULL;
	for (int k = 0;k < vecImportDLLList.size();++k)
	{
		pDllOb = vecImportDLLList[k];
		m_hDLL = NULL;
		m_hDLL = LoadLibrary(pDllOb->GetDllPath());//加载动态链接库MyDll.dll文件；
		int nErr = GetLastError();
		if(m_hDLL==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("算法文件加载失败"), FALSE);
			WarnDlg.DoModal();
			continue ;
		}
		m_pf_Dll = (pfnLogicBase)GetProcAddress(m_hDLL,"fnInitLogic");

		if(m_pf_Dll==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("未找到算法函数入口"), FALSE);
			WarnDlg.DoModal();
			FreeLibrary(m_hDLL);

			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//运行当前加载的dll函数
		CLogicBase *pLBDll = m_pf_Dll();
		if(pLBDll != NULL)
		{
			CString dllver = pLBDll->GetSystVersion();
			CString gatewayver = GetSystVersion();
			double  ddllver = _ttof(dllver.Mid(1,dllver.GetLength()));
			double  dgatewayver = _ttof(gatewayver.Mid(1,gatewayver.GetLength()));

			if (ddllver < dgatewayver)
			{
				CWarningDlg WarnDlg;
				WarnDlg.InitWarn(_T("警告"), pDllOb->GetDllName() + _T("文件版本不匹配！"), FALSE);
				WarnDlg.DoModal();

				FreeLibrary(m_hDLL);

				continue ;
			}

			pLBDll->SetDllName(pDllOb->GetDllName());
			pLBDll->SetMsgWnd(m_pWnd);
			pLBDll->SetDataAccess(m_pDataAccess);

		}else
		{
			FreeLibrary(m_hDLL);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		CDLLObject * curImportDLL = pDllOb;
		//存储dell相关信息
		//dll 句柄
		curImportDLL->SetHinstance(m_hDLL);
		//dll 函数接口
		curImportDLL->SetFun(m_pf_Dll);
		curImportDLL->SetLB(pLBDll);
		curImportDLL->SetMsgWnd(m_pWnd);
		curImportDLL->SetDllRunStatus(false);

		curImportDLL->SetS3DBPath(m_strDBFileName.c_str());


	}
	return true;
}


bool CBEOPLogicManager::StoreDllToDB(CDLLObject* dllOb,bool batching)
{
	FILE *fp;
	long filesize = 0;
	char * ffile;

	if (batching)
	{
		m_wsdesFilePath = dllOb->GetDllPath();
	}

	_bstr_t tem_des = m_wsdesFilePath.c_str();


	char* des = (char*)tem_des;
	fopen_s(&fp,des, "rb");

	if(fp != NULL)
	{
		//计算文件的大小
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		//读取文件
		ffile = new char[filesize+1];
		size_t sz = fread(ffile, sizeof(char), filesize, fp);
		fclose(fp);
	}

	if(!InertDLLtoDB(dllOb,ffile,filesize))
		return false;

	if (ffile != NULL)
	{
		delete [] ffile;
	}
	return true;
}


bool CBEOPLogicManager::InertDLLtoDB(CDLLObject* dllOb,char *pBlock,int nSize)
{

	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}
	char szSQL[1024] = {0};
	memset(szSQL, 0, sizeof(szSQL));
	int rc = 0;
	int nID = 1;
	CString importtime = dllOb->GetDllImportTime();
	CString author = dllOb->GetDllAuthor();
	CString dllname = dllOb->GetDllName();

	wstring importime_ = importtime.GetString();
	_bstr_t tem_des = importime_.c_str();
	char* des = (char*)tem_des;
	std::string importtime__(des);

	wstring dllname_ = dllname.GetString();
	tem_des = dllname_.c_str();
	des = (char*)tem_des;
	std::string dllname__(des);

	wstring author_ = author.GetString();
	tem_des = author_.c_str();
	des = (char*)tem_des;
	std::string author__(des);

	//dll版本
	wstring version_ = dllOb->GetDllVersion();
	tem_des = version_.c_str();
	des = (char*)tem_des;
	std::string version__(des);
	//dll描述
	wstring description_ = dllOb->GetDllDescription();
	tem_des = description_.c_str();
	des = (char*)tem_des;
	std::string description__(des);

	//original dll name
	wstring strOriginalDllName = dllOb->GetDllOriginName();
	tem_des = strOriginalDllName.c_str();
	des = (char*)tem_des;
	std::string strOriginalDllName__(des);

	//original dll thread name
	wstring strThreadName = dllOb->GetDllThreadName();
	tem_des = strThreadName.c_str();
	des = (char*)tem_des;
	std::string strThreadName__(des);

	if(FindCharInString(L"'", description_) || FindCharInString(L"'", strOriginalDllName) || FindCharInString(L"'", author_) || FindCharInString(L"'", dllname_))
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("dll信息中含有符号',插入失败！"), FALSE);
		WarnDlg.DoModal();
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//删除已有的同名dll

	access.BeginTransaction();

	string sql_del;
	ostringstream sqlstream_del;
	sqlstream_del << "delete from list_dllstore where DllName = "<< "'" << dllname__ << "' and unitproperty04='" << strThreadName__ << "';"; 
	sql_del = sqlstream_del.str();
	if (SQLITE_OK == access.SqlExe(sql_del.c_str()))
	{
		;
	}
	//////////////////////////////////////////////////////////////////////////
	int runstatus = 1;
	string sql_;
	ostringstream sqlstream;
	string em = "";
	sqlstream << "insert into list_dllstore(id,DllName,importtime,author,periodicity,dllcontent,runstatus,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values('" << nID<< "','" << dllname__ << "','" \
		<< importtime__ << "','" << author__ << "','" << dllOb->GetTimeSpan() << "',?,'"<<runstatus<<"','"<<version__ <<"','"<<description__<<"','"<<strOriginalDllName__<<"','"<<strThreadName__<<"','"<<em<<"')";
	sql_ = sqlstream.str();

	bool bInsertOK = false;
	if (SQLITE_OK == access.SqlQuery(sql_.c_str()))
	{
		if (access.m_stmt)
		{
			rc = access.SqlBind_Blob(pBlock,1,nSize);
			assert(0 == rc);
			rc = access.SqlNext();
			//assert(0 == rc);
			rc = access.SqlFinalize();
			assert(0 == rc);
		}
		bInsertOK = true;
	}

	access.CommitTransaction();
	access.CloseDataBase();

	return bInsertOK;
}

bool CBEOPLogicManager::ThreadExitFinish()
{
	return m_bThreadExitFinished;
}


bool CBEOPLogicManager::SaveSpanTimetoDB(CString DllName,double SpanTime)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string sql_del;
	ostringstream sqlstream_del;
	wstring dllname_ = DllName.GetString();
	_bstr_t tem_des = dllname_.c_str();
	char* des = (char*)tem_des;
	std::string sdllname(des);

	sqlstream_del << "update list_dllstore set periodicity ="<< SpanTime <<" where DllName = "<< "'" << sdllname << "';"; 
	sql_del = sqlstream_del.str();
	access.SqlExe(sql_del.c_str());

	access.CloseDataBase();
	return true;
}


bool CBEOPLogicManager::UpdateDllInfoToDB()
{
	//软件关闭时更新数据库有关dll的信息

	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	access.BeginTransaction();
	for (int k = 0;k < vecImportDLLList.size();++k)
	{
		string sql_del;
		ostringstream sqlstream_del;
		wstring dllname_ = vecImportDLLList[k]->GetDllName().GetString();
		_bstr_t tem_des = dllname_.c_str();
		char* des = (char*)tem_des;
		std::string dllname__(des);
		int runstatus = vecImportDLLList[k]->GetDllRunStatus()?1:0;
		sqlstream_del << "update list_dllstore set periodicity ="<< vecImportDLLList[k]->GetTimeSpan()<<",runstatus = "<< runstatus <<" where DllName = "<< "'" << dllname__ << "';"; 
		sql_del = sqlstream_del.str();
		access.SqlExe(sql_del.c_str());
	}
	access.CommitTransaction();
	access.CloseDataBase();
	return true;
}

CDLLObject * CBEOPLogicManager::GetDLLObjectByName(CString dllname)
{

	for(int i =0;i<vecImportDLLList.size();i++)
	{
		if(vecImportDLLList[i]->GetDllName()==dllname)
		{
			
			return vecImportDLLList[i];
		}
	}

	return NULL;
}

bool CBEOPLogicManager::ExitDllByName(CString dllname)
{
	for(int i =0;i<vecImportDLLList.size();i++)
	{
		if(vecImportDLLList[i]->GetDllName()==dllname)
		{
			vecImportDLLList[i]->SetDllRunStatus(false);

			vecImportDLLList[i]->Exit();
			return true;
		}
	}

	return false;
}
bool CBEOPLogicManager::DeleteDllfromDB(CString dllname)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	string sql_del;
	ostringstream sqlstream_del;
	wstring dllname_ = dllname.GetString();
	_bstr_t tem_des = dllname_.c_str();
	char* des = (char*)tem_des;
	std::string dllname__(des);
	sqlstream_del << "delete from list_dllstore where DllName = "<< "'" << dllname__ << "';"; 
	sql_del = sqlstream_del.str();
	int re = access.SqlExe(sql_del.c_str());

	access.CloseDataBase();

	if (SQLITE_OK == re)
	{
		return true;
	}

	return false;
}

bool CBEOPLogicManager::ReadDLLfromDB(const wstring &s3dbpath,std::vector<CDLLObject*> &dllObList)
{
	string strUtf8;
	Project::Tools::WideCharToUtf8(s3dbpath, strUtf8);
	CSqliteAcess access(s3dbpath.c_str(), m_bEncry);

	ostringstream sqlstream;
	sqlstream << "select * from list_dllstore;";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		int id = access.getColumn_Int(0);
		CString dllname		= _T("");
		CString importtime  = _T("");
		CString author      = _T("");
		CString version		= _T("");
		CString description = _T("");
		CString dlloriginname = _T("");
		CString threadname = _T("default");

		

		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(name_temp);
			dllname = ss.c_str();
		}
		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   t_temp(const_cast<char*>(access.getColumn_Text(2)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(t_temp);
			importtime = ss.c_str();
		}
		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   a_temp(const_cast<char*>(access.getColumn_Text(3)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(a_temp);
			author = ss.c_str();
		}

		double timespan = access.getColumn_Double(4);

		const void* pf = access.getColumn_Blob(5);
		int nSize      = access.getColumn_Bytes(5);	
		int runstatus  = access.getColumn_Int(6);


		if (const_cast<char*>(access.getColumn_Text(7)) !=NULL)
		{
			string   v_temp(const_cast<char*>(access.getColumn_Text(7)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(v_temp);
			version = ss.c_str();
		}
		if (const_cast<char*>(access.getColumn_Text(8)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(8)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(d_temp);
			description = ss.c_str();
		}
		if (const_cast<char*>(access.getColumn_Text(9)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(9)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(d_temp);
			dlloriginname = ss.c_str();
		}
		if (const_cast<char*>(access.getColumn_Text(10)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(10)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(d_temp);
			threadname = ss.c_str();
			if(threadname.GetLength()<=0)
				threadname = L"default";
		}

		if(dllname.Find(_T("[Thread:]"))>=0)//回避错误的文件内容
		{
			continue;
		}


		CDLLObject* pDllOb = new CDLLObject(m_pDataAccess);

		pDllOb->SetDllName(dllname);
		pDllOb->SetDllAuthor(author);
		pDllOb->SetDllImportTime(importtime);
		pDllOb->SetTimeSpan(timespan);
		pDllOb->SetDllRunStatus(false);//初始化时都不能运行
		pDllOb->SetDllVersion(version);
		pDllOb->SetDllDescription(description);
		pDllOb->SetDllOriginName(dlloriginname);
		pDllOb->SetDllThreadName(threadname);

		dllObList.push_back(pDllOb);
		//////////////////////////////////////////////////////////////////////////
		const char* pff = (char*)pf;
		//从数据库读取文件
		SaveMemToFile(pff,nSize,dllname,pDllOb);
	}
	access.SqlFinalize();
	access.CloseDataBase();
	return true;

}

void CBEOPLogicManager::ReadDLLMarkfromDB(const wstring &s3dbpath,std::vector<CString> &vecMark)
{
	string strUtf8;
	Project::Tools::WideCharToUtf8(s3dbpath, strUtf8);
	CSqliteAcess access(s3dbpath.c_str(), m_bEncry);

	ostringstream sqlstream;
	sqlstream << "SELECT DISTINCT unitproperty03 FROM list_dllstore";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);

	CString strDllOriginName;
	while (true)
	{
		if (SQLITE_ROW != access.SqlNext())
		{
			break;
		}

		if (const_cast<char*>(access.getColumn_Text(0)) != NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(0)));
			wstring ss = UtilString::ConvertMultiByteToWideChar(d_temp);
			strDllOriginName = ss.c_str();
		}

		if (strDllOriginName != _T(""))
		{
			vecMark.push_back(strDllOriginName);
		}
	}

	access.SqlFinalize();
	access.CloseDataBase();
}

BOOL CBEOPLogicManager::ClearFile(CDLLObject *pObject)
{
	CString  tempPath;
	Project::Tools::GetSysPath(tempPath);
	tempPath += _T("\\importdll");

	const wchar_t* lpszFolderPath =  tempPath.GetString();
	if (!FolderExist(lpszFolderPath))
	{
		return TRUE;
	}

	CString strFileName;
	strFileName.Format(L"%s\\%s",lpszFolderPath,pObject->GetDllName());
	if (FileExist(strFileName))
	{
		bool bDelete = DeleteFile(strFileName);
		if(bDelete==false)
		{
			int xstop = 0;
		}
	}

	return TRUE;
}

BOOL CBEOPLogicManager::SaveMemToFile( const char* pBlock, const int nSize, CString szFileName,CDLLObject* pDllOb)
{
	CString  tempPath;
	Project::Tools::GetSysPath(tempPath);
	tempPath += _T("\\importdll");

	const wchar_t* lpszFolderPath =  tempPath.GetString();
	if (!FolderExist(lpszFolderPath))
	{
		CreateFolder(lpszFolderPath);
	}
	CString strFileName;
	strFileName.Format(L"%s\\%s",lpszFolderPath,szFileName);
	if (FileExist(strFileName))
	{
		bool bDelete = DeleteFile(strFileName);
		if(bDelete==false)
			int xstop = 0;
	}
	pDllOb->SetDllPath(strFileName);
	//写入文件
	FILE* pFile = NULL;
	const errno_t rs = _wfopen_s(&pFile, strFileName.GetString(), L"wb");
	assert(0 == rs);
	assert(pFile);
	if(pFile)
	{
		fseek(pFile, 0, SEEK_SET);
		fwrite(pBlock, nSize, 1, pFile);
		fclose(pFile);
	}
	

	return TRUE;
}


bool CBEOPLogicManager::SaveLogicThreadNameToDB(CString strDllName, CString strThreadName)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string sql_del;
	ostringstream sqlstream_del;
	wstring dllname_ = strDllName.GetString();
	_bstr_t tem_des = dllname_.c_str();
	char* des = (char*)tem_des;
	std::string sdllname(des);

	wstring wstrthreadname = strThreadName.GetString();
	_bstr_t tem_des1 = wstrthreadname.c_str();
	char* des1 = (char*)tem_des1;
	std::string sThreadName(des1);

	sqlstream_del << "update list_dllstore set unitproperty04 ='"<< sThreadName <<"' where DllName = "<< "'" << sdllname << "';"; 
	sql_del = sqlstream_del.str();
	access.SqlExe(sql_del.c_str());

	access.CloseDataBase();
	return true;
}


bool CBEOPLogicManager::SaveParametertoDB(int nInOrOutput, CString strDllName,  CString strVariableName, CString strLinkDefine, CString strLinkType)
{

	string strUtf8;
	Project::Tools::WideCharToUtf8(m_strDBFileName, strUtf8);
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}

	//dll name
	wstring dllname = strDllName.GetString();
	_bstr_t tem_des = dllname.c_str();
	char* des = (char*)tem_des;
	std::string dllname__(des);


	//检测该参数是否已经存在
	int nIN = 0;
	bool bAllSuccess = true;
	access.BeginTransaction();
	std::vector<CString> onerecord;

	string sql_in;
	ostringstream sqlstream;

	//变量名
	wstring vname_ = strVariableName;
	tem_des = vname_.c_str();
	des = (char*)tem_des;
	std::string vname__(des);

	//点名
	wstring pointname_ = strLinkDefine;
	tem_des = pointname_.c_str();
	des = (char*)tem_des;
	std::string pointname__(des);

	//类型
	wstring ptype = strLinkType;
	tem_des = ptype.c_str();
	des = (char*)tem_des;
	std::string ptype__(des);

	std::string em("");

	sqlstream << "update list_paramterConfig set pname = '"<<pointname__<<"', ptype = '"<<ptype__<<"' where vname = '"<< vname__<<"' and DllName = '"<<dllname__ <<"' and INorOut = "<<nInOrOutput<<";";
	sql_in = sqlstream.str();
		
	int re = access.SqlExe(sql_in.c_str());

	if (re != SQLITE_OK)
	{
		bAllSuccess =false;
	}
	

	if (access.CommitTransaction() != SQLITE_OK || !bAllSuccess)
	{
		CWarningDlg WarnDlg;

		CString strTemp;
		strTemp.Format(_T("%s 策略保存通用待条件参数时存储参数失败"),strDllName);

		WarnDlg.InitWarn(_T("警告"),strTemp, FALSE);
		WarnDlg.DoModal();
	}
	
	access.CloseDataBase();
	return bAllSuccess;
}

bool CBEOPLogicManager::SaveParametertoDB( int nInOrOutput, CString strDllName, CString strVariableName, CString strLinkDefine, CString strLinkType, CString strCondition)
{
	string strUtf8;
	Project::Tools::WideCharToUtf8(m_strDBFileName, strUtf8);
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}

	//dll name
	wstring dllname = strDllName.GetString();
	_bstr_t tem_des = dllname.c_str();
	char* des = (char*)tem_des;
	std::string dllname__(des);


	//检测该参数是否已经存在
	int nIN = 0;
	bool bAllSuccess = true;
	access.BeginTransaction();
	std::vector<CString> onerecord;

	string sql_in;
	ostringstream sqlstream;

	//变量名
	wstring vname_ = strVariableName;
	tem_des = vname_.c_str();
	des = (char*)tem_des;
	std::string vname__(des);

	//点名
	wstring pointname_ = strLinkDefine;
	tem_des = pointname_.c_str();
	des = (char*)tem_des;
	std::string pointname__(des);

	//类型
	wstring ptype = strLinkType;
	tem_des = ptype.c_str();
	des = (char*)tem_des;
	std::string ptype__(des);

	//条件
	wstring condition = strCondition;
	tem_des = condition.c_str();
	des = (char*)tem_des;
	std::string condition__(des);

	std::string em("");

	sqlstream << "update list_paramterConfig set pname = '"<<pointname__<<"', ptype = '"<<ptype__<<"',unitproperty01='"<<condition__<<"' where vname = '"<< vname__<<"' and DllName = '"<<dllname__ <<"' and INorOut = "<<nInOrOutput<<";";
	sql_in = sqlstream.str();

	int re = access.SqlExe(sql_in.c_str());

	if (re != SQLITE_OK)
	{
		string strERROR0 = access.GetSqliteErr();
		bAllSuccess =false;
	}


	if (access.CommitTransaction() != SQLITE_OK || !bAllSuccess)
	{
		string strERROR = access.GetSqliteErr();
		CWarningDlg WarnDlg;

		CString strTemp;
		strTemp.Format(_T("%s 策略保存通用参数时存储参数失败"),strDllName);

		WarnDlg.InitWarn(_T("警告"), strTemp, FALSE);
		WarnDlg.DoModal();
	}

	access.CloseDataBase();
	return bAllSuccess;
}

bool CBEOPLogicManager::LoadDllThreadRelationship(CLogicDllThread *pDllThread)
{
	int i=0, j=0;
	for(i=0;i<pDllThread->GetDllCount();i++)
	{
		CDLLObject *pObject=  pDllThread->GetDllObject(i);
		std::vector<vec_wstr> strParaList;
		pObject->ClearParameters();

		GetInputParameterfromDB(pObject->GetDllName(),strParaList);
		for(j=0;j< strParaList.size();j++)
		{//[5]("fChRatePower","350.000","const","冷机默认功率","")
			CLogicParameter *pp = NULL;
			vec_wstr strPara = strParaList[j];
			if(strPara[2]==L"const" || strPara[2]==L"strconst")
			{
				pp = new CLogicParameterConst(strPara[0],0, strPara[2], m_pDataAccess, strPara[1]);
			}
			
			else
			{
				pp = new CLogicParameterPoint(strPara[0],0, strPara[2], m_pDataAccess, strPara[1]);
			}
			pObject->m_vecCurDllInputParameterList.push_back(pp);
		}

		//read output
		std::vector<vec_wstr> strOutputParaList;
		GetOutputParameterfromDB(pObject->GetDllName(),strOutputParaList);
		for(j=0;j< strOutputParaList.size();j++)
		{
			CLogicParameter *pp = NULL;
			vec_wstr strPara = strOutputParaList[j];
			if(strPara[2]==L"const" || strPara[2]==L"strconst")
			{
				pp = new CLogicParameterConst(strPara[0],1, strPara[2], m_pDataAccess, strPara[1]);
			}
			else
			{
				pp =  new CLogicParameterPoint(strPara[0],1, strPara[2], m_pDataAccess, strPara[1]);
			}
			pObject->m_vecCurDllOutputParameterList.push_back(pp);
		}

	}

	return true;
}

CDLLObject * CBEOPLogicManager::FindDLLObject(CString strDllName)
{
	for(int i=0;i<vecImportDLLList.size();i++)
	{
		if(vecImportDLLList[i]->GetDllName() == strDllName)
			return vecImportDLLList[i];
	}

	return NULL;
}


bool CBEOPLogicManager::GetInputParameterfromDB(CString strDllName,  std::vector<vec_wstr> &inputPara )
{
	string strUtf8;
	Project::Tools::WideCharToUtf8(m_strDBFileName, strUtf8);
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}

	//dll name
	wstring dllname = strDllName.GetString();
	_bstr_t tem_des = dllname.c_str();
	char* des = (char*)tem_des;
	std::string dllname__(des);

	ostringstream sqlstream;
	sqlstream << "select vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05 from list_paramterConfig where INorOut = 0 and DllName = '"<< dllname__ <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	if (re != SQLITE_OK)
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("读取数据库输入参数失败！"), FALSE);
		WarnDlg.DoModal();
	}
	inputPara.clear();
	std::vector<wstring> onerecord;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		onerecord.clear();
		if(access.getColumn_Text(0) != NULL)
		{
			string  vname(const_cast<char*>(access.getColumn_Text(0)));
			wstring vname_ = UtilString::ConvertMultiByteToWideChar(vname);
			onerecord.push_back(vname_);
		}
		if(access.getColumn_Text(1) != NULL)
		{
			string  pname(const_cast<char*>(access.getColumn_Text(1)));
			wstring pname_ = UtilString::ConvertMultiByteToWideChar(pname);
			onerecord.push_back(pname_);
		}

		if(access.getColumn_Text(2) != NULL)
		{
			string  ptype(const_cast<char*>(access.getColumn_Text(2)));
			wstring ptype_ = UtilString::ConvertMultiByteToWideChar(ptype);
			onerecord.push_back(ptype_);
		}

		if(access.getColumn_Text(3) != NULL)
		{
			string  pexplain(const_cast<char*>(access.getColumn_Text(3)));
			wstring pexplain_ = UtilString::ConvertMultiByteToWideChar(pexplain);
			onerecord.push_back(pexplain_);
		}
		
		if(access.getColumn_Text(7) != NULL)
		{
			string  pcanmodifytype(const_cast<char*>(access.getColumn_Text(7)));
			wstring wstrpcanmodifytype = UtilString::ConvertMultiByteToWideChar(pcanmodifytype);
			onerecord.push_back(wstrpcanmodifytype);
		}

		inputPara.push_back(onerecord);
	}
	access.SqlFinalize();
	access.CloseDataBase();
	return true;
}

bool CBEOPLogicManager::GetOutputParameterfromDB(CString strDllName,  std::vector<vec_wstr> &outputPara )
{
	string strUtf8;
	Project::Tools::WideCharToUtf8(m_strDBFileName, strUtf8);
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}
	//dll name
	wstring dllname = strDllName.GetString();
	_bstr_t tem_des = dllname.c_str();
	char* des = (char*)tem_des;
	std::string dllname__(des);

	ostringstream sqlstream;
	sqlstream << "select vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05 from list_paramterConfig where INorOut = 1 and DllName = '"<< dllname__<<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	if (re != SQLITE_OK)
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("读取数据库输出参数失败！"), FALSE);
		WarnDlg.DoModal();
	}
	outputPara.clear();
	std::vector<wstring> onerecord;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		onerecord.clear();
		if(access.getColumn_Text(0) != NULL)
		{
			string  vname(const_cast<char*>(access.getColumn_Text(0)));
			wstring vname_ = UtilString::ConvertMultiByteToWideChar(vname);
			onerecord.push_back(vname_);
		}
		if(access.getColumn_Text(1) != NULL)
		{
			string  pname(const_cast<char*>(access.getColumn_Text(1)));
			wstring pname_ = UtilString::ConvertMultiByteToWideChar(pname);
			onerecord.push_back(pname_);
		}

		if(access.getColumn_Text(2) != NULL)
		{
			string  ptype(const_cast<char*>(access.getColumn_Text(2)));
			wstring ptype_ = UtilString::ConvertMultiByteToWideChar(ptype);
			onerecord.push_back(ptype_);
		}

		if(access.getColumn_Text(3) != NULL)
		{
			string  pexplain(const_cast<char*>(access.getColumn_Text(3)));
			wstring pexplain_ = UtilString::ConvertMultiByteToWideChar(pexplain);
			onerecord.push_back(pexplain_);
		}

		if(access.getColumn_Text(6) != NULL)
		{
			string  pexplain(const_cast<char*>(access.getColumn_Text(6)));
			wstring pexplain_ = UtilString::ConvertMultiByteToWideChar(pexplain);
			onerecord.push_back(pexplain_);
		}

		outputPara.push_back(onerecord);
	}
	access.SqlFinalize();
	access.CloseDataBase();
	return true;
}

bool CBEOPLogicManager::CompareInputParameter( std::vector<vec_wstr> &inputPara ,std::vector<vec_wstr> &Ini_inputPara)
{

	for (int i=0;i<Ini_inputPara.size();++i)
	{
		for (int j=0;j<inputPara.size();++j)
		{
			if (Ini_inputPara[i][0] == inputPara[j][0])
			{
				Ini_inputPara[i] = inputPara[j];
				break;
			}
		}
	}

	return true;
}

bool CBEOPLogicManager::CompareOutputParameter( std::vector<vec_wstr> &outputPara ,std::vector<vec_wstr> &Ini_outputPara)
{

	for (int i=0;i<Ini_outputPara.size();++i)
	{
		for (int j=0;j<outputPara.size();++j)
		{
			if (Ini_outputPara[i][0] == outputPara[j][0])
			{
				Ini_outputPara[i] = outputPara[j];
				break;
			}
		}
	}

	return true;
}
std::vector<vec_wstr> CBEOPLogicManager::SeparateParameter( CString strLongPatameter )
{

	std::vector<vec_wstr> MatrixPara;
	

	if(strLongPatameter.Find(_T("$"))>0)
	{//第二版的字符串

		std::vector<wstring> strItems;
		Project::Tools::SplitStringByChar(strLongPatameter,'$', strItems);

		TCHAR Sep = '/';
		CString P = _T("");
		for (int i=0;i<strItems.size();++i)
		{

			std::vector<wstring> OneGroup;
			Project::Tools::SplitStringByChar(strItems[i].c_str(),'/', OneGroup);
			MatrixPara.push_back(OneGroup);		
		}
	}
	else
	{
		TCHAR Sep = '/';
		CString P = _T("");
			std::vector<wstring> OneGroup;

		for (int i=0;i<strLongPatameter.GetLength();++i)
		{
			if (strLongPatameter.GetAt(i) == Sep)
			{
				OneGroup.push_back(P.GetString());
				P = _T("");
				if (OneGroup.size() == 4)
				{
					MatrixPara.push_back(OneGroup);
					OneGroup.clear();
				}
			}else
			{
				CString ii(strLongPatameter.GetAt(i));
				P += ii;
			}
		}
	}

	

	return MatrixPara;
}

wstring CBEOPLogicManager::LinkParameter( std::vector<vec_wstr> &matrixpara )
{
	wstring strLongPara = _T("");
	for (int i=0;i<matrixpara.size();++i)
	{
		strLongPara += matrixpara[i][0]+_T("/")+matrixpara[i][1]+_T("/")+matrixpara[i][2]+_T("/")+matrixpara[i][3]+_T("/")+matrixpara[i][4];
		if((i+1)<matrixpara.size())
			strLongPara+=_T("$");
	}
	return strLongPara;
}

bool CBEOPLogicManager::RunDLL( CString dllname,bool runstart )
{
	if (vecImportDLLList.empty())
	{
		return false;
	}
	CDLLObject *pDllOb = NULL;
	for (int k = 0;k < vecImportDLLList.size();++k)
	{
		if (vecImportDLLList[k]->GetDllName() != dllname )
		{
			continue;
		}

		pDllOb = vecImportDLLList[k];
		m_hDLL = NULL;
		m_hDLL = LoadLibrary(pDllOb->GetDllPath());//加载动态链接库MyDll.dll文件；
		int nErr = GetLastError();
		if(m_hDLL==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("算法文件加载失败"), FALSE);
			WarnDlg.DoModal();
			continue ;
		}
		m_pf_Dll = (pfnLogicBase)GetProcAddress(m_hDLL,"fnInitLogic");

		if(m_pf_Dll==NULL)
		{
			CWarningDlg WarnDlg;
			WarnDlg.InitWarn(_T("警告"), _T("未找到算法函数入口"), FALSE);
			WarnDlg.DoModal();
			FreeLibrary(m_hDLL);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//运行当前加载的dll函数
		CLogicBase *pLBDll = m_pf_Dll();
		if(pLBDll != NULL)
		{

			CString dllver = pLBDll->GetSystVersion();
			CString gatewayver = GetSystVersion();
			double  ddllver = _ttof(dllver.Mid(1,dllver.GetLength()));
			double  dgatewayver = _ttof(gatewayver.Mid(1,gatewayver.GetLength()));

			if (ddllver < dgatewayver)
			{
				CWarningDlg WarnDlg;
				WarnDlg.InitWarn(_T("警告"), pDllOb->GetDllName()+_T("文件版本不匹配！"), FALSE);
				WarnDlg.DoModal();
				return false ;
			}

			pLBDll->SetDllName(pDllOb->GetDllName());
			pLBDll->SetMsgWnd(m_pWnd);
			pLBDll->SetDataAccess(m_pDataAccess);

		}else
		{
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		CDLLObject * curImportDLL = pDllOb;
		//存储dell相关信息
		//dll 句柄
		curImportDLL->SetHinstance(m_hDLL);
		//dll 函数接口
		curImportDLL->SetFun(m_pf_Dll);
		curImportDLL->SetLB(pLBDll);
		curImportDLL->SetMsgWnd(m_pWnd);

		curImportDLL->SetS3DBPath(m_strDBFileName.c_str());

		if (runstart)
		{
			curImportDLL->SetDllRunStatus(true);
		}else
		{
			curImportDLL->SetDllRunStatus(false);
		}
	}
	return true;
}


void CBEOPLogicManager::SortDllName( std::vector<CString> &iniDllList,std::vector<vec_wstr> &DllList )
{
	if (iniDllList.empty() || DllList.empty())
	{
		return;
	}
	std::vector<vec_wstr> temList(DllList);
	std::vector<int>  label(DllList.size(),1);

	DllList.clear();
	for (int i= 0;i<iniDllList.size();++i)
	{
		for (int j = 0;j<temList.size();++j)
		{
			wstring strcomp = iniDllList[i].GetString();
			if ( strcomp == temList[j][0])
			{
				DllList.push_back(temList[j]);
				label[j] = 0;
				break;
			}
		}
	}

	for (int i = 0;i<temList.size();++i)
	{
		if (label[i] == 1)
		{
			DllList.push_back(temList[i]);
		}
	}

	return;
}


void CBEOPLogicManager::LoadNewConfigFromS3DB( CString strNewDBFileName )
{
	//Read list_dllstore
	wstring strNewName = strNewDBFileName.GetString();

	string strUtf8;
	Project::Tools::WideCharToUtf8(strNewName, strUtf8);


	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	ReadDLLfromDB(strNewName,vecImportDLLList);

	//Read list_pointwatch
	ostringstream sqlstream;
	sqlstream.str("");
	sqlstream << "select * from list_pointwatch;";
	string sql_ = sqlstream.str();
	char * ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	vector<sPointWatch> vecPointWatch;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		sPointWatch sPoint;
		sPoint.nID = access.getColumn_Int(0);

		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			sPoint.strName = name_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(2)) !=NULL)
		{
			string   t_temp(const_cast<char*>(access.getColumn_Text(2)));
			sPoint.strContent = t_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(3)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(3)));
			sPoint.unitproperty01 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(4)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(4)));
			sPoint.unitproperty02 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(5)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(5)));
			sPoint.unitproperty03 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(6)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(6)));
			sPoint.unitproperty04 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(7)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(7)));
			sPoint.unitproperty05 = d_temp;
		}

		vecPointWatch.push_back(sPoint);	
	}
	access.SqlFinalize();

	//Read list_paramterConfig
	sqlstream.str("");
	sqlstream << "select vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05 from list_paramterConfig;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	vector<sParamConfig> vecParamConfig;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		sParamConfig sParam;

		if (const_cast<char*>(access.getColumn_Text(0)) !=NULL)
		{
			string   vname_temp(const_cast<char*>(access.getColumn_Text(0)));
			sParam.strVName = vname_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   pname_temp(const_cast<char*>(access.getColumn_Text(1)));
			sParam.strPName = pname_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(2)) !=NULL)
		{
			string   pt_temp(const_cast<char*>(access.getColumn_Text(2)));
			sParam.strPType = pt_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(3)) !=NULL)
		{
			string   v_temp(const_cast<char*>(access.getColumn_Text(3)));
			sParam.strVexplain = v_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(4)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(4)));
			sParam.strDllName = d_temp;
		}

		sParam.nINorOut = access.getColumn_Text(5);

		if (const_cast<char*>(access.getColumn_Text(6)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(6)));
			sParam.unitproperty01 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(7)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(7)));
			sParam.unitproperty02 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(8)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(8)));
			sParam.unitproperty03 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(9)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(9)));
			sParam.unitproperty04 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(10)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(10)));
			sParam.unitproperty05 = d_temp;
		}

		vecParamConfig.push_back(sParam);	
	}
	access.SqlFinalize();
	access.CloseDataBase();

	CSqliteAcess accessOld(m_strDBFileName.c_str(), m_bEncry);


	//删除list_pointwatch
	sqlstream.str("");
	sqlstream << "delete from list_pointwatch;"; 
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	if (SQLITE_OK == accessOld.SqlExe(ex_sql))
	{
		;
	}

	//插入list_pointwatch
	accessOld.BeginTransaction();
	for(int i=0; i<vecPointWatch.size(); i++)
	{
		sqlstream.str("");
		sPointWatch sPoint = vecPointWatch[i];
		sqlstream << "insert into list_pointwatch(id,name,content,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values(" << sPoint.nID<< ",'" << sPoint.strName << "','"<< sPoint.strContent <<"','"<<sPoint.unitproperty01 <<"','"<<sPoint.unitproperty02<<"','"<<sPoint.unitproperty03<<"','"<<sPoint.unitproperty04<<"','"<<sPoint.unitproperty05<<"')";
		string sql_in = sqlstream.str();
		accessOld.SqlExe(sql_in.c_str());
	}
	accessOld.CommitTransaction();

	//删除list_paramterConfig
	sqlstream.str("");
	sqlstream << "delete from list_paramterConfig;"; 
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	if (SQLITE_OK == accessOld.SqlExe(ex_sql))
	{
		;
	}
	//插入list_paramterConfig

	accessOld.BeginTransaction();
	for(int i=0; i<vecParamConfig.size(); i++)
	{
		sqlstream.str("");
		sParamConfig sParam = vecParamConfig[i];
		sqlstream << "insert into list_paramterConfig(vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values('" << sParam.strVName<< "','" << sParam.strPName << "','"<< sParam.strPType<< "','" << sParam.strVexplain << "','" << sParam.strDllName<< "','" << sParam.nINorOut \
			<<"','"<<sParam.unitproperty01 <<"','"<<sParam.unitproperty02<<"','"<<sParam.unitproperty03<<"','"<<sParam.unitproperty04<<"','"<<sParam.unitproperty05<<"')";

		string sql_in = sqlstream.str();
		accessOld.SqlExe(sql_in.c_str());
	}
	accessOld.CommitTransaction();
	accessOld.CloseDataBase();
}


void CBEOPLogicManager::FileCopyTo(CString source, CString destination, CString searchStr, BOOL cover)
{
	CString strSourcePath = source;
	CString strDesPath = destination;
	CString strFileName = searchStr;
	CFileFind filefinder;
	CString strSearchPath = strSourcePath + "//" + strFileName;
	CString filename;
	BOOL bfind = filefinder.FindFile(strSearchPath);
	CString SourcePath, DisPath;
	while (bfind)
	{
		bfind = filefinder.FindNextFile();
		filename = filefinder.GetFileName();
		SourcePath = strSourcePath + "//" + filename;
		DisPath = strDesPath + "//" + filename;
		CopyFile(SourcePath.GetString(),DisPath.GetString(),cover);
	}

	filefinder.Close();
}


CString CBEOPLogicManager::CopyDllFile(CString selFilePath,CString DllName)
{
	wstring m_wstrFilePath;
	Project::Tools::GetSysPath(m_wstrFilePath);
	m_wstrFilePath += '\\';
	m_wstrFilePath += L"importdll";
	CString dec(m_wstrFilePath.c_str());
	if (!FolderExist(dec))
	{
		CreateFolder(dec);
	}
	dec +="\\";
	dec += DllName;
	m_wsdesFilePath = dec.GetString();
	BOOL bCopied = CopyFile(selFilePath.GetString(),dec.GetString(),false);

	return dec;
}



BOOL CBEOPLogicManager::FolderExist(CString strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind!=INVALID_HANDLE_VALUE) &&
		(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}


BOOL CBEOPLogicManager::CreateFolder(CString strPath)
{
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	return ::CreateDirectory(strPath, &attrib);
}

BOOL CBEOPLogicManager::FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName);
}

bool CBEOPLogicManager::DeleteFromImportList( std::vector<CString> DllList )
{
	if(DllList.empty())
		return false;

	int i = 0, j=0;

	for(i=0;i<DllList.size();i++)
	{
		for (j=0;j<vecImportDLLList.size();++j)
		{
			if (vecImportDLLList[j]->GetDllName()== DllList[i])
			{
				delete(vecImportDLLList[j]);
				vecImportDLLList.erase(vecImportDLLList.begin()+j);
				j--;
			}

		}

	}
	return true;
}

bool CBEOPLogicManager::DeleteFromImportList( std::vector<CString> DllList,CString strThreadName )
{
	if(DllList.empty())
		return false;

	for(int m=0; m<m_vecDllThreadList.size(); m++)
	{
		if(m_vecDllThreadList[m]->GetName().c_str() == strThreadName)
		{
			for(int n=0;n<DllList.size();n++)
			{
				for(int l=0; l<m_vecDllThreadList[m]->GetDllCount(); l++)
				{
					if(DllList[n] == m_vecDllThreadList[m]->GetDllObject(l)->GetDllName())
					{
						m_vecDllThreadList[m]->EaraseDll(l);
					}
				}
			}
			break;
		}
	}

	int i = 0, j=0;
	for(i=0;i<DllList.size();i++)
	{
		for (j=0;j<vecImportDLLList.size();++j)
		{
			if (vecImportDLLList[j]->GetDllName()== DllList[i])
			{
				delete(vecImportDLLList[j]);
				vecImportDLLList.erase(vecImportDLLList.begin()+j);
				j--;
			}

		}
	}
	return true;
}

bool CBEOPLogicManager::UpdateDLLParameter( CDLLObject* dllOb )
{

	if (dllOb == NULL)
	{
		return false;
	}
	CString strLongInputPara = dllOb->GetLB()->GetAllInputParameter();
	CString strLongOutputPara = dllOb->GetLB()->GetAllOutputParameter();
	m_vecCurDllInputParameterList  = SeparateParameter(strLongInputPara);
	m_vecCurDllOutputParameterList = SeparateParameter(strLongOutputPara);

	for (int i=0;i<m_vecCurDllOutputParameterList.size();++i)
	{
		if (m_vecCurDllOutputParameterList[i].size()<5)
		{
			m_vecCurDllOutputParameterList[i].push_back(_T("1"));
		}
	}

	std::vector<vec_wstr> inputDB,outputDB;
	GetInputParameterfromDB(dllOb->GetDllName(),inputDB);
	GetOutputParameterfromDB(dllOb->GetDllName(),outputDB);
	CompareInputParameter(inputDB,m_vecCurDllInputParameterList);
	CompareOutputParameter(outputDB,m_vecCurDllOutputParameterList);

	for (int i=0;i<m_vecCurDllInputParameterList.size();++i)
	{
		dllOb->GetLB()->SetInputValue(1,m_vecCurDllInputParameterList[i][1].c_str());
	}

//////////////////////////////////////////////////////////////////////////
	std::vector<vec_str> vecCurDllInputParameterList_temp,vecCurDllOutputParameterList_temp;
	for (int i=0;i<m_vecCurDllInputParameterList.size();++i)
	{
		std::vector<CString> temp;
		for (int j = 0;j<m_vecCurDllInputParameterList[i].size();++j)
		{
			temp.push_back(m_vecCurDllInputParameterList[i][j].c_str());
		}

		vecCurDllInputParameterList_temp.push_back(temp);
	}
	for (int i=0;i<m_vecCurDllOutputParameterList.size();++i)
	{
		std::vector<CString> temp;
		for (int j = 0;j<m_vecCurDllOutputParameterList[i].size();++j)
		{
			temp.push_back(m_vecCurDllOutputParameterList[i][j].c_str());
		}

		vecCurDllOutputParameterList_temp.push_back(temp);
	}

//////////////////////////////////////////////////////////////////////////
	SaveInputParametertoDB(dllOb->GetDllName(),vecCurDllInputParameterList_temp);
	SaveOutputParametertoDB(dllOb->GetDllName(),vecCurDllOutputParameterList_temp);
	return true;
}


bool CBEOPLogicManager::SaveInputParametertoDB(CString strDllName,  std::vector<vec_str> &strLongPara )
{

	std::vector<vec_str> inputPara = strLongPara;

	//string strUtf8;
	//Project::Tools::WideCharToUtf8(m_strDBFileName, strUtf8);

	//CSqliteAcess access(strUtf8);


	//wstring strNewName = strNewDBFileName.GetString();

	//string strUtf8;
	//Project::Tools::WideCharToUtf8(strNewName, strUtf8);


	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);


	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("策略风险警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}

	//dll name
	wstring dllname = strDllName.GetString();
	_bstr_t tem_des = dllname.c_str();
	char* des = (char*)tem_des;
	std::string dllname__(des);

	string sql_del;
	ostringstream sqlstream_del;
	sqlstream_del << "delete from list_paramterConfig where INorOut = 0 and DllName = '"<< dllname__ <<"';";
	sql_del = sqlstream_del.str();
	if (SQLITE_OK == access.SqlExe(sql_del.c_str()))
	{
		;
	}
	int nIN = 0;
	access.BeginTransaction();
	std::vector<CString> onerecord;
	for (int k = 0;k < inputPara.size();++k)
	{
		string sql_in;
		ostringstream sqlstream;
		onerecord = inputPara[k];

		//变量名
		wstring vname_ = onerecord[0];
		_bstr_t tem_des = vname_.c_str();
		char* des = (char*)tem_des;
		std::string vname__(des);

		//点名
		wstring pointname_ = onerecord[1];
		tem_des = pointname_.c_str();
		des = (char*)tem_des;
		std::string pointname__(des);

		//类型
		wstring ptype = onerecord[2];
		tem_des = ptype.c_str();
		des = (char*)tem_des;
		std::string ptype__(des);

		//说明
		wstring pexplain = onerecord[3];
		tem_des = pexplain.c_str();
		des = (char*)tem_des;
		std::string pexplain__(des);

		//能否修改参数类型
		std::string strCanTypeModify("");
		if(onerecord.size()>=5)
		{
			wstring pexplain = onerecord[4];
			tem_des = pexplain.c_str();
			des = (char*)tem_des;
			strCanTypeModify =des;
		}



		//wstring conp = onerecord[4];
		//tem_des = conp.c_str();
		//des = (char*)tem_des;
		//std::string con_pointname__(des);

		std::string em("");

		sqlstream << "insert into list_paramterConfig(vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values('" << vname__<< "','" << pointname__ << "','"<< ptype__<< "','" << pexplain__ << "','" << dllname__<< "'," << nIN<< ",'" << em<<"','" << strCanTypeModify<<"','" << em<<"','" << em<<"','" << em<<"');";
		sql_in = sqlstream.str();

		int re = access.SqlExe(sql_in.c_str());

		if (re != SQLITE_OK)
		{
			Sleep(2000);
			re = access.SqlExe(sql_in.c_str());
			if (re != SQLITE_OK)
			{
				Sleep(2000);
				re = access.SqlExe(sql_in.c_str());

				if (re != SQLITE_OK)
				{
					CWarningDlg WarnDlg;

					CString strTemp;
					strTemp.Format(_T("%s 策略保存输入参数时存储参数失败"),strDllName);

					WarnDlg.InitWarn(_T("策略风险警告"), strTemp, FALSE);
					WarnDlg.DoModal();

					OutputSaveErr(L"SaveInputParametertoDB",access.GetSqliteErr());
				}
			}

			
		}
	}

	if (access.CommitTransaction() != SQLITE_OK)
	{
		CWarningDlg WarnDlg;

		CString strTemp;
		strTemp.Format(_T("%s 策略保存输入参数时存储参数失败"),strDllName);


		WarnDlg.InitWarn(_T("策略风险警告"), strTemp, FALSE);
		WarnDlg.DoModal();

		OutputSaveErr(L"SaveInputParametertoDB",access.GetSqliteErr());
	}


	return true;
}


bool CBEOPLogicManager::OutputSaveErr( const wstring strOperation, const char* err )
{

	wstring strFilePath;
	Project::Tools::GetSysPath(strFilePath);
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString strLogPath;
	strLogPath.Format(_T("%s\\SqliteErr.log"),strFilePath.c_str());
	CStdioFile	ff;
	if(ff.Open(strLogPath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
	{
		wstring strTime,strEi;
		Project::Tools::SysTime_To_String(stNow,strTime);
		strEi = Project::Tools::AnsiToWideChar(err);
		CString strLog;
		strLog.Format(_T("%s::%s(%s)...\r\n"),strTime.c_str(),strOperation.c_str(),strEi.c_str());
		ff.Seek(0,CFile::end);
		ff.WriteString(strLog);
		ff.Close();
		return true;
	}
	
	return false;
}

bool CBEOPLogicManager::SaveOutputParametertoDB(CString strDllName,  std::vector<vec_str> &strLongPara )
{
	std::vector<vec_str> outputPara = strLongPara;

	//string strUtf8;
	//Project::Tools::WideCharToUtf8(m_strDBFileName, strUtf8);

	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("策略风险警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}

	//dll name
	wstring dllname = strDllName.GetString();
	_bstr_t tem_des = dllname.c_str();
	char* des = (char*)tem_des;
	std::string dllname__(des);

	string sql_del;
	ostringstream sqlstream_del;
	sqlstream_del << "delete from list_paramterConfig where INorOut = 1 and DllName = '"<< dllname__ <<"';";
	sql_del = sqlstream_del.str();
	if (SQLITE_OK != access.SqlExe(sql_del.c_str()))
	{
		;
	}
	int nIN = 1;
	access.BeginTransaction();
	std::vector<CString> onerecord;
	for (int k = 0;k < outputPara.size();++k)
	{
		string sql_in;
		ostringstream sqlstream;
		onerecord = outputPara[k];

		//变量名
		wstring vname_ = onerecord[0];
		_bstr_t tem_des = vname_.c_str();
		char* des = (char*)tem_des;
		std::string vname__(des);

		//点名
		wstring pointname_ = onerecord[1];
		tem_des = pointname_.c_str();
		des = (char*)tem_des;
		std::string pointname__(des);

		//类型
		wstring ptype = onerecord[2];
		tem_des = ptype.c_str();
		des = (char*)tem_des;
		std::string ptype__(des);

		//说明
		wstring pexplain = onerecord[3];
		tem_des = pexplain.c_str();
		des = (char*)tem_des;
		std::string pexplain__(des);

		wstring conp = onerecord[4];
		tem_des = conp.c_str();
		des = (char*)tem_des;
		std::string con_pointname__(des);

		//能否修改参数类型
		std::string strCanTypeModify("");
		if(onerecord.size()>=6)
		{
			wstring pexplain = onerecord[5];
			tem_des = pexplain.c_str();
			des = (char*)tem_des;
			strCanTypeModify =des;
		}

		std::string em("");
		sqlstream << "insert into list_paramterConfig(vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values('" << vname__<< "','" << pointname__ << "','"<< ptype__<< "','" << pexplain__ << "','" << dllname__<< "'," << nIN<< ",'" << con_pointname__<<"','" << strCanTypeModify<<"','" << em<<"','" << em<<"','" << em<<"');";
		sql_in = sqlstream.str();
		int re = access.SqlExe(sql_in.c_str());
		if (re != SQLITE_OK)
		{
			Sleep(2000);
			re = access.SqlExe(sql_in.c_str());
			if (re != SQLITE_OK)
			{
				Sleep(2000);
				re = access.SqlExe(sql_in.c_str());

				if (re != SQLITE_OK)
				{
					CWarningDlg WarnDlg;
					CString strTemp;
					strTemp.Format(_T("%s 策略保存输出参数时存储参数失败"),strDllName);

					WarnDlg.InitWarn(_T("策略风险警告"), strTemp, FALSE);
					WarnDlg.DoModal();

					OutputSaveErr(L"SaveOutputParametertoDB",access.GetSqliteErr());
				}
			}


			
		}
	}

	//access.CommitTransaction();
	if (access.CommitTransaction() != SQLITE_OK)
	{
		CWarningDlg WarnDlg;


		CString strTemp;
		strTemp.Format(_T("%s 策略保存输出参数时存储参数失败"),strDllName);

		WarnDlg.InitWarn(_T("策略风险警告"), strTemp, FALSE);
		WarnDlg.DoModal();

		OutputSaveErr(L"SaveOutputParametertoDB",access.GetSqliteErr());
	}

	return true;

}

bool CBEOPLogicManager::DeleteAllDllParametersOfThread(CLogicDllThread *pDllThread)
{
	int ncount = pDllThread->GetDllCount();
	
	if (ncount<=0)
	{
		return true;
	}

	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("策略风险警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();

		return false;
	}
    //删除dll所有参数

	string sql_del;
	ostringstream sqlstream_del;
	wstring wsDllName = pDllThread->GetDllObject(0)->GetDllName().GetString();
	_bstr_t tem_des = wsDllName.c_str();
	char* pdes = (char*)tem_des;
	std::string sDllName(pdes);

	sqlstream_del << "delete from list_paramterConfig where DllName = '"<< sDllName <<"' ";
	for (int i =1; i<ncount;++i)
	{
		wstring wsDllName = pDllThread->GetDllObject(i)->GetDllName().GetString();
		_bstr_t tem_des = wsDllName.c_str();
		char* pdes = (char*)tem_des;
		std::string sDllName(pdes);

		sqlstream_del << " or DllName =  '" <<sDllName<<"' ";
	}

	sqlstream_del<<";";

	sql_del = sqlstream_del.str();

	if (SQLITE_OK != access.SqlExe(sql_del.c_str()))
	{
		return false;
	}

	return true;
}


bool CBEOPLogicManager::SaveOneParameter(wstring strVName, wstring strPName, wstring strPType, wstring strDllName, wstring strInOut)
{
	if (strVName == L"" || strDllName == L"")
	{
		return false;
	}
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("策略风险警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}

	//dll name
	_bstr_t tem_des = strDllName.c_str();
	char* pDes = (char*)tem_des;
	std::string sDllName(pDes);

	_bstr_t tem_v = strVName.c_str();
	char* pDev = (char*)tem_v;
	std::string sVname(pDev);

	string sql_del;
	ostringstream sqlstream_del;
	sqlstream_del << "delete from list_paramterConfig where DllName = '"<< sDllName <<"' and vname =  '"<< sVname <<"';";
	sql_del = sqlstream_del.str();
	if (SQLITE_OK != access.SqlExe(sql_del.c_str()))
	{
		;
	}


	int nIN = 0;
	if (strInOut == L"input")
	{
		nIN = 0;
	}else
	{
		nIN = 1;
	}
	//变量名
	//vname_

	//点名
	tem_des = strPName.c_str();
	pDes = (char*)tem_des;
	std::string sPointName(pDes);

	//类型
	tem_des = strPType.c_str();
	pDes = (char*)tem_des;
	std::string sPtype(pDes);

	//说明
	std::string sPexplain("");
	
	//是否可输出
	std::string sCon_PointName("1");

	string sql_in;
	ostringstream sqlstream;
	std::string em("");
	sqlstream << "insert into list_paramterConfig(vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values('" << sVname<< "','" << sPointName << "','"<< sPtype<< "','" << sPexplain << "','" << sDllName<< "'," << nIN<< ",'" << sCon_PointName<<"','" << em<<"','" << em<<"','" << em<<"','" << em<<"');";
	sql_in = sqlstream.str();
	int re = access.SqlExe(sql_in.c_str());
	
	if (re != SQLITE_OK)
	{
		Sleep(2000);
		re = access.SqlExe(sql_in.c_str());
		if (re != SQLITE_OK)
		{
			Sleep(2000);
			re = access.SqlExe(sql_in.c_str());

			if (re != SQLITE_OK)
			{
				CWarningDlg WarnDlg;

				CString strTemp;
				strTemp.Format(_T("%s 策略修改参数时存储参数失败"),strDllName.c_str() );
				WarnDlg.InitWarn(_T("策略风险警告"), strTemp, FALSE);
				WarnDlg.DoModal();

				OutputSaveErr(L"SaveOneParameter",access.GetSqliteErr());

				return false;
			}
		}

	}

	return true;
}


//更新一个策略的参数
bool CBEOPLogicManager::UpdateOneParameter(wstring strVName, wstring strPName, wstring strPType, wstring strDllName, wstring strInOut)
{
	if (strVName == L"" || strDllName == L"")
	{
		return false;
	}
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("策略风险警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}

	//dll name
	_bstr_t tem_des = strDllName.c_str();
	char* pDes = (char*)tem_des;
	std::string sDllName(pDes);

	_bstr_t tem_v = strVName.c_str();
	char* pDev = (char*)tem_v;
	std::string sVname(pDev);

	string sql_del;
	ostringstream sqlstream_del;
	sqlstream_del << "select vname from list_paramterConfig where DllName ='"<< sDllName <<"' and vname ='"<< sVname <<"';";
	sql_del = sqlstream_del.str();
	bool sqlWhetherInsert = false;//
	if (SQLITE_OK != access.SqlQuery(sql_del.c_str()) )
	{
		;
	}else
	{
		//int sqlReturnCount = access.getColumn_Count();//读取返回值长度
		while( access.SqlNext()== SQLITE_ROW )
		{
			const char *name = access.getColumn_Text(0);
			if(strlen(name))
				sqlWhetherInsert = true;
		}
	}
	/*
		输入输出/是否可输出 
		使能位置在sqllite中位置并不同
	*/
	std::string nIN ;
	std::string sInCon_PointName;
	std::string sOutCon_PointName;
	if (strInOut == L"input")
	{
		nIN = "0";
		sInCon_PointName  = "1";
		sOutCon_PointName = " ";
	}else
	{
		nIN = "1";
		sInCon_PointName  = " ";
		sOutCon_PointName = "1";
	}

	//变量名
	//vname_

	//点名
	tem_des = strPName.c_str();
	pDes = (char*)tem_des;
	std::string sPointName(pDes);

	//类型
	tem_des = strPType.c_str();
	pDes = (char*)tem_des;
	std::string sPtype(pDes);

	string sql_in;
	ostringstream sqlstream;
	if ( sqlWhetherInsert == true )//更新数据
	{
		sqlstream << "update list_paramterConfig set "
			<<"pname='"<<sPointName<<"',"
			<<"ptype='"<<sPtype<<"',"
			<<"INorOut='"<<nIN<<"',"
			<<"unitproperty01='"<<sOutCon_PointName<<"',"
			<<"unitproperty02='"<<sInCon_PointName
			<<"' where vname ='"<< sVname<<"' and  "<<"DllName='"<<sDllName<<"';";
	}
	sql_in = sqlstream.str();
	int re = access.SqlExe(sql_in.c_str());

	if (re != SQLITE_OK)
	{
		Sleep(2000);
		re = access.SqlExe(sql_in.c_str());
		if (re != SQLITE_OK)
		{
			Sleep(2000);
			re = access.SqlExe(sql_in.c_str());

			if (re != SQLITE_OK)
			{
				CWarningDlg WarnDlg;

				CString strTemp;
				strTemp.Format(_T("%s 策略修改参数时存储参数失败"),strDllName.c_str() );
				WarnDlg.InitWarn(_T("策略风险警告"), strTemp, FALSE);
				WarnDlg.DoModal();

				OutputSaveErr(L"SaveOneParameter",access.GetSqliteErr());

				return false;
			}
		}

	}

	return true;
}

bool CBEOPLogicManager::UpdateThreadName( const CString strOldThreadName,const CString strNewThreadName )
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string strThreadNewName,strThreadOldName;
	Project::Tools::WideCharToUtf8(strOldThreadName.GetString(), strThreadOldName);
	Project::Tools::WideCharToUtf8(strNewThreadName.GetString(), strThreadNewName);

	ostringstream sqlstream;
	sqlstream << "update list_dllstore set unitproperty04 = '"<< strThreadNewName <<"' where unitproperty04 = '"<< strThreadOldName <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	if (re != SQLITE_OK)
	{
		return false;
	}

	//更新内存
	for(int i=0; i<vecImportDLLList.size(); ++i)
	{
		if(vecImportDLLList[i]->GetDllThreadName() == strOldThreadName)
		{
			vecImportDLLList[i]->SetDllThreadName(strNewThreadName);
		}
	}

	for(int j=0; j<m_vecDllThreadList.size(); ++j)
	{
		if(m_vecDllThreadList[j]->GetName().c_str() == strOldThreadName)
		{
			m_vecDllThreadList[j]->SetName(strNewThreadName.GetString());
		}
	}

	return true;
}

bool CBEOPLogicManager::UpdateThreadPeriodicity(const int nPeriodicity ,const CString strThreadName)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string strUtf8ThreadName;
	Project::Tools::WideCharToUtf8(strThreadName.GetString(), strUtf8ThreadName);

	ostringstream sqlstream;
	sqlstream << "update list_dllstore set periodicity = '"<< nPeriodicity <<"' where unitproperty04 = '"<< strUtf8ThreadName <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	if (SQLITE_OK != re)
	{
		return false;
	}

	//更新内存
	for (int i=0; i<vecImportDLLList.size(); ++i)
	{
		if (strThreadName == vecImportDLLList[i]->GetDllThreadName())
		{
			vecImportDLLList[i]->SetTimeSpan(nPeriodicity);
		}
	}

	return true;
}

bool CBEOPLogicManager::DeleteThreadName( const CString strThreadName )
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string strThreadOldName;
	Project::Tools::WideCharToUtf8(strThreadName.GetString(), strThreadOldName);

	ostringstream sqlstream;
	sqlstream << "delete from list_dllstore where unitproperty04 = '"<< strThreadOldName <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	if (re != SQLITE_OK)
	{
		return false;
	}

	//从内存中删除dll
	for (int j=vecImportDLLList.size()-1;j>=0;--j)
	{
		if (vecImportDLLList[j]->GetDllThreadName()== strThreadName)
		{
			vecImportDLLList[j]->Exit();
			delete(vecImportDLLList[j]);
			vecImportDLLList.erase(vecImportDLLList.begin()+j);
		}
	}

	for(int m=m_vecDllThreadList.size()-1; m>=0; --m)
	{
		if(m_vecDllThreadList[m]->GetName().c_str() == strThreadName)
		{
			delete(m_vecDllThreadList[m]);
			m_vecDllThreadList.erase(m_vecDllThreadList.begin()+m);
		}
	}

	//连接调试时候删除线程
	if(m_pDataAccess && m_pDataAccess->IsMySQLConnected())
	{
		if (!m_pDataAccess->UpdateLibInsertIntoUnit02Cal(L"6",strThreadName.GetString(),L"",L"",L"",L"",L"",L"",L"",L"",L"",L"", 
			m_pDataAccess->GetServerAddress()!=L"127.0.0.1")&& m_pDataAccess->GetServerAddress()!=L"localhost")
		{
			return false;
		}
	}

	return true;
}

bool CBEOPLogicManager::FindDllNameIsExist( const CString strDllName )
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string strDllName__;
	Project::Tools::WideCharToUtf8(strDllName.GetString(), strDllName__);

	ostringstream sqlstream;
	sqlstream << "select * from list_dllstore where DllName = '"<< strDllName__ <<".dll';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	bool bFind = false;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		bFind = true;
	}
	access.SqlFinalize();
	access.CloseDataBase();
	return bFind;
}
bool CBEOPLogicManager::CopyOldDllNameToSdbChangetoNewDllName(const CString strOldDllName,const CString strDllNewName)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string strDllName__;
	Project::Tools::WideCharToUtf8(strOldDllName.GetString(), strDllName__);

	ostringstream sqlstream;
	sqlstream << "insert * from list_dllstore where DllName = '"<< strDllName__ <<".dll';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	access.SqlFinalize();
	access.CloseDataBase();
	return true;
}
bool CBEOPLogicManager::DeleteDllParametersOfThreadByNames( vector<wstring> vecDllNames )
{
	if(vecDllNames.size()<=0)
		return true;

	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("策略风险警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();

		return false;
	}
	
	string sql_del;
	ostringstream sqlstream_del;
	wstring wsDllName = vecDllNames[0];
	_bstr_t tem_des = wsDllName.c_str();
	char* pdes = (char*)tem_des;
	std::string sDllName(pdes);

	sqlstream_del << "delete from list_paramterConfig where DllName = '"<< sDllName <<"' ";
	for (int i =1; i<vecDllNames.size();++i)
	{
		wstring wsDllName = vecDllNames[i];
		_bstr_t tem_des = wsDllName.c_str();
		char* pdes = (char*)tem_des;
		std::string sDllName(pdes);
		sqlstream_del << " or DllName =  '" <<sDllName<<"' ";
	}

	sqlstream_del<<";";

	sql_del = sqlstream_del.str();

	if (SQLITE_OK != access.SqlExe(sql_del.c_str()))
	{
		return false;
	}

	return true;
}

void CBEOPLogicManager::DeleteOldDllAndLoadDllFromNewS3DB( CString strNewDBFileName )
{
	//Read list_dllstore
	wstring strNewName = strNewDBFileName.GetString();

	string strUtf8;
	Project::Tools::WideCharToUtf8(strNewName, strUtf8);

	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	ReadDLLfromDB(strNewName,vecImportDLLList);

	//Read list_pointwatch
	ostringstream sqlstream;
	sqlstream.str("");
	sqlstream << "select * from list_pointwatch;";
	string sql_ = sqlstream.str();
	char * ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	vector<sPointWatch> vecPointWatch;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		sPointWatch sPoint;
		sPoint.nID = access.getColumn_Int(0);

		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   name_temp(const_cast<char*>(access.getColumn_Text(1)));
			sPoint.strName = name_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(2)) !=NULL)
		{
			string   t_temp(const_cast<char*>(access.getColumn_Text(2)));
			sPoint.strContent = t_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(3)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(3)));
			sPoint.unitproperty01 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(4)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(4)));
			sPoint.unitproperty02 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(5)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(5)));
			sPoint.unitproperty03 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(6)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(6)));
			sPoint.unitproperty04 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(7)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(7)));
			sPoint.unitproperty05 = d_temp;
		}

		vecPointWatch.push_back(sPoint);	
	}
	access.SqlFinalize();

	//Read list_paramterConfig
	sqlstream.str("");
	sqlstream << "select vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05 from list_paramterConfig;";
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	re = access.SqlQuery(ex_sql);
	vector<sParamConfig> vecParamConfig;
	while(true)
	{
		if(SQLITE_ROW != access.SqlNext())
		{
			break;
		}
		sParamConfig sParam;

		if (const_cast<char*>(access.getColumn_Text(0)) !=NULL)
		{
			string   vname_temp(const_cast<char*>(access.getColumn_Text(0)));
			sParam.strVName = vname_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(1)) !=NULL)
		{
			string   pname_temp(const_cast<char*>(access.getColumn_Text(1)));
			sParam.strPName = pname_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(2)) !=NULL)
		{
			string   pt_temp(const_cast<char*>(access.getColumn_Text(2)));
			sParam.strPType = pt_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(3)) !=NULL)
		{
			string   v_temp(const_cast<char*>(access.getColumn_Text(3)));
			sParam.strVexplain = v_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(4)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(4)));
			sParam.strDllName = d_temp;
		}

		sParam.nINorOut = access.getColumn_Text(5);

		if (const_cast<char*>(access.getColumn_Text(6)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(6)));
			sParam.unitproperty01 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(7)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(7)));
			sParam.unitproperty02 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(8)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(8)));
			sParam.unitproperty03 = d_temp;
		}
		if (const_cast<char*>(access.getColumn_Text(9)) !=NULL)
		{
			string   r_temp(const_cast<char*>(access.getColumn_Text(9)));
			sParam.unitproperty04 = r_temp;
		}

		if (const_cast<char*>(access.getColumn_Text(10)) !=NULL)
		{
			string   d_temp(const_cast<char*>(access.getColumn_Text(10)));
			sParam.unitproperty05 = d_temp;
		}

		vecParamConfig.push_back(sParam);	
	}
	access.SqlFinalize();
	access.CloseDataBase();

	CSqliteAcess accessOld(m_strDBFileName.c_str(), m_bEncry);


	//删除list_pointwatch
	sqlstream.str("");
	sqlstream << "delete from list_pointwatch;"; 
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	if (SQLITE_OK == accessOld.SqlExe(ex_sql))
	{
		;
	}

	//插入list_pointwatch
	accessOld.BeginTransaction();
	for(int i=0; i<vecPointWatch.size(); i++)
	{
		sqlstream.str("");
		sPointWatch sPoint = vecPointWatch[i];
		sqlstream << "insert into list_pointwatch(id,name,content,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values(" << sPoint.nID<< ",'" << sPoint.strName << "','"<< sPoint.strContent <<"','"<<sPoint.unitproperty01 <<"','"<<sPoint.unitproperty02<<"','"<<sPoint.unitproperty03<<"','"<<sPoint.unitproperty04<<"','"<<sPoint.unitproperty05<<"')";
		string sql_in = sqlstream.str();
		accessOld.SqlExe(sql_in.c_str());
	}
	accessOld.CommitTransaction();

	//删除list_paramterConfig
	sqlstream.str("");
	sqlstream << "delete from list_paramterConfig;"; 
	sql_ = sqlstream.str();
	ex_sql = const_cast<char*>(sql_.c_str());
	if (SQLITE_OK == accessOld.SqlExe(ex_sql))
	{
		;
	}
	//插入list_paramterConfig

	accessOld.BeginTransaction();
	for(int i=0; i<vecParamConfig.size(); i++)
	{
		sqlstream.str("");
		sParamConfig sParam = vecParamConfig[i];
		sqlstream << "insert into list_paramterConfig(vname,pname,ptype,vexplain,DllName,INorOut,unitproperty01,unitproperty02,unitproperty03,unitproperty04,unitproperty05)  values('" << sParam.strVName<< "','" << sParam.strPName << "','"<< sParam.strPType<< "','" << sParam.strVexplain << "','" << sParam.strDllName<< "','" << sParam.nINorOut \
			<<"','"<<sParam.unitproperty01 <<"','"<<sParam.unitproperty02<<"','"<<sParam.unitproperty03<<"','"<<sParam.unitproperty04<<"','"<<sParam.unitproperty05<<"')";

		string sql_in = sqlstream.str();
		accessOld.SqlExe(sql_in.c_str());
	}
	accessOld.CommitTransaction();
	accessOld.CloseDataBase();
}

bool CBEOPLogicManager::FindCharInString( const wstring strChar,const wstring strValue )
{
	if(strChar.length()<=0)
		return false;

	if(strValue.length()<=0)
		return false;

	CString str = strValue.c_str();
	int nPos = -1;
	nPos = str.Find(strChar.c_str());
	if(nPos >= 0)
		return true;
	return false;
}

bool CBEOPLogicManager::UpdateTacticName(const CString strTacticNameOld, const CString strTacticNameNew)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	CString strTacticNameOldEx(strTacticNameOld + _T(".dll"));
	CString strTacticNameNewEx(strTacticNameNew + _T(".dll"));
	string strUtf8TacticNameOld;
	string strUtf8TacticNameNew;
	Project::Tools::WideCharToUtf8(strTacticNameOldEx.GetString(), strUtf8TacticNameOld);
	Project::Tools::WideCharToUtf8(strTacticNameNewEx.GetString(), strUtf8TacticNameNew);

	ostringstream sqlstream;
	sqlstream << "update list_dllstore set DllName = '"<< strUtf8TacticNameNew <<"' where DllName = '"<< strUtf8TacticNameOld <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	if (re != SQLITE_OK)
	{
		return false;
	}


	// 内存修改
	int nSize = vecImportDLLList.size();
	for (int i=0; i<nSize; ++i)
	{
		if (vecImportDLLList[i]->GetDllName() == strTacticNameOldEx)
		{
			vecImportDLLList[i]->SetDllName(strTacticNameNewEx);
			break;
		}
	}


	return true;
}

bool CBEOPLogicManager::UpdateTacticParamConfig(const CString strDllNameOld, const CString strDllNameNew)
{
	string strUtf8;
	Project::Tools::WideCharToUtf8(m_strDBFileName, strUtf8);
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);
	if (!access.GetOpenState())
	{
		CWarningDlg WarnDlg;
		WarnDlg.InitWarn(_T("警告"), _T("打开S3DB数据库失败！"), FALSE);
		WarnDlg.DoModal();
	}


	CString strDllNameOldEx(strDllNameOld + _T(".dll"));
	CString strDllNameNewEx(strDllNameNew + _T(".dll"));

	// dll old name
	wstring dllNameOld = strDllNameOldEx.GetString();
	_bstr_t tem_des = dllNameOld.c_str();
	char* des = (char*)tem_des;
	string dllNameOld__(des);

	// dll new name
	wstring dllNameNew = strDllNameNewEx.GetString();
	tem_des = dllNameNew.c_str();
	des = (char*)tem_des;
	string dllNameNew__(des);


	//检测该参数是否已经存在
	bool bRet = true;
	access.BeginTransaction();

	ostringstream sqlstream;
	sqlstream << "update list_paramterConfig set DllName = '"<<dllNameNew__<<"' where DllName = '"<<dllNameOld__ <<"';";
	string sql_in(sqlstream.str());
	int re = access.SqlExe(sql_in.c_str());
	if (re != SQLITE_OK)
	{
		bRet = false;
	}


	if (access.CommitTransaction() != SQLITE_OK || !bRet)
	{
		CWarningDlg WarnDlg;

		CString strTemp;
		strTemp.Format(_T("%s 策略修改名称时存储参数失败"),strDllNameOld );
		WarnDlg.InitWarn(_T("警告"), strTemp, FALSE);
		WarnDlg.DoModal();
	}

	access.CloseDataBase();
	return bRet;
}

bool CBEOPLogicManager::UpdateTacticNameOriginal(const CString strNameOld, const CString strNameNew)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	CString strNameOldEx(strNameOld);
	CString strNameNewEx(strNameNew);

	USES_CONVERSION;
	string strAnsiNameOld(T2A(strNameOldEx.GetString()));
	string strAnsiNameNew(T2A(strNameNewEx.GetString()));

	ostringstream sqlstream;
	sqlstream << "update list_dllstore set unitproperty03 = '"<< strAnsiNameNew <<"' where unitproperty03 = '"<< strAnsiNameOld <<"';";
	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	if (re != SQLITE_OK)
	{
		return false;
	}

	// 内存修改
	int nSize = vecImportDLLList.size();
	for (int i=0; i<nSize; ++i)
	{
		if (vecImportDLLList[i]->GetDllOriginName() == strNameOldEx)
		{
			vecImportDLLList[i]->SetDllOriginName(strNameNewEx);
			break;
		}
	}

	return true;
}

bool CBEOPLogicManager::UpdateTacticInfo(const CString strName, const CString strNameOrg, const CString strTime, const CString strAuthor, const CString strVer, const CString strDesc)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	CString strNameEx(strName);
	CString strNameOrgEx(strNameOrg);
	CString strTimeEx(strTime);
	CString strAuthorEx(strAuthor);
	CString strVerEx(strVer);
	CString strDescEx(strDesc);

	USES_CONVERSION;
	string strAnsiName(T2A(strNameEx.GetString()));
	string strAnsiNameOrg(T2A(strNameOrgEx.GetString()));
	string strAnsiTime(T2A(strTimeEx.GetString()));
	string strAnsiAuthor(T2A(strAuthorEx.GetString()));
	string strAnsiVer(T2A(strVerEx.GetString()));
	string strAnsiDesc(T2A(strDescEx.GetString()));

	ostringstream sqlstream;
	sqlstream<<"update list_dllstore set unitproperty03='"
		<< strAnsiNameOrg <<"', importtime='"
		<< strAnsiTime <<"', author='"
		<< strAnsiAuthor <<"', unitproperty01='"
		<< strAnsiVer <<"', unitproperty02='"
		<< strAnsiDesc <<"' where DllName='"
		<< strAnsiName <<"';";

	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlExe(ex_sql);
	if (re != SQLITE_OK)
	{
		return false;
	}

	size_t nSize = vecImportDLLList.size();
	for (int i=0; i<nSize; i++)
	{
		CDLLObject* pDllObj = vecImportDLLList[i];
		if (NULL == pDllObj)
		{
			continue;
		}
		if (pDllObj->GetDllName() == strNameEx)
		{
			pDllObj->SetDllOriginName(strNameOrgEx);
			pDllObj->SetDllImportTime(strTimeEx);
			pDllObj->SetDllAuthor(strAuthorEx);
			pDllObj->SetDllVersion(strVerEx);
			pDllObj->SetDllDescription(strDescEx);
			break;
		}
	}

	return true;
}
/*
	从 list_dllstore 表中的 unitproperty05 读出策略说明
*/
bool CBEOPLogicManager::GetDeclareParameterfromDB(const CString strDllName, const CString strDllOriginName, const CString strThreadName, CString &vecMark )
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	string strUtf8DllName;
	Project::Tools::WideCharToUtf8(strDllName.GetString(), strUtf8DllName);
	string strUtf8DllOriginName;
	Project::Tools::WideCharToUtf8(strDllOriginName.GetString(), strUtf8DllOriginName);
	string strUtf8ThreadName;
	Project::Tools::WideCharToUtf8(strThreadName.GetString(), strUtf8ThreadName);

	ostringstream sqlstream;
	sqlstream << "SELECT unitproperty05 FROM list_dllstore WHERE dllName='"<<strUtf8DllName<<"'"
		<<" and "
		<<"unitproperty03= "<<"'"<<strUtf8DllOriginName<<"'"	//策略名
		<<" and "
		<<"unitproperty04= "<<"'"<<strUtf8ThreadName<<"'"	//线程名
		<<";"; 

	string sql_ = sqlstream.str();
	char *ex_sql = const_cast<char*>(sql_.c_str());
	int re = access.SqlQuery(ex_sql);
	if(re == SQLITE_OK)
	{
		CString strDllOriginName;
		if (SQLITE_ROW == access.SqlNext())
		{
			const char *name = access.getColumn_Text(0);
			strDllOriginName = name;
			vecMark =strDllOriginName;
		}

	}else 
	{
		AfxMessageBox(_T("策略说明读取失败"));
	}
	access.SqlFinalize();
	access.CloseDataBase();
	return true;
}
/*
	保存策略说明至 list_dllstore 表中的 unitproperty05 
*/
bool CBEOPLogicManager::SaveDeclareParameterToDB(CString DllName,CString strDeclare, CString strDllOriginName, CString strThreadName)
{
	CSqliteAcess access(m_strDBFileName.c_str(), m_bEncry);

	wstring dllname_ = DllName.GetString();
	_bstr_t tem_des = dllname_.c_str();
	char* des = (char*)tem_des;
	string sdllname(des);

	USES_CONVERSION;
	string strAnsiDeclare(T2A(strDeclare.GetString()));

	wstring temp_ON = strDllOriginName.GetString();
	_bstr_t tem_ON = temp_ON.c_str();
	char* des_ON = (char*)tem_ON;
	string sdllOriginName(des_ON);

	wstring temp_TN = strThreadName.GetString();
	_bstr_t tem_TN = temp_TN.c_str();
	char* des_TN = (char*)tem_TN;
	string sThreadName(des_TN);

	string sql_del;
	ostringstream sqlstream_del;
	sqlstream_del << "update list_dllstore set unitproperty05 ='"<< strAnsiDeclare <<"'"
		<<" where "
		<<"DllName= "<< "'" << sdllname << "'"		//策略别名
		<<" and "
		<<"unitproperty03= "<<"'"<<sdllOriginName<<"'"	//策略名
		<<" and "
		<<"unitproperty04= "<<"'"<<sThreadName<<"'"	//线程名
		<<";"; 
	sql_del = sqlstream_del.str();
	int re = access.SqlExe(sql_del.c_str());
	if(re != SQLITE_OK)
	{
		AfxMessageBox(_T("策略说明写入失败"));
	}
	access.CloseDataBase();
	return true;
}