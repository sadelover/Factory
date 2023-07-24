// DlgSetting_EditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSetting_EditDlg.h"
#include "afxdialogex.h"

#include "LocalConfigManageDlg.h"
#include "MainFrm.h"
#include "json/json.h"

// CDlgSetting_EditDlg �Ի���

IMPLEMENT_DYNAMIC(CDlgSetting_EditDlg, CDialogEx)

CDlgSetting_EditDlg::CDlgSetting_EditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetting_EditDlg::IDD, pParent)
{

}

CDlgSetting_EditDlg::~CDlgSetting_EditDlg()
{
}

void CDlgSetting_EditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_DATATABLE, m_Grid);
}


BEGIN_MESSAGE_MAP(CDlgSetting_EditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgSetting_EditDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgSetting_EditDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CDlgSetting_EditDlg::OnBnClickedButtonModify)
END_MESSAGE_MAP()


// CDlgSetting_EditDlg ��Ϣ�������


void CDlgSetting_EditDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//TODO: Add your control notification handler code here
	CLocalConfigManageDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
		if(pFrame->m_pPrjSqlite)
		{
			pFrame->m_pPrjSqlite->SetValueInLocalConfigTable(dlg.m_strName.GetString(), dlg.m_strValue.GetString());
			for (int i=0;i<m_wstrNameList.size();i++)//��Զѭ��һ�β�֪��ΪɶҪд��ѭ��
			{
				if (dlg.m_strName.GetString()==m_wstrNameList[i])
				{
					AfxMessageBox(_T("�������ظ�!"));
					return;
				}
				else if (dlg.m_strName==_T(""))
				{
					AfxMessageBox(_T("����������Ϊ��!"));
					return;
				}
			}
			m_wstrNameList.push_back(dlg.m_strName.GetString());
			m_wstrValueList.push_back(dlg.m_strValue.GetString());

			m_Grid.SetRowCount(m_wstrNameList.size()+1);
			m_Grid.SetItemText(m_wstrValueList.size(), 0, dlg.m_strName);

			m_Grid.SetItemText(m_wstrValueList.size(), 1, dlg.m_strValue);
			m_Grid.AutoSize(GVS_BOTH);
			m_Grid.Refresh();
		}
	}
	else
	{
		return;
	}
	UpdateData(FALSE);
}


void CDlgSetting_EditDlg::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: Add your control notification handler code here
	CCellID nItem= m_Grid.GetFocusCell();
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if(pFrame->m_pPrjSqlite)
	{
		pFrame->m_pPrjSqlite->RemoveValueInLocalConfigTable(m_Grid.GetItemText(nItem.row,0).GetString());
		m_Grid.DeleteRow(nItem.row);


	}   

	m_Grid.Refresh();

	UpdateData(FALSE);
}


void CDlgSetting_EditDlg::OnBnClickedButtonModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: Add your control notification handler code here
	CCellID nItem= m_Grid.GetFocusCell();
	CString strOldName=m_Grid.GetItemText(nItem.row,0);
	CString strOldValue=m_Grid.GetItemText(nItem.row,1);
	CLocalConfigManageDlg dlg;
	dlg.m_strName=strOldName;
	dlg.m_strValue=strOldValue;
	dlg.m_strValue.Replace(L"\n", L"\r\n");
	if(dlg.DoModal()==IDOK)
	{
		CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
		if(pFrame->m_pPrjSqlite)
		{
			pFrame->m_pPrjSqlite->ModifyValueInLocalConfigTable(strOldName.GetString(),dlg.m_strName.GetString(), dlg.m_strValue.GetString());
			m_Grid.SetItemText(nItem.row,0,dlg.m_strName);
			m_Grid.SetItemText(nItem.row,1,dlg.m_strValue);

		}
		m_Grid.AutoSize(GVS_BOTH);
		m_Grid.Refresh();
	}
	UpdateData(FALSE);
}


BOOL CDlgSetting_EditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if(pFrame->m_pPrjSqlite)
	{
		pFrame->m_pPrjSqlite->GetAllItemsInLocalConfig(m_wstrNameList, m_wstrValueList);
		m_Grid.SetEditable(FALSE);
		m_Grid.SetFixedRowCount(1);
		m_Grid.SetRowCount(m_wstrNameList.size()+1);
		m_Grid.SetColumnCount(2);
		m_Grid.SetItemText(0, 0,_T("name"));
		m_Grid.SetItemText(0, 1,_T("value"));

		for(int i=0;i<m_wstrNameList.size();i++)
		{
			m_Grid.SetItemText(i+1, 0, m_wstrNameList[i].c_str());
			m_Grid.SetItemText(i+1, 1, m_wstrValueList[i].c_str());

		}
		m_Grid.AutoSize(GVS_BOTH);
		m_Grid.Refresh();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
