// CommOPCSearch.cpp : implementation file
//

#include "stdafx.h"
#include "CommOPCSearch.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "OPCCtrl.h"
#include <locale>
#include <codecvt>
#include <algorithm>
#include <vector>
#include "OPCSetting.h"
#include "AddOPCItem.h"
#include "OPCSettingDlg.h"
#include <iostream>
#include <sstream>

// CCommOPCSearch dialog
CViewTree	CCommOPCSearch::m_wndFileView;
HTREEITEM	CCommOPCSearch::m_hRoot;
HTREEITEM	CCommOPCSearch::m_hSelectNode;
CRITICAL_SECTION CCommOPCSearch::m_criticalSection = {0};
hash_map<wstring,hash_map<wstring,HTREEITEM>>	CCommOPCSearch::m_mapAllPoint;
hash_map<wstring,OPCInfo*>	CCommOPCSearch::m_mapDeveice;


IMPLEMENT_DYNAMIC(CCommOPCSearch, CDialog)

CCommOPCSearch::CCommOPCSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CCommOPCSearch::IDD, pParent)
	, m_strOPCIP(_T("127.0.0.1"))
	, m_strOPCFiler(_T("*"))
	, m_nBrowseType(2)
	, m_strLOG(_T(""))
	, m_nMutilLoad(100)
	, m_nLoadInterval(10)
	, m_pLoadingOPC(NULL)
	, m_bLoading(false)
	, m_proHandler(NULL)
	, m_nCurrentCount(0)
	, m_nTotalCount(0)
	, m_bReadDescription(true)
	, m_nMutilAddItem(1000)
	, m_strDataTypeFilter(_T("Native"))
{
	m_strDataTypeFilter = _T("Native");
}

CCommOPCSearch::~CCommOPCSearch()
{
}

void CCommOPCSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG_COM, m_EditLog);
	DDX_Control(pDX, IDC_PROGRESS_LOAD, m_pLoadOPCPoint);
	DDX_Control(pDX, IDC_STATIC_OPC, m_sLoadOPC);
}

BEGIN_MESSAGE_MAP(CCommOPCSearch, CDialog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_OPC_ROOT,SearchOPC)
	ON_COMMAND(ID_MENU_OPC_SETTING,OPCSetting)
	ON_COMMAND(ID_MENU_OPC_CONNECT,OPCConnect)
	ON_COMMAND(ID_MENU_OPC_SEARCH_POINT,ReadAllOPCPoint)
	ON_COMMAND(ID_MENU_OPC_ADD_NEW_POINT,OPCAddNewPoint)
	ON_COMMAND(ID_MENU_OPC_EXPORT_OPC,OPCExportOPC)
	ON_COMMAND(ID_MENU_OPC_POINT,ReadPointProperty)
	ON_COMMAND(ID_MENU_OPC_DISCONNECT,DisConnOPC)
	ON_COMMAND(ID_MENU_OPC_FIND_POINT,FindOPCPoint)
	ON_COMMAND(ID_MENU_OPC_SHOW_POINT,ShowOPCPoint)
END_MESSAGE_MAP()

BOOL CCommOPCSearch::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

bool CCommOPCSearch::InitData( bool bConnected /*= true*/ )
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, IDC_TREE))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	CRect rectClient;
	GetClientRect(rectClient);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 20, rectClient.top+30, 250, rectClient.Height() - 50, SWP_NOACTIVATE | SWP_NOZORDER);

	OPCInfo* opcTreeNode=new OPCInfo;
	opcTreeNode->type = 0;
	HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T("OPC搜索"),0,0,0,0,reinterpret_cast<DWORD>(opcTreeNode),TVI_ROOT,NULL);

	if(Project::Tools::GetDebugLanguage() == 1)
		EnableControl(bConnected);
	return true;
}

bool CCommOPCSearch::Exit()
{
	m_bLoading = false;
	m_mapAllPoint.clear();
	hash_map<wstring,OPCInfo*>::iterator iter = m_mapDeveice.begin();
	while(iter != m_mapDeveice.end())		//未找到显示
	{
		OPCInfo* info = (*iter).second;
		if(info != NULL)
		{
			if(info->type == 1)
			{
				if(info->pOPC)
				{
					if(info->bConnected)
					{
						info->pOPC->Exit();
					}
				}
			}

			delete info;
			info = NULL;
		}
		iter = m_mapDeveice.erase(iter);
	}
	return true;
}

