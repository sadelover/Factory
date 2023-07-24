
#include "stdafx.h"

#include "PropertiesWnd.h"

#include "MainFrm.h"
#include "BEOP-DesignerView.h"
#include "BEOP-designer.h"
#include "PageDlg.h"
#include "../Tools/CustomTools/CustomTools.h"

#include "../ComponentDraw/EqmDrawBase.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "../ComponentDraw/EqmDrawText.h"
#include "../ComponentDraw/EqmDrawPipe.h"
#include "../ComponentDraw/EqmDrawButton.h"
#include "../ComponentDraw/EqmDrawBend.h"
#include "../ComponentDraw/EqmDrawDashBoard.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphic.h"
#include "../ComponentDraw/EqmDrawTimeSelect.h"
#include "../ComponentDraw/EqmDrawMapNavigate.h"
#include "../ComponentDraw/EqmDrawBarGraphic.h"
#include "../ComponentDraw/EqmDrawPieGraphic.h"
#include "../ComponentDraw/EqmDrawLineGraphic.h"
#include "../ComponentDraw/EqmDrawEnergySaveRoi.h"
#include "../ComponentDraw/EqmDrawHistoryTrend.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "../ComponentDraw/EqmDrawShellExecute.h"
#include "../ComponentDraw/EqmDrawAreaChange.h"
#include "../ComponentDraw/EqmDrawLiquidLevel.h"
#include "../ComponentDraw/EqmDrawTimeBar.h"
#include "../ComponentDraw/EqmDrawProgressBar.h"
#include "../ComponentDraw/EqmDrawDataReport.h"
#include "../ComponentDraw/EqmDrawPlaneTempeDistr.h"
#include "../ComponentDraw/EqmDrawDottedLineFrm.h"
#include "../ComponentDraw/EqmDrawGeneralComponent.h"
#include "../ComponentDraw/EqmDrawRectangle.h"
#include "../ComponentDraw/EqmDrawStraightLine.h"
#include "../ComponentDraw/EqmDrawPolygon.h"

#include "DataPoint/DataPointManager.h"
#include "DataComConfigDlg.h"
#include "MetaFileSelectDlg.h"
#include "CanvasDlg.h"
#include "BindPageDiagnoseDlg.h"
#include "StateChangeDlg.h"
#include "SelectDataComId.h"
#include "RelationItemSettingDlg.h"
#include "MapNavigateItemSetDlg.h"
#include "DiagnoseItemPropertySettingDlg.h"
#include <propvarutil.h>
#include "TimePickDlg.h"
#include "ChartItemPropertyDlg.h"
#include "HistoryTrendItemSetDlg.h"
#include "BindPageDiagnoseDlg.h"
#include "ScrawlItemSetDlg.h"
#include "PathDialog.h"
#include "AddAreaDlg.h"
#include "CommandManager.h"
#include "NavigateWindowItemSetDlg.h"
#include "TemperDistriItemSetDlg.h"
#include "LevelRulerItemSetDlg.h"
#include "LevelRulerSectionSetDlg.h"
#include "BindPropertySettingDlg.h"
#include "DataAnalyseItemSetDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

char digits[]=  
{  
	'0', '1', '2', '3', '4', '5',  
	'6', '7', '8', '9', 'a', 'b',  
	'c', 'd', 'e', 'f', 'g', 'h',  
	'i', 'j', 'k', 'l', 'm', 'n',  
	'o', 'p', 'q', 'r', 's', 't',  
	'u', 'v', 'w', 'x', 'y', 'z'  
};  

std::string ToUnsignedString(int i, int shift)  
{  
	char  buf[32];  
	char * pBuf = buf;  
	int charPos = 32;  
	int radix = 1<<shift;  
	int mask = radix - 1;  
	do  
	{  
		pBuf[--charPos] = digits[i&mask];  
		i = i>>shift;   
	}while(i != 0);  

	std::string str;  
	int strLen = 32 - charPos;  
	pBuf = pBuf+charPos;  
	while(strLen)  
	{  
		str.push_back(*pBuf);  
		pBuf++;  
		strLen--;  
	}  

	return str;  
}

CPropertiesWnd::CPropertiesWnd() : m_nColorIndex(0)
{
	m_pText = NULL;
	m_pDevice = NULL;
	m_eShowMode = E_SHOW_NULL;
	m_pArea = NULL;
	SetNull();
}

CPropertiesWnd::~CPropertiesWnd()
{
	if (m_wndPropList)
	{
		m_wndPropList.RemoveAll();
	}
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	 ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)  
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	//CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("TextInfo"));
	////�ı���Ϣ
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("DefaultShow"), (_variant_t) _T("Static Text"), _T("Ĭ���ı�")));
	////����
	//LOGFONT lf;
	//CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	//font->GetLogFont(&lf);
	//lstrcpy(lf.lfFaceName, _T("Arial"));
	//pGroup1->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("����")));
	//
	////��ɫ
	//CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"), RGB(210, 192, 254), NULL, _T("��ɫ"));
	//pColorProp->EnableOtherButton(_T("Other..."));
	//pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	//pGroup1->AddSubItem(pColorProp);

	////��С
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("FontSize"), (_variant_t) _T("16"), _T("�����С")));

	////����
	//CMFCPropertyGridProperty* pPropAlign = new CMFCPropertyGridProperty(_T("Alignment"), _T("���ж���"), _T("���뷽ʽ"));
	//pPropAlign->AddOption(_T("���ж���"));
	//pPropAlign->AddOption(_T("���ж���"));
	//pPropAlign->AddOption(_T("���ж���"));
	//pPropAlign->AddOption(_T("���϶���"));
	//pPropAlign->AddOption(_T("���϶���"));
	//pPropAlign->AddOption(_T("���϶���"));
	//pPropAlign->AddOption(_T("���¶���"));
	//pPropAlign->AddOption(_T("���¶���"));
	//pPropAlign->AddOption(_T("���¶���"));
	//pPropAlign->AllowEdit(FALSE);
	//pGroup1->AddSubItem(pPropAlign);

	////�߿�
	//CMFCPropertyGridProperty* pPropBorder = new CMFCPropertyGridProperty(_T("Border"), _T("�ޱ߿�"), _T("�߿�"));
	//pPropBorder->AddOption(_T("�ޱ߿�"));
	//pPropBorder->AddOption(_T("��ͨ�߿�"));
	//pPropBorder->AddOption(_T("�ڿ�׵�"));
	//pPropBorder->AddOption(_T("3D���߿�"));
	//pPropBorder->AddOption(_T("3D͹�߿�"));
	//pPropBorder->AllowEdit(FALSE);
	//pGroup1->AddSubItem(pPropBorder);

	////�߿򱳾�
	//CMFCPropertyGridColorProperty* pBorderColorProp = new CMFCPropertyGridColorProperty(_T("BorderColor"), RGB(210, 192, 254), NULL, _T("�߿򱳾�"));
	//pBorderColorProp->EnableOtherButton(_T("Other..."));
	//pBorderColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	//pGroup1->AddSubItem(pBorderColorProp);

	////��
	//CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T("�ޱ߿�"), _T("��"));
	//pPropLayer->AddOption(_T("1��"));
	//pPropLayer->AddOption(_T("2��"));
	//pPropLayer->AddOption(_T("3��"));
	//pPropLayer->AddOption(_T("4��"));
	//pPropLayer->AddOption(_T("5��"));
	//pPropLayer->AddOption(_T("6��"));
	//pPropLayer->AddOption(_T("7��"));
	//pPropLayer->AddOption(_T("8��"));
	//pPropLayer->AddOption(_T("9��"));
	//pPropLayer->AddOption(_T("10��"));
	//pPropLayer->AllowEdit(FALSE);
	//pGroup1->AddSubItem(pPropLayer);

	////������
	//CMFCPropertyGridProperty* pPropBKStyle = new CMFCPropertyGridProperty(_T("BKStyle"), _T("��"), _T("������"));
	//pPropBKStyle->AddOption(_T("��"));
	//pPropBKStyle->AddOption(_T("�Զ���1"));
	//pPropBKStyle->AddOption(_T("�Զ���2"));
	//pPropBKStyle->AddOption(_T("�Զ���3"));
	//pPropBKStyle->AddOption(_T("�Զ���4"));
	//pPropBKStyle->AddOption(_T("�Զ���5"));
	//pPropBKStyle->AddOption(_T("�Զ���6"));
	//pPropBKStyle->AddOption(_T("�Զ���7"));
	//pPropBKStyle->AddOption(_T("�Զ���8"));
	//pPropBKStyle->AddOption(_T("�Զ���9"));
	//pPropBKStyle->AddOption(_T("�Զ���10"));
	//pPropBKStyle->AllowEdit(FALSE);
	//pGroup1->AddSubItem(pPropBKStyle);

	////��ʾ����
	//CMFCPropertyGridProperty* pPropMode = new CMFCPropertyGridProperty(_T("Border"), _T("ö��ģʽ"), _T("��ʾ����"));
	//pPropMode->AddOption(_T("ö��ģʽ"));
	//pPropMode->AddOption(_T("ֵģʽ"));
	//pPropMode->AddOption(_T("ʱ��ģʽ"));
	//pPropMode->AddOption(_T("�ַ���ģʽ"));
	//pPropMode->AllowEdit(FALSE);
	//pGroup1->AddSubItem(pPropMode);

	////ʱ�����
	//pGroup1->AddSubItem(new CBTinValueProperty(_T("DateTimePoint"), (_variant_t) _T(""), _T("ʱ�����")));

	////ʱ������  Time series
	//pGroup1->AddSubItem(new CBTinValueProperty(_T("DateTimeSeries"), (_variant_t) _T(""), _T("ʱ������")));

	////��ʱ�����
	//pGroup1->AddSubItem(new CBTinValueProperty(_T("NonDateTimePoint"), (_variant_t) _T(""), _T("��ʱ�����")));

	////ö�ٴ�����
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Enum Event"), (_variant_t) _T(""), _T("ö�ٴ����ã�0:�ر�|1:������:")));

	////С����
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("DecimalPoint"), (_variant_t) _T("0"), _T("С����")));

	//m_wndPropList.AddProperty(pGroup1);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}

LRESULT CPropertiesWnd::OnPropertyChanged( WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;  
	switch(m_eShowMode)
	{
	case E_SHOW_TEXT:
		ChangeTextProperty(pProp,m_pText);
		break;
	case E_SHOW_DEVICE:
		ChangeDeviceProperty(pProp,m_pDevice);
		break;
	case E_SHOW_PIPE:
		ChangePipeProperty(pProp,m_pPipe);
		break;
	case E_SHOW_BUTTON:
		ChangeButtonProperty(pProp,m_pButton);
		break;
	case E_SHOW_BEND:
		ChangeBendProperty(pProp,m_pBend);
		break;
	case E_SHOW_DASHBOARD:
		ChangeBoardProperty(pProp,m_pBoard);
		break;
	case E_SHOW_MAPNAVIGATE:
		ChangeMapNavigateProperty(pProp,m_pMapNavigate);
		break;
	case E_SHOW_DIAGNOSEGRAPHIC:
		ChangepDiagnoseGraphicProperty(pProp,m_pDiagnoseGraphic);
		break;
	case E_SHOW_TIME_SELECT:
		ChangeTimePickerProperty(pProp,m_pTime);
		break;
	case E_SHOW_LINE_CHART:
		ChangeLineChartProperty(pProp,m_pLine);
		break;
	case E_SHOW_BAR_CHART:
		ChangebarChartProperty(pProp,m_pBar);
		break;
	case E_SHOW_PIE_CHART:
		ChangePieChartProperty(pProp,m_pPie);
		break;
	case E_SHOW_ROI:
		ChangeRectangleProperty(pProp,m_pRectangle);
		break;
	case E_SHOW_TREND:
		ChangeTrendProperty(pProp,m_pTrend);
		break;
	case E_SHOW_CHECK_BUTTON:
		ChangeCheckButtonProperty(pProp,m_pCheck);
		break;
	case E_SHOW_EMBEDED:
		ChangeEmbededProperty(pProp,m_pEmbed);
		break;
	case E_SHOW_SHELL_EXECUTE:
		ChangeShellExecuteProperty(pProp,m_pExecute);
		break;
	case E_SHOW_AREA_CHANGE:
		ChangeAreaChangeProperty(pProp,m_pArea);
		break;	
	case E_SHOW_SCRAWL:
		ChangeStraightLineProperty(pProp,m_pStraightLine);
		break;
	case E_SHOW_LIQUID:
		ChangeLiquidProperty(pProp,m_pLiquid);
		break;
	case E_SHOW_TIME_BAR:
		ChangeTimeBarProperty(pProp,m_pTbar);
		break;
	case E_SHOW_PROGRESS:
		ChangeProgressProperty(pProp,m_pProgrs);
		break;
	case E_SHOW_NAVIGATE:
		ChangeNaviProperty(pProp,m_pNav);
		break;
	case E_SHOW_DATA_ANALYSIS:
		ChangeAnlyProperty(pProp,m_pAnalysis);
		break;
	case E_SHOW_REPORT:
		ChangeReportProperty(pProp,m_pReport);
		break;
	case E_SHOW_TEMP_DIS:
		ChangeTemperProperty(pProp,m_pTemp);
		break;
	case E_SHOW_RULER:
		ChangeRulerProperty(pProp,m_pRuler);
		break;
	case E_SHOW_POLYGON:
		ChangePolygonProperty(pProp, m_pPolygon);
		break;
	case E_SHOW_PLANE_TEMPE_DISTR:
		ChangePlaneTempeDistrProperty(pProp, m_pPlaneTempeDistr);
		break;
	case E_SHOW_DOTLINE_FRAME:
		ChangeDottedLineFrmProperty(pProp, m_pDottedLineFrm);
		break;
	case E_SHOW_GENERAL_COMPONENT:
		ChangeGeneralComponentProperty(pProp, m_pGeneralComponent);
		break;
	default:
		break;
	}
	return 0;  
}


void CPropertiesWnd::SetDeviceProperty( CEqmDrawDevice* pDevice )
{
	m_eShowMode = E_SHOW_DEVICE;
	m_pDevice = pDevice;
	if(m_pDevice)
	{
		m_wndPropList.RemoveAll();
		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("DeviceInfo"));

		//ͼԪ����
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("ElementName"), m_pDevice->GetEqmName().c_str(), _T("ͼԪ����")));

		//ͼ��
		CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("��"));
		pPropLayer->AddOption(_T("1��"));
		pPropLayer->AddOption(_T("2��"));
		pPropLayer->AddOption(_T("3��"));
		pPropLayer->AddOption(_T("4��"));
		pPropLayer->AddOption(_T("5��"));
		pPropLayer->AddOption(_T("6��"));
		pPropLayer->AddOption(_T("7��"));
		pPropLayer->AddOption(_T("8��"));
		pPropLayer->AddOption(_T("9��"));
		pPropLayer->AddOption(_T("10��"));
		pPropLayer->AllowEdit(FALSE);
		CString strLayer = pPropLayer->GetOption(m_pDevice->GetLayer());
		pPropLayer->SetValue((_variant_t)strLayer);
		pGroup1->AddSubItem(pPropLayer);

		//ͼƬ���� �滻ͼƬ  �ָ��ߴ� ������
		CMFCPropertyGridProperty* pPropImageOperation = new CMFCPropertyGridProperty(_T("ImageOperation"), _T("�޲���"), _T("ͼƬ�������滻ͼƬ/�ָ��ߴ磩"));
		pPropImageOperation->AddOption(_T("�޲���"));
		pPropImageOperation->AddOption(_T("�滻ͼƬ"));
		pPropImageOperation->AddOption(_T("�ָ��ߴ�"));
		pPropImageOperation->AllowEdit(FALSE);
		pGroup1->AddSubItem(pPropImageOperation);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		//��״̬��		��/
		CBTinValueProperty* pStatePoint = new CBTinValueProperty(_T("StatePoint"), (_variant_t)m_pDevice->GetPhysicalID().c_str(), _T("��״̬��"));
		pStatePoint->SetMainFrame(pMainFrame);
		pStatePoint->SetPopDlgType(E_POP_DLG_POINT);
		pGroup1->AddSubItem(pStatePoint);

		//�¼��󶨵�
		CBTinValueProperty* pEventPoint = new CBTinValueProperty(_T("EventPoint"), (_variant_t)m_pDevice->GetEventInfo().strSettingPointName, _T("�¼��󶨵�"));
		pEventPoint->SetMainFrame(pMainFrame);
		pEventPoint->SetPopDlgType(E_POP_DLG_POINT);
		pGroup1->AddSubItem(pEventPoint);

		//�豸����
		CMFCPropertyGridProperty* pPropDeviceType = new CMFCPropertyGridProperty(_T("DeviceType"),  _T(""), _T("�豸����"));
		pPropDeviceType->AddOption(_T("���"));
		pPropDeviceType->AddOption(_T("ˮ��(Ĭ��)"));
		pPropDeviceType->AddOption(_T("��ȴ��"));
		pPropDeviceType->AddOption(_T("һ���䶳��"));
		pPropDeviceType->AddOption(_T("�����䶳��"));
		pPropDeviceType->AddOption(_T("һ����ˮ��"));
		pPropDeviceType->AddOption(_T("������ˮ��"));
		pPropDeviceType->AddOption(_T("�������"));
		pPropDeviceType->AddOption(_T("��ȴ������"));
		pPropDeviceType->AddOption(_T("��Դ�෧��"));
		pPropDeviceType->AddOption(_T("��ȴ��"));
		pPropDeviceType->AddOption(_T("��ָ��"));
		pPropDeviceType->AddOption(_T("��ͨ����"));
		pPropDeviceType->AllowEdit(FALSE);
		CString strDeviceType = pPropDeviceType->GetOption((int)pDevice->GetEventInfo().deviceType);
		pPropDeviceType->SetValue((_variant_t)strDeviceType);
		pGroup1->AddSubItem(pPropDeviceType);

		//�豸��
		CString strDeviceID;
		strDeviceID.Format(_T("%d"),m_pDevice->GetEventInfo().nDeviceID);
		CMFCPropertyGridProperty* pDeviceID = new CMFCPropertyGridProperty(_T("DeviceID"), strDeviceID, _T("�豸��"));
		pGroup1->AddSubItem(pDeviceID);
		pDeviceID->Enable(FALSE);

		//�豸����
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("DeviceName"), m_pDevice->GetEventInfo().strShowName, _T("�豸����")));

		//����¼�����
		CMFCPropertyGridProperty* pPropMouseEventType = new CMFCPropertyGridProperty(_T("MouseEventType"),  _T(""), _T("����¼�����"));
		pPropMouseEventType->AddOption(_T("�豸����"));
		pPropMouseEventType->AddOption(_T("������"));
		pPropMouseEventType->AddOption(_T("�޸�ֵ"));
		pPropMouseEventType->AddOption(_T("��ָ��"));
		pPropMouseEventType->AddOption(_T("ҳ����ת"));
		pPropMouseEventType->AllowEdit(FALSE);
		CString strMouseEventType = pPropMouseEventType->GetOption((int)m_pDevice->GetEventInfo().mouseEventType);
		pPropMouseEventType->SetValue((_variant_t)strMouseEventType);
		pGroup1->AddSubItem(pPropMouseEventType);

		//�趨ֵ
		CString strValue;
		strValue.Format(_T("%d"),m_pDevice->GetEventInfo().nValue);
		CMFCPropertyGridProperty* pValue = new CMFCPropertyGridProperty(_T("MouseEventValue"), strValue, _T("����¼��趨ֵ"));
		pGroup1->AddSubItem(pValue);
		if(m_pDevice->GetEventInfo().mouseEventType == enumChangeValueWnd)
		{
			pValue->Enable(TRUE);
		}
		else
		{
			pValue->Enable(FALSE);
		}
		

		//����ҳ��
		CMFCPropertyGridProperty* pPropLinkPageID = new CMFCPropertyGridProperty(_T("LinkPageID"),  _T(""), _T("����ҳ��ID"));
		CEqmDrawProject* pProject = pMainFrame->m_DrawContext.GetProject();
		CString strPageName;
		if(pProject)
		{
			vector<CEqmDrawPage*> pageList = pProject->GetPageList();
			const size_t size = pageList.size();
			for (size_t i=0;i<size;++i)
			{
				if (pageList[i])
				{
					CString strPageName = pageList[i]->GetPageName().c_str();
					pPropLinkPageID->AddOption(strPageName);
				}
			}
			CEqmDrawPage* pPage = pProject->GetPageByID(m_pDevice->GetEventInfo().nPageID);
			if(pPage)
				strPageName = pPage->GetPageName().c_str();
		}
		pPropLinkPageID->AllowEdit(FALSE);
		pPropLinkPageID->SetValue((_variant_t)strPageName);
		pGroup1->AddSubItem(pPropLinkPageID);
		if(m_pDevice->GetEventInfo().mouseEventType == enumJumpPage)
		{
			pPropLinkPageID->Enable(TRUE);
		}
		else
		{
			pPropLinkPageID->Enable(FALSE);
		}

		//�¼�
		CMFCPropertyGridProperty* pGroupEvent = new CMFCPropertyGridProperty(_T("BindEvent"));
		pGroup1->AddSubItem(pGroupEvent);

		//
		CMFCPropertyGridProperty* pPropEventEdit = new CMFCPropertyGridProperty(_T("EventOperation"), _T("�޲���"), _T("�¼�����"));
		pPropEventEdit->AddOption(_T("�޲���"));
		pPropEventEdit->AddOption(_T("����¼�"));
		pPropEventEdit->AddOption(_T("ɾ���¼�"));
		pPropEventEdit->AllowEdit(FALSE);
		pGroupEvent->AddSubItem(pPropEventEdit);

		vector<BindInfo> vec = m_pDevice->m_vecBindInfo;
		for (size_t i=0;i<vec.size();++i)
		{
			CString strValue;
			strValue.Format(_T("%d,%d,%d,%d"),vec[i].nValue,vec[i].nPicID,vec[i].nFrame,vec[i].nInterval);

			//�¼�
			CBTinValueProperty* pEvent = new CBTinValueProperty(_T("Event"), (_variant_t)strValue, _T("�¼�����(ֵ,ͼƬ(�򶯻�)ID,֡��,�������(ms))"));
			pEvent->SetMainFrame(pMainFrame);
			pEvent->SetPopDlgType(E_POP_DLG_ADD_DEVICE_INFO);
			pGroupEvent->AddSubItem(pEvent);
		}

		//���
		CMFCPropertyGridProperty* pGroupDiagnose = new CMFCPropertyGridProperty(_T("Diagnose"));
		pGroup1->AddSubItem(pGroupDiagnose);

		//
		CMFCPropertyGridProperty* pPropDiagnoseEdit = new CMFCPropertyGridProperty(_T("DiagnoseAssociation"), _T("�޲���"), _T("��Ϲ���"));
		pPropDiagnoseEdit->AddOption(_T("�޲���"));
		pPropDiagnoseEdit->AddOption(_T("���"));
		pPropDiagnoseEdit->AddOption(_T("ɾ��"));
		pPropDiagnoseEdit->AllowEdit(FALSE);
		pGroupDiagnose->AddSubItem(pPropDiagnoseEdit);

		const vector<PageIDName>& vecPage = m_pDevice->GetDiagnosePageVec();
		for(size_t i=0;i<vecPage.size();++i)
		{
			CString strValue;
			strValue.Format(_T("%d,%s"),vecPage[i].nPageID,vecPage[i].strPageName);

			//��Ϲ���
			CBTinValueProperty* pDiagnose = new CBTinValueProperty(_T("Diagnose"), (_variant_t)strValue, _T("��Ϲ�������(ҳ��ID,ҳ������)"));
			pDiagnose->SetMainFrame(pMainFrame);
			pDiagnose->SetPopDlgType(E_POP_DLG_ADD_DEVICE_DIAGNOSE);
			pGroupDiagnose->AddSubItem(pDiagnose);
		}
		m_wndPropList.AddProperty(pGroup1);
	}
}

void CPropertiesWnd::SetPointMap( CDataPointManager* pointmap )
{
	m_refpointmap = pointmap;
}

void CPropertiesWnd::ShowTextProperty( CEqmDrawText* pText )
{

}

void CPropertiesWnd::ShowDeviceProperty( CEqmDrawDevice* pDevice )
{

}

void CPropertiesWnd::ShowEqmDrawBase( CEqmDrawBase* pBase )
{

}

void CPropertiesWnd::ChangeTextProperty( CMFCPropertyGridProperty* pProp,CEqmDrawText* pText )
{
	m_pText = pText;
	if(m_pText && pProp)
	{
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ı�֮ǰ��ֵ  
		CommandManager::Instance()->CallCommand(m_pText,E_COMMAND_EDIT);
		if(vChange != vBefore)
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());

			m_pText->DeleteFont();
			m_pText->DeleteSolidBrush();
			m_pText->DeleteHatchBrush();
			m_pText->DeleteCommonBorderPen();
			m_pText->Delete3DPenSunken();
			m_pText->Delete3DPenHump();

			if(strParamName == _T("DefaultShow"))			//����
			{
				CString strNewValue = vChange.bstrVal;      //��COleVariant��CString  
				m_pText->SetInitialString(strNewValue.GetString());
				m_pText->SetShowString(_T(""), SHOWTYPE_DRAW);
			}
			else if(strParamName == _T("FontSize"))			//��С
			{
				m_pText->SetFontSize(vChange.intVal);
			}
			else if(strParamName == _T("Color"))			//������ɫ
			{
				m_pText->SetFontColor(vChange.intVal);
			}
			else if(strParamName == _T("Font"))				//����
			{
				CString strNewFontValue = vChange.bstrVal;
				strNewFontValue = strNewFontValue.Left(strNewFontValue.ReverseFind('('));
				m_pText->SetFontName(strNewFontValue);
			}
			else if(strParamName == _T("Alignment"))					//���뷽ʽ
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("���ж���"))
				{
					m_pText->SetAlign(0);
				}
				else if(strNewValue == _T("���ж���"))
				{
					m_pText->SetAlign(1);
				}
				else if(strNewValue == _T("���ж���"))
				{
					m_pText->SetAlign(2);
				}
				else if(strNewValue == _T("���϶���"))
				{
					m_pText->SetAlign(3);
				}
				else if(strNewValue == _T("���϶���"))
				{
					m_pText->SetAlign(4);
				}
				else if(strNewValue == _T("���϶���"))
				{
					m_pText->SetAlign(5);
				}
				else if(strNewValue == _T("���¶���"))
				{
					m_pText->SetAlign(6);
				}
				else if(strNewValue == _T("���¶���"))
				{
					m_pText->SetAlign(7);
				}
				else if(strNewValue == _T("���¶���"))
				{
					m_pText->SetAlign(8);
				}
			}
			else if(strParamName == _T("Border"))			//�߿�
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("�ޱ߿�"))
				{
					m_pText->SetBorderType(0);
				}
				else if(strNewValue == _T("��ͨ�߿�"))
				{
					m_pText->SetBorderType(1);
				}
				else if(strNewValue == _T("�ڿ�׵�"))
				{
					m_pText->SetBorderType(2);
				}
				else if(strNewValue == _T("3D���߿�"))
				{
					m_pText->SetBorderType(3);
				}
				else if(strNewValue == _T("3D͹�߿�"))
				{
					m_pText->SetBorderType(4);
				}				
			}
			else if(strParamName == _T("BorderColor"))			//�߿򱳾�
			{
				m_pText->SetBorderColor(vChange.intVal);
			}
			else if(strParamName == _T("Layer"))			//��
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("1��"))
				{
					m_pText->SetLayer(0);
				}
				else if(strNewValue == _T("2��"))
				{
					m_pText->SetLayer(1);
				}
				else if(strNewValue == _T("3��"))
				{
					m_pText->SetLayer(2);
				}
				else if(strNewValue == _T("4��"))
				{
					m_pText->SetLayer(3);
				}
				else if(strNewValue == _T("5��"))
				{
					m_pText->SetLayer(4);
				}
				else if(strNewValue == _T("6��"))
				{
					m_pText->SetLayer(5);
				}
				else if(strNewValue == _T("7��"))
				{
					m_pText->SetLayer(6);
				}
				else if(strNewValue == _T("8��"))
				{
					m_pText->SetLayer(7);
				}
				else if(strNewValue == _T("9��"))
				{
					m_pText->SetLayer(8);
				}
				else if(strNewValue == _T("10��"))
				{
					m_pText->SetLayer(9);
				}				
			}
			else if(strParamName == _T("BKStyle"))			//��������ʽ
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("��"))
				{
					m_pText->SetTextBKType(0);
				}
				else if(strNewValue == _T("�Զ���1"))
				{
					m_pText->SetTextBKType(1);
				}
				else if(strNewValue == _T("�Զ���2"))
				{
					m_pText->SetTextBKType(2);
				}
				else if(strNewValue == _T("�Զ���3"))
				{
					m_pText->SetTextBKType(3);
				}
				else if(strNewValue == _T("�Զ���4"))
				{
					m_pText->SetTextBKType(4);
				}
				else if(strNewValue == _T("�Զ���5"))
				{
					m_pText->SetTextBKType(5);
				}
				else if(strNewValue == _T("�Զ���6"))
				{
					m_pText->SetTextBKType(6);
				}
				else if(strNewValue == _T("�Զ���7"))
				{
					m_pText->SetTextBKType(7);
				}
				else if(strNewValue == _T("�Զ���8"))
				{
					m_pText->SetTextBKType(8);
				}
				else if(strNewValue == _T("�Զ���9"))
				{
					m_pText->SetTextBKType(9);
				}
				else if(strNewValue == _T("�Զ���10"))
				{
					m_pText->SetTextBKType(10);
				}
			}
			else if(strParamName == _T("EnumMode"))			//��ʾ����
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("ö��ģʽ"))
				{
					m_pText->SetTextShowMode((TEXTSHOWMODE)0);
				}
				else if(strNewValue == _T("ֵģʽ"))
				{
					m_pText->SetTextShowMode((TEXTSHOWMODE)1);
				}
				else if(strNewValue == _T("ʱ��ģʽ"))
				{
					m_pText->SetTextShowMode((TEXTSHOWMODE)2);
				}
				else if(strNewValue == _T("�ַ���ģʽ"))
				{
					m_pText->SetTextShowMode((TEXTSHOWMODE)3);
				}

				//����
				if (m_pText->GetTextShowMode() == 0)//�ı�
				{
					m_wndPropList.GetProperty(0)->GetSubItem(13)->Enable(TRUE);
					m_wndPropList.GetProperty(0)->GetSubItem(14)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(10)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(11)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(12)->Enable(TRUE);
				}
				else if(m_pText->GetTextShowMode() == 1)//ֵ
				{
					m_wndPropList.GetProperty(0)->GetSubItem(13)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(14)->Enable(TRUE);
					m_wndPropList.GetProperty(0)->GetSubItem(10)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(11)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(12)->Enable(TRUE);
				}
				else if (m_pText->GetTextShowMode() == 2)//ʱ��
				{
					m_wndPropList.GetProperty(0)->GetSubItem(13)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(14)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(10)->Enable(TRUE);
					m_wndPropList.GetProperty(0)->GetSubItem(11)->Enable(TRUE);
					m_wndPropList.GetProperty(0)->GetSubItem(12)->Enable(FALSE);
				}
				else if (m_pText->GetTextShowMode() == 3)//�ַ���
				{
					m_wndPropList.GetProperty(0)->GetSubItem(13)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(14)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(10)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(11)->Enable(FALSE);
					m_wndPropList.GetProperty(0)->GetSubItem(12)->Enable(TRUE);
				}
			}
			else if(strParamName == _T("DateTimePoint"))			//ʱ�����
			{
				m_pText->SetDateTimePoint(vChange.bstrVal);
			}
			else if(strParamName == _T("DateTimeSeries"))			//ʱ������
			{
				m_pText->SetDateTime(vChange.bstrVal);
			}
			else if(strParamName == _T("NonDateTimePoint"))			//��ʱ�����
			{
				CString str = vChange.bstrVal;
				m_pText->SetPhysicalID(str.GetString());
			}
			else if(strParamName == _T("Enum Event"))			//ö�ٴ�����
			{
				m_pText->SetTextModeShowString(vChange.bstrVal);
			}
			else if(strParamName == _T("DecimalPoint"))			//С����λ
			{
				m_pText->SetPointNum(vChange.intVal);
			}
			else if(strParamName == _T("TextAttri"))
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("��������"))
				{
					m_pText->SetTextAttribute((TextAttri)0);
				}
				else if(strNewValue == _T("�¶�"))
				{
					m_pText->SetTextAttribute((TextAttri)1);
				}
				else if(strNewValue == _T("ʪ��"))
				{
					m_pText->SetTextAttribute((TextAttri)2);
				}
				else if(strNewValue == _T("SP"))
				{
					m_pText->SetTextAttribute((TextAttri)3);
				}
			}
			m_pText->SetNeedSave(true);
			pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
		}
	}
}

