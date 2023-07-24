// ThreadPeriodicitySet.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadPeriodicitySet.h"
#include "afxdialogex.h"


// CThreadPeriodicitySet dialog

IMPLEMENT_DYNAMIC(CThreadPeriodicitySet, CDialogEx)

CThreadPeriodicitySet::CThreadPeriodicitySet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThreadPeriodicitySet::IDD, pParent)
	, m_nThreadPeriodicity(0)
{

}

CThreadPeriodicitySet::~CThreadPeriodicitySet()
{
}

void CThreadPeriodicitySet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THREAD_PERIO, m_nThreadPeriodicity);
}


BEGIN_MESSAGE_MAP(CThreadPeriodicitySet, CDialogEx)
END_MESSAGE_MAP()


// CThreadPeriodicitySet message handlers
