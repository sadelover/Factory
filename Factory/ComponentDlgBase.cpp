// ComponentDlgBase.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentDlgBase.h"
#include "afxdialogex.h"


// CComponentDlgBase 对话框

IMPLEMENT_DYNAMIC(CComponentDlgBase, CDialogEx)

CComponentDlgBase::CComponentDlgBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialogEx(nIDTemplate, pParent)
{

}

CComponentDlgBase::~CComponentDlgBase()
{
}

void CComponentDlgBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CComponentDlgBase, CDialogEx)
END_MESSAGE_MAP()


// CComponentDlgBase 消息处理程序
