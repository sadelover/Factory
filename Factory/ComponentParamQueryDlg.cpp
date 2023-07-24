// ComponentParamQueryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentParamQueryDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"

#include "afxdialogex.h"
#include "DataPointConfigDlg.h"
#include "DataPoint/DataPointEntry.h"
#include "PointParamEditDlg.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"
// CComponentParamQuery 对话框

IMPLEMENT_DYNAMIC(CComponentParamQuery, CDialogEx)

CComponentParamQuery::CComponentParamQuery(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponentParamQuery::IDD, pParent)
{

}

CComponentParamQuery::~CComponentParamQuery()
{
}

void CComponentParamQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
}


/************************************************************************/
/*函数说明：刷新控件参数                                                */
/************************************************************************/
void CComponentParamQuery::RefreshControls( Json::Value jsonRoot)
{
	m_treeCtrl.DeleteAllItems();
	m_rootList.clear();

	if(!jsonRoot.isMember("type"))
	{
		return ;
	}
	if(jsonRoot["type"] == "ParamQuery")
	{
		if( jsonRoot.isMember("structure") == false){
			return;
		}

		Json::Value jsonFirst = jsonRoot.get("structure",NULL);

		for(int i=0;i<jsonFirst.size(); i++)//遍历一级目录
		{
			//插入内容
			CString cstrFsName( Project::Tools::AnsiToWideChar(jsonFirst[i].get("name", NULL).asString().c_str()).c_str() );
			HTREEITEM hitemFs= m_treeCtrl.InsertItem(cstrFsName, NULL);
			m_rootList.push_back(hitemFs);

			Json::Value jsonSecond = jsonFirst[i].get("children",NULL);
			for(int nSec = 0;nSec<jsonSecond.size(); nSec++)//遍历二级目录
			{
				if (jsonSecond[nSec].isMember("type") == true)
				{
					if (jsonSecond[nSec].get("type", NULL).asString() == "point")
					{
						CString cstrScName( Project::Tools::AnsiToWideChar(jsonSecond[nSec].get("pointname", NULL).asString().c_str()).c_str() );
						HTREEITEM hitemSc = m_treeCtrl.InsertItem(cstrScName, hitemFs);
					}
					else
					{
						CString cstrScName( Project::Tools::AnsiToWideChar(jsonSecond[nSec].get("name", NULL).asString().c_str()).c_str() );
						HTREEITEM hitemSc = m_treeCtrl.InsertItem(cstrScName, hitemFs);

						if (jsonSecond[nSec].isMember("children") == true)
						{
							Json::Value jsonThird = jsonSecond[nSec].get("children", NULL);
							for(int nTr = 0;nTr<jsonThird.size(); nTr++)//遍历三级目录
							{
								CString cstrScName( Project::Tools::AnsiToWideChar(jsonThird[nTr].get("pointname", NULL).asString().c_str()).c_str() );
								m_treeCtrl.InsertItem(cstrScName, hitemSc);
							}
						}
					}
				}
			}
		}
	}
	UpdateData(FALSE);
}

/************************************************************************/
/* 函数说明：按照参数打包json并返回                                     */
/************************************************************************/
Json::Value CComponentParamQuery::GenerateJson()
{
	UpdateData(TRUE);
	
	/*打包*/
	Json::Value JsonPacket;
	JsonPacket["type"] = "ParamQuery";

	JsonPacket["structure"];
	//一级菜单
	for(UINT i = 0; i < m_rootList.size(); i++)
	{
		if(m_rootList[i] == NULL){
			continue;
		}
		Json::Value jsonFirst;
		jsonFirst["name"] = Project::Tools::WideCharToAnsi(m_treeCtrl.GetItemText(m_rootList[i])).c_str();
		jsonFirst["type"] = "group";
		//二级菜单
		if(m_treeCtrl.GetChildItem(m_rootList[i]) != NULL)
		{
			HTREEITEM hitemSc = m_treeCtrl.GetChildItem(m_rootList[i]);
			for( ; hitemSc != NULL;  hitemSc = m_treeCtrl.GetNextSiblingItem(hitemSc) )
			{
				Json::Value jsonSecond;
				//检查是否有第三级
				if (m_treeCtrl.GetChildItem(hitemSc) == NULL)//无
				{
					jsonSecond["pointname"] = Project::Tools::WideCharToAnsi( m_treeCtrl.GetItemText(hitemSc) ).c_str();
					jsonSecond["type"] = "point";
				}
				else//有
				{
					jsonSecond["name"] = Project::Tools::WideCharToAnsi( m_treeCtrl.GetItemText(hitemSc) ).c_str();
					jsonSecond["type"] = "group";

					HTREEITEM hitemTr = m_treeCtrl.GetChildItem(hitemSc);
					for( ; hitemTr != NULL;  hitemTr = m_treeCtrl.GetNextSiblingItem(hitemTr) )
					{
						Json::Value jsonThird;
						jsonThird["pointname"] = Project::Tools::WideCharToAnsi( m_treeCtrl.GetItemText(hitemTr) ).c_str();
						jsonThird["type"] = "point";
						jsonSecond["children"].append(jsonThird);
					}
				}
				jsonFirst["children"].append(jsonSecond);
			}
		}
		JsonPacket["structure"].append(jsonFirst);
	}
	return JsonPacket;
}

