#pragma once

#include "../ControlEx/SkinDlg.h"

#include "afxwin.h"
#include "gridctrl/GridCtrl.h"
#include "DataPoint/DataPointEntry.h"
#include "PointParamEditDlg.h"

class CDataPointManager;

// CPointParamEditDlg �Ի���
//enum DLG_TYPE
//{
//	enum_Add = 0,
//	enum_Edit,
//};

// CBatchPointsDlg �Ի���

class CBatchPointsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBatchPointsDlg)

public:
	CBatchPointsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBatchPointsDlg();

// �Ի�������
	enum { IDD = IDD_DATA_POINT_CONFIG_BATCH_POINTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddRow();
	void SetDlgType( DLG_TYPE eType );
	void SetPointMap(CDataPointManager* pPointMap);
	int  GetMaxKeyID();

	int m_nRowNumber;	//�б�����
	int ncolNum;		//�б�����
	int m_nReallNum;//��ʵ��������:

	vector<CString> m_TableOfName;		//������
	vector<CString> m_TableOfNotation;	//ע�ͱ�

	DLG_TYPE m_eType;
	CDataPointManager* m_pPointMap;
	CString m_strProperty;
	CComboBox m_combo_store_cycle;
//������
private:
	void InitGRID (void);
	CGridCtrl m_Grid;
	
};