bool CCommOPCSearch::EnableControl( bool bConnected /*= true*/ )
{
	

	return true;
}

void CCommOPCSearch::OnContextMenu( CWnd* pWnd, CPoint point )
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);

		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);

			OPCInfo *temStru=reinterpret_cast<OPCInfo *>(pWndTree->GetItemData(hTreeItem));
			if(temStru == NULL)
				return;

			// 创建自定义菜单
			CMenu *pMenu = new CMenu();
			VERIFY(pMenu->CreatePopupMenu());

			if(temStru->type==0)		//根节点
			{
				pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_ROOT,_T("OPC搜索"));
				pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_SETTING,_T("OPC设置"));
			}
			else if(temStru->type==1)  //opc设备
			{			
				if(!temStru->bConnected)
				{
					pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_CONNECT,_T("OPC连接"));
				}
				else
				{
					pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_DISCONNECT,_T("OPC断开"));
					if(!temStru->bLoadPointed)
					{
						pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_SEARCH_POINT,_T("OPC搜索点"));
						pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_ADD_NEW_POINT,_T("OPC添加新的点"));
					}
					else
					{
						pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_FIND_POINT,_T("OPC查找点"));
						if(!temStru->bShow)
							pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_SHOW_POINT,_T("OPC显示点"));
					}
					pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_EXPORT_OPC,_T("OPC导出点"));
				}
			}
			else				//opc 点
			{
				pMenu->AppendMenu(MF_STRING,ID_MENU_OPC_POINT,_T("读点属性"));
			}

			pMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
			pMenu->DestroyMenu();

			//禁用菜单
			/*GetDlgItem(ID_MENU_OPC_CONNECT)->EnableWindow(FALSE);
			GetDlgItem(ID_MENU_OPC_SEARCH_POINT)->EnableWindow(FALSE);
			GetDlgItem(ID_MENU_OPC_ADD_NEW_POINT)->EnableWindow(FALSE);
			GetDlgItem(ID_MENU_OPC_EXPORT_OPC)->EnableWindow(FALSE);
			GetDlgItem(ID_MENU_OPC_POINT)->EnableWindow(FALSE);*/
			
			if(pMenu!=NULL)
			{
				delete pMenu;
				pMenu=NULL;
			}		
		}
	}

	pWndTree->SetFocus();
}

void CCommOPCSearch::SearchOPC()
{
	AddLog(_T("搜索OPC(进行中)..."));
	Exit();
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode,false);
	COPCConnection_AutoBrowse* pOPC = new COPCConnection_AutoBrowse();
	if(pOPC)
	{
		if(pOPC->SearchOPCServer(m_strOPCIP.GetString(),m_clsidmap))
		{
			RefreshOPCServer(m_clsidmap);
		}
		
		delete pOPC;
		pOPC = NULL;
	}
	AddLog(_T("搜索OPC(成功)..."));
}

void CCommOPCSearch::OPCSetting()
{
	COPCSetting dlg;
	dlg.m_strOPCServerIP = m_strOPCIP;
	dlg.m_strOPCFiler = m_strOPCFiler;
	dlg.m_nBrowseType = m_nBrowseType;
	dlg.m_nOPCMultiLoad = m_nMutilLoad;
	dlg.m_nLoadInterval = m_nLoadInterval;
	dlg.m_bReadDescription = m_bReadDescription;
	dlg.m_strFilterType = m_strDataTypeFilter;
	dlg.m_nMultiAdd = m_nMutilAddItem;
	if(dlg.DoModal() == IDOK)
	{
		m_strOPCFiler = dlg.m_strOPCFiler;
		m_nBrowseType = dlg.m_nBrowseType;
		m_strOPCIP = dlg.m_strOPCServerIP;
		m_nLoadInterval = dlg.m_nLoadInterval;
		m_nMutilLoad = dlg.m_nOPCMultiLoad;
		m_bReadDescription = dlg.m_bReadDescription;
		m_nMutilAddItem = dlg.m_nMultiAdd;
		m_strDataTypeFilter = dlg.m_strFilterType;
	}
}

