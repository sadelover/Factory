// CalcModulePanel.cpp : 实现文件
//

#include "stdafx.h"
#include "CalcModulePanel.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "CalcAddItemDlg.h"


// CCalcModulePanel 对话框

IMPLEMENT_DYNAMIC(CCalcModulePanel, CDialogEx)

CCalcModulePanel::CCalcModulePanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalcModulePanel::IDD, pParent)
	, m_strPointName(_T(""))
	, m_pMainFrame(NULL)
	, m_pPrjSql(NULL)
{

}

CCalcModulePanel::~CCalcModulePanel()
{
}

void CCalcModulePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXT_CALC_POINT_NAME, m_strPointName);
	DDX_Control(pDX, IDC_LIST_ITEM, m_list);
}


BEGIN_MESSAGE_MAP(CCalcModulePanel, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ITEM_ADD, &CCalcModulePanel::OnBnClickedBtnItemAdd)
	ON_BN_CLICKED(IDC_BTN_ITEM_DEL, &CCalcModulePanel::OnBnClickedBtnItemDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ITEM, &CCalcModulePanel::OnNMDblclkListItem)
END_MESSAGE_MAP()


// CCalcModulePanel 消息处理程序


BOOL CCalcModulePanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();

	InitControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/*
void CCalcModulePanel::OnBnClickedBtnPointTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileStatus rStatus;
	if (!CFile::GetStatus(m_strTempFold, rStatus))
	{
		if(!CreateDirectory((LPCTSTR)m_strTempFold, NULL))
		{	// error
			return;
		}
	}

	USES_CONVERSION;
	// generate python file into temp folder
	CString strPtPath;
	strPtPath.Format(_T("%s\\%s.py"), m_strTempFold, m_strPointName);
	CFile file;
	if (!file.Open(strPtPath, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite|CFile::shareDenyRead))
	{
		return;
	}
	try
	{
		UpdateData(TRUE);
		char szBufWrite[100000] = {0};
		char szHead[] = {"\
# -*- encoding=utf-8 -*-\r\n\
from MySQLOperator import MySQLOperator as mysql\r\n\
\r\n\
def Run():\r\n\
    \
"};
		CString strContent(m_strPointContent);
		strContent.Replace(_T("get_data_float"), _T("mysql.get_data_float"));
		strContent.Replace(_T("get_data_int"), _T("mysql.get_data_int"));
		strContent.Replace(_T("get_data_string"), _T("mysql.get_data_string"));

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

		pModule = PyImport_ImportModule(T2A(m_strPointName));
		pFunc   = PyObject_GetAttrString(pModule, "Run");
		pReturn = PyEval_CallObject(pFunc, NULL);
		float fRes = 0;
		PyArg_Parse(pReturn, "f", &fRes);

		Py_Finalize();

		CString strLog;
		strLog.Format(_T("测试结果为：%.06f\r\n"), fRes);
		m_strPointLog += strLog;
		UpdateData(FALSE);
	}
	catch (...)
	{
		return;
	}
}
*/

void CCalcModulePanel::OnBnClickedBtnItemAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_pPrjSql)
	{
		m_pPrjSql = m_pMainFrame->m_pPrjSqlite;
		if (NULL == m_pPrjSql)
		{
			return;
		}
	}

	CCalcAddItemDlg dlg(TRUE);
	if (IDOK == dlg.DoModal())
	{
		// insert into memory
		vector<CalcModuleInfo>::iterator	iter;
		for (iter=m_pPrjSql->m_vecCalcModuleInfo.begin(); iter!=m_pPrjSql->m_vecCalcModuleInfo.end(); iter++)
		{
			if (m_strPointName == iter->strName)
			{
				CalcModuleItem item;
				item.strName = dlg.m_strName;
				item.strContent = dlg.m_strContent;
				item.strParentName = m_strPointName;
				iter->vecItem.push_back(item);
				break;
			}
		}

		m_list.InsertItem(m_list.GetItemCount(), dlg.m_strName);
		m_pPrjSql->InsertOneIntoCalcModItem(dlg.m_strName, m_strPointName, dlg.m_strContent, m_pMainFrame->m_bSyncDec);
	}
}


