// CompletionData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CompletionData.h"
#include "afxdialogex.h"

// CCompletionData �Ի���

IMPLEMENT_DYNAMIC(CCompletionData, CDialogEx)

CCompletionData::CCompletionData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompletionData::IDD, pParent)
	, m_CompletionDataList(_T(""))
	, m_tFrom(COleDateTime::GetCurrentTime())
	, m_tTo(COleDateTime::GetCurrentTime())
	, m_nPeriodType(0)
{

}

CCompletionData::~CCompletionData()
{
}

void CCompletionData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POINT, m_CompletionDataList);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_FROM, m_tFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_tTo);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nPeriodType);
	DDX_Control(pDX, IDC_COMBO1, m_TimeDivisionCon);
}


BEGIN_MESSAGE_MAP(CCompletionData, CDialogEx)
	/*ON_BN_CLICKED(IDC_MFCLINK1, &CCompletionData::OnBnClickedMfclink1)*/
	ON_EN_CHANGE(IDC_EDIT_POINT, &CCompletionData::OnEnChangeEditPoint)
END_MESSAGE_MAP()


// CCompletionData ��Ϣ�������

BOOL CCompletionData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_TimeDivisionCon.InsertString(0, _T("1����"));
	m_TimeDivisionCon.InsertString(1, _T("5����"));
	m_TimeDivisionCon.InsertString(2, _T("1Сʱ"));
	m_TimeDivisionCon.InsertString(3, _T("1��"));
	m_TimeDivisionCon.SetCurSel(0);//��ʾ��һ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCompletionData::OnEnChangeEditPoint()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������	
}