void CCommOPCSearch::OPCExportOPC()
{
	CString strOut,strTag;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 1)
		{
			if(temStru->bLoadPointed)
			{
				if(temStru->pOPC)
				{
					strOut.Format(_T("Export OPC:%s(ing).."),temStru->strOPCServerName);				
					AddLog(strOut);

					map<wstring, CKItem*> searchmap;
					if(temStru->pOPC->GetAllOPCMap(searchmap))
					{
						temStru->nPointCount = searchmap.size();
						strTag.Format(_T("%s(%d)"),temStru->strOPCServerName,temStru->nPointCount);
						m_wndFileView.SetItemText(m_hSelectNode,strTag);

						//存CSV
						char* old_locale=_strdup( setlocale(LC_CTYPE,NULL) ); 
						setlocale( LC_CTYPE,"chs");
						CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|"), NULL);
						if (dlg.DoModal() == IDOK)
						{
							CString strFilePath = dlg.GetPathName();
							CString strName = dlg.GetFileName();
							if (strName.GetLength() == 0)
							{
								AfxMessageBox(_T("文件名称不能为空"));

								strOut.Format(_T("导出OPC:%s(失败)..."),temStru->strOPCServerName);				
								AddLog(strOut);
								return;
							}

							CStdioFile	ff;
							if(ff.Open(strFilePath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
							{
								map<wstring, CKItem*>::iterator iter = searchmap.begin();
								while(iter != searchmap.end())
								{
									CKItem* pItem = (*iter).second;
									CString str,strType,strValue;
									CString strItemDescription = pItem->GetItemDescription();
									StringFromVartype (pItem->GetDataType (), strType);
									pItem->GetValue(strValue);
									str.Format(_T(",%s,%s,%s"),strType,strValue,strItemDescription);
									ff.Seek(0,CFile::end);
									string strContent = Project::Tools::WideCharToAnsi(str);
									string strName = Project::Tools::WideCharToAnsi(pItem->GetItemID());
									//AddLog(Project::Tools::AnsiToWideChar(strName.c_str()).c_str());
									ff.Write(strName.c_str(),strName.length());
									ff.Flush();
									ff.Write(strContent.c_str(),strContent.length());
									ff.WriteString(_T("\n"));
									++iter;
								}
								ff.Close();

								strOut.Format(_T("导出OPC:%s(成功).."),temStru->strOPCServerName);				
								AddLog(strOut);
							}
							else
							{
								strOut.Format(_T("导出OPC:%s(失败)..."),temStru->strOPCServerName);				
								AddLog(strOut);
							}
						}
						setlocale( LC_ALL, old_locale );
						free(old_locale);
					}
				}
			}
			else
			{
				if(temStru->bConnected)
				{
					if(temStru->pOPC)
					{
						CString strOut,strTag;
						strOut.Format(_T("Export OPC:%s(ing).."),temStru->strOPCServerName);				
						AddLog(strOut);

						m_pLoadingOPC = temStru->pOPC;
						m_bLoading = true;
						if(temStru->pOPC->AddItems(m_bReadDescription,m_nBrowseType+1,m_strOPCFiler.GetString(),m_nMutilLoad,m_nLoadInterval,VartypeFromString(m_strDataTypeFilter),m_nMutilAddItem,OnAddPoint,(DWORD)this))
						{
							AddLog(_T("AddItems Success!"));
							temStru->bLoadPointed = true;
							map<wstring, CKItem*> searchmap;
							if(temStru->pOPC->GetAllOPCMap(searchmap))
							{
								temStru->nPointCount = searchmap.size();
								strTag.Format(_T("%s(%d)"),temStru->strOPCServerName,temStru->nPointCount);
								m_wndFileView.SetItemText(m_hSelectNode,strTag);

								//存CSV
								char* old_locale=_strdup( setlocale(LC_CTYPE,NULL) ); 
								setlocale( LC_CTYPE,"chs");
								CFileDialog dlg(FALSE, _T(""), _T(""), 0, _T("CSV File (*.csv)|*.csv|"), NULL);
								if (dlg.DoModal() == IDOK)
								{
									CString strFilePath = dlg.GetPathName();
									CString strName = dlg.GetFileName();
									if (strName.GetLength() == 0)
									{
										AfxMessageBox(_T("文件名称不能为空"));
										strOut.Format(_T("导出OPC:%s(失败)..."),temStru->strOPCServerName);				
										AddLog(strOut);
										return;
									}

									CStdioFile	ff;
									if(ff.Open(strFilePath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
									{
										map<wstring, CKItem*>::iterator iter = searchmap.begin();
										while(iter != searchmap.end())
										{
											CKItem* pItem = (*iter).second;
											CString str,strType,strValue;
											CString strItemDescription = pItem->GetItemDescription();
											StringFromVartype (pItem->GetDataType (), strType);
											pItem->GetValue(strValue);
											str.Format(_T(",%s,%s,%s"),strType,strValue,strItemDescription);
											ff.Seek(0,CFile::end);
											string strContent = Project::Tools::WideCharToAnsi(str);
											string strName = Project::Tools::WideCharToAnsi(pItem->GetItemID());
											//AddLog(Project::Tools::AnsiToWideChar(strName.c_str()).c_str());
											ff.Write(strName.c_str(),strName.length());
											ff.Flush();
											ff.Write(strContent.c_str(),strContent.length());
											ff.WriteString(_T("\n"));
											++iter;
										}
										ff.Close();
										strOut.Format(_T("导出OPC:%s(成功).."),temStru->strOPCServerName);				
										AddLog(strOut);
									}
									else
									{
										strOut.Format(_T("导出OPC:%s(失败)..."),temStru->strOPCServerName);				
										AddLog(strOut);
									}
								}
								setlocale( LC_ALL, old_locale );
								free(old_locale);
							}
						}
						else
						{		
							AddLog(_T("AddItems Fail!"));
							temStru->bLoadPointed = false;
						}
						m_bLoading = false;
					}
				}
			}
		}
		strOut.Format(_T("导出OPC:%s(结束)..."),temStru->strOPCServerName);				
		AddLog(strOut);
	}
}

void CCommOPCSearch::ReadAllOPCPoint()
{
	CString strOut,strTag;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 1 && temStru->bConnected)
		{
			if(!temStru->bShow)
			{
				strOut.Format(_T("加载OPC:%s(ing)..."),temStru->strOPCServerName);				
				AddLog(strOut);

				if(temStru->bLoadPointed)
				{
					if(temStru->pOPC)
					{
						map<wstring, CKItem*> searchmap;
						if(temStru->pOPC->GetAllOPCMap(searchmap))
						{
							map<wstring, CKItem*>::iterator iter = searchmap.begin();
							temStru->nPointCount = searchmap.size();
							strTag.Format(_T("%s(%d)"),temStru->strOPCServerName,temStru->nPointCount);
							m_wndFileView.SetItemText(m_hSelectNode,strTag);
							while(iter != searchmap.end())
							{
								CKItem* pItem = (*iter).second;
								if(pItem)
								{
									OPCInfo* opcTreeNode = new OPCInfo;
									opcTreeNode->type = 2;
									CString strType;
									CString strItemID = pItem->GetItemID();
									CString strItemDescription = pItem->GetItemDescription();
									StringFromVartype (pItem->GetDataType (), strType);
									opcTreeNode->strOPCPointName = strItemID;
									opcTreeNode->strOPCPointType = strType;
									opcTreeNode->strOPCPointRemark = strItemDescription;
									opcTreeNode->strOPCServerIP = temStru->strOPCServerIP;
									opcTreeNode->pOPC = temStru->pOPC;
									opcTreeNode->bConnected = temStru->bConnected;
									opcTreeNode->bLoadPointed = temStru->bLoadPointed;
									pItem->GetValue(opcTreeNode->strOPCPointValue);

									HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strItemID,2,2,0,0,reinterpret_cast<DWORD>(opcTreeNode),m_hSelectNode,TVI_LAST);
									m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);

									m_mapAllPoint[temStru->strOPCServerName.GetString()][strItemID.GetString()] = hRoot;
								}
								++iter;
							}
							temStru->bShow = true;
						}
					}
					else
					{
						strTag.Format(_T("%s"),temStru->strOPCServerName);
						m_wndFileView.SetItemText(m_hSelectNode,strTag);
					}
				}
				else
				{
					if(temStru->pOPC)
					{
						m_pLoadingOPC = temStru->pOPC;
						m_bLoading = true;
						if(temStru->pOPC->AddItems(m_bReadDescription,m_nBrowseType+1,m_strOPCFiler.GetString(),m_nMutilLoad,m_nLoadInterval,VartypeFromString(m_strDataTypeFilter),m_nMutilAddItem,OnAddPoint,(DWORD)this))
						{
							temStru->bLoadPointed = true;
							map<wstring, CKItem*> searchmap;
							if(temStru->pOPC->GetAllOPCMap(searchmap))
							{
								map<wstring, CKItem*>::iterator iter = searchmap.begin();
								temStru->nPointCount = searchmap.size();
								CString strTag;
								strTag.Format(_T("%s(%d)"),temStru->strOPCServerName,temStru->nPointCount);
								m_wndFileView.SetItemText(m_hSelectNode,strTag);
								while(iter != searchmap.end())
								{
									CKItem* pItem = (*iter).second;
									if(pItem)
									{
										OPCInfo* opcTreeNode = new OPCInfo;
										opcTreeNode->type = 2;
										CString strType;
										CString strItemID = pItem->GetItemID();
										CString strItemDescription = pItem->GetItemDescription();
										StringFromVartype (pItem->GetDataType (), strType);
										opcTreeNode->strOPCPointName = strItemID;
										opcTreeNode->strOPCPointType = strType;
										opcTreeNode->strOPCPointRemark = strItemDescription;
										opcTreeNode->strOPCServerIP = temStru->strOPCServerIP;
										opcTreeNode->pOPC = temStru->pOPC;
										opcTreeNode->bConnected = temStru->bConnected;
										opcTreeNode->bLoadPointed = temStru->bLoadPointed;
										pItem->GetValue(opcTreeNode->strOPCPointValue);

										HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strItemID,2,2,0,0,reinterpret_cast<DWORD>(opcTreeNode),m_hSelectNode,TVI_LAST);
										m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);

										m_mapAllPoint[temStru->strOPCServerName.GetString()][strItemID.GetString()] = hRoot;
									}
									++iter;
								}
								temStru->bShow = true;
							}
						}
						else
						{
							temStru->bLoadPointed = false;
						}
						m_bLoading = false;
					}
					else
					{
						strTag.Format(_T("%s"),temStru->strOPCServerName);
						m_wndFileView.SetItemText(m_hSelectNode,strTag);
					}
				}
			}
			else
			{

			}
		}
		else
		{
			MessageBox(_T("请先连接OPC服务器"));
		}
		strOut.Format(_T("Loading OPC:%s(finish).."),temStru->strOPCServerName);				
		AddLog(strOut);
	}
}

