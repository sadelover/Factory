#include "StdAfx.h"
#include "OperateExcelFile.h"
#include "Tools/CustomTools/CustomTools.h"
#include "afxwin.h"
#include "..\ImportTipsDlg.h"

COperateExcelFile* COperateExcelFile::s_pInstance = NULL;

//
COleVariant
	covTrue((short)TRUE),
	covFalse((short)FALSE),
	covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);    

//
CApplication COperateExcelFile::excel_application_;


COperateExcelFile::COperateExcelFile(void) : already_preload_(FALSE)
{
}

COperateExcelFile::~COperateExcelFile(void)
{
	CloseExcelFile();
}

COperateExcelFile* COperateExcelFile::Instance(void)
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new COperateExcelFile();
		ASSERT(NULL != s_pInstance);
	}
	return s_pInstance;
}

void COperateExcelFile::Destroy(void)
{
	if (NULL != s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = NULL;
	}	
}

//初始化EXCEL文件，
BOOL COperateExcelFile::InitExcel(void)
{
	CoUninitialize();
	if (S_FALSE == CoInitialize(NULL)) 
	{
		//AfxMessageBox(_T("初始化COM支持库失败！")); 
		//return FALSE; 
	}
	//创建Excel 2000服务器(启动Excel) 
	if (!excel_application_.CreateDispatch(_T("Excel.Application"),NULL)) 
	{ 
		AfxMessageBox(_T("创建Excel服务失败,你可能没有安装EXCEL，请检查!")); 
		return FALSE;
	}

	excel_application_.put_DisplayAlerts(FALSE); 
	return TRUE;
}

//
void COperateExcelFile::ReleaseExcel(void)
{
	excel_application_.Quit();
	excel_application_.ReleaseDispatch();
	excel_application_=NULL;
}

//打开excel文件
BOOL COperateExcelFile::OpenExcelFile(LPCTSTR file_name, BOOL bSave)
{
	//先关闭
	CloseExcelFile();

	//利用模板文件建立新文档 
	excel_books_.AttachDispatch(excel_application_.get_Workbooks(),true); 

	LPDISPATCH lpDis = NULL;
	if (bSave)
	{	// save to new file
		lpDis = excel_books_.Add(vtMissing);
	}
	else
	{	// open selected file
		lpDis = excel_books_.Add(COleVariant(file_name)); 
	}
	if (lpDis)
	{
		excel_work_book_.AttachDispatch(lpDis); 
		//得到Worksheets 
		excel_sheets_.AttachDispatch(excel_work_book_.get_Worksheets(),true); 

		//记录打开的文件名称
		open_excel_file_ = file_name;

		return TRUE;
	}

	return FALSE;
}

//关闭打开的Excel 文件,默认情况不保存文件
void COperateExcelFile::CloseExcelFile(BOOL if_save)
{
	//如果已经打开，关闭文件
	if (!open_excel_file_.IsEmpty())
	{
		//如果保存,交给用户控制,让用户自己存，如果自己SAVE，会出现莫名的等待
		if (if_save)
		{
			ShowInExcel(TRUE);
		}
		else
		{
			//
			excel_work_book_.Close(COleVariant(short(FALSE)),COleVariant(open_excel_file_),covOptional);
			excel_books_.Close();
		}

		//打开文件的名称清空
		open_excel_file_.Empty();
	}



	excel_sheets_.ReleaseDispatch();
	excel_work_sheet_.ReleaseDispatch();
	excel_current_range_.ReleaseDispatch();
	excel_work_book_.ReleaseDispatch();
	excel_books_.ReleaseDispatch();
}

void COperateExcelFile::SaveasXSLFile(const CString &xls_file)
{
	excel_work_book_.SaveAs(COleVariant(xls_file),
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		0,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional);
	return;
}

int COperateExcelFile::GetSheetCount(void)
{
	return excel_sheets_.get_Count();
}


CString COperateExcelFile::GetSheetName(long table_index)
{
	CWorksheet sheet;
	sheet.AttachDispatch(excel_sheets_.get_Item(COleVariant((long)table_index)),true);
	CString name = sheet.get_Name();
	sheet.ReleaseDispatch();
	return name;
}

