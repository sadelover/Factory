// CheckMissImage.cpp : 实现文件
//

#include "stdafx.h"
#include "MainFrm.h"
#include "CheckMissImageDlg.h"
#include "afxdialogex.h"

// CCheckMissImageDlg 对话框

IMPLEMENT_DYNAMIC(CCheckMissImageDlg, CDialogEx)

CCheckMissImageDlg::CCheckMissImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckMissImageDlg::IDD, pParent)
	, m_pMainFrame(NULL)
{

}

CCheckMissImageDlg::~CCheckMissImageDlg()
{
}

void CCheckMissImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_INFO, m_listCtrl);
	DDX_Control(pDX, IDC_GRID_INFO, m_gridCtrl);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
}


BEGIN_MESSAGE_MAP(CCheckMissImageDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CCheckMissImageDlg::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// CCheckMissImageDlg 消息处理程序


BOOL CCheckMissImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if(m_pMainFrame == NULL)
		return FALSE;

	m_gridCtrl.SetRowCount(1);
	m_gridCtrl.SetColumnCount(6);//设置列数量
	m_gridCtrl.SetFixedRowCount(1);
	m_gridCtrl.SetEditable(true);//表格内容不可编辑设置
	m_gridCtrl.SetItemText(0, 0, _T("分组"));//设置内容
	m_gridCtrl.SetItemText(0, 1, _T("页面"));//设置内容
	m_gridCtrl.SetItemText(0, 2, _T("页面ID"));//设置内容
	m_gridCtrl.SetItemText(0, 3, _T("元素ID"));//设置内容
	m_gridCtrl.SetItemText(0, 4, _T("图片ID"));//设置内容
	m_gridCtrl.SetItemText(0, 5, _T("绑定图片ID"));//设置内容

	m_progressCtrl.SetRange(0,100);

	AfxBeginThread(CCheckMissImageDlg::Thread_Check, (LPVOID)this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCheckMissImageDlg::InitPageInfo(void)
{
	//获取所有的页面的ID和名字 分组Id和名字。
	vector<CEqmDrawPage*>& PageList = m_pMainFrame->m_project.GetPageList();
	for(vector<CEqmDrawPage*>::iterator it = PageList.begin()
		; it!=PageList.end()
		; it++)
	{
		CString csGroupName(m_pMainFrame->m_project.GetGroupNameByGroupId((*it)->GetGroupID()).c_str());
		CString csPageName((*it)->GetPageName().c_str());
		m_pageInfoList.push_back(CheckDlg_PageInfo(csGroupName, (*it)->GetGroupID(), csPageName, (*it)->GetID()));
	}
}

void CCheckMissImageDlg::InitDeviceInfo(void)
{
	m_pMainFrame->m_pPrjSqlite->GetDeviceInfo_Check(&m_deviceInfoList);
}

void CCheckMissImageDlg::InitPictureList(void)
{
	m_pMainFrame->m_pPrjSqlite->GetPictureId_Check(&m_pictureList);
}

//分组  页面 图片ID 绑定图ID
void CCheckMissImageDlg::CheckMissingImage(void)
{
	//获取所有的页面的ID和名字 分组Id和名字。
	InitPageInfo();
	m_progressCtrl.SetPos(10);
	//获取所有device id 和device 绑定的图片资源id
	InitDeviceInfo();
	m_progressCtrl.SetPos(20);
	//获取所有的图片资源ID
	InitPictureList();
	m_progressCtrl.SetPos(30);
#if 0
	for(auto it = m_deviceInfoList.begin(); it!= m_deviceInfoList.end(); it++)
	{
		//device图
		if(it->m_nPicInfo.bPicOrAnmt == true)
		{
			if(FindPictureId(it->m_nPicInfo.bindID) == false)
			{
				CString csGroupName = L"";
				CString csPageName = L"";
				GetPageAndGroupNameForPageId(it->m_nPageId, csPageName, csGroupName);
				if(csGroupName.GetLength() == 0 || csPageName.GetLength() == 0)
				{
					csPageName = L"页面不存在";
				}
				m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID));
			}
		}
		else
		{
			 if(false == m_pMainFrame->m_pPrjSqlite->IsAnimationExist(it->m_nPicInfo.bindID, true))
			 {
				 CString csGroupName = L"";
				 CString csPageName = L"";
				 GetPageAndGroupNameForPageId(it->m_nPageId, csPageName, csGroupName);
				 if(csGroupName.GetLength() == 0 || csPageName.GetLength() == 0)
				 {
					 csPageName = L"页面不存在";
				 }
				 m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID));
			 }
		}

		//绑定图
		for(auto it_bind = it->m_bindList.begin(); it_bind != it->m_bindList.end(); it_bind++)
		{
			if(it_bind->bPicOrAnmt)
			{
				if(FindPictureId((*it_bind).bindID) == false)
				{
					CString csGroupName = L"";
					CString csPageName = L"";
					GetPageAndGroupNameForPageId(it->m_nPageId, csPageName, csGroupName);
					if(csGroupName.GetLength() == 0 || csPageName.GetLength() == 0)
					{
						csPageName = L"页面不存在";
					}
					m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID, it_bind->bindID));
				}
			}
			else
			{
				if(false == m_pMainFrame->m_pPrjSqlite->IsAnimationExist((*it_bind).bindID, false))
				{
					CString csGroupName = L"";
					CString csPageName = L"";
					GetPageAndGroupNameForPageId(it->m_nPageId, csPageName, csGroupName);
					if(csGroupName.GetLength() == 0 || csPageName.GetLength() == 0)
					{
						csPageName = L"页面不存在";
					}
					m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID, it_bind->bindID));
				}
			}
		}
	}
