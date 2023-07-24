// ComponentParamQueryDlg.cpp : ʵ���ļ�
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
// CComponentParamQuery �Ի���

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
/*����˵����ˢ�¿ؼ�����                                                */
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

		for(int i=0;i<jsonFirst.size(); i++)//����һ��Ŀ¼
		{
			//��������
			CString cstrFsName( Project::Tools::AnsiToWideChar(jsonFirst[i].get("name", NULL).asString().c_str()).c_str() );
			HTREEITEM hitemFs= m_treeCtrl.InsertItem(cstrFsName, NULL);
			m_rootList.push_back(hitemFs);

			Json::Value jsonSecond = jsonFirst[i].get("children",NULL);
			for(int nSec = 0;nSec<jsonSecond.size(); nSec++)//��������Ŀ¼
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
							for(int nTr = 0;nTr<jsonThird.size(); nTr++)//��������Ŀ¼
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
/* ����˵�������ղ������json������                                     */
/************************************************************************/
Json::Value CComponentParamQuery::GenerateJson()
{
	UpdateData(TRUE);
	
	/*���*/
	Json::Value JsonPacket;
	JsonPacket["type"] = "ParamQuery";

	JsonPacket["structure"];
	//һ���˵�
	for(UINT i = 0; i < m_rootList.size(); i++)
	{
		if(m_rootList[i] == NULL){
			continue;
		}
		Json::Value jsonFirst;
		jsonFirst["name"] = Project::Tools::WideCharToAnsi(m_treeCtrl.GetItemText(m_rootList[i])).c_str();
		jsonFirst["type"] = "group";
		//�����˵�
		if(m_treeCtrl.GetChildItem(m_rootList[i]) != NULL)
		{
			HTREEITEM hitemSc = m_treeCtrl.GetChildItem(m_rootList[i]);
			for( ; hitemSc != NULL;  hitemSc = m_treeCtrl.GetNextSiblingItem(hitemSc) )
			{
				Json::Value jsonSecond;
				//����Ƿ��е�����
				if (m_treeCtrl.GetChildItem(hitemSc) == NULL)//��
				{
					jsonSecond["pointname"] = Project::Tools::WideCharToAnsi( m_treeCtrl.GetItemText(hitemSc) ).c_str();
					jsonSecond["type"] = "point";
				}
				else//��
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


// CComponentParamQuery ��Ϣ�������

BOOL CComponentParamQuery::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CComponentParamQuery::OnBnClickedButtonSeletepoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem = m_treeCtrl.GetSelectedItem();
	if(hItem == NULL){
		AfxMessageBox(L"��Ҫѡ��һ�������");
		return;
	}
	if(GetTreeItemLevel(hItem) == 3)
	{
		AfxMessageBox(L"��֧������");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hItem = m_treeCtrl.InsertItem(L"Root", NULL);
	m_rootList.push_back(hItem);
	m_treeCtrl.SelectItem(hItem);
	UpdateData(FALSE);
}

void CComponentParamQuery::OnBnClickedButtonAddSub()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hitem = m_treeCtrl.GetSelectedItem();

	if(hitem == NULL){
		AfxMessageBox(L"��ѡ��һ��");
	}

	if(GetTreeItemLevel(hitem) == 3)
	{
		AfxMessageBox(L"��֧������");
		return;
	}

	m_treeCtrl.SetRedraw(FALSE);
	m_treeCtrl.InsertItem(L"Sub", hitem);
	m_treeCtrl.SetRedraw(TRUE);
}

void CComponentParamQuery::OnBnClickedButtonDeleteItem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hitem = m_treeCtrl.GetSelectedItem();

	if(hitem == NULL){
		AfxMessageBox(L"��ѡ��һ��");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strName(pTVDispInfo->item.pszText);
	if( strName.GetLength() > 0 )
	{
		m_treeCtrl.SetItemText( pTVDispInfo->item.hItem, strName);
	}
	UpdateData(FALSE);
	*pResult = 0;
}