//按照序号加载Sheet表格,可以提前加载所有的表格内部数据
BOOL COperateExcelFile::LoadSheet(long table_index,BOOL pre_load)
{
	LPDISPATCH lpDis = NULL;
	excel_current_range_.ReleaseDispatch();
	excel_work_sheet_.ReleaseDispatch();
	lpDis = excel_sheets_.get_Item(COleVariant((long)table_index));
	if (lpDis)
	{
		excel_work_sheet_.AttachDispatch(lpDis,true);
		excel_current_range_.AttachDispatch(excel_work_sheet_.get_Cells(), true);
	}
	else
	{
		return FALSE;
	}

	already_preload_ = FALSE;
	//如果进行预先加载
	if (pre_load)
	{
		PreLoadSheet();
		already_preload_ = TRUE;
	}

	return TRUE;
}

//按照名称加载Sheet表格,可以提前加载所有的表格内部数据
BOOL COperateExcelFile::LoadSheet(LPCTSTR sheet,BOOL pre_load)
{
	LPDISPATCH lpDis = NULL;
	excel_current_range_.ReleaseDispatch();
	excel_work_sheet_.ReleaseDispatch();
	lpDis = excel_sheets_.get_Item(COleVariant(sheet));
	if (lpDis)
	{
		excel_work_sheet_.AttachDispatch(lpDis,true);
		excel_current_range_.AttachDispatch(excel_work_sheet_.get_Cells(), true);

	}
	else
	{
		return FALSE;
	}
	//
	already_preload_ = FALSE;
	//如果进行预先加载
	if (pre_load)
	{
		already_preload_ = TRUE;
		PreLoadSheet();
	}

	return TRUE;
}

//得到列的总数
int COperateExcelFile::GetColumnCount(void)
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(excel_work_sheet_.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Columns(), true);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

//得到行的总数
int COperateExcelFile::GetRowCount(void)
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(excel_work_sheet_.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Rows(), true);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

//检查一个CELL是否是字符串
BOOL COperateExcelFile::IsCellString(long irow, long icolumn)
{
	CRange range;
	range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
	COleVariant vResult =range.get_Value2();
	//VT_BSTR标示字符串
	if(vResult.vt == VT_BSTR)       
	{
		return TRUE;
	}
	return FALSE;
}

//检查一个CELL是否是数值
BOOL COperateExcelFile::IsCellInt(long irow, long icolumn)
{
	CRange range;
	range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
	COleVariant vResult =range.get_Value2();
	//好像一般都是VT_R8
	if(vResult.vt == VT_INT || vResult.vt == VT_R8)       
	{
		return TRUE;
	}
	return FALSE;
}

//
CString COperateExcelFile::GetCellString(long irow, long icolumn)
{
	COleVariant vResult ;
	CString str;
	//字符串
	if (!already_preload_)
	{
		CRange range;
		range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
		vResult =range.get_Value2();
		range.ReleaseDispatch();
	}
	//如果数据依据预先加载了
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = irow;
		read_address[1] = icolumn;
		ole_safe_array_.GetElement(read_address, &val);
		vResult = val;
	}

	if(vResult.vt == VT_BSTR)
	{
		str=vResult.bstrVal;
	}
	//整数
	else if (vResult.vt==VT_INT)
	{
		str.Format(_T("%d"),vResult.pintVal);
	}
	//8字节的数字 
	else if (vResult.vt==VT_R8)     
	{
		str.Format(_T("%0.6f"),vResult.dblVal);

		
	}
	//时间格式
	else if(vResult.vt==VT_DATE)    
	{/*
	 SYSTEMTIME st;
	 VariantTimeToSystemTime(vResult.date, &st);
	 CTime tm(st); 
	 str=tm.Format("%Y-%m-%d");*/
		
		SYSTEMTIME st; 
		VariantTimeToSystemTime(vResult.date, &st);

		CString str;
		int year=st.wYear;
		int month = st.wMonth;
		int day = st.wDay;
		int hour = st.wHour;
		int minute = st.wMinute;
		int second = st.wSecond;
		int millisecond = st.wSecond;
		str.Format(_T("%d-%d-%d %d:%d:%d"),year,month,
			day,hour,minute,second,millisecond);


	}
	//单元格空的
	else if(vResult.vt==VT_EMPTY)   
	{
		str="";
	}  

	return str;
}

