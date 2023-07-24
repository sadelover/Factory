// ImportTemplateBatchSettingExDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImportTemplateBatchSettingExDlg.h"
#include "afxdialogex.h"


// CImportTemplateBatchSettingExDlg 对话框

IMPLEMENT_DYNAMIC(CImportTemplateBatchSettingExDlg, CDialogEx)

CImportTemplateBatchSettingExDlg::CImportTemplateBatchSettingExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportTemplateBatchSettingExDlg::IDD, pParent)
	, m_strModelSrcName(_T(""))
	, m_strBatchHolderNum1(_T(""))
	, m_strBatchHolderNum2(_T(""))
	, m_strBatchHolderNum3(_T(""))
	, m_strBatchNewPageName(_T(""))
	, m_strPointNameFrom1(_T(""))
	, m_strPointNameTo1(_T(""))
	, m_strPointNameFrom2(_T(""))
	, m_strPointNameTo2(_T(""))
	, m_strPointNameFrom3(_T(""))
	, m_strPointNameTo3(_T(""))
{

}

CImportTemplateBatchSettingExDlg::~CImportTemplateBatchSettingExDlg()
{
}

void CImportTemplateBatchSettingExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL_SRC_NAME, m_strModelSrcName);
	DDX_Text(pDX, IDC_EDIT_BATCH_HOLDER_NUM, m_strBatchHolderNum1);
	DDX_Text(pDX, IDC_EDIT_BATCH_HOLDER_NUM2, m_strBatchHolderNum2);
	DDX_Text(pDX, IDC_EDIT_BATCH_HOLDER_NUM3, m_strBatchHolderNum3);
	DDX_Text(pDX, IDC_EDIT_BATCH_NEW_PAGE, m_strBatchNewPageName);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_FROM, m_strPointNameFrom1);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_TO, m_strPointNameTo1);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_FROM2, m_strPointNameFrom2);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_TO2, m_strPointNameTo2);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_FROM3, m_strPointNameFrom3);
	DDX_Text(pDX, IDC_EDIT_POINT_NAME_TO3, m_strPointNameTo3);
}


BEGIN_MESSAGE_MAP(CImportTemplateBatchSettingExDlg, CDialogEx)
END_MESSAGE_MAP()


// CImportTemplateBatchSettingExDlg 消息处理程序