void CPropertiesWnd::ChangeDeviceProperty( CMFCPropertyGridProperty* pProp,CEqmDrawDevice* pDevice )
{
	m_pDevice = pDevice;
	if(m_pDevice && pProp)
	{
		CommandManager::Instance()->CallCommand(m_pDevice,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ı�֮ǰ��ֵ  
		int nNum = pProp->GetData();
		if(vChange != vBefore)
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
			if(strParamName == _T("ElementName"))	//ͼԪ����
			{
				CString strNewValue = vChange.bstrVal;      //��COleVariant��CString  
				m_pDevice->SetEqmName(strNewValue.GetString());
			}
			else if(strParamName == _T("Layer"))		//ͼ��
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("1��"))
				{
					m_pDevice->SetLayer(0);
				}
				else if(strNewValue == _T("2��"))
				{
					m_pDevice->SetLayer(1);
				}
				else if(strNewValue == _T("3��"))
				{
					m_pDevice->SetLayer(2);
				}
				else if(strNewValue == _T("4��"))
				{
					m_pDevice->SetLayer(3);
				}
				else if(strNewValue == _T("5��"))
				{
					m_pDevice->SetLayer(4);
				}
				else if(strNewValue == _T("6��"))
				{
					m_pDevice->SetLayer(5);
				}
				else if(strNewValue == _T("7��"))
				{
					m_pDevice->SetLayer(6);
				}
				else if(strNewValue == _T("8��"))
				{
					m_pDevice->SetLayer(7);
				}
				else if(strNewValue == _T("9��"))
				{
					m_pDevice->SetLayer(8);
				}
				else if(strNewValue == _T("10��"))
				{
					m_pDevice->SetLayer(9);
				}	
			}
			else if(strParamName == _T("ImageOperation"))	//ͼƬ���� �滻ͼƬ  �ָ��ߴ� ������
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("�滻ͼƬ"))
				{
					CMetaFileSelectDlg dlg;
					dlg.SetWndType(e_picture);
					dlg.SetCurrentPicID(m_pDevice->GetPicID());
					if (dlg.DoModal() == IDOK)
					{	
						int nPicId = 0;
						pView->m_PageDlg->m_dlgCanvas.InsertImageIntoOpenS3db(dlg.m_nID,nPicId);				
						if (dlg.m_nID != m_pDevice->GetPicID())
						{
							Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(dlg.m_nID, false);
							if(pBitmap)
							{
								m_pDevice->SetWidth(pBitmap->GetWidth());
								m_pDevice->SetHeight(pBitmap->GetHeight());
								m_pDevice->SetBitmap(pBitmap);
								m_pDevice->SetPicID(dlg.m_nID);
							}
						}
					}
				}
				else if(strNewValue == _T("�ָ��ߴ�"))
				{
					m_pDevice->SetWidth(m_pDevice->GetBitmap()->GetWidth());
					m_pDevice->SetHeight(m_pDevice->GetBitmap()->GetHeight());
				}

				pProp->SetValue((_variant_t)_T("�޲���"));
			}
			else if(strParamName == _T("StatePoint"))//��״̬��
			{
				m_pDevice->SetPhysicalID(vChange.bstrVal);
			}
			else if(strParamName == _T("EventPoint"))//�¼��󶨵�
			{
				m_pDevice->SetEventInfo(vChange.bstrVal, m_pDevice->GetEventInfo().deviceType,  m_pDevice->GetEventInfo().mouseEventType, m_pDevice->GetEventInfo().nDeviceID, m_pDevice->GetEventInfo().nPageID, m_pDevice->GetEventInfo().nValue, m_pDevice->GetEventInfo().strShowName,_T(""));
			}
			else if(strParamName == _T("DeviceType"))//�豸����
			{
				CString strNewValue = vChange.bstrVal;
				DeviceTypeForMeta nType = enumNull;
				if(strNewValue == _T("���"))
				{
					nType = enumChiller;
				}
				else if(strNewValue == _T("ˮ��(Ĭ��)"))
				{
					nType = enumPump;
				}
				else if(strNewValue == _T("��ȴ��"))
				{
					nType = enumCoolingPump;
				}
				else if(strNewValue == _T("һ���䶳��"))
				{
					nType = enumCoolingPump1;
				}
				else if(strNewValue == _T("�����䶳��"))
				{
					nType = enumCoolingPump2;
				}
				else if(strNewValue == _T("һ����ˮ��"))
				{
					nType = enumWarmingPump1;
				}
				else if(strNewValue == _T("������ˮ��"))
				{
					nType = enumWarmingPump2;
				}
				else if(strNewValue == _T("�������"))
				{
					nType = enumChillerValve;
				}
				else if(strNewValue == _T("��ȴ������"))
				{
					nType = enumCTValve;
				}
				else if(strNewValue == _T("��Դ�෧��"))
				{
					nType = enumGroundValve;
				}
				else if(strNewValue == _T("��ȴ��"))
				{
					nType = enumCoolingTower;
				}
				else if(strNewValue == _T("��ָ��"))
				{
					nType = enumNull;
				}
				else if(strNewValue == _T("��ͨ����"))
				{
					nType = enumCommonValve;
				}

				m_pDevice->SetEventInfo(m_pDevice->GetEventInfo().strSettingPointName, nType,  m_pDevice->GetEventInfo().mouseEventType, m_pDevice->GetEventInfo().nDeviceID, m_pDevice->GetEventInfo().nPageID, m_pDevice->GetEventInfo().nValue, m_pDevice->GetEventInfo().strShowName, m_pDevice->GetOutsideLink().c_str());
			}
			else if(strParamName == _T("DeviceID"))//�豸��
			{
				m_pDevice->SetEventInfo(m_pDevice->GetEventInfo().strSettingPointName, m_pDevice->GetEventInfo().deviceType,  m_pDevice->GetEventInfo().mouseEventType, vChange.intVal, m_pDevice->GetEventInfo().nPageID, m_pDevice->GetEventInfo().nValue, m_pDevice->GetEventInfo().strShowName,m_pDevice->GetOutsideLink().c_str());
			}
			else if(strParamName == _T("DeviceName"))//�豸����
			{
				m_pDevice->SetEventInfo(m_pDevice->GetEventInfo().strSettingPointName, m_pDevice->GetEventInfo().deviceType,  m_pDevice->GetEventInfo().mouseEventType, m_pDevice->GetEventInfo().nDeviceID, m_pDevice->GetEventInfo().nPageID, m_pDevice->GetEventInfo().nValue, vChange.bstrVal,m_pDevice->GetOutsideLink().c_str());
			}
			else if(strParamName == _T("MouseEventType"))//����¼�����
			{
				MouseEventTypeForMeta nType = enumNullWnd;
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("�豸����"))
				{
					nType = enumDevicePropertyWnd;
				}
				else if(strNewValue == _T("������"))
				{
					nType = enumSettingWnd;
				}
				else if(strNewValue == _T("�޸�ֵ"))
				{
					nType = enumChangeValueWnd;
				}
				else if(strNewValue == _T("��ָ��"))
				{
					nType = enumNullWnd;
				}
				else if(strNewValue == _T("ҳ����ת"))
				{
					nType = enumJumpPage;
				}
				m_pDevice->SetEventInfo(m_pDevice->GetEventInfo().strSettingPointName, m_pDevice->GetEventInfo().deviceType,  nType, m_pDevice->GetEventInfo().nDeviceID, m_pDevice->GetEventInfo().nPageID, m_pDevice->GetEventInfo().nValue, m_pDevice->GetEventInfo().strShowName, pDevice->GetOutsideLink().c_str());
				
				if (nType == enumChangeValueWnd)
				{
					m_wndPropList.GetProperty(0)->GetSubItem(9)->Enable(TRUE);
				}
				else
				{
					m_wndPropList.GetProperty(0)->GetSubItem(9)->Enable(FALSE);
				}
				if (nType == enumJumpPage)
				{
					m_wndPropList.GetProperty(0)->GetSubItem(10)->Enable(TRUE);
				}
				else
				{
					m_wndPropList.GetProperty(0)->GetSubItem(10)->Enable(FALSE);
				}
			
			}
			else if(strParamName == _T("MouseEventValue"))//�趨ֵ
			{
				m_pDevice->SetEventInfo(m_pDevice->GetEventInfo().strSettingPointName, m_pDevice->GetEventInfo().deviceType,  m_pDevice->GetEventInfo().mouseEventType, m_pDevice->GetEventInfo().nDeviceID, m_pDevice->GetEventInfo().nPageID, _wtoi(vChange.bstrVal), m_pDevice->GetEventInfo().strShowName, m_pDevice->GetOutsideLink().c_str());
			}
			else if(strParamName == _T("LinkPageID"))//����ҳ��
			{
				CEqmDrawProject* pProject = pMainFrame->m_DrawContext.GetProject();
				int nPageID = -1;
				if(pProject)
				{
					vector<CEqmDrawPage*> pageList = pProject->GetPageList();
					const size_t size = pageList.size();
					for (size_t i=0;i<size;++i)
					{
						if (pageList[i])
						{
							CString strPageName = pageList[i]->GetPageName().c_str();
							if(strPageName == vChange.bstrVal)
							{
								nPageID = pageList[i]->GetID();
								break;
							}
						}
					}
				}
				m_pDevice->SetEventInfo(m_pDevice->GetEventInfo().strSettingPointName, m_pDevice->GetEventInfo().deviceType,  m_pDevice->GetEventInfo().mouseEventType, m_pDevice->GetEventInfo().nDeviceID, nPageID, m_pDevice->GetEventInfo().nValue, m_pDevice->GetEventInfo().strShowName, m_pDevice->GetOutsideLink().c_str());
			}
			else if(strParamName == _T("EventOperation"))	//�¼�����
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("����¼�"))
				{
					CStateChangeDlg dlg(&(pView->m_PageDlg->m_dlgCanvas));
					dlg.SetProjectSqlPointer(pMainFrame->m_pPrjSqlite);
					dlg.m_nValue = 0;
					dlg.m_nFrame = 1;
					dlg.m_nPicID = 0;
					dlg.m_nInterval = 0;
					if (dlg.DoModal() == IDOK)
					{
						CString strParam10 = m_pDevice->GetBindString().c_str();
						CString strValue;
						strValue.Format(_T("%d,%d,%d,%d"),dlg.m_nValue,dlg.m_nPicID,dlg.m_nFrame,dlg.m_nInterval);

						strParam10 += L"|";
						strParam10 += strValue;
						m_pDevice->SetBindString(strParam10.GetString());

						m_pDevice->m_vecBindInfo.clear();
						if (strParam10.GetLength()>0)
						{
							m_pDevice->ParseParam10(strParam10);
						}

						//�¼�
						CBTinValueProperty* pEvent = new CBTinValueProperty(_T("Event"), (_variant_t)strValue, _T("�¼�����(ֵ,ͼƬ(�򶯻�)ID,֡��,�������(ms))"));
						pEvent->SetMainFrame(pMainFrame);
						pEvent->SetPopDlgType(E_POP_DLG_ADD_DEVICE_INFO);
						pProp->GetParent()->AddSubItem(pEvent);
						pProp->GetParent()->Expand(FALSE);
						pProp->GetParent()->Expand(TRUE);
					}
				}
				pProp->SetValue((_variant_t)_T("�޲���"));
			}
			else if(strParamName == _T("Event"))	//�¼�����
			{
				CMFCPropertyGridProperty* pParent = pProp->GetParent();
				if(pParent)
				{
					COleVariant vOperation = pParent->GetSubItem(0)->GetValue();
					CString strOperation = vOperation.bstrVal;
					CString strNewValue = vChange.bstrVal;

					//ɾ��
					if(strNewValue == _T(""))
					{							
						if(pParent->RemoveSubItem((CMFCPropertyGridProperty*)pProp))
						{
							pParent->Expand(FALSE);
							pParent->Expand(TRUE);
							pParent->Redraw();
						}							
					}

					//����
					int nCount = pParent->GetSubItemsCount();
					m_pDevice->m_vecBindInfo.clear();
					CString strParam10;
					for(int i=1;i<nCount; ++i)
					{
						strParam10 += pParent->GetSubItem(i)->GetValue();
						if(i != nCount-1)
						{
							strParam10 += L"|";
						}
					}
					m_pDevice->SetBindString(strParam10.GetString());
					if (strParam10.GetLength()>0)
					{
						m_pDevice->ParseParam10(strParam10);
					}
				}
			}
			else if(strParamName == _T("DiagnoseAssociation"))//���
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("���"))
				{
					CBindPageDiagnoseDlg dlg;
					dlg.m_strPageID = _T("-1");
					dlg.m_str_page_diagnose = _T("PageName");
					if (dlg.DoModal() == IDOK)
					{
						m_pDevice->InsertDiagnoseVec(_wtoi(dlg.m_strPageID.GetString()),dlg.m_str_page_diagnose);

						CString strValue;
						strValue.Format(_T("%d,%s"),_wtoi(dlg.m_strPageID.GetString()),dlg.m_str_page_diagnose);

						//�¼�
						CBTinValueProperty* pDiagnose = new CBTinValueProperty(_T("Diagnose"), (_variant_t)strValue, _T("��Ϲ�������(ҳ��ID,ҳ������)"));
						pDiagnose->SetMainFrame(pMainFrame);
						pDiagnose->SetPopDlgType(E_POP_DLG_ADD_DEVICE_DIAGNOSE);
						pProp->GetParent()->AddSubItem(pDiagnose);
						pProp->GetParent()->Expand(FALSE);
						pProp->GetParent()->Expand(TRUE);
					}
				}
				pProp->SetValue((_variant_t)_T("�޲���"));
			}
			else if(strParamName == _T("Diagnose"))  //��ϲ���
			{
				CMFCPropertyGridProperty* pParent = pProp->GetParent();
				if(pParent)
				{
					COleVariant vOperation = pParent->GetSubItem(0)->GetValue();
					CString strOperation = vOperation.bstrVal;
					CString strNewValue = vChange.bstrVal;

					//ɾ��
					if(strNewValue == _T(""))
					{
						if(pParent->RemoveSubItem((CMFCPropertyGridProperty*)pProp))
						{
							pParent->Expand(FALSE);
							pParent->Expand(TRUE);
							pParent->Redraw();
						}	
					}
		
					m_pDevice->ClearDiagnoseVec();
					int nCount = pParent->GetSubItemsCount();
					CString strValue;
					for(int i=1;i<nCount; ++i)
					{
						strValue = pParent->GetSubItem(i)->GetValue();
						vector<wstring> vecResult;
						Project::Tools::SplitStringByChar(strValue,L',',vecResult);
						if(vecResult.size()==2)
						{
							m_pDevice->InsertDiagnoseVec(_wtoi(vecResult[0].c_str()),vecResult[1].c_str());
						}
					}
				}
			}
			m_pDevice->SetNeedSave(TRUE);
			pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
		}
	}
}

void CPropertiesWnd::ChangeEqmDrawBase( CMFCPropertyGridProperty* pProp,CEqmDrawBase* pBase )
{

}

void CPropertiesWnd::ShowPropertyWnd( BOOL bShow )
{
	ShowPane(bShow,FALSE,TRUE);
}


BOOL CBTinValueProperty::HasButton() const  
{  
	return TRUE;  
}  

void CBTinValueProperty::OnClickButton(CPoint point)  
{  
	CMFCPropertyGridProperty* pHit = HitTest(point);
	if(pHit)
	{
		if(m_eDlgType == E_POP_DLG_POINT)
		{
			if(m_pMainFrame)
			{
				if(&m_pMainFrame->m_plcpointmap)
				{
					CDataComConfigDlg dlg(m_pMainFrame->m_plcpointmap);
					if (dlg.DoModal() == IDOK)
					{
						pHit->SetValue((_variant_t)dlg.m_strPointName);
					}
				}
			}
		}
		else if(m_eDlgType == E_POP_DLG_IMAGE)
		{
			CMetaFileSelectDlg dlg;
			dlg.SetWndType(e_picture);
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			dlg.SetCurrentPicID(pFrame->m_nSelectPicID);
			if (dlg.DoModal() == IDOK)
			{				
				pFrame->m_nSelectPicID = dlg.m_nID;		
				long tmp = dlg.m_nID;			//!!! VT_INT was converted to	VT_I4.
				pHit->SetValue((_variant_t)tmp);
			}			
		}
		else if(m_eDlgType == E_POP_DLG_ADD_DEVICE_INFO)
		{
			if(m_pMainFrame)
			{
				CMFCPropertyGridProperty* pParent = pHit->GetParent();
				if(pParent)
				{
					COleVariant vOperation = pParent->GetSubItem(0)->GetValue();
					if(vOperation.bstrVal == _T("ɾ���¼�"))
					{						
					}
					else
					{
						CString strValue = pHit->GetValue();
						vector<wstring> vecValue;
						Project::Tools::SplitStringByChar(strValue,L',',vecValue);
						if(vecValue.size() == 4)
						{
							CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(m_pMainFrame->GetActiveView());
							CStateChangeDlg dlg(&(pView->m_PageDlg->m_dlgCanvas));
							dlg.SetProjectSqlPointer(m_pMainFrame->m_pPrjSqlite);
							dlg.m_nValue = _wtoi(vecValue[0].c_str());
							dlg.m_nFrame = _wtoi(vecValue[2].c_str());
							dlg.m_nPicID = _wtoi(vecValue[1].c_str());
							dlg.m_nInterval = _wtoi(vecValue[3].c_str());
							if (dlg.DoModal() == IDOK)
							{
								CString strValue;
								strValue.Format(_T("%d,%d,%d,%d"),dlg.m_nValue,dlg.m_nPicID,dlg.m_nFrame,dlg.m_nInterval);
								pHit->SetValue((_variant_t)strValue);
							}
						}
					}
				}
			}
		}
		else if(m_eDlgType == E_POP_DLG_ADD_DEVICE_DIAGNOSE)
		{
			if(m_pMainFrame)
			{
				CMFCPropertyGridProperty* pParent = pHit->GetParent();
				if(pParent)
				{
					COleVariant vOperation = pParent->GetSubItem(0)->GetValue();
					if(vOperation.bstrVal == _T("ɾ��"))
					{	

					}
					else
					{
						CString strValue = pHit->GetValue();
						vector<wstring> vecValue;
						Project::Tools::SplitStringByChar(strValue,L',',vecValue);
						if(vecValue.size() == 2)
						{
							CBindPageDiagnoseDlg dlg;
							dlg.m_strPageID =vecValue[0].c_str();
							dlg.m_str_page_diagnose = vecValue[1].c_str();
							if (dlg.DoModal() == IDOK)
							{
								CString strValue;
								strValue.Format(_T("%s,%s"),dlg.m_strPageID,dlg.m_str_page_diagnose);
								pHit->SetValue((_variant_t)strValue);
							}
						}
					}
				}
			}
		}
		else if(m_eDlgType == E_POP_DLG_ADD_NAVI_ITEM)		
		{
			if(m_pMainFrame)
			{
				CMFCPropertyGridProperty* pParent = pHit->GetParent();
				if(pParent)
				{
					COleVariant vOperation = pParent->GetSubItem(0)->GetValue();
					if(vOperation.bstrVal == _T("ɾ��"))
					{	

					}
					else
					{
						CString strValue = pHit->GetValue();
						vector<wstring> vecValue;
						Project::Tools::SplitStringByChar(strValue,L',',vecValue);
						if(vecValue.size() == 5)
						{
							CMapPageInfo mapinfo;
							mapinfo.SetLongitudeValue(1,_wtof(vecValue[0].c_str()));
							mapinfo.SetLatitudeValue(1,_wtof(vecValue[1].c_str()));
							mapinfo.SetLongitudeValue(2,_wtof(vecValue[2].c_str()));
							mapinfo.SetLatitudeValue(2,_wtof(vecValue[3].c_str()));
							mapinfo.SetPageId(_wtoi(vecValue[4].c_str()));
							CMapNavigateItemSetDlg dlg(mapinfo);
							if (dlg.DoModal() == IDOK)
							{
								mapinfo = dlg.GetMapPageInfo();
								CString strValue;
								strValue.Format(_T("%0.6f,%0.6f,%0.6f,%0.6f,%d"),mapinfo.GetLongitudeValue(1),mapinfo.GetLatitudeValue(1),mapinfo.GetLongitudeValue(2),mapinfo.GetLatitudeValue(2),mapinfo.GetPageId());
								pHit->SetValue((_variant_t)strValue);
							}
						}
					}
				}
			}
		}
		else if (m_eDlgType ==E_POP_DATE_TIME)
		{
			CBTinValueProperty *pSon = static_cast<CBTinValueProperty *> (pHit);

			CString strDesc(pSon->GetName());
			RoiTimeType		tmType;
			COleDateTime	tmStart;
			COleDateTime	tmOccur;
			COleDateTime	tmEnd;

			if (strDesc == _T("Start Time"))
			{
				tmType = RoiTimeStart;
			}
			else if (strDesc == _T("Occur Time"))
			{
				tmType = RoiTimeOccur;
			}
			else if (strDesc == _T("End Time"))
			{
				tmType = RoiTimeEnd;
			}

			CMFCPropertyGridProperty* pTmSet = pSon->GetParent();
			if (pTmSet != NULL)
			{
				if (3 == pTmSet->GetSubItemsCount())
				{
					CBTinValueProperty* pStartTm = static_cast<CBTinValueProperty*>(pTmSet->GetSubItem(0));
					if (pStartTm != NULL)
					{
						tmStart = Project::Tools::Time_tToSystemTime(pStartTm->m_data);
					}
					CBTinValueProperty* pOccurTm = static_cast<CBTinValueProperty*>(pTmSet->GetSubItem(1));
					if (pOccurTm != NULL)
					{
						tmOccur = Project::Tools::Time_tToSystemTime(pOccurTm->m_data);
					}
					CBTinValueProperty* pEndTm = static_cast<CBTinValueProperty*>(pTmSet->GetSubItem(2));
					if (pEndTm != NULL)
					{
						tmEnd = Project::Tools::Time_tToSystemTime(pEndTm->m_data);
					}
				}
			}

			CTimePickDlg dlg(tmType, tmStart, tmOccur, tmEnd);
			dlg.m_nDateTime = pSon->m_data;
			if (dlg.DoModal()==	IDOK)
			{
				pSon->m_data = dlg.m_nDateTime;
				COleDateTime timeSel(Project::Tools::Time_tToSystemTime(pSon->m_data));
				CString str = timeSel.Format(L"%Y-%m-%d %H:%M:%S");
				pSon->SetValue(str);
			}
		}
		else if (m_eDlgType == E_POP_DLG_SELECT_PATH)
		{
			CPathDialog dlg;
			if(dlg.DoModal()==IDOK)
			{
				pHit->SetValue(dlg.GetPathName());
			}
		}
	}
} 

CBTinValueProperty::CBTinValueProperty(const CString& strName,const COleVariant& varValue,LPCTSTR lpszDescr)
	:	CMFCPropertyGridProperty(strName,varValue,lpszDescr)
{
	m_pMainFrame = NULL;
	m_refpointmap = NULL;
	m_eDlgType = E_POP_DLG_NULL;
}

CBTinValueProperty::CBTinValueProperty( const CString& strName )
	:	CMFCPropertyGridProperty(strName)
{
	m_pMainFrame = NULL;
	m_refpointmap = NULL;
	m_eDlgType = E_POP_DLG_NULL;
}

void CBTinValueProperty::SetPointMap( CDataPointManager* pointmap )
{
	m_refpointmap = pointmap;
}

void CBTinValueProperty::SetMainFrame( CMainFrame* pMainFrame )
{
	m_pMainFrame = pMainFrame;
}

void CBTinValueProperty::SetPopDlgType( E_POP_DLG_TYPE type )
{
	m_eDlgType = type;
}