double COperateExcelFile::GetCellDouble(long irow, long icolumn)
{
	double rtn_value = 0;
	COleVariant vresult;
	//字符串
	if (!already_preload_)
	{
		CRange range;
		range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
		vresult =range.get_Value2();
		range.ReleaseDispatch();
	}
	//如果数据依据预先加载了
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = irow;
		read_address[1] = icolumn;
		ole_safe_array_.GetElement(read_address, &val);
		vresult = val;
	}

	if (vresult.vt==VT_R8)     
	{
		rtn_value = vresult.dblVal;
	}

	return rtn_value;
}

//VT_R8
int COperateExcelFile::GetCellInt(long irow, long icolumn)
{
	int num;
	COleVariant vresult;

	if (!already_preload_)
	{
		CRange range;
		range.AttachDispatch(excel_current_range_.get_Item (COleVariant((long)irow),COleVariant((long)icolumn)).pdispVal, true);
		vresult = range.get_Value2();
		range.ReleaseDispatch();
	}
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = irow;
		read_address[1] = icolumn;
		ole_safe_array_.GetElement(read_address, &val);
		vresult = val;
	}
	//
	num = static_cast<int>(vresult.dblVal);

	return num;
}

void COperateExcelFile::SetCellString(long irow, long icolumn,CString new_string)
{
	COleVariant new_value(new_string);
	CRange start_range = excel_work_sheet_.get_Range(COleVariant(_T("A1")),covOptional);
	CRange write_range = start_range.get_Offset(COleVariant((long)irow -1),COleVariant((long)icolumn -1) );
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();

}

void COperateExcelFile::SetCellInt(long irow, long icolumn,int new_int)
{
	COleVariant new_value((long)new_int);

	CRange start_range = excel_work_sheet_.get_Range(COleVariant(_T("A1")),covOptional);
	CRange write_range = start_range.get_Offset(COleVariant((long)irow -1),COleVariant((long)icolumn -1) );
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();
}


//
void COperateExcelFile::ShowInExcel(BOOL bShow)
{
	excel_application_.put_Visible(bShow);
	excel_application_.put_UserControl(bShow);
}

//返回打开的EXCEL文件名称
CString COperateExcelFile::GetOpenFileName(void)
{
	return open_excel_file_;
}

//取得打开sheet的名称
CString COperateExcelFile::GetLoadSheetName(void)
{
	return excel_work_sheet_.get_Name();
}

//取得列的名称，比如27->AA
char *COperateExcelFile::GetColumnName(long icolumn)
{   
	static char column_name[64];
	size_t str_len = 0;

	while(icolumn > 0)
	{
		int num_data = icolumn % 26;
		icolumn /= 26;
		if (num_data == 0)
		{
			num_data = 26;
			icolumn--;
		}
		column_name[str_len] = (char)((num_data-1) + 'A' );
		str_len ++;
	}
	column_name[str_len] = '\0';
	//反转
	_strrev(column_name);

	return column_name;
}

//预先加载
void COperateExcelFile::PreLoadSheet(void)
{
	CRange used_range;

	used_range = excel_work_sheet_.get_UsedRange();	


	VARIANT ret_ary = used_range.get_Value2();
	if (!(ret_ary.vt & VT_ARRAY))
	{
		return;
	}
	//
	ole_safe_array_.Clear();
	ole_safe_array_.Attach(ret_ary); 
}

void COperateExcelFile::SetCellStringTitle(const long iRow, VARIANT vtData)
{
	COleVariant new_value(vtData);
	CRange start_range = excel_work_sheet_.get_Range(COleVariant(_T("A1")), COleVariant(_T("Y1")));
	CRange write_range = start_range.get_Offset(COleVariant((long)iRow -1), COleVariant((long)0) );
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();
}

void COperateExcelFile::SetCellStringData(const long iRow, VARIANT vtData)
{
	CString strRowNum;
	strRowNum.Format(_T("Y%d"), (int)iRow+1);
	COleVariant new_value(vtData);
	CRange start_range = excel_work_sheet_.get_Range(COleVariant(_T("A2")), COleVariant(strRowNum));
	CRange write_range = start_range.get_Offset(COleVariant((long)0), COleVariant((long)0) );
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();
}

