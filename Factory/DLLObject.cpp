#include "StdAfx.h"
#include "DLLObject.h"


#include "../Tools/warning/MsgDefine.h"
#include "LogicParameter.h"


const int LogStoreSize = 500;

CDLLObject::CDLLObject(CUPPCServerDataAccess *pDataAccess):m_pDataAccess(pDataAccess)
{

	m_pLB			= NULL;
	m_pHinstance	= NULL;
	m_sDllName		= _T("");
	m_bDllRun       = true;
	m_pFun			= NULL;
	m_iTimeSpan     = 0;
	m_thisOver		= false;
	m_sImportTime	= _T("");
	m_sDllAuthor	= _T("");
	m_sDllPathFile  = _T("");
	m_sDllVersion	= _T("");
	m_sDllDescription	= _T("");
	//初始处于非激发状态，在变成激发状态后，不重置为非激发状态
	m_hEventClose   = CreateEvent(NULL,TRUE,FALSE,NULL);
	//初始处于非激发状态，在变成激发状态后，不重置为非激发状态
	m_hEventDelete  = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_bAvailable    = true;

	m_vecCurDllInputParameterList.clear();
	m_vecCurDllOutputParameterList.clear();

	m_sS3DBPath		= _T("");

	m_strLogList.clear();
}

CDLLObject::~CDLLObject(void)
{
	if (m_hEventClose != NULL)
	{
		CloseHandle(m_hEventClose);
		m_hEventClose =NULL;
	}
	if (m_hEventDelete != NULL)
	{
		CloseHandle(m_hEventDelete);
		m_hEventDelete =NULL;
	}


}
void CDLLObject::SetMsgWnd(CWnd * MsgWnd)
{
	m_MsgWnd = MsgWnd;
}

void CDLLObject::SentMsgInfo(LPCTSTR loginfo)
{
	if (m_MsgWnd)
	{
		if(m_MsgWnd->GetSafeHwnd())
			::SendMessage(m_MsgWnd->GetSafeHwnd(), WM_DATAENGINELOG, (WPARAM)(L"Logic"), (LPARAM)loginfo);
	}
}

CLogicBase*	CDLLObject::GetLB()
{
	return m_pLB;
}
bool CDLLObject::SetLB(CLogicBase* pLB)
{
	m_pLB = pLB;
	return true;
}
HINSTANCE CDLLObject::GetHinstance()
{
	return m_pHinstance;
}
bool CDLLObject::SetHinstance(HINSTANCE pHinstance)
{
	m_pHinstance = pHinstance;
	return true;
}
CString	CDLLObject::GetDllImportTime()
{
	return m_sImportTime;
}

CString	CDLLObject::GetDllAuthor()
{
	return m_sDllAuthor;
}
bool CDLLObject::SetDllAuthor(CString author)
{
	m_sDllAuthor = author;

	return true;
}
CString	CDLLObject::GetDllPath()
{
	return m_sDllPathFile;
}
bool CDLLObject::SetDllPath(CString path)
{
	m_sDllPathFile = path;

	return true;
}

bool CDLLObject::SetDllImportTime(CString dllImporttime)
{
	m_sImportTime = dllImporttime;

	return true;
}

void CDLLObject::ClearParameters()
{
	int i = 0;
	for(i=0;i<m_vecCurDllInputParameterList.size();i++)
	{
		SAFE_DELETE(m_vecCurDllInputParameterList[i]);
	}

	for(i=0;i<m_vecCurDllOutputParameterList.size();i++)
	{
		SAFE_DELETE(m_vecCurDllOutputParameterList[i]);
	}

	m_vecCurDllInputParameterList.clear();
	m_vecCurDllOutputParameterList.clear();
}

CString CDLLObject::GetDllOriginName()
{
	return m_sDllOriginName;
}


bool CDLLObject::SetDllOriginName(CString strDllOriginName)
{
	m_sDllOriginName = strDllOriginName;
	return true;
}

bool CDLLObject::SetDllThreadName(CString strDllThreadName)
{
	m_sDllThreadName = strDllThreadName;

	return true;
}

CString  CDLLObject::GetDllThreadName()
{
	return m_sDllThreadName;
}

CString CDLLObject::GetDllName()
{
	return m_sDllName;
}


bool CDLLObject::SetDllName(CString dllname)
{
	m_sDllName = dllname;
	return true;
}
bool CDLLObject::GetDllRunStatus()
{
	return m_bDllRun;
}
bool CDLLObject::SetDllRunStatus(bool runstatus)
{
	m_bDllRun = runstatus ;
	return true;
}
pfnLogicBase CDLLObject::GetFun()
{
	return m_pFun;
}
bool CDLLObject::SetFun(pfnLogicBase fun)
{
	m_pFun = fun;
	return true;
}
double	CDLLObject::GetTimeSpan()
{
	return m_iTimeSpan;
}
bool CDLLObject::SetTimeSpan(double Span)
{
	m_iTimeSpan = Span;
	return true;
}

