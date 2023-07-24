// DiagnoseGraphicPlusItemPropertySettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DiagnoseGraphicPlusItemPropertySettingDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"

// CDiagnoseGraphicPlusItemPropertySettingDlg �Ի���

IMPLEMENT_DYNAMIC(CDiagnoseGraphicPlusItemPropertySettingDlg, CDialog)

CDiagnoseGraphicPlusItemPropertySettingDlg::CDiagnoseGraphicPlusItemPropertySettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagnoseGraphicPlusItemPropertySettingDlg::IDD, pParent)
	, m_strItemName(_T(""))
	, m_strBindPointName(_T(""))
	, m_nColor(RGB(255,255,255))
{

}

CDiagnoseGraphicPlusItemPropertySettingDlg::~CDiagnoseGraphicPlusItemPropertySettingDlg()
{
}

void CDiagnoseGraphicPlusItemPropertySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DAIGNOSE_GRAPHIC_PLUS_ITEM_NAME, m_strItemName);
	DDX_Text(pDX, IDC_EDIT_DIAGNOSE_GRAPHIC_PLUS_ITEM_BIND_POINT_NAME, m_strBindPointName);
}


BEGIN_MESSAGE_MAP(CDiagnoseGraphicPlusItemPropertySettingDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_DIAGNOSE_GRAPHIC_PLUS_ITEM_COLOR, &CDiagnoseGraphicPlusItemPropertySettingDlg::OnStnClickedStaticDiagnoseGraphicPlusItemColor)
	ON_BN_CLICKED(IDOK, &CDiagnoseGraphicPlusItemPropertySettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DIAGNOSE_GRAPHIC_PLUS_ITEM_BIND_POINT_NAME, &CDiagnoseGraphicPlusItemPropertySettingDlg::OnBnClickedButtonDiagnoseGraphicPlusItemBindPointName)
END_MESSAGE_MAP()


// CDiagnoseGraphicPlusItemPropertySettingDlg ��Ϣ�������


HBRUSH CDiagnoseGraphicPlusItemPropertySettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_DIAGNOSE_GRAPHIC_PLUS_ITEM_COLOR)
	{
		HBRUSH hBrush = CreateSolidBrush(m_nColor);
		if (hBrush)
		{
			return hBrush;
		}
	}
	return hbr;
}


void CDiagnoseGraphicPlusItemPropertySettingDlg::OnStnClickedStaticDiagnoseGraphicPlusItemColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	return;
	COLORREF cusColor[16];
	memset( cusColor, 0, sizeof( COLORREF) * 16 );
	CHOOSECOLOR cc = {sizeof( CHOOSECOLOR ),m_hWnd,NULL,m_nColor,cusColor,CC_FULLOPEN,NULL,NULL,NULL};
	if (ChooseColor(&cc))
	{
		m_nColor = cc.rgbResult;
		Invalidate();
	}
}


void CDiagnoseGraphicPlusItemPropertySettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDialog::OnOK();
}


BOOL CDiagnoseGraphicPlusItemPropertySettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDiagnoseGraphicPlusItemPropertySettingDlg::OnBnClickedButtonDiagnoseGraphicPlusItemBindPointName()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pMainFrm->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		UpdateData(1);
		m_strBindPointName = dlg.m_strPointName;
		DataPointEntry entry = pMainFrm->m_plcpointmap.GetEntry(m_strBindPointName.GetString());
		const POINT_STORE_CYCLE cycle = entry.GetStoreCycle();
		if (cycle == E_STORE_NULL)
		{
			AfxMessageBox(L"�õ�Ĵ洢����Ϊ�գ������������ԣ�");
			return;
		}
		else
		{
			UpdateData(0);
		}
	}
}