COleSafeArray COperateExcelFile::GetMatrixValueTitle(wchar_t **pMatrix, const int nRow, const int nCol)
{
	COleSafeArray saRet;

	ASSERT(NULL != *pMatrix);
	if (NULL == *pMatrix)
	{
		return saRet;
	}

	DWORD numElements[2] = {nRow, nCol};

	// Create the 2 dimensional safe-array of type VT_BSTR with size
	saRet.Create(VT_BSTR, 2, numElements);

	// Initialize safearray  with values
	long index[2] = {0, 0};
	for (int i=0; i<nRow; i++)
	{
		index[0] = i;
		for (int j=0; j<nCol; j++)
		{
			index[1] = j;

			BSTR bStr = _bstr_t(*(pMatrix + i*nCol + j));
			saRet.PutElement(index, bStr);
		}
	}

	return saRet;
}

COleSafeArray COperateExcelFile::GetMatrixValueData(vector<vector<wstring>> vecData, const int nRow, const int nCol)
{
	vector<vector<wstring>>::const_iterator iterWhole;
	vector<wstring>::const_iterator iterPoint;
	COleSafeArray saRet;
	DWORD numElements[2] = {nRow, nCol};
	saRet.Create(VT_BSTR, 2, numElements);
	long index[2] = {0, 0};
	int i = 0;
	int j = 0;
	for (iterWhole=vecData.begin(); iterWhole!=vecData.end(); iterWhole++,i++)
	{
		index[0] = i;
		j = 0;
		for (iterPoint=iterWhole->begin(); iterPoint!=iterWhole->end(); iterPoint++,j++)
		{
			index[1] = j;
			BSTR bStr = _bstr_t((*iterPoint).c_str());
			saRet.PutElement(index, bStr);
		}
	}

	return saRet;
}

