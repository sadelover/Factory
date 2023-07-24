// ComponentEnergy.cpp : 实现文件
//

#include "stdafx.h"
#include "ComponentEnergyDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"

// CComponentEnergy 对话框

IMPLEMENT_DYNAMIC(CComponentEnergy, CDialogEx)

CComponentEnergy::CComponentEnergy(CWnd* pParent /*=NULL*/)
	: CComponentDlgBase(CComponentEnergy::IDD, pParent)
{

}

CComponentEnergy::~CComponentEnergy()
{
}

void CComponentEnergy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

/************************************************************************/
/* 函数说明：控件刷新函数                                               */
/************************************************************************/
void CComponentEnergy::RefreshControls(Json::Value jsonRoot)
{
	if(jsonRoot["type"] == "energy")
	{
		return;
	}
	UpdateData(FALSE);
}

Json::Value CComponentEnergy::GenerateJson()
{
	UpdateData(TRUE);

	/*打包*/
	Json::Value cJsonValue;
	cJsonValue["type"] = "energy";

	return cJsonValue;
}
BEGIN_MESSAGE_MAP(CComponentEnergy, CDialogEx)
END_MESSAGE_MAP()


// CComponentEnergy 消息处理程序
