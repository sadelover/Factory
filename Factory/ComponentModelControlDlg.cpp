// ComponentModelControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentModelControlDlg.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"

// CComponentModelControlDlg 对话框

IMPLEMENT_DYNAMIC(CComponentModelControlDlg, CDialogEx)

CComponentModelControlDlg::CComponentModelControlDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponentModelControlDlg::IDD, pParent)
	, m_nModelType(0)
{

}

CComponentModelControlDlg::~CComponentModelControlDlg()
{
}

void CComponentModelControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nModelType);
}


BEGIN_MESSAGE_MAP(CComponentModelControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CComponentModelControlDlg 消息处理程序

BOOL CComponentModelControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/************************************************************************/
/*函数说明：刷新控件参数                                                */
/************************************************************************/
void CComponentModelControlDlg::RefreshControls( Json::Value jsonRoot)
{
	if(jsonRoot["type"] == "modelControl")
	{
		m_nModelType = jsonRoot["modelType"].asInt();
	}
	UpdateData(FALSE);
}

/************************************************************************/
/* 函数说明：按照参数打包json并返回                                     */
/************************************************************************/
Json::Value CComponentModelControlDlg::GenerateJson()
{
	UpdateData(TRUE);
	/*打包*/
	Json::Value cJsonPacket;
	cJsonPacket["type"] = Project::Tools::WideCharToAnsi(_T("modelControl"));
	cJsonPacket["modelType"] = m_nModelType;

	return cJsonPacket;
}