void COperateExcelFile::SaveToExcel(const CString& strFilePath, CDataPointManager& pointmap)
{
	if (!InitExcel())
	{
		return;
	}

	if (!OpenExcelFile(strFilePath, TRUE))
	{
		MessageBox(NULL, L"新建Excel文件失败！", L"错误", MB_OK|MB_ICONERROR);
		return;
	}

	if (!LoadSheet(L"Sheet1", TRUE))
	{
		MessageBox(NULL, L"新建Sheet失败！", L"错误", MB_OK|MB_ICONERROR);
		return;
	}

	wchar_t* szTitle[1][25] = {
		L"pointindex", L"physicalid", L"source", L"remark", L"Unit",
		L"RWProperty", L"param1", L"param2", L"param3", L"param4",
		L"param5", L"param6", L"param7", L"param8", L"param9",
		L"param10", L"param11", L"param12", L"param13", L"param14",
		L"storecycle", L"customName", L"system", L"device", L"type"};
	COleSafeArray arrMatrixTitle = GetMatrixValueTitle((wchar_t**)szTitle, 1, 25);
	SetCellStringTitle(1, arrMatrixTitle);

	map<int, DataPointEntry>& mapPoint = pointmap.GetPointMap();
	map<int, DataPointEntry>::const_iterator iter;
	const CString cFlag(L"'");
	int nRow = 0;
	vector<wstring> vecPoint;
	vector<vector<wstring>> vecWhole;
	for (iter=mapPoint.begin(); iter!=mapPoint.end(); iter++,nRow++)
	{
		const DataPointEntry& entry = iter->second;
		CString strIndex;
		strIndex.Format(L"%d",entry.GetPointIndex());
		CString strPhysicalID(entry.GetShortName().c_str());
		CString strCource(entry.GetSourceType().c_str());
		CString strRemark(entry.GetDescription().c_str());
		CString strUnit(entry.GetUnit().c_str());
		CString strProperty;
		CString strStoreCycle;
		strStoreCycle.Format(L"%d",(int)entry.GetStoreCycle());
		if(entry.GetProperty() == PLC_READ)
		{
			strProperty = L"R";
		}
		else if(entry.GetProperty() == PLC_WRITE)
		{
			strProperty = L"W";
		}
		else if(entry.GetProperty() == PLC_PROP_MAX)
		{
			strProperty = L"R/W";
		}
		CString strParam1(entry.GetParam(1).c_str());
		CString strParam2(entry.GetParam(2).c_str());
		CString strParam3(entry.GetParam(3).c_str());
		CString strParam4(entry.GetParam(4).c_str());
		CString strParam5(entry.GetParam(5).c_str());
		CString strParam6(entry.GetParam(6).c_str());
		CString strParam7(entry.GetParam(7).c_str());
		CString strParam8(entry.GetParam(8).c_str());
		CString strParam9(entry.GetParam(9).c_str());
		CString strParam10(entry.GetParam(10).c_str());
		CString strParam11(entry.GetParam(11).c_str());
		CString strParam12(entry.GetParam(12).c_str());
		CString strParam13(entry.GetParam(13).c_str());
		CString strParam14(entry.GetParam(14).c_str());
		CString strParam15(entry.GetParam(15).c_str());
		CString strParam16(entry.GetParam(16).c_str());
		CString strParam17(entry.GetParam(17).c_str());
		CString strParam18(entry.GetParam(18).c_str());
		if (cFlag == strIndex
			|| cFlag == strPhysicalID
			|| cFlag == strCource
			|| cFlag == strRemark
			|| cFlag == strUnit
			|| cFlag == strProperty
			|| cFlag == strParam1
			|| cFlag == strParam2
			|| cFlag == strParam3
			|| cFlag == strParam4
			|| cFlag == strParam5
			|| cFlag == strParam6
			|| cFlag == strParam7
			|| cFlag == strParam8
			|| cFlag == strParam9
			|| cFlag == strParam10
			|| cFlag == strParam11
			|| cFlag == strParam12
			|| cFlag == strParam13
			|| cFlag == strParam14
			|| cFlag == strStoreCycle
			|| cFlag == strParam15
			|| cFlag == strParam16
			|| cFlag == strParam17
			|| cFlag == strParam18)
		{
			CString strErr;
			CString strNum;
			int nPos = strIndex.Find(L".");
			if (nPos != -1) {
				strNum = strIndex.Left(nPos);
			}
			strErr.Format(L"序号%s，\r\n点%s，\r\n参数中包含单引号，导出中止！", strNum, strPhysicalID);
			AfxMessageBox(strErr);
			return;
		}

		vecPoint.clear();
		wchar_t szBuf[MAX_PATH] = {0};
		_itow_s(entry.GetPointIndex(), szBuf, 10);
		vecPoint.push_back(szBuf);

		vecPoint.push_back(entry.GetShortName());
		vecPoint.push_back(entry.GetSourceType().c_str());
		vecPoint.push_back(entry.GetDescription().c_str());
		vecPoint.push_back(entry.GetUnit().c_str());

		wchar_t* pProperty = strProperty.GetBuffer();
		vecPoint.push_back(pProperty);
		strProperty.ReleaseBuffer();

		vecPoint.push_back(entry.GetParam(1).c_str());
		vecPoint.push_back(entry.GetParam(2).c_str());
		vecPoint.push_back(entry.GetParam(3).c_str());
		vecPoint.push_back(entry.GetParam(4).c_str());
		vecPoint.push_back(entry.GetParam(5).c_str());
		vecPoint.push_back(entry.GetParam(6).c_str());
		vecPoint.push_back(entry.GetParam(7).c_str());
		vecPoint.push_back(entry.GetParam(8).c_str());
		vecPoint.push_back(entry.GetParam(9).c_str());
		vecPoint.push_back(entry.GetParam(10).c_str());
		vecPoint.push_back(entry.GetParam(11).c_str());
		vecPoint.push_back(entry.GetParam(12).c_str());
		vecPoint.push_back(entry.GetParam(13).c_str());
		vecPoint.push_back(entry.GetParam(14).c_str());

		memset(szBuf, 0, MAX_PATH);
		_itow_s(entry.GetStoreCycle(), szBuf, 10);
		vecPoint.push_back(szBuf);

		vecPoint.push_back(entry.GetParam(15).c_str());
		vecPoint.push_back(entry.GetParam(16).c_str());
		vecPoint.push_back(entry.GetParam(17).c_str());
		vecPoint.push_back(entry.GetParam(18).c_str());

		vecWhole.push_back(vecPoint);
	}

	COleSafeArray arrMatrixData = GetMatrixValueData(vecWhole, nRow, 25);
	SetCellStringData(nRow, arrMatrixData);

	SaveasXSLFile(strFilePath);
	CloseExcelFile(FALSE);
	ReleaseExcel();
	Destroy();
	//AfxMessageBox(L"导出表成功!");
}

