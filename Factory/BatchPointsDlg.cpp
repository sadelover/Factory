// BatchPoints.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchPointsDlg.h"
#include "afxdialogex.h"
#include "DataPointConfigDlg.h"

// CBatchPointsDlg 对话框

IMPLEMENT_DYNAMIC(CBatchPointsDlg, CDialogEx)

CBatchPointsDlg::CBatchPointsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBatchPointsDlg::IDD, pParent)
	, m_nRowNumber(0)
{

}

CBatchPointsDlg::~CBatchPointsDlg()
{
}

void CBatchPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_ROW_NUMBER, m_nRowNumber);
}


BEGIN_MESSAGE_MAP(CBatchPointsDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_ADD_ROW, &CBatchPointsDlg::OnBnClickedButtonAddRow)
	ON_BN_CLICKED(IDOK, &CBatchPointsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBatchPointsDlg 消息处理程序


BOOL CBatchPointsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	InitGRID();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBatchPointsDlg::InitGRID (void)
{
	CRect cr;
	m_Grid.GetClientRect(&cr);

	m_nRowNumber = 10;
	ncolNum = 3;

	m_Grid.SetRowCount(m_nRowNumber+1);//设置行数量
	m_Grid.SetColumnCount(ncolNum);//设置列数量
	
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);//表头一行一列
	
	COLORREF clr = RGB(0, 0, 0);
	m_Grid.SetFixedTextColor(clr);//设置固定行的颜色
	m_Grid.SetFixedBkColor(RGB(192, 192, 192));//设置固定行的背景颜色

	m_Grid.SetTextBkColor(RGB(255, 255, 255));//设置可编辑区域背景颜色
	m_Grid.SetTextColor(RGB(0, 0, 0));//设置可编辑区域文字颜色
	m_Grid.SetEditable(true);//表格内容不可编辑设置

	m_Grid.SetColumnWidth(0, 35);//设置第一列宽
	m_Grid.SetColumnWidth(1, 200);//设置第一列宽
	m_Grid.SetColumnWidth(2, 200);//设置第一列宽
	m_Grid.SetRowHeight(0, 35);//设置行高

	m_Grid.SetItemText(0, 1, _T("点名"));//设置内容
	m_Grid.SetItemText(0, 2, _T("注释"));//设置内容

	CString strRowNum;
	for(int i=1; i<m_nRowNumber+1; i++)
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid.SetItemText(i, 0, strRowNum);
	}
}

