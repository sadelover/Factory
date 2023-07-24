// ComponentEnergy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComponentEnergyDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"

// CComponentEnergy �Ի���

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
/* ����˵�����ؼ�ˢ�º���                                               */
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

	/*���*/
	Json::Value cJsonValue;
	cJsonValue["type"] = "energy";

	return cJsonValue;
}
BEGIN_MESSAGE_MAP(CComponentEnergy, CDialogEx)
END_MESSAGE_MAP()


// CComponentEnergy ��Ϣ�������
