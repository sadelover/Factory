#pragma once

#include "../ControlEx/SkinDlg.h"

#include "afxwin.h"
#include "gridctrl/GridCtrl.h"
#include "DataPoint/DataPointEntry.h"
#include "PointParamEditDlg.h"

class CDataPointManager;

// CPointParamEditDlg 对话框
//enum DLG_TYPE
//{
//	enum_Add = 0,
//	enum_Edit,
//};

// CBatchPointsDlg 对话框

class CBatchPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBatchPointsDlg)

public:
	CBatchPointsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBatchPointsDlg();

// 对话框数据
	enum { IDD = IDD_DATA_POINT_CONFIG_BATCH_POINTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddRow();
	void SetDlgType( DLG_TYPE eType );
	void SetPointMap(CDataPointManager* pPointMap);
	int  GetMaxKeyID();

	int m_nRowNumber;	//列表行数
	int ncolNum;		//列表列数
	int m_nReallNum;//真实点名数量:

	vector<CString> m_TableOfName;		//点名表
	vector<CString> m_TableOfNotation;	//注释表

	DLG_TYPE m_eType;
	CDataPointManager* m_pPointMap;
	CString m_strProperty;
	CComboBox m_combo_store_cycle;
//表格参数
private:
	void InitGRID (void);
	CGridCtrl m_Grid;
	
};
