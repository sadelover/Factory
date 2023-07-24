// MetaFileInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MainFrm.h"
#include "MetaFileInfoDlg.h"
#include "DataPoint/DataPointEntry.h"
#include "DataComConfigDlg.h"
#include "Tools/CustomTools/CustomTools.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "StateChangeDlg.h"
#include "VPointSettingDlg.h"
#include "BEOP-DesignerView.h"
#include "PageDlg.h"
#include "CanvasDlg.h"
#include "BindPageDiagnoseDlg.h"
#include <vector>
#include "MetaFileSelectDlg.h"
using namespace std;

// CMetaFileInfoDlg 对话框
IMPLEMENT_DYNAMIC(CMetaFileInfoDlg, CDialog)

CMetaFileInfoDlg::CMetaFileInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMetaFileInfoDlg::IDD, pParent),
	m_pDevice(NULL)
	, m_pCanvalDlg((CCanvasDlg*)pParent)
	, m_strOutsideLink(_T(""))
	, m_nBindType(0)
{
	m_nDeviceType = 11;
	m_nMouseEventType = 3;
	m_nDeviceIDNum = 0;
	m_nLayer = 9;
	m_nInterval = 0;
	m_nValue = 0;
}

CMetaFileInfoDlg::~CMetaFileInfoDlg()
{
}

void CMetaFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_METAFILE_NAME, m_edit_name);
	DDX_Control(pDX, IDC_EDIT_BIND_POINT_NAME, m_edit_point_name);
	DDX_Control(pDX, IDC_LIST_DEFINE, m_listCtrl);
	DDX_Control(pDX, IDC_EDIT_BIND_POINT_NAME_SETTING, m_edit_setting_point);
	DDX_Control(pDX, IDC_COMBO_DEVTYPE, m_combo_device_type);
	DDX_Control(pDX, IDC_COMBO_MOUSE_EVENT, m_combo_mouse_event_type);
	DDX_Control(pDX, IDC_EDIT_ID, m_nDeviceID);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_combo_layer);
	DDX_Control(pDX, IDC_COMBO_PAGE_ID, m_combo_page);
	DDX_Control(pDX, IDC_LIST_DIAGNOSE_PAGE, m_list_diagnose);
	DDX_Text(pDX, IDC_EDIT_LINK_OUTSIDE, m_strOutsideLink);
	DDX_CBIndex(pDX, IDC_COMBO_BINDPOINTTYPE, m_nBindType);
}


BEGIN_MESSAGE_MAP(CMetaFileInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_POINT_SELECT, &CMetaFileInfoDlg::OnBnClickedButtonPointSelect)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMetaFileInfoDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTONDELETE, &CMetaFileInfoDlg::OnBnClickedButtondelete)
	ON_BN_CLICKED(IDOK, &CMetaFileInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_POINT_SELECT_SETTING, &CMetaFileInfoDlg::OnBnClickedButtonPointSelectSetting)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEFINE, &CMetaFileInfoDlg::OnNMDblclkListDefine)
	ON_BN_CLICKED(IDC_BUTTON_POINT_CLEAR, &CMetaFileInfoDlg::OnBnClickedButtonPointClear)
	ON_CBN_SELCHANGE(IDC_COMBO_MOUSE_EVENT, &CMetaFileInfoDlg::OnCbnSelchangeComboMouseEvent)
	ON_BN_CLICKED(IDC_BUTTON_VPOINT, &CMetaFileInfoDlg::OnBnClickedButtonVpoint)
	ON_BN_CLICKED(IDC_BUTTON_PIC_RESTORE, &CMetaFileInfoDlg::OnBnClickedButtonPicRestore)
	ON_BN_CLICKED(IDC_BUTTON_ADD_DIAGNOSE, &CMetaFileInfoDlg::OnBnClickedButtonAddDiagnose)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_DIAGNOSE, &CMetaFileInfoDlg::OnBnClickedButtonDeleteDiagnose)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIAGNOSE_PAGE, &CMetaFileInfoDlg::OnNMDblclkListDiagnosePage)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, &CMetaFileInfoDlg::OnBnClickedButtonReplace)
	ON_CBN_SELCHANGE(IDC_COMBO_BINDPOINTTYPE, &CMetaFileInfoDlg::OnCbnSelchangeComboBindpointtype)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER, &CMetaFileInfoDlg::OnCbnSelchangeComboLayer)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVTYPE, &CMetaFileInfoDlg::OnCbnSelchangeComboDevtype)
