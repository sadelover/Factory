// ComponentReportHistoryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComponentReportHistoryDlg.h"
#include "afxdialogex.h"

#include "Tools/CustomTools/CustomTools.h"
// CComponentReportHistory �Ի���

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
/* ����˵�����ؼ�ˢ�º���                                               */
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
/*����˵����Json���ݴ��                                                */
/************************************************************************/
Json::Value CComponentReportHistory::GenerateJson()
{
	UpdateData(TRUE);

	/*���*/
	Json::Value cJsonPacket;
	cJsonPacket["type"] = "reportHistory";

	return cJsonPacket;
}

BEGIN_MESSAGE_MAP(CComponentReportHistory, CDialogEx)
END_MESSAGE_MAP()


// CComponentReportHistory ��Ϣ�������
