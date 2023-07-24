// CreateSample.cpp : implementation file
//

#include "stdafx.h"
#include "CreateSample.h"
#include "afxdialogex.h"


// CCreateSample dialog

IMPLEMENT_DYNAMIC(CCreateSample, CDialogEx)

CCreateSample::CCreateSample(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateSample::IDD, pParent)
	, m_nInputNum(0)
	, m_nOutputNum(0)
	, m_strSampleName(_T(""))
{

}

CCreateSample::~CCreateSample()
{
}

void CCreateSample::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT_NUM, m_nInputNum);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_NUM, m_nOutputNum);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_NAME, m_strSampleName);
}


BEGIN_MESSAGE_MAP(CCreateSample, CDialogEx)
END_MESSAGE_MAP()


// CCreateSample message handlers
