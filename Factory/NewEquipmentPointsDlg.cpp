// NewEquipmentPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewEquipmentPointsDlg.h"
#include "afxdialogex.h"


// CNewEquipmentPointsDlg dialog

IMPLEMENT_DYNAMIC(CNewEquipmentPointsDlg, CDialogEx)

CNewEquipmentPointsDlg::CNewEquipmentPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewEquipmentPointsDlg::IDD, pParent)
	, m_nChillerCount(3)
	, m_nEquipmentStartNum(1)
	, m_nChTowerCount(3)
	, m_nCWPCount(3)
	, m_nPriChCount(3)
	, m_nSecCWPCount(0)
	, m_nPriHWPCount(0)
	, m_nSecHCWCount(0)
	, m_strNewRoomName(_T(""))
	, m_bAddControlPoints(TRUE)
	, m_bChillerHasValve(TRUE)
	, m_nPriChWPCount(3)
	,m_nFanBlowerCount(1)
	, m_PlantDescription(_T(""))
{

}

CNewEquipmentPointsDlg::~CNewEquipmentPointsDlg()
	
{
}

void CNewEquipmentPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM_CHILLER, m_nChillerCount);
	DDX_Text(pDX, IDC_EDIT_NUM_STARTNUM, m_nEquipmentStartNum);
	DDX_Text(pDX, IDC_EDIT_NUM_CHTOWER, m_nChTowerCount);
	DDX_Text(pDX, IDC_EDIT_NUM_CWP, m_nCWPCount);
	DDX_Text(pDX, IDC_EDIT_NUM_CHILLER6, m_nPriChCount);;
	DDX_Text(pDX, IDC_EDIT_NUM_SecChCWP, m_nSecCWPCount);
	DDX_Text(pDX, IDC_EDIT_NUM_PriHW_COUNT, m_nPriHWPCount);
	DDX_Text(pDX, IDC_EDIT_NUM_SecChHWP, m_nSecHCWCount);
	DDX_Text(pDX, IDC_EDIT_ROOM_NAME, m_strNewRoomName);
	DDX_Check(pDX, IDC_CHECK_ADD_CONTROL, m_bAddControlPoints);
	DDX_Check(pDX, IDC_CHECK_CHILLER_HAS_VALVE, m_bChillerHasValve);
	DDX_Text(pDX, IDC_EDIT_NUM_PriChWP, m_nPriChWPCount);
	DDX_Text(pDX, IDC_EDIT_NUM_FANOFTOWER, m_nFanBlowerCount);
	DDX_Text(pDX, IDC_EDIT_ROOM_DESCRIPTION, m_PlantDescription);
}


BEGIN_MESSAGE_MAP(CNewEquipmentPointsDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NUM_CHILLER, &CNewEquipmentPointsDlg::OnEnChangeEditNumChiller)
	ON_BN_CLICKED(IDOK, &CNewEquipmentPointsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewEquipmentPointsDlg message handlers


void CNewEquipmentPointsDlg::OnEnChangeEditNumChiller()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}



void CNewEquipmentPointsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}