void CCommOPCSearch::ReadPointProperty()
{
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	DeleteNode(m_hSelectNode);
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 2)
		{
			if(temStru->pOPC)
			{
				if(temStru->pOPC->ReadItemProperty(temStru->strOPCPointName.GetString()))
				{
					CKItem* pItem = temStru->pOPC->FindItem(temStru->strOPCPointName.GetString());
					if(pItem)
					{
						OPCInfo* opcTreeNode = new OPCInfo;
						opcTreeNode->type = 3;

						CString strType,strType_,strOPCPointValue,strOPCPointValue_,strItemDescription;
						strItemDescription.Format(_T("备注:%s"),pItem->GetItemDescription());					
						StringFromVartype (pItem->GetDataType (), strType);
						strType_.Format(_T("类型:%s"),strType);
						pItem->GetValue(strOPCPointValue);
						strOPCPointValue_.Format(_T("数值:%s"),strOPCPointValue);
						//值
						HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strOPCPointValue_,3,3,0,0,0,m_hSelectNode,TVI_LAST);
						m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);

						//类型
						hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strType_,3,3,0,0,0,m_hSelectNode,TVI_LAST);
						m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);

						//注释
						hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strItemDescription,3,3,0,0,0,m_hSelectNode,TVI_LAST);
						m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
					}
				}
				else
				{
					MessageBox(_T("OPC服务器读取点失败"));
				}
			}
		}
		else
		{
			MessageBox(_T("请先连接OPC服务器"));
		}
	}
}

