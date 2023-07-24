// BendPropertyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BendPropertyDlg.h"
#include "afxdialogex.h"


// CBendPropertyDlg �Ի���

IMPLEMENT_DYNAMIC(CBendPropertyDlg, CDialog)

CBendPropertyDlg::CBendPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBendPropertyDlg::IDD, pParent)
	,m_nRotateAngle(0)
	, m_nLayer(4)
	, m_nBendSize(14)
{

}

CBendPropertyDlg::~CBendPropertyDlg()
{
}

void CBendPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROTATE_ANGLE, m_combo_rotate_angle);
	DDX_Control(pDX, IDC_COMBO_LAYER, m_comboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nLayer);
	DDX_Control(pDX, IDC_COMBO_BEND_SIZE, m_comboBendSize);
}


BEGIN_MESSAGE_MAP(CBendPropertyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBendPropertyDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER, &CBendPropertyDlg::OnCbnSelchangeComboLayer)
END_MESSAGE_MAP()


// CBendPropertyDlg ��Ϣ�������


void CBendPropertyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	switch (m_combo_rotate_angle.GetCurSel())
	{
	case 0:
		m_nRotateAngle = 0;
		break;
	case 1:
		m_nRotateAngle = 90;
		break;
	case 2:
		m_nRotateAngle = 180;
		break;
	case 3:
		m_nRotateAngle = 270;
		break;
	case 4:
		m_nRotateAngle = -90;
		break;
	case 5:
		m_nRotateAngle = -180;
		break;
	case 6:
		m_nRotateAngle = -270;
		break;
	default:
		m_nRotateAngle = 0;
		break;
	}

	CString	strCombo;
	int nSel = m_comboBendSize.GetCurSel();
	m_comboBendSize.GetLBText(nSel, strCombo);
	m_nBendSize = _ttoi(strCombo);

	CDialog::OnOK();
}

void CBendPropertyDlg::InitComboRotate()
{
	m_combo_rotate_angle.AddString(L"����ת");
	m_combo_rotate_angle.AddString(L"˳ʱ����ת90��");
	m_combo_rotate_angle.AddString(L"˳ʱ����ת180��");
	m_combo_rotate_angle.AddString(L"˳ʱ����ת270��");
	m_combo_rotate_angle.AddString(L"��ʱ����ת90��");
	m_combo_rotate_angle.AddString(L"��ʱ����ת180��");
	m_combo_rotate_angle.AddString(L"��ʱ����ת270��");
}


BOOL CBendPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitControl();
	InitComboRotate();
	SetCurSel();
	InitComboSize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBendPropertyDlg::SetCurSel()
{
	switch (m_nRotateAngle)
	{
	case 0:
		m_combo_rotate_angle.SetCurSel(0);
		break;
	case 90:
		m_combo_rotate_angle.SetCurSel(1);
		break;
	case 180:
		m_combo_rotate_angle.SetCurSel(2);
		break;
	case 270:
		m_combo_rotate_angle.SetCurSel(3);
		break;
	case -90:
		m_combo_rotate_angle.SetCurSel(4);
		break;
	case -180:
		m_combo_rotate_angle.SetCurSel(5);
		break;
	case -270:
		m_combo_rotate_angle.SetCurSel(6);
		break;
	default:
		m_combo_rotate_angle.SetCurSel(0);
		break;
	}
}

void CBendPropertyDlg::InitControl(void)
{
	// layer
	m_comboLayer.AddString(_T("1��"));
	m_comboLayer.AddString(_T("2��"));
	m_comboLayer.AddString(_T("3��"));
	m_comboLayer.AddString(_T("4��"));
	m_comboLayer.AddString(_T("5��"));
	m_comboLayer.AddString(_T("6��"));
	m_comboLayer.AddString(_T("7��"));
	m_comboLayer.AddString(_T("8��"));
	m_comboLayer.AddString(_T("9��"));
	m_comboLayer.AddString(_T("10��"));

	m_comboLayer.SetCurSel(m_nLayer);
}

void CBendPropertyDlg::InitComboSize(void)
{
	m_comboBendSize.AddString(_T("6"));
	m_comboBendSize.AddString(_T("7"));
	m_comboBendSize.AddString(_T("8"));
	m_comboBendSize.AddString(_T("9"));
	m_comboBendSize.AddString(_T("10"));
	m_comboBendSize.AddString(_T("11"));
	m_comboBendSize.AddString(_T("12"));
	m_comboBendSize.AddString(_T("13"));
	m_comboBendSize.AddString(_T("14"));
	if (m_nBendSize >= 6 && m_nBendSize <= 14)
	{
		m_comboBendSize.SetCurSel(m_nBendSize - 6);
	}
	else
	{
		CString strCustomSize;
		strCustomSize.Format(_T("%d"), m_nBendSize);
		m_comboBendSize.AddString(strCustomSize);
		m_comboBendSize.SetCurSel(9);
	}
}

void CBendPropertyDlg::OnCbnSelchangeComboLayer()
{
	// TODO: Add your control notification handler code here
}
