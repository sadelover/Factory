// ChangePointNameForHistoricalDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChangePointNameForHistoricalDataDlg.h"
#include "afxdialogex.h"

#include "Tools/Tools_String.h"

// CChangePointNameForHistoricalDataDlg �Ի���

UINT Modify(LPVOID pParam);

IMPLEMENT_DYNAMIC(CChangePointNameForHistoricalDataDlg, CDialogEx)

CChangePointNameForHistoricalDataDlg::CChangePointNameForHistoricalDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangePointNameForHistoricalDataDlg::IDD, pParent)
	, m_nGridRow(5)
	, m_cstrTempPointName(_T(""))
{

}

CChangePointNameForHistoricalDataDlg::~CChangePointNameForHistoricalDataDlg()
{
}

void CChangePointNameForHistoricalDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_NAME_LIST, m_Grid_NameList);
	DDX_Text(pDX, IDC_EDIT_GRID_ROW, m_nGridRow);
	DDX_Text(pDX, IDC_EDIT_GET_POINT_NAME, m_cstrTempPointName);
	DDX_Control(pDX, IDC_PROGRESS2, m_PgsCtlModify);
}



BEGIN_MESSAGE_MAP(CChangePointNameForHistoricalDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SET_GRID_ROW, &CChangePointNameForHistoricalDataDlg::OnBnClickedButtonSetGridRow)

	ON_BN_CLICKED(IDC_BUTTON_QUERY_POINT, &CChangePointNameForHistoricalDataDlg::OnBnClickedButtonQueryPoint)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CChangePointNameForHistoricalDataDlg::OnBnClickedButtonModify)
END_MESSAGE_MAP()


// CChangePointNameForHistoricalDataDlg ��Ϣ�������

BOOL CChangePointNameForHistoricalDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect cr;
	m_Grid_NameList.GetClientRect(&cr);

	m_Grid_NameList.SetRowCount(m_nGridRow+1);//����������
	m_Grid_NameList.SetColumnCount(3);//����������
	m_Grid_NameList.SetFixedRowCount(1);
	m_Grid_NameList.SetFixedColumnCount(1);//��ͷһ��һ��
	m_Grid_NameList.SetEditable(true);//������ݲ��ɱ༭����
	m_Grid_NameList.SetItemText(0, 1, _T("�ɵ���"));//��������
	m_Grid_NameList.SetItemText(0, 2, _T("�µ���"));//��������
	m_Grid_NameList.SetColumnWidth(1,200);
	m_Grid_NameList.SetColumnWidth(2,200);

	//�����б���п�
	CString cstrRow;
	cstrRow.Format(L"%d",m_nGridRow);
	int nNumber = cstrRow.GetLength()*7+15;
	m_Grid_NameList.SetColumnWidth(0, nNumber);
	//ˢ���б�ʶ
	CString strRowNum;
	for(int i=1; i<=m_nGridRow; i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid_NameList.SetItemText(i, 0, strRowNum);
	}
	m_PgsCtlModify.SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CChangePointNameForHistoricalDataDlg::OnBnClickedButtonSetGridRow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ���������롢
	UpdateData(TRUE);

	//�����б���п�
	CString cstrRow;
	cstrRow.Format(L"%d",m_nGridRow);
	int nNumber = cstrRow.GetLength()*7+15;
	m_Grid_NameList.SetColumnWidth(0, nNumber);

	//ˢ���б�ʶ
	int nRow = m_nGridRow+1;
	m_Grid_NameList.SetRowCount(nRow);
	CString strRowNum;
	for(int i=1; i<=m_nGridRow; i++)//��һ�в�д
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid_NameList.SetItemText(i, 0, strRowNum);
	}

	UpdateData(FALSE);
}

void CChangePointNameForHistoricalDataDlg::OnBnClickedButtonQueryPoint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_cstrTempPointName = dlg.m_strPointName;
	}
	UpdateData(FALSE);
}

