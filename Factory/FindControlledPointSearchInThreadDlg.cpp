// ControlledPointSearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FindControlledPointSearchInThreadDlg.h"
#include "afxdialogex.h"

// CFindControlledPointSearchInThreadDlg �Ի���

IMPLEMENT_DYNAMIC(CFindControlledPointSearchInThreadDlg, CDialogEx)

CFindControlledPointSearchInThreadDlg::CFindControlledPointSearchInThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindControlledPointSearchInThreadDlg::IDD, pParent)
	, m_cstrSearchPoint(_T(""))
{

}

CFindControlledPointSearchInThreadDlg::~CFindControlledPointSearchInThreadDlg()
{
}

void CFindControlledPointSearchInThreadDlg::Set_m_logicList(vector<CLogicDllThread *>& logicList)
{
	m_logicList = logicList;
}

void CFindControlledPointSearchInThreadDlg::Set_m_pLogicManager(CBEOPLogicManager * pLogicManager)
{
	m_pLogicManager = pLogicManager;
}

void CFindControlledPointSearchInThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCH_POINT, m_cstrSearchPoint);
	DDX_Control(pDX, IDC_LIST_THREAD, m_listBoxThread);
	DDX_Control(pDX, IDC_PROGRESS_Search, m_ProgressSearch);
}


BEGIN_MESSAGE_MAP(CFindControlledPointSearchInThreadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CFindControlledPointSearchInThreadDlg::OnBnClickedButtonSearch)
	/*ON_BN_CLICKED(IDOK, &CFindControlledPointSearchInThreadDlg::OnBnClickedOk)*/
	ON_LBN_DBLCLK(IDC_LIST_THREAD, &CFindControlledPointSearchInThreadDlg::OnLbnDblclkListThread)
END_MESSAGE_MAP()


// CFindControlledPointSearchInThreadDlg ��Ϣ�������


void CFindControlledPointSearchInThreadDlg::OnBnClickedButtonSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_listBoxThread.ResetContent();
	m_ProgressSearch.SetRange(0,1000);
	AfxBeginThread(SearchPointThread, (LPVOID *)this);
	UpdateData(FALSE);
}

UINT CFindControlledPointSearchInThreadDlg::SearchPointThread(LPVOID pParam)
{
	CFindControlledPointSearchInThreadDlg* pThis = (CFindControlledPointSearchInThreadDlg*)pParam;
	pThis->SearchPoint();
	return 1;
}

void CFindControlledPointSearchInThreadDlg::SearchPoint()
{
	int nUnit = Progress_MAX/m_logicList.size();
	int nPos = 0;
	m_ProgressSearch.SetPos(5);
	int nC_m_listBoxThread = 0;
	//�����߳�
	for(int nC_Thread=0; nC_Thread<m_logicList.size(); nC_Thread++)
	{
		m_pLogicManager->LoadDllThreadRelationship(m_logicList[nC_Thread]);//����dll��������������m_vecCurDllInputParameterList
		//��������
		for(int nC_Strategy=0; nC_Strategy<m_logicList[nC_Thread]->GetDllCount(); nC_Strategy++)
		{
			CDLLObject * temp_dllObject = m_logicList[nC_Thread]->GetDllObject(nC_Strategy);
			//��������
			for(int nC_Parame=0;nC_Parame<temp_dllObject->m_vecCurDllInputParameterList.size();nC_Parame++)
			{
				wstring tempWstring = temp_dllObject->m_vecCurDllInputParameterList[nC_Parame]->GetSettingValue();
				//�ű���ʽ���ص�
				wstring tempParam =L"<%";
				tempParam += m_cstrSearchPoint.GetString();
				tempParam += L"%>";

				if((tempWstring == m_cstrSearchPoint.GetString())//��ͨ�жϣ���������find��ȫƥ��ȽϺ��ʡ�
					|| (tempWstring.find(tempParam) != -1)//�ű���ʽ���ص��ж�
					)
				{
					CString cstrFindPointThread;
					cstrFindPointThread.Format(L"%s::%s", m_logicList[nC_Thread]->GetName().c_str(), temp_dllObject->GetDllOriginName()/*temp_dllObject->GetDllName()*/);
					m_listBoxThread.InsertString(nC_m_listBoxThread++,cstrFindPointThread);
					break;
				}
			}
		}
		nPos+=nUnit;
		m_ProgressSearch.SetPos(nPos);
	}
	if(m_listBoxThread.GetCount() == 0)
		AfxMessageBox(L"�˵�û�в������ҵ���");
	m_ProgressSearch.SetPos(Progress_MAX);
}

void CFindControlledPointSearchInThreadDlg::SetCFileView(CFileView* pFileView)
{
	m_pCFileView = pFileView;
}

void CFindControlledPointSearchInThreadDlg::OnLbnDblclkListThread()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int curCel = m_listBoxThread.GetCurSel();
	if(curCel != -1 && curCel < m_listBoxThread.GetCount())
	{
		CString strAll;
		m_listBoxThread.GetText(curCel, strAll);
		if( strAll.GetLength() != 0)
		{
			strAll = strAll.Left(strAll.Find(L':'));
			HTREEITEM selectH = m_pCFileView->FinItem_m_wndFileView(m_pCFileView->GetTactHTI(), strAll);
			if(selectH  == NULL)
				return;
			m_pCFileView->m_wndFileView.SelectItem(selectH);
			LRESULT temp;
			m_pCFileView->OnNMDblclkTree(NULL, &temp);
		}	
	}
}