END_MESSAGE_MAP()

void CMetaFileInfoDlg::SetName( const CString strName )
{
	m_strName = strName;
}


// CMetaFileInfoDlg 消息处理程序


BOOL CMetaFileInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化

	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CString csPicID = L"";
	csPicID.Format(L"%d", m_pDevice->GetPicID());
	GetDlgItem(IDC_EDIT_PICTURE_ID)->SetWindowTextW(csPicID);
	InitCombo();

	m_listCtrl.InsertColumn(0,L"值",LVCFMT_LEFT,50,0);
	m_listCtrl.InsertColumn(1,L"图片(或动画)ID",LVCFMT_LEFT,300,1);
	m_listCtrl.InsertColumn(2,L"帧数",LVCFMT_LEFT,50,2);
	m_listCtrl.InsertColumn(3,L"动画间隔(ms)",LVCFMT_LEFT,150,3);

	m_edit_name.SetWindowTextW(m_strName);
	m_edit_point_name.SetWindowTextW(m_strPointName);
	GetDlgItem(IDC_EDIT_SHOW_NAME)->SetWindowTextW(m_strShowName);

	vector<BindInfo> vec = m_pDevice->m_vecBindInfo;
	for (size_t i=0;i<vec.size();++i)
	{
		m_listCtrl.InsertItem(i,L"");
		CString str;
		str.Format(L"%d", vec[i].nValue);
		m_listCtrl.SetItemText(i,0,str);
		str.Format(L"%d", vec[i].nPicID);
		m_listCtrl.SetItemText(i,1,str);
		str.Format(L"%d", vec[i].nFrame);
		m_listCtrl.SetItemText(i,2,str);
		str.Format(L"%d", vec[i].nInterval);
		m_listCtrl.SetItemText(i,3,str);
	}

	m_edit_setting_point.SetWindowTextW(m_strSettingPointName);
	m_combo_device_type.SetCurSel(m_nDeviceType);
	m_combo_mouse_event_type.SetCurSel(m_nMouseEventType);
	CString strValue;
	strValue.Format(L"%d",m_nValue);
	GetDlgItem(IDC_EDIT_SETTING_VALUE)->SetWindowTextW(strValue);
	if (m_nMouseEventType == enumChangeValueWnd)
	{
		GetDlgItem(IDC_EDIT_SETTING_VALUE)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SETTING_VALUE)->EnableWindow(0);
	}
	if (m_combo_mouse_event_type.GetCurSel() == enumJumpPage)
	{
		GetDlgItem(IDC_COMBO_PAGE_ID)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PAGE_ID)->EnableWindow(0);
	}
	GetDlgItem(IDC_EDIT_SHOW_NAME)->SetWindowTextW(m_strShowName);
	CString strDeviceID;
	strDeviceID.Format(L"%d", m_nDeviceIDNum);
	m_nDeviceID.SetWindowTextW(strDeviceID);
	m_combo_layer.SetCurSel(m_nLayer);

	if ( m_pDevice->GetEqmType() != EQMDRAW_METAFILE)
	{
		DefaultDeviceInit();
	}
	GetDlgItem(IDC_EDIT_ID)->EnableWindow(FALSE);
	InitComboPage();

	InitDiagnoseList();
	{
		const vector<PageIDName>& vec = m_pDevice->GetDiagnosePageVec();
		for(size_t i=0;i<vec.size();++i)
		{
			m_list_diagnose.InsertItem(i,L"");
			CString str;
			str.Format(L"%d",vec[i].nPageID);
			m_list_diagnose.SetItemText(i,0,str);
			m_list_diagnose.SetItemText(i,1,vec[i].strPageName);
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMetaFileInfoDlg::SetPointMap( CDataPointManager* pointmap )
{
	m_refpointmap = pointmap;
}


void CMetaFileInfoDlg::OnBnClickedButtonPointSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataComConfigDlg dlg(*m_refpointmap);
	if (dlg.DoModal() == IDOK)
	{
		bool bIsNum = false;
		m_strPointName = dlg.m_strPointName;
		m_edit_point_name.SetWindowTextW(m_strPointName);
		CString strID = m_strPointName.Right(2);
		for (int i=0;i<strID.GetLength();++i)
		{
			wchar_t wc = strID.GetAt(i);	
			if((wc>=L'0') && (wc<=L'9'))
			{
				bIsNum = true;
			}
			else
			{
				bIsNum = false;
			}
		}
		if (bIsNum)
		{
			GetDlgItem(IDC_EDIT_ID)->SetWindowTextW(strID);
		}
		else
		{
			GetDlgItem(IDC_EDIT_ID)->SetWindowTextW(L"0");
		}
	}
}

void CMetaFileInfoDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	const int nCount = m_listCtrl.GetItemCount();
	m_listCtrl.InsertItem(nCount,L"");
	CString str;
	str.Format(L"%d", nCount);
	m_listCtrl.SetItemText(nCount,0,str);
	m_listCtrl.SetItemText(nCount,1,L"请选择图片或动画");
	m_listCtrl.SetItemText(nCount,2,L"1");
	m_listCtrl.SetItemText(nCount,3,L"0");
}


