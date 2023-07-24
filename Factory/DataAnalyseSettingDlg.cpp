// DataAnalyseSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataAnalyseSettingDlg.h"
#include "afxdialogex.h"
#include "DataComConfigDlg.h"
#include "MainFrm.h"
#include "DataAnalyseItemSetDlg.h"


// CDataAnalyseSettingDlg dialog

IMPLEMENT_DYNAMIC(CDataAnalyseSettingDlg, CDialogEx)

CDataAnalyseSettingDlg::CDataAnalyseSettingDlg(CDataPointManager* pDataPointMgr, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataAnalyseSettingDlg::IDD, pParent)
	, m_pDataPointMgr(pDataPointMgr)
	, m_nLayer(4)
	, m_strPtTime(_T(""))
	, m_strPtCount(_T(""))
	, m_strPtStatus(_T(""))
	, m_pMainFrm(NULL)
{

}

CDataAnalyseSettingDlg::~CDataAnalyseSettingDlg()
{
}

void CDataAnalyseSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Control(pDX, IDC_LIST_POINT_INFO, m_listPtInfo);
	DDX_Text(pDX, IDC_EDIT_PT_TIME, m_strPtTime);
	DDX_Text(pDX, IDC_EDIT_PT_COUNT, m_strPtCount);
	DDX_Text(pDX, IDC_EDIT_PT_STATUS, m_strPtStatus);
}


BEGIN_MESSAGE_MAP(CDataAnalyseSettingDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_POINT_INFO, &CDataAnalyseSettingDlg::OnNMDblclkListPointInfo)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDataAnalyseSettingDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDataAnalyseSettingDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_BROWSE_TIME, &CDataAnalyseSettingDlg::OnBnClickedBtnBrowseTime)
	ON_BN_CLICKED(IDC_BTN_BROWSE_COUNT, &CDataAnalyseSettingDlg::OnBnClickedBtnBrowseCount)
	ON_BN_CLICKED(IDC_BTN_BROWSE_STATUS, &CDataAnalyseSettingDlg::OnBnClickedBtnBrowseStatus)
END_MESSAGE_MAP()


// CDataAnalyseSettingDlg message handlers


BOOL CDataAnalyseSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitControls();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDataAnalyseSettingDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CDialogEx::OnOK();
}

void CDataAnalyseSettingDlg::InitControls(void)
{
	// combobox
	m_comboLayer.AddString(_T("1²ã"));
	m_comboLayer.AddString(_T("2²ã"));
	m_comboLayer.AddString(_T("3²ã"));
	m_comboLayer.AddString(_T("4²ã"));
	m_comboLayer.AddString(_T("5²ã"));
	m_comboLayer.AddString(_T("6²ã"));
	m_comboLayer.AddString(_T("7²ã"));
	m_comboLayer.AddString(_T("8²ã"));
	m_comboLayer.AddString(_T("9²ã"));
	m_comboLayer.AddString(_T("10²ã"));


	// list
	m_listPtInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listPtInfo.InsertColumn(0, _T("µãÃû"), LVCFMT_LEFT, 200);
	m_listPtInfo.InsertColumn(1, _T("×¢ÊÍ"), LVCFMT_LEFT, 200);
}


void CDataAnalyseSettingDlg::InitData(void)
{
	m_comboLayer.SetCurSel(m_nLayer);


	vector<DaPointInfo>::const_iterator	iter;
	int i = 0;
	for (iter=m_vecDaPtInfo.begin(); iter!=m_vecDaPtInfo.end(); ++iter,++i)
	{
		m_listPtInfo.InsertItem(i, iter->strPointName);
		m_listPtInfo.SetItemText(i, 1, iter->strPointRemarks);
	}


	m_pMainFrm = static_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	ASSERT(m_pMainFrm != NULL);
	if (NULL == m_pMainFrm)
	{
		return;
	}
}

