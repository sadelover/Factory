// EmbededPageSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbededPageSettingDlg.h"
#include "afxdialogex.h"
#include "MetaFileSelectDlg.h"
#include "BindPageDiagnoseDlg.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "MainFrm.h"
// CEmbededPageSettingDlg 对话框

IMPLEMENT_DYNAMIC(CEmbededPageSettingDlg, CDialog)

CEmbededPageSettingDlg::CEmbededPageSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmbededPageSettingDlg::IDD, pParent)
	, m_nPicID(0)
	, m_nInterval(0)
	,m_pEmbededPage(NULL)
	,m_nLayer(0)
{

}

CEmbededPageSettingDlg::~CEmbededPageSettingDlg()
{
}

void CEmbededPageSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EMBEDED_PAGE_PIC, m_nPicID);
	DDX_Control(pDX, IDC_COMBO_EMBEDED_PAGE, m_combo_layer);
	DDX_Control(pDX, IDC_LIST_EMBEDED_PAGE, m_list_embeded_page_id);
	DDX_Text(pDX, IDC_EDIT_EMBEDED_PAGE_CHANGE_INTERVAL, m_nInterval);
}


BEGIN_MESSAGE_MAP(CEmbededPageSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_EMBEDED_PAGE_PIC_SELECT, &CEmbededPageSettingDlg::OnBnClickedButtonEmbededPagePicSelect)
	ON_BN_CLICKED(IDC_BUTTON_EMBEDED_PAGE_ADD, &CEmbededPageSettingDlg::OnBnClickedButtonEmbededPageAdd)
	ON_BN_CLICKED(IDC_BUTTON_EMBEDED_PAGE_DELETE, &CEmbededPageSettingDlg::OnBnClickedButtonEmbededPageDelete)
	ON_BN_CLICKED(IDOK, &CEmbededPageSettingDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_EMBEDED_PAGE, &CEmbededPageSettingDlg::OnNMDblclkListEmbededPage)
END_MESSAGE_MAP()


// CEmbededPageSettingDlg 消息处理程序


void CEmbededPageSettingDlg::OnBnClickedButtonEmbededPagePicSelect()
{
	UpdateData(TRUE);
	CMetaFileSelectDlg dlg;
	dlg.SetWndType(e_picture);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
	if (dlg.DoModal() == IDOK)
	{
		m_nPicID = dlg.m_nID;
		pFrame->m_nSelectPicID = m_nPicID;
	}
	UpdateData(FALSE);
}


void CEmbededPageSettingDlg::OnBnClickedButtonEmbededPageAdd()
{
	const int nCount = m_list_embeded_page_id.GetItemCount();
	m_list_embeded_page_id.InsertItem(nCount,L"");
	m_list_embeded_page_id.SetItemText(nCount,0,L"-1");
	m_list_embeded_page_id.SetItemText(nCount,1,L"PageName");
}


void CEmbededPageSettingDlg::OnBnClickedButtonEmbededPageDelete()
{
	POSITION pos = m_list_embeded_page_id.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选中要删除的行!");
		return;
	}
	const int nItem = (int)pos;
	m_list_embeded_page_id.DeleteItem(nItem-1);
}


void CEmbededPageSettingDlg::OnBnClickedOk()
{
	UpdateData();
	if (0 == m_nPicID)
	{
		MessageBox(_T("请先选择图片！"), _T("提示"), MB_OK|MB_ICONASTERISK);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_EMBEDED_PAGE_PIC);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}
	UpdateData(FALSE);
	m_PageInfoVec.clear();
	if (m_pEmbededPage)
	{
		m_pEmbededPage->ClearPageList();
	}
	const int nCount = m_list_embeded_page_id.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		const CString strPageID = m_list_embeded_page_id.GetItemText(i,0);
		const CString strPageName = m_list_embeded_page_id.GetItemText(i,1);
		const int nPageID = _wtoi(strPageID.GetString());
		PageIDName st;
		st.nPageID = nPageID;
		st.strPageName = strPageName;
		m_PageInfoVec.push_back(st);
		if (m_pEmbededPage)
		{
			m_pEmbededPage->InsertPageList(nPageID, strPageName);
		}
	}
	m_nLayer = m_combo_layer.GetCurSel();
	CDialog::OnOK();
}


void CEmbededPageSettingDlg::OnNMDblclkListEmbededPage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem < 0)
	{
		return;
	}
	CBindPageDiagnoseDlg dlg;
	dlg.m_strPageID = m_list_embeded_page_id.GetItemText(pNMItemActivate->iItem,0);
	dlg.m_str_page_diagnose = m_list_embeded_page_id.GetItemText(pNMItemActivate->iItem,1);
	if (dlg.DoModal() == IDOK)
	{
		m_list_embeded_page_id.SetItemText(pNMItemActivate->iItem,0,dlg.m_strPageID);
		m_list_embeded_page_id.SetItemText(pNMItemActivate->iItem,1,dlg.m_str_page_diagnose);
	}
	*pResult = 0;
}


BOOL CEmbededPageSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_embeded_page_id.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_list_embeded_page_id.InsertColumn(0,L"页面ID",LVCFMT_LEFT,150);
	m_list_embeded_page_id.InsertColumn(1,L"页面名称",LVCFMT_LEFT,300);

	m_combo_layer.AddString(L"1层");
	m_combo_layer.AddString(L"2层");
	m_combo_layer.AddString(L"3层");
	m_combo_layer.AddString(L"4层");
	m_combo_layer.AddString(L"5层");
	m_combo_layer.AddString(L"6层");
	m_combo_layer.AddString(L"7层");
	m_combo_layer.AddString(L"8层");
	m_combo_layer.AddString(L"9层");
	m_combo_layer.AddString(L"10层");
	m_list_embeded_page_id.DeleteAllItems();
	if (m_pEmbededPage)
	{
		const vector<PageIDName>& vec = m_pEmbededPage->GetPageList();
		for(size_t i=0;i<vec.size();++i)
		{
			m_list_embeded_page_id.InsertItem(i,L"");
			CString str;
			str.Format(L"%d",vec[i].nPageID);
			m_list_embeded_page_id.SetItemText(i,0,str);
			m_list_embeded_page_id.SetItemText(i,1,vec[i].strPageName);
		}
	}

	m_combo_layer.SetCurSel(m_nLayer);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