void CMetaFileInfoDlg::OnBnClickedButtondelete()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选中要删除的行!");
		return;
	}
	const int nItem = (int)pos;
	m_listCtrl.DeleteItem(nItem-1);
}

void CMetaFileInfoDlg::ReCalListCtrl()
{
	const int nCount = m_listCtrl.GetItemCount();
	for (int i=0;i<nCount;++i)
	{
		CString str;
		str.Format(L"%d",i);
		m_listCtrl.SetItemText(i,0,str);
	}
}

void CMetaFileInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strPointName;
	CString strParam10 = _T("");
	CString	strName;
	m_edit_name.GetWindowTextW(strName);

	CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame != NULL);
	if (pMainFrame != NULL)
	{
		int nLen = strName.GetLength();
		if (nLen > pMainFrame->m_nItemNameLen)
		{
			CString strErr;
			strErr.Format(_T("名称长度不能大于%d！"), pMainFrame->m_nItemNameLen);
			MessageBox(strErr, _T("错误"), MB_OK|MB_ICONERROR);
			m_edit_name.SetSel(0, -1);
			m_edit_name.SetFocus();
			return;
		}
	}
	m_strName = strName;

	m_edit_point_name.GetWindowTextW(strPointName);
	m_nDeviceType = m_combo_device_type.GetCurSel();
	m_nMouseEventType = m_combo_mouse_event_type.GetCurSel();
	m_nLayer = m_combo_layer.GetCurSel();

	CString strDeviceID;
	m_nDeviceID.GetWindowTextW(strDeviceID);

	if (strDeviceID.GetLength()>0)
	{
		m_nDeviceIDNum = _wtoi(strDeviceID.GetString());
	}

	for (int i=0;i<m_listCtrl.GetItemCount();++i)
	{
		CString strValue = m_listCtrl.GetItemText(i,0);
		CString strPicID = m_listCtrl.GetItemText(i,1);
		CString strFrame = m_listCtrl.GetItemText(i,2);
		CString strInterval = m_listCtrl.GetItemText(i,3);
		strParam10 += strValue+L","+strPicID+L","+strFrame+L","+strInterval;
		if (i != m_listCtrl.GetItemCount()-1)
		{
			strParam10 += L"|";
		}
	}	
	if (m_pDevice)
	{
		m_pDevice->SetEqmName(m_strName.GetString());
		m_pDevice->SetBindString(strParam10.GetString());
		m_pDevice->m_vecBindInfo.clear();
		if (strParam10.GetLength()>0)
		{
			m_pDevice->ParseParam10(strParam10);
		}
		m_pDevice->SetPhysicalID(strPointName.GetString());
		m_pDevice->SetBindType(m_nBindType);
		CString str;
		m_edit_setting_point.GetWindowTextW(str);
		CString strContent;
		m_combo_page.GetLBText(m_combo_page.GetCurSel(),strContent);
		ASSERT(strContent.GetLength()>0);

		CEqmDrawPage* pPage = pMainFrame->m_DrawContext.GetProject()->GetPage(strContent.GetString());
		CString strValue;
		GetDlgItem(IDC_EDIT_SETTING_VALUE)->GetWindowText(strValue);
		CString strShowName;
		GetDlgItem(IDC_EDIT_SHOW_NAME)->GetWindowTextW(strShowName);
		if (strValue.GetLength()>0)
		{
			m_nValue = _wtoi(strValue);
		}
		else
		{
			m_nValue = -99;
		}
		if (pPage)
		{
			m_pDevice->SetEventInfo(str, (DeviceTypeForMeta)m_nDeviceType, (MouseEventTypeForMeta)m_nMouseEventType,m_nDeviceIDNum,pPage->GetID(),m_nValue,strShowName,m_strOutsideLink);
		}
		else
		{
			m_pDevice->SetEventInfo(str, (DeviceTypeForMeta)m_nDeviceType, (MouseEventTypeForMeta)m_nMouseEventType,m_nDeviceIDNum,-1,m_nValue,strShowName,m_strOutsideLink);
		}
		m_pDevice->SetLayer((UINT)m_nLayer);
		{
			m_pDevice->ClearDiagnoseVec();
			const int nCount = m_list_diagnose.GetItemCount();
			for (int i=0;i<nCount;++i)
			{
				CString strID = m_list_diagnose.GetItemText(i,0);
				m_pDevice->InsertDiagnoseVec(_wtoi(strID.GetString()),m_list_diagnose.GetItemText(i,1));
			}
		}
	}
	OnOK();
}

void CMetaFileInfoDlg::OnBnClickedButtonPointSelectSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataComConfigDlg dlg(*m_refpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_strSettingPointName = dlg.m_strPointName;
		m_edit_setting_point.SetWindowTextW(m_strSettingPointName);
	}
}

void CMetaFileInfoDlg::InitCombo()
{
	m_combo_device_type.AddString(L"冷机");
	m_combo_device_type.AddString(L"水泵(默认)");
	m_combo_device_type.AddString(L"冷却泵");
	m_combo_device_type.AddString(L"一次冷冻泵");
	m_combo_device_type.AddString(L"二次冷冻泵");
	m_combo_device_type.AddString(L"一次热水泵");
	m_combo_device_type.AddString(L"二次热水泵");
	m_combo_device_type.AddString(L"冷机阀门");
	m_combo_device_type.AddString(L"冷却塔阀门");
	m_combo_device_type.AddString(L"地源侧阀门");
	m_combo_device_type.AddString(L"冷却塔");
	m_combo_device_type.AddString(L"不指定");
	m_combo_device_type.AddString(L"普通阀门");
	m_combo_device_type.SetCurSel(enumNull);

	m_combo_mouse_event_type.AddString(L"设备属性");
	m_combo_mouse_event_type.AddString(L"点设置");
	m_combo_mouse_event_type.AddString(L"修改值");
	m_combo_mouse_event_type.AddString(L"不指定");
	m_combo_mouse_event_type.AddString(L"页面跳转");
	m_combo_mouse_event_type.AddString(L"外链跳转");
	m_combo_mouse_event_type.SetCurSel(enumNullWnd);

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
	m_combo_layer.SetCurSel(E_LAYER_10);
}

void CMetaFileInfoDlg::OnNMDblclkListDefine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem < 0)
	{
		return;
	}
	CStateChangeDlg dlg(m_pCanvalDlg, this);
	CString str;
	str = m_listCtrl.GetItemText(pNMItemActivate->iItem, 0);
	dlg.m_nValue = _wtoi(str.GetString());
	str = m_listCtrl.GetItemText(pNMItemActivate->iItem, 2);
	dlg.m_nFrame = _wtoi(str.GetString());
	str = m_listCtrl.GetItemText(pNMItemActivate->iItem, 1);
	dlg.m_nPicID = _wtoi(str.GetString());
	str = m_listCtrl.GetItemText(pNMItemActivate->iItem, 3);
	dlg.m_nInterval = _wtoi(str.GetString());
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	dlg.SetProjectSqlPointer(pFrame->m_pPrjSqlite);
	if (dlg.DoModal() == IDOK)
	{
		CString strTemp;
		strTemp.Format(L"%d", dlg.m_nValue);
		m_listCtrl.SetItemText(pNMItemActivate->iItem, 0, strTemp);
		strTemp.Format(L"%d", dlg.m_nFrame);
		m_listCtrl.SetItemText(pNMItemActivate->iItem, 2, strTemp);
		strTemp.Format(L"%d", dlg.m_nPicID);
		m_listCtrl.SetItemText(pNMItemActivate->iItem, 1, strTemp);
		strTemp.Format(L"%d", dlg.m_nInterval);
		m_listCtrl.SetItemText(pNMItemActivate->iItem, 3, strTemp);
	}
	
	*pResult = 0;
}

void CMetaFileInfoDlg::DefaultDeviceInit()
{
	SetWindowTextW(L"模板图元");
	if (m_strPointName.GetLength() == 0)
	{
		m_edit_point_name.SetWindowTextW(L"请点击右边的按钮选择绑定的状态点!");
	}

	const EQMDRAWTYPE type = m_pDevice->GetEqmType();
	m_listCtrl.DeleteAllItems();
	m_listCtrl.InsertItem(0,L"0");//关闭
	m_listCtrl.InsertItem(1,L"1");//运行
	m_listCtrl.InsertItem(2,L"2");//动作中
	switch (type)
	{
	case EQMDRAW_CHILLER:
		if(m_pDevice->GetPicID() == 20)
		{
			m_listCtrl.SetItemText(0,1,L"37");
			m_listCtrl.SetItemText(1,1,L"20");
			m_listCtrl.SetItemText(2,1,L"");
		}
		if (m_pDevice->GetPicID() == 21)
		{
			m_listCtrl.SetItemText(0,1,L"38");
			m_listCtrl.SetItemText(1,1,L"21");
			m_listCtrl.SetItemText(2,1,L"");
		}
		if(m_pDevice->GetPicID() == 22)
		{
			m_listCtrl.SetItemText(0,1,L"39");
			m_listCtrl.SetItemText(1,1,L"22");
			m_listCtrl.SetItemText(2,1,L"");
		}
		if(m_pDevice->GetPicID() == 23)
		{
			m_listCtrl.SetItemText(0,1,L"40");
			m_listCtrl.SetItemText(1,1,L"23");
			m_listCtrl.SetItemText(2,1,L"");
		}
		if(m_pDevice->GetPicID() == 24)
		{
			m_listCtrl.SetItemText(0,1,L"41");
			m_listCtrl.SetItemText(1,1,L"24");
			m_listCtrl.SetItemText(2,1,L"");
		}
		if(m_pDevice->GetPicID() == 25)
		{
			m_listCtrl.SetItemText(0,1,L"42");
			m_listCtrl.SetItemText(1,1,L"25");
			m_listCtrl.SetItemText(2,1,L"");
		}
		if(m_pDevice->GetPicID() == 26)
		{
			m_listCtrl.SetItemText(0,1,L"43");
			m_listCtrl.SetItemText(1,1,L"26");
			m_listCtrl.SetItemText(2,1,L"");
		}
		break;
	case EQMDRAW_PUMP:
		if(m_pDevice->GetPicID() == 27)
		{
			m_listCtrl.SetItemText(0,1,L"44");
			m_listCtrl.SetItemText(1,1,L"27");
			m_listCtrl.SetItemText(2,1,L"");
		}
		break;
	case EQMDRAW_COOLINGTOWER:
		if(m_pDevice->GetPicID() == 28)
		{
			m_listCtrl.SetItemText(0,1,L"45");
			m_listCtrl.SetItemText(1,1,L"28");
			m_listCtrl.SetItemText(2,1,L"");
		}
		break;
	case EQMDRAW_VALVE:
		if(m_pDevice->GetPicID() == 29)
		{
			m_listCtrl.SetItemText(0,1,L"46");
			m_listCtrl.SetItemText(1,1,L"29");
			m_listCtrl.SetItemText(2,1,L"49");
		}
		if(m_pDevice->GetPicID() == 30)
		{
			m_listCtrl.SetItemText(0,1,L"47");
			m_listCtrl.SetItemText(1,1,L"30");
			m_listCtrl.SetItemText(2,1,L"50");
		}
		if(m_pDevice->GetPicID() == 31)
		{
			m_listCtrl.SetItemText(0,1,L"48");
			m_listCtrl.SetItemText(1,1,L"31");
			m_listCtrl.SetItemText(2,1,L"51");
		}
		break;
	default:
		break;
	}
	m_listCtrl.SetItemText(0,2,L"1");
	m_listCtrl.SetItemText(1,2,L"1");
	m_listCtrl.SetItemText(2,2,L"1");
	m_listCtrl.SetItemText(0,3,L"0");
	m_listCtrl.SetItemText(1,3,L"0");
	m_listCtrl.SetItemText(2,3,L"0");
	m_listCtrl.EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_BIND_POINT_NAME_SETTING)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_POINT_SELECT_SETTING)->EnableWindow(FALSE);

	m_combo_mouse_event_type.SetCurSel(0);
	m_combo_mouse_event_type.EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
}

