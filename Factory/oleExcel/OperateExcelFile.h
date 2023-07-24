#pragma once

//OLE��ͷ�ļ�
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CApplication.h"
#include "DataPoint/DataPointManager.h"

///
///����OLE�ķ�ʽ��EXCEL��д��
class COperateExcelFile
{
public:
	static COperateExcelFile* Instance(void);
	static void Destroy(void);
	void SaveToExcel(const CString& strFilePath, CDataPointManager& pointmap);
	void ReadFromExcel(const CString& strFilePath, CDataPointManager& pointmap);
	bool ReadSampleDataFromExcel(CString strSamplePath,vector<vector<CString>>&vecSampleList,vector<vector<CString>>&vecInputList,vector<vector<CString>>&vecOutputList);
private:
	//���캯������������
	COperateExcelFile(void);
	virtual ~COperateExcelFile(void);
	static COperateExcelFile* s_pInstance;

protected:
	///�򿪵�EXCEL�ļ�����
	CString       open_excel_file_;

	///EXCEL BOOK���ϣ�������ļ�ʱ��
	CWorkbooks    excel_books_; 
	///��ǰʹ�õ�BOOK����ǰ������ļ�
	CWorkbook     excel_work_book_; 
	///EXCEL��sheets����
	CWorksheets   excel_sheets_; 
	///��ǰʹ��sheet
	CWorksheet    excel_work_sheet_; 
	///��ǰ�Ĳ�������
	CRange        excel_current_range_; 


	///�Ƿ��Ѿ�Ԥ������ĳ��sheet������
	BOOL          already_preload_;
	///Create the SAFEARRAY from the VARIANT ret.
	COleSafeArray ole_safe_array_;

protected:
	///EXCEL�Ľ���ʵ��
	static CApplication excel_application_;

private:
	///
	void ShowInExcel(BOOL bShow);

	///���һ��CELL�Ƿ����ַ���
	BOOL    IsCellString(long iRow, long iColumn);
	///���һ��CELL�Ƿ�����ֵ
	BOOL    IsCellInt(long iRow, long iColumn);

	///�õ�һ��CELL��String
	CString GetCellString(long iRow, long iColumn);
	///�õ�����
	int     GetCellInt(long iRow, long iColumn);
	///�õ�double������
	double  GetCellDouble(long iRow, long iColumn);

	///ȡ���е�����
	int GetRowCount(void);
	///ȡ���е�����
	int GetColumnCount(void);

	///ͨ��idʹ��ĳ��sheet
	BOOL LoadSheet(long table_index,BOOL pre_load = FALSE);
	///ͨ������ʹ��ĳ��sheet��
	BOOL LoadSheet(LPCTSTR sheet,BOOL pre_load = FALSE);
	///ͨ�����ȡ��ĳ��Sheet������
	CString GetSheetName(long table_index);

	///�õ�Sheet������
	int GetSheetCount(void);

	///���ļ�
	BOOL OpenExcelFile(LPCTSTR file_name, BOOL bSave);
	///�رմ򿪵�Excel �ļ�����ʱ���EXCEL�ļ���Ҫ
	void CloseExcelFile(BOOL if_save = FALSE);
	//���Ϊһ��EXCEL�ļ�
	void SaveasXSLFile(const CString &xls_file);
	///ȡ�ô��ļ�������
	CString GetOpenFileName(void);
	///ȡ�ô�sheet������
	CString GetLoadSheetName(void);

	///д��һ��CELLһ��int
	void SetCellInt(long irow, long icolumn,int new_int);
	///д��һ��CELLһ��string
	void SetCellString(long irow, long icolumn,CString new_string);

	//
	void SetCellStringTitle(const long iRow, VARIANT vtData);
	void SetCellStringData(const long iRow, VARIANT vtData);

private:
	///��ʼ��EXCEL OLE
	static BOOL InitExcel(void);
	///�ͷ�EXCEL�� OLE
	static void ReleaseExcel(void);
	///ȡ���е����ƣ�����27->AA
	static char *GetColumnName(long iColumn);

	COleSafeArray GetMatrixValueTitle(wchar_t **pMatrix, const int nRow, const int nCol);
	COleSafeArray GetMatrixValueData(vector<vector<wstring>> vecData, const int nRow, const int nCol);


protected:
	//Ԥ�ȼ���
	void PreLoadSheet(void);
};