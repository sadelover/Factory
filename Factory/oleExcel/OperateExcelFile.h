#pragma once

//OLE的头文件
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CApplication.h"
#include "DataPoint/DataPointManager.h"

///
///用于OLE的方式的EXCEL读写，
class COperateExcelFile
{
public:
	static COperateExcelFile* Instance(void);
	static void Destroy(void);
	void SaveToExcel(const CString& strFilePath, CDataPointManager& pointmap);
	void ReadFromExcel(const CString& strFilePath, CDataPointManager& pointmap);
	bool ReadSampleDataFromExcel(CString strSamplePath,vector<vector<CString>>&vecSampleList,vector<vector<CString>>&vecInputList,vector<vector<CString>>&vecOutputList);
private:
	//构造函数和析构函数
	COperateExcelFile(void);
	virtual ~COperateExcelFile(void);
	static COperateExcelFile* s_pInstance;

protected:
	///打开的EXCEL文件名称
	CString       open_excel_file_;

	///EXCEL BOOK集合，（多个文件时）
	CWorkbooks    excel_books_; 
	///当前使用的BOOK，当前处理的文件
	CWorkbook     excel_work_book_; 
	///EXCEL的sheets集合
	CWorksheets   excel_sheets_; 
	///当前使用sheet
	CWorksheet    excel_work_sheet_; 
	///当前的操作区域
	CRange        excel_current_range_; 


	///是否已经预加载了某个sheet的数据
	BOOL          already_preload_;
	///Create the SAFEARRAY from the VARIANT ret.
	COleSafeArray ole_safe_array_;

protected:
	///EXCEL的进程实例
	static CApplication excel_application_;

private:
	///
	void ShowInExcel(BOOL bShow);

	///检查一个CELL是否是字符串
	BOOL    IsCellString(long iRow, long iColumn);
	///检查一个CELL是否是数值
	BOOL    IsCellInt(long iRow, long iColumn);

	///得到一个CELL的String
	CString GetCellString(long iRow, long iColumn);
	///得到整数
	int     GetCellInt(long iRow, long iColumn);
	///得到double的数据
	double  GetCellDouble(long iRow, long iColumn);

	///取得行的总数
	int GetRowCount(void);
	///取得列的总数
	int GetColumnCount(void);

	///通过id使用某个sheet
	BOOL LoadSheet(long table_index,BOOL pre_load = FALSE);
	///通过名称使用某个sheet，
	BOOL LoadSheet(LPCTSTR sheet,BOOL pre_load = FALSE);
	///通过序号取得某个Sheet的名称
	CString GetSheetName(long table_index);

	///得到Sheet的总数
	int GetSheetCount(void);

	///打开文件
	BOOL OpenExcelFile(LPCTSTR file_name, BOOL bSave);
	///关闭打开的Excel 文件，有时候打开EXCEL文件就要
	void CloseExcelFile(BOOL if_save = FALSE);
	//另存为一个EXCEL文件
	void SaveasXSLFile(const CString &xls_file);
	///取得打开文件的名称
	CString GetOpenFileName(void);
	///取得打开sheet的名称
	CString GetLoadSheetName(void);

	///写入一个CELL一个int
	void SetCellInt(long irow, long icolumn,int new_int);
	///写入一个CELL一个string
	void SetCellString(long irow, long icolumn,CString new_string);

	//
	void SetCellStringTitle(const long iRow, VARIANT vtData);
	void SetCellStringData(const long iRow, VARIANT vtData);

private:
	///初始化EXCEL OLE
	static BOOL InitExcel(void);
	///释放EXCEL的 OLE
	static void ReleaseExcel(void);
	///取得列的名称，比如27->AA
	static char *GetColumnName(long iColumn);

	COleSafeArray GetMatrixValueTitle(wchar_t **pMatrix, const int nRow, const int nCol);
	COleSafeArray GetMatrixValueData(vector<vector<wstring>> vecData, const int nRow, const int nCol);


protected:
	//预先加载
	void PreLoadSheet(void);
};