void CMetaFileInfoDlg::InitComboPage()
{
	m_combo_page.ResetContent();
	m_combo_page.AddString(L"不指定");
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CEqmDrawProject* pProject = pMainFrame->m_DrawContext.GetProject();
	ASSERT(pProject);
	vector<CEqmDrawPage*> pageList = pProject->GetPageList();
	const size_t size = pageList.size();
	for (size_t i=0;i<size;++i)
	{
		if (pageList[i])
		{
			CString strPageName = pageList[i]->GetPageName().c_str();
			m_combo_page.AddString(strPageName);
		}
	}

	EventInfo info = m_pDevice->GetEventInfo();
	const int nPageID = info.nPageID;
	CEqmDrawPage* pPage = pMainFrame->m_DrawContext.GetProject()->GetPageByID(nPageID);
	if (pPage)
	{
		CString strPageName = pPage->GetPageName().c_str();
		ASSERT(strPageName.GetLength()>0);
		for (int j=0;j<m_combo_page.GetCount();++j)
		{
			CString strContent;
			m_combo_page.GetLBText(j,strContent);
			ASSERT(strContent.GetLength()>0);
			if (strContent == strPageName)
			{
				m_combo_page.SetCurSel(j);
				return;
			}
		}
	}
	else
	{
		m_combo_page.SetCurSel(0);
	}
}


void CMetaFileInfoDlg::OnBnClickedButtonPointClear()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_BIND_POINT_NAME)->SetWindowText(L"");
	UpdateData(FALSE);
}


void CMetaFileInfoDlg::OnCbnSelchangeComboMouseEvent()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_combo_mouse_event_type.GetCurSel() == enumChangeValueWnd)
	{
		GetDlgItem(IDC_EDIT_SETTING_VALUE)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SETTING_VALUE)->EnableWindow(0);
	}
	if (m_combo_mouse_event_type.GetCurSel() == enumJumpPage)
	{
		GetDlgItem(IDC_COMBO_PAGE_ID)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PAGE_ID)->EnableWindow(0);
	}

	if(m_combo_mouse_event_type.GetCurSel()== enumJumpOutsideLink)
	{
		GetDlgItem(IDC_EDIT_LINK_OUTSIDE)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_LINK_OUTSIDE)->EnableWindow(0);
	}
	

}

