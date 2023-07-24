// ChangePointNameForHistoricalDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChangePointNameForHistoricalDataDlg.h"
#include "afxdialogex.h"

#include "Tools/Tools_String.h"

// CChangePointNameForHistoricalDataDlg 对话框

UINT Modify(LPVOID pParam);

IMPLEMENT_DYNAMIC(CChangePointNameForHistoricalDataDlg, CDialogEx)

CChangePointNameForHistoricalDataDlg::CChangePointNameForHistoricalDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangePointNameForHistoricalDataDlg::IDD, pParent)
	, m_nGridRow(5)
	, m_cstrTempPointName(_T(""))
{

}

CChangePointNameForHistoricalDataDlg::~CChangePointNameForHistoricalDataDlg()
{
}

void CChangePointNameForHistoricalDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_NAME_LIST, m_Grid_NameList);
	DDX_Text(pDX, IDC_EDIT_GRID_ROW, m_nGridRow);
	DDX_Text(pDX, IDC_EDIT_GET_POINT_NAME, m_cstrTempPointName);
	DDX_Control(pDX, IDC_PROGRESS2, m_PgsCtlModify);
}



BEGIN_MESSAGE_MAP(CChangePointNameForHistoricalDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SET_GRID_ROW, &CChangePointNameForHistoricalDataDlg::OnBnClickedButtonSetGridRow)

	ON_BN_CLICKED(IDC_BUTTON_QUERY_POINT, &CChangePointNameForHistoricalDataDlg::OnBnClickedButtonQueryPoint)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CChangePointNameForHistoricalDataDlg::OnBnClickedButtonModify)
END_MESSAGE_MAP()


// CChangePointNameForHistoricalDataDlg 消息处理程序

BOOL CChangePointNameForHistoricalDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// TODO:  在此添加额外的初始化
	CRect cr;
	m_Grid_NameList.GetClientRect(&cr);

	m_Grid_NameList.SetRowCount(m_nGridRow+1);//设置行数量
	m_Grid_NameList.SetColumnCount(3);//设置列数量
	m_Grid_NameList.SetFixedRowCount(1);
	m_Grid_NameList.SetFixedColumnCount(1);//表头一行一列
	m_Grid_NameList.SetEditable(true);//表格内容不可编辑设置
	m_Grid_NameList.SetItemText(0, 1, _T("旧点名"));//设置内容
	m_Grid_NameList.SetItemText(0, 2, _T("新点名"));//设置内容
	m_Grid_NameList.SetColumnWidth(1,200);
	m_Grid_NameList.SetColumnWidth(2,200);

	//设置行标的列宽
	CString cstrRow;
	cstrRow.Format(L"%d",m_nGridRow);
	int nNumber = cstrRow.GetLength()*7+15;
	m_Grid_NameList.SetColumnWidth(0, nNumber);
	//刷新行标识
	CString strRowNum;
	for(int i=1; i<=m_nGridRow; i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid_NameList.SetItemText(i, 0, strRowNum);
	}
	m_PgsCtlModify.SetRange(0,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChangePointNameForHistoricalDataDlg::OnBnClickedButtonSetGridRow()
{
	// TODO: 在此添加控件通知处理程序代码、
	UpdateData(TRUE);

	//设置行标的列宽
	CString cstrRow;
	cstrRow.Format(L"%d",m_nGridRow);
	int nNumber = cstrRow.GetLength()*7+15;
	m_Grid_NameList.SetColumnWidth(0, nNumber);

	//刷新行标识
	int nRow = m_nGridRow+1;
	m_Grid_NameList.SetRowCount(nRow);
	CString strRowNum;
	for(int i=1; i<=m_nGridRow; i++)//第一行不写
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid_NameList.SetItemText(i, 0, strRowNum);
	}

	UpdateData(FALSE);
}

void CChangePointNameForHistoricalDataDlg::OnBnClickedButtonQueryPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if (dlg.DoModal() == IDOK)
	{
		m_cstrTempPointName = dlg.m_strPointName;
	}
	UpdateData(FALSE);
}