//"设置行数"按钮服务函数
void CBatchPointsDlg::OnBnClickedButtonAddRow()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_Grid.SetRowCount(m_nRowNumber+1);
	CString strRowNum;
	for(int i=1; i<=m_nRowNumber; i++)//包括一个不可更改行
	{
		strRowNum = _T("");
		strRowNum.Format(_T("%d"),i);
		m_Grid.SetItemText(i, 0, strRowNum);
	}

	//自动适配第一列宽度
	CString cstrRomNum;
	cstrRomNum.Format(L"%d",m_nRowNumber);
	int nRomWidth = (cstrRomNum.GetLength() *10) +5;
		m_Grid.SetColumnWidth(0, nRomWidth);//设置第一列宽

	UpdateData(FALSE);
}
void CBatchPointsDlg::SetDlgType( DLG_TYPE eType )
{
	m_eType = eType;
}
void CBatchPointsDlg::SetPointMap(CDataPointManager* pPointMap)
{
	ASSERT(pPointMap != NULL);
	if (pPointMap != NULL)
	{
		m_pPointMap = pPointMap;
	}
}
int CBatchPointsDlg::GetMaxKeyID()
{
	int nMaxID = 0;
	CDataPointConfigDlg* pWnd = (CDataPointConfigDlg*)GetParent();
	ASSERT(pWnd);
	map<int, DataPointEntry>& datalist = pWnd->m_plcpointmap.GetPointMap();
	map<int, DataPointEntry>::iterator iter = datalist.begin();
	for (;iter!=datalist.end();++iter)
	{
		if (iter->first > nMaxID)
		{
			nMaxID = iter->first;
		}
	}
	return nMaxID;
}
void CBatchPointsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
		点名检查和填充点空间
	*/
	m_nReallNum=0;
	m_TableOfName.clear();
	m_TableOfNotation.clear();
	for(int i=1;i<m_nRowNumber;i++)
	{

		CString cstrPointName = m_Grid.GetItemText(i,1);
		CString cstrPointDes  = m_Grid.GetItemText(i,2);
		
		//检查空行直接跳过
		if(cstrPointName == L"")
		{
			continue;
		}

		//检查首字母
		if((cstrPointName[0]>=L'a'& cstrPointName[0]<='z') || (cstrPointName[0]>=L'A' & cstrPointName[0]<=L'Z'))
		{

		}
		else
		{
			CString cstrAfx;
			cstrAfx.Format(L"第%d行\"%s\"变量ID必须以英文字符开头!",i ,cstrPointName);
			AfxMessageBox(cstrAfx);
			return;
		}

		//删除点名中存在的空格
		if(cstrPointName.Find(L' ') != -1)
		{
			int nWordMax = cstrPointName.GetLength();
			for(int nWord=0; nWord<nWordMax; nWord++)
			{
				int nSpaceLocation = cstrPointName.Find(L' ');
				if(nSpaceLocation != -1)
					cstrPointName.Delete(nSpaceLocation);
				else
					break;
			}
		}

		//检查点名中的非法字符
		for(int j=0;j<cstrPointName.GetLength();j++)
		{
			wchar_t wcOne = cstrPointName.GetAt(j);
			if ((wcOne>=L'a' && wcOne<=L'z') || (wcOne>=L'A' && wcOne<=L'Z') || wcOne=='_' || (wcOne>=L'0' && wcOne<=L'9'))
			{

			}
			else
			{
				CString cstrAfx;
				cstrAfx.Format(L"第%d行的\"%s\"变量名中含有非法字符(只支持英文字母、阿拉伯数字、下划线!",i , cstrPointName);
				AfxMessageBox(cstrAfx);
				return;
			}
		}

		////检测末尾是否有非法字符
		//int nErr_1 = cstrPointName.Find(_T(" "));
		//int nErr_2 = cstrPointName.Find(_T("\n"));
		//int nErr_3 = cstrPointName.Find(_T("\r"));
		//int nErr_4 = cstrPointName.Find(_T("@"));
		//int nErr_5 = cstrPointName.Find(_T("*"));
		//if (nErr_1!=-1||nErr_2!=-1||nErr_3!=-1||nErr_4!=-1||nErr_5!=-1)
		//{
		//	CString AfxStr = _T("%s有非法字符中又非法字符:",cstrPointName);
		//	if(nErr_1!=-1)
		//		AfxStr+=_T("空格");
		//	if(nErr_2!=-1)
		//		AfxStr+=_T("\\n");
		//	if(nErr_3!=-1)
		//		AfxStr+=_T("\\r");
		//	if(nErr_4!=-1)
		//		AfxStr+=_T("@");
		//	if(nErr_5!=-1)
		//		AfxStr+=_T("*");
		//	AfxMessageBox(AfxStr);
		//	return;
		//}

		m_nReallNum++;
		m_TableOfName.push_back(cstrPointName);
		m_TableOfNotation.push_back(cstrPointName);
	}

	/*
		点保存
	*/
	CDataPointConfigDlg* pWnd = (CDataPointConfigDlg*)GetParent();
	ASSERT(pWnd);

	DataPointEntry entry;
	map<int, DataPointEntry>& datalist = pWnd->m_plcpointmap.GetPointMap();
	for(int i=0;i<m_nReallNum;i++)
	{
		//检查ID是否重复
		map<int, DataPointEntry>::const_iterator it = datalist.begin();
		for (;it != datalist.end(); it++)
		{
			const DataPointEntry& entryMap = it->second;
			if (entryMap.GetShortName() == m_TableOfName.at(i).GetString())
			{
				AfxMessageBox(L"PhysicalID重复!");
				return;
			}
		}
		entry.SetPointIndex(GetMaxKeyID()+1);
		entry.SetShortName(m_TableOfName.at(i).GetString());
		entry.SetShortNameOrigin(m_TableOfName.at(i).GetString());
		entry.SetSourceType(L"vpoint");
		entry.SetDescription( m_TableOfNotation.at(i).GetString() );
		entry.SetUnit(L"");
		entry.SetProperty(PLC_WRITE);
	
		entry.SetParam(1, L"");
		entry.SetParam(2, L"");
		entry.SetParam(3, L"");
		entry.SetParam(4, L"");
		entry.SetParam(5, L"");
		entry.SetParam(6, L"");
		entry.SetParam(7, L"");
		entry.SetParam(8, L"");
		entry.SetParam(9, L"");
		entry.SetParam(10, L"");
		entry.SetParam(11, L"");
		entry.SetParam(12, L"");
		entry.SetParam(13, L"");
		entry.SetParam(14, L"0");
		entry.SetStoreCycle(E_STORE_ONE_MINUTE);
		entry.SetParam(15, L"");
		entry.SetParam(16, L"");
		entry.SetParam(17, L"");
		entry.SetParam(18, L"");

		pWnd->AddOneEntry(entry);
	}
	//delete pWnd;
	CDialogEx::OnOK();
}
