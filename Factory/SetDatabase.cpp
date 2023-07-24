// SetDatabase.cpp : implementation file
//

#include "stdafx.h"
#include "SetDatabase.h"
#include "afxdialogex.h"
#include "../DB_BasicIO/CommonDBAccess.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"
#include "Tools/ToolsFunction.h"
#include <stdlib.h>

// CSetDatabase dialog

IMPLEMENT_DYNAMIC(CSetDatabase, CDialog)

CSetDatabase::CSetDatabase(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDatabase::IDD, pParent)
	, m_nBackupDay(30)
	,m_bDecimalpointdigit(2)
	
	, m_nTmpTableSize(0)
	, m_nMaxHeapTableSize(0)
	, m_bInnodbFilePertable(FALSE)
	, m_bIsChange(FALSE)
{

}

CSetDatabase::~CSetDatabase()
{
}
BOOL CSetDatabase::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetDatabase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BACKUP_DAY, m_nBackupDay);
	DDX_Text(pDX, IDC_EDIT_DECIMAL_DIGIT, m_bDecimalpointdigit);

	
	DDX_Text(pDX, IDC_EDIT_DECIMAL_DIGIT2, m_nTmpTableSize);
	DDX_Text(pDX, IDC_EDIT_DECIMAL_DIGIT4, m_nMaxHeapTableSize);
	DDX_Check(pDX, IDC_CHECK1, m_bInnodbFilePertable);
}