//修改按钮点击事件函数
void CChangePointNameForHistoricalDataDlg::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	CWinThread	*pMyFirstWorker;
	LPVOID		pParam = (void *)this;
	pMyFirstWorker = AfxBeginThread(Modify, pParam);//如果采用默认值
	SetButtonModifyState(FALSE);
}

//设置修改按钮的禁用情况
void CChangePointNameForHistoricalDataDlg::SetButtonModifyState(BOOL bState)
{
	CButton * btCtl = (CButton*)GetDlgItem(IDC_BUTTON_MODIFY);
	btCtl->EnableWindow(bState);
}

//其他方法

void CChangePointNameForHistoricalDataDlg::Set_m_pDataAccess(CUPPCServerDataAccess* pDataAccess)
{
	m_pDataAccess = pDataAccess;
}
CUPPCServerDataAccess* CChangePointNameForHistoricalDataDlg::Get_m_pDataAccess()
{
	return m_pDataAccess;
}

//设置进度条
void CChangePointNameForHistoricalDataDlg::SetProgressPos(int nPos)
{
	m_PgsCtlModify.SetPos(nPos);
}

//从表格中获取新旧点名
void CChangePointNameForHistoricalDataDlg::GetAllPointName(vector<wstring>& _wstrOldNameList, vector<wstring>& _wstrNewNameList)
{
	for(int i=1; i<=m_nGridRow; i++)
	{
		wstring wstrOldName = m_Grid_NameList.GetItemText(i,1).GetString();
		wstring wstrNewName = m_Grid_NameList.GetItemText(i,2).GetString();
		if(wstrOldName.length() <= 0 || wstrNewName.length() <= 0)
		{
			//全空跳过
			if(wstrOldName.length() <= 0 && wstrNewName.length() <= 0)
				continue;
			CString cstrMsg;cstrMsg.Format(L"%d行，有空的这是不被允许的。",i);
			AfxMessageBox(cstrMsg);
			return ;
		}
		_wstrOldNameList.push_back(wstrOldName);
		_wstrNewNameList.push_back(wstrNewName);
	}
}

//修改线程
UINT Modify(LPVOID pParam)
{
	CChangePointNameForHistoricalDataDlg* pThis = (CChangePointNameForHistoricalDataDlg*)pParam;

	pThis->SetProgressPos(0);

	vector<wstring> wstrOldNameList;
	vector<wstring> wstrNewNameList;
	vector<wstring> wstrHistoryDataTableList;
	pThis->GetAllPointName(wstrOldNameList, wstrNewNameList);
	pThis->SetProgressPos(20);
	//错误检查
	{
		CString cstrMessage;
		CString cstrTempMsg;
		bool bFindError = true;
		cstrMessage+=L"有问题的点名:\n";
		//原点名
		for( int nCount=0;nCount<wstrOldNameList.size();nCount++ )
		{
			//首字母不允许用字母下划线以外的字符
			if(Tools_String::Whether_First_LettersUnderscores(wstrOldNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"第%d行 %s\n", nCount+1, wstrOldNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
				continue;//不进行下一步判断
			}

			//字符串只允许字符数字下划线
			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrOldNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"第%d行 %s\n", nCount+1, wstrOldNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
			}
		}
		pThis->SetProgressPos(40);
		//新点名
		for( int nCount=0;nCount<wstrNewNameList.size();nCount++ )
		{
			//首字母不允许用字母下划线以外的字符
			if(Tools_String::Whether_First_LettersUnderscores(wstrNewNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"第%d行 %s\n", nCount+1, wstrNewNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
				continue;//不进行下一步判断
			}

			//字符串只允许字符数字下划线
			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrNewNameList[nCount]) == false)
			{
				cstrTempMsg.Empty();
				cstrTempMsg.Format(L"第%d行 %s.\n",nCount+1,wstrNewNameList[nCount].c_str());
				cstrMessage+=cstrTempMsg;
				bFindError = false;
			}
		}
		pThis->SetProgressPos(60);
		if(bFindError == false)
		{
			pThis->SetProgressPos(0);
			pThis->SetButtonModifyState(TRUE);
			AfxMessageBox(cstrMessage);
			return 0;
		}
	}

	CUPPCServerDataAccess* pDataAccess = pThis->Get_m_pDataAccess();
	pDataAccess->ReadBeopdataAllHistorydataList(wstrHistoryDataTableList);
	pThis->SetProgressPos(65);
	pDataAccess->UpdateHistoryDataPointName(wstrHistoryDataTableList, wstrOldNameList, wstrNewNameList);
	pThis->SetProgressPos(100);
	AfxMessageBox(L"修改完成");
	pThis->SetButtonModifyState(TRUE);
	return 1;
}

