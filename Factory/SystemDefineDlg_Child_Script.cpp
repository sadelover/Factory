// SystemDefineDlg_Child_Script.cpp : 实现文件
//

#include "stdafx.h"
#include "SystemDefineDlg_Child_Script.h"
#include "afxdialogex.h"
#include "./json/json.h"
#include "./Tools/CustomTools/CustomTools.h"

// CSystemDefineDlg_Child_Script 对话框

IMPLEMENT_DYNAMIC(CSystemDefineDlg_Child_Script, CDialogEx)

CSystemDefineDlg_Child_Script::CSystemDefineDlg_Child_Script(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemDefineDlg_Child_Script::IDD, pParent),
	m_strSystemDefine(_T("{\r\n}"))
{

}

CSystemDefineDlg_Child_Script::~CSystemDefineDlg_Child_Script()
{
}

void CSystemDefineDlg_Child_Script::SetData(CString & cstrSysDefine)
{
	m_strSystemDefine.Empty();
	if(cstrSysDefine == L"null" || cstrSysDefine == L"")
		m_strSystemDefine = L"{\r\n}";
	else
		m_strSystemDefine = cstrSysDefine;
	m_strSystemDefine.Replace(L"\n",L"\r\n");//在Win系统中换行需要\r\n
	UpdateData(FALSE);
}

bool CSystemDefineDlg_Child_Script::GetData(CString & cstrSysDefine)
{
	UpdateData(TRUE);
	Json::Value JsonValue;
	Json::Reader JsonReader;

	if(m_strSystemDefine == L"")
	{
		cstrSysDefine = L"{\r\n}";
		return true;
	}

	string strTemp = Project::Tools::WideCharToUtf8(m_strSystemDefine.GetString());
	if(JsonReader.parse(strTemp, JsonValue))
	{
		cstrSysDefine = m_strSystemDefine;
		return true;
	}
	else
	{
		AfxMessageBox(L"json格式不正确");
		return false;
	}

}

BOOL CSystemDefineDlg_Child_Script::PreTranslateMessage(MSG* pMsg)
{
	//判断是否为键盘消息
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		BOOL bCtrl = ::GetKeyState(VK_CONTROL) & 0x80;
		//判断是否按下键盘Enter键
		if(pMsg->wParam==VK_ESCAPE)
		{

			return TRUE;
		}
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return TRUE;
			break;
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CSystemDefineDlg_Child_Script::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONFIG, m_strSystemDefine);
}


BEGIN_MESSAGE_MAP(CSystemDefineDlg_Child_Script, CDialogEx)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CSystemDefineDlg_Child_Script 消息处理程序



BOOL CSystemDefineDlg_Child_Script::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CEdit *)GetDlgItem(IDC_EDIT_CONFIG))->LimitText(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSystemDefineDlg_Child_Script::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nChar == VK_ESCAPE)
	{
		return;
	}
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CSystemDefineDlg_Child_Script::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nChar == VK_ESCAPE)
	{
		return;
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