void CPropertiesWnd::SetTextProperty( CEqmDrawText* pText )
{
	m_eShowMode = E_SHOW_TEXT;
	m_pText = pText;
	if(m_pText)
	{
		m_wndPropList.RemoveAll();
		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("TextInfo"));
		//�ı���Ϣ
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("DefaultShow"), (_variant_t)m_pText->GetInitialstring().c_str(), _T("Ĭ���ı�")));
		//����
		LOGFONT lf;
		CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
		font->GetLogFont(&lf);
		lstrcpy(lf.lfFaceName, m_pText->GetFontName());
		pGroup1->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("����")));

		//��ɫ
		CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"), m_pText->GetFontColor(), NULL, _T("��ɫ"));
		pColorProp->EnableOtherButton(_T("Other..."));
		pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
		pGroup1->AddSubItem(pColorProp);

		//��С
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("FontSize"), (_variant_t)m_pText->GetFontSize(), _T("�����С")));

		//����
		CMFCPropertyGridProperty* pPropAlign = new CMFCPropertyGridProperty(_T("Alignment"), _T(""), _T("���뷽ʽ"));
		pPropAlign->AddOption(_T("���ж���"));
		pPropAlign->AddOption(_T("���ж���"));
		pPropAlign->AddOption(_T("���ж���"));
		pPropAlign->AddOption(_T("���϶���"));
		pPropAlign->AddOption(_T("���϶���"));
		pPropAlign->AddOption(_T("���϶���"));
		pPropAlign->AddOption(_T("���¶���"));
		pPropAlign->AddOption(_T("���¶���"));
		pPropAlign->AddOption(_T("���¶���"));
		pPropAlign->AllowEdit(FALSE);
		CString strAlign = pPropAlign->GetOption(m_pText->GetAlign());
		pPropAlign->SetValue((_variant_t)strAlign);
		pGroup1->AddSubItem(pPropAlign);


		//�߿�
		CMFCPropertyGridProperty* pPropBorder = new CMFCPropertyGridProperty(_T("Border"), _T(""), _T("�߿�"));
		pPropBorder->AddOption(_T("�ޱ߿�"));
		pPropBorder->AddOption(_T("��ͨ�߿�"));
		pPropBorder->AddOption(_T("�ڿ�׵�"));
		pPropBorder->AddOption(_T("3D���߿�"));
		pPropBorder->AddOption(_T("3D͹�߿�"));
		pPropBorder->AllowEdit(FALSE);
		CString strBorder = pPropBorder->GetOption(m_pText->GetBorderType());
		pPropBorder->SetValue((_variant_t)strBorder);
		pGroup1->AddSubItem(pPropBorder);
		pPropBorder->Enable(FALSE);

		//�߿򱳾�
		CMFCPropertyGridColorProperty* pBorderColorProp = new CMFCPropertyGridColorProperty(_T("BorderColor"), m_pText->GetBorderColor(), NULL, _T("�߿򱳾�"));
		pBorderColorProp->EnableOtherButton(_T("Other..."));
		pBorderColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
		pGroup1->AddSubItem(pBorderColorProp);

		//��
		CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
		pPropLayer->AddOption(_T("1��"));
		pPropLayer->AddOption(_T("2��"));
		pPropLayer->AddOption(_T("3��"));
		pPropLayer->AddOption(_T("4��"));
		pPropLayer->AddOption(_T("5��"));
		pPropLayer->AddOption(_T("6��"));
		pPropLayer->AddOption(_T("7��"));
		pPropLayer->AddOption(_T("8��"));
		pPropLayer->AddOption(_T("9��"));
		pPropLayer->AddOption(_T("10��"));
		pPropLayer->AllowEdit(FALSE);
		CString strLayer = pPropLayer->GetOption(m_pText->GetLayer());
		pPropLayer->SetValue((_variant_t)strLayer);
		pGroup1->AddSubItem(pPropLayer);

		//������
		CMFCPropertyGridProperty* pPropBKStyle = new CMFCPropertyGridProperty(_T("BKStyle"), _T(""), _T("������"));
		pPropBKStyle->AddOption(_T("��"));
		pPropBKStyle->AddOption(_T("�Զ���1"));
		pPropBKStyle->AddOption(_T("�Զ���2"));
		pPropBKStyle->AddOption(_T("�Զ���3"));
		pPropBKStyle->AddOption(_T("�Զ���4"));
		pPropBKStyle->AddOption(_T("�Զ���5"));
		pPropBKStyle->AddOption(_T("�Զ���6"));
		pPropBKStyle->AddOption(_T("�Զ���7"));
		pPropBKStyle->AddOption(_T("�Զ���8"));
		pPropBKStyle->AddOption(_T("�Զ���9"));
		pPropBKStyle->AddOption(_T("�Զ���10"));
		pPropBKStyle->AllowEdit(FALSE);
		CString strBKStyle = pPropBKStyle->GetOption(m_pText->GetTextBKType());
		pPropBKStyle->SetValue((_variant_t)strBKStyle);
		pGroup1->AddSubItem(pPropBKStyle);

		//��ʾ����
		CMFCPropertyGridProperty* pPropMode = new CMFCPropertyGridProperty(_T("EnumMode"), _T(""), _T("��ʾ����"));
		pPropMode->AddOption(_T("ö��ģʽ"));
		pPropMode->AddOption(_T("ֵģʽ"));
		pPropMode->AddOption(_T("ʱ��ģʽ"));
		pPropMode->AddOption(_T("�ַ���ģʽ"));
		pPropMode->AddOption(_T("������Ӽ��޸�ֵģʽ"));
		pPropMode->AddOption(_T("ֵģʽ����λ��ʾ"));
		pPropMode->AllowEdit(FALSE);
		CString strMode = pPropMode->GetOption(m_pText->GetTextShowMode());
		pPropMode->SetValue((_variant_t)strMode);
		pGroup1->AddSubItem(pPropMode);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		//ʱ�����  10
		CBTinValueProperty* pDateTimePoint = new CBTinValueProperty(_T("DateTimePoint"), (_variant_t)m_pText->GetDateTimePoint(), _T("ʱ�����"));
		pDateTimePoint->SetMainFrame(pMainFrame);
		pDateTimePoint->SetPopDlgType(E_POP_DLG_POINT);
		pGroup1->AddSubItem(pDateTimePoint);

		//ʱ������  11
		//pGroup1->AddSubItem(new CBTinValueProperty(_T("DateTimeSeries"), (_variant_t)m_pText->GetDateTime(), _T("ʱ������")));
		CBTinValueProperty* pDateTimeSeries = new CBTinValueProperty(_T("DateTimeSeries"), (_variant_t)m_pText->GetDateTime(), _T("ʱ������"));
		pDateTimeSeries->SetMainFrame(pMainFrame);
		pDateTimeSeries->SetPopDlgType(E_POP_DLG_POINT);
		pGroup1->AddSubItem(pDateTimeSeries);

		//��ʱ����� 12
		//pGroup1->AddSubItem(new CBTinValueProperty(_T("NonDateTimePoint"), (_variant_t)m_pText->GetPhysicalID().c_str(), _T("��ʱ�����")));
		CBTinValueProperty* pNonDateTimePoint = new CBTinValueProperty(_T("NonDateTimePoint"), (_variant_t)m_pText->GetPhysicalID().c_str(), _T("��ʱ�����"));
		pNonDateTimePoint->SetMainFrame(pMainFrame);
		pNonDateTimePoint->SetPopDlgType(E_POP_DLG_POINT);
		pGroup1->AddSubItem(pNonDateTimePoint);

		//ö�ٴ����� 13
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Enum Event"), (_variant_t) _T(""), _T("ö�ٴ����ã�0:�ر�|1:������:")));

		//С���� 14
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("DecimalPoint"), (_variant_t)m_pText->GetPointNum(), _T("С����")));

		//���� 15
		//CMFCPropertyGridProperty* pPropAttri = new CMFCPropertyGridProperty(_T("TextAttri"), _T(""), _T("����"));
		//pPropAttri->AddOption(_T("��������"));
		//pPropAttri->AddOption(_T("�¶�"));
		//pPropAttri->AddOption(_T("ʪ��"));
		//pPropAttri->AddOption(_T("SP"));
		//pPropAttri->AllowEdit(FALSE);
		//CString strAttri = pPropAttri->GetOption(m_pText->GetTextAttribute());
		//pPropAttri->SetValue((_variant_t)strAttri);
		//pGroup1->AddSubItem(pPropAttri);


		//����
		if (m_pText->GetTextShowMode() == 0)//�ı�
		{
			pGroup1->GetSubItem(13)->Enable(TRUE);
			pGroup1->GetSubItem(14)->Enable(FALSE);
			pGroup1->GetSubItem(10)->Enable(FALSE);
			pGroup1->GetSubItem(11)->Enable(FALSE);
			pGroup1->GetSubItem(12)->Enable(TRUE);
		}
		else if(m_pText->GetTextShowMode() == 1)//ֵ
		{
			pGroup1->GetSubItem(13)->Enable(FALSE);
			pGroup1->GetSubItem(14)->Enable(TRUE);
			pGroup1->GetSubItem(10)->Enable(FALSE);
			pGroup1->GetSubItem(11)->Enable(FALSE);
			pGroup1->GetSubItem(12)->Enable(TRUE);
		}
		else if (m_pText->GetTextShowMode() == 2)//ʱ��
		{
			pGroup1->GetSubItem(13)->Enable(FALSE);
			pGroup1->GetSubItem(14)->Enable(FALSE);
			pGroup1->GetSubItem(10)->Enable(TRUE);
			pGroup1->GetSubItem(11)->Enable(TRUE);
			pGroup1->GetSubItem(12)->Enable(FALSE);
		}
		else if (m_pText->GetTextShowMode() == 3)//�ַ���
		{
			pGroup1->GetSubItem(13)->Enable(FALSE);
			pGroup1->GetSubItem(14)->Enable(FALSE);
			pGroup1->GetSubItem(10)->Enable(FALSE);
			pGroup1->GetSubItem(11)->Enable(FALSE);
			pGroup1->GetSubItem(12)->Enable(TRUE);
		}
		m_wndPropList.AddProperty(pGroup1);
	}
}




void CPropertiesWnd::SetPipeProperty( CEqmDrawPipe * pPipe )
{
	SetNull();
	m_eShowMode = E_SHOW_PIPE;
	m_pPipe = pPipe;
	if (pPipe)
	{
		m_wndPropList.RemoveAll();
		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));		
		pGroup1->AddSubItem(new CMFCPropertyGridColorProperty(_T("Fill Color"), pPipe->GetPipeColor().innercolor,NULL, _T("�����ɫ")));
		pGroup1->AddSubItem(new CMFCPropertyGridColorProperty(_T("Edge Color"), pPipe->GetPipeColor().outercolor,NULL, _T("�߿���ɫ")));
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(m_pPipe->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			pGroup1->AddSubItem(pPropLayer);
		}		
		{
			CMFCPropertyGridProperty* pPropWidth = new CMFCPropertyGridProperty(_T("Width"), _T(""), _T("���"));
			pPropWidth->AddOption(_T("1"));
			pPropWidth->AddOption(_T("2"));
			pPropWidth->AddOption(_T("3"));
			pPropWidth->AddOption(_T("4"));
			pPropWidth->AddOption(_T("5"));
			pPropWidth->AddOption(_T("6"));
			pPropWidth->AddOption(_T("7"));
			pPropWidth->AddOption(_T("8"));
			pPropWidth->AddOption(_T("9"));
			pPropWidth->AddOption(_T("10"));
			pPropWidth->AddOption(_T("11"));
			pPropWidth->AddOption(_T("12"));
			pPropWidth->AddOption(_T("13"));
			pPropWidth->AddOption(_T("14"));		
			
			pPropWidth->AllowEdit(FALSE);
			
			CString strWidth = pPropWidth->GetOption(pPipe->GetPipeWidth()-1); //14?
			pPropWidth->SetValue((_variant_t)strWidth);
			pGroup1->AddSubItem(pPropWidth);
		}
		
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Current Details"));		
		
		{
			m_vecPipePoints = pPipe->GetDataComIDVector();
			CString tmp;
			if (m_vecPipePoints.size()==0)
			{
				tmp = _T("��");
			} 
			else
			{
				tmp = getMerged(m_vecPipePoints,L",");
			}
			
			CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Bind Points"), tmp.GetString(), _T("��㣩"));
			pPropBindPoints->AddOption(_T("ѡ��"));
			pPropBindPoints->AddOption(_T("���"));			
			pPropBindPoints->AllowEdit(TRUE);
			pGroup2->AddSubItem(pPropBindPoints);
		}
		
		{
			_tagPipeDirection dir = pPipe->GetDirection();
			
			CString tmp;
			if (dir==e_Forward)
			{
				tmp = _T("����");
			}
			else
			{
				tmp = _T("����");
			}

			CMFCPropertyGridProperty* pPropDirection = new CMFCPropertyGridProperty(_T("Direction"), tmp, _T("����"));
			pPropDirection->AddOption(_T("����"));
			pPropDirection->AddOption(_T("����"));			
			pPropDirection->AllowEdit(FALSE);
			pGroup2->AddSubItem(pPropDirection);
		}

		{
			int logic = pPipe->m_nLogic;
			CString tmp;
			switch(logic)
			{
			case 0: tmp = _T("��");
				break;
			case 1: tmp = _T("��");
				break;
			case 2: tmp = _T("��");
				break;
			//default:			
			}

			CMFCPropertyGridProperty* pPropLogic = new CMFCPropertyGridProperty(_T("Logical Operation"), tmp, _T("�߼���ϵ��"));
			pPropLogic->AddOption(_T("��"));
			pPropLogic->AddOption(_T("��"));
			pPropLogic->AddOption(_T("��"));		
			//pPropDirection->SetValue()
			pPropLogic->AllowEdit(FALSE);
			pGroup2->AddSubItem(pPropLogic);
			
		}

		{	
			CMFCPropertyGridProperty* pPropWType = new CMFCPropertyGridProperty(_T("Water Type"), _T(""), _T("ˮ�����ͣ�"));
			pPropWType->AddOption(_T("�䶳ˮ����ˮ"));
			pPropWType->AddOption(_T("��ȴˮ����ˮ"));
			pPropWType->AddOption(_T("��ˮ"));	
			pPropWType->AddOption(_T("��ˮ"));	
			pPropWType->AddOption(_T("�䶳ˮ����ˮ"));
			pPropWType->AddOption(_T("��ȴˮ����ˮ"));
			pPropWType->AddOption(_T("�Զ���1"));		
			pPropWType->AddOption(_T("�Զ���2"));		
			pPropWType->AddOption(_T("�Զ���3"));		
			pPropWType->AddOption(_T("�Զ���4"));		
			pPropWType->AddOption(_T("�Զ���5"));		
			pPropWType->AddOption(_T("�Զ���6"));		
			pPropWType->AddOption(_T("�Զ���7"));		
			pPropWType->AddOption(_T("�Զ���8"));		
			pPropWType->AddOption(_T("�Զ���9"));		
			pPropWType->AddOption(_T("�Զ���10"));

			CString strType = pPropWType->GetOption(pPipe->GetPipeType());
			pPropWType->SetValue(strType);
			pPropWType->AllowEdit(FALSE);
			pGroup2->AddSubItem(pPropWType);
		}
		{
			CBTinValueProperty* pPic = new CBTinValueProperty(_T("Texture"), (_variant_t)(pPipe->GetPipeBitMapID()), _T("����"));
			pPic->SetMainFrame(pMainFrame);
			pPic->SetPopDlgType(E_POP_DLG_IMAGE);		
			pPic->AllowEdit(FALSE);
			pGroup2->AddSubItem(pPic);
		}
		{	
			CMFCPropertyGridProperty* pDensity = new CMFCPropertyGridProperty(_T("Density"), _T("����"), _T("ˮ���ܶȣ�"));
			pDensity->AddOption(_T("����"));
			pDensity->AddOption(_T("������"));
			pDensity->AddOption(_T("һ��"));
			pDensity->AddOption(_T("���ܼ�"));
			pDensity->AddOption(_T("�ܼ�"));			

			CString strType = pDensity->GetOption(pPipe->GetPipeDenSity());
			pDensity->SetValue(strType);
			pDensity->AllowEdit(FALSE);
			pGroup2->AddSubItem(pDensity);
		}
		{	
			CMFCPropertyGridProperty* pSpeed = new CMFCPropertyGridProperty(_T("Speed"), _T("��"), _T("ˮ���ٶȣ�"));
			pSpeed->AddOption(_T("��"));
			pSpeed->AddOption(_T("��"));
			pSpeed->AddOption(_T("��"));
					

			CString strType = pSpeed->GetOption(pPipe->GetPipeSpeed());
			pSpeed->SetValue(strType);
			pSpeed->AllowEdit(FALSE);
			pGroup2->AddSubItem(pSpeed);
		}
	
		m_wndPropList.AddProperty(pGroup1);
		m_wndPropList.AddProperty(pGroup2);

	}
}

CString CPropertiesWnd::getMerged( vector<CString> & vec,CString sepa = _T(",") )
{
	CString str,total;
	for(UINT idx=0; idx< vec.size(); idx++)
	{
		str = vec[idx];
		if (idx != vec.size()-1)
		{
			str += sepa;
		}
		total += str;
	}

	return total;
}



void CPropertiesWnd::ChangePipeProperty( CMFCPropertyGridProperty* pProp,CEqmDrawPipe * pPipe )
{
	m_pPipe = pPipe;
	if(m_pPipe && pProp)
	{

		CommandManager::Instance()->CallCommand(m_pPipe,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
	//	COleVariant vBefore = pProp->GetOriginalValue();  //��ʼֵ  
		//int nNum = pProp->GetData();
//		if(vChange != vBefore)
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	

// 			if(strParamName == _T("ElementName"))	//ͼԪ����
// 			{
// 				CString strNewValue = vChange.bstrVal;      //��COleVariant��CString  
// 				m_pDevice->SetEqmName(strNewValue.GetString());
// 			}
			if(strParamName == _T("Layer"))		//ͼ��
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("1��"))
				{
					m_pPipe->SetLayer(0);
				}
				else if(strNewValue == _T("2��"))
				{
					m_pPipe->SetLayer(1);
				}
				else if(strNewValue == _T("3��"))
				{
					m_pPipe->SetLayer(2);
				}
				else if(strNewValue == _T("4��"))
				{
					m_pPipe->SetLayer(3);
				}
				else if(strNewValue == _T("5��"))
				{
					m_pPipe->SetLayer(4);
				}
				else if(strNewValue == _T("6��"))
				{
					m_pPipe->SetLayer(5);
				}
				else if(strNewValue == _T("7��"))
				{
					m_pPipe->SetLayer(6);
				}
				else if(strNewValue == _T("8��"))
				{
					m_pPipe->SetLayer(7);
				}
				else if(strNewValue == _T("9��"))
				{
					m_pPipe->SetLayer(8);
				}
				else if(strNewValue == _T("10��"))
				{
					m_pPipe->SetLayer(9);
				}	
			}
			else if(strParamName == _T("Bind Points"))	
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("���"))
				{
					m_vecPipePoints.clear();
					m_pPipe->SetDataPointList(m_vecPipePoints);
					pProp->SetValue(_T("��"));
				}
				else if(strNewValue == _T("ѡ��"))
				{
					SelectDataComId dlg;
					dlg.m_vecDataName = m_vecPipePoints;
					dlg.SetPointMap(&(pMainFrame->m_plcpointmap));
					if(IDOK==dlg.DoModal())
					{
						m_vecPipePoints = dlg.m_vecDataName;
						m_pPipe->SetDataPointList(m_vecPipePoints);
						CString tmp = getMerged(m_vecPipePoints);
						pProp->SetValue(tmp.GetString());
					}
					else
					{						
						pProp->SetValue(getMerged(m_vecPipePoints));
					}
				}
				else
				{
					CString strFlag(_T(","));
					CString strSrc(strNewValue);
					strSrc.TrimRight(strFlag);
					strSrc += strFlag;
					vector<CString> vecPoint;
					while (TRUE)
					{
						int nPos = strSrc.Find(strFlag);
						if (-1 == nPos)
						{
							break;
						}
						CString strTemp = strSrc.Left(nPos);
						vecPoint.push_back(strTemp);
						strSrc = strSrc.Mid(nPos + 1);
					}
					pProp->SetValue(strNewValue.GetString());
					m_pPipe->SetDataPointList(vecPoint);
				}
			}
			else if (strParamName == _T("Fill Color"))
			{
				PipeColor tmp = pPipe->GetPipeColor();
				tmp.innercolor = vChange.intVal;
				pPipe->SetPipeColor(tmp);
			}
			else if (strParamName == _T("Edge Color"))
			{
				PipeColor tmp = pPipe->GetPipeColor();
				tmp.outercolor = vChange.intVal;
				pPipe->SetPipeColor(tmp);
			}
			else if (strParamName == _T("Width"))
			{				
				CString tmp = vChange.bstrVal;
				m_pPipe->SetPipeWidth(_ttoi(tmp.GetString()));				
			}
			else if (strParamName == _T("Direction") )
			{
				CString dir = vChange.bstrVal;
				if (dir==_T("����"))
				{
					pPipe->SetDirection(e_Forward);
				}
				else
				{
					pPipe->SetDirection(e_Reverse);
				}
			}
			else if (strParamName == _T("Logical Operation"))
			{
				CString logic = vChange.bstrVal;
				int nlogic(0);
				if (logic == _T("��") )
				{
					nlogic = 1;
				} 
				else if(logic == _T("��") )
				{
					nlogic = 2;
				}				
				pPipe->m_nLogic = nlogic;
			}
			else if (strParamName == _T("Water Type"))
			{
				int size = pProp->GetOptionCount();
				CString option;
				CString choosed = vChange.bstrVal;
				for (int i = 0;i<size;++i)
				{
					option = pProp->GetOption(i);					
					if (option == choosed)
					{
						pPipe->SetPipeType(i);
						break;
					}
				}
			}
			else if (strParamName == _T("Texture"))
			{				
				m_pPipe->SetPipeBitMapID(vChange.lVal);
			}
			else if (strParamName == _T("Density"))
			{
				int size = pProp->GetOptionCount();
				CString option;
				CString choosed = vChange.bstrVal;
				for (int i = 0;i<size;++i)
				{
					option = pProp->GetOption(i);					
					if (option == choosed)
					{
						pPipe->SetPipeDenSity(i);
						break;
					}
				}
			}
			else if (strParamName == _T("Speed"))
			{
				int size = pProp->GetOptionCount();
				CString option;
				CString choosed = vChange.bstrVal;
				for (int i = 0;i<size;++i)
				{
					option = pProp->GetOption(i);					
					if (option == choosed)
					{
						pPipe->SetPipeSpeed(i);
						break;
					}
				}
			}
			pPipe->SetNeedSave(TRUE);
			pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
		}
	}

}

void CPropertiesWnd::SetNull()
{
	m_pPipe = NULL;
	m_pDevice = NULL;
	m_pText = NULL;
	m_pBase = NULL;
	m_pButton = NULL;
	m_pBend = NULL;
	m_pBoard = NULL;
	m_pMapNavigate = NULL;
	m_pDiagnoseGraphic = NULL;
	m_pTime = NULL;
	m_pLine = NULL;
	m_pPie = NULL;
	m_pBar = NULL;
	m_pROI = NULL;
	m_pTrend = NULL;
	m_pCheck = NULL;
	m_pEmbed = NULL;
	m_pExecute = NULL;
	m_pArea = NULL;
	m_pScrawl = NULL;
	m_pLiquid = NULL;
	m_pTbar = NULL;
	m_pProgrs = NULL;
	m_pNav = NULL;
	m_pAnalysis = NULL;
	m_pReport = NULL;
	m_pTemp = NULL;
	m_pRuler = NULL;
	m_pPolygon = NULL;
	m_pPlaneTempeDistr = NULL;
	m_pDottedLineFrm = NULL;
	m_pGeneralComponent = NULL;
	m_pStraightLine = NULL;
}

void CPropertiesWnd::SetButtonProperty( CEqmDrawButton * pButton )
{
	SetNull();
	m_eShowMode = E_SHOW_BUTTON;
	m_pButton = pButton;
	if (pButton)
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		m_wndPropList.RemoveAll();	

		{
			CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Background Pic"));

			int nComm = 0;
			int nDown = 0;
			int nOver = 0;
			int nDisable = 0;
			int nChecked = 0;
			pButton->Get5PicID(nComm, nDisable, nDown, nOver,nChecked);
			{
				CBTinValueProperty* pPic1 = new CBTinValueProperty(_T("Common"), (_variant_t)nComm, _T("����궯��ʱ��ʾ"));
				pPic1->SetMainFrame(pMainFrame);
				pPic1->SetPopDlgType(E_POP_DLG_IMAGE);		
				pPic1->AllowEdit(FALSE);
				pGroup2->AddSubItem(pPic1);
			}
			{
				CBTinValueProperty* pPic2 = new CBTinValueProperty(_T("Down"), (_variant_t)nDown, _T("��갴�º���ʾ"));
				pPic2->SetMainFrame(pMainFrame);
				pPic2->SetPopDlgType(E_POP_DLG_IMAGE);		
				pPic2->AllowEdit(FALSE);
				pGroup2->AddSubItem(pPic2);
			}
			{
				CBTinValueProperty* pPic3 = new CBTinValueProperty(_T("Over"), (_variant_t)nOver, _T("����ڰ�ť��ʱ��ʾ"));
				pPic3->SetMainFrame(pMainFrame);
				pPic3->SetPopDlgType(E_POP_DLG_IMAGE);		
				pPic3->AllowEdit(FALSE);
				pGroup2->AddSubItem(pPic3);
			}
			{
				CBTinValueProperty* pPic4 = new CBTinValueProperty(_T("Disabled"), (_variant_t)nDisable, _T("��ť������ʱ��ʾ"));
				pPic4->SetMainFrame(pMainFrame);
				pPic4->SetPopDlgType(E_POP_DLG_IMAGE);		
				pPic4->AllowEdit(FALSE);
				pGroup2->AddSubItem(pPic4);
			}
			{
				CBTinValueProperty* pPic5 = new CBTinValueProperty(_T("Checked"), (_variant_t)nChecked, _T("��ť��ѡ��ʱ��ʾ"));
				pPic5->SetMainFrame(pMainFrame);
				pPic5->SetPopDlgType(E_POP_DLG_IMAGE);		
				pPic5->AllowEdit(FALSE);
				pGroup2->AddSubItem(pPic5);
			}
			m_wndPropList.AddProperty(pGroup2);
		}
		{
			CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Text"));
			pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Content"), (_variant_t)pButton->GetTextContent(), _T("��������")));
			pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Font Size"), (_variant_t)pButton->GetTextSize(), _T("���ִ�С")));
			pGroup3->AddSubItem(new CMFCPropertyGridColorProperty(_T("Color"), (_variant_t)pButton->GetTextColor(),NULL, _T("������ɫ")));

			m_wndPropList.AddProperty(pGroup3);
		}
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Value"), (_variant_t) pButton->GetSettingValue(), _T("����ֵ")));
			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(m_pButton->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup1->AddSubItem(pPropLayer);
			}	
			{
				CString current;				
				int curID = pButton->GetLinkPageID();
				CMFCPropertyGridProperty* pPropLink = new CMFCPropertyGridProperty(_T("LinkPage"), _T("��ָ��"), _T("ҳ�泬����"));

				CEqmDrawProject& project = pMainFrame->m_project;
				vector<CEqmDrawPage*>& page_list = project.GetPageList();				
				for (size_t i=0;i<page_list.size();++i)
				{
					if (page_list[i])
					{
						const CString strPageName = page_list[i]->GetPageName().c_str();
						const int nPageID = page_list[i]->GetID();
						if (nPageID == curID)
						{
							current = strPageName;
						}
						pPropLink->AddOption(strPageName);
					}
				}
				pPropLink->SetValue(current);
				pPropLink->AllowEdit(FALSE);
				pGroup1->AddSubItem(pPropLink);
			}			
			{			
				CString tmp = pButton->GetBindSetPointName();	

				//pGroup1->AddSubItem(new CBTinValueProperty(_T("NonDateTimePoint"), (_variant_t)m_pText->GetPhysicalID().c_str(), _T("��ʱ�����")));
				//CBTinValueProperty* pNonDateTimePoint = new CBTinValueProperty(_T("BindPoint"), (_variant_t)tmp.GetString(), _T("�����õ�"));
				//pNonDateTimePoint->SetMainFrame(pMainFrame);
				//pNonDateTimePoint->SetPopDlgType(E_POP_DLG_POINT);
				//pGroup1->AddSubItem(pNonDateTimePoint);

				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("BindPoint"), (_variant_t)tmp.GetString(), _T("�����õ�")));
				//pPropBindPoints->AddOption(_T("ѡ��"));
				//pPropBindPoints->AddOption(_T("���"));			
				//pPropBindPoints->AllowEdit(TRUE);			
				//pGroup1->AddSubItem(pPropBindPoints);
			}
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Description"), (_variant_t) pButton->GetOperation(), _T("��ť����¼�����")));				
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Download Enable"), (_variant_t) pButton->GetDownloadEnable(), _T("�����ļ�Enable")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Download Name"), (_variant_t) pButton->GetDownloadName(), _T("�����ļ���ʶ��")));
			m_wndPropList.AddProperty(pGroup1);
		}
		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Active Config"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("��Ч״̬���ã�"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));			
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}
			
			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Expression List"));
			vector<_tagRelationExpress> relationVec =  pButton->GetRelationVec();
			int size = relationVec.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Expression"), relation2Cstring(relationVec[i]), _T("�󶨵㣬�߼���ϵ��ƥ��ֵ"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);			
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);

			{				
				int index = pButton->GetRelation();
				CString tmp(_T("��"));
				if (index)
				{
					tmp = _T("��");
				}				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Relation"), tmp,_T("���ʽ֮��Ĺ�ϵ"));
				pPropBindPoints->AddOption(_T("��"));
				pPropBindPoints->AddOption(_T("��"));			
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}	

			m_wndPropList.AddProperty(pGroup4);
		}	
	}
}

