// EqmRectangleConfig_BlueWhitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EqmRectangleConfigDlg_BlueWhit.h"
#include "afxdialogex.h"
#include "RectanglePointInfoDlg.h"
#include "MainFrm.h"
#include "CanvasDlg.h"
// CEqmRectangleConfigDlg_BlueWhit 对话框

IMPLEMENT_DYNAMIC(CEqmRectangleConfigDlg_BlueWhit, CDialogEx)

CEqmRectangleConfigDlg_BlueWhit::CEqmRectangleConfigDlg_BlueWhit(CCanvasDlg* pCanvalDlg,  CEqmDrawRectangle* pRectangle, CWnd* pParent /*=NULL*/)
	: CDialogEx(CEqmRectangleConfigDlg_BlueWhit::IDD, pParent)
	, m_pCanvalDlg(pCanvalDlg)
	, m_pRectangle(pRectangle)
	, m_nTitileSize(0)
	, m_nBodySize(0)
	, m_nParaphraseSize(0)
	, m_bRound(FALSE)
	, m_nRoundHeight(0)
	, m_csTitil(_T(""))
{

}

CEqmRectangleConfigDlg_BlueWhit::~CEqmRectangleConfigDlg_BlueWhit()
{
}

void CEqmRectangleConfigDlg_BlueWhit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_TITILE, m_colorBtn_title);
	DDX_Text(pDX, IDC_Edit_Title, m_nTitileSize);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_Body, m_colorBtn_Body);
	DDX_Text(pDX, IDC_EDIT_BODY, m_nBodySize);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_PARAPHRASE, m_colorBtn_paraphrase);
	DDX_Text(pDX, IDC_EDIT_PARAPHRASE, m_nParaphraseSize);
	DDX_Control(pDX, IDC_LIST_POINT, m_listCtrl_point);
	DDX_Check(pDX, IDC_CHECK_ROUND, m_bRound);
	DDX_Text(pDX, IDC_EDIT_ROUND_HIGHT, m_nRoundHeight);
	DDX_Text(pDX, IDC_EDIT_ROUND_WIDTH, m_nRoundWidth);
	DDX_Text(pDX, IDC_EDIT_TEXT_TITLE, m_csTitil);
}


