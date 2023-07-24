// DatabaseBackupManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DatabaseBackupManagerDlg.h"
#include "afxdialogex.h"
#include "Tools/CustomTools/CustomTools.h"
#include "Tools/DirectoryOperations.h"
// CDatabaseBackupManagerDlg 对话框

IMPLEMENT_DYNAMIC(CDatabaseBackupManagerDlg, CDialogEx)

CDatabaseBackupManagerDlg::CDatabaseBackupManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseBackupManagerDlg::IDD, pParent)
{

}

CDatabaseBackupManagerDlg::~CDatabaseBackupManagerDlg()
{
}

void CDatabaseBackupManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ZIP_FILE, m_ZipFileList_Ctl);
	DDX_Control(pDX, IDC_PROGRESS_Export, m_ExportProgress_CTL);
}


BEGIN_MESSAGE_MAP(CDatabaseBackupManagerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CDatabaseBackupManagerDlg::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// CDatabaseBackupManagerDlg 消息处理程序


void CDatabaseBackupManagerDlg::OnBnClickedButtonExport()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(MyControllingFunction,(LPVOID)this);
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
	
	//CFolderPickerDialog dlg;
	//if(dlg.DoModal() == IDOK)
	//{
	//	CString cstrNewPath = dlg.GetPathName();
	//	cstrNewPath += L"\\";

	//	int nMax = m_ZipFileList_Ctl.GetSelCount();
	//	int * nSelList = new int[nMax];
	//	m_ZipFileList_Ctl.GetSelItems(nMax, nSelList);

	//	for(int i = 0;i<nMax; i++)
	//	{
	//		CString cstrFileName;
	//		m_ZipFileList_Ctl.GetText(i,cstrFileName);
	//		CopyFile(m_cstrBackupPath+cstrFileName, cstrNewPath+cstrFileName, FALSE);
	//		if(false == DirectoryOperations::CheckFileExist(cstrNewPath, cstrFileName))
	//		{
	//			while(true)//一直查询 临时方案
	//			{
	//				if(true == DirectoryOperations::CheckFileExist(cstrNewPath, cstrFileName))
	//				{
	//					break;
	//				}
	//				Sleep(500);
	//			}
	//		}
	//	}
	//	delete [] nSelList;

	//	AfxMessageBox(L"导出成功");
	//}
}
UINT MyControllingFunction( LPVOID pParam )
{
	auto pthis = (CDatabaseBackupManagerDlg *) pParam;

	//获取被选中的索引
	int nMax = pthis->m_ZipFileList_Ctl.GetSelCount();
	if(nMax == 0)
	{
		pthis->GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
		return 0;
	}
	int * nSelList = new int[nMax];
	pthis->m_ZipFileList_Ctl.GetSelItems(nMax, nSelList);

	CFolderPickerDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString cstrNewPath = dlg.GetPathName();
		cstrNewPath += L"\\";

		//进度条分度设置
		int nPos = 100/nMax;

		//复制文件
		for(int i = 0;i<nMax; i++)
		{
			CString cstrFileName;
			pthis->m_ZipFileList_Ctl.GetText(i,cstrFileName);
			CopyFile(pthis->m_cstrBackupPath+cstrFileName, cstrNewPath+cstrFileName, FALSE);
			if(false == DirectoryOperations::CheckFileExist(cstrNewPath, cstrFileName))
			{
				for(int i=0;i<120;i++)//一直查询 临时方案
				{
					if(true == DirectoryOperations::CheckFileExist(cstrNewPath, cstrFileName))
					{
						break;
					}
					Sleep(500);
				}
			}
			pthis->m_ExportProgress_CTL.SetPos(nPos);
		}
		pthis->m_ExportProgress_CTL.SetPos(100);
		delete [] nSelList;

		AfxMessageBox(L"导出成功");
	}
	pthis->GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
	
	return 0;
}

BOOL CDatabaseBackupManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	vector<CString> cstrZipNameList;

	Project::Tools::GetCorePath(m_cstrBackupPath);
	m_cstrBackupPath = m_cstrBackupPath +  L"\\mysqlLocalBackup\\";
	DirectoryOperations::GetAllFileNameList(m_cstrBackupPath, cstrZipNameList);

	vector<CString>::iterator pItem = cstrZipNameList.begin();
	for(; pItem!= cstrZipNameList.end(); pItem++)
	{
		m_ZipFileList_Ctl.AddString(*pItem);
	}

	m_ExportProgress_CTL.SetRange(0,100);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