void CPropertiesWnd::ChangeButtonProperty( CMFCPropertyGridProperty* pProp,CEqmDrawButton * pButton )
{	
	if(pButton && pProp)
	{
		m_pButton = pButton;
		CommandManager::Instance()->CallCommand(m_pButton,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  

		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());

		CMFCPropertyGridProperty *pParent = pProp->GetParent();
		if (pParent)
		{
			CString parentName = pParent->GetName();
			if(parentName ==_T("Background Pic"))
			{
				int nComm = 0;
				int nDown = 0;
				int nOver = 0;
				int nDisable = 0;
				int nChecked = 0;
				pButton->Get5PicID(nComm, nDisable, nDown, nOver,nChecked);
				long newID = vChange.lVal;


				if (strParamName == _T("Common"))
				{
					nComm = newID;
					pParent->GetSubItem(0)->SetValue((_variant_t)newID);
				} 
				else if (strParamName == _T("Down"))
				{
					nDown = newID;
					pParent->GetSubItem(1)->SetValue((_variant_t)newID);
				}
				else if (strParamName == _T("Over"))
				{
					nOver = newID;
					pParent->GetSubItem(2)->SetValue((_variant_t)newID);
				}
				else if (strParamName == _T("Disabled"))
				{
					nDisable = newID;
					pParent->GetSubItem(3)->SetValue((_variant_t)newID);
				}
				else if (strParamName == _T("Checked"))
				{
					nChecked = newID;
					pParent->GetSubItem(4)->SetValue((_variant_t)newID);
				}		

				//�Զ����
				if (nComm == 0)
				{
					nComm = newID;
					pParent->GetSubItem(0)->SetValue((_variant_t)newID);
				}
				if (nDown == 0)
				{
					nDown = newID;
					pParent->GetSubItem(1)->SetValue((_variant_t)newID);
				}
				if (nOver == 0)
				{
					nOver = newID;
					pParent->GetSubItem(2)->SetValue((_variant_t)newID);
				}
				if (nDisable == 0)
				{
					nDisable = newID;
					pParent->GetSubItem(3)->SetValue((_variant_t)newID);

				}
				if (nChecked == 0)
				{
					nChecked = newID;
					pParent->GetSubItem(4)->SetValue((_variant_t)newID);
				}		

				pButton->Set5PicID(nComm,nDisable,nDown,nOver,nChecked);
			
			}
		//	if(parentName == _T("Active Config"))
			{
				
			}
		}

		if (strParamName == _T("Content"))
		{
			CString strNewValue = vChange.bstrVal;  
			pButton->SetTextContent(strNewValue);
		} 
		else if (strParamName == _T("Font Size"))
		{
			pButton->SetTextSize(vChange.intVal);
		}
		else if (strParamName == _T("Color"))
		{
			pButton->SetTextColor(vChange.intVal);
		}
		else if (strParamName == _T("Value"))
		{
			CString strNewValue = vChange.bstrVal;  
			pButton->SetSettingValue(strNewValue);
		}
		else if (strParamName == _T("Layer"))
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pButton->SetLayer(i);
					break;
				}
			}

		}
		else if (strParamName == _T("LinkPage"))
		{
			CString current = vChange.bstrVal;				
			int curID = -1;
			CEqmDrawProject& project = pMainFrame->m_project;
			vector<CEqmDrawPage*>& page_list = project.GetPageList();				
			for (size_t i=0;i<page_list.size();++i)
			{
				if (page_list[i])
				{
					const CString strPageName = page_list[i]->GetPageName().c_str();
					if (strPageName == current)
					{
						curID = page_list[i]->GetID();
						break;
					}					
				}
			}
			pButton->SetLinkPageID(curID);
		}
		else if (strParamName == _T("BindPoint"))
		{
			CString strNewValue = vChange.bstrVal;
			

			pButton->SetBindSetPointName(strNewValue);
		}
		else if (strParamName == _T("Description"))
		{
			CString strNewValue = vChange.bstrVal;
			pButton->SetOperation(strNewValue);
		}
		else if (strParamName == _T("Operation"))
		{
			CString tmpstr = vChange.bstrVal; 
			if (tmpstr == _T("����"))
			{
				CRelationItemSettingDlg dlg;
				dlg.m_strPointName = L"�󶨵�";
				dlg.m_nRelation = 0;
				dlg.m_fResult = 0;
				if (dlg.DoModal() == IDOK)
				{
					_tagRelationExpress tmp;
					tmp.strPointName = dlg.m_strPointName;
					tmp.nRelationExpress = dlg.m_nRelation;
					tmp.fResult = dlg.m_fResult;
					pButton->InsertExpress(tmp);
// 					{				
// 						CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("expression"), relation2Cstring(tmp), _T("�󶨵㣬�߼���ϵ��ƥ��ֵ��"));
// 						pPropBindPoints->AddOption(_T("�޸�"));
// 						pPropBindPoints->AddOption(_T("ɾ��"));			
// 						pPropBindPoints->AllowEdit(FALSE);			
// 						//pParent->AddSubItem(pPropBindPoints);
// 					}	

				}
			}					
		}
		else if (strParamName == _T("Expression") )
		{
			CString tmpstr = vChange.bstrVal; 
			if (tmpstr == _T("�޸�"))
			{
				CString tmp = vBefore.bstrVal;
				vector<wstring> vecPos;
				Project::Tools::SplitStringByChar(tmp, L' ', vecPos);
				if (vecPos.size() == 3)
				{
					CRelationItemSettingDlg dlg;
					dlg.m_strPointName = vecPos[0].c_str();	
					int tmp = rel2Int(vecPos[1].c_str());
					if (tmp<0&&tmp>5)
					{
						tmp = 0;
					}
					dlg.m_nRelation = tmp;
					dlg.m_fResult = _wtoi(vecPos[2].c_str());								
					if (dlg.DoModal() == IDOK)
					{
						_tagRelationExpress tmp;
						tmp.strPointName = dlg.m_strPointName;
						tmp.nRelationExpress = dlg.m_nRelation;
						tmp.fResult = dlg.m_fResult;

						pButton->DeleteExpress(tmp.strPointName);			
						pButton->InsertExpress(tmp);								
						pProp->SetValue(relation2Cstring(tmp));
					}
				}						
			}
			else if (tmpstr ==_T("ɾ��"))
			{
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{
					CString tmp = vBefore.bstrVal;
					vector<wstring> vecPos;
					Project::Tools::SplitStringByChar(tmp, L' ', vecPos);
					pButton->DeleteExpress(vecPos[0].c_str());
					//pParent->RemoveSubItem(pProp);
				}

			}
		}
		else if (strParamName == _T("Relation"))
		{
			CString tmp = vChange.bstrVal;
			int nRel(0);
			if (tmp == L"��")
			{
				nRel = 1;
			}
			pButton->SetRelation(nRel);
		}
		else if (_T("Download Enable") == strParamName)
		{
			CString tmp = vChange.bstrVal;
			pButton->SetDownloadEnable(tmp);
		}
		else if (_T("Download Name") == strParamName)
		{
			CString tmp = vChange.bstrVal;
			pButton->SetDownloadName(tmp);
		}

		SetButtonProperty(pButton);
		pButton->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
	}
}

CString CPropertiesWnd::relation2Cstring( _tagRelationExpress rel )
{
	CString result = rel.strPointName;
	switch (rel.nRelationExpress)
	{
	case 0:	result += " = ";
		break;
	case 1:	result += " > ";
		break;
	case 2:	result += " < ";
		break;
	case 3:	result += " >= ";
		break;
	case 4:	result += " <= ";
		break;
	case 5:	result += " != ";
		break;
	default:
		result += "  ";
	}	
	
	CString tmp;
	tmp.Format(L"%.2f",rel.fResult);	
	result+= tmp;
	return result;
}

int CPropertiesWnd::rel2Int( CString rel )
{
	int result = -1;
	if (rel == L"=")
	{
		result = 0;
	} 
	else if (rel == L">")
	{
		result = 1;
	}
	else if (rel == L"<")
	{
		result = 2;
	}
	else if (rel == L">=")
	{
		result = 3;
	}
	else if (rel == L"<=")
	{
		result = 4;
	}
	else if (rel == L"!=")
	{
		result = 5;
	}
	return result;
}

void CPropertiesWnd::SetBendProperty( CEqmDrawBend *pBend )
{
	SetNull();
	m_eShowMode = E_SHOW_BEND;
	m_pBend = pBend;
	if (pBend)
	{
		m_wndPropList.RemoveAll();	
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Bend Pipe"));

		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pBend->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			pGroup->AddSubItem(pPropLayer);
		}
		{
			int index = angle2Index(pBend->GetRotateAngle());
			CMFCPropertyGridProperty* pAngel = new CMFCPropertyGridProperty(_T("Angle"), L"",_T("��ת�Ƕ�"));		
			pAngel->AddOption(L"����ת");
			pAngel->AddOption(L"˳ʱ����ת90��");
			pAngel->AddOption(L"˳ʱ����ת180��");
			pAngel->AddOption(L"˳ʱ����ת270��");
			pAngel->AddOption(L"��ʱ����ת90��");
			pAngel->AddOption(L"��ʱ����ת180��");
			pAngel->AddOption(L"��ʱ����ת270��");
			CString tmp = pAngel->GetOption(index);
			pAngel->SetValue(tmp);
			pAngel->AllowEdit(FALSE);			
			pGroup->AddSubItem(pAngel);

		}		
		m_wndPropList.AddProperty(pGroup);
	}

}

void CPropertiesWnd::ChangeBendProperty( CMFCPropertyGridProperty* pProp,CEqmDrawBend *pBend )
{
	if(pBend && pProp)
	{
		m_pBend = pBend;
		CommandManager::Instance()->CallCommand(m_pBend,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == _T("Angle"))
		{
			CString strAngle = vChange.bstrVal;
			int size = pProp->GetOptionCount();
			for (int i = 0; i< size;i++)
			{
				CString tmp = pProp->GetOption(i);
				if (tmp == strAngle)
				{
					int nAngle(i);
					if (nAngle>3)
					{
						nAngle = 3-nAngle;
					}					
					pBend->SetRotateAngle(nAngle*90);
					break;
				}
			}			
		}
		else if(strParamName == _T("Layer"))			//��
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pBend->SetLayer(i);
					break;
				}
			}
		}

		//SetBendProperty(pBend);
		pBend->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
	}

}

int CPropertiesWnd::angle2Index( int angle )
{
	angle/=90;
	if (angle < 0)
	{
		angle = 3-angle;
	}
	return angle;
}

void CPropertiesWnd::SetBoardProperty( CEqmDrawDashBoard *pBoard )
{
	SetNull();
	m_eShowMode = E_SHOW_DASHBOARD;
	m_pBoard = pBoard;
	if (pBoard)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Dashboard"));

		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pBoard->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			pGroup->AddSubItem(pPropLayer);
		}
		{
			CBTinValueProperty* pPoint = new CBTinValueProperty(_T("Bind Point"), (_variant_t)(pBoard->GetBindPointName()), _T("�󶨵�����"));
			pPoint->SetMainFrame(pMainFrame);
			pPoint->SetPopDlgType(E_POP_DLG_POINT);		
			pPoint->AllowEdit(FALSE);
			pGroup->AddSubItem(pPoint);
		}

		{
			CString tmp;
			if (pBoard->GetMaxBindFlag())
			{
				tmp = pBoard->GetMaxString();
			} 
			else
			{
				tmp.Format(L"%.1f",pBoard->GetMaxValue());
			}
			CMFCPropertyGridProperty* pMax= new CMFCPropertyGridProperty(_T("Ceiling"),tmp,_T("���ֵ"));	
			pMax->AddOption(L"��ֵ");
			pMax->AddOption(L"���");
			if (pBoard->GetMaxBindFlag())
			{
				pMax->AllowEdit(FALSE);
			}
			pGroup->AddSubItem(pMax);
		}

		{
			CString tmp;
			if (pBoard->GetMinBindFlag())
			{
				tmp = pBoard->GetMinString();
			} 
			else
			{
				tmp.Format(L"%.1f",pBoard->GetMinValue());
			}
			CMFCPropertyGridProperty* pMax= new CMFCPropertyGridProperty(_T("Floor"),tmp,_T("��Сֵ"));	
			pMax->AddOption(L"��ֵ");
			pMax->AddOption(L"���");
			if (pBoard->GetMinBindFlag())
			{
				pMax->AllowEdit(FALSE);
			}
			pGroup->AddSubItem(pMax);
		}
		
		m_wndPropList.AddProperty(pGroup);
	}
}

void CPropertiesWnd::ChangeBoardProperty( CMFCPropertyGridProperty* pProp,CEqmDrawDashBoard *pBoard )
{
	if(pBoard && pProp)
	{
		m_pBoard = pBoard;
		CommandManager::Instance()->CallCommand(m_pBoard,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if(strParamName == _T("Layer"))			//��
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pBoard->SetLayer(i);
					break;
				}
			}
		}

		else if (strParamName == _T("Bind Point"))
		{
			CString tmp = vChange.bstrVal;
			pBoard->SetBindPointName(tmp);
		}
		else if (strParamName == _T("Ceiling"))
		{
			CString tmpOp = vChange.bstrVal;
			if (tmpOp == L"���")
			{
				CDataComConfigDlg dlg(pMainFrame->m_plcpointmap);
				if (dlg.DoModal() == IDOK)
				{
					pBoard->SetMaxBindFlag(true);
					pBoard->SetMaxString(dlg.m_strPointName);					
				}
				
				pProp->SetValue(pBoard->GetMaxString());
				pProp->AllowEdit(FALSE);
			}
			else if (tmpOp == L"��ֵ")
			{
				pBoard->SetMaxBindFlag(false);
				pProp->AllowEdit(TRUE);
				
				CString tmp;
				tmp.Format(L"%.1f",pBoard->GetMaxValue());
				pProp->SetValue(tmp);
			}
			else
			{
				double tmpvalue = VariantToDoubleWithDefault(vChange,0);
				pBoard->SetMaxValue(tmpvalue);

				CString tmp;
				tmp.Format(L"%.1f",tmpvalue);	
				pProp->SetValue(tmp);				
			}
			
			//SetBoardProperty(pBoard);//why outside crash
		}
		else if (strParamName == _T("Floor"))
		{
			CString tmpOp = vChange.bstrVal;
			if (tmpOp == L"���")
			{
				CDataComConfigDlg dlg(pMainFrame->m_plcpointmap);
				if (dlg.DoModal() == IDOK)
				{
					pBoard->SetMinBindFlag(true);
					pBoard->SetMinString(dlg.m_strPointName);					
				}

				pProp->SetValue(pBoard->GetMinString());
				pProp->AllowEdit(FALSE);
			}
			else if (tmpOp == L"��ֵ")
			{
				pBoard->SetMinBindFlag(false);
				pProp->AllowEdit(TRUE);

				CString tmp;
				tmp.Format(L"%.1f",pBoard->GetMinValue());
				pProp->SetValue(tmp);
			}
			else
			{
				double tmpvalue = VariantToDoubleWithDefault(vChange,0);
				pBoard->SetMinValue(tmpvalue);

				CString tmp;
				tmp.Format(L"%.1f",tmpvalue);	
				pProp->SetValue(tmp);				
			}

			//SetBoardProperty(pBoard);//why outside crash
		}

	
		pBoard->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	

	}
}

void CPropertiesWnd::SetDiagnoseGraphicProperty( CEqmDrawDiagnoseGraphic *pDiagnoseGraphic )
{
	SetNull();
	m_eShowMode = E_SHOW_DIAGNOSEGRAPHIC;
	m_pDiagnoseGraphic = pDiagnoseGraphic;
	if (m_pDiagnoseGraphic)
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		m_wndPropList.RemoveAll();	

		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("DiagnoseGraphic"));
			{						
				CMFCPropertyGridProperty* pPropDiagnoseGraphicItem = new CMFCPropertyGridProperty(_T("DiagnoseOperation"), _T("�޲���"), _T("���ͼ�����ã�"));
				pPropDiagnoseGraphicItem->AddOption(_T("�޲���"));
				pPropDiagnoseGraphicItem->AddOption(_T("������"));			
				pPropDiagnoseGraphicItem->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropDiagnoseGraphicItem);
			}

			CMFCPropertyGridProperty* pItemGroup = new CMFCPropertyGridProperty(_T("DiagnoseGraphic Item List"));
			vector<_tagDiagnoseItem>& vec =  m_pDiagnoseGraphic->GetItemVec();
			for (size_t i=0;i<vec.size();++i)
			{
				CString strValue;
				strValue.Format(_T("%s,%s,%d,%d"),vec[i].strItemName,vec[i].strPointName,vec[i].color,vec[i].vecItemInLine.size());
				CMFCPropertyGridProperty* pPropDiagnoseItem = new CMFCPropertyGridProperty(_T("DiagnoseItem"), (_variant_t)strValue, _T("������,����,��ɫ,������)"));
				pPropDiagnoseItem->AddOption(_T("�޸�"));
				pPropDiagnoseItem->AddOption(_T("ɾ��"));			
				pPropDiagnoseItem->AllowEdit(FALSE);			
				pItemGroup->AddSubItem(pPropDiagnoseItem);
			}
			pGroup4->AddSubItem(pItemGroup);
			m_wndPropList.AddProperty(pGroup4);
		}
	}
		
}

void CPropertiesWnd::ChangepDiagnoseGraphicProperty( CMFCPropertyGridProperty* pProp,CEqmDrawDiagnoseGraphic *pDiagnoseGraphic )
{
	if(pDiagnoseGraphic && pProp)
	{
		m_pDiagnoseGraphic = pDiagnoseGraphic;
		CommandManager::Instance()->CallCommand(m_pDiagnoseGraphic,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  

		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());

		CMFCPropertyGridProperty *pParent = pProp->GetParent();
		if (pParent)
		{
			CString parentName = pParent->GetName();
			if (strParamName == _T("DiagnoseOperation"))
			{
				CString tmpstr = vChange.bstrVal; 
				if (tmpstr == _T("������"))
				{
					_tagDiagnoseItem pItem;
					pItem.color = pMainFrame->m_colorArray[m_nColorIndex];
					CDiagnoseItemPropertySettingDlg dlg(&pItem);
					if (dlg.DoModal() == IDOK)
					{
						CString strValue;
						strValue.Format(_T("%s,%s,%d,%d"),pItem.strItemName,pItem.strPointName,pItem.color,pItem.vecItemInLine.size());
						CMFCPropertyGridProperty* pPropDiagnoseItem = new CMFCPropertyGridProperty(_T("DiagnoseItem"), (_variant_t)strValue, _T("������,����,��ɫ,������)"));
						pPropDiagnoseItem->AddOption(_T("�޸�"));
						pPropDiagnoseItem->AddOption(_T("ɾ��"));			
						pPropDiagnoseItem->AllowEdit(FALSE);	

						pProp->GetParent()->GetSubItem(1)->AddSubItem(pPropDiagnoseItem);
						pProp->GetParent()->GetSubItem(1)->Expand(FALSE);
						pProp->GetParent()->GetSubItem(1)->Expand(TRUE);

						m_pDiagnoseGraphic->InsertItem(pItem);
						m_pDiagnoseGraphic->SetNeedSave(true);

						++m_nColorIndex;
						if (m_nColorIndex >= 8)
						{
							m_nColorIndex = 0;
						}
					}
				}	
				pProp->SetValue((_variant_t)_T("�޲���"));
			}
			else if (strParamName == _T("DiagnoseItem") )
			{
				CString tmpstr = vChange.bstrVal; 
				if (tmpstr == _T("�޸�"))
				{
					CString tmp = vBefore.bstrVal;
					vector<wstring> vecPos;
					Project::Tools::SplitStringByChar(tmp, L',', vecPos);
					if (vecPos.size() == 4)
					{
						_tagDiagnoseItem* pItem = m_pDiagnoseGraphic->GetItem(vecPos[0].c_str());
						CDiagnoseItemPropertySettingDlg dlg(pItem);
						if (dlg.DoModal() == IDOK)
						{
							CString strValue;
							strValue.Format(_T("%s,%s,%d,%d"),pItem->strItemName,pItem->strPointName,pItem->color,pItem->vecItemInLine.size());
							pProp->SetValue((_variant_t)strValue);

							m_pDiagnoseGraphic->SetNeedSave(true);
						}
						else
						{
							pProp->SetValue(vBefore.bstrVal);
						}
					}						
				}
				else if (tmpstr ==_T("ɾ��"))
				{
					if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
					{
						CString tmp = vBefore.bstrVal;
						vector<wstring> vecPos;
						Project::Tools::SplitStringByChar(tmp, L',', vecPos);
						if (vecPos.size() == 4)
						{
							m_pDiagnoseGraphic->DeleteItem(vecPos[0].c_str());
							pParent->RemoveSubItem(pProp);
							pParent->Expand(FALSE);
							pParent->Expand(TRUE);
						}
					}
				}
			}
			m_pDiagnoseGraphic->SetNeedSave(TRUE);
			pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
		}
	}
}

void CPropertiesWnd::SetMapNavigateProperty( CEqmDrawMapNavigate *pMapNavigate )
{
	SetNull();
	m_eShowMode = E_SHOW_MAPNAVIGATE;
	m_pMapNavigate = pMapNavigate;
	if (pMapNavigate)
	{
		m_wndPropList.RemoveAll();
		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("MapNavigate"));	
		//��ͼ������
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("MapScale"), (_variant_t)pMapNavigate->GetScaleRuler(), _T("��ͼ������")));
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pMapNavigate->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			pGroup1->AddSubItem(pPropLayer);
		}	

		//��ͼ���
		CMFCPropertyGridProperty* pGroupMakers = new CMFCPropertyGridProperty(_T("Makers"));
		pGroup1->AddSubItem(pGroupMakers);

		//
		CMFCPropertyGridProperty* pPropMakerEdit = new CMFCPropertyGridProperty(_T("MakerOperation"), _T("�޲���"), _T("��ͼ��ǲ���"));
		pPropMakerEdit->AddOption(_T("�޲���"));
		pPropMakerEdit->AddOption(_T("��ӵ�ͼ���"));
		pPropMakerEdit->AllowEdit(FALSE);
		pGroupMakers->AddSubItem(pPropMakerEdit);

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		vector<CMapPageInfo> vec = m_pMapNavigate->GetMapPageInfo();
		for (size_t i=0;i<vec.size();++i)
		{
			CString strValue;
			strValue.Format(_T("%0.6f,%0.6f,%0.6f,%0.6f,%d"),vec[i].GetLongitudeValue(1),vec[i].GetLatitudeValue(1),vec[i].GetLongitudeValue(2),vec[i].GetLatitudeValue(2),vec[i].GetPageId());

			//��ͼ���
			CBTinValueProperty* pMaker = new CBTinValueProperty(_T("Maker"), (_variant_t)strValue, _T("��ͼ���(����1,γ��1,����2,γ��2,��תҳ��ID)"));
			pMaker->SetMainFrame(pMainFrame);
			pMaker->SetPopDlgType(E_POP_DLG_ADD_NAVI_ITEM);
			pGroupMakers->AddSubItem(pMaker);
		}

		m_wndPropList.AddProperty(pGroup1);
	}
}

void CPropertiesWnd::ChangeMapNavigateProperty( CMFCPropertyGridProperty* pProp,CEqmDrawMapNavigate *pMapNavigate )
{
	m_pMapNavigate = pMapNavigate;
	if(m_pMapNavigate && pProp)
	{
		CommandManager::Instance()->CallCommand(m_pMapNavigate,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ı�֮ǰ��ֵ  
		int nNum = pProp->GetData();
		if(vChange != vBefore)
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
			if(strParamName == _T("MapScale"))	//��ͼ����
			{
				m_pMapNavigate->SetScaleRuler(vChange.intVal);
			}
			else if(strParamName == _T("Layer"))		//ͼ��
			{
				int size = pProp->GetOptionCount();
				CString option;
				CString choosed = vChange.bstrVal;
				for (int i = 0;i<size;++i)
				{
					option = pProp->GetOption(i);					
					if (option == choosed)
					{
						m_pMapNavigate->SetLayer(i);
						break;
					}
				}
			}
			else if(strParamName == _T("MakerOperation"))	//��ͼ��ǲ���
			{
				CString strNewValue = vChange.bstrVal;
				if(strNewValue == _T("��ӵ�ͼ���"))
				{
					CMapPageInfo mapinfo;
					CMapNavigateItemSetDlg dlg(mapinfo);
					if (dlg.DoModal() == IDOK)
					{
						mapinfo = dlg.GetMapPageInfo();
						CString strValue;
						strValue.Format(_T("%0.6f,%0.6f,%0.6f,%0.6f,%d"),mapinfo.GetLongitudeValue(1),mapinfo.GetLatitudeValue(1),mapinfo.GetLongitudeValue(2),mapinfo.GetLatitudeValue(2),mapinfo.GetPageId());

						//�¼�
						CBTinValueProperty* pMaker = new CBTinValueProperty(_T("Maker"), (_variant_t)strValue, _T("��ͼ���(����1,γ��1,����2,γ��2,��תҳ��ID)"));
						pMaker->SetMainFrame(pMainFrame);
						pMaker->SetPopDlgType(E_POP_DLG_ADD_NAVI_ITEM);
						pProp->GetParent()->AddSubItem(pMaker);
						pProp->GetParent()->Expand(FALSE);
						pProp->GetParent()->Expand(TRUE);
					}
				}
				pProp->SetValue((_variant_t)_T("�޲���"));
			}
			else if(strParamName == _T("Maker"))	//��ͼ��ǲ���
			{
				CMFCPropertyGridProperty* pParent = pProp->GetParent();
				if(pParent)
				{
					COleVariant vOperation = pParent->GetSubItem(0)->GetValue();
					CString strOperation = vOperation.bstrVal;
					CString strNewValue = vChange.bstrVal;

					//ɾ��
					if(strNewValue == _T(""))
					{							
						if(pParent->RemoveSubItem((CMFCPropertyGridProperty*)pProp))
						{
							pParent->Expand(FALSE);
							pParent->Expand(TRUE);
							pParent->Redraw();
						}							
					}

					//����
					vector<CMapPageInfo> vecMarker;
					int nCount = pParent->GetSubItemsCount();
					CString strValue;
					for(int i=1;i<nCount; ++i)
					{
						strValue = pParent->GetSubItem(i)->GetValue();
						vector<wstring> vecResult;
						Project::Tools::SplitStringByChar(strValue,L',',vecResult);
						if(vecResult.size()==5)
						{
							CMapPageInfo	item;
							item.SetLongitudeValue(1, _wtof(vecResult[0].c_str()));
							item.SetLongitudeValue(2, _wtof(vecResult[2].c_str()));
							item.SetLatitudeValue(1, _wtof(vecResult[1].c_str()));
							item.SetLatitudeValue(2, _wtof(vecResult[3].c_str()));
							item.SetPageId(_wtoi(vecResult[4].c_str()));
							vecMarker.push_back(item);
						}
					}
					m_pMapNavigate->SetMapPageInfo(vecMarker);
				}
			}		
			m_pMapNavigate->SetNeedSave(TRUE);
			pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
		}
	}
}
void CPropertiesWnd::SetTimePickerProperty( CEqmDrawTimeSelect *pTime )
{
	SetNull();
	m_eShowMode = E_SHOW_TIME_SELECT;
	m_pTime = pTime;
	if (pTime)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Time Picker"));
		{
			CBTinValueProperty* pPoint = new CBTinValueProperty(_T("Bind Point"), (_variant_t)(pTime->GetPointName()), _T("�󶨵�����"));
			pPoint->SetMainFrame(pMainFrame);
			pPoint->SetPopDlgType(E_POP_DLG_POINT);		
			pPoint->AllowEdit(FALSE);
			pGroup->AddSubItem(pPoint);
		}

		{			
			CMFCPropertyGridProperty* pMax= new CMFCPropertyGridProperty(_T("Permission"),L"",_T("��д����"));	
			pMax->AddOption(L"��");
			pMax->AddOption(L"д");
			pMax->SetValue(pMax->GetOption(pTime->GetRWProperty()));
			pMax->AllowEdit(FALSE);
			pGroup->AddSubItem(pMax);
		}
	
		{
			CMFCPropertyGridProperty* pSelectTimeDataType= new CMFCPropertyGridProperty(_T("TimeDataType"),L"",_T("ʱ�����ݸ�ʽ"));	
			pSelectTimeDataType->AddOption(L"���ں�ʱ��");
			pSelectTimeDataType->AddOption(L"������");
			pSelectTimeDataType->AddOption(L"��ʱ��");
			pSelectTimeDataType->SetValue(pSelectTimeDataType->GetOption(pTime->GetTimeDataFormat()));
			pSelectTimeDataType->AllowEdit(FALSE);
			pGroup->AddSubItem(pSelectTimeDataType);
		}
		m_wndPropList.AddProperty(pGroup);
	}
}