bool CDLLObject::StrIsNum(CString Str )
{
	if (Str == _T(""))
	{
		return false;
	}
	if (Str[0] == '-')
	{
		Str = Str.Mid(1,Str.GetLength());
	}

	CString StrNum = _T(".0123456789");
	int iPointCount = 0;
	int iStrLen = Str.GetLength();
	for (int i=0;i<iStrLen;++i)
	{
		CString wch(Str[i]);
		if (StrNum.FindOneOf(wch)<0)
		{
			return false;
		}
		if (wch == _T("."))
		{
			iPointCount++;
		}
	}
	if (Str[0] == '.'||Str[Str.GetLength()-1] == '.')
	{
		return false;
	}

	if (iPointCount >1)
	{
		return false;
	}
	return true;
}

bool CDLLObject::SetDllVersion( const CString &ver )
{
	m_sDllVersion = ver;
	return true;
}

CString CDLLObject::GetDllVersion()
{
	return m_sDllVersion;
}

bool CDLLObject::SetDllDescription( const CString &des )
{
	m_sDllDescription = des;

	return true;
}

CString CDLLObject::GetDllDescription()
{
	return m_sDllDescription;
}

CString CDLLObject::GetS3DBPath()
{
	return m_sS3DBPath;
}

bool CDLLObject::SetS3DBPath(const CString &s3dbpath)
{
	m_sS3DBPath = s3dbpath;

	return true;
}


bool CDLLObject::Exit()
{
	//告诉线程不在进行下一运行
	SetEvent(m_hEventClose);

	ClearParameters();

	if (m_pLB != NULL)
	{
		m_pLB->Exit();
		delete m_pLB;
		m_pLB = NULL;
	}
	if (m_pHinstance != NULL)
	{
		BOOL bFreeSuccess = FreeLibrary(m_pHinstance);
		if(bFreeSuccess==false)
		{
			//AfxMessageBox(L"FreeLiabrary Failed, Please restart Factory!");
		}
	}

	return true;
}


void CDLLObject::AddLogInfo(const CString &strLog)
{
	
		if (m_strLogList.size()<LogStoreSize)
		{
			m_strLogList.push_back(strLog);
		}
		else
		{
			// 超过一定条数量，将记录为一个文件，清空
			CString strFilePath;
			Project::Tools::GetSysPath(strFilePath);

			COleDateTime tnow = COleDateTime::GetCurrentTime();
			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
			setlocale( LC_CTYPE, ("chs"));

			CString strLogPath;
			strLogPath.Format(L"%s\\log\\%s-%s.txt",strFilePath, GetDllName(), tnow.Format(L"%Y%m%d%H%M%S"));
			CStdioFile ff(strLogPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			for(int iLine=0;iLine<m_strLogList.size();iLine++)
			{
				CString strInfo = m_strLogList[iLine];
				strInfo+=L"\r\n";
				ff.WriteString(strInfo);
			}
			ff.Close();
			setlocale( LC_CTYPE, old_locale ); 
			free( old_locale ); 


			Project::Tools::Scoped_Lock<Mutex>	scopelock(m_lock_strLog);
			m_strLogList.clear();
		}
	
}


bool CDLLObject::GetLogAll(CString &strAllLog)
{
	Project::Tools::Scoped_Lock<Mutex>	scopelock(m_lock_strLog);
	for (int i =0;i<m_strLogList.size();++i)
	{
		strAllLog += m_strLogList[i];
		strAllLog +="\r\n";
	}

	return true;
}


bool CDLLObject::GetDllInputParamterValue( CLogicBase* LB )
{
	if (LB == NULL)
	{
		return false;
	}

	for (int i=0;i<m_vecCurDllInputParameterList.size();++i)
	{
		wstring strCurValue;
		COleDateTime oleTime;
		Project::Tools::String_To_OleTime( LB->GetActTime().GetString(), oleTime);
		if(m_vecCurDllInputParameterList[i]->UpdateValue(strCurValue, LB->GetActMode(), oleTime))
			m_pLB->SetInputValue(i, strCurValue.c_str());
		
		
	}

	return true;
}

bool CDLLObject::SetDllOutputParamterValue( CLogicBase* LB )
{
	if (LB == NULL)
	{
		return false;
	}

	for (int i=0;i<m_vecCurDllOutputParameterList.size();++i)
	{
		CLogicParameter *pPara = m_vecCurDllOutputParameterList[i];
		if (pPara->Enabled())
		{
			if (LB->GetOutputChangeFlag(i))//如果有值更新
			{
				CString strOutputValue;
				LB->GetOutputValue(i,strOutputValue);
				wstring wstr = strOutputValue.GetString();

				COleDateTime oleTime;
				Project::Tools::String_To_OleTime( LB->GetActTime().GetString(), oleTime);

				bool bSetSuccess = pPara->UpdateValue(wstr, LB->GetActMode(), oleTime); 


				//变量设为已写
				LB->SetOutputChangeFlag(i,FALSE);

				if(bSetSuccess==false)
					return false;

			}
		}
	}

	return true;
}