void CMetaFileInfoDlg::OnBnClickedButtonVpoint()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CMetaFileInfoDlg::OnBnClickedButtonPicRestore()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pDevice->SetWidth(m_pDevice->GetBitmap()->GetWidth());
	m_pDevice->SetHeight(m_pDevice->GetBitmap()->GetHeight());
	CBEOPDesignerView* pView = (CBEOPDesignerView*)pMainFrame->GetActiveView();
	pView->m_PageDlg->m_dlgCanvas.Invalidate(FALSE);
}

void CMetaFileInfoDlg::SetDevice( CEqmDrawDevice* pDevice )
{
	if (pDevice)
	{
		m_pDevice = pDevice;
	}
}


void CMetaFileInfoDlg::OnBnClickedButtonAddDiagnose()
{
	const int nCount = m_list_diagnose.GetItemCount();
	m_list_diagnose.InsertItem(nCount,L"");
	m_list_diagnose.SetItemText(nCount,0,L"-1");
	m_list_diagnose.SetItemText(nCount,1,L"PageName");
}


void CMetaFileInfoDlg::OnBnClickedButtonDeleteDiagnose()
{
	POSITION pos = m_list_diagnose.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选中要删除的行!");
		return;
	}
	const int nItem = (int)pos;
	m_list_diagnose.DeleteItem(nItem-1);
}

void CMetaFileInfoDlg::InitDiagnoseList()
{
	m_list_diagnose.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_list_diagnose.InsertColumn(0,L"页面ID",LVCFMT_LEFT,150);
	m_list_diagnose.InsertColumn(1,L"页面名称",LVCFMT_LEFT,300);
}


void CMetaFileInfoDlg::OnNMDblclkListDiagnosePage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem < 0)
	{
		return;
	}
	CBindPageDiagnoseDlg dlg;
	dlg.m_strPageID = m_list_diagnose.GetItemText(pNMItemActivate->iItem,0);
	dlg.m_str_page_diagnose = m_list_diagnose.GetItemText(pNMItemActivate->iItem,1);
	if (dlg.DoModal() == IDOK)
	{
		m_list_diagnose.SetItemText(pNMItemActivate->iItem,0,dlg.m_strPageID);
		m_list_diagnose.SetItemText(pNMItemActivate->iItem,1,dlg.m_str_page_diagnose);
	}
	*pResult = 0;
}


void CMetaFileInfoDlg::OnBnClickedButtonReplace()
{
	// TODO: Add your control notification handler code here
	CMetaFileSelectDlg dlg;
	dlg.SetWndType(e_picture);
	dlg.SetCurrentPicID(m_pDevice->GetPicID());
	if (dlg.DoModal() == IDOK)
	{	
		int nPicId = 0;
		m_pCanvalDlg->InsertImageIntoOpenS3db(dlg.m_nID,nPicId);

		if (dlg.m_nID != m_pDevice->GetPicID())
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nID, false);
			if(pBitmap)
			{
				m_pDevice->SetWidth(pBitmap->GetWidth());
				m_pDevice->SetHeight(pBitmap->GetHeight());
				m_pDevice->SetBitmap(pBitmap);
				m_pDevice->SetPicID(nPicId);
			}
		}
	}
	UpdateData(FALSE);
}

void CMetaFileInfoDlg::OnCbnSelchangeComboBindpointtype()
{
	// TODO: Add your control notification handler code here

}


void CMetaFileInfoDlg::OnCbnSelchangeComboLayer()
{
	// TODO: Add your control notification handler code here
}


void CMetaFileInfoDlg::OnCbnSelchangeComboDevtype()
{
	// TODO: Add your control notification handler code here
}
