// ModbusComConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModbusComConfigDlg.h"
#include "afxdialogex.h"

#include "json/json.h"
#include "./ControlEx/WarningDlg.h"
#include "./Tools/CustomTools/CustomTools.h"


// CModbusComConfigDlg dialog

IMPLEMENT_DYNAMIC(CModbusComConfigDlg, CDialogEx)

CModbusComConfigDlg::CModbusComConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModbusComConfigDlg::IDD, pParent)
	, m_strConfig(_T(""))
{

}

CModbusComConfigDlg::~CModbusComConfigDlg()
{
}

void CModbusComConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONFIG, m_strConfig);
}


BEGIN_MESSAGE_MAP(CModbusComConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CModbusComConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CModbusComConfigDlg message handlers


void CModbusComConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	try
	{
		string strAll;
		Project::Tools::WideCharToUtf8(m_strConfig.GetString(), strAll);
		Json::Value myRoot(strAll);

		Json::Reader jsReader;

		if (strAll.length()>0 && jsReader.parse(strAll, myRoot))
		{
			if(myRoot.isObject())
				CDialogEx::OnOK();
			else
			{
				CWarningDlg dlg;
				dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
				dlg.DoModal();
			}
		}
		else
		{
			CWarningDlg dlg;
			dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
			dlg.DoModal();
		}

	}
	catch (CMemoryException* e)
	{
		CWarningDlg dlg;
		dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
		dlg.DoModal();
		return;
	}
	catch (CFileException* e)
	{
		CWarningDlg dlg;
		dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
		dlg.DoModal();
		return;
	}
	catch (CException* e)
	{
		CWarningDlg dlg;
		dlg.InitWarn(_T("提示"),_T("配置内容不合标准，请核对修改。"));
		dlg.DoModal();
		return;
	}


}


BOOL CModbusComConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		switch (pMsg->wParam)
		{
		case 'a':
		case 'A':
			if (bCtrl)
			{
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CONFIG);
				if (pEdit != NULL)
				{
					pEdit->SetFocus();
					pEdit->SetSel(0, -1);
				}
			}  
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