void CPropertiesWnd::ChangeTimePickerProperty( CMFCPropertyGridProperty* pProp,CEqmDrawTimeSelect *pTime )
{
	if(pTime && pProp)
	{
		m_pTime = pTime;
		CommandManager::Instance()->CallCommand(m_pTime,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == _T("Bind Point"))
		{
			CString tmp = vChange.bstrVal;
			pTime->SetPointName(tmp);
		}
		else if(strParamName == _T("Permission"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"��")
			{
				pTime->SetRWProperty(0);
			}
			else
			{
				pTime->SetRWProperty(1);
			}
		}
		else if (strParamName == _T("TimeDataType"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"���ں�ʱ��")
			{
				pTime->SetTimeDataFormat(0);
			}
			else if(strValue == L"������")
			{
				pTime->SetTimeDataFormat(1);
			}
			else if(strValue == L"��ʱ��")
			{
				pTime->SetTimeDataFormat(2);
			}
		}
		
		//SetBendProperty(pBend);
		pTime->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	

	}

}

void CPropertiesWnd::SetLineChartProperty( CEqmDrawLineGraphic* pLine )
{
	SetNull();
	m_eShowMode = E_SHOW_LINE_CHART;
	m_pLine  = pLine;
	if (pLine)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		
		//1
		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Basic"));			
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Title"), (_variant_t)pLine->GetTitleName(), _T("ͼ�����")));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Refresh Interval :ms"),(_variant_t)pLine->GetUpdateInterval(), _T("ˢ�¼��")));
			pGroup->AddSubItem(new CMFCPropertyGridColorProperty(_T("Text Color"), (_variant_t)pLine->GetTextColor(),NULL, _T("ͼ��������ɫ")));

			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(pLine->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup->AddSubItem(pPropLayer);
			}

			m_wndPropList.AddProperty(pGroup);
		}

		//2
		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Axis"));			
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("X-axis Title"), (_variant_t)pLine->GetXName(), _T("��������")));
			
			{				
				CMFCPropertyGridProperty* pScale= new CMFCPropertyGridProperty(_T("X-axis Unit"),(_variant_t)pLine->GetXUnit(),_T("���ᵥλ"));	
				pScale->AddOption(L"��");
				pScale->AddOption(L"�Զ�");
				pScale->AddOption(L"��");
				pScale->AddOption(L"��");
				pScale->AddOption(L"��-��");
				pScale->AddOption(L"��-��");
				pScale->AddOption(L"��-�� ʱ");
				pScale->AddOption(L"ʱ:��");	
				pScale->AllowEdit(FALSE);				
				pGroup->AddSubItem(pScale);
			}
			
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Y-axis Title"), (_variant_t)pLine->GetYName(), _T("��������")));

			{
				CMFCPropertyGridProperty* pMax= new CMFCPropertyGridProperty(_T("Y-axis Ceiling"),L"",_T("��������"));	
				pMax->AddOption(L"�Զ�");
				pMax->AddOption(L"��ֵ");

				if (pLine->GetMaxCheck())
				{
					pMax->AllowEdit(FALSE);
					pMax->SetValue(L"�Զ�");
				}
				else
				{
					CString tmp;
					tmp.Format(L"%f",pLine->GetMaxValue());
					pMax->SetValue(tmp);
				}
				pGroup->AddSubItem(pMax);
			}
			{
				CMFCPropertyGridProperty* pMin= new CMFCPropertyGridProperty(_T("Y-axis Floor"),L"",_T("��������"));	
				pMin->AddOption(L"�Զ�");
				pMin->AddOption(L"��ֵ");

				if (pLine->GetMinCheck())
				{
					pMin->AllowEdit(FALSE);
					pMin->SetValue(L"�Զ�");
				}
				else
				{
					CString tmp;
					tmp.Format(L"%f",pLine->GetMinValue());
					pMin->SetValue(tmp);
					pMin->AllowEdit(TRUE);
				}
				pGroup->AddSubItem(pMin);
			}

			m_wndPropList.AddProperty(pGroup);
		}

		//3
		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Items"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ��������"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));	
			//	pPropBindPoints->AddOption(_T("ɾ��"));	
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Item List"));
			vector<_Graph_Item_Property> itemVec =  pLine->GetItemVector();
			int size = itemVec.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)itemVec[i].strItemName, _T("ͼ������ƣ���ɫ���󶨵㣬�洢����"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);			
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);	

			m_wndPropList.AddProperty(pGroup4);
		}		
	}
}

void CPropertiesWnd::ChangeLineChartProperty( CMFCPropertyGridProperty* pProp,CEqmDrawLineGraphic* pLine )
{
	if(pLine && pProp)
	{		
		CommandManager::Instance()->CallCommand(pLine,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		//1
		if (strParamName == _T("Title"))
		{
			CString strValue = vChange.bstrVal;
			pLine->SetTitleName(strValue);
		}
		else if (strParamName == _T("Refresh Interval :ms"))
		{
			 long newInt = VariantToInt32WithDefault(vChange,0);
			 if (newInt < 0)
			 {
				 newInt = VariantToInt32WithDefault(vBefore,0);
			 }
			 pLine->SetUpdateInterval(newInt);
			 pProp->SetValue((_variant_t)newInt);			
		}
		else if (strParamName == _T("Text Color"))
		{
			m_pLine->SetTextColor(vChange.intVal);
		}
		else if(strParamName == _T("Layer"))			//��
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pLine->SetLayer(i);
					break;
				}
			}
		}

		//2
		else if (strParamName == _T("X-axis Title"))
		{
			pLine->SetXName(vChange.bstrVal);
		}
		else if (strParamName == _T("X-axis Unit"))
		{
			pLine->SetXUnit(vChange.bstrVal);
		}
		else if (strParamName == _T("Y-axis Title"))
		{
			pLine->SetYName(vChange.bstrVal);
		}
		else if (strParamName == _T("Y-axis Ceiling"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�Զ�")
			{
				pLine->SetMaxCheck(1);
				pProp->AllowEdit(FALSE);
			}
			else if (strValue == L"��ֵ")
			{
				pLine->SetMaxCheck(0);			
				CString tmp;
				tmp.Format(L"%f",pLine->GetMaxValue());		
				SetLineChartProperty(pLine);//to allow edit immediately
			/*	pProp->SetValue(tmp);
				pProp->AllowEdit(TRUE);*/
			}
			else
			{
				double newV = VariantToDoubleWithDefault(vChange,0);
				pLine->SetMaxValue(newV);

				CString tmp;
				tmp.Format(L"%f",newV);
				pProp->SetValue(tmp);
				//pProp->SetValue((_variant_t)newV);
			}
		}
		else if (strParamName == _T("Y-axis Floor"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�Զ�")
			{
				pLine->SetMinCheck(1);
				pProp->AllowEdit(FALSE);
			}
			else if (strValue == L"��ֵ")
			{
				pLine->SetMinCheck(0);				
				CString tmp;
				tmp.Format(L"%f",pLine->GetMinValue());				
				pProp->SetValue(tmp);
				pProp->AllowEdit(TRUE);
			}
			else
			{
				double newV = VariantToDoubleWithDefault(vChange,0);
				pLine->SetMinValue(newV);

				CString tmp;
				tmp.Format(L"%f",newV);
				pProp->SetValue(tmp);			
			}
		}
		
		//3
		else if (strParamName == _T("Operation"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"����")
			{
				CChartItemPropertyDlg dlg;
				dlg.SetPointMap(&pMainFrame->m_plcpointmap);
				dlg.m_strItemName = L"����";
				dlg.m_nColor = 0;
				dlg.m_strBindPointName = L"�󶨵���";
				CString strType	= L"�洢����";
				dlg.m_nBindPointStoreType = _wtoi(strType);
				if (dlg.DoModal() == IDOK)
				{
					_Graph_Item_Property item;
					item.strItemName = dlg.m_strItemName;
					item.strPointName = dlg.m_strBindPointName;
					item.color = dlg.m_nColor;
					item.nStoreType = dlg.m_nBindPointStoreType;
					pLine->InsertGraphItem(item);
					SetLineChartProperty(pLine);
				}
			} 			
		}
		else if (strParamName == _T("Item"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�޸�")
			{
				CString strBefore = vBefore.bstrVal;
				//CString strBefore =( (_Graph_Item_Property*)(pProp->GetData()))->strItemName;
				vector<_Graph_Item_Property> &itemVec = pLine->GetItemVector();
				int size = itemVec.size();
				for (int i = 0;i<size;i++)
				{
					if (itemVec[i].strItemName == strBefore)
					{
						CChartItemPropertyDlg dlg;
						dlg.SetPointMap(&pMainFrame->m_plcpointmap);
						dlg.m_strItemName = itemVec[i].strItemName;
						dlg.m_nColor = itemVec[i].color;
						dlg.m_strBindPointName = itemVec[i].strPointName;
						dlg.m_nBindPointStoreType = itemVec[i].nStoreType;
						if (dlg.DoModal() == IDOK)
						{
							//pLine->DeleteGraphItem(strBefore);

							_Graph_Item_Property item;
							item.strItemName = dlg.m_strItemName;
							item.strPointName = dlg.m_strBindPointName;
							item.color = dlg.m_nColor;
							item.nStoreType = dlg.m_nBindPointStoreType;
							itemVec[i] = item;
							//pLine->InsertGraphItem(item);
							//pProp->SetData((DWORD)&(pLine->m_vecItem.back()));
							//pProp->SetValue(item.strItemName);
							SetLineChartProperty(pLine);
						}
						else
						{
							pProp->SetValue(strBefore);
						}
						break;
					}
				}				
			} 
			else if (strValue ==L"ɾ��")
			{
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{
					CString strBefore = vBefore.bstrVal;
					pLine->DeleteGraphItem(strBefore);
					SetLineChartProperty(pLine);
				}
				else
				{
					pProp->SetValue(vBefore);
				}
			}		
		}

	//	SetLineChartProperty(pLine);
		pLine->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	

	}
}

void CPropertiesWnd::SetBarChartProperty( CEqmDrawBarGraphic* pBar)
{
	SetNull();
	m_eShowMode = E_SHOW_BAR_CHART;
	m_pBar  = pBar;
	if (pBar)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();		

		//1
		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Basic"));			
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Title"), (_variant_t)pBar->GetTitleName(), _T("ͼ�����")));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Refresh Interval :ms"),(_variant_t)pBar->GetUpdateInterval(), _T("ˢ�¼��")));
			pGroup->AddSubItem(new CMFCPropertyGridColorProperty(_T("Text Color"), (_variant_t)pBar->GetTextColor(),NULL, _T("ͼ��������ɫ")));
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pBar->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			pGroup->AddSubItem(pPropLayer);
		}

			m_wndPropList.AddProperty(pGroup);
		}

		//type
		E_BAR_TYPE barT = pBar->GetType();
		{
			CString tmp(L"��ͨ��״ͼ");
			if (barT == eBarCompare)
			{
				tmp = L"�Ƚ���״ͼ";
			} 
			
			CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Type"),tmp, _T("��״ͼ����"));
			pPropBindPoints->AddOption(_T("��ͨ��״ͼ"));
			pPropBindPoints->AddOption(_T("�Ƚ���״ͼ"));	
			//	pPropBindPoints->AddOption(_T("ɾ��"));	
			pPropBindPoints->AllowEdit(FALSE);			
			m_wndPropList.AddProperty(pPropBindPoints);
		}

		if (barT == eBarNormal)
		{
			//2
			{
				CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Axis"));			
				pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("X-axis Title"), (_variant_t)pBar->GetXName(), _T("��������")));

				{				
					CMFCPropertyGridProperty* pScale= new CMFCPropertyGridProperty(_T("X-axis Unit"),(_variant_t)pBar->GetXUnit(),_T("���ᵥλ"));	
					pScale->AddOption(L"��");
					pScale->AddOption(L"�Զ�");
					pScale->AddOption(L"��");
					pScale->AddOption(L"��");
					pScale->AddOption(L"��-��");
					pScale->AddOption(L"��-��");
					pScale->AddOption(L"��-�� ʱ");
					pScale->AddOption(L"ʱ:��");	
					pScale->AllowEdit(FALSE);				
					pGroup->AddSubItem(pScale);
				}

				pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Y-axis Title"), (_variant_t)pBar->GetYName(), _T("��������")));

				// 			{
				// 				CMFCPropertyGridProperty* pMax= new CMFCPropertyGridProperty(_T("Y-axis Ceiling"),L"",_T("��������"));	
				// 				pMax->AddOption(L"�Զ�");
				// 				pMax->AddOption(L"��ֵ");
				// 
				// 				if (pBar->GetMaxCheck())
				// 				{
				// 					pMax->AllowEdit(FALSE);
				// 					pMax->SetValue(L"�Զ�");
				// 				}
				// 				else
				// 				{
				// 					CString tmp;
				// 					tmp.Format(L"%f",pBar->GetMaxValue());
				// 					pMax->SetValue(tmp);
				// 				}
				// 				pGroup->AddSubItem(pMax);
				// 			}
				// 			{
				// 				CMFCPropertyGridProperty* pMin= new CMFCPropertyGridProperty(_T("Y-axis Floor"),L"",_T("��������"));	
				// 				pMin->AddOption(L"�Զ�");
				// 				pMin->AddOption(L"��ֵ");
				// 
				// 				if (pBar->GetMinCheck())
				// 				{
				// 					pMin->AllowEdit(FALSE);
				// 					pMin->SetValue(L"�Զ�");
				// 				}
				// 				else
				// 				{
				// 					CString tmp;
				// 					tmp.Format(L"%f",pBar->GetMinValue());
				// 					pMin->SetValue(tmp);
				// 					pMin->AllowEdit(TRUE);
				// 				}
				// 				pGroup->AddSubItem(pMin);
				// 			}
				m_wndPropList.AddProperty(pGroup);
			}

			//3
			{
				CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Items"));
				{						
					CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ�������ã�"));
					pPropBindPoints->AddOption(_T("�޲���"));
					pPropBindPoints->AddOption(_T("����"));	
					//	pPropBindPoints->AddOption(_T("ɾ��"));	
					pPropBindPoints->AllowEdit(FALSE);			
					pGroup4->AddSubItem(pPropBindPoints);
				}

				CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Item List"));
				vector<_Graph_Item_Property> itemVec =  pBar->GetItemVector();
				int size = itemVec.size();
				for (int i = 0;i<size;++i)
				{				
					CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)itemVec[i].strItemName, _T("ͼ������ƣ���ɫ���󶨵㣬�洢����"));
					pPropBindPoints->AddOption(_T("�޸�"));
					pPropBindPoints->AddOption(_T("ɾ��"));			
					pPropBindPoints->AllowEdit(FALSE);			
					pSubGroup->AddSubItem(pPropBindPoints);
				}	
				pGroup4->AddSubItem(pSubGroup);	

				m_wndPropList.AddProperty(pGroup4);
			}	

			//5
			{
				CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Time Binding"));
				{					
					CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Point"), (_variant_t)pBar->GetStartPoint(), _T("��ʱ���"));
					pPropBindPoints->AddOption(_T("ѡ��"));
					pPropBindPoints->AddOption(_T("���"));	
					//	pPropBindPoints->AddOption(_T("ɾ��"));	
					pPropBindPoints->AllowEdit(FALSE);			
					pGroup4->AddSubItem(pPropBindPoints);
				}
				{
					
					CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Time Precision"), _T(""), _T("ʱ�侫������"));
					pPropBindPoints->AddOption(_T("����"));
					pPropBindPoints->AddOption(_T("Сʱ"));	
					pPropBindPoints->AddOption(_T("��"));
					pPropBindPoints->AddOption(_T("��"));
					pPropBindPoints->AddOption(_T("��"));
					pPropBindPoints->AddOption(_T("��"));
					//	pPropBindPoints->AddOption(_T("ɾ��"));	
					pPropBindPoints->SetValue(pPropBindPoints->GetOption(pBar->GetQueryType()));
					pPropBindPoints->AllowEdit(FALSE);			
					pGroup4->AddSubItem(pPropBindPoints);
				}
				m_wndPropList.AddProperty(pGroup4);
			}
		}
		else if (barT == eBarCompare)
		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Compare Points"));
			vector<_Graph_Item_Property> &vecItem = pBar->GetItemVector();
			if (vecItem.size()!=2)
			{
				vecItem.clear();
				_Graph_Item_Property item;
				item.color = 0;
				item.strItemName = L"";
				item.strPointName = L"";
				vecItem.push_back(item);
				vecItem.push_back(item);
			}
			{
				CString tmp(L"");
				if (vecItem.size()>0)
				{
					tmp = vecItem[0].strPointName;
				}
					
				CBTinValueProperty* pPic4 = new CBTinValueProperty(_T("Item1 Point"), tmp, _T("��1������"));
				pPic4->SetMainFrame(pMainFrame);
				pPic4->SetPopDlgType(E_POP_DLG_POINT);		
				pPic4->AllowEdit(FALSE);
				pGroup->AddSubItem(pPic4);
			}
			{
				CString tmp(L"");
				if (vecItem.size()>0)
				{
					tmp = vecItem[0].strItemName;
				}
				pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Item1 Name"),tmp, _T("��1����")));
			}
			
			{
				CString tmp(L"");
				if (vecItem.size()>1)
				{
					tmp = vecItem[1].strPointName;
				}

				CBTinValueProperty* pPic4 = new CBTinValueProperty(_T("Item2 Point"), tmp, _T("��2������"));
				pPic4->SetMainFrame(pMainFrame);
				pPic4->SetPopDlgType(E_POP_DLG_POINT);		
				pPic4->AllowEdit(FALSE);
				pGroup->AddSubItem(pPic4);
			}
			{
				CString tmp(L"");
				if (vecItem.size()>1)
				{
					tmp = vecItem[1].strItemName;
				}
				pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Item2 Name"),tmp, _T("��2����")));
			}
			m_wndPropList.AddProperty(pGroup);
		}		
	}
}


void CPropertiesWnd::ChangebarChartProperty( CMFCPropertyGridProperty* pProp,CEqmDrawBarGraphic* pBar )
{
	if(pBar && pProp)
	{			
		CommandManager::Instance()->CallCommand(pBar,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		//1
		if (strParamName == _T("Title"))
		{
			CString strValue = vChange.bstrVal;
			pBar->SetTitleName(strValue);
		}
		else if(strParamName == _T("Layer"))			//��
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pBar->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == _T("Refresh Interval :ms"))
		{
			 long newInt = VariantToInt32WithDefault(vChange,0);
			 if (newInt < 0)
			 {
				 newInt = VariantToInt32WithDefault(vBefore,0);
			 }
			 pBar->SetUpdateInterval(newInt);
			 pProp->SetValue((_variant_t)newInt);			
		}
		else if (strParamName == _T("Text Color"))
		{
			pBar->SetTextColor(vChange.intVal);
		}
		else if (strParamName ==L"Type" )
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"��ͨ��״ͼ")
			{
				pBar->SetType(eBarNormal);
			} 
			else if (strValue == L"�Ƚ���״ͼ")
			{
				pBar->SetType(eBarCompare);
			}
			pBar->ClearGraphItem();
			SetBarChartProperty(pBar);
		}
		else
		{
			E_BAR_TYPE barT = pBar->GetType();
			if (barT == eBarNormal)
			{
				//2
				if (strParamName == _T("X-axis Title"))
				{
					pBar->SetXName(vChange.bstrVal);
				}
				else if (strParamName == _T("X-axis Unit"))
				{
					pBar->SetXUnit(vChange.bstrVal);
				}
				else if (strParamName == _T("Y-axis Title"))
				{
					pBar->SetYName(vChange.bstrVal);
				}
				// 		else if (strParamName == _T("Y-axis Ceiling"))
				// 		{
				// 			CString strValue = vChange.bstrVal;
				// 			if (strValue == L"�Զ�")
				// 			{
				// 				pLine->SetMaxCheck(1);
				// 				pProp->AllowEdit(FALSE);
				// 			}
				// 			else if (strValue == L"��ֵ")
				// 			{
				// 				pLine->SetMaxCheck(0);			
				// 				CString tmp;
				// 				tmp.Format(L"%f",pLine->GetMaxValue());		
				// 				SetLineChartProperty(pLine);//to allow edit immediately
				// 			/*	pProp->SetValue(tmp);
				// 				pProp->AllowEdit(TRUE);*/
				// 			}
				// 			else
				// 			{
				// 				double newV = VariantToDoubleWithDefault(vChange,0);
				// 				pLine->SetMaxValue(newV);
				// 
				// 				CString tmp;
				// 				tmp.Format(L"%f",newV);
				// 				pProp->SetValue(tmp);
				// 				//pProp->SetValue((_variant_t)newV);
				// 			}
				// 		}
				// 		else if (strParamName == _T("Y-axis Floor"))
				// 		{
				// 			CString strValue = vChange.bstrVal;
				// 			if (strValue == L"�Զ�")
				// 			{
				// 				pLine->SetMinCheck(1);
				// 				pProp->AllowEdit(FALSE);
				// 			}
				// 			else if (strValue == L"��ֵ")
				// 			{
				// 				pLine->SetMinCheck(0);				
				// 				CString tmp;
				// 				tmp.Format(L"%f",pLine->GetMinValue());				
				// 				pProp->SetValue(tmp);
				// 				pProp->AllowEdit(TRUE);
				// 			}
				// 			else
				// 			{
				// 				double newV = VariantToDoubleWithDefault(vChange,0);
				// 				pLine->SetMinValue(newV);
				// 
				// 				CString tmp;
				// 				tmp.Format(L"%f",newV);
				// 				pProp->SetValue(tmp);			
				// 			}
				// 		}

				//3
				else if (strParamName == _T("Operation"))
				{
					CString strValue = vChange.bstrVal;
					if (strValue == L"����")
					{
						CChartItemPropertyDlg dlg;
						dlg.SetPointMap(&pMainFrame->m_plcpointmap);
						dlg.m_strItemName = L"����";
						dlg.m_nColor = 0;
						dlg.m_strBindPointName = L"�󶨵���";
						CString strType	= L"�洢����";
						dlg.m_nBindPointStoreType = _wtoi(strType);
						if (dlg.DoModal() == IDOK)
						{
							_Graph_Item_Property item;
							item.strItemName = dlg.m_strItemName;
							item.strPointName = dlg.m_strBindPointName;
							item.color = dlg.m_nColor;
							item.nStoreType = dlg.m_nBindPointStoreType;
							pBar->InsertGraphItem(item);
							SetBarChartProperty(pBar);
						}
					} 			
				}
				else if (strParamName == _T("Item"))
				{
					CString strValue = vChange.bstrVal;
					if (strValue == L"�޸�")
					{
						CString strBefore = vBefore.bstrVal;
						//CString strBefore =( (_Graph_Item_Property*)(pProp->GetData()))->strItemName;
						vector<_Graph_Item_Property> &itemVec = pBar->GetItemVector();
						int size = itemVec.size();
						for (int i = 0;i<size;i++)
						{
							if (itemVec[i].strItemName == strBefore)
							{
								CChartItemPropertyDlg dlg;
								dlg.SetPointMap(&pMainFrame->m_plcpointmap);
								dlg.m_strItemName = itemVec[i].strItemName;
								dlg.m_nColor = itemVec[i].color;
								dlg.m_strBindPointName = itemVec[i].strPointName;
								dlg.m_nBindPointStoreType = itemVec[i].nStoreType;
								if (dlg.DoModal() == IDOK)
								{
									//pBar->DeleteGraphItem(strBefore);
									
									_Graph_Item_Property item;
									item.strItemName = dlg.m_strItemName;
									item.strPointName = dlg.m_strBindPointName;
									item.color = dlg.m_nColor;
									item.nStoreType = dlg.m_nBindPointStoreType;
									itemVec[i] = item;
									//pBar->InsertGraphItem(item);
									//pProp->SetData((DWORD)&(pLine->m_vecItem.back()));
									//pProp->SetValue(item.strItemName);
									SetBarChartProperty(pBar);
								}
								else
								{
									pProp->SetValue(strBefore);
								}
								break;
							}
						}				
					} 
					else if (strValue ==L"ɾ��")
					{
						if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
						{
							CString strBefore = vBefore.bstrVal;
							pBar->DeleteGraphItem(strBefore);
							SetBarChartProperty(pBar);
						}
						else
						{
							pProp->SetValue(vBefore);
						}
					}		
				}

				//5
				else if (strParamName == _T("Point"))
				{
					CString strValue = vChange.bstrVal;
					if (strValue == L"ѡ��")
					{
						CDataComConfigDlg dlg(pMainFrame->m_plcpointmap);
						if (dlg.DoModal() == IDOK)
						{							
							pBar->SetStartPoint(dlg.m_strPointName);
						}										
					} 
					else if(strValue == L"���")
					{
						pBar->SetStartPoint(L"");
					}
					pProp->SetValue((_variant_t)pBar->GetStartPoint());	
				}
				else if (strParamName == _T("Time Precision"))
				{
					int size = pProp->GetOptionCount();
					for (int i = 0;i<size;i++)
					{
						CString strValue = vChange.bstrVal;
						CString tmp = pProp->GetOption(i);
						if(tmp == strValue)
						{
							pBar->SetQueryType(i);
							break;
						}
					}
				
				}
			} 
			else
			{
				if(strParamName == _T("Item1 Point"))
				{
					CString tmp = vChange.bstrVal;
					(pBar->GetGraphItem(0))->strPointName = tmp;
				} 
				else if(strParamName == _T("Item1 Name"))
				{
					(pBar->GetGraphItem(0))->strItemName = vChange.bstrVal;
				}
				else if(strParamName == _T("Item2 Point"))
				{
					(pBar->GetGraphItem(1))->strPointName = vChange.bstrVal;
				}
				else if(strParamName == _T("Item1 Name"))
				{
					(pBar->GetGraphItem(1))->strItemName = vChange.bstrVal;
				}
			}
		}
		//SetBendProperty(pBend);
		pBar->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	

	}
}

void CPropertiesWnd::SetPieChartProperty( CEqmDrawPieGraphic* pPie )
{
	SetNull();
	m_eShowMode = E_SHOW_PIE_CHART;
	m_pPie  = pPie;
	if (pPie)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();		

		//1
		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Basic"));			
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Title"), (_variant_t)pPie->GetTitleName(), _T("ͼ�����")));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Refresh Interval :ms"),(_variant_t)pPie->GetUpdateInterval(), _T("ˢ�¼��")));
			pGroup->AddSubItem(new CMFCPropertyGridColorProperty(_T("Text Color"), (_variant_t)pPie->GetTextColor(),NULL, _T("ͼ��������ɫ")));

		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pPie->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			pGroup->AddSubItem(pPropLayer);
		}
			m_wndPropList.AddProperty(pGroup);
		}
		//3
		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Items"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ��������"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));	
				//	pPropBindPoints->AddOption(_T("ɾ��"));	
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Item List"));
			vector<_Graph_Item_Property> itemVec =  pPie->GetItemVector();
			int size = itemVec.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)itemVec[i].strItemName, _T("ͼ������ƣ���ɫ���󶨵㣬�洢����"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);			
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);	

			m_wndPropList.AddProperty(pGroup4);
		}		
	}
}

void CPropertiesWnd::ChangePieChartProperty( CMFCPropertyGridProperty* pProp,CEqmDrawPieGraphic* pPie )
{
	if(pPie && pProp)
	{		
		CommandManager::Instance()->CallCommand(pPie,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		//1
		if (strParamName == _T("Title"))
		{
			CString strValue = vChange.bstrVal;
			pPie->SetTitleName(strValue);
		}
		else if (strParamName == _T("Refresh Interval :ms"))
		{
			long newInt = VariantToInt32WithDefault(vChange,0);
			if (newInt < 0)
			{
				newInt = VariantToInt32WithDefault(vBefore,0);
			}
			pPie->SetUpdateInterval(newInt);
			pProp->SetValue((_variant_t)newInt);			
		}
		else if (strParamName == _T("Text Color"))
		{
			pPie->SetTextColor(vChange.intVal);
		}
		else if(strParamName == _T("Layer"))			//��
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pPie->SetLayer(i);
					break;
				}
			}
		}

		//3
		else if (strParamName == _T("Operation"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"����")
			{
				CChartItemPropertyDlg dlg;
				dlg.SetPointMap(&pMainFrame->m_plcpointmap);
				dlg.m_strItemName = L"����";
				dlg.m_nColor = 0;
				dlg.m_strBindPointName = L"�󶨵���";
				CString strType	= L"�洢����";
				dlg.m_nBindPointStoreType = _wtoi(strType);
				if (dlg.DoModal() == IDOK)
				{
					_Graph_Item_Property item;
					item.strItemName = dlg.m_strItemName;
					item.strPointName = dlg.m_strBindPointName;
					item.color = dlg.m_nColor;
					item.nStoreType = dlg.m_nBindPointStoreType;
					pPie->InsertGraphItem(item);
					SetPieChartProperty(pPie);
				}
			} 			
		}
		else if (strParamName == _T("Item"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�޸�")
			{
				CString strBefore = vBefore.bstrVal;
				//CString strBefore =( (_Graph_Item_Property*)(pProp->GetData()))->strItemName;
				vector<_Graph_Item_Property> &itemVec = pPie->GetItemVector();
				int size = itemVec.size();
				for (int i = 0;i<size;i++)
				{
					if (itemVec[i].strItemName == strBefore)
					{
						CChartItemPropertyDlg dlg;
						dlg.SetPointMap(&pMainFrame->m_plcpointmap);
						dlg.m_strItemName = itemVec[i].strItemName;
						dlg.m_nColor = itemVec[i].color;
						dlg.m_strBindPointName = itemVec[i].strPointName;
						dlg.m_nBindPointStoreType = itemVec[i].nStoreType;
						if (dlg.DoModal() == IDOK)
						{
							//pLine->DeleteGraphItem(strBefore);

							_Graph_Item_Property item;
							item.strItemName = dlg.m_strItemName;
							item.strPointName = dlg.m_strBindPointName;
							item.color = dlg.m_nColor;
							item.nStoreType = dlg.m_nBindPointStoreType;
							itemVec[i] = item;
							//pLine->InsertGraphItem(item);
							//pProp->SetData((DWORD)&(pLine->m_vecItem.back()));
							//pProp->SetValue(item.strItemName);
							SetPieChartProperty(pPie);
						}
						else
						{
							pProp->SetValue(strBefore);
						}
						break;
					}
				}				
			} 
			else if (strValue ==L"ɾ��")
			{
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{
					CString strBefore = vBefore.bstrVal;
					pPie->DeleteGraphItem(strBefore);
					SetPieChartProperty(pPie);
				}
				else
				{
					pProp->SetValue(vBefore);
				}
			}		
		}

		//	SetLineChartProperty(pLine);
		pPie->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	

	}
}

