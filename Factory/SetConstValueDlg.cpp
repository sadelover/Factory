
#include "stdafx.h"
#include "SetConstValueDlg.h"
#include "afxdialogex.h"
#include "ControlEx/WarningDlg.h"
#include "Tools/CustomTools/CustomTools.h"
#include "Tools/Util/UtilString.h"
#include "MainFrm.h"
#include "DataComConfigDlg.h"
#include "HttpOperation.h"
#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"


IMPLEMENT_DYNAMIC(CSetConstValueDlg, CDialog)

CSetConstValueDlg::CSetConstValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetConstValueDlg::IDD, pParent)
	, m_sSelStrName(_T(""))
	, m_dSelCurValue(_T(""))
	, m_strSetValue(_T(""))
	, m_ConValue(_T(""))
	, m_sValueType(_T(""))
	, m_bCanModifyType(true)
{
	vecPointList.clear();
	vecPointIndex.clear();
}
CSetConstValueDlg::~CSetConstValueDlg()
{

}
void CSetConstValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_strategyname, m_sSelStrName);
	DDX_Text(pDX, IDC_EDIT_curtimelong, m_dSelCurValue);

	DDX_Text(pDX, IDC_EDIT_PARAM_VALUE, m_strParamSetting);

	DDX_Control(pDX, IDC_COMBO_PointType, m_ComPointType);
}

BEGIN_MESSAGE_MAP(CSetConstValueDlg, CDialog)

	ON_BN_CLICKED(IDOK, &CSetConstValueDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetConstValueDlg::OnBnClickedCancel)
	//ON_EN_KILLFOCUS(IDC_EDIT_Serch, &CSetConstValueDlg::OnEnKillfocusEditSerch)
	//ON_EN_KILLFOCUS(IDC_EDIT_CONSerch, &CSetConstValueDlg::OnEnKillfocusEditConserch)
//	ON_EN_CHANGE(IDC_EDIT_CONSerch, &CSetConstValueDlg::OnEnChangeEditConserch)
	ON_EN_CHANGE(IDC_EDIT_strategyname, &CSetConstValueDlg::OnEnChangeEditstrategyname)
	
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_POINT, &CSetConstValueDlg::OnBnClickedButtonChoosePoint)
	ON_BN_CLICKED(IDC_BUTTON_TEST_SCRIPT, &CSetConstValueDlg::OnBnClickedButtonTestScript)
END_MESSAGE_MAP()


void CSetConstValueDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	
	m_ComPointType.GetWindowText(m_sValueType);

	m_strSetValue = m_strParamSetting;

	if(m_strSetValue.GetLength()<0)
	{
			CWarningDlg dlgWarn;
			dlgWarn.InitWarn(L"错误", L"配置值不合法");
			dlgWarn.DoModal();
			return;
	}
	else if(m_strSetValue.Left(1)==_T(" ") || m_strSetValue.Right(1)==_T(" "))
	{
		CWarningDlg dlgWarn;
		dlgWarn.InitWarn(L"错误", L"配置值首位和末尾存在空格，请重新检查后保存");
		dlgWarn.DoModal();
		return;
	}

	int nValue = 0;
	if (UtilString::CheckStringIsNumber(m_strSetValue.GetString(), nValue))
	{ 
	
	}else
	{
		ReloadNameAndIndexForPoint();//可能有新添加的点所以要重载，参数表。
		bool bFind = false;
		for(UINT idx = 0; idx< vecPointList.size(); idx++)
		{
			wstring ini_str    = vecPointList[idx].GetString();
			if(UtilString::FindStringInString(ini_str,m_strSetValue.GetString()))
			{
				bFind = true;
				break;
			}
		}

		if (m_sValueType == _T("point"))
		{
			if(bFind==false)
			{
				/*CWarningDlg dlgWarn;
				dlgWarn.InitWarn(L"错误", L"配置值不合法");
				dlgWarn.DoModal();
				return;*/
 				if (AfxMessageBox(L"配置点名不存在，是否继续保存？",MB_OKCANCEL) == IDOK)
				{

                  ////CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				  //int nid=pFrame->m_pPrjSqlite->GetMaxIDInPointTable();
				  //bool bset=pFrame->m_pPrjSqlite->InsertNewPointIntoPointTable(nid+1,m_strSetValue);
				  /*if (bset)
				  {
					  AfxMessageBox(_T("保存成功！"));
				  }
				  else
				  {
					  AfxMessageBox(_T("保存失败！"));
				  }*/

				}
				
			}
		}
	}


	CDialog::OnOK();
}
bool CSetConstValueDlg::StrIsNum( std::wstring StrCheck )
{
	CString Str = StrCheck.c_str();
	if (Str == _T(""))
	{
		return false;
	}
	if (Str[0] == '-')
	{
		Str = Str.Mid(1,Str.GetLength());
	}

	CString StrNum = _T(".0123456789");

	int iStrLen = Str.GetLength();
	int iPointCount = 0;
	for (int i=0;i<iStrLen;++i)
	{
		CString wch(Str[i]);
		if (StrNum.FindOneOf(wch)<0)
		{
			return false;
		}

		if (wch == _T("."))
		{
			iPointCount++;
		}
	}
	if (Str[0] == '.'||Str[Str.GetLength()-1] == '.')
	{
		return false;
	}

	if (iPointCount >1)
	{
		return false;
	}

	return true;
}

void CSetConstValueDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL CSetConstValueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//if (m_sValueType != _T("strconst"))
	{
		UpdateAllPointList();
	}

	m_strParamSetting = m_strSetValue;
	UpdateData(FALSE);


	m_ComPointType.ResetContent();

	if(m_bCanModifyType)
	{
		int ncur = m_ComPointType.AddString(_T("const"));
		m_ComPointType.SetItemData(ncur,1);

		//ncur = m_ComPointType.AddString(_T("strconst"));
		//m_ComPointType.SetItemData(ncur,2);

		ncur = m_ComPointType.AddString(_T("point"));
		m_ComPointType.SetItemData(ncur,3);
	}
	else
	{
		if (m_sValueType == _T("strconst") || m_sValueType == _T("const"))
		{
			int ncur = m_ComPointType.AddString(_T("const"));
			m_ComPointType.SetItemData(ncur,1);
		}
		else
		{
			int ncur = m_ComPointType.AddString(_T("point"));
			m_ComPointType.SetCurSel(ncur);
			m_ComPointType.SetItemData(ncur,3);
		}
		m_ComPointType.EnableWindow(FALSE);
	}



	if (m_sValueType == _T("strconst"))
	{
		m_ComPointType.SetCurSel(0);
	}else if (m_sValueType == _T("const"))
	{
		m_ComPointType.SetCurSel(0);
	}else
	{
		m_ComPointType.SetCurSel(1);
	}

	return TRUE;  
}

bool CSetConstValueDlg::UpdateAllPointList()
{
	if (vecPointList.empty())
	{
		return false;
	}


	return true;
}




void CSetConstValueDlg::UpdateListPoint()
{
	if (vecSearchPointList.empty())
	{
		return;
	}


	return;
}


//void CSetConstValueDlg::OnEnKillfocusEditSerch()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	SearchListBySearch();
//	UpdateListPoint();
//}


void CSetConstValueDlg::OnEnChangeEditSerch()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//if (m_sValueType == _T("strconst"))
	//{
	//	return;
	//}


}

void CSetConstValueDlg::UpdateListPoint_Con()
{
	if (vecConSearchPointList.empty())
	{
		return;
	}

	return;
}



//void CSetConstValueDlg::OnEnKillfocusEditConserch()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	//SearchListBySearch_Con();
//	//UpdateListPoint_Con();
//}



void CSetConstValueDlg::OnEnChangeEditstrategyname()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}




