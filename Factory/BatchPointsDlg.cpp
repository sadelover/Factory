// BatchPoints.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BatchPointsDlg.h"
#include "afxdialogex.h"
#include "DataPointConfigDlg.h"

// CBatchPointsDlg �Ի���

IMPLEMENT_DYNAMIC(CBatchPointsDlg, CDialogEx)

CBatchPointsDlg::CBatchPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBatchPointsDlg::IDD, pParent)
	, m_nRowNumber(0)
{

}

CBatchPointsDlg::~CBatchPointsDlg()
{
}

void CBatchPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_ROW_NUMBER, m_nRowNumber);
}


BEGIN_MESSAGE_MAP(CBatchPointsDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_ADD_ROW, &CBatchPointsDlg::OnBnClickedButtonAddRow)
	ON_BN_CLICKED(IDOK, &CBatchPointsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBatchPointsDlg ��Ϣ�������


BOOL CBatchPointsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitGRID();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBatchPointsDlg::InitGRID (void)
{
	CRect cr;
	m_Grid.GetClientRect(&cr);

	m_nRowNumber = 10;
	ncolNum = 3;

	m_Grid.SetRowCount(m_nRowNumber+1);//����������
	m_Grid.SetColumnCount(ncolNum);//����������
	
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);//��ͷһ��һ��
	
	COLORREF clr = RGB(0, 0, 0);
	m_Grid.SetFixedTextColor(clr);//���ù̶��е���ɫ
	m_Grid.SetFixedBkColor(RGB(192, 192, 192));//���ù̶��еı�����ɫ

	m_Grid.SetTextBkColor(RGB(255, 255, 255));//���ÿɱ༭���򱳾���ɫ
	m_Grid.SetTextColor(RGB(0, 0, 0));//���ÿɱ༭����������ɫ
	m_Grid.SetEditable(true);//������ݲ��ɱ༭����

	m_Grid.SetColumnWidth(0, 35);//���õ�һ�п�
	m_Grid.SetColumnWidth(1, 200);//���õ�һ�п�
	m_Grid.SetColumnWidth(2, 200);//���õ�һ�п�
	m_Grid.SetRowHeight(0, 35);//�����и�

	m_Grid.SetItemText(0, 1, _T("����"));//��������
	m_Grid.SetItemText(0, 2, _T("ע��"));//��������

	CString strRowNum;
	for(int i=1; i<m_nRowNumber+1; i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid.SetItemText(i, 0, strRowNum);
	}
}

//"��������"��ť������
void CBatchPointsDlg::OnBnClickedButtonAddRow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_Grid.SetRowCount(m_nRowNumber+1);
	CString strRowNum;
	for(int i=1; i<=m_nRowNumber; i++)//����һ�����ɸ�����
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid.SetItemText(i, 0, strRowNum);
	}

	//�Զ������һ�п��
	CString cstrRomNum;
	cstrRomNum.Format(L"%d",m_nRowNumber);
	int nRomWidth = (cstrRomNum.GetLength() *10) +5;
		m_Grid.SetColumnWidth(0, nRomWidth);//���õ�һ�п�

	UpdateData(FALSE);
}
void CBatchPointsDlg::SetDlgType( DLG_TYPE eType )
{
	m_eType = eType;
}
void CBatchPointsDlg::SetPointMap(CDataPointManager* pPointMap)
{
	ASSERT(pPointMap != NULL);
	if (pPointMap != NULL)
	{
		m_pPointMap = pPointMap;
	}
}
int CBatchPointsDlg::GetMaxKeyID()
{
	int nMaxID = 0;
	CDataPointConfigDlg* pWnd = (CDataPointConfigDlg*)GetParent();
	ASSERT(pWnd);
	map<int, DataPointEntry>& datalist = pWnd->m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::iterator iter = datalist.begin();
	for (;iter!=datalist.end();++iter)
	{
		if (iter->first > nMaxID)
		{
			nMaxID = iter->first;
		}
	}
	return nMaxID;
}
void CBatchPointsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
		������������ռ�
	*/
	m_nReallNum=0;
	m_TableOfName.clear();
	m_TableOfNotation.clear();
	for(int i=1;i<m_nRowNumber;i++)
	{

		CString cstrPointName = m_Grid.GetItemText(i,1);
		CString cstrPointDes  = m_Grid.GetItemText(i,2);
		
		//������ֱ������
		if(cstrPointName == L"")
		{
			continue;
		}

		//�������ĸ
		if((cstrPointName[0]>=L'a'& cstrPointName[0]<='z') || (cstrPointName[0]>=L'A' & cstrPointName[0]<=L'Z'))
		{

		}
		else
		{
			CString cstrAfx;
			cstrAfx.Format(L"��%d��\"%s\"����ID������Ӣ���ַ���ͷ!",i ,cstrPointName);
			AfxMessageBox(cstrAfx);
			return;
		}

		//ɾ�������д��ڵĿո�
		if(cstrPointName.Find(L' ') != -1)
		{
			int nWordMax = cstrPointName.GetLength();
			for(int nWord=0; nWord<nWordMax; nWord++)
			{
				int nSpaceLocation = cstrPointName.Find(L' ');
				if(nSpaceLocation != -1)
					cstrPointName.Delete(nSpaceLocation);
				else
					break;
			}
		}

		//�������еķǷ��ַ�
		for(int j=0;j<cstrPointName.GetLength();j++)
		{
			wchar_t wcOne = cstrPointName.GetAt(j);
			if ((wcOne>=L'a' && wcOne<=L'z') || (wcOne>=L'A' && wcOne<=L'Z') || wcOne=='_' || (wcOne>=L'0' && wcOne<=L'9'))
			{

			}
			else
			{
				CString cstrAfx;
				cstrAfx.Format(L"��%d�е�\"%s\"�������к��зǷ��ַ�(ֻ֧��Ӣ����ĸ�����������֡��»���!",i , cstrPointName);
				AfxMessageBox(cstrAfx);
				return;
			}
		}

		////���ĩβ�Ƿ��зǷ��ַ�
		//int nErr_1 = cstrPointName.Find(_T(" "));
		//int nErr_2 = cstrPointName.Find(_T("\n"));
		//int nErr_3 = cstrPointName.Find(_T("\r"));
		//int nErr_4 = cstrPointName.Find(_T("@"));
		//int nErr_5 = cstrPointName.Find(_T("*"));
		//if (nErr_1!=-1||nErr_2!=-1||nErr_3!=-1||nErr_4!=-1||nErr_5!=-1)
		//{
		//	CString AfxStr = _T("%s�зǷ��ַ����ַǷ��ַ�:",cstrPointName);
		//	if(nErr_1!=-1)
		//		AfxStr+=_T("�ո�");
		//	if(nErr_2!=-1)
		//		AfxStr+=_T("\\n");
		//	if(nErr_3!=-1)
		//		AfxStr+=_T("\\r");
		//	if(nErr_4!=-1)
		//		AfxStr+=_T("@");
		//	if(nErr_5!=-1)
		//		AfxStr+=_T("*");
		//	AfxMessageBox(AfxStr);
		//	return;
		//}

		m_nReallNum++;
		m_TableOfName.push_back(cstrPointName);
		m_TableOfNotation.push_back(cstrPointName);
	}

	/*
		�㱣��
	*/
	CDataPointConfigDlg* pWnd = (CDataPointConfigDlg*)GetParent();
	ASSERT(pWnd);

	DataPointEntry entry;
	map<int, DataPointEntry>& datalist = pWnd->m_plcpointmap.GetPointMap();
	for(int i=0;i<m_nReallNum;i++)
	{
		//���ID�Ƿ��ظ�
		map<int, DataPointEntry>::const_iterator it = datalist.begin();
		for (;it != datalist.end(); it++)
		{
			const DataPointEntry& entryMap = it->second;
			if (entryMap.GetShortName() == m_TableOfName.at(i).GetString())
			{
				AfxMessageBox(L"PhysicalID�ظ�!");
				return;
			}
		}
		entry.SetPointIndex(GetMaxKeyID()+1);
		entry.SetShortName(m_TableOfName.at(i).GetString());
		entry.SetShortNameOrigin(m_TableOfName.at(i).GetString());
		entry.SetSourceType(L"vpoint");
		entry.SetDescription( m_TableOfNotation.at(i).GetString() );
		entry.SetUnit(L"");
		entry.SetProperty(PLC_WRITE);
	
		entry.SetParam(1, L"");
		entry.SetParam(2, L"");
		entry.SetParam(3, L"");
		entry.SetParam(4, L"");
		entry.SetParam(5, L"");
		entry.SetParam(6, L"");
		entry.SetParam(7, L"");
		entry.SetParam(8, L"");
		entry.SetParam(9, L"");
		entry.SetParam(10, L"");
		entry.SetParam(11, L"");
		entry.SetParam(12, L"");
		entry.SetParam(13, L"");
		entry.SetParam(14, L"0");
		entry.SetStoreCycle(E_STORE_ONE_MINUTE);
		entry.SetParam(15, L"");
		entry.SetParam(16, L"");
		entry.SetParam(17, L"");
		entry.SetParam(18, L"");

		pWnd->AddOneEntry(entry);
	}
	//delete pWnd;
	CDialogEx::OnOK();
}