void CDataAnalyseSettingDlg::OnNMDblclkListPointInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;


	DaPointInfo ptInf;
	const int nCurSel = pNMItemActivate->iItem;
	if (nCurSel < 0)
	{
		return;
	}

	try
	{
		ptInf = m_vecDaPtInfo.at(nCurSel);
	}
	catch (...)
	{
		MessageBox(_T("Ñ¡ÔñÏîÊ±Êý¾ÝÒì³££¡"), _T("´íÎó"), MB_OK|MB_ICONERROR);
		return;
	}

	if (NULL == m_pMainFrm)
	{
		return;
	}
	CDataAnalyseItemSetDlg dlg(&m_pMainFrm->m_plcpointmap, ptInf.strPointName, ptInf.strPointRemarks);
	if (IDOK == dlg.DoModal())
	{
		CString	strPtName(dlg.m_strPtName);
		CString	strRemarks(dlg.m_strRemarks);


		vector<DaPointInfo>::iterator	iter;
		int i = 0;
		for (iter=m_vecDaPtInfo.begin(); iter!=m_vecDaPtInfo.end(); ++iter,++i)
		{
			if (nCurSel == i)
			{
				iter->strPointName		= strPtName;
				iter->strPointRemarks	= strRemarks;

				m_listPtInfo.SetItemText(i, 0, strPtName);
				m_listPtInfo.SetItemText(i, 1, strRemarks);

				break;
			}
		}
	}
}


void CDataAnalyseSettingDlg::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pMainFrm)
	{
		return;
	}
	CDataAnalyseItemSetDlg dlg(&m_pMainFrm->m_plcpointmap, _T(""), _T(""));
	if (IDOK == dlg.DoModal())
	{
		DaPointInfo ptInf;
		ptInf.strPointName = dlg.m_strPtName;
		ptInf.strPointRemarks = dlg.m_strRemarks;
		m_vecDaPtInfo.push_back(ptInf);

		int nCount = m_vecDaPtInfo.size() - 1;
		m_listPtInfo.InsertItem(nCount, dlg.m_strPtName);

		m_listPtInfo.SetItemText(nCount, 1, dlg.m_strRemarks);
	}
}


void CDataAnalyseSettingDlg::OnBnClickedBtnDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = NULL;
	vector<int> vecDel;
	int nItem = 0;
	vector<DaPointInfo>::const_iterator	iter;
	vector<int>::const_iterator			iterDel;

	pos = m_listPtInfo.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		MessageBox(_T("ÇëÑ¡ÔñËùÐèÉ¾³ýÏî£¡"), _T("´íÎó"), MB_OK|MB_ICONERROR);
		return;
	}

	while (pos != NULL)
	{
		nItem = m_listPtInfo.GetNextSelectedItem(pos);
		vecDel.push_back(nItem);
	}
	for (int i=vecDel.size()-1; i>=0; --i)
	{
		m_listPtInfo.DeleteItem(vecDel.at(i));

		nItem = 0;
		for (iter=m_vecDaPtInfo.begin(); iter!=m_vecDaPtInfo.end(); ++iter,++nItem)
		{
			if (nItem == vecDel[i])
			{
				m_vecDaPtInfo.erase(iter);
				break;
			}
		}
	}
}


void CDataAnalyseSettingDlg::OnBnClickedBtnBrowseTime()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pDataPointMgr)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		m_strPtTime = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CDataAnalyseSettingDlg::OnBnClickedBtnBrowseCount()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pDataPointMgr)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		m_strPtCount = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}


void CDataAnalyseSettingDlg::OnBnClickedBtnBrowseStatus()
{
	// TODO: Add your control notification handler code here
	if (NULL == m_pDataPointMgr)
	{
		return;
	}

	CDataComConfigDlg dlg(*m_pDataPointMgr);
	if (IDOK == dlg.DoModal())
	{
		UpdateData(TRUE);
		m_strPtStatus = dlg.m_strPointName;
		UpdateData(FALSE);
	}
}

vector<DaPointInfo>	CDataAnalyseSettingDlg::GetDaPointInfo(void) const
{
	return m_vecDaPtInfo;
}

void CDataAnalyseSettingDlg::SetDaPointInfo(const vector<DaPointInfo>& data)
{
	m_vecDaPtInfo = data;
}