//�޸İ�ť����¼�����
void CChangePointNameForHistoricalDataDlg::OnBnClickedButtonModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CWinThread	*pMyFirstWorker;
	LPVOID		pParam = (void *)this;
	pMyFirstWorker = AfxBeginThread(Modify, pParam);//�������Ĭ��ֵ
	SetButtonModifyState(FALSE);
}

//�����޸İ�ť�Ľ������
void CChangePointNameForHistoricalDataDlg::SetButtonModifyState(BOOL bState)
{
	CButton * btCtl = (CButton*)GetDlgItem(IDC_BUTTON_MODIFY);
	btCtl->EnableWindow(bState);
}

//��������

void CChangePointNameForHistoricalDataDlg::Set_m_pDataAccess(CUPPCServerDataAccess* pDataAccess)
{
	m_pDataAccess = pDataAccess;
}
CUPPCServerDataAccess* CChangePointNameForHistoricalDataDlg::Get_m_pDataAccess()
{
	return m_pDataAccess;
}

//���ý�����
void CChangePointNameForHistoricalDataDlg::SetProgressPos(int nPos)
{
	m_PgsCtlModify.SetPos(nPos);
}

//�ӱ���л�ȡ�¾ɵ���
void CChangePointNameForHistoricalDataDlg::GetAllPointName(vector<wstring>& _wstrOldNameList, vector<wstring>& _wstrNewNameList)
{
	for(int i=1; i<=m_nGridRow; i++)
	{
		wstring wstrOldName = m_Grid_NameList.GetItemText(i,1).GetString();
		wstring wstrNewName = m_Grid_NameList.GetItemText(i,2).GetString();
		if(wstrOldName.length() <= 0 || wstrNewName.length() <= 0)
		{
			//ȫ������
			if(wstrOldName.length() <= 0 && wstrNewName.length() <= 0)
				continue;
			CString cstrMsg;cstrMsg.Format(L"%d�У��пյ����ǲ�������ġ�",i);
			AfxMessageBox(cstrMsg);
			return ;
		}
		_wstrOldNameList.push_back(wstrOldName);
		_wstrNewNameList.push_back(wstrNewName);
	}
}

//�޸��߳�
UINT Modify(LPVOID pParam)
{
	CChangePointNameForHistoricalDataDlg* pThis = (CChangePointNameForHistoricalDataDlg*)pParam;

	pThis->SetProgressPos(0);

	vector<wstring> wstrOldNameList;
	vector<wstring> wstrNewNameList;
	vector<wstring> wstrHistoryDataTableList;
	pThis->GetAllPointName(wstrOldNameList, wstrNewNameList);
	pThis->SetProgressPos(20);
	//������
	{
		CString cstrMessage;
		CString cstrTempMsg;
		bool bFindError = true;
		cstrMessage+=L"������ĵ���:\n";
		//ԭ����
		for( int nCount=0;nCount<wstrOldNameList.size();nCount++ )
		{
			//����ĸ����������ĸ�»���������ַ�
			if(Tools_String::Whether_First_LettersUnderscores(wstrOldNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"��%d�� %s\n", nCount+1, wstrOldNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
				continue;//��������һ���ж�
			}

			//�ַ���ֻ�����ַ������»���
			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrOldNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"��%d�� %s\n", nCount+1, wstrOldNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
			}
		}
		pThis->SetProgressPos(40);
		//�µ���
		for( int nCount=0;nCount<wstrNewNameList.size();nCount++ )
		{
			//����ĸ����������ĸ�»���������ַ�
			if(Tools_String::Whether_First_LettersUnderscores(wstrNewNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"��%d�� %s\n", nCount+1, wstrNewNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
				continue;//��������һ���ж�
			}

			//�ַ���ֻ�����ַ������»���
			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrNewNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"��%d�� %s.\n",nCount+1,wstrNewNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
			}
		}
		pThis->SetProgressPos(60);
		if(bFindError == false)
		{
			pThis->SetProgressPos(0);
			pThis->SetButtonModifyState(TRUE);
			AfxMessageBox(cstrMessage);
			return 0;
		}
	}

	CUPPCServerDataAccess* pDataAccess = pThis->Get_m_pDataAccess();
	pDataAccess->ReadBeopdataAllHistorydataList(wstrHistoryDataTableList);
	pThis->SetProgressPos(65);
	pDataAccess->UpdateHistoryDataPointName(wstrHistoryDataTableList, wstrOldNameList, wstrNewNameList);
	pThis->SetProgressPos(100);
	AfxMessageBox(L"�޸����");
	pThis->SetButtonModifyState(TRUE);
	return 1;
}