//UINT MyThreadFunction(LPVOID pParam)
//{
//	CChangePointNameForHistoricalDataDlg* pThis = (CChangePointNameForHistoricalDataDlg*)pParam;
//	
//	pThis->SetProgressPos(0);
//
//	vector<wstring> wstrOldNameList;
//	vector<wstring> wstrNewNameList;
//	vector<wstring> wstrHistoryDataTableList;
//	pThis->GetAllPointName(wstrOldNameList, wstrNewNameList);
//	for(int i=1; i<=m_nGridRow; i++)
//	{
//		wstring wstrOldName = m_Grid_NameList.GetItemText(i,1).GetString();
//		wstring wstrNewName = m_Grid_NameList.GetItemText(i,2).GetString();
//		if(wstrOldName.length() <= 0 || wstrNewName.length() <= 0)
//		{
//			//全空跳过
//			if(wstrOldName.length() <= 0 && wstrNewName.length() <= 0)
//				continue;
//			CString cstrMsg;cstrMsg.Format(L"%d行，有空的这是不被允许的。",i);
//			AfxMessageBox(cstrMsg);
//			return 0;
//		}
//		wstrOldNameList.push_back(wstrOldName);
//		wstrNewNameList.push_back(wstrNewName);
//	}
//
//	pThis->SetProgressPos(20);
//	//错误检查
//	{
//		CString cstrMessage;
//		CString cstrTempMsg;
//		bool bFindError = true;
//		cstrMessage+=L"有问题的点名:\n";
//		//原点名
//		for( int nCount=0;nCount<wstrOldNameList.size();nCount++ )
//		{
//			//首字母不允许用字母下划线以外的字符
//			if(Tools_String::Whether_First_LettersUnderscores(wstrOldNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"第%d行 %s\n", nCount+1, wstrOldNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//				continue;//不进行下一步判断
//			}
//
//			//字符串只允许字符数字下划线
//			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrOldNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"第%d行 %s\n", nCount+1, wstrOldNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//			}
//		}
//		pThis->SetProgressPos(40);
//		//新点名
//		for( int nCount=0;nCount<wstrNewNameList.size();nCount++ )
//		{
//			//首字母不允许用字母下划线以外的字符
//			if(Tools_String::Whether_First_LettersUnderscores(wstrNewNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"第%d行 %s\n", nCount+1, wstrNewNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//				continue;//不进行下一步判断
//			}
//
//			//字符串只允许字符数字下划线
//			if(Tools_String::Whether_Only_LettersNumbersUnderscores(wstrNewNameList[nCount]) == false)
//			{
//				cstrTempMsg.Empty();
//				cstrTempMsg.Format(L"第%d行 %s.\n",nCount+1,wstrNewNameList[nCount].c_str());
//				cstrMessage+=cstrTempMsg;
//				bFindError = false;
//			}
//		}
//		pThis->SetProgressPos(60);
//		if(bFindError == false)
//		{
//			AfxMessageBox(cstrMessage);
//			return 0;
//		}
//	}
//
//	m_pDataAccess->ReadBeopdataAllHistorydataList(wstrHistoryDataTableList);
//	pThis->SetProgressPos(65);
//	m_pDataAccess->UpdateHistoryDataPointName(wstrHistoryDataTableList, wstrOldNameList, wstrNewNameList);
//	pThis->SetProgressPos(100);
//	AfxMessageBox(L"修改完成");
//	return 1;
//}