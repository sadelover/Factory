#pragma once

#include "DataPoint/DataPointEntry.h"
#include "DataPoint/DataPointManager.h"

class CExcelOperator
{
public:
	CExcelOperator(void);
	~CExcelOperator(void);
	static CExcelOperator& Instance();
	static void DestoryInstance();
	void SaveToExcel(CString sExcelFile, CDataPointManager& pointmap);
	void ReadFromExcel(CString sExcelFile, CDataPointManager& pointmap);
	CString GetExcelDriver();
private:
	static CExcelOperator* m_instance;
};