bool CCommOPCSearch::DeleteNode( HTREEITEM hItem,bool bReleaseNode/*=true*/ )
{
	HTREEITEM hChild = m_wndFileView.GetChildItem(hItem);
	vector<HTREEITEM> vecDelete;
	while (hChild)
	{
		OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(hChild));
		if(temStru != NULL)
		{
			if(temStru->type == 1)
			{
				if(bReleaseNode)
				{
					hash_map<wstring,OPCInfo*>::iterator iter = m_mapDeveice.find(temStru->strOPCServerName.GetString());
					if(iter != m_mapDeveice.end())		//未找到显示
					{
						m_mapDeveice.erase(iter);
					}
				}
			}
			if(bReleaseNode)
			{
				delete temStru;
				temStru = NULL;
			}
		}
		vecDelete.push_back(hChild);
		hChild = m_wndFileView.GetNextItem(hChild, TVGN_NEXT);
	}
	for (size_t i=0;i<vecDelete.size();++i)
	{
		m_wndFileView.DeleteItem(vecDelete[i]);
	}

	return true;
}

bool CCommOPCSearch::RefreshOPCServer( hash_map<wstring,CLSID> clsidmap )
{
	hash_map<wstring,CLSID>::iterator iter = clsidmap.begin();
	while(iter != clsidmap.end())
	{
		wstring strOPCName = (*iter).first;
		hash_map<wstring,OPCInfo*>::iterator iter1 = m_mapDeveice.find(strOPCName);
		if(iter1 == m_mapDeveice.end())		//未找到显示
		{
			OPCInfo* opcTreeNode=new OPCInfo;
			opcTreeNode->type = 1;
			opcTreeNode->strOPCServerIP = m_strOPCIP;
			opcTreeNode->strOPCServerName = strOPCName.c_str();
			HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strOPCName.c_str(),1,1,0,0,reinterpret_cast<DWORD>(opcTreeNode),m_hSelectNode,TVI_LAST);
			m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);
			m_mapDeveice[strOPCName] = opcTreeNode;
		}
		++iter;
	}
	return true;
}