void CPropertiesWnd::SetRectangleProperty( CEqmDrawRectangle* pRectangle )
{
	SetNull();
	m_eShowMode = E_SHOW_ROI;
	m_pRectangle  = pRectangle;
	if (pRectangle)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pRectangle->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}	
		{
			CMFCPropertyGridProperty* pPropWidth = new CMFCPropertyGridProperty(_T("Width"), _T(""), _T("���"));
			pPropWidth->AddOption(_T("1"));
			pPropWidth->AddOption(_T("2"));
			pPropWidth->AddOption(_T("3"));
			pPropWidth->AddOption(_T("4"));
			pPropWidth->AddOption(_T("5"));
			pPropWidth->AddOption(_T("6"));
			pPropWidth->AddOption(_T("7"));
			pPropWidth->AddOption(_T("8"));
			pPropWidth->AddOption(_T("9"));
			pPropWidth->AddOption(_T("10"));
			pPropWidth->AddOption(_T("11"));
			pPropWidth->AddOption(_T("12"));
			pPropWidth->AddOption(_T("13"));
			pPropWidth->AddOption(_T("14"));		

			pPropWidth->AllowEdit(FALSE);

			CString strWidth = pPropWidth->GetOption(pRectangle->GetLineWidth()-1); //14?
			pPropWidth->SetValue((_variant_t)strWidth);
			m_wndPropList.AddProperty(pPropWidth);
		}
		//�Ƿ����
		{
			CMFCPropertyGridProperty* pFill = new CMFCPropertyGridProperty(_T("Fill"), _T(""), _T("���"));
			pFill->AddOption(L"No");
			pFill->AddOption(L"Yes");
			CString strFill= pFill->GetOption(pRectangle->GetFillStyl());
			pFill->SetValue((_variant_t)strFill);
			m_wndPropList.AddProperty(pFill);
		}
		//Բ��
		{
			//�Ƿ�Բ��
			CMFCPropertyGridProperty* pRound = new CMFCPropertyGridProperty(_T("Round"), _T(""), _T("����Բ��"));
			pRound->AddOption(L"No");
			pRound->AddOption(L"Yes");
			CString strFill= pRound->GetOption(pRectangle->GetToRoundRect());
			pRound->SetValue((_variant_t)strFill);
			m_wndPropList.AddProperty(pRound);
		
			CMFCPropertyGridProperty* pProperty = NULL;
			//Բ�Ǹ߶�
			pProperty = new CMFCPropertyGridProperty(_T("RoundHeight"), (_variant_t)(pRectangle->GetRoundHeight()), _T("Բ�Ǹ߶�1-100"));
			pProperty->AllowEdit(TRUE);
			m_wndPropList.AddProperty(pProperty);
			//Բ�ǿ��
			pProperty = new CMFCPropertyGridProperty(_T("RoundWidth"), (_variant_t)(pRectangle->GetRoundWidth()), _T("Բ�ǿ��1-100"));
			pProperty->AllowEdit(TRUE);
			m_wndPropList.AddProperty(pProperty);
		}
		//�߿���ɫѡ��
		{
			CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"), pRectangle->GetLineColorRGB(), NULL, _T("�߿���ɫ"));
			pColorProp->EnableOtherButton(L"����..");
			pColorProp->EnableAutomaticButton(L"Ĭ��", ::GetSysColor(COLOR_3DFACE));
			m_wndPropList.AddProperty(pColorProp);
		}
		//�߿�͸����
		{
			CMFCPropertyGridProperty* pTransparency = new CMFCPropertyGridProperty(_T("Transparency"), (_variant_t)(pRectangle->GetTransparency()), _T("�߿�͸����0-100"));
			pTransparency->AllowEdit(TRUE);
			m_wndPropList.AddProperty(pTransparency);
		}
		//�����ɫѡ��
		{
			CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("FillColor"), pRectangle->GetFillColorRGB(), NULL, _T("�����ɫ"));
			pColorProp->EnableOtherButton(L"����..");
			pColorProp->EnableAutomaticButton(L"Ĭ��", ::GetSysColor(COLOR_3DFACE));
			m_wndPropList.AddProperty(pColorProp);
		}
		//���͸����
		{
			CMFCPropertyGridProperty* pTransparency = new CMFCPropertyGridProperty(_T("FillTransparency"), (_variant_t)(pRectangle->GetFillTransparency()), _T("���͸����0-100"));
			pTransparency->AllowEdit(TRUE);
			m_wndPropList.AddProperty(pTransparency);
		}
	}
}

void CPropertiesWnd::ChangeRectangleProperty( CMFCPropertyGridProperty* pProp,CEqmDrawRectangle* pRectangle )
{
	if (pRectangle && pProp)
	{	
		CommandManager::Instance()->CallCommand(pRectangle,E_COMMAND_EDIT);
		const CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (_T("Layer") == strParamName)
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pRectangle->SetLayer(i);
					break;
				}
			}
		}
		else if (_T("Width") == strParamName)
		{
			CString tmp = vChange.bstrVal;
			m_pRectangle->SetLineWidth(_ttoi(tmp.GetString()));	
		}
		else if(_T("Fill") == strParamName)
		{
			CString tmp = vChange.bstrVal;
			if(tmp == L"Yes")
				pRectangle->SetFillStyl(true);
			else
				pRectangle->SetFillStyl(false);
		}
		else if(_T("Round") == strParamName)
		{
			CString tmp = vChange.bstrVal;
			if(tmp == L"Yes")
				pRectangle->SetToRoundRect(true);
			else
				pRectangle->SetToRoundRect(false);
		}
		else if(_T("RoundHeight") == strParamName)
		{
			UINT tempA = vChange.uintVal;
			pRectangle->SetRoundHeight(tempA);
		}
		else if(_T("RoundWidth") == strParamName)
		{
			UINT tempA = vChange.uintVal;
			pRectangle->SetRoundWidth(tempA);
		}
		else if (strParamName == _T("Color"))
		{
			pRectangle->SetLineColorRGB(vChange.uintVal);
		}
		else if (strParamName == _T("Transparency"))
		{
			UINT tempA = vChange.uintVal;
			if(tempA>=0 & tempA<=100)
			{
				pRectangle->SetTransparency(tempA);
			}
			else
			{
				AfxMessageBox(L"͸����ֵ��Χ0-100");
			}
		}
		else if (strParamName == _T("FillColor"))
		{
			pRectangle->SetFillColorRGB(vChange.uintVal);
		}
		else if (strParamName == _T("FillTransparency"))
		{
			UINT tempA = vChange.uintVal;
			if(tempA>=0 & tempA<=100)
			{
				pRectangle->SetFillTransparency(tempA);
			}
			else
			{
				AfxMessageBox(L"͸����ֵ��Χ0-100");
			}
		}
		pRectangle->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetTrendProperty( CEqmDrawHistoryTrend* pTrend )
{
	SetNull();
	m_eShowMode = E_SHOW_TREND;
	m_pTrend = pTrend;
	if (pTrend)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pTrend->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}
		{
			HistoryTrendType chartType = pTrend->GetHistoryTrendType();
			CString tmp(L"");
			if (chartType == enumCurve)
			{
				tmp = L"����";
			} 
			else
			{
				tmp = L"��";
			}

			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Type"),tmp, _T("ͼ����"));
			pPropLayer->AddOption(_T("����"));
			pPropLayer->AddOption(_T("��"));			
			pPropLayer->AllowEdit(FALSE);			
			m_wndPropList.AddProperty(pPropLayer);
		}

		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Items"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ��������"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));	
				//	pPropBindPoints->AddOption(_T("ɾ��"));	
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Item List"));
			m_vecTrendInfo =  pTrend->GetHistoryTrendVector();
			int size = m_vecTrendInfo.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)m_vecTrendInfo[i].strBindPointName, _T("ͼ�����ţ���ɫ���󶨵㣬�洢����"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);		
				pPropBindPoints->SetData(i);
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);	

			m_wndPropList.AddProperty(pGroup4);
		}	
	}

}

void CPropertiesWnd::ChangeTrendProperty( CMFCPropertyGridProperty* pProp,CEqmDrawHistoryTrend* pTrend )
{
	if(pTrend && pProp)
	{		
		CommandManager::Instance()->CallCommand(pTrend,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());

		if(strParamName == _T("Layer"))			//��
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pTrend->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == L"Type")
		{
			CString strType = vChange.bstrVal;
			if (strType == L"����")
			{
				pTrend->SetHistoryTrendType(enumCurve);
			} 
			else
			{
				pTrend->SetHistoryTrendType(enumColumn);
			}
		}
		else if (strParamName == _T("Operation"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"����")
			{
				CHistoryTrendItemSetDlg dlg(&pMainFrame->m_plcpointmap);
				HistoryTrendInfo tmp;
				dlg.SetHistoryTrendInfo(tmp);				
				if (dlg.DoModal() == IDOK)
				{
					m_vecTrendInfo.push_back(dlg.GetHistoryTrendInfo());
					pTrend->SetHistoryTrendVector(m_vecTrendInfo);
					SetTrendProperty(pTrend);
				}
			} 			
		}
		else if (strParamName == _T("Item"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�޸�")
			{
				int index = pProp->GetData();
				CHistoryTrendItemSetDlg dlg(&pMainFrame->m_plcpointmap);				
				dlg.SetHistoryTrendInfo(m_vecTrendInfo[index]);	
					
				if (dlg.DoModal() == IDOK)
				{
					m_vecTrendInfo[index] = dlg.GetHistoryTrendInfo();
					pTrend->SetHistoryTrendVector(m_vecTrendInfo);					
				}
				pProp->SetValue(m_vecTrendInfo[index].strBindPointName);						
			} 
			else if (strValue ==L"ɾ��")
			{
				int index = pProp->GetData();
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{
					
					vector<HistoryTrendInfo>::iterator iter = m_vecTrendInfo.begin()+ index;					
					m_vecTrendInfo.erase(iter);
					pTrend->SetHistoryTrendVector(m_vecTrendInfo);
					SetTrendProperty(pTrend);
				}
				else
				{
					pProp->SetValue(m_vecTrendInfo[index].strBindPointName);
				}
			}		
		}

		pTrend->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetCheckButtonProperty( CEqmDrawButtonPlus* pCheck )
{
	SetNull();
	m_eShowMode = E_SHOW_CHECK_BUTTON;
	m_pCheck = pCheck;
	if(pCheck)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));
			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(pCheck->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup1->AddSubItem(pPropLayer);
			}
			//�ı���Ϣ
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Text Description"), (_variant_t)pCheck->GetTextContent(), _T("�ı�����")));
			//��ɫ
			CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Text Color"), pCheck->GetTextColor(), NULL, _T("������ɫ"));
			pColorProp->EnableOtherButton(_T("Other..."));
			pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
			pGroup1->AddSubItem(pColorProp);
			//��С
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("FontSize"), (_variant_t)pCheck->GetTextSize(), _T("���ִ�С")));			
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Expression"), (_variant_t)pCheck->GetExpression(), _T("��Ч״̬����")));
			m_wndPropList.AddProperty(pGroup1);
		}	

		{
			CString tmp(L"��ѡ");
			if (pCheck->GetButtonStyle() == E_CHECK_BUTTON)
			{
				tmp = L"��ѡ";
			}
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Style"), tmp, _T("�ؼ���ʽ"));
			pPropLayer->AddOption(_T("��ѡ"));
			pPropLayer->AddOption(_T("��ѡ"));			
			pPropLayer->AllowEdit(FALSE);			
			m_wndPropList.AddProperty(pPropLayer);
		}

		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Assignment"));
			{
				CBTinValueProperty* pDateTimePoint = new CBTinValueProperty(_T("Bind Point"), (_variant_t)pCheck->GetBindPointName(), _T("�󶨵�λ"));
				pDateTimePoint->SetMainFrame(pMainFrame);
				pDateTimePoint->SetPopDlgType(E_POP_DLG_POINT);
				pGroup1->AddSubItem(pDateTimePoint);
			}

			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Value Checked"), (_variant_t)pCheck->GetValueForCheck(), _T("ѡ�и�ֵ")));
			
			if (pCheck->GetButtonStyle() == E_RADIO_BUTTON)
			{
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("GroupID"), (_variant_t)pCheck->GetGroupID(), _T("��")));
			}
			else
			{
				pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Value Unchecked"), (_variant_t)pCheck->GetValueForUnCheck(), _T("��ѡ�и�ֵ")));
			}
			
			m_wndPropList.AddProperty(pGroup1);
		}
	}
}

void CPropertiesWnd::ChangeCheckButtonProperty( CMFCPropertyGridProperty* pProp,CEqmDrawButtonPlus* pCheck )
{
	if(pCheck && pProp)
	{	
		CommandManager::Instance()->CallCommand(pCheck,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if(strParamName == _T("Text Description"))			//����
		{
			CString strNewValue = vChange.bstrVal;     
			//m_pText->SetInitialString(strNewValue.GetString());
			pCheck->SetTextContent(strNewValue);			
		}
		else if(strParamName == _T("FontSize"))			//��С
		{
			long tmp = VariantToInt32WithDefault(vChange,0);
			if (tmp < 0)
			{
				tmp = 0;				
			}
			pCheck->SetTextSize(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		else if(strParamName == _T("Text Color"))			//������ɫ
		{
			pCheck->SetTextColor(vChange.intVal);
		}
		else if(strParamName == _T("Expression"))			//����
		{
			CString strNewValue = vChange.bstrVal;      			
			pCheck->SetExpression(strNewValue);		
			if (!pCheck->CheckExpression(strNewValue))
			{
				MessageBox(_T("���ʽ��ʽ����"), _T("Error"), MB_OK|MB_ICONERROR);
			}
		}
		else if(strParamName == _T("Layer"))			//��
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pCheck->SetLayer(i);
					break;
				}
			}
		}
		else if(strParamName == _T("Bind Point"))	
		{
			pCheck->SetBindPointName(vChange.bstrVal);
		}
		else if(strParamName == _T("Value Checked"))	
		{
			pCheck->SetValueForCheck(vChange.bstrVal);
		}
		else if(strParamName == _T("GroupID"))	
		{
			long ID = VariantToInt32WithDefault(vChange,0);
			if (ID < 0)
			{
				ID = 0;				
			}
			pCheck->SetGroupID(ID);
			pProp->SetValue((_variant_t)ID);
		}
		else if(strParamName == _T("Value Unchecked"))	
		{
			pCheck->SetValueForUnCheck(vChange.bstrVal);
		}
		else if(strParamName == _T("Style"))	
		{
			CString tmp = vChange.bstrVal;
			if (tmp == L"��ѡ")
			{
				pCheck->SetButtonStyle(E_CHECK_BUTTON);
			} 
			else
			{
				pCheck->SetButtonStyle(E_RADIO_BUTTON);
			}
			SetCheckButtonProperty(pCheck);
		}		
		pCheck->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	

	}
}

void CPropertiesWnd::SetEmbededProperty( CEqmDrawEmbededPage* pEmbed )
{
	SetNull();
	m_eShowMode = E_SHOW_EMBEDED;
	m_pEmbed = pEmbed;
	if (pEmbed)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));
			{
				CBTinValueProperty* pDateTimePoint = new CBTinValueProperty(_T("Picture"), (_variant_t)pEmbed->GetPicID(), _T("��ͼ"));
				pDateTimePoint->SetMainFrame(pMainFrame);
				pDateTimePoint->SetPopDlgType(E_POP_DLG_IMAGE);
				pGroup1->AddSubItem(pDateTimePoint);
			}
			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(pEmbed->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup1->AddSubItem(pPropLayer);
			}

			int nInterval = pEmbed->GetChangeInterval();
			CMFCPropertyGridProperty* pInterval = new CMFCPropertyGridProperty(_T("Switch Interval"), (_variant_t)nInterval, _T("��ҳ���л���ʱ����"));
			pGroup1->AddSubItem(pInterval);	
			pInterval->SetData(nInterval);
			m_wndPropList.AddProperty(pGroup1);
		}

		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Page Config"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("��Ч״̬���ã�"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));			
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Page List"));
			vector<PageIDName> relationVec =  pEmbed->GetPageList();
			int size = relationVec.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Page"), relationVec[i].strPageName, _T("��Ƕ��ʽҳ��"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);	
				pPropBindPoints->SetData(i);
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);			

			m_wndPropList.AddProperty(pGroup4);
		}	
	}
}

void CPropertiesWnd::ChangeEmbededProperty( CMFCPropertyGridProperty* pProp,CEqmDrawEmbededPage* pEmbed )
{
	if(pEmbed && pProp)
	{		
		CommandManager::Instance()->CallCommand(pEmbed,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == _T("Picture"))
		{
			long newID = vChange.lVal;

			if (newID != pEmbed->GetPicID())
			{				
				Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(newID, false);
				if(pBitmap)
				{
					pEmbed->SetWidth(pBitmap->GetWidth());
					pEmbed->SetHeight(pBitmap->GetHeight());
					pEmbed->SetDefaultBitmap(pBitmap);
				}
			}
			pEmbed->SetPicID(newID);
			//pProp->SetValue(newID);
		}
		else if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pEmbed->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == L"Switch Interval")
		{
			int original = pProp->GetData();
			long inter = VariantToInt32WithDefault(vChange,original);
			pProp->SetData(inter);
			pEmbed->SetChangeInterval(inter);
			pProp->SetValue((_variant_t)inter);
		}
		else if (strParamName == _T("Operation"))
		{
			CString tmpstr = vChange.bstrVal; 
			if (tmpstr == _T("����"))
			{
				CBindPageDiagnoseDlg dlg;
				CString tmp;
				tmp.Format(L"%d",-1);
				dlg.m_strPageID = tmp;
				dlg.m_str_page_diagnose = L"PageName";
				if (dlg.DoModal() == IDOK)
				{
					int tmp = _wtoi(dlg.m_strPageID.GetString());
					m_pEmbed->InsertPageList(tmp,dlg.m_str_page_diagnose);
					SetEmbededProperty(pEmbed);
				}
				else
				{
					pProp->SetValue(vBefore);
				}
			}					
		}
		else if (strParamName == _T("Page"))
		{
			CString tmpstr = vChange.bstrVal; 
			if (tmpstr == _T("�޸�"))
			{
				int index = pProp->GetData();
				vector<PageIDName> &pageVec = pEmbed->GetPageList();
				CBindPageDiagnoseDlg dlg;

				CString tmp;
				tmp.Format(L"%d",pageVec[index].nPageID);
				dlg.m_strPageID = tmp;
			
				dlg.m_str_page_diagnose = pageVec[index].strPageName;
				if (dlg.DoModal() == IDOK)
				{
					pageVec[index].nPageID = _wtoi(dlg.m_strPageID.GetString());
					pageVec[index].strPageName = dlg.m_str_page_diagnose;
				}
				pProp->SetValue(pageVec[index].strPageName);			
			}
			else if (tmpstr ==_T("ɾ��"))
			{
				int index = pProp->GetData();
				vector<PageIDName> &pageVec = pEmbed->GetPageList();

				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{					
					vector<PageIDName>::iterator iter = pageVec.begin()+ index;
					pageVec.erase(iter);
					SetEmbededProperty(pEmbed);
				}
				else
				{
					pProp->SetValue(pageVec[index].strPageName);
				}
			}
		}

		//SetBendProperty(pBend);
		pEmbed->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	

	}
}

void CPropertiesWnd::SetShellExecuteProperty( CEqmDrawShellExecute* pExecute )
{
	SetNull();
	m_eShowMode = E_SHOW_SHELL_EXECUTE;
	m_pExecute = pExecute;
	if (pExecute)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));
			{
				CBTinValueProperty* pDateTimePoint = new CBTinValueProperty(_T("Picture"), (_variant_t)pExecute->GetBKPicID(), _T("��ͼ"));
				pDateTimePoint->SetMainFrame(pMainFrame);
				pDateTimePoint->SetPopDlgType(E_POP_DLG_IMAGE);
				pGroup1->AddSubItem(pDateTimePoint);
			}
			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(pExecute->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup1->AddSubItem(pPropLayer);
			}
			m_wndPropList.AddProperty(pGroup1);
		}
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("File"));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Server Address"), (_variant_t)pExecute->GetServerIP(), _T("��������ַ")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("File ID"), (_variant_t)pExecute->GetFileID(), _T("�ļ�ID")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("File Name"), (_variant_t)pExecute->GetFileName(), _T("�ļ���")));

			{
				CBTinValueProperty* pPoint = new CBTinValueProperty(_T("Download Directory"), (_variant_t)pExecute->GetDownloadDirectory(), _T("����Ŀ¼"));
				pPoint->SetMainFrame(pMainFrame);
				pPoint->SetPopDlgType(E_POP_DLG_SELECT_PATH);		
				pPoint->AllowEdit(FALSE);			
				pGroup1->AddSubItem(pPoint);
			}
			m_wndPropList.AddProperty(pGroup1);
		}

	}
}

void CPropertiesWnd::ChangeShellExecuteProperty( CMFCPropertyGridProperty* pProp,CEqmDrawShellExecute* pExecute )
{
	if(pExecute && pProp)
	{			
		CommandManager::Instance()->CallCommand(pExecute,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == _T("Picture"))
		{
			long newID = vChange.lVal;

			if (newID != pExecute->GetBKPicID())
			{				
				Image* pBitmap = pMainFrame->m_pPrjSqlite->GetBitmapByIDFromPictureLib(newID, false);
				if(pBitmap)
				{
					pExecute->SetWidth(pBitmap->GetWidth());
					pExecute->SetHeight(pBitmap->GetHeight());
					pExecute->SetDefaultBitmap(pBitmap);
				}
			}
			pExecute->SetBKPicID(newID);
			pProp->SetValue(newID);
		}
		else if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pExecute->SetLayer(i);
					break;
				}
			}
		}
		else if(strParamName == _T("Server Address"))	
		{
			pExecute->SetServerIP(vChange.bstrVal);
		}	
		else if(strParamName == _T("File ID"))	
		{
			pExecute->SetFileID(vChange.bstrVal);
		}	
		else if(strParamName == _T("File Name"))	
		{
			pExecute->SetFileName(vChange.bstrVal);
		}	
		else if(strParamName == _T("Download Directory"))	
		{
			pExecute->SetDownloadDirectory(vChange.bstrVal);
		}	
		pExecute->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetAreaChangeProperty( CEqmDrawAreaChange* pArea )
{
	SetNull();
	m_eShowMode = E_SHOW_AREA_CHANGE;
	m_pArea = pArea;
	if (pArea)
	{
		{
			m_areaPos.x = pArea->GetPos().X;
			m_areaPos.y = pArea->GetPos().Y;
			m_areaPos.w = pArea->GetWidth();
			m_areaPos.h = pArea->GetHeight();
		}
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));			
			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(pArea->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup1->AddSubItem(pPropLayer);
			}
			{				
				CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(_T("X Pos"), (_variant_t)(pArea->GetPos()).X, _T("ͼƬX"));
				pPos->AllowEdit(FALSE);
				pGroup1->AddSubItem(pPos);
			}		
			{
				CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(_T("Y Pos"), (_variant_t)(pArea->GetPos()).Y, _T("ͼƬY"));
				pPos->AllowEdit(FALSE);
				pGroup1->AddSubItem(pPos);
			}	
			{
				CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(_T("Width"), (_variant_t)pArea->GetWidth(), _T("ͼƬ���"));
				pPos->AllowEdit(FALSE);
				pGroup1->AddSubItem(pPos);
			}	
			{
				CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(_T("Height"), (_variant_t)pArea->GetHeight(), _T("ͼƬ�߶�"));
				pPos->AllowEdit(FALSE);
				pGroup1->AddSubItem(pPos);
			}	

			m_wndPropList.AddProperty(pGroup1);
		}

		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Items"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ�������ã�"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));	
				//	pPropBindPoints->AddOption(_T("ɾ��"));	
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Area List"));
			m_vecAreaInfo =  pArea->GetAreaInfo();
			int size = m_vecAreaInfo.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)m_vecAreaInfo[i].strPointList, _T("����"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);	
				pPropBindPoints->SetData(i);
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);	

			m_wndPropList.AddProperty(pGroup4);
		}	
	}
}

void CPropertiesWnd::ChangeAreaChangeProperty( CMFCPropertyGridProperty* pProp,CEqmDrawAreaChange* pArea )
{
	if(pArea && pProp)
	{			
		CommandManager::Instance()->CallCommand(pArea,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pArea->SetLayer(i);
					break;
				}
			}
		}
// 		else if (strParamName == L"X Pos")
// 		{
// 			long tmp = VariantToInt32WithDefault(vChange,0);
// 			pProp->SetValue(tmp);
// 		}

		else if (strParamName == _T("Operation"))
		{
			CString strValue = vChange.bstrVal;

			CMFCPropertyGridProperty *pParent = pProp->GetParent();
			CMFCPropertyGridProperty* pList = pParent->GetSubItem(1);
			if (strValue == L"����")
			{				
				CAddAreaDlg dlg;
				dlg.m_dWidth = pArea->GetWidth();
				dlg.m_dHeight = pArea->GetHeight();
				dlg.m_strPointList = L"";
				//CString strBitMapID	= m_listAreaChange.GetItemText(pNMItemActivate->iItem, 2);
				dlg.m_nBitMapID = 0;
				//CString strPageID	= m_listAreaChange.GetItemText(pNMItemActivate->iItem, 3);
				dlg.m_nLinkPageID = 0;

				if (dlg.DoModal() == IDOK)
				{
					AreaInfo item;
					item.strPointList = dlg.m_strPointList;
					item.BnmID = dlg.m_nBitMapID;
					item.nPageID = dlg.m_nLinkPageID;
					m_vecAreaInfo.push_back(item);
					pArea->SetAreaInfo(m_vecAreaInfo);
					{
						CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)item.strPointList, _T("����"));
						pPropBindPoints->AddOption(_T("�޸�"));
						pPropBindPoints->AddOption(_T("ɾ��"));			
						pPropBindPoints->AllowEdit(FALSE);
						pPropBindPoints->SetData(m_vecAreaInfo.size()-1);
						pList->AddSubItem(pPropBindPoints);
						pList->Expand(FALSE);
						pList->Expand(TRUE);
					}
				}
				pProp->SetValue(L"�޲���");				
			} 			
		}
		else if (strParamName == _T("Item"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�޸�")
			{
				CAddAreaDlg dlg;
				dlg.m_dWidth = pArea->GetWidth();
				dlg.m_dHeight = pArea->GetHeight();
				int index = pProp->GetData();
				dlg.m_strPointList = m_vecAreaInfo[index].strPointList;			
				dlg.m_nBitMapID = m_vecAreaInfo[index].BnmID;
				dlg.m_nLinkPageID = m_vecAreaInfo[index].nPageID;
				if (dlg.DoModal() == IDOK)
				{
					 m_vecAreaInfo[index].strPointList = dlg.m_strPointList;
					 m_vecAreaInfo[index].BnmID = dlg.m_nBitMapID;
					 m_vecAreaInfo[index].nPageID = dlg.m_nLinkPageID;
					 pArea->SetAreaInfo(m_vecAreaInfo);
				}
				pProp->SetValue(m_vecAreaInfo[index].strPointList);
			} 
			else if (strValue ==L"ɾ��")
			{
				int index = pProp->GetData();
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{					
					vector<AreaInfo>::iterator iter = m_vecAreaInfo.begin() + index;
					m_vecAreaInfo.erase(iter);
					pArea->SetAreaInfo(m_vecAreaInfo);
					SetAreaChangeProperty(pArea);
				}
				else
				{
					pProp->SetValue(m_vecAreaInfo[index].strPointList);
				}
			}		
		}

		pArea->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}


