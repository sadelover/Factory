// CompletionData.cpp : 实现文件
//

#include "stdafx.h"
#include "CompletionData.h"
#include "afxdialogex.h"

// CCompletionData 对话框

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


// CCompletionData 消息处理程序

BOOL CCompletionData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_TimeDivisionCon.InsertString(0, _T("1分钟"));
	m_TimeDivisionCon.InsertString(1, _T("5分钟"));
	m_TimeDivisionCon.InsertString(2, _T("1小时"));
	m_TimeDivisionCon.InsertString(3, _T("1天"));
	m_TimeDivisionCon.SetCurSel(0);//显示第一个
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCompletionData::OnEnChangeEditPoint()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码	
}


