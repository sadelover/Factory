// InputLogicPeriodDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InputLogicPeriodDlg.h"
#include "afxdialogex.h"


// CInputLogicPeriodDlg 对话框

IMPLEMENT_DYNAMIC(CInputLogicPeriodDlg, CDialogEx)

CInputLogicPeriodDlg::CInputLogicPeriodDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputLogicPeriodDlg::IDD, pParent)
	, m_nLogicPeriod(0)
{

}

CInputLogicPeriodDlg::~CInputLogicPeriodDlg()
{
}

void CInputLogicPeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGIC_PERIOD, m_nLogicPeriod);
	DDV_MinMaxInt(pDX, m_nLogicPeriod, 500, INT_MAX);
}


BEGIN_MESSAGE_MAP(CInputLogicPeriodDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputLogicPeriodDlg 消息处理程序
