// ChangeValueDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChangeValueDlg.h"
#include "afxdialogex.h"
#include "../ServerDataAccess/UPPCServerDataAccess.h"


// CChangeValueDlg �Ի���

IMPLEMENT_DYNAMIC(CChangeValueDlg, CDialogEx)

CChangeValueDlg::CChangeValueDlg(CUPPCServerDataAccess *pDataAccess, CString strName,  CString strValue, BOOL bSingle, int nTimeout, CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangeValueDlg::IDD, pParent)
	,m_pDataAccess(pDataAccess)
	, m_strName(strName)
	, m_strValue(strValue)
	, m_bSingle(bSingle)
	, m_nTimeout(nTimeout)
{
	
	m_nCheckCount = 0;
}

CChangeValueDlg::~CChangeValueDlg()
{
}

void CChangeValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangeValueDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChangeValueDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CChangeValueDlg::SetValue( CString strValue )
{
	m_strValue = strValue;
}

CString CChangeValueDlg::GetValue()
{
	return m_strValue;
}


// CChangeValueDlg ��Ϣ�������


BOOL CChangeValueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_bSingle)
	{
		GetDlgItem(IDC_EDIT_NAME)->SetWindowText(m_strName);
		GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_strValue);
	}
	else
	{
		CString strName = GetMultiNameFromVector(m_vecPoint);
		GetDlgItem(IDC_EDIT_NAME)->SetWindowText(strName);
		GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(m_strValue);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CChangeValueDlg::SetName( CString strName )
{
	m_strName = strName;
}


void CChangeValueDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_bSingle)
	{
		GetDlgItem(IDC_EDIT_NAME)->GetWindowText(m_strName);
		GetDlgItem(IDC_EDIT_VALUE)->GetWindowText(m_strValue);
	}
	else
	{
		CString strName = GetMultiNameFromVector(m_vecPoint);
		GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strName);
		GetDlgItem(IDC_EDIT_VALUE)->GetWindowText(m_strValue);
	}

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	wstring strValueToSet = m_strValue;
	if (m_bSingle)
	{
		m_pDataAccess->SetValueString(m_strName.GetString(), strValueToSet);
	}
	else
	{
		vector<tstring>	vecPtName;
		vector<CString>::const_iterator	iter;
		for (iter=m_vecPoint.begin(); iter!=m_vecPoint.end(); iter++)
		{
			vecPtName.push_back((*iter).GetString());
		}
		m_pDataAccess->SetValueStringList(vecPtName, strValueToSet);
	}
	m_pDataAccess->InsertPointSetValueLogToLogicLog(L"[Factory][Realtime-DataManger]",m_strName.GetString(),strValueToSet);
	SetTimer(1,500,NULL);

//	CDialogEx::OnOK();
}

//��ʱ���ص�����
void CChangeValueDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		m_nCheckCount++;
		if(m_nCheckCount>=2*m_nTimeout)
		{
			KillTimer(1);
			AfxMessageBox(L"�趨ֵ�·��󣬼��10�����δɨ�赽ֵ�޸ĳɹ�����Ϊ�趨ʧ�ܡ���ϵͳ��Ӧ��������Ҫ�ȴ�һ��ʱ����ɨ��ɹ�");
			CDialogEx::OnCancel();
		}
		wstring strValueGet;
		if(m_pDataAccess->GetValue(m_strName.GetString(), strValueGet))
		{
			if(strValueGet==m_strValue.GetString() || _wtof(strValueGet.c_str()) == _wtof(m_strValue))
			{
				KillTimer(1);
				CDialogEx::OnOK();
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CChangeValueDlg::SetMultiName(const vector<CString>& vecPoint)
{
	m_vecPoint = vecPoint;
}

CString	CChangeValueDlg::GetMultiNameFromVector(const vector<CString>& vecPoint)
{
	CString strMultiName;
	const CString strComma(_T(","));
	vector<CString>::const_iterator	iter;
	for (iter=vecPoint.begin(); iter!=vecPoint.end(); iter++)
	{
		strMultiName += *iter;
		strMultiName += strComma;
	}
	strMultiName.TrimRight(strComma);
	return strMultiName;
}