BEGIN_MESSAGE_MAP(CEqmRectangleConfigDlg_BlueWhit, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEqmRectangleConfigDlg_BlueWhit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD_POINT, &CEqmRectangleConfigDlg_BlueWhit::OnBnClickedButtonAddPoint)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_POINT, &CEqmRectangleConfigDlg_BlueWhit::OnBnClickedButtonDeletePoint)
	ON_BN_CLICKED(IDC_CHECK_ROUND, &CEqmRectangleConfigDlg_BlueWhit::OnBnClickedCheckRound)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_POINT, &CEqmRectangleConfigDlg_BlueWhit::OnNMDblclkListPoint)
	ON_BN_CLICKED(IDCANCEL, &CEqmRectangleConfigDlg_BlueWhit::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEqmRectangleConfigDlg_BlueWhit 消息处理程序


BOOL CEqmRectangleConfigDlg_BlueWhit::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if(m_pRectangle == NULL)
	{
		AfxMessageBox(L"获取不到矩形的属性");
		OnBnClickedOk();
	}

	// TODO:  在此添加额外的初始化
	m_csTitil = m_pRectangle->GetTitle();

	m_colorBtn_title.SetColor(m_pRectangle->GetTitleProperties().m_TextColor.ToCOLORREF());
	m_nTitileSize		= m_pRectangle->GetTitleProperties().m_TextSize;

	m_colorBtn_Body.SetColor(m_pRectangle->GetBodyProperties().m_TextColor.ToCOLORREF());
	m_nBodySize			= m_pRectangle->GetBodyProperties().m_TextSize;

	m_colorBtn_paraphrase.SetColor(m_pRectangle->GetParaphraseProperties().m_TextColor.ToCOLORREF());
	m_nParaphraseSize	= m_pRectangle->GetParaphraseProperties().m_TextSize;

	m_bRound = m_pRectangle->GetToRoundRect();
	m_nRoundHeight = m_pRectangle->GetRoundHeight();
	m_nRoundWidth = m_pRectangle->GetRoundWidth();
	if(FALSE == m_bRound)
	{
		GetDlgItem(IDC_EDIT_ROUND_HIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ROUND_WIDTH)->EnableWindow(FALSE);
	}

	m_listCtrl_point.SetBkColor(RGB(235, 235, 235));
	m_listCtrl_point.SetTextColor(RGB(0, 0, 128));
	m_listCtrl_point.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_listCtrl_point.InsertColumn(0,  L"序号", LVCFMT_LEFT);
	m_listCtrl_point.InsertColumn(1,  L"点名", LVCFMT_LEFT);
	m_listCtrl_point.InsertColumn(2,  L"图标ID", LVCFMT_LEFT);
	//自动宽度
	m_listCtrl_point.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_listCtrl_point.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_listCtrl_point.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

	auto iItem = m_pRectangle->GetShowPointList().begin();
	int index = 1;
	for(; iItem!=m_pRectangle->GetShowPointList().end(); iItem++)
	{
		CString csTemp;
		csTemp.Format(L"%d",  index++);
		m_listCtrl_point.InsertItem(m_listCtrl_point.GetItemCount(),  csTemp);

		m_listCtrl_point.SetItemText(m_listCtrl_point.GetItemCount()-1,  1,  iItem->m_csPoint);

		csTemp.Format(L"%d", iItem->m_nPicId);
		m_listCtrl_point.SetItemText(m_listCtrl_point.GetItemCount()-1,  2,  csTemp);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEqmRectangleConfigDlg_BlueWhit::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pRectangle->SetRectangleStyle(1);
	m_pRectangle->SetTitle(m_csTitil);
	m_pRectangle->GetTitleProperties().m_TextColor.SetFromCOLORREF(m_colorBtn_title.GetColor());
	m_pRectangle->GetTitleProperties().m_TextSize = m_nTitileSize;
	m_pRectangle->GetBodyProperties().m_TextColor.SetFromCOLORREF(m_colorBtn_Body.GetColor());
	m_pRectangle->GetBodyProperties().m_TextSize = m_nBodySize;
	m_pRectangle->GetParaphraseProperties().m_TextColor.SetFromCOLORREF(m_colorBtn_paraphrase.GetColor());
	m_pRectangle->GetParaphraseProperties().m_TextSize = m_nParaphraseSize;
	m_pRectangle->SetLineColorRGB(RGB(255,255,255));
	m_pRectangle->SetTransparency(100);
	m_pRectangle->SetFillColorRGB(RGB(255,255,255));
	m_pRectangle->SetFillTransparency(100);
	m_pRectangle->SetFillStyl(true);

	m_pRectangle->SetToRoundRect(m_bRound);
	if(TRUE == m_bRound)
	{
		m_pRectangle->SetRoundHeight(m_nRoundHeight);
		m_pRectangle->SetRoundWidth(m_nRoundWidth);
	}

	m_pRectangle->GetShowPointList().clear();
	for(int i=0; i<m_listCtrl_point.GetItemCount(); i++)
	{
		m_pRectangle->PushShowPointList(
			m_listCtrl_point.GetItemText(i,1), 
			_wtoi(m_listCtrl_point.GetItemText(i,2))	);
	}

	CWnd *pParent = GetParent();
	if(pParent == NULL)
		return;
	pParent->PostMessage(WM_CLOSE);
	CDialogEx::OnOK();
}

void CEqmRectangleConfigDlg_BlueWhit::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *pParent = GetParent();
	if(pParent == NULL)
		return;
	pParent->PostMessage(WM_CLOSE);
	CDialogEx::OnCancel();
}

void CEqmRectangleConfigDlg_BlueWhit::OnBnClickedButtonAddPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	temp.Format(L"%d", m_listCtrl_point.GetItemCount());
	m_listCtrl_point.InsertItem(m_listCtrl_point.GetItemCount(),  temp);
}


void CEqmRectangleConfigDlg_BlueWhit::OnBnClickedButtonDeletePoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	 POSITION pos = m_listCtrl_point.GetFirstSelectedItemPosition();
	 int nIndex = m_listCtrl_point.GetNextSelectedItem(pos);
	 m_listCtrl_point.DeleteItem(nIndex);
}

void CEqmRectangleConfigDlg_BlueWhit::OnBnClickedCheckRound()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_ROUND_HIGHT)->EnableWindow(m_bRound);
	GetDlgItem(IDC_EDIT_ROUND_WIDTH)->EnableWindow(m_bRound);
}


void CEqmRectangleConfigDlg_BlueWhit::OnNMDblclkListPoint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nItem = pNMItemActivate->iItem;
	if (nItem < 0)
	{
		return;
	}

	CRectanglePointInfoDlg dlg(m_pCanvalDlg, this);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString str = m_listCtrl_point.GetItemText(pNMItemActivate->iItem, 2);
	dlg.m_nPicID = _wtoi(str.GetString());
	dlg.m_csPointName =  m_listCtrl_point.GetItemText(pNMItemActivate->iItem, 1);
	
	dlg.SetProjectSqlPointer(pFrame->m_pPrjSqlite);
	if (dlg.DoModal() == IDOK)
	{
		CString strTemp;
		strTemp.Format(L"%d", dlg.m_nPicID);
		m_listCtrl_point.SetItemText(pNMItemActivate->iItem, 2, strTemp);
		m_listCtrl_point.SetItemText(pNMItemActivate->iItem, 1, dlg.m_csPointName);
	}
	*pResult = 0;
}