void CCommOPCSearch::OPCConnect()
{
	CString strOut,strTag;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		strOut.Format(_T("连接OPC:%s(进行中).."),temStru->strOPCServerName);				
		AddLog(strOut);
		if(temStru->type == 1)
		{
			if(temStru->bConnected)
			{
				MessageBox(_T("OPCServer Connect OK"));
				strOut.Format(_T("Connect OPC:%s(ok).."),temStru->strOPCServerName);				
				AddLog(strOut);
			}
			else
			{
				CString strTag;
				if(temStru->pOPC)
				{
					temStru->pOPC->DisConnect();
					delete temStru->pOPC;
					temStru->pOPC = NULL;
				}

				if(temStru->pOPC == NULL)
				{
					temStru->pOPC = new COPCConnection_ManualBrowse();
				}

				if(temStru->pOPC)
				{
					temStru->pOPC->SetServerProgName(temStru->strOPCServerName.GetString());
					if(temStru->strOPCServerName.Find(_T("TACOS.OPCServices"))>=0)
					{
						m_nMutilAddItem = 10;
					}

					hash_map<wstring,CLSID>::iterator iter =  m_clsidmap.find(temStru->strOPCServerName.GetString());
					if(iter != m_clsidmap.end())
					{
						if(!temStru->pOPC->ConnectRemoteOPC(temStru->strOPCServerIP.GetString(),(*iter).second))
						{
							MessageBox(_T("OPCServer Connect Fail"));
							temStru->bConnected = false;
							strOut.Format(_T("Connect OPC:%s(fail).."),temStru->strOPCServerName);				
							AddLog(strOut);
						}
						else
						{
							temStru->bConnected = true;
							temStru->pOPC->AddGroup();
							MessageBox(_T("OPCServer Connect OK"));
							strOut.Format(_T("Connect OPC:%s(ok).."),temStru->strOPCServerName);				
							AddLog(strOut);
						}
						strTag.Format(_T("%s"),temStru->strOPCServerName);
						m_wndFileView.SetItemText(m_hSelectNode,strTag);
					}
				}
			}
		}
		strOut.Format(_T("连接OPC:%s(结束).."),temStru->strOPCServerName);				
		AddLog(strOut);
	}
}

