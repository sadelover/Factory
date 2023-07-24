// CheckMissImage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainFrm.h"
#include "CheckMissImageDlg.h"
#include "afxdialogex.h"

// CCheckMissImageDlg �Ի���

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


// CCheckMissImageDlg ��Ϣ�������


BOOL CCheckMissImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if(m_pMainFrame == NULL)
		return FALSE;

	m_gridCtrl.SetRowCount(1);
	m_gridCtrl.SetColumnCount(6);//����������
	m_gridCtrl.SetFixedRowCount(1);
	m_gridCtrl.SetEditable(true);//������ݲ��ɱ༭����
	m_gridCtrl.SetItemText(0, 0, _T("����"));//��������
	m_gridCtrl.SetItemText(0, 1, _T("ҳ��"));//��������
	m_gridCtrl.SetItemText(0, 2, _T("ҳ��ID"));//��������
	m_gridCtrl.SetItemText(0, 3, _T("Ԫ��ID"));//��������
	m_gridCtrl.SetItemText(0, 4, _T("ͼƬID"));//��������
	m_gridCtrl.SetItemText(0, 5, _T("��ͼƬID"));//��������

	m_progressCtrl.SetRange(0,100);

	AfxBeginThread(CCheckMissImageDlg::Thread_Check, (LPVOID)this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCheckMissImageDlg::InitPageInfo(void)
{
	//��ȡ���е�ҳ���ID������ ����Id�����֡�
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

//����  ҳ�� ͼƬID ��ͼID
void CCheckMissImageDlg::CheckMissingImage(void)
{
	//��ȡ���е�ҳ���ID������ ����Id�����֡�
	InitPageInfo();
	m_progressCtrl.SetPos(10);
	//��ȡ����device id ��device �󶨵�ͼƬ��Դid
	InitDeviceInfo();
	m_progressCtrl.SetPos(20);
	//��ȡ���е�ͼƬ��ԴID
	InitPictureList();
	m_progressCtrl.SetPos(30);
#if 0
	for(auto it = m_deviceInfoList.begin(); it!= m_deviceInfoList.end(); it++)
	{
		//deviceͼ
		if(it->m_nPicInfo.bPicOrAnmt == true)
		{
			if(FindPictureId(it->m_nPicInfo.bindID) == false)
			{
				CString csGroupName = L"";
				CString csPageName = L"";
				GetPageAndGroupNameForPageId(it->m_nPageId, csPageName, csGroupName);
				if(csGroupName.GetLength() == 0 || csPageName.GetLength() == 0)
				{
					csPageName = L"ҳ�治����";
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
					 csPageName = L"ҳ�治����";
				 }
				 m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID));
			 }
		}

		//��ͼ
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
						csPageName = L"ҳ�治����";
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
						csPageName = L"ҳ�治����";
					}
					m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID, it_bind->bindID));
				}
			}
		}
	}
#else
	//device ����һ�����������ҳ���Ѳ����ڵ���������Ϣ������ʾ��ͼƬ��Դ������
	for(auto it = m_deviceInfoList.begin(); it!= m_deviceInfoList.end(); it++)
	{
		CString csGroupName = L"";
		CString csPageName = L"";
		GetPageAndGroupNameForPageId(it->m_nPageId, csPageName, csGroupName);
		if(csGroupName.GetLength() == 0 || csPageName.GetLength() == 0)
		{
			csGroupName = L"          ";
			csPageName  = L"ҳ�治����";
			m_MissImageInfoList.push_back(MissImageDeviceInfo(csGroupName, csPageName, it->m_nPageId, it->m_nID, it->m_nPicInfo.bindID));
			continue;
		}

		//deviceͼ
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

		//��ͼ
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
	m_gridCtrl.SetRedraw(FALSE);//��ֹ�ػ滭��
	m_gridCtrl.SetRowCount(m_MissImageInfoList.size()+1);
	m_gridCtrl.SetRedraw(TRUE);
	int i=1;
	for(auto it = m_MissImageInfoList.begin();it!=m_MissImageInfoList.end(); it++)
	{
		InsertItemListCrtl(&(*it), i++);
	}
	m_progressCtrl.SetPos(100);
	m_gridCtrl.SetRedraw(TRUE);//�����ػ滭��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	delete this;
	/*CDialogEx::OnClose();*/
}


void CCheckMissImageDlg::OnBnClickedButtonExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	CFileDialog dlg(FALSE, L"Text File(*.txt)|*.txt||", NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Text File(*.txt)|*.txt||", this);
	if(dlg.DoModal() == IDOK)
	{
		CString csFilePath = dlg.GetPathName();
		if(csFilePath.GetLength() == 0)
		{
			AfxMessageBox(L"����ʧ�ܣ�û�л�ȡ������λ�á�");
			return;
		}

		CStdioFile fileCtrl;
		if(!fileCtrl.Open(csFilePath, CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox(L"����ʧ�ܣ��޷������ļ���");
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
		AfxMessageBox(L"����ɹ�");
	}
}
