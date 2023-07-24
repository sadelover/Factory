// ComponentReportHistoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentReportHistoryDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"
// CComponentReportHistory 对话框

IMPLEMENT_DYNAMIC(CComponentReportHistory, CDialogEx)

CComponentReportHistory::CComponentReportHistory(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponentReportHistory::IDD, pParent)
{

}

CComponentReportHistory::~CComponentReportHistory()
{
}

void CComponentReportHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


/************************************************************************/
/* 函数说明：控件刷新函数                                               */
/************************************************************************/
void CComponentReportHistory::RefreshControls(Json::Value jsonRoot)
{
	if(jsonRoot["type"] == "reportHistory")
	{
		;
	}
	UpdateData(FALSE);
}

/************************************************************************/
/*函数说明：Json数据打包                                                */
/************************************************************************/
Json::Value CComponentReportHistory::GenerateJson()
{
	UpdateData(TRUE);

	/*打包*/
	Json::Value cJsonPacket;
	cJsonPacket["type"] = "reportHistory";

	return cJsonPacket;
}

BEGIN_MESSAGE_MAP(CComponentReportHistory, CDialogEx)
END_MESSAGE_MAP()


// CComponentReportHistory 消息处理程序
