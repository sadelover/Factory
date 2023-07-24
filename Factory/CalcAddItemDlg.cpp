// CalcAddItemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CalcAddItemDlg.h"
#include "afxdialogex.h"
#include "HttpOperation.h"
#include "MainFrm.h"


// CCalcAddItemDlg 对话框

IMPLEMENT_DYNAMIC(CCalcAddItemDlg, CDialogEx)

CCalcAddItemDlg::CCalcAddItemDlg(const BOOL bIsAdd, CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalcAddItemDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strContent(_T(""))
	, m_bIsAdd(bIsAdd)
	, m_strShow(_T(""))
	, m_pMainFrm(NULL)
	, m_nCalcPort(5000)
{

}

CCalcAddItemDlg::~CCalcAddItemDlg()
{
}

void CCalcAddItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_strContent);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_strShow);
}


BEGIN_MESSAGE_MAP(CCalcAddItemDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_TEST, &CCalcAddItemDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CCalcAddItemDlg 消息处理程序


BOOL CCalcAddItemDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		switch (pMsg->wParam)
		{
		case 'a':
		case 'A':
			{
				if (bCtrl)
				{
					CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CONTENT);
					if (pEdit != NULL)
					{
						pEdit->SetFocus();
						pEdit->SetSel(0, -1);
					}
				}  
				break;
			}
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CCalcAddItemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_bIsAdd)
	{
		SetWindowText(_T("新建"));
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow(TRUE);
	}
	else
	{
		SetWindowText(_T("编辑"));
		GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
	}

	TCHAR szAppDir[MAX_PATH] = {0};
	GetModuleFileName(NULL, szAppDir, MAX_PATH);
	PathRemoveFileSpec(szAppDir);
	m_strTempFold.Format(_T("%s\\temp"), szAppDir);
	if (NULL == m_pMainFrm)
	{
		m_pMainFrm = (CMainFrame*)AfxGetMainWnd();
		if (NULL != m_pMainFrm)
		{
			m_strCalcIp = m_pMainFrm->m_strCalcIp;
			m_nCalcPort = m_pMainFrm->m_nCalcPort;
			m_strCalcPostName = m_pMainFrm->m_strCalcPostName;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCalcAddItemDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	m_strContent.Replace(_T("\""), _T("\'"));
	UpdateData(FALSE);

	CDialogEx::OnOK();
}


void CCalcAddItemDlg::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSrc;
	CString strResult;
	CHttpOperation http(m_strCalcIp, m_nCalcPort, m_strCalcPostName);

	UpdateData(TRUE);
	USES_CONVERSION;
	m_strContent.Replace(_T("\""), _T("\'"));
	UpdateData(FALSE);

	CString strParse(m_strContent);
	strParse.Replace(_T("+"), _T("%2b"));
	strParse.Replace(_T("&"), _T("%26"));
	strParse.Replace(_T("="), _T("%3d"));

	const int nFileSize = 50000;
	char szBuf[nFileSize] = {};
	strcpy_s(szBuf, nFileSize, T2A(strParse.GetString()));

	bool bRet = http.HttpPostGetRequestEx(0, szBuf, strResult);
	CString strShow;
	CString strRet(_T("\r\n")); 
	CString strTips;
	if (!bRet)
	{
		strTips = _T("测试失败！");
	} 
	else
	{
		strTips = _T("测试成功！");
	}
	strShow.Format(_T("%s%s%s"), strTips, strRet, strResult);
	SetResultText(strShow);
	return;


	//////////////////////////////////////////////////////////////////////////
	/*
	UpdateData(TRUE);
	CString strContent(m_strContent);
	CString strLog;
	if (strContent.GetLength() <= 0)
	{
		SetResultText(_T("未输入公式！"));
		return;
	}

	CFileStatus rStatus;
	if (!CFile::GetStatus(m_strTempFold, rStatus))
	{
		if(!CreateDirectory((LPCTSTR)m_strTempFold, NULL))
		{	// error
			return;
		}
	}

	//USES_CONVERSION;
	// generate python file into temp folder
	CString strPtPath;
	strPtPath.Format(_T("%s\\%s.py"), m_strTempFold, m_strName);
	CFile file;
	if (!file.Open(strPtPath, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite|CFile::shareDenyRead))
	{
		return;
	}
	try
	{
		char szBufWrite[100000] = {0};
		char szHead[] = {"\
# -*- encoding=utf-8 -*-\r\n\
from MySQLOperator import MySQLOperator as mysql\r\n\
\r\n\
"};
		strContent.Replace(_T("get_data_float"), _T("mysql.get_data_float"));
		strContent.Replace(_T("get_data_int"), _T("mysql.get_data_int"));
		strContent.Replace(_T("get_data_string"), _T("mysql.get_data_string"));
		strContent.Replace(_T("set_data"), _T("mysql.set_data"));

		sprintf_s(szBufWrite, 100000, "%s%s", szHead, T2A(strContent));
		int len = strlen(szBufWrite);
		file.Write(szBufWrite, len);         
		file.Flush();
		file.Close();
	}
	catch (...)
	{
		return;
	}

	// execute python file
	try
	{
		Py_Initialize();

		CString strTemp(m_strTempFold);
		strTemp.Replace(_T('\\'), _T('/'));
		char szFold[MAX_PATH] = {0};
		sprintf_s(szFold, MAX_PATH, "sys.path.append('%s')", T2A(strTemp));

		PyRun_SimpleString("import sys");
		PyRun_SimpleString(szFold);

		PyObject* pModule	= NULL;
		PyObject* pFunc		= NULL;
		PyObject* pReturn	= NULL;

		pModule = PyImport_ImportModule(T2A(m_strName));
		if (NULL == pModule)
		{
			SetResultText(_T("输入有错，测试不通过！"));
			Py_Finalize();
			return;
		}

		pFunc = PyObject_GetAttrString(pModule, "main");
		if (NULL == pFunc)
		{
			SetResultText(_T("寻找main函数出错，测试不通过！"));
			Py_Finalize();
			return;
		}

		pReturn = PyEval_CallObject(pFunc, NULL);
		if (NULL == pReturn)
		{
			SetResultText(_T("执行出错，测试不通过！"));
			Py_Finalize();
			return;
		}

		float fRes = 0;
		PyArg_ParseTuple(pReturn, "f", &fRes);//

		SetResultText(_T("测试通过！"));
		Py_Finalize();
	}
	catch (...)
	{
		SetResultText(_T("出现异常，测试不通过！"));
		return;
	}
	*/
}

void CCalcAddItemDlg::SetResultText(CString strShow)
{
	const CString strRet(_T("\r\n"));
	CString strLog;
	strLog.Format(_T("%s%s"), strShow, strRet);
	m_strShow += strLog;
	UpdateData(FALSE);
}