void CCommOPCSearch::OPCAddNewPoint()
{
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 1)
		{
			if(temStru->pOPC)
			{
				if(temStru->bConnected)
				{
					CAddOPCItem dlg;
					if(dlg.DoModal() == IDOK)
					{
						wstring strItemID = dlg.m_strOPCItemName.GetString();
						if(strItemID.length() <= 0)
							return;

						if(temStru->bLoadPointed)		//定位到该点
						{
							hash_map<wstring,hash_map<wstring,HTREEITEM>>::iterator iter = m_mapAllPoint.find(temStru->strOPCServerName.GetString());
							if(iter != m_mapAllPoint.end())
							{
								hash_map<wstring,HTREEITEM> mapItem = (*iter).second;
								hash_map<wstring,HTREEITEM>::iterator iter1 = mapItem.find(strItemID);
								if(iter1 != mapItem.end())
								{
									HTREEITEM hSelect = (*iter1).second;
									m_wndFileView.SetFocus();
									m_wndFileView.SelectItem(hSelect);
								}
							}
						}
						else	//添加
						{
							bool bExist = false;
							hash_map<wstring,hash_map<wstring,HTREEITEM>>::iterator iter = m_mapAllPoint.find(temStru->strOPCServerName.GetString());
							if(iter != m_mapAllPoint.end())
							{
								hash_map<wstring,HTREEITEM> mapItem = (*iter).second;
								hash_map<wstring,HTREEITEM>::iterator iter1 = mapItem.find(strItemID);
								if(iter1 != mapItem.end())
								{
									bExist = true;
									HTREEITEM hSelect = (*iter1).second;
									m_wndFileView.SetFocus();
									m_wndFileView.SelectItem(hSelect);
								}
							}

							if(!bExist)
							{
								if(temStru->pOPC->AddItem(strItemID))
								{
									CKItem* pItem = temStru->pOPC->FindItem(strItemID);
									if(pItem)
									{
										OPCInfo* opcTreeNode = new OPCInfo;
										opcTreeNode->type = 2;
										CString strType;
										CString strItemID = pItem->GetItemID();
										CString strItemDescription = pItem->GetItemDescription();
										StringFromVartype (pItem->GetDataType (), strType);
										opcTreeNode->strOPCPointName = strItemID;
										opcTreeNode->strOPCPointType = strType;
										opcTreeNode->strOPCPointRemark = strItemDescription;
										opcTreeNode->strOPCServerIP = temStru->strOPCServerIP;
										opcTreeNode->pOPC = temStru->pOPC;
										opcTreeNode->bConnected = temStru->bConnected;
										opcTreeNode->bLoadPointed = temStru->bLoadPointed;
										pItem->GetValue(opcTreeNode->strOPCPointValue);

										HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strItemID,2,2,0,0,reinterpret_cast<DWORD>(opcTreeNode),m_hSelectNode,TVI_LAST);
										m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);

										m_mapAllPoint[temStru->strOPCServerName.GetString()][strItemID.GetString()] = hRoot;
									}
									else
									{
										MessageBox(_T("OPC点位无效"));
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CCommOPCSearch::DisConnOPC()
{
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 1)
		{
			if(temStru->pOPC)
			{
				if(temStru->bConnected)
				{
					temStru->pOPC->Exit();			
					temStru->bConnected = false;
					temStru->bShow = false;
					if(temStru->bLoadPointed)
					{
						DeleteNode(m_hSelectNode);
						temStru->bLoadPointed = false;
						temStru->nPointCount = 0;
						CString strTag;
						strTag.Format(_T("%s"),temStru->strOPCServerName);
						m_wndFileView.SetItemText(m_hSelectNode,strTag);
						hash_map<wstring,hash_map<wstring,HTREEITEM>>::iterator iter = m_mapAllPoint.find(temStru->strOPCServerName.GetString());
						if(iter != m_mapAllPoint.end())
						{
							m_mapAllPoint.erase(iter);
						}
					}
				}
			}
		}
	}
}

void CCommOPCSearch::FindOPCPoint()
{
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		if(temStru->type == 1)
		{
			if(temStru->pOPC)
			{
				if(temStru->bConnected)
				{
					CAddOPCItem dlg;
					if(dlg.DoModal() == IDOK)
					{
						wstring strItemID = dlg.m_strOPCItemName.GetString();
						if(strItemID.length() <= 0)
							return;

						if(temStru->bLoadPointed)		//定位到该点
						{
							hash_map<wstring,hash_map<wstring,HTREEITEM>>::iterator iter = m_mapAllPoint.find(temStru->strOPCServerName.GetString());
							if(iter != m_mapAllPoint.end())
							{
								hash_map<wstring,HTREEITEM> mapItem = (*iter).second;
								hash_map<wstring,HTREEITEM>::iterator iter1 = mapItem.find(strItemID);
								if(iter1 != mapItem.end())
								{
									HTREEITEM hSelect = (*iter1).second;
									m_wndFileView.SetFocus();
									m_wndFileView.SelectItem(hSelect);
								}
							}
						}
					}
				}
			}
		}
	}
}

void CCommOPCSearch::ShowOPCPoint()
{
	CString strOut,strTag;
	m_hSelectNode = m_wndFileView.GetSelectedItem();
	OPCInfo *temStru=reinterpret_cast<OPCInfo *>(m_wndFileView.GetItemData(m_hSelectNode));
	if(temStru != NULL)
	{
		strOut.Format(_T("显示OPC点:%s(进行中).."),temStru->strOPCServerName);				
		AddLog(strOut);
		if(temStru->type == 1 && !temStru->bShow)
		{
			CString strTag;
			if(temStru->bLoadPointed)
			{
				if(temStru->pOPC)
				{
					map<wstring, CKItem*> searchmap;
					if(temStru->pOPC->GetAllOPCMap(searchmap))
					{
						map<wstring, CKItem*>::iterator iter = searchmap.begin();
						temStru->nPointCount = searchmap.size();
						strTag.Format(_T("%s(%d)"),temStru->strOPCServerName,temStru->nPointCount);
						m_wndFileView.SetItemText(m_hSelectNode,strTag);
						while(iter != searchmap.end())
						{
							CKItem* pItem = (*iter).second;
							if(pItem)
							{
								OPCInfo* opcTreeNode = new OPCInfo;
								opcTreeNode->type = 2;
								CString strType;
								CString strItemID = pItem->GetItemID();
								CString strItemDescription = pItem->GetItemDescription();
								StringFromVartype (pItem->GetDataType (), strType);
								opcTreeNode->strOPCPointName = strItemID;
								opcTreeNode->strOPCPointType = strType;
								opcTreeNode->strOPCPointRemark = strItemDescription;
								opcTreeNode->strOPCServerIP = temStru->strOPCServerIP;
								opcTreeNode->pOPC = temStru->pOPC;
								opcTreeNode->bConnected = temStru->bConnected;
								opcTreeNode->bLoadPointed = temStru->bLoadPointed;
								pItem->GetValue(opcTreeNode->strOPCPointValue);

								HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,strItemID,2,2,0,0,reinterpret_cast<DWORD>(opcTreeNode),m_hSelectNode,TVI_LAST);
								m_wndFileView.Expand(m_hSelectNode,TVE_EXPAND);

								m_mapAllPoint[temStru->strOPCServerName.GetString()][strItemID.GetString()] = hRoot;
							}
							++iter;
						}
						temStru->bShow = true;
					}
				}
				else
				{
					strTag.Format(_T("%s"),temStru->strOPCServerName);
					m_wndFileView.SetItemText(m_hSelectNode,strTag);
				}
			}
		}
		strOut.Format(_T("显示OPC点:%s(结束).."),temStru->strOPCServerName);				
		AddLog(strOut);
	}
}

bool CCommOPCSearch::AddLog( CString strLog )
{
	m_strLOG+= strLog;
	m_strLOG+= L"\r\n";
	m_EditLog.SetWindowTextW(m_strLOG);
	m_EditLog.LineScroll(m_EditLog.GetLineCount()-1, 0);
	return true;
}

bool CCommOPCSearch::UpdateProgress( DWORD dwCurrent, DWORD dwTotal ,char* pItemName)
{
	Project::Tools::Scoped_Lock<Mutex>	scopelock(m_lockProgress);
	if(pItemName== NULL)
	{
		if(dwCurrent>0)
		{
			CString strLoadInfo;
			strLoadInfo.Format(_T("Loading OPC Point Infomation(%d/%d)"),dwCurrent,dwTotal);
			dwTotal = (dwTotal<=0)?1:dwTotal;
			int nLoadPos = dwCurrent*100/dwTotal;
			m_pLoadOPCPoint.SetPos(nLoadPos);
			m_sLoadOPC.SetWindowText(strLoadInfo);
			return true;
		}
	}
	else
	{
		if(dwCurrent>0)
		{
			CString strLoadInfo;
			strLoadInfo.Format(_T("Add OPC Point(%d/%d):%s"),dwCurrent,dwTotal,Project::Tools::AnsiToWideChar(pItemName).c_str());
			dwTotal = (dwTotal<=0)?1:dwTotal;
			int nLoadPos = dwCurrent*100/dwTotal;
			m_pLoadOPCPoint.SetPos(nLoadPos);
			m_sLoadOPC.SetWindowText(strLoadInfo);
			return true;
		}
	}

	return false;
}

LRESULT CCommOPCSearch::OnAddPoint( DWORD dwCurrent, DWORD dwTotal,DWORD userdata ,char* pItemName)
{
	CCommOPCSearch *pOPCCtrl=(CCommOPCSearch *)userdata;
	if(pOPCCtrl != NULL)
	{
		pOPCCtrl->UpdateProgress(dwCurrent,dwTotal,pItemName);
	}
	return 0;
}

// CCommOPCSearch message handlers
