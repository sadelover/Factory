// SearchStrategyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SearchStrategyDlg.h"
#include "afxdialogex.h"


// CSearchStrategyDlg 对话框

IMPLEMENT_DYNAMIC(CSearchStrategyDlg, CDialogEx)

CSearchStrategyDlg::CSearchStrategyDlg(CBEOPLogicManager * pLogicManager, vector<CLogicDllThread *> * pLogicList, CFileView* pFileView, HTREEITEM pTreeRoot, CWnd* pParent /*= NULL*/) : CDialogEx(CSearchStrategyDlg::IDD, pParent)
	, m_strStrategyNameList(_T(""))
	, m_pLogicManager(pLogicManager)
	, m_pLogicList(pLogicList)
	, m_pFileView(pFileView)
	, m_pTreeRoot(pTreeRoot)
{

}

CSearchStrategyDlg::~CSearchStrategyDlg()
{
}

void CSearchStrategyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRATEGY_NAME, m_strStrategyNameList);
	DDX_Control(pDX, IDC_LIST_STRATEGY, m_list_strategy);
	DDX_Control(pDX, IDC_PROGRESS_Search, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CSearchStrategyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearchStrategyDlg::OnBnClickedButtonSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STRATEGY, &CSearchStrategyDlg::OnNMDblclkListStrategy)
END_MESSAGE_MAP()


// CSearchStrategyDlg 消息处理程序


void CSearchStrategyDlg::OnBnClickedButtonSearch()
{
	UpdateData(TRUE);
	AfxBeginThread(Thread_ButtonSearch, (LPVOID)this);
}

void CSearchStrategyDlg::OnNMDblclkListStrategy(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nIndex =  m_list_strategy.GetItemData(pNMItemActivate->iItem);
	if(nIndex != EOF)
	{
		HTREEITEM hNextItem = FindHTREEITEM(m_pTreeRoot, m_foundStrategyList[nIndex].strThreadName);
		if(hNextItem == NULL)
			return;
		m_pFileView->m_wndFileView.SelectItem(hNextItem);
		LRESULT temp;
		m_pFileView->OnNMDblclkTree(NULL,&temp);
	}
	*pResult = 0;
}


HTREEITEM CSearchStrategyDlg::FindHTREEITEM(HTREEITEM item, CString strtext)
{
	HTREEITEM  hfind;  
	HTREEITEM hNextItem = item;
	//空树，直接返回NULL
	if(hNextItem ==  NULL)  
		return  NULL;  
	//遍历查找
	while(hNextItem!=NULL)  
	{  
		//当前节点即所需查找节点
		CString cstrName = m_pFileView->m_wndFileView.GetItemText(hNextItem);
		cstrName = cstrName.Left(cstrName.Find(L":"));
		if( cstrName == strtext)  
			return hNextItem;  
		//查找当前节点的子节点
		if(m_pFileView->m_wndFileView.ItemHasChildren(hNextItem))//如果指定的项目有子项，则返回非零值。  
		{  
			hNextItem = m_pFileView->m_wndFileView.GetChildItem(hNextItem);//获取子项地址
			//递归调用查找子节点下节点
			hfind = FindHTREEITEM(hNextItem,strtext);  
			if(hfind)  
			{  
				return  hfind;  
			}  
			else   //子节点中未发现所需节点，继续查找兄弟节点
				hNextItem = m_pFileView->m_wndFileView.GetNextSiblingItem(m_pFileView->m_wndFileView.GetParentItem(hNextItem));  
		}  
		else
		{   //若无子节点，继续查找兄弟节点
			hNextItem = m_pFileView->m_wndFileView.GetNextSiblingItem(hNextItem);  
		}  
	}  
	return hNextItem;  
}

UINT CSearchStrategyDlg::Thread_ButtonSearch(LPVOID pParam)
{
	CSearchStrategyDlg* pThis =  (CSearchStrategyDlg*)pParam;
	pThis->GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);
	pThis->SearchStartegy();
	pThis->GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(TRUE);
	return 0;
}

void CSearchStrategyDlg::SearchStartegy()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pLogicManager == NULL)
		return;
	if(m_pLogicList == NULL)
		return;
	vector<CString> strPhraseList;
	Project::Tools::SplitStringByChar(m_strStrategyNameList, L' ', strPhraseList);
	if(strPhraseList.size() == 0)
		return;

	m_foundStrategyList.clear();
	m_list_strategy.DeleteAllItems();
	m_ProgressCtrl.SetPos(0);
	m_ProgressCtrl.SetRange(0,100);
	int nDivisionVal  = 100/m_pLogicList->size();

	//遍历线程
	for(int nC_Thread=0; nC_Thread<m_pLogicList->size(); nC_Thread++)
	{
		m_pLogicManager->LoadDllThreadRelationship(m_pLogicList->at(nC_Thread));//加载dll读出输出输入参数m_vecCurDllInputParameterList
		//遍历策略
		for(int nC_Strategy=0; nC_Strategy<m_pLogicList->at(nC_Thread)->GetDllCount(); nC_Strategy++)
		{
			CDLLObject * temp_dllObject = m_pLogicList->at(nC_Thread)->GetDllObject(nC_Strategy);
			CString strDllname = temp_dllObject->GetDllName();
			strDllname = strDllname.Left(strDllname.Find(L".dll"));

			if(strDllname.GetLength()==0)
				continue;

			for(int i=0;i<strPhraseList.size();i++)
			{
				if(strDllname.Find(strPhraseList[i]) != EOF){
					m_foundStrategyList.push_back(StrategyStruct(m_pLogicList->at(nC_Thread)->GetName().c_str(),strDllname));
					break;
				}
			}
		}
		m_ProgressCtrl.SetPos(m_ProgressCtrl.GetPos()+nDivisionVal);
	}

	if(m_foundStrategyList.size() == 0)
		AfxMessageBox(L"没有找到类似策略");
	for(int i=0;i<m_foundStrategyList.size();i++)
	{
		m_list_strategy.InsertItem(i, m_foundStrategyList[i].strStrategyName);
		m_list_strategy.SetItemData(i, i);
	}
}