//UINT MyThreadFunction(LPVOID pParam)
//{
//	CChangePointNameForHistoricalDataDlg* pThis = (CChangePointNameForHistoricalDataDlg*)pParam;
//	
//	pThis->SetProgressPos(0);
//
//	vector<wstring> wstrOldNameList;
//	vector<wstring> wstrNewNameList;
//	vector<wstring> wstrHistoryDataTableList;
//	pThis->GetAllPointName(wstrOldNameList, wstrNewNameList);
//	for(int i=1; i<=m_nGridRow; i++)
//	{
//		wstring wstrOldName = m_Grid_NameList.GetItemText(i,1).GetString();
//		wstring wstrNewName = m_Grid_NameList.GetItemText(i,2).GetString();
//		if(wstrOldName.length() <= 0 || wstrNewName.length() <= 0)
//		{
//			//ȫ������
//			if(wstrOldName.length() <= 0 && wstrNewName.length() <= 0)
//				continue;
//			CString cstrMsg;cstrMsg.Format(L"%d�У��пյ����ǲ�������ġ�",i);
//			AfxMessageBox(cstrMsg);
//			return 0;
//		}
//		wstrOldNameList.push_back(wstrOldName);
//		wstrNewNameList.push_back(wstrNewName);
//	}
//
//	pThis->SetProgressPos(20);
//	//������
//	{
//		CString cstrMessage;
//		CString cstrTempMsg;
//		bool bFindError = true;
//		cstrMessage+=L"������ĵ���:\n";
//		//ԭ����
//		for( int nCount=0;nCount<wstrOldNameList.size();nCount++ )
//		{
//			//����ĸ����������ĸ�»���������ַ�
//			if(Tools_String::Whether_First_LettersUnderscores(wstrOldNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"��%d�� %s\n", nCount+1, wstrOldNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//				continue;//��������һ���ж�
//			}
//
//			//�ַ���ֻ�����ַ������»���
//			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrOldNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"��%d�� %s\n", nCount+1, wstrOldNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//			}
//		}
//		pThis->SetProgressPos(40);
//		//�µ���
//		for( int nCount=0;nCount<wstrNewNameList.size();nCount++ )
//		{
//			//����ĸ����������ĸ�»���������ַ�
//			if(Tools_String::Whether_First_LettersUnderscores(wstrNewNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"��%d�� %s\n", nCount+1, wstrNewNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//				continue;//��������һ���ж�
//			}
//
//			//�ַ���ֻ�����ַ������»���
//			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrNewNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"��%d�� %s.\n",nCount+1,wstrNewNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//			}
//		}
//		pThis->SetProgressPos(60);
//		if(bFindError == false)
//		{
//			AfxMessageBox(cstrMessage);
//			return 0;
//		}
//	}
//
//	m_pDataAccess->ReadBeopdataAllHistorydataList(wstrHistoryDataTableList);
//	pThis->SetProgressPos(65);
//	m_pDataAccess->UpdateHistoryDataPointName(wstrHistoryDataTableList, wstrOldNameList, wstrNewNameList);
//	pThis->SetProgressPos(100);
//	AfxMessageBox(L"�޸����");
//	return 1;
//}