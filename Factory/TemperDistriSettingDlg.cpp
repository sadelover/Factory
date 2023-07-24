// TemperDistriSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TemperDistriSettingDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "TemperDistriItemSetDlg.h"
#include "CanvasDlg.h"


// CTemperDistriSettingDlg dialog

IMPLEMENT_DYNAMIC(CTemperDistriSettingDlg, CDialogEx)

CTemperDistriSettingDlg::CTemperDistriSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTemperDistriSettingDlg::IDD, pParent)
	, m_nLayer(4)
	, m_pMainFrm(NULL)
	, m_pCanvas(static_cast<CCanvasDlg*>(pParent))
{

}

CTemperDistriSettingDlg::~CTemperDistriSettingDlg()
{
}

void CTemperDistriSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_listShow);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
}


BEGIN_MESSAGE_MAP(CTemperDistriSettingDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW, &CTemperDistriSettingDlg::OnNMDblclkListShow)
	ON_BN_CLICKED(IDC_BTN_ADD, &CTemperDistriSettingDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CTemperDistriSettingDlg::OnBnClickedBtnDel)
END_MESSAGE_MAP()


// CTemperDistriSettingDlg message handlers


BOOL CTemperDistriSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CTemperDistriSettingDlg::OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	TemperDistriInfo info;
	const int nCurSel = pNMItemActivate->iItem;
	try
	{
		info = m_vecTemperDistriInfo.at(nCurSel);
	}
	catch (...)
	{
		MessageBox(_T("选择项时数据异常！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	if (NULL == m_pMainFrm)
	{
		return;
	}
	CTemperDistriItemSetDlg dlg(info.nPicId, info.strPointList, info.strBindPtName, &m_pMainFrm->m_plcpointmap, m_pCanvas);
	if (IDOK == dlg.DoModal())
	{
		int		nPicId = dlg.m_nPicId;
		CString	strPicId;
		strPicId.Format(_T("%d"), nPicId);
		CString	strPointList(dlg.m_strPointList);
		CString	strBindPtName(dlg.m_strBindPtName);


		vector<TemperDistriInfo>::iterator	iter;
		int i = 0;
		for (iter=m_vecTemperDistriInfo.begin(); iter!=m_vecTemperDistriInfo.end(); ++iter,++i)
		{
			if (nCurSel == i)
			{
				iter->nPicId		= nPicId;
				iter->strPointList	= strPointList;
				iter->strBindPtName	= strBindPtName;

				m_listShow.SetItemText(i, 0, strPicId);
				m_listShow.SetItemText(i, 1, strPointList);
				m_listShow.SetItemText(i, 2, strBindPtName);

				break;
			}
		}
	}
}


void CTemperDistriSettingDlg::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pMainFrm)
	{
		return;
	}
	CTemperDistriItemSetDlg dlg(0, _T(""), _T(""), &m_pMainFrm->m_plcpointmap, m_pCanvas);
	if (IDOK == dlg.DoModal())
	{
		TemperDistriInfo info;
		info.nPicId			= dlg.m_nPicId;
		info.strPointList	= dlg.m_strPointList;
		info.strBindPtName	= dlg.m_strBindPtName;
		m_vecTemperDistriInfo.push_back(info);

		int nCount = m_vecTemperDistriInfo.size() - 1;

		CString strPicId;
		strPicId.Format(_T("%d"), dlg.m_nPicId);
		m_listShow.InsertItem(nCount, strPicId);

		m_listShow.SetItemText(nCount, 1, dlg.m_strPointList);
		m_listShow.SetItemText(nCount, 2, dlg.m_strBindPtName);
	}
}


void CTemperDistriSettingDlg::OnBnClickedBtnDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;
	vector<TemperDistriInfo>::const_iterator	iter;
	vector<int>::const_iterator					iterDel;

	pos = m_listShow.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("请选择所需删除项！"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	while (pos != NULL)
	{
		nItem = m_listShow.GetNextSelectedItem(pos);
		vecDel.push_back(nItem);
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		m_listShow.DeleteItem(vecDel.at(i));

		nItem = 0;
		for (iter=m_vecTemperDistriInfo.begin(); iter!=m_vecTemperDistriInfo.end(); ++iter,++nItem)
		{
			if (nItem == vecDel[i])
			{
				m_vecTemperDistriInfo.erase(iter);
				break;
			}
		}
	}
}

void CTemperDistriSettingDlg::InitControls(void)
{
	// combobox
	m_comboLayer.AddString(_T("1层"));
	m_comboLayer.AddString(_T("2层"));
	m_comboLayer.AddString(_T("3层"));
	m_comboLayer.AddString(_T("4层"));
	m_comboLayer.AddString(_T("5层"));
	m_comboLayer.AddString(_T("6层"));
	m_comboLayer.AddString(_T("7层"));
	m_comboLayer.AddString(_T("8层"));
	m_comboLayer.AddString(_T("9层"));
	m_comboLayer.AddString(_T("10层"));

	// list
	m_listShow.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listShow.InsertColumn(0, _T("绑定点"), LVCFMT_LEFT, 80);
	m_listShow.InsertColumn(1, _T("区域点集"), LVCFMT_LEFT, 160);
	m_listShow.InsertColumn(2, _T("绑定点"), LVCFMT_LEFT, 160);
}

void CTemperDistriSettingDlg::InitData(void)
{
	m_comboLayer.SetCurSel(m_nLayer);

	vector<TemperDistriInfo>::const_iterator	iter;
	int i = 0;
	CString strPicId;
	for (iter=m_vecTemperDistriInfo.begin(); iter!=m_vecTemperDistriInfo.end(); ++iter,++i)
	{
		strPicId.Format(_T("%d"), iter->nPicId);
		m_listShow.InsertItem(i, strPicId);

		m_listShow.SetItemText(i, 1, iter->strPointList);
		m_listShow.SetItemText(i, 2, iter->strBindPtName);
	}

	m_pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(m_pMainFrm != NULL);
	if (NULL == m_pMainFrm)
	{
		return;
	}
}

vector<TemperDistriInfo> CTemperDistriSettingDlg::GetTemperDistriInfo(void) const
{
	return m_vecTemperDistriInfo;
}

void CTemperDistriSettingDlg::SetTemperDistriInfo(const vector<TemperDistriInfo> vecTemperDistriInfo)
{
	m_vecTemperDistriInfo = vecTemperDistriInfo;
}