void CCalcModulePanel::OnBnClickedBtnItemDel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_pPrjSql)
	{
		m_pPrjSql = m_pMainFrame->m_pPrjSqlite;
		if (NULL == m_pPrjSql)
		{
			return;
		}
	}

	CString strName;
	int nSelRow = -1;
	if (!GetListCurrentSelectedInfo(nSelRow, strName))
	{
		return;
	}
	if (strName.GetLength() <= 0)
	{
		return;
	}

	CString strConfirm(_T("确认删除 ") + strName + _T(" ?"));
	if (IDYES != MessageBox(strConfirm, _T("提示"), MB_YESNO | MB_ICONASTERISK))
	{
		return;
	}

	// delete from memory
	vector<CalcModuleInfo>::iterator	iter;
	vector<CalcModuleItem>::iterator	iterItem;
	BOOL bFind = FALSE;
	for (iter=m_pPrjSql->m_vecCalcModuleInfo.begin(); iter!=m_pPrjSql->m_vecCalcModuleInfo.end(); iter++)
	{
		if (m_strPointName == iter->strName)
		{
			vector<CalcModuleItem>& vecItem(iter->vecItem);
			for (iterItem=vecItem.begin(); iterItem!=vecItem.end(); iterItem++)
			{
				if (strName == iterItem->strName)
				{
					bFind = TRUE;
					vecItem.erase(iterItem);
					break;
				}
			}
			break;
		}
	}

	if (bFind)
	{
		m_list.DeleteItem(nSelRow);
		m_pPrjSql->DeleteOneFromCalcModItem(strName, m_strPointName, m_pMainFrame->m_bSyncDec);
	}
}


void CCalcModulePanel::InitControls(void)
{
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("名称"));
	m_list.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

	ResetListControl();
}


void CCalcModulePanel::ResetListControl(void)
{
	m_list.DeleteAllItems();

	vector<CalcModuleItem>::const_iterator	iter;
	int i= 0;
	for (iter=m_vecCalcItem.begin(); iter!=m_vecCalcItem.end(); iter++,i++)
	{
		m_list.InsertItem(i, iter->strName);
	}
}


BOOL CCalcModulePanel::GetListCurrentSelectedInfo(int& nSelRow, CString& strName)
{
	nSelRow = m_list.GetNextItem(-1, LVIS_SELECTED);
	if (nSelRow < 0)
	{
		return FALSE;
	}
	strName = m_list.GetItemText(nSelRow, 0);
	return TRUE;
}

void CCalcModulePanel::SetCalcItemVector(const vector<CalcModuleItem>& vecSrc)
{
	m_vecCalcItem = vecSrc;

	ResetListControl();
}

void CCalcModulePanel::ClearCalcItemVector(void)
{
	m_vecCalcItem.clear();
}


void CCalcModulePanel::OnNMDblclkListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (NULL == m_pPrjSql)
	{
		m_pPrjSql = m_pMainFrame->m_pPrjSqlite;
		if (NULL == m_pPrjSql)
		{
			return;
		}
	}

	const int nCurSel = pNMItemActivate->iItem;
	CString strName = m_list.GetItemText(nCurSel, 0);

	vector<CalcModuleInfo>& vecCalc = m_pPrjSql->m_vecCalcModuleInfo;
	vector<CalcModuleInfo>::iterator	iter;
	vector<CalcModuleItem>::iterator	iterItem;
	BOOL bFind = FALSE;
	CString strContent;
	for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
	{
		if (m_strPointName == iter->strName)
		{
			for (iterItem=iter->vecItem.begin(); iterItem!=iter->vecItem.end(); iterItem++)
			{
				if (strName == iterItem->strName)
				{
					strContent = iterItem->strContent;
					bFind = TRUE;
					break;
				}
			}
			break;
		}
	}

	if (bFind)
	{
		CCalcAddItemDlg dlg(FALSE);
		dlg.m_strName = strName;
		dlg.m_strContent = strContent;
		if (IDOK == dlg.DoModal())
		{
			for (iter=vecCalc.begin(); iter!=vecCalc.end(); iter++)
			{
				if (m_strPointName == iter->strName)
				{
					for (iterItem=iter->vecItem.begin(); iterItem!=iter->vecItem.end(); iterItem++)
					{
						if (strName == iterItem->strName)
						{
							iterItem->strContent = dlg.m_strContent;
							m_pPrjSql->UpdateCalcModItem(strName, m_strPointName, dlg.m_strContent, m_pMainFrame->m_bSyncDec);
							break;
						}
					}
					break;
				}
			}
		}
	}
}