#else
	//device 存在一种情况：所在页面已不存在但是他的信息和所显示的图片资源还存在
	for(auto it = m_deviceInfoList.begin(); it!= m_deviceInfoList.end(); it++)
	{
		CString csGroupName = L"";
		CString csPageName = L"";
		GetPageAndGroupNameForPageId(it->m_nPageId, csPageName, csGroupName);
		if(csGroupName.GetLength() == 0 || csPageName.GetLength() == 0)
		{
			csGroupName = L"          ";
			csPageName  = L"页面不存在";
			m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID));
			continue;
		}

		//device图
		if(it->m_nPicInfo.bPicOrAnmt == true)
		{
			if(FindPictureId(it->m_nPicInfo.bindID) == false)
			{	
				m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID));
			}
		}
		else
		{
			if(false == m_pMainFrame->m_pPrjSqlite->IsAnimationExist(it->m_nPicInfo.bindID, true))
			{
				m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID));
			}
		}

		//绑定图
		for(auto it_bind = it->m_bindList.begin(); it_bind != it->m_bindList.end(); it_bind++)
		{
			if(it_bind->bPicOrAnmt)
			{
				if(FindPictureId((*it_bind).bindID) == false)
				{
					m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID, it_bind->bindID));
				}
			}
			else
			{
				if(false == m_pMainFrame->m_pPrjSqlite->IsAnimationExist((*it_bind).bindID, false))
				{
					m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID, it_bind->bindID));
				}
			}
		}
	}
	m_progressCtrl.SetPos(70);
#endif
	m_gridCtrl.SetRedraw(FALSE);//禁止重绘画面
	m_gridCtrl.SetRowCount(m_MissImageInfoList.size()+1);
	m_gridCtrl.SetRedraw(TRUE);
	int i=1;
	for(auto it = m_MissImageInfoList.begin();it!=m_MissImageInfoList.end(); it++)
	{
		InsertItemListCrtl(&(*it), i++);
	}
	m_progressCtrl.SetPos(100);
	m_gridCtrl.SetRedraw(TRUE);//允许重绘画面
	m_gridCtrl.ResetScrollBars();
}

void CCheckMissImageDlg::GetPageAndGroupNameForPageId(int nPageId, CString& csPageName, CString& csGroupName)
{
	for(auto it = m_pageInfoList.begin(); it != m_pageInfoList.end(); it++)
	{
		if(it->m_nPageId == nPageId)
		{
			csPageName = it->m_csPageName;
			csGroupName = it->m_csGroupName;
		}
	}
}

bool CCheckMissImageDlg::FindPictureId(int nPicture)
{
	for(auto it = m_pictureList.begin(); it != m_pictureList.end(); it++)
	{
		if((*it) == nPicture)
			return true;
	}
	return false;
}

void CCheckMissImageDlg::InsertItemListCrtl(MissImageDeviceInfo* pItem, int nRow)
{
	m_gridCtrl.SetItemText(nRow, 0, pItem->m_csGroupName);
	m_gridCtrl.SetItemText(nRow, 1, pItem->m_csPageName);
	m_gridCtrl.SetItemText(nRow, 2, pItem->m_csPageID);
	m_gridCtrl.SetItemText(nRow, 3, pItem->m_csDeviceID);
	m_gridCtrl.SetItemText(nRow, 4, pItem->m_csPictureID);
	m_gridCtrl.SetItemText(nRow, 5, pItem->m_csBindPictureID);
}


UINT CCheckMissImageDlg::Thread_Check(LPVOID pParam)
{
	CCheckMissImageDlg * pThis = (CCheckMissImageDlg*)pParam;
	pThis->CheckMissingImage();
	return 0;
}

void CCheckMissImageDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	delete this;
	/*CDialogEx::OnClose();*/
}


void CCheckMissImageDlg::OnBnClickedButtonExport()
{
	// TODO: 在此添加控件通知处理程序代码
	

	CFileDialog dlg(FALSE, L"Text File(*.txt)|*.txt||", NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Text File(*.txt)|*.txt||", this);
	if(dlg.DoModal() == IDOK)
	{
		CString csFilePath = dlg.GetPathName();
		if(csFilePath.GetLength() == 0)
		{
			AfxMessageBox(L"导出失败，没有获取到保存位置。");
			return;
		}

		CStdioFile fileCtrl;
		if(!fileCtrl.Open(csFilePath, CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox(L"导出失败，无法创建文件。");
			return;
		}

		for(auto it = m_MissImageInfoList.begin();it!=m_MissImageInfoList.end(); it++)
		{
			CString temp;
			temp.Format(L"%s|%s|%s|%s|%s|%s\r\n"
				, it->m_csGroupName.GetString()
				, it->m_csPageName.GetString()
				, it->m_csPageID.GetString()
				, it->m_csDeviceID.GetString()
				, it->m_csPictureID.GetString()
				, it->m_csBindPictureID.GetString());
			string stemp = Project::Tools::WideCharToUtf8(temp.GetString());
			fileCtrl.Write(stemp.c_str(), stemp.size());
			/*fileCtrl.WriteString(temp.GetString());*/
		}
		fileCtrl.Close();
		AfxMessageBox(L"保存成功");
	}
}