void CPropertiesWnd::SetStraightLineProperty(CEqmDrawStraightLine* pStraightLine)
{
	SetNull();
	m_eShowMode = E_SHOW_SCRAWL;
	m_pStraightLine = pStraightLine;
	if (pStraightLine)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();		
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pStraightLine->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}	
		{
			CMFCPropertyGridProperty* pPropWidth = new CMFCPropertyGridProperty(_T("Width"), _T(""), _T("���"));
			pPropWidth->AddOption(_T("1"));
			pPropWidth->AddOption(_T("2"));
			pPropWidth->AddOption(_T("3"));
			pPropWidth->AddOption(_T("4"));
			pPropWidth->AddOption(_T("5"));
			pPropWidth->AddOption(_T("6"));
			pPropWidth->AddOption(_T("7"));
			pPropWidth->AddOption(_T("8"));
			pPropWidth->AddOption(_T("9"));
			pPropWidth->AddOption(_T("10"));
			pPropWidth->AddOption(_T("11"));
			pPropWidth->AddOption(_T("12"));
			pPropWidth->AddOption(_T("13"));
			pPropWidth->AddOption(_T("14"));		

			pPropWidth->AllowEdit(FALSE);

			CString strWidth = pPropWidth->GetOption(pStraightLine->GetLineWidth()-1); //14?
			pPropWidth->SetValue((_variant_t)strWidth);
			m_wndPropList.AddProperty(pPropWidth);
		}
		{
			CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"), pStraightLine->GetLineColor(), NULL, _T("�����ɫ"));
			pColorProp->EnableOtherButton(L"����..");
			pColorProp->EnableAutomaticButton(L"Ĭ��", ::GetSysColor(COLOR_3DFACE));
			m_wndPropList.AddProperty(pColorProp);
		}
		//͸����
		{
			CMFCPropertyGridProperty* pTransparency = new CMFCPropertyGridProperty(_T("Transparency"), (_variant_t)(pStraightLine->GetTransparency()), _T("͸����0-100"));
			pTransparency->AllowEdit(TRUE);
			m_wndPropList.AddProperty(pTransparency);
		}
		{
			CMFCPropertyGridProperty* pPropWidth = new CMFCPropertyGridProperty(_T("LineStyle"), _T(""), _T("����"));
			pPropWidth->AddOption(_T("ʵ��"));
			pPropWidth->AddOption(_T("����"));
			pPropWidth->AddOption(_T("����"));
			pPropWidth->AddOption(_T("���������"));
			pPropWidth->AddOption(_T("����ĵ㻮��"));	

			pPropWidth->AllowEdit(FALSE);

			CString strWidth = pPropWidth->GetOption(pStraightLine->GetLineStyle()); //14?
			pPropWidth->SetValue((_variant_t)strWidth);
			m_wndPropList.AddProperty(pPropWidth);
		}
		//�����״�ʹ�С
		{
			CMFCPropertyGridProperty* pPropWidth = new CMFCPropertyGridProperty(_T("StartShape"), _T(""), _T("�����״"));
			pPropWidth->AddOption(_T("��"));
			pPropWidth->AddOption(_T("����Բ"));

			pPropWidth->AllowEdit(FALSE);

			CString strWidth = pPropWidth->GetOption(pStraightLine->GetStartShape()); //14?
			pPropWidth->SetValue((_variant_t)strWidth);
			m_wndPropList.AddProperty(pPropWidth);
		}
		{
			CMFCPropertyGridProperty* pTransparency = new CMFCPropertyGridProperty(_T("StartShapeSize"), (_variant_t)(pStraightLine->GetStartShapeSize()), _T("�˵��С"));
			pTransparency->AllowEdit(TRUE);
			m_wndPropList.AddProperty(pTransparency);
		}
		//�յ���״�ʹ�С
		{
			CMFCPropertyGridProperty* pPropWidth = new CMFCPropertyGridProperty(_T("EndShape"), _T(""), _T("�յ���״"));
			pPropWidth->AddOption(_T("��"));
			pPropWidth->AddOption(_T("����Բ"));

			pPropWidth->AllowEdit(FALSE);

			CString strWidth = pPropWidth->GetOption(pStraightLine->GetEndShape()); //14?
			pPropWidth->SetValue((_variant_t)strWidth);
			m_wndPropList.AddProperty(pPropWidth);
		}
		{
			CMFCPropertyGridProperty* pTransparency = new CMFCPropertyGridProperty(_T("EndShapeSize"), (_variant_t)(pStraightLine->GetEndShapeSize()), _T("�յ��С"));
			pTransparency->AllowEdit(TRUE);
			m_wndPropList.AddProperty(pTransparency);
		}

	}
}

void CPropertiesWnd::ChangeStraightLineProperty(CMFCPropertyGridProperty* pProp,CEqmDrawStraightLine* pStraightLine)
{
	if (pStraightLine && pProp)
	{	
		CommandManager::Instance()->CallCommand(pStraightLine,E_COMMAND_EDIT);
		const CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (_T("Layer") == strParamName)
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pStraightLine->SetLayer(i);
					break;
				}
			}
		}
		else if (_T("Width") == strParamName)
		{
			CString tmp = vChange.bstrVal;
			pStraightLine->SetLineWidth(_ttoi(tmp.GetString()));	
		}
		else if (strParamName == _T("Color"))
		{
			pStraightLine->SetLineColor(vChange.uintVal);
		}
		else if (strParamName == _T("Transparency"))
		{
			UINT tempA = vChange.uintVal;
			if(tempA>=0 & tempA<=100)
			{
				pStraightLine->SetTransparency(tempA);
			}
			else
			{
				AfxMessageBox(L"͸����ֵ��Χ0-100");
			}
		}
		else if (strParamName == _T("LineStyle"))
		{
			CString tmp = vChange.bstrVal;
			if(tmp == _T("ʵ��"))
				pStraightLine->SetLineStyle(0);
			else if(tmp == _T("����"))
				pStraightLine->SetLineStyle(1);
			else if(tmp == _T("����"))
				pStraightLine->SetLineStyle(2);
			else if(tmp == _T("���������"))
				pStraightLine->SetLineStyle(3);
			else if(tmp == _T("����ĵ㻮��"))
				pStraightLine->SetLineStyle(4);
		}
		else if (strParamName == _T("StartShape"))
		{
			CString tmp = vChange.bstrVal;
			if(tmp == _T("��"))
				pStraightLine->SetStartShape(Shape_None);
			else if(tmp == _T("����Բ"))
				pStraightLine->SetStartShape(Shape_SolidCircle);
		}
		else if (strParamName == _T("StartShapeSize"))
		{
			pStraightLine->SetStartShapeSize(vChange.uintVal);
		}
		else if (strParamName == _T("EndShape"))
		{
			CString tmp = vChange.bstrVal;
			if(tmp == _T("��"))
				pStraightLine->SetEndShape(Shape_None);
			else if(tmp == _T("����Բ"))
				pStraightLine->SetEndShape(Shape_SolidCircle);
		}
		else if (strParamName == _T("EndShapeSize"))
		{
			pStraightLine->SetEndShapeSize(vChange.uintVal);
		}

		pStraightLine->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

//void CPropertiesWnd::SetScrawlProperty( CEqmDrawScrawl* pScrawl )
//{
//	SetNull();
//	m_eShowMode = E_SHOW_SCRAWL;
//	m_pScrawl = pScrawl;
//	if (pScrawl)
//	{
//		m_wndPropList.RemoveAll();	
//		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
//		{
//			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));			
//			{
//				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
//				pPropLayer->AddOption(_T("1��"));
//				pPropLayer->AddOption(_T("2��"));
//				pPropLayer->AddOption(_T("3��"));
//				pPropLayer->AddOption(_T("4��"));
//				pPropLayer->AddOption(_T("5��"));
//				pPropLayer->AddOption(_T("6��"));
//				pPropLayer->AddOption(_T("7��"));
//				pPropLayer->AddOption(_T("8��"));
//				pPropLayer->AddOption(_T("9��"));
//				pPropLayer->AddOption(_T("10��"));
//				pPropLayer->AllowEdit(FALSE);
//				CString strLayer = pPropLayer->GetOption(pScrawl->GetLayer());
//				pPropLayer->SetValue((_variant_t)strLayer);
//				pGroup1->AddSubItem(pPropLayer);
//			}
//			{				
//				CMFCPropertyGridProperty* pPos = new CMFCPropertyGridProperty(_T("Title"), (_variant_t)pScrawl->GetShowWords(), _T("�������"));				
//				pGroup1->AddSubItem(pPos);
//			}	
//			m_wndPropList.AddProperty(pGroup1);
//		}
//
//		{
//			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Curve Items"));
//			CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Bar Items"));
//			{						
//				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("���������ã�"));
//				pPropBindPoints->AddOption(_T("�޲���"));
//				pPropBindPoints->AddOption(_T("��������"));	
//				//	pPropBindPoints->AddOption(_T("ɾ��"));	
//				pPropBindPoints->AllowEdit(FALSE);			
//				pGroup4->AddSubItem(pPropBindPoints);
//			}
//			{						
//				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("��ͼ�����ã�"));
//				pPropBindPoints->AddOption(_T("�޲���"));
//				pPropBindPoints->AddOption(_T("����"));	
//				//	pPropBindPoints->AddOption(_T("ɾ��"));	
//				pPropBindPoints->AllowEdit(FALSE);			
//				pGroup3->AddSubItem(pPropBindPoints);
//			}
//
//			CMFCPropertyGridProperty* pSubGroup1 = new CMFCPropertyGridProperty(_T("Item List"));
//			CMFCPropertyGridProperty* pSubGroup2 = new CMFCPropertyGridProperty(_T("Item List"));
//
//			m_vecScrawl =  pScrawl->GetCurveColumn();
//			int size = m_vecScrawl.size();
//			for (int i = 0;i<size;++i)
//			{				
//				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)m_vecScrawl[i].strPointName, _T("ͼ��"));
//				pPropBindPoints->AddOption(_T("�޸�"));
//				pPropBindPoints->AddOption(_T("ɾ��"));			
//				pPropBindPoints->AllowEdit(FALSE);	
//				pPropBindPoints->SetData(i);
//				if (m_vecScrawl[i].scrawlType == scrawlCurve)
//				{
//					pSubGroup1->AddSubItem(pPropBindPoints);
//				}
//				else
//				{
//					pSubGroup2->AddSubItem(pPropBindPoints);
//				}				
//			}	
//			
//			pGroup4->AddSubItem(pSubGroup1);	
//			pGroup3->AddSubItem(pSubGroup2);
//			m_wndPropList.AddProperty(pGroup4);
//			m_wndPropList.AddProperty(pGroup3);
//		}	
//	}
//}
//
//void CPropertiesWnd::ChangeScrawlProperty( CMFCPropertyGridProperty* pProp,CEqmDrawScrawl* pScrawl )
//{
//	if(pScrawl && pProp)
//	{	
//		CommandManager::Instance()->CallCommand(pScrawl,E_COMMAND_EDIT);
//		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
//		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
//		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
//		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
//		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
//		if (strParamName == L"Layer")
//		{
//			int size = pProp->GetOptionCount();
//			CString option;
//			CString choosed = vChange.bstrVal;
//			for (int i = 0;i<size;++i)
//			{
//				option = pProp->GetOption(i);					
//				if (option == choosed)
//				{
//					pScrawl->SetLayer(i);
//					break;
//				}
//			}
//		}
//		else if (strParamName == L"Title")
//		{
//			pScrawl->SetShowWords(vChange.bstrVal);
//		}
//
//		else if (strParamName == _T("Operation"))
//		{
//			CString strValue = vChange.bstrVal;
//
//			CMFCPropertyGridProperty *pParent = pProp->GetParent();
//			CMFCPropertyGridProperty* pList = pParent->GetSubItem(1);		
//
//			if (strValue == L"����")
//			{				
//				CScrawlItemSetDlg dlg(&(pMainFrame->m_plcpointmap));
//				CurveColumn item;
//				dlg.SetPointName(L"����");
//				dlg.SetColor(0);
//				dlg.SetMaxValue(100);
//				dlg.SetMinValue(0);
//				dlg.SetRemarks(L"");
//
//				if (dlg.DoModal() == IDOK)
//				{
//					item.scrawlType = scrawlColumn;
//					item.color = dlg.GetColor();
//					item.nMax = dlg.GetMaxValue();
//					item.nMin = dlg.GetMinValue();
//					item.strPointName = dlg.GetPointName();
//					item.strRemarks = dlg.GetRemarks();					
//					m_vecScrawl.push_back(item);
//					pScrawl->SetCurveColumn(m_vecScrawl);					
//					{
//						CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)item.strPointName, _T("ͼ��"));
//						pPropBindPoints->AddOption(_T("�޸�"));
//						pPropBindPoints->AddOption(_T("ɾ��"));			
//						pPropBindPoints->AllowEdit(FALSE);
//						pPropBindPoints->SetData(m_vecScrawl.size()-1);
//						pList->AddSubItem(pPropBindPoints);
//						pList->Expand(FALSE);
//						pList->Expand(TRUE);
//					}
//				}
//				pProp->SetValue(L"�޲���");				
//			}
//			else if (strValue == L"��������")
//			{
//				CScrawlItemSetDlg dlg(&(pMainFrame->m_plcpointmap));
//				CurveColumn item;
//				dlg.SetPointName(L"����");
//				dlg.SetColor(0);
//				dlg.SetMaxValue(100);
//				dlg.SetMinValue(0);
//				dlg.SetRemarks(L"");
//
//				if (dlg.DoModal() == IDOK)
//				{
//					item.scrawlType = scrawlCurve;
//					item.color = dlg.GetColor();
//					item.nMax = dlg.GetMaxValue();
//					item.nMin = dlg.GetMinValue();
//					item.strPointName = dlg.GetPointName();
//					item.strRemarks = dlg.GetRemarks();					
//					m_vecScrawl.push_back(item);
//					pScrawl->SetCurveColumn(m_vecScrawl);					
//					{
//						CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)item.strPointName, _T("ͼ��"));
//						pPropBindPoints->AddOption(_T("�޸�"));
//						pPropBindPoints->AddOption(_T("ɾ��"));			
//						pPropBindPoints->AllowEdit(FALSE);
//						pPropBindPoints->SetData(m_vecScrawl.size()-1);
//						pList->AddSubItem(pPropBindPoints);
//						pList->Expand(FALSE);
//						pList->Expand(TRUE);
//					}
//				}
//				pProp->SetValue(L"�޲���");		
//			}
//		}
//		else if (strParamName == _T("Item"))
//		{
//			CString strValue = vChange.bstrVal;
//			if (strValue == L"�޸�")
//			{
//				CScrawlItemSetDlg dlg(&(pMainFrame->m_plcpointmap));	
//				int index = pProp->GetData();
//				CurveColumn &item = m_vecScrawl[index];
//				dlg.SetPointName(item.strPointName);
//				dlg.SetColor(item.color);
//				dlg.SetMaxValue(item.nMax);
//				dlg.SetMinValue(item.nMin);
//				dlg.SetRemarks(item.strRemarks);
//				if (dlg.DoModal() == IDOK)
//				{				
//					item.color = dlg.GetColor();
//					item.nMax = dlg.GetMaxValue();
//					item.nMin = dlg.GetMinValue();
//					item.strPointName = dlg.GetPointName();
//					item.strRemarks = dlg.GetRemarks();	
//					pScrawl->SetCurveColumn(m_vecScrawl);
//				}					
//				pProp->SetValue(item.strPointName);
//			} 
//			else if (strValue ==L"ɾ��")
//			{
//				int index = pProp->GetData();
//				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
//				{					
//					vector<CurveColumn>::iterator iter = m_vecScrawl.begin() + index;
//					m_vecScrawl.erase(iter);
//					pScrawl->SetCurveColumn(m_vecScrawl);
//					SetScrawlProperty(pScrawl);
//				}
//				else
//				{
//					pProp->SetValue(m_vecScrawl[index].strPointName);
//				}
//			}		
//		}
//
//		pScrawl->SetNeedSave(TRUE);
//		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
//	}
//}

void CPropertiesWnd::SetLiquidProperty( CEqmDrawLiquidLevel* pLiquid )
{
	SetNull();
	m_eShowMode = E_SHOW_LIQUID;
	m_pLiquid = pLiquid;
	if (pLiquid)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Liquid Level"));			
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pLiquid->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			pGroup1->AddSubItem(pPropLayer);
		}

		{
			CBTinValueProperty* pDateTimePoint = new CBTinValueProperty(_T("Bind Point"), (_variant_t)pLiquid->GetBindPoint(), _T("�����õ�"));
			pDateTimePoint->SetMainFrame(pMainFrame);
			pDateTimePoint->SetPopDlgType(E_POP_DLG_POINT);
			pDateTimePoint->AllowEdit(FALSE);
			pGroup1->AddSubItem(pDateTimePoint);
		}
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)pLiquid->GetMaxValue(), _T("���ֵ")));
		pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)pLiquid->GetMinValue(), _T("��Сֵ")));
		m_wndPropList.AddProperty(pGroup1);
	}
}

void CPropertiesWnd::ChangeLiquidProperty( CMFCPropertyGridProperty* pProp,CEqmDrawLiquidLevel* pLiquid )
{
	if(pLiquid && pProp)
	{	
		CommandManager::Instance()->CallCommand(pLiquid,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pLiquid->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName ==L"Bind Point")
		{
			pLiquid->SetBindPoint(vChange.bstrVal);
		}
		else if (strParamName ==L"Max")
		{
			long tmp = VariantToInt32WithDefault(vChange,0);
			pLiquid->SetMaxValue(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		else if (strParamName ==L"Min")
		{
			long tmp = VariantToInt32WithDefault(vChange,0);
			pLiquid->SetMinValue(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		pLiquid->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetNullProperty()
{
	SetNull();
	m_eShowMode = E_SHOW_NULL;
	m_wndPropList.RemoveAll();	
	m_wndPropList.RedrawWindow();
}

void CPropertiesWnd::SetTimeBarProperty( CEqmDrawTimeBar* pTbar )
{
	SetNull();
	m_eShowMode = E_SHOW_TIME_BAR;
	m_pTbar = pTbar;
	if (pTbar)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));			
			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(pTbar->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup1->AddSubItem(pPropLayer);
			}			
			pGroup1->AddSubItem(new CMFCPropertyGridColorProperty(_T("Color"), pTbar->GetBarColor(), NULL, _T("��ɫ")));
			m_wndPropList.AddProperty(pGroup1);			
		}

		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Point Binding"));
			{
				CBTinValueProperty* pDateTimeSeries = new CBTinValueProperty(_T("Time Point"), (_variant_t)pTbar->GetBindPointStart(), _T("��ʱ���"));
				pDateTimeSeries->SetMainFrame(pMainFrame);
				pDateTimeSeries->SetPopDlgType(E_POP_DLG_POINT);
				pGroup->AddSubItem(pDateTimeSeries);
			}
			{
				CMFCPropertyGridProperty* pPropAlign = new CMFCPropertyGridProperty(_T("Time Precision"), _T(""), _T("ʱ�侫������"));
				pPropAlign->AddOption(_T("����"));
				pPropAlign->AddOption(_T("Сʱ"));
				pPropAlign->AddOption(_T("��"));
				pPropAlign->AddOption(_T("��"));
				pPropAlign->AddOption(_T("��"));
				pPropAlign->AddOption(_T("��"));				
				pPropAlign->AllowEdit(FALSE);
				CString strAlign = pPropAlign->GetOption(pTbar->GetQueryType());
				pPropAlign->SetValue((_variant_t)strAlign);
				pGroup->AddSubItem(pPropAlign);
			}
			{
				CBTinValueProperty* pDateTimeSeries = new CBTinValueProperty(_T("Status Point"), (_variant_t)pTbar->GetBindPointStatus(), _T("��״̬��"));
				pDateTimeSeries->SetMainFrame(pMainFrame);
				pDateTimeSeries->SetPopDlgType(E_POP_DLG_POINT);
				pGroup->AddSubItem(pDateTimeSeries);
			}
			m_wndPropList.AddProperty(pGroup);
		}
	}
}

void CPropertiesWnd::ChangeTimeBarProperty( CMFCPropertyGridProperty* pProp,CEqmDrawTimeBar* pTbar )
{
	if(pTbar && pProp)
	{	
		CommandManager::Instance()->CallCommand(pTbar,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pTbar->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == L"Color")
		{
			pTbar->SetBarColor(vChange.intVal);
		}
		else if (strParamName == L"Time Point")
		{
			pTbar->SetBindPointStart(vChange.bstrVal);
		}
		else if (strParamName == L"Status Point")
		{
			pTbar->SetBindPointStatus(vChange.bstrVal);
		}
		else if (strParamName == L"Time Precision")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pTbar->SetQueryType(i);
					break;
				}
			}
		}
		pTbar->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetProgressProperty( CEqmDrawProgressBar* pProgrs )
{
	SetNull();
	m_eShowMode = E_SHOW_PROGRESS;
	m_pProgrs = pProgrs;
	if (pProgrs)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));			
			{
				CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
				pPropLayer->AddOption(_T("1��"));
				pPropLayer->AddOption(_T("2��"));
				pPropLayer->AddOption(_T("3��"));
				pPropLayer->AddOption(_T("4��"));
				pPropLayer->AddOption(_T("5��"));
				pPropLayer->AddOption(_T("6��"));
				pPropLayer->AddOption(_T("7��"));
				pPropLayer->AddOption(_T("8��"));
				pPropLayer->AddOption(_T("9��"));
				pPropLayer->AddOption(_T("10��"));
				pPropLayer->AllowEdit(FALSE);
				CString strLayer = pPropLayer->GetOption(pProgrs->GetLayer());
				pPropLayer->SetValue((_variant_t)strLayer);
				pGroup1->AddSubItem(pPropLayer);
			}			
			pGroup1->AddSubItem(new CMFCPropertyGridColorProperty(_T("Color"), pProgrs->GetBarColor(), NULL, _T("��ɫ")));
			pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Factor"), (_variant_t)pProgrs->GetFactorValue(), _T("ϵ��")));
			m_wndPropList.AddProperty(pGroup1);			
		}

		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Point Binding"));
			{
				CBTinValueProperty* pDateTimeSeries = new CBTinValueProperty(_T("Status Point"), (_variant_t)pProgrs->GetBindPoint(), _T("��״̬��"));
				pDateTimeSeries->SetMainFrame(pMainFrame);
				pDateTimeSeries->SetPopDlgType(E_POP_DLG_POINT);
				pGroup->AddSubItem(pDateTimeSeries);
			}
			{
				CBTinValueProperty* pDateTimeSeries = new CBTinValueProperty(_T("Reference Point"), (_variant_t)pProgrs->GetBindPointBase(), _T("�󶨻�׼��"));
				pDateTimeSeries->SetMainFrame(pMainFrame);
				pDateTimeSeries->SetPopDlgType(E_POP_DLG_POINT);
				pGroup->AddSubItem(pDateTimeSeries);
			}
			m_wndPropList.AddProperty(pGroup);
		}
	}
}

void CPropertiesWnd::ChangeProgressProperty( CMFCPropertyGridProperty* pProp,CEqmDrawProgressBar* pProgrs )
{
	if(pProgrs && pProp)
	{	
		CommandManager::Instance()->CallCommand(pProgrs,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pProgrs->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == L"Color")
		{
			pProgrs->SetBarColor(vChange.intVal);		
		}
		else if (strParamName == L"Factor")
		{
			double oldvalue = VariantToDoubleWithDefault(vBefore,0);
			double newValue = VariantToDoubleWithDefault(vChange,oldvalue);
			pProgrs->SetFactorValue(newValue);
			pProp->SetValue((_variant_t)newValue);
		}
		else if (strParamName == L"Status Point")
		{
			pProgrs->SetBindPoint(vChange.bstrVal);
		}
		else if (strParamName == L"Reference Point")
		{
			pProgrs->SetBindPointBase(vChange.bstrVal);
		}

		pProgrs->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetNaviProperty( CEqmDrawNavigateWindow* pNavi )
{
	SetNull();
	m_eShowMode = E_SHOW_NAVIGATE;
	m_pNav = pNavi;
	if (pNavi)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pNavi->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}
		
		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Page Items"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ�������ã�"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));					
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Page List"));
			m_vecPage =  pNavi->GetPageInfo();
			int size = m_vecPage.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)m_vecPage[i].strPageDesc, _T("ҳ��"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);	
				pPropBindPoints->SetData(i);
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);	

			m_wndPropList.AddProperty(pGroup4);
		}	
	}
}

void CPropertiesWnd::ChangeNaviProperty( CMFCPropertyGridProperty* pProp,CEqmDrawNavigateWindow* pNavi )
{
	if(pNavi && pProp)
	{	
		CommandManager::Instance()->CallCommand(pNavi,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pNavi->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == _T("Operation"))
		{
			CString strValue = vChange.bstrVal;

			CMFCPropertyGridProperty *pParent = pProp->GetParent();
			CMFCPropertyGridProperty* pList = pParent->GetSubItem(1);
			if (strValue == L"����")
			{				
				CNavigateWindowItemSetDlg dlg(_T(""), _T(""), 0);
				if (IDOK == dlg.DoModal())
				{
					PageInfo pageInfo;
					pageInfo.strPageDesc = dlg.m_strDesc;
					pageInfo.strPageName = dlg.m_strPageName;
					pageInfo.nPageId = dlg.m_nPageId;
					m_vecPage.push_back(pageInfo);
					pNavi->SetPageInfo(m_vecPage);

					{
						CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)pageInfo.strPageDesc, _T("ҳ��"));
						pPropBindPoints->AddOption(_T("�޸�"));
						pPropBindPoints->AddOption(_T("ɾ��"));			
						pPropBindPoints->AllowEdit(FALSE);
						pPropBindPoints->SetData(m_vecPage.size()-1);
						pList->AddSubItem(pPropBindPoints);
						pList->Expand(FALSE);
						pList->Expand(TRUE);
					}
				}			
				pProp->SetValue(L"�޲���");				
			} 			
		}
		else if (strParamName == _T("Item"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�޸�")
			{				
				int index = pProp->GetData();
				CNavigateWindowItemSetDlg dlg(m_vecPage[index].strPageDesc, m_vecPage[index].strPageName, m_vecPage[index].nPageId);				
				if (dlg.DoModal() == IDOK)
				{
					m_vecPage[index].strPageDesc = dlg.m_strDesc;
					m_vecPage[index].strPageName = dlg.m_strPageName;
					m_vecPage[index].nPageId = dlg.m_nPageId;
					pNavi->SetPageInfo(m_vecPage);
				}
				pProp->SetValue(m_vecPage[index].strPageDesc);
			} 
			else if (strValue ==L"ɾ��")
			{
				int index = pProp->GetData();
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{					
					vector<PageInfo>::iterator iter = m_vecPage.begin() + index;
					m_vecPage.erase(iter);
					pNavi->SetPageInfo(m_vecPage);
					SetNaviProperty(pNavi);					
				}
				else
				{
					pProp->SetValue(m_vecPage[index].strPageDesc);
				}
			}		
		}

		pNavi->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetAnlyProperty( CEqmDrawDataAnalyse* pAnly )
{
	SetNull();
	m_eShowMode = E_SHOW_DATA_ANALYSIS;
	m_pAnalysis = pAnly;
	if (pAnly)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		ASSERT(pMainFrame != NULL);
		{
			{
				CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic"));
				{
					CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
					pPropLayer->AddOption(_T("1��"));
					pPropLayer->AddOption(_T("2��"));
					pPropLayer->AddOption(_T("3��"));
					pPropLayer->AddOption(_T("4��"));
					pPropLayer->AddOption(_T("5��"));
					pPropLayer->AddOption(_T("6��"));
					pPropLayer->AddOption(_T("7��"));
					pPropLayer->AddOption(_T("8��"));
					pPropLayer->AddOption(_T("9��"));
					pPropLayer->AddOption(_T("10��"));
					pPropLayer->AllowEdit(FALSE);
					CString strLayer = pPropLayer->GetOption(pAnly->GetLayer());
					pPropLayer->SetValue((_variant_t)strLayer);

					pGroup1->AddSubItem(pPropLayer);
				}

				m_wndPropList.AddProperty(pGroup1);
			}

			{
				CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("PointSet"));
				{
					CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("��Ч״̬���ã�"));
					pItem->AddOption(_T("�޲���"));
					pItem->AddOption(_T("����"));
					pItem->AllowEdit(FALSE);
					pGroup2->AddSubItem(pItem);
				}
				{
					CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("PointItemSet"));
					vector<DaPointInfo> vecDaPtInf =  pAnly->GetDaPointInfo();
					vector<DaPointInfo>::const_iterator	iter;

					for (iter=vecDaPtInf.begin(); iter!=vecDaPtInf.end(); ++iter)
					{
						CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("PointItem"), GetDataAnalysePointSetString(*iter), _T("������"));
						pItem->AddOption(_T("�޸�"));
						pItem->AddOption(_T("ɾ��"));
						pItem->AllowEdit(FALSE);
						pSubGroup->AddSubItem(pItem);
					}	
					pGroup2->AddSubItem(pSubGroup);
				}

				m_wndPropList.AddProperty(pGroup2);
			}

			{
				CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Calculate"));
				{
					CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("TimePoint"), pAnly->GetPointTime().GetString(), _T("ʱ�������"));
					pItem->AddOption(_T("ѡ��"));
					pItem->AllowEdit(FALSE);
					pGroup3->AddSubItem(pItem);
				}
				{
					CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("CalcCount"), pAnly->GetPointCount().GetString(), _T("����������"));
					pItem->AddOption(_T("ѡ��"));
					pItem->AllowEdit(FALSE);
					pGroup3->AddSubItem(pItem);
				}
				{
					CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("CalcStatus"), pAnly->GetPointStatus().GetString(), _T("����״̬"));
					pItem->AddOption(_T("ѡ��"));
					pItem->AllowEdit(FALSE);
					pGroup3->AddSubItem(pItem);
				}

				m_wndPropList.AddProperty(pGroup3);
			}
		}
	}
}

void CPropertiesWnd::ChangeAnlyProperty( CMFCPropertyGridProperty* pProp,CEqmDrawDataAnalyse* pAnaly )
{
	if(pAnaly && pProp)
	{	
		CommandManager::Instance()->CallCommand(pAnaly,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pAnaly->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == _T("Operation"))
		{
			CMFCPropertyGridProperty* pParent = pProp->GetParent();
			CMFCPropertyGridProperty* pList = pParent->GetSubItem(1);

			CString strValue = vChange.bstrVal;
			if (_T("����") == strValue)
			{
				vector<DaPointInfo> vecDaPtInf = pAnaly->GetDaPointInfo();
				CDataAnalyseItemSetDlg dlg(&pMainFrame->m_plcpointmap, _T(""), _T(""));
				if (IDOK == dlg.DoModal())
				{
					DaPointInfo	daPtInf;
					daPtInf.strPointName = dlg.m_strPtName;
					daPtInf.strPointRemarks = dlg.m_strRemarks;
					vecDaPtInf.push_back(daPtInf);
					pAnaly->SetDaPointInfo(vecDaPtInf);

					CString	strItem;
					strItem.Format(_T("%s,%s"), dlg.m_strPtName, dlg.m_strRemarks);
					CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("PointItem"), (_variant_t)strItem, _T("������"));
					pItem->AddOption(_T("�޸�"));
					pItem->AddOption(_T("ɾ��"));
					pItem->AllowEdit(FALSE);
					pItem->SetData(vecDaPtInf.size()-1);
					pList->AddSubItem(pItem);
					pList->Expand(FALSE);
					pList->Expand(TRUE);
				}

				pProp->SetValue(_T("�޲���"));
			}
		}
		else if (strParamName == _T("PointItem"))
		{
			CString strValue = vChange.bstrVal;
			if (_T("�޸�") == strValue)
			{
				int nIdx = pProp->GetData();
				vector<DaPointInfo> vecDaPtInf(pAnaly->GetDaPointInfo());

				CDataAnalyseItemSetDlg dlg(&pMainFrame->m_plcpointmap, vecDaPtInf.at(nIdx).strPointName, vecDaPtInf.at(nIdx).strPointRemarks);
				if (IDOK == dlg.DoModal())
				{
					vecDaPtInf.at(nIdx).strPointName = dlg.m_strPtName;
					vecDaPtInf.at(nIdx).strPointRemarks = dlg.m_strRemarks;

					pProp->SetValue(GetDataAnalysePointSetString(vecDaPtInf.at(nIdx)));
				}
			}
			else if (_T("ɾ��") == strValue)
			{
				int nIdx = pProp->GetData();
				vector<DaPointInfo> vecDaPtInf(pAnaly->GetDaPointInfo());
				if (IDOK == AfxMessageBox(_T("ȷ��Ҫɾ����?"), MB_OKCANCEL))
				{
					vector<DaPointInfo>::iterator iter = vecDaPtInf.begin() + nIdx;
					vecDaPtInf.erase(iter);
					pAnaly->SetDaPointInfo(vecDaPtInf);
					SetAnlyProperty(pAnaly);
				}
				else
				{
					pProp->SetValue(GetDataAnalysePointSetString(vecDaPtInf.at(nIdx)));
				}
			}
		}
		else if (strParamName == _T("TimePoint"))
		{
			CString strValue = vChange.bstrVal;
			if (_T("ѡ��") == strValue)
			{
				CDataComConfigDlg dlg(pMainFrame->m_plcpointmap);
				if (IDOK == dlg.DoModal())
				{
					pAnaly->SetPointTime(dlg.m_strPointName);
					pProp->SetValue(dlg.m_strPointName);
				}				
				else
				{	
					pProp->SetValue(pAnaly->GetPointTime());
				}
			}
		}
		else if (strParamName == _T("CalcCount"))
		{
			CString strValue = vChange.bstrVal;
			if (_T("ѡ��") == strValue)
			{
				CDataComConfigDlg dlg(pMainFrame->m_plcpointmap);
				if (IDOK == dlg.DoModal())
				{
					pAnaly->SetPointCount(dlg.m_strPointName);
					pProp->SetValue(dlg.m_strPointName);
				}				
				else
				{	
					pProp->SetValue(pAnaly->GetPointCount());
				}
			}
		}
		else if (strParamName == _T("CalcStatus"))
		{
			CString strValue = vChange.bstrVal;
			if (_T("ѡ��") == strValue)
			{
				CDataComConfigDlg dlg(pMainFrame->m_plcpointmap);
				if (IDOK == dlg.DoModal())
				{
					pAnaly->SetPointStatus(dlg.m_strPointName);
					pProp->SetValue(dlg.m_strPointName);
				}				
				else
				{	
					pProp->SetValue(pAnaly->GetPointStatus());
				}
			}
		}

		pAnaly->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}

}

void CPropertiesWnd::SetReportProperty( CEqmDrawDataReport* pReport )
{
	SetNull();
	m_eShowMode = E_SHOW_REPORT;
	m_pReport = pReport;
	if (pReport)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pReport->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}

			m_wndPropList.AddProperty(new CMFCPropertyGridProperty(_T("Link Address"), (_variant_t)pReport->GetPath(), _T("·����ַ")));
	}
}

void CPropertiesWnd::ChangeReportProperty( CMFCPropertyGridProperty* pProp,CEqmDrawDataReport* pReport )
{

	if(pReport && pProp)
	{	
		CommandManager::Instance()->CallCommand(pReport,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pReport->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == L"Link Address")
		{
			pReport->SetPath(vChange.bstrVal);
			pProp->SetValue(vChange.bstrVal);
		}

		pReport->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}

}

void CPropertiesWnd::SetTemperProperty( CEqmDrawTemperDistri* pTemper )
{
	SetNull();
	m_eShowMode = E_SHOW_TEMP_DIS;
	m_pTemp = pTemper;
	if (pTemper)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pTemper->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}

		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Distribution Items"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ�������ã�"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));					
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Item List"));
			m_vecTemper = pTemper->GetTemperDistri();			
			int size = m_vecTemper.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)m_vecTemper[i].strPointList, _T("�¶ȷֲ���"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);	
				pPropBindPoints->SetData(i);
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);	

			m_wndPropList.AddProperty(pGroup4);
		}	
	}

}

void CPropertiesWnd::ChangeTemperProperty( CMFCPropertyGridProperty* pProp,CEqmDrawTemperDistri* pTemper )
{
	if(pTemper && pProp)
	{	
		CommandManager::Instance()->CallCommand(pTemper,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pTemper->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == _T("Operation"))
		{
			CString strValue = vChange.bstrVal;

			CMFCPropertyGridProperty *pParent = pProp->GetParent();
			CMFCPropertyGridProperty* pList = pParent->GetSubItem(1);
			if (strValue == L"����")
			{
				CTemperDistriItemSetDlg dlg(0, _T(""), _T(""), &pMainFrame->m_plcpointmap,&pView->m_PageDlg->m_dlgCanvas);
				if (IDOK == dlg.DoModal())
				{
					TemperDistriInfo info;
					info.nPicId			= dlg.m_nPicId;
					info.strPointList	= dlg.m_strPointList;
					info.strBindPtName	= dlg.m_strBindPtName;
					m_vecTemper.push_back(info);
					pTemper->AddTemperDistri(info);					
					{
						CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)info.strPointList, _T("�¶ȷֲ���"));
						pPropBindPoints->AddOption(_T("�޸�"));
						pPropBindPoints->AddOption(_T("ɾ��"));			
						pPropBindPoints->AllowEdit(FALSE);
						pPropBindPoints->SetData(m_vecTemper.size()-1);
						pList->AddSubItem(pPropBindPoints);
						pList->Expand(FALSE);
						pList->Expand(TRUE);
					}
				}			
				pProp->SetValue(L"�޲���");				
			} 			
		}
		else if (strParamName == _T("Item"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�޸�")
			{				
				int index = pProp->GetData();
				CTemperDistriItemSetDlg dlg(m_vecTemper[index].nPicId, m_vecTemper[index].strPointList, m_vecTemper[index].strBindPtName,&pMainFrame->m_plcpointmap,&pView->m_PageDlg->m_dlgCanvas);						
				if (dlg.DoModal() == IDOK)
				{
					m_vecTemper[index].nPicId = dlg.m_nPicId;
					m_vecTemper[index].strPointList = dlg.m_strPointList;
					m_vecTemper[index].strBindPtName = dlg.m_strBindPtName;
					pTemper->SetTemperDistri(m_vecTemper);					
				}
				pProp->SetValue(m_vecTemper[index].strPointList);
			} 
			else if (strValue ==L"ɾ��")
			{
				int index = pProp->GetData();
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{					
					vector<TemperDistriInfo>::iterator iter = m_vecTemper.begin() + index;
					m_vecTemper.erase(iter);
					pTemper->SetTemperDistri(m_vecTemper);
					SetTemperProperty(pTemper);					
				}
				else
				{
					pProp->SetValue(m_vecTemper[index].strPointList);
				}
			}		
		}

		pTemper->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}

}

void CPropertiesWnd::SetRulerProperty( CEqmDrawLevelRuler* pRuler )
{
	SetNull();
	m_eShowMode = E_SHOW_RULER;
	m_pRuler = pRuler;
	if (pRuler)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pRuler->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);			
		}

		m_wndPropList.AddProperty(new CMFCPropertyGridProperty(_T("Name"), (_variant_t)pRuler->GetName(), _T("�������")));

		{
			CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Value Setting"));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)(double)(pRuler->GetMax()), _T("���ֵ")));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)(double)(pRuler->GetMin()), _T("��Сֵ")));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Main Interval"), (_variant_t)(double)(pRuler->GetMainScaleInterval()), _T("���̶ȼ��")));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Minor Interval"), (_variant_t)(double)(pRuler->GetMinorScaleInterval()), _T("���̶ȼ��")));
			pGroup->AddSubItem(new CMFCPropertyGridProperty(_T("Decimal Digits"), (_variant_t)(double)(pRuler->GetDecimalPlace()), _T("С����λ��")));//int
			m_wndPropList.AddProperty(pGroup);
		}


		{
			CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Label Binding"));
			{						
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Operation"), _T("�޲���"), _T("ͼ�������ã�"));
				pPropBindPoints->AddOption(_T("�޲���"));
				pPropBindPoints->AddOption(_T("����"));					
				pPropBindPoints->AllowEdit(FALSE);			
				pGroup4->AddSubItem(pPropBindPoints);
			}

			CMFCPropertyGridProperty* pSubGroup = new CMFCPropertyGridProperty(_T("Label List"));
			m_vecLabelInfo = pRuler->GetLabelInfo();				
			int size = m_vecLabelInfo.size();
			for (int i = 0;i<size;++i)
			{				
				CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)m_vecLabelInfo[i].strName, _T("��ǩ��"));
				pPropBindPoints->AddOption(_T("�޸�"));
				pPropBindPoints->AddOption(_T("ɾ��"));			
				pPropBindPoints->AllowEdit(FALSE);	
				pPropBindPoints->SetData(i);
				pSubGroup->AddSubItem(pPropBindPoints);
			}	
			pGroup4->AddSubItem(pSubGroup);	

			m_wndPropList.AddProperty(pGroup4);
		}

		{
			CMFCPropertyGridProperty* pGroupSection = new CMFCPropertyGridProperty(_T("Section List"));

			CMFCPropertyGridProperty* pSectionOpera = new CMFCPropertyGridProperty(_T("SectionOp"), _T("�޲���"), _T(""));
			pSectionOpera->AddOption(_T("�޲���"));
			pSectionOpera->AddOption(_T("����"));
			pSectionOpera->AllowEdit(FALSE);
			pGroupSection->AddSubItem(pSectionOpera);

			CMFCPropertyGridProperty* pSectionLst = new CMFCPropertyGridProperty(_T("Section List"));
			m_vecSectionInfo = pRuler->GetSectionInfo();
			vector<SectionInfo>::const_iterator	iter;
			int i = 0;
			for (iter=m_vecSectionInfo.begin(); iter!=m_vecSectionInfo.end(); ++iter,++i)
			{
				CMFCPropertyGridProperty* pSectItem = new CMFCPropertyGridProperty(_T("Section"), (_variant_t)iter->strName, _T("��ǩ��"));
				pSectItem->AddOption(_T("�޸�"));
				pSectItem->AddOption(_T("ɾ��"));
				pSectItem->AllowEdit(FALSE);	
				pSectItem->SetData(i);
				pSectionLst->AddSubItem(pSectItem);
			}
			pGroupSection->AddSubItem(pSectionLst);

			m_wndPropList.AddProperty(pGroupSection);
		}
	}
}

void CPropertiesWnd::ChangeRulerProperty( CMFCPropertyGridProperty* pProp,CEqmDrawLevelRuler* pRuler )
{
	if(pRuler && pProp)
	{	
		CommandManager::Instance()->CallCommand(pRuler,E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pRuler->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == L"Name")
		{
			pRuler->SetName(vChange.bstrVal);
		}
		else if (strParamName == L"Max")
		{
			double tmp = VariantToDoubleWithDefault(vChange,0);
			pRuler->SetMax(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		else if (strParamName == L"Min")
		{
			double tmp = VariantToDoubleWithDefault(vChange,0);
			pRuler->SetMin(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		else if (strParamName == L"Main Interval")
		{
			double tmp = VariantToDoubleWithDefault(vChange,0);
			pRuler->SetMainScaleInterval(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		else if (strParamName == L"Minor Interval")
		{
			double tmp = VariantToDoubleWithDefault(vChange,0);
			pRuler->SetMinorScaleInterval(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		else if (strParamName == L"Decimal Digits")
		{
			long tmp = VariantToInt32WithDefault(vChange,0);
			pRuler->SetDecimalPlace(tmp);
			pProp->SetValue((_variant_t)tmp);
		}
		else if (strParamName == _T("Operation"))
		{
			CString strValue = vChange.bstrVal;

			CMFCPropertyGridProperty *pParent = pProp->GetParent();
			CMFCPropertyGridProperty* pList = pParent->GetSubItem(1);
			if (strValue == L"����")
			{
				CLevelRulerItemSetDlg dlg(_T(""), 0, _T(""), 0, _T(""), FALSE, &pMainFrame->m_plcpointmap);
				if (IDOK == dlg.DoModal())
				{
					LabelInfo labelInfo;
					labelInfo.strName = dlg.m_strName;
					labelInfo.dValue = dlg.m_dValue;
					labelInfo.strBindPtName = dlg.m_strBindPtName;
					labelInfo.nLinkPageId = dlg.m_nLinkPageId;
					labelInfo.strLinkPageName = dlg.m_strLinkPageName;
					labelInfo.bIsUp	= static_cast<bool>(dlg.m_bIsUp);
					m_vecLabelInfo.push_back(labelInfo);
					pRuler->SetLabelInfo(m_vecLabelInfo);								
					{
						CMFCPropertyGridProperty* pPropBindPoints = new CMFCPropertyGridProperty(_T("Item"),(_variant_t)labelInfo.strName, _T("��ǩ��"));
						pPropBindPoints->AddOption(_T("�޸�"));
						pPropBindPoints->AddOption(_T("ɾ��"));			
						pPropBindPoints->AllowEdit(FALSE);
						pPropBindPoints->SetData(m_vecLabelInfo.size()-1);
						pList->AddSubItem(pPropBindPoints);
						pList->Expand(FALSE);
						pList->Expand(TRUE);
					}
				}			
				pProp->SetValue(L"�޲���");				
			} 			
		}
		else if (strParamName == _T("Item"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == L"�޸�")
			{				
				int index = pProp->GetData();
				LabelInfo &labelInfo = m_vecLabelInfo[index];

				CLevelRulerItemSetDlg dlg(labelInfo.strName, labelInfo.dValue, labelInfo.strBindPtName, labelInfo.nLinkPageId, labelInfo.strLinkPageName, static_cast<BOOL>(labelInfo.bIsUp), &pMainFrame->m_plcpointmap);		
				if (dlg.DoModal() == IDOK)
				{
					labelInfo.strName = dlg.m_strName;
					labelInfo.dValue = dlg.m_dValue;
					labelInfo.strBindPtName = dlg.m_strBindPtName;
					labelInfo.nLinkPageId = dlg.m_nLinkPageId;
					labelInfo.strLinkPageName = dlg.m_strLinkPageName;
					labelInfo.bIsUp	= static_cast<bool>(dlg.m_bIsUp);
					pRuler->SetLabelInfo(m_vecLabelInfo);
				}
				pProp->SetValue(labelInfo.strName);
			} 
			else if (strValue ==L"ɾ��")
			{
				int index = pProp->GetData();
				if (AfxMessageBox(L"ȷ��Ҫɾ����?",MB_OKCANCEL) == IDOK)
				{					
					vector<LabelInfo>::iterator iter = m_vecLabelInfo.begin() + index;
					m_vecLabelInfo.erase(iter);
					pRuler->SetLabelInfo(m_vecLabelInfo);
					SetRulerProperty(pRuler);					
				}
				else
				{
					pProp->SetValue(m_vecLabelInfo[index].strName);
				}
			}		
		}
		else if (strParamName == _T("SectionOp"))
		{
			CString strValue = vChange.bstrVal;

			CMFCPropertyGridProperty* pParent = pProp->GetParent();
			CMFCPropertyGridProperty* pList = pParent->GetSubItem(1);
			if (strValue == _T("����"))
			{
				CLevelRulerSectionSetDlg dlg(_T(""), 0, 0);
				if (IDOK == dlg.DoModal())
				{
					SectionInfo sectInf;
					sectInf.strName	= dlg.m_strName;
					sectInf.fMax	= dlg.m_fMax;
					sectInf.fMin	= dlg.m_fMin;
					m_vecSectionInfo.push_back(sectInf);
					pRuler->SetSectionInfo(m_vecSectionInfo);								
					{
						CMFCPropertyGridProperty* pSection = new CMFCPropertyGridProperty(_T("Section"),(_variant_t)sectInf.strName, _T("��ǩ��"));
						pSection->AddOption(_T("�޸�"));
						pSection->AddOption(_T("ɾ��"));			
						pSection->AllowEdit(FALSE);
						pSection->SetData(m_vecSectionInfo.size() - 1);
						pList->AddSubItem(pSection);
						pList->Expand(FALSE);
						pList->Expand(TRUE);
					}
				}			
				pProp->SetValue(_T("�޲���"));
			} 			
		}
		else if (strParamName == _T("Section"))
		{
			CString strValue = vChange.bstrVal;
			if (strValue == _T("�޸�"))
			{
				int nIndex = pProp->GetData();
				SectionInfo& sectInf = m_vecSectionInfo[nIndex];

				CLevelRulerSectionSetDlg dlg(sectInf.strName, sectInf.fMax, sectInf.fMin);
				if (IDOK == dlg.DoModal())
				{
					sectInf.strName = dlg.m_strName;
					sectInf.fMax = dlg.m_fMax;
					sectInf.fMin = dlg.m_fMin;
					pRuler->SetSectionInfo(m_vecSectionInfo);
				}
				pProp->SetValue(sectInf.strName);
			} 
			else if (strValue == _T("ɾ��"))
			{
				int nIndex = pProp->GetData();
				if (IDOK == MessageBox(_T("ȷ��Ҫɾ����"), _T("��ʾ"), MB_OKCANCEL))
				{
					vector<SectionInfo>::iterator iter = m_vecSectionInfo.begin() + nIndex;
					m_vecSectionInfo.erase(iter);
					pRuler->SetSectionInfo(m_vecSectionInfo);
					SetRulerProperty(pRuler);
				}
				else
				{
					pProp->SetValue(m_vecLabelInfo[nIndex].strName);
				}
			}
		}

		pRuler->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetPolygonProperty(CEqmDrawPolygon* pChart)
{
	SetNull();
	m_eShowMode = E_SHOW_POLYGON;
	m_pPolygon = pChart;
	if (pChart)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		ASSERT(pMainFrame != NULL);
		if (NULL == pMainFrame)
		{
			return;
		}

		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pChart->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}

		{
			CMFCPropertyGridProperty* pPropWidth = new CMFCPropertyGridProperty(_T("Width"), _T(""), _T("���"));
			pPropWidth->AddOption(_T("1"));
			pPropWidth->AddOption(_T("2"));
			pPropWidth->AddOption(_T("3"));
			pPropWidth->AddOption(_T("4"));
			pPropWidth->AddOption(_T("5"));
			pPropWidth->AddOption(_T("6"));
			pPropWidth->AddOption(_T("7"));
			pPropWidth->AddOption(_T("8"));
			pPropWidth->AddOption(_T("9"));
			pPropWidth->AddOption(_T("10"));
			pPropWidth->AddOption(_T("11"));
			pPropWidth->AddOption(_T("12"));
			pPropWidth->AddOption(_T("13"));
			pPropWidth->AddOption(_T("14"));		

			pPropWidth->AllowEdit(FALSE);

			CString strWidth = pPropWidth->GetOption(pChart->GetLineWidth_PptsWnd()-1); //14?
			pPropWidth->SetValue((_variant_t)strWidth);
			m_wndPropList.AddProperty(pPropWidth);
		}

		{
			CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"), pChart->GetLineColor_PptsWnd(), NULL, _T("�����ɫ"));
			pColorProp->EnableOtherButton(L"����..");
			pColorProp->EnableAutomaticButton(L"Ĭ��", ::GetSysColor(COLOR_3DFACE));
			m_wndPropList.AddProperty(pColorProp);
		}
	}
}

void CPropertiesWnd::ChangePolygonProperty(CMFCPropertyGridProperty* pProp, CEqmDrawPolygon* pChart)
{
	if (pProp != NULL && pChart != NULL)
	{			
		CommandManager::Instance()->CallCommand(pChart, E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();		//���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue();		//�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();//�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());

		if (_T("Layer") == strParamName)
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pChart->SetLayer(i);
					break;
				}
			}
		}
		else if (_T("Width") == strParamName)
		{
			CString tmp = vChange.bstrVal;
			pChart->SetLineWidth_PptsWnd(_ttoi(tmp.GetString()));	
		}
		else if (strParamName == _T("Color"))
		{
			pChart->SetLineColor_PptsWnd(vChange.uintVal);
		}

		pChart->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetPlaneTempeDistrProperty(CEqmDrawPlaneTempeDistr* pPlanTempDist)
{
	SetNull();
	m_eShowMode = E_SHOW_PLANE_TEMPE_DISTR;
	m_pPlaneTempeDistr = pPlanTempDist;
	if (pPlanTempDist != NULL)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pPlanTempDist->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}

		//m_wndPropList.AddProperty(new CMFCPropertyGridProperty(_T("Point Name"), (_variant_t)pPlanTempDist->GetPointName(), _T("����")));
		CMFCPropertyGridProperty* pPropType = new CMFCPropertyGridProperty(_T("Config Type"), _T(""), _T("����"));
		pPropType->AddOption(_T("�¶�"));
		pPropType->AddOption(_T("CO"));
		pPropType->AddOption(_T("�Զ���"));
		pPropType->AllowEdit(FALSE);
		CString strCfgType = pPropType->GetOption(pPlanTempDist->GetConfigType());
		pPropType->SetValue((_variant_t)strCfgType);
		m_wndPropList.AddProperty(pPropType);
	}
}

void CPropertiesWnd::ChangePlaneTempeDistrProperty(CMFCPropertyGridProperty* pProp, CEqmDrawPlaneTempeDistr* pPlanTempDist)
{
	if (pProp != NULL && pPlanTempDist != NULL)
	{	
		CommandManager::Instance()->CallCommand(pPlanTempDist, E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();	//���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue();	//�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();	//�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pPlanTempDist->SetLayer(i);
					break;
				}
			}
		}
		else if (strParamName == L"Point Name")
		{
			pPlanTempDist->SetPointName(vChange.bstrVal);
			pProp->SetValue(vChange.bstrVal);
		}
		else if (strParamName == L"Config Type")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pPlanTempDist->SetConfigType(i);
					break;
				}
			}
		}

		pPlanTempDist->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetDottedLineFrmProperty(CEqmDrawDottedLineFrm* pDotFrm)
{
	SetNull();
	m_eShowMode = E_SHOW_DOTLINE_FRAME;
	m_pDottedLineFrm = pDotFrm;
	if (pDotFrm != NULL)
	{
		m_wndPropList.RemoveAll();	
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pDotFrm->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}
	}
}

void CPropertiesWnd::ChangeDottedLineFrmProperty(CMFCPropertyGridProperty* pProp, CEqmDrawDottedLineFrm* pDotFrm)
{
	if (pProp != NULL && pDotFrm != NULL)
	{
		CommandManager::Instance()->CallCommand(pDotFrm, E_COMMAND_EDIT);
		CString strParamName = pProp->GetName();	//���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue();	//�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();	//�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (strParamName == L"Layer")
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pDotFrm->SetLayer(i);
					break;
				}
			}
		}

		pDotFrm->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

void CPropertiesWnd::SetGeneralComponentProperty(CEqmDrawGeneralComponent* pGenCom)
{
	SetNull();
	m_eShowMode = E_SHOW_DOTLINE_FRAME;
	m_pGeneralComponent = pGenCom;
	if (pGenCom != NULL)
	{
		m_wndPropList.RemoveAll();	
		{
			CMFCPropertyGridProperty* pPropLayer = new CMFCPropertyGridProperty(_T("Layer"), _T(""), _T("ͼ��"));
			pPropLayer->AddOption(_T("1��"));
			pPropLayer->AddOption(_T("2��"));
			pPropLayer->AddOption(_T("3��"));
			pPropLayer->AddOption(_T("4��"));
			pPropLayer->AddOption(_T("5��"));
			pPropLayer->AddOption(_T("6��"));
			pPropLayer->AddOption(_T("7��"));
			pPropLayer->AddOption(_T("8��"));
			pPropLayer->AddOption(_T("9��"));
			pPropLayer->AddOption(_T("10��"));
			pPropLayer->AllowEdit(FALSE);
			CString strLayer = pPropLayer->GetOption(pGenCom->GetLayer());
			pPropLayer->SetValue((_variant_t)strLayer);
			m_wndPropList.AddProperty(pPropLayer);
		}
		m_wndPropList.AddProperty(new CMFCPropertyGridProperty(_T("ConfigValue"), _T(""), _T("")));
	}
}

void CPropertiesWnd::ChangeGeneralComponentProperty(CMFCPropertyGridProperty* pProp, CEqmDrawGeneralComponent* pGenCom)
{
	if (pGenCom && pProp)
	{	
		CommandManager::Instance()->CallCommand(pGenCom,E_COMMAND_EDIT);
		const CString strParamName = pProp->GetName();  //���ı�Ĳ�����  
		COleVariant vChange = pProp->GetValue(); //�ı�֮���ֵ  
		COleVariant vBefore = pProp->GetOriginalValue();  //�ؼ���ʼֵ
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
		if (_T("Layer") == strParamName)
		{
			int size = pProp->GetOptionCount();
			CString option;
			CString choosed = vChange.bstrVal;
			for (int i = 0;i<size;++i)
			{
				option = pProp->GetOption(i);					
				if (option == choosed)
				{
					pGenCom->SetLayer(i);
					break;
				}
			}
		}
		else if (_T("ConfigValue") == strParamName)
		{
			pGenCom->SetConfigValue(vChange.bstrVal);
			pProp->SetValue(vChange.bstrVal);
		}

		pGenCom->SetNeedSave(TRUE);
		pView->m_PageDlg->m_dlgCanvas.Invalidate(false);	
	}
}

CString	CPropertiesWnd::GetDiagGraphPlusBindPtString(const _tagBoolBindInfo& bindInfo)
{
	CString	strRet;
	strRet.Format(_T("%s,%s,%s,%s,%s"),
		bindInfo.strBoolPointName,
		bindInfo.strResultPointName,
		bindInfo.strResultPointName2,
		bindInfo.strResultPointName3,
		bindInfo.strTreatmentStatePointName);

	return strRet;
}

CString	CPropertiesWnd::GetDataAnalysePointSetString(const DaPointInfo& daPtInfo)
{
	CString	strRet;
	strRet.Format(_T("%s,%s"),
		daPtInfo.strPointName,
		daPtInfo.strPointRemarks);

	return strRet;
}