void CSetConstValueDlg::OnBnClickedButtonChoosePoint()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	CDataComConfigDlg dlg(pFrame->m_plcpointmap);
	if(dlg.DoModal()==IDOK)
	{
		if(m_strParamSetting.GetLength()==0)
		{
			m_strParamSetting = dlg.m_strPointName;
		}
		else if(m_strParamSetting.Find(_T("<%"))>=0 || m_strParamSetting.Find(_T("if"))>=0 || m_strParamSetting.Find(_T(" "))>=0)
		{
			m_strParamSetting += _T("<%");
			m_strParamSetting += dlg.m_strPointName;
			m_strParamSetting += _T("%>");
		}
		else
		{

			m_strParamSetting += dlg.m_strPointName;
		}

		UpdateData(FALSE);
	}
}


void CSetConstValueDlg::OnBnClickedButtonTestScript()
{
	// TODO: Add your control notification handler code here


	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	int nId = -1;
	CString strSrc;
	CString strResult;
	CHttpOperation http(pFrame->m_strCalcIp, pFrame->m_nCalcPort, _T("/tool/evalStringExpression"));
	wstring wstrTimeStart,wstrTimeEnd ;

	Json::Value myRoot;


	myRoot["str"] = Project::Tools::WideCharToUtf8(m_strParamSetting.GetString());

	string szBuf = myRoot.toStyledString();

	bool bRet = http.HttpPostGetRequestEx(0, szBuf.data(), strResult,  _T("Content-Type: application/json"));//
	if(bRet)
	{
		Json::Reader jsReader;
		string strResult_utf8;
		Project::Tools::WideCharToUtf8(strResult.GetString(), strResult_utf8);
		Json::Value myReturnRoot;
		if (!jsReader.parse(strResult_utf8, myReturnRoot))
		{
			AfxMessageBox(_T("计算失败"));
			return;
		}
		else
		{
			if(myReturnRoot.isObject())
			{
				Json::Value dataObj = myReturnRoot["err"];
				if(dataObj.isInt())
				{
					int nErr = dataObj.asInt();
					if(nErr==1)
					{
						CString strTemp;
						strTemp.Format(_T("公式格式有误，计算失败"));
						AfxMessageBox(strTemp);
					}
					else
					{
						Json::Value dataObj = myReturnRoot["data"];
						if(dataObj.isDouble())
						{
							CString strTemp;
							strTemp.Format(_T("结果为:%.4f"), dataObj.asDouble());
							AfxMessageBox(strTemp);
						}
						else if (dataObj.isInt())
						{
							CString strTemp;
							strTemp.Format(_T("结果为:%d"), dataObj.asInt());
							AfxMessageBox(strTemp);
						}else if (dataObj.isString())
						{
							CString strTemp;
							wstring wstr = Project::Tools::AnsiToWideChar(dataObj.asString().data());
							strTemp.Format(_T("结果为:%s"), wstr.c_str());
							AfxMessageBox(strTemp);
						}
					}

				}
				else{
					CString strTemp;
					strTemp.Format(_T("后台有误，计算失败"));
					AfxMessageBox(strTemp);
				}

			}
		}
	}
	else
	{
		AfxMessageBox(_T("公式计算失败"));
		return;
	}
}

void CSetConstValueDlg::ReloadNameAndIndexForPoint()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	

	std::vector<CString> allPointDescr;
	std::vector<int>	 allPointIndex;
	vector<DataPointEntry> pointmanager = pMainFrame->m_plcpointmap.GetPointList();
	const size_t size = pointmanager.size();
	for(size_t i=0;i<size;++i)
	{
		wstring wstrName = pointmanager[i].GetShortName();
		CString strRemarkName = wstrName.c_str();
		allPointDescr.push_back(strRemarkName);
		int nIndex = pointmanager[i].GetPointIndex();
		allPointIndex.push_back(nIndex);
	}
	vecPointList = allPointDescr;
	vecPointIndex = allPointIndex;
}