int CComponentParamQuery::GetTreeItemLevel(HTREEITEM item)
{
	int nLevel = 0;
	if( item != NULL ){
		nLevel++;
	}

	while( item != NULL )
	{
		item = m_treeCtrl.GetParentItem(item);
		if(item != NULL){
			nLevel++;
		}
	}
	return nLevel;
}

BOOL CComponentParamQuery::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			//PostMessage(TVN_ENDLABELEDIT);
			m_treeCtrl.SendMessage(TVN_ENDLABELEDIT);
			//SendMessage(TVN_ENDLABELEDIT);
			return TRUE;
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CComponentParamQuery, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELETEPOINT, &CComponentParamQuery::OnBnClickedButtonSeletepoint)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ROOT, &CComponentParamQuery::OnBnClickedButtonAddRoot)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CComponentParamQuery::OnTvnEndlabeleditTree1)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SUB, &CComponentParamQuery::OnBnClickedButtonAddSub)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ITEM, &CComponentParamQuery::OnBnClickedButtonDeleteItem)
END_MESSAGE_MAP()


// CComponentParamQuery 消息处理程序

BOOL CComponentParamQuery::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CComponentParamQuery::OnBnClickedButtonSeletepoint()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = m_treeCtrl.GetSelectedItem();
	if(hItem == NULL){
		AfxMessageBox(L"需要选择一个插入点");
		return;
	}
	if(GetTreeItemLevel(hItem) == 3)
	{
		AfxMessageBox(L"仅支持三层");
		return;
	}
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_treeCtrl.SetRedraw(FALSE);
		m_treeCtrl.InsertItem(dlg.m_strPointName, hItem);
		m_treeCtrl.SetRedraw(TRUE);
	}
	UpdateData(FALSE);
}


void CComponentParamQuery::OnBnClickedButtonAddRoot()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hItem = m_treeCtrl.InsertItem(L"Root", NULL);
	m_rootList.push_back(hItem);
	m_treeCtrl.SelectItem(hItem);
	UpdateData(FALSE);
}

void CComponentParamQuery::OnBnClickedButtonAddSub()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hitem = m_treeCtrl.GetSelectedItem();

	if(hitem == NULL){
		AfxMessageBox(L"请选择一个");
	}

	if(GetTreeItemLevel(hitem) == 3)
	{
		AfxMessageBox(L"仅支持三层");
		return;
	}

	m_treeCtrl.SetRedraw(FALSE);
	m_treeCtrl.InsertItem(L"Sub", hitem);
	m_treeCtrl.SetRedraw(TRUE);
}

void CComponentParamQuery::OnBnClickedButtonDeleteItem()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hitem = m_treeCtrl.GetSelectedItem();

	if(hitem == NULL){
		AfxMessageBox(L"请选择一个");
	}

	m_treeCtrl.DeleteItem(hitem);
	if(GetTreeItemLevel(hitem) == 1)
	{
		auto iter = m_rootList.begin();
		for (; iter != m_rootList.end(); iter++)
		{
			if((*iter) == hitem)
			{
				m_rootList.erase(iter);
				break;
			}
		}
	}
}

void CComponentParamQuery::OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CString strName(pTVDispInfo->item.pszText);
	if( strName.GetLength() > 0 )
	{
		m_treeCtrl.SetItemText( pTVDispInfo->item.hItem, strName);
	}
	UpdateData(FALSE);
	*pResult = 0;
}