void COperateExcelFile::ReadFromExcel(const CString& strFilePath, CDataPointManager& pointmap)
{
	if (!InitExcel())
	{
		return;
	}

	if (!OpenExcelFile(strFilePath, FALSE))
	{
		MessageBox(NULL, L"打开Excel文件失败！", L"错误", MB_OK|MB_ICONERROR);
		return;
	}

	if (!LoadSheet((long)1, TRUE))
	{
		MessageBox(NULL, L"打开Sheet失败！", L"错误", MB_OK|MB_ICONERROR);
		return;
	}

	int nRow = GetRowCount();
	int nCol = GetColumnCount();
	map<CString, int>map_PointName;//重复点名列表查询使用
	pair<map<CString, int>::iterator, bool>rec;//重复点名列表查询使用
	vector<CString> cstrPointRepeatList;//重复的点名
	vector<CString> errPointNameList;//不规范的点名
	for (int i=2; i<=nRow; i++)
	{
		DataPointEntry entry;
		CString strIndex = GetCellString(i, 1);
		CString strPhysicalID = GetCellString(i, 2);
		if(strPhysicalID.GetLength()<=0)
		{
			continue;
		}

		if(!Project::Tools::IsPointNameValid(strPhysicalID))
		{
			errPointNameList.push_back(strPhysicalID);
			continue;
		}

		//添加失败即为重复
		rec = map_PointName.insert(pair<CString, int>(strPhysicalID,0));
		if( rec.second == false)
		{
			cstrPointRepeatList.push_back(strPhysicalID);
			continue;
		}

		CString strSource = GetCellString(i, 3);
		CString strRemark = GetCellString(i, 4);
		CString strUnit = GetCellString(i, 5);
		CString strRWProperty = GetCellString(i, 6);
		CString strparam1 = GetCellString(i, 7);
		CString strparam2 = GetCellString(i, 8);
		CString strparam3 = GetCellString(i, 9);
		CString strparam4 = GetCellString(i, 10);
		CString strparam5 = GetCellString(i, 11);
		CString strparam6 = GetCellString(i, 12);
		CString strparam7 = GetCellString(i, 13);
		CString strparam8 = GetCellString(i, 14);
		CString strparam9 = GetCellString(i, 15);
		CString strparam10 = GetCellString(i, 16);
		CString strparam11 = GetCellString(i, 17);
		CString strparam12 = GetCellString(i, 18);
		CString strparam13 = GetCellString(i, 19);
		CString strparam14 = GetCellString(i, 20);
		CString strStoreCycle = GetCellString(i, 21);
		CString strparam15;
		CString strparam16;
		CString strparam17;
		CString strparam18;
		//2020-02-02
		if(strStoreCycle == L"")//存储周期不得为空
			strStoreCycle = L"2.0";
		if (nCol > 21)
		{	// compatible old version
			strparam15 = GetCellString(i, 22);
			strparam16 = GetCellString(i, 23);
			strparam17 = GetCellString(i, 24);
			strparam18 = GetCellString(i, 25);
		}

		//int nIndex = _wtoi(strIndex);
		entry.SetPointIndex(i-1); //golding add 20180509 兼容用户的Index有重复的



		entry.SetShortName(strPhysicalID.GetString());


		entry.SetSourceType(strSource.GetString());

		entry.SetDescription(strRemark.GetString());

		entry.SetUnit(strUnit.GetString());

		if(strRWProperty == L"R")
		{
			entry.SetProperty(PLC_READ);
		}
		else if(strRWProperty == L"W")
		{
			entry.SetProperty(PLC_WRITE);
		}
		else if(strRWProperty == L"R/W")
		{
			entry.SetProperty(PLC_PROP_MAX);
		}
		else
		{
			entry.SetProperty(PLC_READ);
		}
		entry.SetParam(1, strparam1.GetString());
		entry.SetParam(2, strparam2.GetString());
		entry.SetParam(3, strparam3.GetString());
		entry.SetParam(4, strparam4.GetString());
		entry.SetParam(5, strparam5.GetString());
		entry.SetParam(6, strparam6.GetString());
		entry.SetParam(7, strparam7.GetString());
		entry.SetParam(8, strparam8.GetString());
		entry.SetParam(9, strparam9.GetString());
		entry.SetParam(10, strparam10.GetString());
		entry.SetParam(11, strparam11.GetString());
		entry.SetParam(12, strparam12.GetString());
		entry.SetParam(13, strparam13.GetString());
		entry.SetParam(14, strparam14.GetString());
		entry.SetStoreCycle((POINT_STORE_CYCLE)_wtoi(strStoreCycle));
		entry.SetParam(15, strparam15.GetString());
		entry.SetParam(16, strparam16.GetString());
		entry.SetParam(17, strparam17.GetString());
		entry.SetParam(18, strparam18.GetString());
		entry.m_bNeedSave = true;
		if(entry.GetShortName().size() != 0)
		{
			pointmap.AddEntry(entry);
		}
	}

	CloseExcelFile(FALSE);
	ReleaseExcel();
	Destroy();

	

	
	if((errPointNameList.size()==0) && (cstrPointRepeatList.size() == 0))
	{
		/*AfxMessageBox(L"导入点表文件成功!");*/
	}
	else
	{
		pointmap.Clear();
		CString strAll = _T("点名不合法: ");
		for(int i=0;i<errPointNameList.size();i++)
		{
			strAll+= errPointNameList[i];
			strAll+=_T("\r\n");
		}
		strAll += _T("\r\n\r\n如下点名重复:\r\n ");
		for(int i=0;i<cstrPointRepeatList.size();i++)
		{
			strAll+= cstrPointRepeatList[i];
			strAll+=_T("\r\n");
		}
		CImportTipsDlg dlg;
		dlg.m_strShow = strAll;
		dlg.DoModal();
	}
}
bool COperateExcelFile::ReadSampleDataFromExcel(CString strSamplePath,vector<vector<CString>>&vecSampleList,vector<vector<CString>>&vecInputList,vector<vector<CString>>&vecOutputList)
{
	if (!InitExcel())
	{
		return false;
	}

	if (!OpenExcelFile(strSamplePath, FALSE))
	{
		MessageBox(NULL, L"打开Excel文件失败！", L"错误", MB_OK|MB_ICONERROR);
		return false;
	}

	if (!LoadSheet((long)1, TRUE))
	{
		MessageBox(NULL, L"打开Sheet失败！", L"错误", MB_OK|MB_ICONERROR);
		return false;
	}
	int nRow = GetRowCount();
	int nCol = GetColumnCount();
	vecSampleList.resize(nRow-1);
	vecInputList.resize(nRow-1);
	vecOutputList.resize(nRow-1);
	for (int i=2;i<=nRow;i++)
	{
		//初始化id，开始时间，结束时间
		for (int j=1;j<=3;j++)
		{
		    

			//vecSampleList[i-2].push_back(strParam);
			
				CString strTime = GetCellString(i, j);
				
				vecSampleList[i-2].push_back(strTime);
			
		}
		//初始化input数组
		for (int j=4;j<=4;j++)
		{
			CString str =GetCellString(1,j);
		
			int bfind =str.Find(_T("input"));
			if (bfind==-1)
			{
				AfxMessageBox(_T("此文件非样本文件!"));
				return false;
				
			}
			/*else
			{
                
			}*/
		}
		for (int j=4;j<=nCol;j++)
		{
			CString str =GetCellString(1,j);
		
			int bfind =str.Find(_T("input"));
			if (bfind!=-1)
			{
				CString strInputParam = GetCellString(i,j);
				vecInputList[i-2].push_back(strInputParam);
			}
			/*else
			{
                 AfxMessageBox(_T("此文件非样本文件!"));
				 return false;
			}*/
		}
		//初始化output数组
		for (int j=vecInputList[i-2].size()+vecSampleList[i-2].size();j<=nCol;j++)
		{
			CString str =GetCellString(1,j);
			int bfind =str.Find(_T("output"));
			if (bfind!=-1)
			{
				CString strOutputParam = GetCellString(i,j);
				vecOutputList[i-2].push_back(strOutputParam);
			}
		}
		
		
	}

	CloseExcelFile(FALSE);
	ReleaseExcel();
	Destroy();
	AfxMessageBox(L"导入样本文件成功!");
	return true;
		
}
