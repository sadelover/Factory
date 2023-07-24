#pragma once

#include "../ComponentDraw/EqmDrawDefines.h"
#include "LogicBase.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"

class CUPPCServerDataAccess;
class CLogicBase;
class CLogicParameter;

typedef CLogicBase* (*pfnLogicBase)();

class CDLLObject
{
public:
	CDLLObject(CUPPCServerDataAccess *pDataAccess);
	virtual ~CDLLObject(void);
public:
	//创建线程

	CLogicBase*	GetLB();
	bool		SetLB(CLogicBase* pLB);
	HINSTANCE	GetHinstance();
	bool		SetHinstance(HINSTANCE pHinstance);

	CString		GetDllName();
	bool		SetDllName(CString dllname);
	CString     GetDllOriginName();
	bool        SetDllOriginName(CString strDllOriginName);
	bool        SetDllThreadName(CString strDllThreadName);
	CString     GetDllThreadName();
	CString		GetDllAuthor();
	bool		SetDllAuthor(CString author);

	CString		GetDllPath();
	bool		SetDllPath(CString path);

	void        ClearParameters();
	bool		Exit();

	CString		GetDllImportTime();
	bool		SetDllImportTime(CString dllImporttime);

	bool		GetDllRunStatus();
	bool		SetDllRunStatus(bool runstatus);
	pfnLogicBase	GetFun();
	bool		SetFun(pfnLogicBase fun);

	double		GetTimeSpan();
	bool		SetTimeSpan(double Span);

	void		SetMsgWnd(CWnd * MsgWnd);
	void		SentMsgInfo(LPCTSTR loginfo);

	bool		StrIsNum(CString Str);

	bool		SetDllVersion(const CString &ver);
	CString		GetDllVersion();
	bool		SetDllDescription(const CString &des);
	CString		GetDllDescription();
	CString     GetS3DBPath();
	bool		SetS3DBPath(const CString &s3dbpath);
	void		AddLogInfo(const CString &strLog);
	bool        GetLogAll(CString &strAllLog);

	bool		GetDllInputParamterValue( CLogicBase* LB );
	bool		SetDllOutputParamterValue( CLogicBase* LB );
protected:

	CLogicBase*					m_pLB;
	HINSTANCE					m_pHinstance;
	CString						m_sDllOriginName;
	CString						m_sDllName;
	CString						m_sDllVersion;
	CString						m_sDllDescription;
	bool						m_bDllRun;
	CString                     m_sImportTime;
	CString						m_sDllAuthor;
	CString						m_sDllPathFile;
	CString                     m_sDllThreadName;
	pfnLogicBase	m_pFun;
	double			m_iTimeSpan;
	HANDLE			m_hEventClose;
	bool			m_bAvailable;
	HANDLE          m_hEventDelete;

public:
	CUPPCServerDataAccess *m_pDataAccess;
	bool			m_thisOver;
	std::vector<CLogicParameter *> m_vecCurDllInputParameterList;
	std::vector<CLogicParameter *> m_vecCurDllOutputParameterList;

	Project::Tools::Mutex	m_lock_strLog;
	std::vector<CString> m_strLogList; 
private:
	CWnd*			m_MsgWnd;
	CString			m_sS3DBPath;
};

