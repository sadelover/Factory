// EqmRectangleConfig_RectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EqmRectangleConfigDlg_Rect.h"
#include "afxdialogex.h"


// CEqmRectangleConfigDlg_Rect 对话框

IMPLEMENT_DYNAMIC(CEqmRectangleConfigDlg_Rect, CDialogEx)

CEqmRectangleConfigDlg_Rect::CEqmRectangleConfigDlg_Rect(CEqmDrawRectangle* pRectangle, CWnd* pParent /*=NULL*/)
	: CDialogEx(CEqmRectangleConfigDlg_Rect::IDD, pParent)
	, m_pRectangle(pRectangle)
	, m_nBordeTransparency(0)
	, m_bFill(FALSE)
	, m_bRound(FALSE)
	, m_nRoundHeight(0)
	, m_nRoundWidth(0)
{

}

CEqmRectangleConfigDlg_Rect::~CEqmRectangleConfigDlg_Rect()
{
}

void CEqmRectangleConfigDlg_Rect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_BORDER, m_colorBtn_border);
	DDX_Text(pDX, IDC_EDIT_BORDER, m_nBordeTransparency);
	DDX_Check(pDX, IDC_CHECK_FILL, m_bFill);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_FILL, m_colorBtn_fill);
	DDX_Text(pDX, IDC_EDIT_FILL, m_nFillTransparency);
	DDX_Check(pDX, IDC_CHECK_ROUND, m_bRound);
	DDX_Text(pDX, IDC_EDIT_ROUND_HIGHT, m_nRoundHeight);
	DDX_Text(pDX, IDC_EDIT_ROUND_WIDTH, m_nRoundWidth);
}


BEGIN_MESSAGE_MAP(CEqmRectangleConfigDlg_Rect, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEqmRectangleConfigDlg_Rect::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_FILL, &CEqmRectangleConfigDlg_Rect::OnBnClickedCheckFill)
	ON_BN_CLICKED(IDC_CHECK_ROUND, &CEqmRectangleConfigDlg_Rect::OnBnClickedCheckRound)
	ON_BN_CLICKED(IDCANCEL, &CEqmRectangleConfigDlg_Rect::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEqmRectangleConfigDlg_Rect 消息处理程序

BOOL CEqmRectangleConfigDlg_Rect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(m_pRectangle == NULL)
	{
		AfxMessageBox(L"获取不到矩形的属性");
		OnBnClickedOk();
	}

	m_colorBtn_border.SetColor(m_pRectangle->GetLineColorRGB());
	m_nBordeTransparency = m_pRectangle->GetTransparency();
	m_bFill = m_pRectangle->GetFillStyl();
	m_colorBtn_fill.SetColor(m_pRectangle->GetFillColorRGB());
	m_nFillTransparency = m_pRectangle->GetFillTransparency();
	m_bRound = m_pRectangle->GetToRoundRect();
	m_nRoundHeight = m_pRectangle->GetRoundHeight();
	m_nRoundWidth = m_pRectangle->GetRoundWidth();
	if(FALSE == m_bFill)
	{
		GetDlgItem(IDC_MFCCOLORBUTTON_FILL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILL)->EnableWindow(FALSE);
	}
	if(FALSE == m_bRound)
	{
		GetDlgItem(IDC_EDIT_ROUND_HIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROUND_WIDTH)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEqmRectangleConfigDlg_Rect::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pRectangle->SetRectangleStyle(0);
	m_pRectangle->SetLineColorRGB(m_colorBtn_border.GetColor());
	m_pRectangle->SetTransparency(m_nBordeTransparency);

	m_pRectangle->SetFillStyl(m_bFill);
	if(TRUE == m_bFill)
	{
		m_pRectangle->SetFillColorRGB(m_colorBtn_fill.GetColor());
		m_pRectangle->SetFillTransparency(m_nFillTransparency);
	}

	m_pRectangle->SetToRoundRect(m_bRound);
	if(TRUE == m_bRound)
	{
		m_pRectangle->SetRoundHeight(m_nRoundHeight);
		m_pRectangle->SetRoundWidth(m_nRoundWidth);
	}

	//关闭窗口
	CWnd *pParent = GetParent();
	if(pParent == NULL)
		return;
	pParent->PostMessage(WM_CLOSE);
	CDialogEx::OnOK();
}

void CEqmRectangleConfigDlg_Rect::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//关闭窗口
	CWnd *pParent = GetParent();
	if(pParent == NULL)
		return;
	pParent->PostMessage(WM_CLOSE);
	CDialogEx::OnCancel();
}



void CEqmRectangleConfigDlg_Rect::OnBnClickedCheckFill()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_MFCCOLORBUTTON_FILL)->EnableWindow(m_bFill);
	GetDlgItem(IDC_EDIT_FILL)->EnableWindow(m_bFill);
}


void CEqmRectangleConfigDlg_Rect::OnBnClickedCheckRound()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_ROUND_HIGHT)->EnableWindow(m_bRound);
	GetDlgItem(IDC_EDIT_ROUND_WIDTH)->EnableWindow(m_bRound);
}
