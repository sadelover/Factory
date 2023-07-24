// ComponentModelControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComponentModelControlDlg.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"

// CComponentModelControlDlg �Ի���

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


// CComponentModelControlDlg ��Ϣ�������

BOOL CComponentModelControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/************************************************************************/
/*����˵����ˢ�¿ؼ�����                                                */
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
/* ����˵�������ղ������json������                                     */
/************************************************************************/
Json::Value CComponentModelControlDlg::GenerateJson()
{
	UpdateData(TRUE);
	/*���*/
	Json::Value cJsonPacket;
	cJsonPacket["type"] = Project::Tools::WideCharToAnsi(_T("modelControl"));
	cJsonPacket["modelType"] = m_nModelType;

	return cJsonPacket;
}