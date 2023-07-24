#pragma once
#include "../ComponentDraw/EqmDrawDefines.h"
#include "DLLObject.h"


class CLogicBase;
class CCallOptimizeLogicBEOP;
class CLogicDllThread;
typedef std::vector<wstring>  vec_wstr;
typedef CLogicBase* (*pfnLogicBase)();



struct sDllStore
{
	string	nID;
	string	strDllName;
	string	strImportTime;
	string	strAuthor;
	string	strPeriodicity;
	string	strDllContent;
	string	strRunStatus;
	string unitproperty01;
	string	unitproperty02;
	string unitproperty03;
	string	unitproperty04;
	string unitproperty05;

};

struct sPointWatch
{
	int		nID;
	string	strName;
	string	strContent;

	string unitproperty01;
	string	unitproperty02;
	string unitproperty03;
	string	unitproperty04;
	string unitproperty05;
};

struct sParamConfig
{

	string	strVName;
	string	strPName;
	string	strPType;
	string	strVexplain;
	string	strDllName;
	string	nINorOut;

	string unitproperty01;
	string	unitproperty02;
	string unitproperty03;
	string	unitproperty04;
	string unitproperty05;
};



class CBEOPLogicManager
{
public:
	CBEOPLogicManager(wstring strDBFileName, const bool bEncry, CWnd *pWnd);
	~CBEOPLogicManager(void);

	bool ThreadExitFinish();

	bool StoreDllToDB(CDLLObject* dllOb,bool batching = false);
	bool InertDLLtoDB(CDLLObject* dllOb,char *pBlock,int nSize);
	bool ReadDLLfromDB(const wstring &s3dbpath,std::vector<CDLLObject*> &dllObList);
	void ReadDLLMarkfromDB(const wstring &s3dbpath,std::vector<CString> &vecMark);
	bool LoadDLLfromDB();
	int GetCurTimeSpanFromManager(CString strDllName);
	bool DeleteDllfromDB(CString dllname);
	bool ExitDllByName(CString dllname);
	CDLLObject * GetDLLObjectByName(CString dllname);
	bool UpdateTacticName(const CString strTacticNameOld, const CString strTacticNameNew);
	bool UpdateTacticParamConfig(const CString strDllNameOld, const CString strDllNameNew);
	bool UpdateTacticNameOriginal(const CString strNameOld, const CString strNameNew);
	bool UpdateTacticInfo(const CString strName, const CString strNameOrg, const CString strTime, const CString strAuthor, const CString strVer, const CString strDesc);

	bool LoadAndSaveDll(std::vector<CString> PathList,std::vector<CString> DllList, std::vector<CString> strDllOriginNameList);
	bool LoadAndSaveDll(std::vector<CString> PathList,std::vector<CString> DllList, std::vector<CString> strDllOriginNameList,CString strThreadName);
	bool LoadAndSaveDll(std::vector<CString> PathList,std::vector<CString> DllList, std::vector<CString> strDllOriginNameList,CString strThreadName,int nTimeSpan);	//保存配置时间间隔
	bool LoadAndSaveDll(std::vector<CString> PathList,std::vector<CString> DllList, std::vector<CString> strDllOriginNameList, std::vector<CString> strThreadNameList, std::vector<int> vecTimeSpan);	//保存配置时间间隔


	bool UnloadDll(std::vector<CString> DllList);
	bool UnloadDll(std::vector<CString> DllList,CString strThreadName);
	bool UnloadDll(std::vector<CString> DllList,std::vector<CString> ThreadList);
	bool UnloadDll(CString strDllName);
	bool DeleteFromImportList(std::vector<CString> DllList);
	bool DeleteFromImportList(std::vector<CString> DllList,CString strThreadName);

	void SortDllName(std::vector<CString> &iniDllList,std::vector<vec_wstr> &DllList);
	bool RunDLL(CString dllname,bool runstart = true);

	bool Exit();

	bool UpdateDllInfoToDB();

	void FileCopyTo(CString source,CString destination,CString searchStr,BOOL cover = TRUE);
	BOOL CreateFolder(CString strPath);
	BOOL FolderExist(CString strPath);
	BOOL FileExist(CString strFileName);
	CString CopyDllFile(CString selFilePath,CString DllName);
	BOOL SaveMemToFile( const char* pBlock, const int nSize, CString szFileName,CDLLObject* pDllOb);
	BOOL ClearFile(CDLLObject *pObject);
	bool    SaveParametertoDB(int nInOrOutput, CString strDllName,  CString strVariableName, CString strLinkDefine, CString strLinkType);
	bool    SaveParametertoDB(int nInOrOutput, CString strDllName,  CString strVariableName, CString strLinkDefine, CString strLinkType, CString strCondition);
	bool	SaveLogicThreadNameToDB(CString strDllName, CString strThreadName);
	bool    GetInputParameterfromDB(CString strDllName, std::vector<vec_wstr> &inputPara);
	bool	LoadDllThreadRelationship(CLogicDllThread *pDllThread);
	bool    GetOutputParameterfromDB(CString strDllName, std::vector<vec_wstr> &outputPara);
	bool	CompareInputParameter(std::vector<vec_wstr> &inputPara,std::vector<vec_wstr> &Ini_inputPara);
	bool	CompareOutputParameter(std::vector<vec_wstr> &outputPara,std::vector<vec_wstr> &Ini_outputPara);
	bool    UpdateDLLParameter(CDLLObject* dllOb);
	
	bool    SaveInputParametertoDB(CString strDllName, std::vector<vec_str> &strLongPara);
	bool    SaveOutputParametertoDB(CString strDllName, std::vector<vec_str> &strLongPara);

	bool    SaveSpanTimetoDB(CString DllName,double SpanTime);
	std::vector<vec_wstr> SeparateParameter(CString strLongPatameter);
	wstring LinkParameter(std::vector<vec_wstr> &matrixpara);

	CString GetSystVersion();

	void LoadNewConfigFromS3DB(CString strNewDBFileName);
	void	DeleteOldDllAndLoadDllFromNewS3DB(CString strNewDBFileName);		//删除原有dll及参数 从新的s3db中读取

	CDLLObject * FindDLLObject(CString strDllName);

	bool DeleteAllDllParametersOfThread(CLogicDllThread *pDllThread);
	bool DeleteDllParametersOfThreadByNames(vector<wstring> vecDllNames);			//删除包含在vecDllName里的所有dll的参数 add robert

	bool SaveOneParameter(wstring strVName, wstring strPName, wstring strPType, wstring strDllName, wstring strInOut);
	bool UpdateOneParameter(wstring strVName, wstring strPName, wstring strPType, wstring strDllName, wstring strInOut);

	bool	UpdateThreadName(const CString strOldThreadName,const CString strNewThreadName);
	bool	UpdateThreadPeriodicity(const int nPeriodicity ,const CString strThreadName);
	bool	DeleteThreadName(const CString strThreadName);

	bool	FindDllNameIsExist(const CString strDllName);
	bool	FindCharInString(const wstring strChar,const wstring strValue);
	bool    CopyOldDllNameToSdbChangetoNewDllName(const CString strOldDllName,const CString strNewDllName);

	bool OutputSaveErr( const wstring strOperation, const char* err );
	bool GetDeclareParameterfromDB(const CString strDllName, const CString strDllOriginName, const CString strThreadName, CString &vecMark );
	bool SaveDeclareParameterToDB(CString DllName,CString strDeclare, CString strDllOriginName, CString );
public:
	std::vector<vec_wstr> m_vecCurDllInputParameterList;
	std::vector<vec_wstr> m_vecCurDllOutputParameterList;
	std::vector<CString> one_para;

	Project::Tools::Mutex	m_lock;

	wstring m_wsdesFilePath;

	wstring m_strDBFileName;


	bool m_bExitThread;
	bool m_bThreadExitFinished;

	std::vector<CString>	 m_vecDllPath_Rep;
	std::vector<CString>     m_vecSelDllName_Rep; 
	std::vector<CString>	 m_vecSelDllName;   
	std::vector<CDLLObject*> vecImportDLLList;
	std::vector<CLogicDllThread *> m_vecDllThreadList;
	int						 m_iDeleteItem;
	double m_calcinternal;

	CString m_sSystVerion;
public:
	CUPPCServerDataAccess *m_pDataAccess;
	CLogicBase            *m_pCLogicBaseOptimize;
	CCallOptimizeLogicBEOP*m_pOptimizeLogicBEOP;
	pfnLogicBase		  m_pf_Dll;
	HINSTANCE m_hDLL;
	bool m_bEncry;


	CWnd *m_pWnd;
};