BEGIN_MESSAGE_MAP(CSetDatabase, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_MYSQL, &CSetDatabase::OnBnClickedButtonSettingMysql)
	ON_EN_CHANGE(IDC_EDIT_DECIMAL_DIGIT2, &CSetDatabase::OnEnChangeEditDecimalDigit2)
	ON_EN_CHANGE(IDC_EDIT_DECIMAL_DIGIT4, &CSetDatabase::OnEnChangeEditDecimalDigit4)
	ON_BN_CLICKED(IDC_CHECK1, &CSetDatabase::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CSetDatabase message handlers


void CSetDatabase::OnBnClickedButtonSettingMysql()
{
	// TODO: Add your control notification handler code here
	BOOL tempM_bInnodbFilePertable=m_bInnodbFilePertable;
	int tempM_nTmpTableSize=m_nTmpTableSize;
	int tempM_nMaxHeapTableSize=m_nMaxHeapTableSize;
	UpdateData(TRUE);
	if(m_pDataAccess)
	{

		CString str;
		str.Format(_T("%d"),m_nBackupDay);
		m_pDataAccess->WriteCoreDebugItemValue(L"backupday",str.GetString());

		str.Format(_T("%d"),m_bDecimalpointdigit);
		m_pDataAccess->WriteCoreDebugItemValue(L"precision",str.GetString());

		/*
		//julian 0824
		
		int middletempTmpTableSize=m_nTmpTableSize*1024*1024;
		str.Format(_T("%d"),middletempTmpTableSize);
		bool bWrite1 = m_pDataAccess->WriteMysqlGlobalVariable(L"tmp_table_size",str.GetString());

		middletempTmpTableSize= m_nMaxHeapTableSize*1024*1024;
		str.Format(_T("%d"), middletempTmpTableSize);
		bool bWrite2 = m_pDataAccess->WriteMysqlGlobalVariable(L"max_heap_table_size",str.GetString());
		*/
		//end

		//julian 0826
		
		
		wstring strIniPath= m_pDataAccess->ReadGlobalMysqlVariable_Defalut(L"basedir");
		wstring iniPath= strIniPath+L"my.ini";

		str.Format(_T("%d"),m_bInnodbFilePertable);
		WritePrivateProfileString(L"mysqld", L"innodb_file_per_table", str,iniPath.c_str());

		str.Format(_T("%d"),m_nTmpTableSize);
		WritePrivateProfileString(L"mysqld", L"tmp_table_size", str+L"M",iniPath.c_str());

		str.Format(_T("%d"),m_nMaxHeapTableSize);
		WritePrivateProfileString(L"mysqld", L"max_heap_table_size", str+L"M",iniPath.c_str());

		
		
		//重启mysql 
		
		/*
		if (m_bIsChange)
		{
			ShellExecuteA(0,"runas", LPCSTR("cmd.exe"),LPCSTR("/c net stop mysql &net start mysql"),"",SW_HIDE);
			m_bIsChange = FALSE;
		}
		
		//ShellExecuteA(0,"runas",LPCSTR("cmd.exe"),LPCSTR("/c net start mysql"),"",SW_HIDE);
		*/
		if(tempM_bInnodbFilePertable!=m_bInnodbFilePertable || tempM_nTmpTableSize!=m_nTmpTableSize ||tempM_nMaxHeapTableSize!=m_nMaxHeapTableSize)
		{
			ShellExecuteA(0,"runas", LPCSTR("cmd.exe"),LPCSTR("/c net stop mysql &net start mysql"),"",SW_HIDE);
		}
		AfxMessageBox(_T("保存成功,首先可能需要重启你的计算机生效!"));

		//UpdateWindow();
		Invalidate();

	}
}
bool CSetDatabase::InitData()
{
	//mysql

	wstring strBackupDay= m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"backupday",L"30");
	if(strBackupDay==L"")
		strBackupDay = L"30";
	m_nBackupDay = _wtoi(strBackupDay.c_str());

	wstring strDecimalpointdigit = m_pDataAccess->ReadOrCreateCoreDebugItemValue_Defalut(L"precision",L"2");
	if(strDecimalpointdigit ==L"")
		strDecimalpointdigit  = L"2";
	m_bDecimalpointdigit = _wtoi(strDecimalpointdigit.c_str());

	/*
	//julian 0824
	wstring strInnodbFilePerTable= m_pDataAccess->ReadGlobalMysqlVariable_Defalut(L"innodb_file_per_table");
	m_nInnodbFilePerTable= _wtoi(strInnodbFilePerTable.c_str());

	wstring strTmpTableSize= m_pDataAccess->ReadGlobalMysqlVariable_Defalut(L"tmp_table_size");
	m_nTmpTableSize= _wtoi(strTmpTableSize.c_str());
	

	wstring strMaxHeapTableSize= m_pDataAccess->ReadGlobalMysqlVariable_Defalut(L"max_heap_table_size");
	m_nMaxHeapTableSize= _wtoi(strMaxHeapTableSize.c_str());
	*/
	

	//julian 0826
	
	wchar_t value[1024];
	//该出的路径要用"/",如果用"\"则路径无效
	wstring strIniPath= m_pDataAccess->ReadGlobalMysqlVariable_Defalut(L"basedir");
	wstring iniPath= strIniPath+L"my.ini";

	DWORD num=GetPrivateProfileString(L"mysqld", L"max_heap_table_size", L"",value,1024,iniPath.c_str());
	wstring  strMaxHeapTableSize=value;
	if(strMaxHeapTableSize == L""  )
	{
		strMaxHeapTableSize=L"128M";
		WritePrivateProfileString(L"mysqld", L"max_heap_table_size", strMaxHeapTableSize.c_str(),iniPath.c_str());
	}
	m_nMaxHeapTableSize=_wtoi(strMaxHeapTableSize.c_str());

	
	GetPrivateProfileString(L"mysqld", L"innodb_file_per_table", L"",value,1024,iniPath.c_str());
	wstring  strInnodbFilePerTable=value;
	if(strInnodbFilePerTable == L"" )
	{
		strInnodbFilePerTable=L"1";
		WritePrivateProfileString(L"mysqld", L"innodb_file_per_table", strInnodbFilePerTable.c_str(),iniPath.c_str());
	}
	m_bInnodbFilePertable=_wtoi(strInnodbFilePerTable.c_str());


	GetPrivateProfileString(L"mysqld", L"tmp_table_size", L"",value,1024,iniPath.c_str());
	wstring  strTmpTableSize=value;
	if(strTmpTableSize == L"" )
	{
		strTmpTableSize=L"128M";
		BOOL result=WritePrivateProfileString(L"mysqld", L"tmp_table_size", strTmpTableSize.c_str(),iniPath.c_str());
	}
	m_nTmpTableSize=_wtoi(strTmpTableSize.c_str());
	
	

	UpdateData(FALSE);
	return true;
}


void CSetDatabase::OnEnChangeEditDecimalDigit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	m_bIsChange = TRUE;
}


void CSetDatabase::OnEnChangeEditDecimalDigit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	m_bIsChange = TRUE;
}




void CSetDatabase::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	m_bIsChange = TRUE;
}
